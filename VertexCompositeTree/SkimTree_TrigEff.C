#include "Selections.C"


//================================================================================================
//== Global Variables
const std::vector<int> HLT_TrigIdx = {0, 8, 10, 13};
const int nTrig = HLT_TrigIdx.size();

TH1D *hnEvts;
TH3D *hPtvsEtavsPhi;
TH3D *hPtvsRapvsPhi;
std::vector<TH3D*> vPtvsEtavsPhi_trigHLT(nTrig);
std::vector<TH3D*> vPtvsRapvsPhi_trigHLT(nTrig);
std::vector<TH1D*> vPt_trigHLT_Eff(nTrig);


//== Functions ====================================================================================
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

	const int nBins_Pt  = 50;
	const double BinsLow_Pt  = 0.0;
	const double BinsHigh_Pt = 1;

	const int nBins_Eta = 40;
	const double BinsLow_Eta = -4;
	const double BinsHigh_Eta = 4;
	
	const int nBins_Phi = 30;
	const double BinsLow_Phi = -PI;
	const double BinsHigh_Phi = PI;
	
	const int nBins_Rap = 60;
	const double BinsLow_Rap = -3;
	const double BinsHigh_Rap = 3;

	hPtvsEtavsPhi    = new TH3D("hPtvsEtavsPhi",	"hPtvsEtavsPhi;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsRapvsPhi    = new TH3D("hPtvsRapvsPhi",	"hPtvsRapvsPhi;	p_{T} (GeV/c);	y;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Rap, BinsLow_Rap, BinsHigh_Rap, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	for (int iTrig = 0; iTrig < nTrig; ++iTrig)
	{
		vPtvsEtavsPhi_trigHLT[iTrig] = new TH3D(Form("hPtvsEtavsPhi_trigHLT%d", iTrig), Form("hPtvsEtavsPhi_trigHLT%d;	p_{T} (GeV/c);	#eta;	#phi", iTrig), nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
		vPtvsRapvsPhi_trigHLT[iTrig] = new TH3D(Form("hPtvsRapvsPhi_trigHLT%d", iTrig), Form("hPtvsRapvsPhi_trigHLT%d;	p_{T} (GeV/c);	y;	#phi", iTrig), nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Rap, BinsLow_Rap, BinsHigh_Rap, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	}

}

TH1D *cal_eff(TH1D *hPass, TH1D *hTotal)
{
	TH1D *hEff = (TH1D*)hPass->Clone();
	hEff->SetTitle(Form("%s_eff; %s; Efficiency", hPass->GetName(), hPass->GetXaxis()->GetTitle()));
	hEff->Divide(hPass, hTotal, 1, 1, "B");
	return hEff;
}

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

	//* Draw the frame
	auto c = new TCanvas("c", "", 800, 600);
	//stat box and disable title
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);

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

	//* Draw a line at y = 1
	TLine *line = new TLine(vh1Eff[0]->GetXaxis()->GetXmin(), 1, vh1Eff[0]->GetXaxis()->GetXmax(), 1);
	line->SetLineColor(kRed);
	line->SetLineStyle(2);
	line->SetLineWidth(2);
	line->Draw("same");

	draw_header_Run3();

	c->SaveAs(Form("./outFigures/Eff_HLT_Trig_%s.pdf", project.Data()));

	hnEvts->Draw();
	draw_latex(0.89, 0.20, Form("Total Evt: %.0f", hnEvts->GetBinContent(5)));
	c->SaveAs(Form("./outFigures/Eff_HLT_Trig_%s_nEvts.pdf", project.Data()));

	return vh1Eff;
}

