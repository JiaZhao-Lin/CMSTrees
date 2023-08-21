#include "./common/VertexCompositeTree.h"
#include "./common/constants.h"

//================================================================================================
//== Global Variables
const std::vector<int> HLT_TrigIdx = {0, 8, 10, 13};
const int nTrig = HLT_TrigIdx.size();
const int L1_TrigIdx = 0;

TH1D *hnEvts;

//* for tracks distributions
TH2D *hNtrkofflinevsNtrkHP;

//* for single muon reconstruction efficiency
TH3D *hPtvsEtavsPhi_gen_mu_pos;
TH3D *hPtvsEtavsPhi_gen_mu_neg;
TH3D *hPtvsEtavsPhi_gen_mu_pos_reconstructed;
TH3D *hPtvsEtavsPhi_gen_mu_neg_reconstructed;

//* for single muon L1 trigger efficiency: L1_SingleMuOpen_NotMBHF2AND
TH3D *hPtvsEtavsPhi_mu_pos;
TH3D *hPtvsEtavsPhi_mu_neg;
TH3D *hPtvsEtavsPhi_mu_pos_trigL1;
TH3D *hPtvsEtavsPhi_mu_neg_trigL1;

//* for single muon L1 trigger efficiency within pair: L1_SingleMuOpen_NotMBHF2AND
TH3D *hPtvsEtavsPhi_mu_pos_pair;
TH3D *hPtvsEtavsPhi_mu_neg_pair;
TH3D *hPtvsEtavsPhi_mu_pos_trigL1_pair;
TH3D *hPtvsEtavsPhi_mu_neg_trigL1_pair;

//* for paired muon Reconstruction Efficiency
TH3D *hPtvsEtavsPhi_gen;
TH3D *hPtvsRapvsPhi_gen;
TH3D *hPtvsEtavsPhi_gen_reconstructed;
TH3D *hPtvsRapvsPhi_gen_reconstructed;

