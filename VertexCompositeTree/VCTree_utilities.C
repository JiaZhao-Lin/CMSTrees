#include "./common/VertexCompositeTree.h"
#include "./common/constants.h"

//# Global Variables #####################################################################
TF1 *funTunedPtRes;
TF1 *funRawPtRes;
TF1 *funPtMeanShift;
const std::vector<int> HLT_TrigIdx = {0, 8, 10, 13};
const int nTrig = HLT_TrigIdx.size();
const int L1_TrigIdx = 0;


//# Histograms ###########################################################################
TH1D *hnEvts;

//* for tracks distributions
TH2D *hNtrkofflinevsNtrkHP;

//* for paired dimuon Reconstruction Efficiency
TH3D *hPtvsEtavsPhi_gen;
TH3D *hPtvsRapvsPhi_gen;
TH3D *hPtvsEtavsPhi_gen_reconstructed;
TH3D *hPtvsRapvsPhi_gen_reconstructed;

//* for single muon reconstruction efficiency
TH3D *hPtvsEtavsPhi_gen_mu_pos;
TH3D *hPtvsEtavsPhi_gen_mu_neg;
TH3D *hPtvsEtavsPhi_gen_mu_pos_reconstructed;
TH3D *hPtvsEtavsPhi_gen_mu_neg_reconstructed;

//* for single muon in pair reconstruction efficiency
TH3D *hPtvsEtavsPhi_gen_mu_pos_reconstructed_pair;
TH3D *hPtvsEtavsPhi_gen_mu_neg_reconstructed_pair;

//* for single muon L1 trigger efficiency: L1_SingleMuOpen_NotMBHF2AND
TH3D *hPtvsEtavsPhi_mu_pos;
TH3D *hPtvsEtavsPhi_mu_neg;
TH3D *hPtvsEtavsPhi_mu_pos_trigL1;
TH3D *hPtvsEtavsPhi_mu_neg_trigL1;

//* for single muon in pair L1 trigger efficiency: L1_SingleMuOpen_NotMBHF2AND
TH3D *hPtvsEtavsPhi_mu_pos_pair;
TH3D *hPtvsEtavsPhi_mu_neg_pair;
TH3D *hPtvsEtavsPhi_mu_pos_pair_trigL1;
TH3D *hPtvsEtavsPhi_mu_neg_pair_trigL1;

//* for HLT Trigger Efficiency
TH3D *hPtvsEtavsPhi;
TH3D *hPtvsRapvsPhi;
std::vector<TH3D*> vPtvsEtavsPhi_trigHLT(nTrig);
std::vector<TH3D*> vPtvsRapvsPhi_trigHLT(nTrig);
std::vector<TH1D*> vPt_trigHLT_Eff(nTrig);

//* for data anlysis
TH3D *hRapvsPtvsM_woEvtSel;
TH3D *hRapvsPtvsM_woEvtSel_woSmear;
TH3D *hRapvsPtvsM_woSmear;