//== Main =========================================================================================
void SkimTree_TrigEff()
{
	TH1::SetDefaultSumw2(kTRUE);
	const TString inputFileDir = "inFiles/VCTree_STARLIGHT_CohJpsiToMuMu_5p36TeV_Run3.root";
	TFile *inputFile = new TFile(inputFileDir, "READ");

	const TString TreeDir = "dimucontana_mc/VertexCompositeNtuple";

	// ===================================================================================
	// = Set up the TTreeReader and TTreeReaderValue objects.
	TTreeReader treeReader(TreeDir, inputFile);

	TTreeReaderValue<Int_t>					NtrkHP(treeReader,				"NtrkHP");
	TTreeReaderArray<Bool_t>				evtSel(treeReader,				"evtSel");
	
	TTreeReaderValue<UInt_t>				candSize_gen(treeReader,		"candSize_gen");
	TTreeReaderArray<Short_t>				chargeD1_gen(treeReader,		"chargeD1_gen");
	TTreeReaderArray<Short_t>				chargeD2_gen(treeReader,		"chargeD2_gen");
	TTreeReaderArray<Float_t>				pTD1_gen(treeReader,			"pTD1_gen");
	TTreeReaderArray<Float_t>				pTD2_gen(treeReader,			"pTD2_gen");
	TTreeReaderArray<Float_t>				EtaD1_gen(treeReader,			"EtaD1_gen");
	TTreeReaderArray<Float_t>				EtaD2_gen(treeReader,			"EtaD2_gen");
	TTreeReaderArray<Float_t>				PhiD1_gen(treeReader,			"PhiD1_gen");
	TTreeReaderArray<Float_t>				PhiD2_gen(treeReader,			"PhiD2_gen");

	TTreeReaderValue<UInt_t>				candSize(treeReader,		"candSize");
	TTreeReaderArray<Short_t>				chargeD1(treeReader,		"chargeD1");
	TTreeReaderArray<Short_t>				chargeD2(treeReader,		"chargeD2");
	TTreeReaderArray<Float_t>				pTD1(treeReader,			"pTD1");
	TTreeReaderArray<Float_t>				pTD2(treeReader,			"pTD2");
	TTreeReaderArray<Float_t>				EtaD1(treeReader,			"EtaD1");
	TTreeReaderArray<Float_t>				EtaD2(treeReader,			"EtaD2");
	TTreeReaderArray<Float_t>				PhiD1(treeReader,			"PhiD1");
	TTreeReaderArray<Float_t>				PhiD2(treeReader,			"PhiD2");

	TTreeReaderValue<UInt_t>				candSize_mu(treeReader,				"candSize_mu");
	TTreeReaderArray<Bool_t>				softMuon_mu(treeReader,				"softMuon_mu");
	TTreeReaderArray<Float_t>				pT_mu(treeReader,					"pT_mu");
	TTreeReaderArray<Float_t>				eta_mu(treeReader,					"eta_mu");
	TTreeReaderArray<Float_t>				phi_mu(treeReader,					"phi_mu");

	TTreeReaderValue<std::vector< std::vector<UChar_t> >>
											trigMuon_mu(treeReader,				"trigMuon_mu");
	TTreeReaderArray<Bool_t>				trigHLT(treeReader,					"trigHLT");
	// =======================================================================================


	bookHistos();

	int jentry = -1;
	while (treeReader.Next())
	{
		jentry++;
		if (jentry % (treeReader.GetEntries() / 10) == 0)
			cout << "begin " << jentry << "th entry...." << endl;

		hnEvts->Fill(0.5);

		//= Event Level Selections ============================================================
		// Select Event - require this event has a valid vertex and is not beam-halo event
		Bool_t goodVtx				= (evtSel[2] && evtSel[3]);

		//evtSel[4-15]
		//[4]=0: HFPlusMaxTower < 3 GeV;  [4]=1: HFPlusMaxTower > 3 GeV
		//[5]=0: HFMinusMaxTower < 3 GeV;  [5]=1: HFMinusMaxTower > 3 GeV
		//[6] is for Plus & [7] is for Minus; Threshold = 4 GeV
		//[8] is for Plus & [9] is for Minus; Threshold = 5 GeV
		//[10] is for Plus & [11] is for Minus; Threshold = 6 GeV
		//[12] is for Plus & [13] is for Minus; Threshold = 7 GeV
		//[14] is for Plus & [15] is for Minus; Threshold = 8 GeV
		//[16] is for Plus (Th = 7.6 GeV) & [17] is for Minus (Th = 7.3 GeV);
		Bool_t goodHFVeto 			= (!evtSel[16] && !evtSel[17]);
		Bool_t passNTrkHP 			= *NtrkHP == 2;
		Bool_t passEvtSel 			= goodVtx && goodHFVeto && passNTrkHP;
		Bool_t passNRecoCandidate 	= *candSize == 1;

		if (goodVtx)				hnEvts->Fill(1.5);
		if (goodVtx && goodHFVeto)	hnEvts->Fill(2.5);
		if (passEvtSel) 			hnEvts->Fill(3.5);
		if (passNRecoCandidate)		hnEvts->Fill(4.5);

		if (!passEvtSel) 			continue;
		if (!passNRecoCandidate) 	continue;

		//= Fill HLT Trig Eff Hist ===================================================
		for (UInt_t icand = 0; icand < *candSize; icand++)
		{
			//* Get the four-momentum of the pair
			Double_t posPt  = chargeD1[icand] > 0 ? pTD1[icand]  : pTD2[icand];
			Double_t posEta = chargeD1[icand] > 0 ? EtaD1[icand] : EtaD2[icand];
			Double_t posPhi = chargeD1[icand] > 0 ? PhiD1[icand] : PhiD2[icand];
			Double_t negPt  = chargeD1[icand] < 0 ? pTD1[icand]  : pTD2[icand];
			Double_t negEta = chargeD1[icand] < 0 ? EtaD1[icand] : EtaD2[icand];
			Double_t negPhi = chargeD1[icand] < 0 ? PhiD1[icand] : PhiD2[icand];

			TLorentzVector posFourMom, negFourMom, pairFourMom;
			posFourMom.SetPtEtaPhiM(posPt, posEta, posPhi, MASS_MUON);
			negFourMom.SetPtEtaPhiM(negPt, negEta, negPhi, MASS_MUON);
			pairFourMom = posFourMom + negFourMom;

			Double_t pt		= pairFourMom.Pt();
			Double_t eta	= pairFourMom.Eta();
			Double_t phi	= pairFourMom.Phi();
			Double_t mass	= pairFourMom.M();
			Double_t rap	= pairFourMom.Rapidity();

			//= Dimuon Selections ============================================================
			if (abs(rap) > 2.4) continue;

			//* fill denominator of HLT trig eff
			hPtvsEtavsPhi	->Fill(pt,	eta,	phi);
			hPtvsRapvsPhi	->Fill(pt,	rap,	phi);

			//* fill numerator of HLT trig eff for each trigger
			for (int iTrig = 0; iTrig < nTrig; ++iTrig)
			{
				if (trigHLT[HLT_TrigIdx[iTrig]])
				{
					vPtvsEtavsPhi_trigHLT[iTrig]->Fill(pt, eta, phi);
					vPtvsRapvsPhi_trigHLT[iTrig]->Fill(pt, rap, phi);
				}
			}
		}
	}

	//= Calculate HLT Trig Eff in Pt===================================================
	pipeline_draw_HLT_TrigEff(vPtvsRapvsPhi_trigHLT, hPtvsRapvsPhi, "x", "");
	pipeline_draw_HLT_TrigEff(vPtvsRapvsPhi_trigHLT, hPtvsRapvsPhi, "y", "");
	pipeline_draw_HLT_TrigEff(vPtvsRapvsPhi_trigHLT, hPtvsRapvsPhi, "z", "");
}