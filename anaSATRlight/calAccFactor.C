// #include "../common/headers.h"
#include "function.h"
// #include "../common/funUtil.h"

const int nDiffRapBins = 6;
const double mDiffRapBds[nDiffRapBins+2] = {-2.4, -2.1, -1.9, -1.6, 1.6, 1.9, 2.1, 2.4};

// even bins in rapidity from -2.4 to 2.4 with 0.1 width
// const int nDiffRapBins = 10;
// const double mDiffRapBds[nDiffRapBins+2] = {-2.4, -2.2, -2.0, -1.8, -1.6, 0,1.6, 1.8, 2.0, 2.2, 2.4};


const Double_t mTinyNum = 1.e-6;

int    mGenMarker = 20;
int    mGenColor  = 1;
int    mGenWidth  = 2;
Double_t mGenSize   = 0.8;
int    mFont      = 42;

// const int   nSpecs            = 9;
// const TString specName[nSpecs]  = {"CohJpsi", "CohJpsi_0n0n", "CohJpsi_0nXn", "CohJpsi_XnXn", 
// 	"InCohJpsi", "CohPsi2SFeeddown", "CohPsi2S", "InCohPsi2S", "LowMassGammaGamma"};
// const TString specTitle[nSpecs] = {"Coherent J/#psi", "Coherent J/#psi (0n0n)", "Coherent J/#psi (0nXn)", "Coherent J/#psi (XnXn)", 
// 	"Incoherent J/#psi", "Coherent #psi(2S) #rightarrow J/#psi + X", "Coherent #psi(2S)", "Incoherent #psi(2S)", "#gamma#gamma#rightarrow#mu#mu"};

const int   nSpecs            = 1;
const TString specName[nSpecs]  = {"CohPhi_200k"};
const TString specTitle[nSpecs] = {"Coherent #phi"};


TFile*infile[nSpecs];
TH3D *hMvsPtvsRap_Den[nSpecs];
TH3D *hMvsPtvsRap_Num[nSpecs];
TH1D *hRap_Den[nSpecs];
TH1D *hRap_Num[nSpecs];
TH1D *hRap_Den_Symm[nSpecs];
TH1D *hRap_Num_Symm[nSpecs];

TH1D *hAccvsRap[nSpecs];
TH1D *hAccvsRap_Symm[nSpecs];

//--------------------------------------
//--------------------------------------
void readFiles();
void getAcc();
void drawAcc();
void saveFiles();
//--------------------------------------
//--------------------------------------

TString inFileDir = "./outFiles/AccHist";
TString outFileDir = "./outFiles/AccFactor";

void calAccFactor()
{
	auto temp = "mkdir -p " + outFileDir;
	system(temp.Data());

	readFiles();
	getAcc();
	drawAcc();
	// saveFiles();
}

void readFiles()
{
	for(int is=0; is<nSpecs; is++)
	{		
		infile[is] = new TFile( Form("%s/%s.root", inFileDir.Data(), specName[is].Data()), "read" );
		if (!infile[is])
		{
			cout<<"Error: File "<<infile[is]->GetName()<<" does not exist!"<<endl;
			return;
		}

		cout<<"is: "<<specName[is].Data()<<endl;
		cout<<"readin: "<<infile[is]->GetName()<<endl;
	
		hMvsPtvsRap_Den[is]      = (TH3D *) infile[is]->Get( "hMvsPtvsRap_STARlight" );  // Denominator
		hMvsPtvsRap_Num[is]      = (TH3D *) infile[is]->Get( "hMvsPtvsRap_KaonFilter"  );  // Numerator

		const int massBinLow     = 0;
		const int massBinHi      = hMvsPtvsRap_Den[is]->GetNbinsZ();
		const int ptBinLow       = 0;
		const int ptBinHig       = hMvsPtvsRap_Num[is]->GetNbinsY();

		//Denominator
		hRap_Den[is]             = (TH1D *)hMvsPtvsRap_Den[is]        ->ProjectionX( Form("hRap_Den_%s",  specName[is].Data()), ptBinLow, ptBinHig, massBinLow, massBinHi );
		//Numerator
		hRap_Num[is]             = (TH1D*) hMvsPtvsRap_Num[is]        ->ProjectionX( Form("hRap_%s",      specName[is].Data()), ptBinLow, ptBinHig, massBinLow, massBinHi );

		//Rebin rapidity bins to be same as signal rapidity bins
		hRap_Den[is]             = (TH1D*) rebHisto( hRap_Den[is],    Form("hRap_Den_ispec%d",     is), nDiffRapBins+1, mDiffRapBds, "NO");
		hRap_Num[is]             = (TH1D*) rebHisto( hRap_Num[is],    Form("hRap_ispec%d",         is), nDiffRapBins+1, mDiffRapBds, "NO");

		hRap_Den_Symm[is]		 = (TH1D*) hRap_Den[is] 	->Clone( Form("hRap_Den_Symm_ispec%d",	is) );
		hRap_Num_Symm[is]		 = (TH1D*) hRap_Num[is]		->Clone( Form("hRap_Den_Symm_ispec%d",	is) );

		//For SymmetricRapBin, the positive rap side of the histogram is combined y 
		for (int i = nDiffRapBins/2 + 1; i < nDiffRapBins + 1; ++i)
		{
			hRap_Den_Symm[is]		->AddBinContent(i+1,	hRap_Den[is]	 ->GetBinContent(nDiffRapBins + 1 - i));
			hRap_Num_Symm[is] 		->AddBinContent(i+1,	hRap_Num[is]	 ->GetBinContent(nDiffRapBins + 1 - i));
		}
	}//ispec
}