//# Functions ############################################################################
void bookHistos()
{
	// event level
	hnEvts = new TH1D("hnEvts", "hnEvts;", 5, 0, 5);
	hnEvts->GetXaxis()->SetBinLabel(1, "trigEvt");
	hnEvts->GetXaxis()->SetBinLabel(2, "validVtx & !Beam-halo");
	hnEvts->GetXaxis()->SetBinLabel(3, "HFMaxE <= 7.6(7.3) GeV");
	hnEvts->GetXaxis()->SetBinLabel(4, "N_{trk}^{HP} == 2");
	hnEvts->GetXaxis()->SetBinLabel(5, "Dimuon Reconstructed");
	hnEvts->GetXaxis()->LabelsOption("d");
	hnEvts->GetXaxis()->SetLabelSize(0.04);

	// track level
	hNtrkofflinevsNtrkHP = new TH2D("hNtrkofflinevsNtrkHP", "hNtrkofflinevsNtrkHP; N_{trk}^{offline}; N_{trk}^{HP}", 5, 0, 5, 5, 0, 5);
	for (int i = 0; i < 5; ++i)
	{
		hNtrkofflinevsNtrkHP->GetXaxis()->SetBinLabel(i + 1, Form("%d", i));
		hNtrkofflinevsNtrkHP->GetYaxis()->SetBinLabel(i + 1, Form("%d", i));
	}

	const int nBins_Pt  = 50;
	const double BinsLow_Pt  = 0;
	const double BinsHigh_Pt = 5;

	const int nBins_Eta = 10;
	const double BinsLow_Eta = -2.6;
	const double BinsHigh_Eta = 2.6;
	
	const int nBins_Phi = 30;
	const double BinsLow_Phi = -PI;
	const double BinsHigh_Phi = PI;
	
	const int nBins_Rap = 60;
	const double BinsLow_Rap = -3;
	const double BinsHigh_Rap = 3;

	//* for paired dimuon Reconstruction Efficiency
	hPtvsEtavsPhi_gen = new TH3D("hPtvsEtavsPhi_gen",	"hPtvsEtavsPhi_gen;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsRapvsPhi_gen = new TH3D("hPtvsRapvsPhi_gen",	"hPtvsRapvsPhi_gen;	p_{T} (GeV/c);	y;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Rap, BinsLow_Rap, BinsHigh_Rap, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_gen_reconstructed = new TH3D("hPtvsEtavsPhi_gen_reconstructed",	"hPtvsEtavsPhi_gen_reconstructed;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsRapvsPhi_gen_reconstructed = new TH3D("hPtvsRapvsPhi_gen_reconstructed",	"hPtvsRapvsPhi_gen_reconstructed;	p_{T} (GeV/c);	y;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Rap, BinsLow_Rap, BinsHigh_Rap, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);

	//* for single muon reconstruction efficiency
	hPtvsEtavsPhi_gen_mu_pos = new TH3D("hPtvsEtavsPhi_gen_mu_pos",	"hPtvsEtavsPhi_gen_mu_pos;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_gen_mu_neg = new TH3D("hPtvsEtavsPhi_gen_mu_neg",	"hPtvsEtavsPhi_gen_mu_neg;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_gen_mu_pos_reconstructed = new TH3D("hPtvsEtavsPhi_gen_mu_pos_reconstructed",	"hPtvsEtavsPhi_gen_mu_pos_reconstructed;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_gen_mu_neg_reconstructed = new TH3D("hPtvsEtavsPhi_gen_mu_neg_reconstructed",	"hPtvsEtavsPhi_gen_mu_neg_reconstructed;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);

	//* for single muon in pair reconstruction efficiency
	hPtvsEtavsPhi_gen_mu_pos_reconstructed_pair = new TH3D("hPtvsEtavsPhi_gen_mu_pos_reconstructed_pair",	"hPtvsEtavsPhi_gen_mu_pos_reconstructed_pair;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_gen_mu_neg_reconstructed_pair = new TH3D("hPtvsEtavsPhi_gen_mu_neg_reconstructed_pair",	"hPtvsEtavsPhi_gen_mu_neg_reconstructed_pair;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);

	//* for single muon L1 trigger efficiency: L1_SingleMuOpen_NotMBHF2AND
	hPtvsEtavsPhi_mu_pos = new TH3D("hPtvsEtavsPhi_mu_pos",	"hPtvsEtavsPhi_mu_pos;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_mu_neg = new TH3D("hPtvsEtavsPhi_mu_neg",	"hPtvsEtavsPhi_mu_neg;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_mu_pos_trigL1 = new TH3D("hPtvsEtavsPhi_mu_pos_trigL1",	"hPtvsEtavsPhi_mu_pos_trigL1;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,  nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_mu_neg_trigL1 = new TH3D("hPtvsEtavsPhi_mu_neg_trigL1",	"hPtvsEtavsPhi_mu_neg_trigL1;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,  nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);

	//* for single muon in pair L1 trigger efficiency: L1_SingleMuOpen_NotMBHF2AND
	hPtvsEtavsPhi_mu_pos_pair = new TH3D("hPtvsEtavsPhi_mu_pos_pair",	"hPtvsEtavsPhi_mu_pos_pair;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_mu_neg_pair = new TH3D("hPtvsEtavsPhi_mu_neg_pair",	"hPtvsEtavsPhi_mu_neg_pair;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_mu_pos_pair_trigL1 = new TH3D("hPtvsEtavsPhi_mu_pos_pair_trigL1",	"hPtvsEtavsPhi_mu_pos_pair_trigL1;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,  nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_mu_neg_pair_trigL1 = new TH3D("hPtvsEtavsPhi_mu_neg_pair_trigL1",	"hPtvsEtavsPhi_mu_neg_pair_trigL1;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,  nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);

	//* for HLT Trigger Efficiency
	hPtvsEtavsPhi    = new TH3D("hPtvsEtavsPhi",	"hPtvsEtavsPhi;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsRapvsPhi    = new TH3D("hPtvsRapvsPhi",	"hPtvsRapvsPhi;	p_{T} (GeV/c);	y;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Rap, BinsLow_Rap, BinsHigh_Rap, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);

	for (int iTrig = 0; iTrig < nTrig; ++iTrig)
	{
		vPtvsEtavsPhi_trigHLT[iTrig] = new TH3D(Form("hPtvsEtavsPhi_trigHLT%d", iTrig), Form("hPtvsEtavsPhi_trigHLT%d;	p_{T} (GeV/c);	#eta;	#phi", iTrig), nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
		vPtvsRapvsPhi_trigHLT[iTrig] = new TH3D(Form("hPtvsRapvsPhi_trigHLT%d", iTrig), Form("hPtvsRapvsPhi_trigHLT%d;	p_{T} (GeV/c);	y;	#phi", iTrig), nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Rap, BinsLow_Rap, BinsHigh_Rap, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	}

	//* for data anlysis
	hRapvsPtvsM_woEvtSel = new TH3D("hRapvsPtvsM_woEvtSel", "hRapvsPtvsM_woEvtSel; p_{T} (GeV/c); y; M (GeV/c^{2})", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Rap, BinsLow_Rap, BinsHigh_Rap, 100, 0, 10);
	hRapvsPtvsM_woEvtSel_woSmear = new TH3D("hRapvsPtvsM_woEvtSel_woSmear", "hRapvsPtvsM_woEvtSel_woSmear; p_{T} (GeV/c); y; M (GeV/c^{2})", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Rap, BinsLow_Rap, BinsHigh_Rap, 100, 0, 10);
	hRapvsPtvsM_woSmear = new TH3D("hRapvsPtvsM_woSmear", "hRapvsPtvsM_woSmear; p_{T} (GeV/c); y; M (GeV/c^{2})", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Rap, BinsLow_Rap, BinsHigh_Rap, 100, 0, 10);

}

//= Calculations =========================================================================
TH1D *cal_eff(TH1D *hPass, TH1D *hTotal)
{
	TH1D *hEff = (TH1D*)hPass->Clone();
	hEff->SetTitle(Form("%s_eff; %s; Efficiency", hPass->GetName(), hPass->GetXaxis()->GetTitle()));
	hEff->Divide(hPass, hTotal, 1, 1, "B");
	return hEff;
}

TH2D *cal_eff2D(TH2D *hPass, TH2D *hTotal)
{
	TH2D *hEff = (TH2D*)hPass->Clone();
	hEff->SetTitle(Form("%s_eff; %s; %s", hPass->GetName(), hPass->GetXaxis()->GetTitle(), hPass->GetYaxis()->GetTitle()));
	hEff->Divide(hPass, hTotal, 1, 1, "B");
	return hEff;
}

Double_t trigAcc(Double_t *x, Double_t *par)
{
	//? For a given eta, return the pT threshold for a muon to be triggered
	//? Find the segment in which the eta falls, and then interpolate the pT threshold

	par = NULL;

	const Int_t nPts = 14;
	Double_t mEta[nPts] = {-2.4, -2.1, -1.65, -1.45, -1.1, -0.3, -0.3,  0.3, 0.3, 1.1, 1.45, 1.65, 2.1, 2.4};
	Double_t mPt[nPts]  = { 1.2,  1.2,  2.15,  2.15,  3.3,  3.3, 3.45, 3.45, 3.3, 3.3, 2.15, 2.15, 1.2, 1.2};

	Int_t iseg = -1;
	for(Int_t i=0; i<nPts-1; i++){
		if(x[0]>=mEta[i] && x[0]<mEta[i+1]){
			iseg = i;
			break;
		}
	}

	if(x[0]==mEta[nPts-1]) iseg = nPts - 2;

	if(iseg<0) return 999999.;

	Double_t mSlope = (mPt[iseg+1] - mPt[iseg]) / (mEta[iseg+1] - mEta[iseg]);
	Double_t mPtTh  = mSlope * (x[0] - mEta[iseg]) + mPt[iseg];

	return mPtTh;
}
TF1 *fTrigAcc = new TF1("fTrigAcc", trigAcc, -2.5, 2.5, 0);

Double_t trkAcc(Double_t *x, Double_t *par)
{
	//? For a given eta, return the pT threshold for a muon track acceptance
	//? Find the segment in which the eta falls, and then interpolate the pT threshold

	par = NULL;

	const Int_t nPts = 10;
	Double_t mEta[nPts] = {-2.4, -1.7,  -1.3, -1.3, -1.0, 1.0, 1.3,  1.3, 1.7, 2.4};
	Double_t mPt[nPts]  = { 1.0,  1.0,  1.53,  2.1,  3.3, 3.3, 2.1, 1.53, 1.0, 1.0};

	Int_t iseg = -1;
	for(Int_t i=0; i<nPts-1; i++)
	{
		if(x[0]>=mEta[i] && x[0]<mEta[i+1])
		{
			iseg = i;
			break;
		}
	}

	if( x[0]==mEta[nPts-1] ) iseg = nPts - 2;

	if(iseg<0) return 999999.;

	Double_t mSlope = (mPt[iseg+1] - mPt[iseg]) / (mEta[iseg+1] - mEta[iseg]);
	Double_t mPtTh  = mSlope * (x[0] - mEta[iseg]) + mPt[iseg];

	return mPtTh;
}

TF1 *fTrkAcc = new TF1("fTrkAcc", trkAcc, -2.5, 2.5, 0);

void init_smear_input()
{
	const double mPar0       = 0.0145;
	// smear input
	TFile *fRawPtRes = TFile::Open("./inFiles/rawPtRes.root");
	funPtMeanShift   = (TF1 *)fRawPtRes->Get("funPtMeanShift");
	funRawPtRes      = (TF1 *)fRawPtRes->Get("funRawPtRes");

	funTunedPtRes    = new TF1("funTunedPtRes", "sqrt([0]*[0]/x/x+[1]*[1])", 0, 5);
	funTunedPtRes    ->SetParameters(mPar0, funRawPtRes->GetParameter(1));
}

std::pair<Double_t, Double_t> smear_pt(Double_t posPt, Double_t posPt_gen, Double_t negPt, Double_t negPt_gen)
{
	//? Smear the input pT according to the resolution function
	//? The resolution function is tuned to reproduce the pT resolution in the data

	// shift mean
	Double_t shiftPosPt = posPt - funPtMeanShift->Eval(posPt_gen)*posPt_gen;
	Double_t shiftNegPt = negPt - funPtMeanShift->Eval(negPt_gen)*negPt_gen;

	// smear width
	Double_t posPtNew = posPt_gen + (shiftPosPt - posPt_gen)/funRawPtRes->Eval(posPt_gen)*funTunedPtRes->Eval(posPt_gen);
	Double_t negPtNew = negPt_gen + (shiftNegPt - negPt_gen)/funRawPtRes->Eval(negPt_gen)*funTunedPtRes->Eval(negPt_gen);

	return {posPtNew, negPtNew};
}


//= Drawings =============================================================================
void draw_latex(double latex_x, double latex_y, TString latex_text)
{
	TLatex *latex = new TLatex();
	latex->SetNDC();
	latex->SetTextFont(42);
	latex->SetTextSize(0.04);
	latex->SetTextAlign(31);
	latex->DrawLatex(latex_x, latex_y, Form("%s", latex_text.Data()));
}

void draw_header_Run3()
{
	TLatex *latex = new TLatex();
	latex->SetNDC();
	latex->SetTextFont(42);
	latex->SetTextSize(0.04);
	latex->DrawLatex(0.10, 0.92, "#bf{CMS} #it{Simulation}");
	latex->DrawLatex(0.63, 0.92, "PbPb Run3 (5.36 TeV)");
}

std::pair<TCanvas*, TLine*> get_eff_common(TH1D *hEff)
{
	//* Draw the frame
	auto c = new TCanvas();
	c->SetCanvasSize(800, 600);
	//stat box and disable title
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);

	//* Draw a line at y = 1
	TLine *line = new TLine(hEff->GetXaxis()->GetXmin(), 1, hEff->GetXaxis()->GetXmax(), 1);
	line->SetLineColor(kRed);
	line->SetLineStyle(2);
	line->SetLineWidth(2);

	return {c, line};
}

void draw_EvtSel()
{
	//* Get the canvas and line
	auto c = new TCanvas();
	c->SetCanvasSize(800, 600);
	//stat box and disable title
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);

	hnEvts->Draw();
	draw_latex(0.89, 0.20, Form("Total Evt: %.0f", hnEvts->GetBinContent(5)));
	c->SaveAs(Form("./outFigures/VCTree_EvtSel.pdf"));

	delete c;
}

std::vector<TH1D*> pipeline_draw_HLT_TrigEff(std::vector<TH3D*> vh3Num, TH3D * h3Den, TString project, TString name)
{
	//* Project the 3D histogram to 1D histogram and calculate the efficiency
	std::vector<TH1D*> vh1Eff;
	auto h1Den = (TH1D*)h3Den->Project3D(project);

	for (int iTrig = 0; iTrig < nTrig; ++iTrig)
	{
		auto h1Num = (TH1D*) vh3Num[iTrig]->Project3D(project);
		vh1Eff.push_back(cal_eff(h1Num, h1Den));
	}

	//* Get the canvas and line
	TCanvas *c; TLine *line;
	std::tie(c, line) = get_eff_common(vh1Eff[0]);

	//* Draw the legend
	auto legend = new TLegend(0.3, 0.6, 0.4, 0.8);
	legend->SetBorderSize(0);
	legend->SetFillStyle(0);
	// legend->SetTextFont(42);
	legend->SetTextSize(0.02);

	//* Draw the efficiency for each trigger
	for (int iTrig = 0; iTrig < nTrig; ++iTrig)
	{
		vh1Eff[iTrig]->GetYaxis()->SetRangeUser(0, 1.1);
		vh1Eff[iTrig]->SetLineColor(iTrig + 1);
		vh1Eff[iTrig]->Draw("same");

		//* Draw the legend
		legend->AddEntry(vh1Eff[iTrig], Form("%s", HLT_TRIG_LIST[HLT_TrigIdx[iTrig]].Data()), "l");
	}

	legend->Draw("same");
	line->Draw("same");
	draw_header_Run3();

	c->SaveAs(Form("./outFigures/Eff_HLT_Trig_%s.pdf", project.Data()));

	delete c; delete line;
	delete legend;
	return vh1Eff;
}

void pipeline_draw_RecoEff(TH1D *hPass, TH1D *hTotal, TString name)
{
	//* Calculate the efficiency
	auto hEff = cal_eff(hPass, hTotal);

	//* Get the canvas and line
	TCanvas *c; TLine *line;
	std::tie(c, line) = get_eff_common(hEff);

	//* Draw the efficiency
	//set range
	hEff->GetYaxis()->SetRangeUser(0, 1.1);
	hEff->Draw();
	line->Draw("same");
	draw_header_Run3();
	
	c->SaveAs(Form("./outFigures/Eff_Reco.pdf"));

	delete c; delete line;
}

void pipeline_draw_RecoEff_SingleMu(TH3D *h3Pass, TH3D *h3Total, TString project, TString name = "./outFigures/Eff_Reco_SingleMu.pdf")
{
	//* Project the 3D histogram to  histogram and calculate the efficiency
	if (project.Length() == 1)
	{
		auto hPass = (TH1D*)h3Pass->Project3D(project);
		auto hTotal = (TH1D*)h3Total->Project3D(project);
		
		//* Calculate the efficiency
		auto hEff = cal_eff(hPass, hTotal);

		//* Get the canvas and line
		TCanvas *c; TLine *line;
		std::tie(c, line) = get_eff_common(hEff);

		//* Draw the efficiency
		hEff->GetYaxis()->SetRangeUser(0, 1.1);
		hEff->Draw();
		line->Draw("same");
		draw_header_Run3();

		c->SaveAs(name.Data());

		delete c; delete line;
	}
	else if (project.Length() == 2)
	{
		auto hPass = (TH2D*)h3Pass->Project3D(project);
		auto hTotal = (TH2D*)h3Total->Project3D(project);
		
		//* Calculate the efficiency
		auto hEff = cal_eff2D(hPass, hTotal);

		//* Get the canvas and line
		TCanvas *c; TLine *line;
		std::tie(c, line) = get_eff_common(hEff->ProjectionX());

		//* Draw the efficiency
		//set range
		hEff->Draw("colz");
		// hEff->GetZaxis()->SetRangeUser(0, 1.1);
		// line->Draw("same");
		draw_header_Run3();

		c->SaveAs(name.Data());

		delete c; delete line;
	}
	else
	{
		std::cout << "Error: project.Length() = " << project.Length() << std::endl;
	}
}

void pipeline_draw_L1_TrigEff(TH3D *h3Pass, TH3D *h3Total, TString project, TString name = "./outFigures/Eff_L1_Trig.pdf")
{
	//* Project the 3D histogram to 1D histogram and calculate the efficiency
	auto hPass = (TH1D*)h3Pass->Project3D(project);
	auto hTotal = (TH1D*)h3Total->Project3D(project);
	// auto hPass = (TH1D*)h3Pass->ProjectionX("", 1,2);
	// auto hTotal = (TH1D*)h3Total->ProjectionX("", 1,2);

	
	//* Calculate the efficiency
	auto hEff = cal_eff(hPass, hTotal);

	//* Get the canvas and line
	TCanvas *c; TLine *line;
	std::tie(c, line) = get_eff_common(hEff);

	//* Draw the efficiency
	hEff->GetYaxis()->SetRangeUser(0, 1.1);
	hEff->Draw();
	line->Draw("same");
	draw_header_Run3();

	c->SaveAs(name.Data());

	delete c; delete line;
}

void pipeline_draw_L1_TrigEff_inEtaBins(TH3D *h3Pass, TH3D *h3Total, TString project, TString name = "./outFigures/Eff_L1_Trig.pdf")
{
	//? Project the 3D histogram to 1D histogram and calculate the efficiency
	//? The 3D histogram is projected to 1D histogram in eta bins

	//* Calculate the efficiency
	std::vector<TH1D*> vh1Eff;
	int nBins_Eta = h3Pass->GetNbinsY();
	for (int iEta = 0; iEta < nBins_Eta; ++iEta)
	{
		// auto hPass = (TH1D*)h3Pass->ProjectionX("", iEta + 1, iEta + 1);
		// auto hTotal = (TH1D*)h3Total->ProjectionX("", iEta + 1, iEta + 1);
		auto h2Pass = (TH2D*)h3Pass->Project3D("yx");
		auto h2Total = (TH2D*)h3Total->Project3D("yx");
		auto hPass = (TH1D*)h2Pass->ProjectionX("", iEta + 1, iEta + 1);
		auto hTotal = (TH1D*)h2Total->ProjectionX("", iEta + 1, iEta + 1);
		vh1Eff.push_back(cal_eff(hPass, hTotal));
	}

	//* Get the canvas, and divide it according to the number of eta bins with 3 columns
	auto c = new TCanvas();
	c->SetCanvasSize(800, 600);
	// int nrows = (nBins_Eta / 3) if (nBins_Eta % 3 == 0) else nBins_Eta / 3 + 1;
	int nrows = (nBins_Eta % 3 == 0) ? nBins_Eta / 3 : nBins_Eta / 3 + 1;
	c->Divide(3, nrows);

	//* Draw the efficiency for each eta bin
	for (int iEta = 0; iEta < nBins_Eta; ++iEta)
	{
		c->cd(iEta + 1);
		vh1Eff[iEta]->GetYaxis()->SetRangeUser(0, 1.1);
		vh1Eff[iEta]->Draw();
		vh1Eff[iEta]->SetMarkerStyle(20);
	}

	draw_header_Run3();
	
	c->SaveAs(name.Data());

	delete c;
}

void draw_Ntrk_distribution()
{
	auto c = new TCanvas("c", "", 800, 600);
	//stat box and disable title
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);

	auto hNtrkoffline = (TH1D*)hNtrkofflinevsNtrkHP->ProjectionX();
	auto hNtrkHP = (TH1D*)hNtrkofflinevsNtrkHP->ProjectionY();

	hNtrkHP->SetLineColor(kBlue);
	hNtrkHP->Draw();

	hNtrkoffline->SetLineColor(kRed);
	hNtrkoffline->Draw("same");

	draw_header_Run3();

	//* Draw the legend
	auto legend = new TLegend(0.7, 0.6, 0.9, 0.75);
	legend->SetBorderSize(0);
	legend->SetFillStyle(0);
	// legend->SetTextFont(42);
	legend->SetTextSize(0.04);
	legend->AddEntry(hNtrkoffline, "N_{trk}^{offline}", "l");
	legend->AddEntry(hNtrkHP, "N_{trk}^{HP}", "l");
	legend->Draw("same");

	//* Draw latex fraction of tracks
	auto nTrkoffline_entries = hNtrkoffline->GetEntries();
	auto nTrkoffline_frac_0 = hNtrkoffline->GetBinContent(1) / nTrkoffline_entries;
	auto nTrkoffline_frac_1 = hNtrkoffline->GetBinContent(2) / nTrkoffline_entries;
	auto nTrkoffline_frac_2 = hNtrkoffline->GetBinContent(3) / nTrkoffline_entries;
	auto nTrkoffline_frac_3 = hNtrkoffline->GetBinContent(4) / nTrkoffline_entries;

	auto nTrkHP_entries = hNtrkHP->GetEntries();
	auto nTrkHP_frac_0 = hNtrkHP->GetBinContent(1) / nTrkHP_entries;
	auto nTrkHP_frac_1 = hNtrkHP->GetBinContent(2) / nTrkHP_entries;
	auto nTrkHP_frac_2 = hNtrkHP->GetBinContent(3) / nTrkHP_entries;
	auto nTrkHP_frac_3 = hNtrkHP->GetBinContent(4) / nTrkHP_entries;

	draw_latex(0.80, 0.40, Form("Frac: N_{trk}^{offline} = 0: %.2f,  N_{trk}^{HP} = 0: %.2f", nTrkoffline_frac_0, nTrkHP_frac_0));
	draw_latex(0.80, 0.34, Form("Frac: N_{trk}^{offline} = 1: %.2f,  N_{trk}^{HP} = 1: %.2f", nTrkoffline_frac_1, nTrkHP_frac_1));
	draw_latex(0.80, 0.28, Form("Frac: N_{trk}^{offline} = 2: %.2f,  N_{trk}^{HP} = 2: %.2f", nTrkoffline_frac_2, nTrkHP_frac_2));
	draw_latex(0.80, 0.22, Form("Frac: N_{trk}^{offline} = 3: %.2f,  N_{trk}^{HP} = 3: %.2f", nTrkoffline_frac_3, nTrkHP_frac_3));
	
	c->SaveAs("./outFigures/Ntrkoffline.pdf");

	delete c;
}


//= Utilities ============================================================================
bool RecGenMatched(TVector3 Mom_rec, TVector3 Mom_gen, double BestDeltaR)
{
	// check if the reconstructed particle is matched to the generated one
	// DeltaPt/genPt < 0.15 ensures to keep 99.9842% events
	// DeltaR < 0.05 ensures to keep 99.9763% events
	// DeltaR < BestDeltaR ensures to keep only the best match
	double DeltaR	= Mom_gen.DeltaR(Mom_rec);
	double PtRes	= TMath::Abs((Mom_rec.Pt()-Mom_gen.Pt())/Mom_gen.Pt());

	return (PtRes < 0.15) && (DeltaR < 0.05) && (DeltaR < BestDeltaR);
}

const std::vector<TLorentzVector> GetFourMom(Short_t chargeD1, Short_t chargeD2, Float_t pTD1, Float_t pTD2, Float_t EtaD1, Float_t EtaD2, Float_t PhiD1, Float_t PhiD2)
{
	TLorentzVector posFourMom, negFourMom, pairFourMom;
	posFourMom.SetPtEtaPhiM(chargeD1 > 0 ? pTD1 : pTD2, chargeD1 > 0 ? EtaD1 : EtaD2, chargeD1 > 0 ? PhiD1 : PhiD2, MASS_MUON);
	negFourMom.SetPtEtaPhiM(chargeD1 < 0 ? pTD1 : pTD2, chargeD1 < 0 ? EtaD1 : EtaD2, chargeD1 < 0 ? PhiD1 : PhiD2, MASS_MUON);
	pairFourMom = posFourMom + negFourMom;
	return {posFourMom, negFourMom, pairFourMom};
}

Bool_t goodMuPair(VertexCompositeTree& evtTree, const int icand, const int trigIdx)
{
	Double_t mTrkPtTh1  = fTrkAcc  ->Eval( evtTree.EtaD1()[icand] );
	Double_t mTrkPtTh2  = fTrkAcc  ->Eval( evtTree.EtaD2()[icand] );
	Double_t mTrigPtTh1 = fTrigAcc ->Eval( evtTree.EtaD1()[icand] );
	Double_t mTrigPtTh2 = fTrigAcc ->Eval( evtTree.EtaD2()[icand] );

	if(evtTree.pTD1()[icand] < mTrkPtTh1 || evtTree.pTD2()[icand] < mTrkPtTh2) return kFALSE;
	// cout<<bool(evtTree.trigMuon1()[trigIdx][icand])<<endl;

	Bool_t isTrigAcc1 = kFALSE, isTrigAcc2 = kFALSE;
	if(evtTree.trigMuon1()[trigIdx][icand] && evtTree.pTD1()[icand] >= mTrigPtTh1) isTrigAcc1 = kTRUE;
	if(evtTree.trigMuon2()[trigIdx][icand] && evtTree.pTD2()[icand] >= mTrigPtTh2) isTrigAcc2 = kTRUE;

	if(!isTrigAcc1 && !isTrigAcc2) return kFALSE;

	return kTRUE;
}

Bool_t goodMuPair(const TLorentzVector posFourMom, const Bool_t isPosTrig, const TLorentzVector negFourMom, const Bool_t isNegTrig)
{
	Double_t mPosTrkPtTh  = fTrkAcc  ->Eval( posFourMom.Eta() );
	Double_t mNegTrkPtTh  = fTrkAcc  ->Eval( negFourMom.Eta() );
	Double_t mPosTrigPtTh = fTrigAcc ->Eval( posFourMom.Eta() );
	Double_t mNegTrigPtTh = fTrigAcc ->Eval( negFourMom.Eta() );

	if(posFourMom.Pt() < mPosTrkPtTh || negFourMom.Pt() < mNegTrkPtTh) return kFALSE;

	Bool_t isPosTrigAcc = kFALSE, isNegTrigAcc = kFALSE;
	if(isPosTrig && posFourMom.Pt() >= mPosTrigPtTh) isPosTrigAcc = kTRUE;
	if(isNegTrig && negFourMom.Pt() >= mNegTrigPtTh) isNegTrigAcc = kTRUE;

	if(!isPosTrigAcc && !isNegTrigAcc) return kFALSE;

	return kTRUE;
}

void update_progress(int ientry, int total_entries, int percentage_increment)
{
	if (ientry % (total_entries / percentage_increment) == 0)
	{
		std::cout << "Processing " << ientry << "th entry... (" << (int)((double)ientry / total_entries * 100) << "%)" << std::endl;
	}
}