//* for HLT Trigger Efficiency
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

	const int nBins_Eta = 40;
	const double BinsLow_Eta = -2.6;
	const double BinsHigh_Eta = 2.6;
	
	const int nBins_Phi = 30;
	const double BinsLow_Phi = -PI;
	const double BinsHigh_Phi = PI;
	
	const int nBins_Rap = 60;
	const double BinsLow_Rap = -3;
	const double BinsHigh_Rap = 3;

	//* for paired muon Reconstruction Efficiency
	hPtvsEtavsPhi_gen = new TH3D("hPtvsEtavsPhi_gen",	"hPtvsEtavsPhi_gen;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsRapvsPhi_gen = new TH3D("hPtvsRapvsPhi_gen",	"hPtvsRapvsPhi_gen;	p_{T} (GeV/c);	y;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Rap, BinsLow_Rap, BinsHigh_Rap, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_gen_reconstructed = new TH3D("hPtvsEtavsPhi_gen_reconstructed",	"hPtvsEtavsPhi_gen_reconstructed;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsRapvsPhi_gen_reconstructed = new TH3D("hPtvsRapvsPhi_gen_reconstructed",	"hPtvsRapvsPhi_gen_reconstructed;	p_{T} (GeV/c);	y;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Rap, BinsLow_Rap, BinsHigh_Rap, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);

	//* for single muon reconstruction efficiency
	hPtvsEtavsPhi_gen_mu_pos = new TH3D("hPtvsEtavsPhi_gen_mu_pos",	"hPtvsEtavsPhi_gen_mu_pos;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_gen_mu_neg = new TH3D("hPtvsEtavsPhi_gen_mu_neg",	"hPtvsEtavsPhi_gen_mu_neg;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_gen_mu_pos_reconstructed = new TH3D("hPtvsEtavsPhi_gen_mu_pos_reconstructed",	"hPtvsEtavsPhi_gen_mu_pos_reconstructed;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_gen_mu_neg_reconstructed = new TH3D("hPtvsEtavsPhi_gen_mu_neg_reconstructed",	"hPtvsEtavsPhi_gen_mu_neg_reconstructed;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);

	//* for single muon L1 trigger efficiency: L1_SingleMuOpen_NotMBHF2AND
	hPtvsEtavsPhi_mu_pos = new TH3D("hPtvsEtavsPhi_mu_pos",	"hPtvsEtavsPhi_mu_pos;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_mu_neg = new TH3D("hPtvsEtavsPhi_mu_neg",	"hPtvsEtavsPhi_mu_neg;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt, nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_mu_pos_trigL1 = new TH3D("hPtvsEtavsPhi_mu_pos_trigL1",	"hPtvsEtavsPhi_mu_pos_trigL1;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,  nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_mu_neg_trigL1 = new TH3D("hPtvsEtavsPhi_mu_neg_trigL1",	"hPtvsEtavsPhi_mu_neg_trigL1;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,  nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);

	//* for single muon L1 trigger efficiency within pair: L1_SingleMuOpen_NotMBHF2AND
	hPtvsEtavsPhi_mu_pos_pair = new TH3D("hPtvsEtavsPhi_mu_pos_pair",	"hPtvsEtavsPhi_mu_pos_pair;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,  nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_mu_neg_pair = new TH3D("hPtvsEtavsPhi_mu_neg_pair",	"hPtvsEtavsPhi_mu_neg_pair;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,  nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_mu_pos_trigL1_pair = new TH3D("hPtvsEtavsPhi_mu_pos_trigL1_pair",	"hPtvsEtavsPhi_mu_pos_trigL1_pair;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,  nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);
	hPtvsEtavsPhi_mu_neg_trigL1_pair = new TH3D("hPtvsEtavsPhi_mu_neg_trigL1_pair",	"hPtvsEtavsPhi_mu_neg_trigL1_pair;	p_{T} (GeV/c);	#eta;	#phi", nBins_Pt, BinsLow_Pt, BinsHigh_Pt,  nBins_Eta, BinsLow_Eta, BinsHigh_Eta, nBins_Phi, BinsLow_Phi, BinsHigh_Phi);

	//* for HLT Trigger Efficiency
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

TH2D *cal_eff2D(TH2D *hPass, TH2D *hTotal)
{
	TH2D *hEff = (TH2D*)hPass->Clone();
	hEff->SetTitle(Form("%s_eff; %s; %s", hPass->GetName(), hPass->GetXaxis()->GetTitle(), hPass->GetYaxis()->GetTitle()));
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

void pipeline_draw_RecoEff_SingleMu(TH3D *h3Pass, TH3D *h3Total, TString project, TString name)
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

		c->SaveAs(Form("./outFigures/Eff_Reco_SingleMu_%s.pdf", project.Data()));

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

		c->SaveAs(Form("./outFigures/Eff2D_Reco_SingleMu_%s.pdf", project.Data()));

		delete c; delete line;
	}
	else
	{
		std::cout << "Error: project.Length() = " << project.Length() << std::endl;
	}
}

void pipeline_draw_L1_TrigEff(TH3D *h3Pass, TH3D *h3Total, TString project, TString name)
{
	//* Project the 3D histogram to 1D histogram and calculate the efficiency
	// auto hPass = (TH1D*)h3Pass->Project3D(project);
	// auto hTotal = (TH1D*)h3Total->Project3D(project);
	auto hPass = (TH1D*)h3Pass->ProjectionX("", 1,2);
	auto hTotal = (TH1D*)h3Total->ProjectionX("", 1,2);

	
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

	c->SaveAs(Form("./outFigures/Eff_L1_Trig_%s.pdf", project.Data()));

	delete c; delete line;
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

void update_progress(int ientry, int total_entries, int percentage_increment)
{
	if (ientry % (total_entries / percentage_increment) == 0)
	{
		std::cout << "Processing " << ientry << "th entry... (" << (int)((double)ientry / total_entries * 100) << "%)" << std::endl;
	}
}

//== Main =========================================================================================
void VCTree_TrigEff()
{
	TH1::SetDefaultSumw2(kTRUE);
	// std::string inputFileDir = "inFiles/2023/VCTree_STARLIGHT_CohJpsiToMuMu_5p36TeV_Run3.root";
	// std::string inputFileDir = "inFiles/2018/VertexCompositeTree_STARLIGHT_CohJpsiToMuMu_GenFilter_DiMuMC_20200906.root";
	std::string inputFileDir = "inFiles/2018/VertexCompositeTree_STARLIGHT_GGToMuMu_woPtCut_DiMuMC_20191122.root";

	const auto& csTreeDir = "dimucontana_mc";

	// Extract the tree
	VertexCompositeTree csTree;
	if (!csTree.GetTree(inputFileDir, csTreeDir)) 
	{
		cout << "Invalid Correct-Sign tree!" << endl;
		return;
	}

	bookHistos();

	int multiple_matched_tracks = 0;
	int total_tracks = 0;
	int total_entries = csTree.GetEntries();
	int max_entries = 100000;
	cout << "max_entries = " << total_entries << endl;
	for (Long64_t jentry = 1; jentry < total_entries; jentry++) 
	{
		if (jentry >= max_entries) break;

		update_progress(jentry, max_entries, 10);

		// Get the entry
		if (csTree.GetEntry(jentry) < 0) 
		{
			cout << "Invalid correct-sign entry!" << endl;
			return;
		}

		hnEvts->Fill(0.5);

		//= Event Level Selections ============================================================
		// Select Event - require this event has a valid vertex and is not beam-halo event
		//[2] = Flag_primaryVertexFilter: events have valid vertex
		//[3] = Flag_clusterCompatibilityFilter: the shapes of the clusters in the pixel detector are required to be compatible with the shapes expected from a heavy ion collision
		Bool_t goodVtx				= (csTree.evtSel()[2] && csTree.evtSel()[3]);

		//evtSel[4-15]
		//[4]=0: HFPlusMaxTower < 3 GeV;  [4]=1: HFPlusMaxTower > 3 GeV
		//[5]=0: HFMinusMaxTower < 3 GeV;  [5]=1: HFMinusMaxTower > 3 GeV
		//[6] is for Plus & [7] is for Minus; Threshold = 4 GeV
		//[8] is for Plus & [9] is for Minus; Threshold = 5 GeV
		//[10] is for Plus & [11] is for Minus; Threshold = 6 GeV
		//[12] is for Plus & [13] is for Minus; Threshold = 7 GeV
		//[14] is for Plus & [15] is for Minus; Threshold = 8 GeV
		//[16] is for Plus (Th = 7.6 GeV) & [17] is for Minus (Th = 7.3 GeV);
		Bool_t goodHFVeto 			= (!csTree.evtSel()[16] && !csTree.evtSel()[17]);
		Bool_t passNTrkHP 			= csTree.NtrkHP() == 2;	// contain only two high-purity tracks and nothing else
		Bool_t passEvtSel 			= goodVtx && goodHFVeto && passNTrkHP;
		Bool_t passNRecoCandidate 	= csTree.candSize() == 1;

		if (goodVtx)				hnEvts->Fill(1.5);
		if (goodVtx && goodHFVeto)	hnEvts->Fill(2.5);
		if (passEvtSel) 			hnEvts->Fill(3.5);
		if (passNRecoCandidate)		hnEvts->Fill(4.5);

		//* fill Ntrk distributions
		hNtrkofflinevsNtrkHP->Fill(csTree.Ntrkoffline(), csTree.NtrkHP());

		//= Fill Paired Muon Reco Eff Hist ===========================================================================
		for (UInt_t icand_gen = 0; icand_gen < csTree.candSize_gen(); icand_gen++)
		{
			//* Get the four-momentum of the pair
			const std::vector<TLorentzVector> fourMom_gen = GetFourMom(csTree.chargeD1_gen()[icand_gen], csTree.chargeD2_gen()[icand_gen], csTree.pTD1_gen()[icand_gen], csTree.pTD2_gen()[icand_gen], csTree.EtaD1_gen()[icand_gen], csTree.EtaD2_gen()[icand_gen], csTree.PhiD1_gen()[icand_gen], csTree.PhiD2_gen()[icand_gen]);
			TLorentzVector pairFourMom_gen = fourMom_gen[2];

			Double_t pt_gen		= pairFourMom_gen.Pt();
			Double_t eta_gen	= pairFourMom_gen.Eta();
			Double_t phi_gen	= pairFourMom_gen.Phi();
			Double_t mass_gen	= pairFourMom_gen.M();
			Double_t rap_gen	= pairFourMom_gen.Rapidity();

			//* fill denominator of Paired Muon reconstruction eff
			hPtvsEtavsPhi_gen	->Fill(pt_gen,	eta_gen,	phi_gen);
			hPtvsRapvsPhi_gen	->Fill(pt_gen,	rap_gen,	phi_gen);

			//* fill numerator of Paired Muon reconstruction eff
			Short_t recoIdx = csTree.RecIdx_gen()[icand_gen]; // recoIdx = -1 if not reconstructed

			if (csTree.candSize() > 0)
			{
				hPtvsEtavsPhi_gen_reconstructed	->Fill(pt_gen,	eta_gen,	phi_gen);
				hPtvsRapvsPhi_gen_reconstructed	->Fill(pt_gen,	rap_gen,	phi_gen);
			}
		}

		//= Fill Single Muon Reconstruction and L1Trigger Eff Hist ===================================================
		for (UInt_t icand_gen = 0; icand_gen < csTree.candSize_gen(); icand_gen++)
		{
			//* Get the four-momentum of the pair
			const std::vector<TLorentzVector> fourMom_gen = GetFourMom(csTree.chargeD1_gen()[icand_gen], csTree.chargeD2_gen()[icand_gen], csTree.pTD1_gen()[icand_gen], csTree.pTD2_gen()[icand_gen], csTree.EtaD1_gen()[icand_gen], csTree.EtaD2_gen()[icand_gen], csTree.PhiD1_gen()[icand_gen], csTree.PhiD2_gen()[icand_gen]);
			TLorentzVector pairFourMom_gen = fourMom_gen[2];

			Double_t pt_gen_mu_pos		= fourMom_gen[0].Pt();
			Double_t eta_gen_mu_pos		= fourMom_gen[0].Eta();
			Double_t phi_gen_mu_pos		= fourMom_gen[0].Phi();
			Double_t mass_gen_mu_pos	= fourMom_gen[0].M();
			Double_t rap_gen_mu_pos		= fourMom_gen[0].Rapidity();

			Double_t pt_gen_mu_neg		= fourMom_gen[1].Pt();
			Double_t eta_gen_mu_neg		= fourMom_gen[1].Eta();
			Double_t phi_gen_mu_neg		= fourMom_gen[1].Phi();
			Double_t mass_gen_mu_neg	= fourMom_gen[1].M();
			Double_t rap_gen_mu_neg		= fourMom_gen[1].Rapidity();

			//* fill denominator of single muon reconstruction eff
			hPtvsEtavsPhi_gen_mu_pos	->Fill(pt_gen_mu_pos,	eta_gen_mu_pos,	phi_gen_mu_pos);
			hPtvsEtavsPhi_gen_mu_neg	->Fill(pt_gen_mu_neg,	eta_gen_mu_neg,	phi_gen_mu_neg);

			total_tracks += 2;

			//= Using muon from the single muon branch =================================================
			Double_t BestDeltaR_mu_pos = 99999999.;
			Double_t BestDeltaR_mu_neg = 99999999.;
			Int_t    RecoIdx_mu_pos   = -1;
			Int_t    RecoIdx_mu_neg   = -1;

			for (UInt_t icand_mu = 0; icand_mu < csTree.candSize_mu(); icand_mu++)
			{
				//! Muon Level Selections !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				if (!csTree.softMuon_mu()[icand_mu]) continue;
				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

				TVector3 Mom_mu; Mom_mu.SetPtEtaPhi(csTree.pT_mu()[icand_mu], csTree.eta_mu()[icand_mu], csTree.phi_mu()[icand_mu]);
				TVector3 Mom_gen_mu_pos = fourMom_gen[0].Vect();
				TVector3 Mom_gen_mu_neg = fourMom_gen[1].Vect();

				//* Check if the current reconstructed muon (pos or neg) is matched to a generated one
				if ( RecGenMatched(Mom_mu, Mom_gen_mu_pos, BestDeltaR_mu_pos) )
				{
					BestDeltaR_mu_pos = Mom_gen_mu_pos.DeltaR(Mom_mu);
					RecoIdx_mu_pos = icand_mu;
				}
				if ( RecGenMatched(Mom_mu, Mom_gen_mu_neg, BestDeltaR_mu_neg) )
				{
					BestDeltaR_mu_neg = Mom_gen_mu_neg.DeltaR(Mom_mu);
					RecoIdx_mu_neg = icand_mu;
				}
			}

			//* warning if one reconstructed muon is matched to multiple generated muons
			if(RecoIdx_mu_pos>=0 && RecoIdx_mu_neg>=0 && RecoIdx_mu_pos == RecoIdx_mu_neg)
			{
				multiple_matched_tracks++;
				// cout<<"One reco-track is matched to multiple gen-tracks !"<<endl;
			}

			//* fill numerator of single muon reconstruction eff
			if (RecoIdx_mu_pos >= 0)
			{
				//* get the matched muon
				Double_t pt_mu_		= csTree.pT_mu()[RecoIdx_mu_pos];
				Double_t eta_mu_	= csTree.eta_mu()[RecoIdx_mu_pos];
				Double_t phi_mu_	= csTree.phi_mu()[RecoIdx_mu_pos];

				hPtvsEtavsPhi_gen_mu_pos_reconstructed	->Fill(pt_gen_mu_pos,	eta_gen_mu_pos,	phi_gen_mu_pos);
				hPtvsEtavsPhi_mu_pos		->Fill(pt_mu_,	eta_mu_,	phi_mu_);

				//* fill denominator of L1 trigger eff
				if (bool( csTree.trigMuon_mu()[L1_TrigIdx][RecoIdx_mu_pos]) )	
				{
					hPtvsEtavsPhi_mu_pos_trigL1->Fill(pt_mu_, eta_mu_, phi_mu_);
				}
			}
			if (RecoIdx_mu_neg >= 0)
			{
				//* get the matched muon
				Double_t pt_mu_		= csTree.pT_mu()[RecoIdx_mu_neg];
				Double_t eta_mu_	= csTree.eta_mu()[RecoIdx_mu_neg];
				Double_t phi_mu_	= csTree.phi_mu()[RecoIdx_mu_neg];

				hPtvsEtavsPhi_gen_mu_neg_reconstructed	->Fill(pt_gen_mu_neg,	eta_gen_mu_neg,	phi_gen_mu_neg);
				hPtvsEtavsPhi_mu_neg		->Fill(pt_mu_,	eta_mu_,	phi_mu_);

				//* fill denominator of L1 trigger eff
				if (bool(csTree.trigMuon_mu()[L1_TrigIdx][RecoIdx_mu_neg]))
				{
					hPtvsEtavsPhi_mu_neg_trigL1->Fill(pt_mu_, eta_mu_, phi_mu_);
				}
			}
			//=============================================================================================

			// //= Using muon from the paired muon branch =================================================
			// Int_t recoIdx = csTree.RecIdx_gen()[icand_gen];
			// if ( recoIdx<0 || csTree.candSize() <= 0 ) continue;
			// //! Muon Level Selections !!!!
			// if( !softCand[recoIdx] ) continue;
			// //!!!!!!!!!!!!!!!!!!!!!!!!!!!

			// //* Get the four-momentum of the pair
			// const std::vector<TLorentzVector> fourMom = GetFourMom(chargeD1[recoIdx], chargeD2[recoIdx], pTD1[recoIdx], pTD2[recoIdx], EtaD1[recoIdx], EtaD2[recoIdx], PhiD1[recoIdx], PhiD2[recoIdx]);
			// TLorentzVector pairFourMom = fourMom[2];

		}


		//! Event Level Selections !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (!passEvtSel) 			continue;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


		//= Fill HLT Trig Eff Hist =======================================================================
		for (UInt_t icand = 0; icand < csTree.candSize(); icand++)
		{
			//* Get the four-momentum of the pair
			const std::vector<TLorentzVector> fourMom = GetFourMom(csTree.chargeD1()[icand], csTree.chargeD2()[icand], csTree.pTD1()[icand], csTree.pTD2()[icand], csTree.EtaD1()[icand], csTree.EtaD2()[icand], csTree.PhiD1()[icand], csTree.PhiD2()[icand]);
			TLorentzVector pairFourMom = fourMom[2];

			Double_t pt		= pairFourMom.Pt();
			Double_t eta	= pairFourMom.Eta();
			Double_t phi	= pairFourMom.Phi();
			Double_t mass	= pairFourMom.M();
			Double_t rap	= pairFourMom.Rapidity();

			//* fill denominator of HLT trig eff
			hPtvsEtavsPhi	->Fill(pt,	eta,	phi);
			hPtvsRapvsPhi	->Fill(pt,	rap,	phi);

			//* fill numerator of HLT trig eff for each trigger
			for (int iTrig = 0; iTrig < nTrig; ++iTrig)
			{
				if (csTree.trigHLT()[HLT_TrigIdx[iTrig]])
				{
					vPtvsEtavsPhi_trigHLT[iTrig]->Fill(pt, eta, phi);
					vPtvsRapvsPhi_trigHLT[iTrig]->Fill(pt, rap, phi);
				}
			}
		}
	}

	cout<<"multiple_matched_tracks: "<<multiple_matched_tracks<<endl;
	cout<<"Percentage of multiple_matched_tracks: "<<multiple_matched_tracks*100./total_tracks<<"%"<<endl;

	draw_EvtSel();
	draw_Ntrk_distribution();

	//= Calculate HLT Trig Eff ========================================================================
	pipeline_draw_HLT_TrigEff(vPtvsRapvsPhi_trigHLT, hPtvsRapvsPhi, "x", "");
	pipeline_draw_HLT_TrigEff(vPtvsRapvsPhi_trigHLT, hPtvsRapvsPhi, "y", "");
	pipeline_draw_HLT_TrigEff(vPtvsRapvsPhi_trigHLT, hPtvsRapvsPhi, "z", "");

	//= Draw Reco Eff in Pt ===========================================================================
	pipeline_draw_RecoEff(hPtvsRapvsPhi_gen_reconstructed->ProjectionY(), hPtvsRapvsPhi_gen->ProjectionY(), "");

	//= Draw Reco Eff in Pt for Single Muon ============================================================
	pipeline_draw_RecoEff_SingleMu(hPtvsEtavsPhi_gen_mu_pos_reconstructed, hPtvsEtavsPhi_gen_mu_pos, "y", "");

	//= Draw L1 Trig Eff in Pt =========================================================================
	pipeline_draw_L1_TrigEff(hPtvsEtavsPhi_mu_pos_trigL1, hPtvsEtavsPhi_mu_pos, "x", "");
}