void getAcc()
{
	for(int is=0; is<nSpecs; is++)
	{
		if( is!=0 && is!=1 && is!=2 && is!=3 && is!=6 ) continue;

		//calculate acceptance factors
		//------------------------------------------------------------------------------------------------------------------------------------------
		hAccvsRap[is]   = (TH1D *)hRap_Num[is]->Clone( Form("hAccvsRap_%s", specName[is].Data()) );
		hAccvsRap[is]   ->Divide( hRap_Num[is], hRap_Den[is], 1, 1, "B");
		hAccvsRap[is]   ->SetTitle( specTitle[is].Data() );
		hAccvsRap[is]   ->GetYaxis()->SetTitle("Acceptance");

		hAccvsRap_Symm[is]   = (TH1D *)hRap_Num_Symm[is]->Clone( Form("hAccvsRap_Symm_%s", specName[is].Data()) );
		hAccvsRap_Symm[is]   ->Divide( hRap_Num_Symm[is], hRap_Den_Symm[is], 1, 1, "B");
		hAccvsRap_Symm[is]   ->SetTitle( specTitle[is].Data() );
		hAccvsRap_Symm[is]   ->GetYaxis()->SetTitle("Acceptance");
	}//ispec
}

void drawAcc()
{

	TCanvas* c2 = new TCanvas("c2", "c2", 0, 0, 1200, 450);
	c2->Divide(2, 1);

	Double_t xPos = 0.15;
	Double_t yPos = 0.84;

	TLegend* leg1 = new TLegend(0.36, 0.45, 0.6, 0.65);
	leg1->SetFillStyle(0);
	leg1->SetTextSize(0.06);

	TLegend* leg2 = new TLegend(0.36, 0.45, 0.6, 0.65);
	leg1->SetFillStyle(0);
	leg1->SetTextSize(0.06);

	for(int is=0; is<nSpecs; is++)
	{
		// if( is!=0 && is!=1 && is!=2 && is!=3 && is!=6 ) continue;
		
		c2->cd(1);
		gPad->SetLogy(0);

		setHisto(hRap_Den[is],      20, 1, 1, 1, 2);
		setHisto(hRap_Num[is],      25, 1, 4, 4, 2);

		hRap_Den[is]      ->GetYaxis()->SetTitle("Entries");
		hRap_Den[is]      ->GetXaxis()->CenterTitle();
		hRap_Den[is]      ->SetMinimum(1);
		hRap_Den[is]      ->Draw("p");
		hRap_Num[is]      ->Draw("psame");

		if(is==0)
		{
			leg1->AddEntry( hRap_Den[is],      "GEN",            "pl");
			leg1->AddEntry( hRap_Num[is],      "GEN #times Acc.", "pl");
		}
		leg1->Draw("same");

		if(     specName[is].EqualTo("CohPsi2SFeeddown"))  drawLatex(0.27, 0.95, Form("%s", specTitle[is].Data()), mFont, 0.06, 1);
		else if(specName[is].EqualTo("LowMassGammaGamma")) drawLatex(0.43, 0.95, Form("%s", specTitle[is].Data()), mFont, 0.06, 1);
		else                                               drawLatex(0.38, 0.95, Form("%s", specTitle[is].Data()), mFont, 0.06, 1);

		c2->cd(2);
		gPad->SetLogy(0);
		setHisto(hAccvsRap[is],          25, 1, 4, 4, 2);

		hAccvsRap[is]          ->Draw("p");

		if(is==0)
		{
			leg2->AddEntry(hAccvsRap[is],      "Acc.", "pl");
		}
		leg2->Draw("same");

		if(     specName[is].EqualTo("CohPsi2SFeeddown") ) drawLatex(0.27, 0.95, Form("%s", specTitle[is].Data()), mFont, 0.06, 1);
		else if(specName[is].EqualTo("LowMassGammaGamma")) drawLatex(0.43, 0.95, Form("%s", specTitle[is].Data()), mFont, 0.06, 1);
		else                                               drawLatex(0.38, 0.95, Form("%s", specTitle[is].Data()), mFont, 0.06, 1);
		
		c2->SaveAs(Form("outFigures/AccvsRap_%s.pdf", specName[is].Data()));
		//----------------------------------------------------------------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------------------------------------------------------------
	}//is

	delete c2;
	delete leg1;
	delete leg2;

}



void saveFiles()
{

	TFile *fOut = new TFile(Form("%s/Acceptance_AllSpecs_%dRapBins.root", outFileDir.Data(), nDiffRapBins), "recreate");
	cout<<"save efficiencies into: "<<fOut->GetName()<<endl;
	fOut->cd();

	for(int is=0; is<nSpecs; is++)
	{
		if( is!=0 && is!=1 && is!=2 && is!=3 && is!=6 ) continue;

		hAccvsRap[is]          ->Write();
		hAccvsRap_Symm[is]     ->Write();
	}

	fOut->Close();
}


