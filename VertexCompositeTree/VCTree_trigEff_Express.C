/*
	Use to plot online express data trigger efficiency
	Should use looser cuts when skimming the RECO data when producing VC tree

	JiaZhao Lin, Sep 2023
*/

#include "./VCTree_routines.C"

//# Global Variables #######################################################################################################
const int triggers_Idx = 0;

const int nbin_pt = 100;
const double min_pt = 0;
const double max_pt = 4;
const int nbin_eta = 100;
const double min_eta = -3;
const double max_eta = 3;
const int nbin_phi = 100;
const double min_phi = -3.14;
const double max_phi = 3.14;
const int nbin_mass = 100;
const double min_mass = 2;
const double max_mass = 5;
const int nbin_rap = 100;
const double min_rap = -2.6;
const double max_rap = 2.6;

//# Histograms ##############################################################################################################
TH1D *hRunNb = new TH1D("hRunNb", "hRunNb; Run Number", 200, 374700, 374900);
TH3D *hNum_PtEtaPhi = new TH3D("hNum_PtEtaPhi", "hNum_PtEtaPhi; p_{T} [GeV]; #eta; #phi", nbin_pt, min_pt, max_pt, nbin_eta, min_eta, max_eta, nbin_phi, min_phi, max_phi);
TH3D *hDen_PtEtaPhi = new TH3D("hDen_PtEtaPhi", "hDen_PtEtaPhi; p_{T} [GeV]; #eta; #phi", nbin_pt, min_pt, max_pt, nbin_eta, min_eta, max_eta, nbin_phi, min_phi, max_phi);

TH3D *hPtRapMass = new TH3D("hPtRapMass", "hPtRapMass; p_{T} [GeV]; y; Mass [GeV]", nbin_pt, min_pt, max_pt, nbin_rap, min_rap, max_rap, nbin_mass, min_mass, max_mass);
TH3D *hPtRapMass_trig = new TH3D("hPtRapMass_trig", "hPtRapMass_trig; p_{T} [GeV]; y; Mass [GeV]", nbin_pt, min_pt, max_pt, nbin_rap, min_rap, max_rap, nbin_mass, min_mass, max_mass);

TH1D *hnTracks_generalTracks = new TH1D("hnTracks_generalTracks", "hnTracks_generalTracks; nTracks", 100, 0, 100);

//# Functions ###############################################################################################################
TH1D* cal_eff(TH3D* h3Num, TH3D* h3Den, TString projection)
{
	TH1D *hNum = (TH1D*)h3Num->Project3D(projection.Data());
	TH1D *hDen = (TH1D*)h3Den->Project3D(projection.Data());

	TH1D *hEff = (TH1D*)hNum->Clone(Form("hEff_PtEtaPhi_%s", projection.Data()));
	hEff->SetTitle(Form("hEff_PtEtaPhi_%s; %s; Efficiency", projection.Data(), hNum->GetXaxis()->GetTitle()));
	hEff->Divide(hNum, hDen, 1, 1, "B");

	return hEff;
}

void fit_mass(TH1D *h1)
{
	//= fit the mass distribution with a crystal ball function within a jpis mass window =================
	// TF1 *f1 = new TF1("f1", "crystalball", 2.85, 3.3);
	// f1->SetParameters(20, 3.1, 0.1, 1, 1);
	// TF1 *f2 = new TF1("f2", "pol2", 2.85, 3.3);
	// f2->SetParNames("pol0", "pol1", "pol2");

	// TF1 *func = new TF1("func", "f1+f2", 2, 5);
	// // TF1 *func = new TF1("func", "f1", 2.70, 3.4);

	// h1->Fit("func", "R");

	//= use roofit to fit two crystal ball functions, one at the Jpsi mass and one at the psi(2S) mass, and a pol2 background function =================
	RooRealVar x("x", "m_{#mu^{+}#mu^{-}} [GeV]", 2.0, 5);
	RooRealVar  JpsiMean("JpsiMean", "JpsiMean", 3.1, 2.9, 3.3);
	RooRealVar  JpsiSigma("JpsiSigma", "JpsiSigma", 0.04, 0.01, 0.2);
	RooRealVar  JpsiAlpha("JpsiAlpha", "JpsiAlpha", 1, 0.1, 10);
	RooRealVar  JpsiN("JpsiN", "JpsiN", 5, 0.1, 20);
	RooCBShape Jpsi("Jpsi", "Jpsi", x, JpsiMean, JpsiSigma, JpsiAlpha, JpsiN);


	RooConstVar massRatio = RooConstVar(  "massRatio",   "massRatio",   3.686/3.097 );
	// psiMu = jpsiMu * massRatio; psiSigma = jpsiSigma * massRatio
	RooFormulaVar Psi2SMean("Psi2SMean", "Psi2SMean", "@0*@1", RooArgList(JpsiMean, massRatio));
	RooFormulaVar Psi2SSigma("Psi2SSigma", "Psi2SSigma", "@0*@1", RooArgList(JpsiSigma, massRatio));
	RooCBShape Psi2S("Psi2S", "Psi2S", x, Psi2SMean, Psi2SSigma, JpsiAlpha, JpsiN);

	RooRealVar  pol0("pol0", "pol0", 10, 0, 100);
	RooRealVar  pol1("pol1", "pol1", -2, -10, 10);
	RooRealVar  pol2("pol2", "pol2", 1, -20, 20);
	RooPolynomial Bkg("Bkg", "Bkg", x, RooArgList(pol0));
	// RooPolynomial Bkg("Bkg", "Bkg", x, RooArgList(pol0, pol1));
	// RooPolynomial Bkg("Bkg", "Bkg", x, RooArgList(pol0, pol1, pol2));

	double nJpsi_init = h1->Integral(h1->FindBin(3.0), h1->FindBin(3.2));
	double nPsi2S_init = nJpsi_init * 0.05;
	//use sideband to estimate the background
	double nBkg_init = h1->Integral(h1->FindBin(2.), h1->FindBin(2.8)) + h1->Integral(h1->FindBin(3.3), h1->FindBin(3.5)) + h1->Integral(h1->FindBin(3.7), h1->FindBin(5.0));

	RooRealVar nJpsi("nJpsi", "nJpsi", nJpsi_init, 0, nJpsi_init * 10);
	RooRealVar nPsi2S("nPsi2S", "nPsi2S", nPsi2S_init, 0, nPsi2S_init * 10);
	RooRealVar nBkg("nBkg", "nBkg", nBkg_init, 0, nBkg_init * 10);

	RooAddPdf totMassPdf("totMassPdf", "totMassPdf", RooArgList(Jpsi, Psi2S, Bkg), RooArgList(nJpsi, nPsi2S, nBkg));
	// RooAddPdf totMassPdf("totMassPdf", "totMassPdf", RooArgList(Jpsi, Bkg), RooArgList(nJpsi, nBkg));

	h1->Rebin(2);
	RooDataHist dataMass("dataMass", "dataMass", x, h1);
	auto fitResult = totMassPdf.fitTo(dataMass, RooFit::Extended(kTRUE), RooFit::SumW2Error(kTRUE), RooFit::Hesse(kTRUE), RooFit::Minos(kFALSE), RooFit::PrintLevel(-1), RooFit::Save(kTRUE));

	RooPlot *frame = x.frame();
	dataMass.plotOn(frame, RooFit::MarkerStyle(20), RooFit::MarkerSize(1), RooFit::MarkerColor(2), RooFit::LineColor(2), RooFit::LineWidth(2), RooFit::DrawOption("pz"));
	totMassPdf.plotOn(frame, RooFit::LineColor(1), RooFit::LineStyle(1), RooFit::LineWidth(2));
	totMassPdf.plotOn(frame, RooFit::Components("Jpsi"), RooFit::LineColor(kRed), 		RooFit::LineStyle(5), RooFit::LineWidth(2));
	totMassPdf.plotOn(frame, RooFit::Components("Psi2S"), RooFit::LineColor(kGreen), 	RooFit::LineStyle(6), RooFit::LineWidth(2));
	totMassPdf.plotOn(frame, RooFit::Components("Bkg"), RooFit::LineColor(kBlue), 		RooFit::LineStyle(2), RooFit::LineWidth(3));
	// totMassPdf.paramOn(frame, RooFit::Layout(0.5, 0.9, 0.9));
	//Set y axis label
	frame->GetYaxis()->SetTitle(Form("Events / (%.2f GeV)", h1->GetBinWidth(1)));
	frame->Draw();
	//draw chi2/ndf on the plot
	double chi2 = frame->chiSquare("totMassPdf_Norm[x]", "h_dataMass", fitResult->floatParsFinal().getSize());
	
	// add legend
	TLegend *leg = new TLegend(0.65, 0.55, 0.9, 0.85);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->AddEntry(frame->findObject("h_dataMass"), "Data", "pe");
	leg->AddEntry(frame->findObject("totMassPdf_Norm[x]"), "Fit", "l");
	leg->AddEntry(frame->findObject("totMassPdf_Norm[x]_Comp[Jpsi]"), "J/#psi", "l");
	leg->AddEntry(frame->findObject("totMassPdf_Norm[x]_Comp[Psi2S]"), "#psi(2S)", "l");
	leg->AddEntry(frame->findObject("totMassPdf_Norm[x]_Comp[Bkg]"), "Bkg", "l");
	leg->Draw("same");
	
	TLatex *t = new TLatex();
	t->SetNDC();
	t->SetTextFont(42);
	t->SetTextSize(0.05);
	// cout << "chi2/ndf = " << chi2 << endl;
	// t->DrawLatex(0.15, 0.85, Form("#chi^{2}/ndf = %.2f", chi2));
	t->DrawLatex(0.13, 0.94, Form("#bf{CMS} #it{Preliminary}"));
	t->DrawLatex(0.62, 0.94, Form("PbPb (2023, 5.36 TeV)"));


	
}

void plot_eff(TH1D* hEff)
{
	auto c = new TCanvas("c", "", 800, 600);

	//set y axis range
	hEff->GetYaxis()->SetRangeUser(0, 1.1);
	hEff->Draw();

	//draw a line at 1
	TLine *line = new TLine(hEff->GetXaxis()->GetXmin(), 1, hEff->GetXaxis()->GetXmax(), 1);
	line->SetLineColor(kRed);
	line->SetLineStyle(2);
	line->SetLineWidth(2);
	line->Draw("same");

	c->SaveAs(Form("outFigures/%s.png", hEff->GetName()));

	delete c;
}

void plot_hist2D_compare(TH3D *h3Num, TH3D *h3Den, TString projection)
{
	auto c = new TCanvas("c", "", 800, 600);

	TH2D *h2Num = (TH2D*)h3Num->Project3D(projection.Data());
	TH2D *h2Den = (TH2D*)h3Den->Project3D(projection.Data());

	h2Den->Draw();

	h2Num->Draw("same");
	h2Num->SetMarkerStyle(24);
	h2Num->SetMarkerColor(kRed);

	//legend
	auto legend = new TLegend(0.40, 0.2, 0.60, 0.3);
	// make the legend transparent
	legend->SetFillStyle(0);
	// legend->SetTextSize(0.03);
	legend->AddEntry(h2Den, "Soft Muon", "p");
	legend->AddEntry(h2Num, "Trig Muon", "p");
	legend->Draw("same");

	c->SaveAs(Form("outFigures/SoftVsTrig_PtEtaPhi_%s.png", projection.Data()));

	delete c;
}

void plot_hist_projection(TH3D *h3)
{
	auto c = new TCanvas("c", "", 800, 600);

	TH1D *h1_x = (TH1D*)h3->Project3D("x");
	TH1D *h1_y = (TH1D*)h3->Project3D("y");
	TH1D *h1_z = (TH1D*)h3->Project3D("z");

	// h1_x->Draw("E");
	// c->SaveAs(Form("outFigures/%s_x.png", h3->GetName()));

	// h1_y->Draw("E");
	// c->SaveAs(Form("outFigures/%s_y.png", h3->GetName()));

	fit_mass(h1_z);
	// h1_z->Draw("same E");
	c->SaveAs(Form("outFigures/%s_z.png", h3->GetName()));

	delete c;
}

//# Main ####################################################################################################################
void VCTree_trigEff_Express()
{
	// TH1::SetDefaultSumw2(kTRUE);
	// std::string inputFile = "inFiles/2023/VCTree_STARLIGHT_CohJpsiToMuMu_5p36TeV_Run3.root";
	// std::string inputFile = "inFiles/2023/VCTree_Express_374345.root";
	// std::string inputFile = "inFiles/2023/VCTree_HIForward0_streamer_ppReco_Run374719.root";
	// std::string inputFile = "inFiles/2023/VCTree_HIForward0_streamer_ppReco_Run374778.root";
	// std::string inputFile = "inFiles/2023/VCTree_HIForward0_streamer_ppReco_Run374803.root";
	// std::string inputFile = "inFiles/2023/VCTree_HIForward0_streamer_ppReco_Run374810.root";
	std::string inputFile = "inFiles/2023/VCTree_HIForward0_streamer_ppReco_Run374719_374730_374778_374803_374810_374833.root";

	// std::string inputFile = "inFiles/2023/VCTree_Express_374345_withTrk.root";
	// std::string inputFile = "inFiles/2023/VCTree_HIReco_Run374666.root";
	// std::string inputFile = "inFiles/2018/VertexCompositeTree_STARLIGHT_CohJpsiToMuMu_GenFilter_DiMuMC_20200906.root";

	// Extract the tree
	VertexCompositeTree csTree;
	open_VCTree(csTree, inputFile, "dimucontana");

	int total_entries = csTree.GetEntries();
	int total_softMuon = 0;
	int total_trigMuon = 0;
	for (int jentry = 0; jentry < total_entries; jentry++) 
	{
		if (csTree.GetEntry(jentry) < 0) 
		{
			cout << "Invalid correct-sign entry!" << endl;
			return;
		}
		//= Event Level Selections ============================================================
		//= Check SkimTree code for EvtSel() index ============================================
		// Bool_t goodVtx				= (csTree.evtSel()[2] && csTree.evtSel()[3]);
		Bool_t goodVtx				=  csTree.evtSel()[2];
		// Bool_t goodHFVeto 			= (!csTree.evtSel()[16] && !csTree.evtSel()[17]);
		Bool_t passNTrkHP 			= csTree.NtrkHP() == 2;	// contain only two high-purity tracks and nothing else
		// Bool_t passEvtSel 			= goodVtx && goodHFVeto && passNTrkHP;
		// Bool_t passNRecoCandidate 	= csTree.candSize() > 0;
		Bool_t passHFVeto 			= (csTree.HFmaxETPlus() < 7.3) && (csTree.HFmaxETMinus() < 7.6);

		// if (goodVtx)				hnEvts->Fill(1.5);
		// if (goodVtx && goodHFVeto)	hnEvts->Fill(2.5);
		// if (passEvtSel) 			hnEvts->Fill(3.5);
		// if (passNRecoCandidate)		hnEvts->Fill(4.5);

		if (!goodVtx) continue;
		if (!passNTrkHP) continue;
		// if (!passHFVeto) continue;

		// for (int imu = 0; imu < csTree.candSize_mu(); ++imu)
		// {
		// 	if( !csTree.softMuon_mu()[imu] ) continue;
		// 	if( !csTree.HPMuon_mu()[imu] ) continue;

		// 	total_softMuon++;

		// 	double pT_mu = csTree.pT_mu()[imu];
		// 	double eta_mu = csTree.eta_mu()[imu];
		// 	double phi_mu = csTree.phi_mu()[imu];

		// 	if (csTree.trigMuon_mu()[triggers_Idx][imu])
		// 	{
		// 		hNum_PtEtaPhi->Fill(pT_mu, eta_mu, phi_mu);
		// 		total_trigMuon++;
		// 	}
		// 	hDen_PtEtaPhi->Fill(pT_mu, eta_mu, phi_mu);
		// }

		for (int icand = 0; icand < csTree.candSize(); ++icand)
		{
			if (!csTree.softMuon1()[icand]) continue;
			if (!csTree.softMuon2()[icand]) continue;
			if (!csTree.HPMuon1()[icand]) continue;
			if (!csTree.HPMuon2()[icand]) continue;

			double mass = csTree.mass()[icand];
			double rap = csTree.y()[icand];
			double pt = csTree.pT()[icand];

			if (mass < 2.0 || mass > 5.) continue;
			if (abs(rap) < 1.6 || abs(rap) > 2.4 ) continue;

			hRunNb->Fill(csTree.RunNb());
			total_softMuon += 2;
			hPtRapMass->Fill(pt, rap, mass);

			double ptD1 = csTree.pTD1()[icand];
			double etaD1 = csTree.EtaD1()[icand];
			double phiD1 = csTree.PhiD1()[icand];
			double ptD2 = csTree.pTD2()[icand];
			double etaD2 = csTree.EtaD2()[icand];
			double phiD2 = csTree.PhiD2()[icand];
			bool triggerD1 = csTree.trigMuon1()[triggers_Idx][icand];
			bool triggerD2 = csTree.trigMuon2()[triggers_Idx][icand];

			if (triggerD1)
			{
				hNum_PtEtaPhi->Fill(ptD1, etaD1, phiD1);
				total_trigMuon++;
			}
			if (triggerD2)
			{
				hNum_PtEtaPhi->Fill(ptD2, etaD2, phiD2);
				total_trigMuon++;
			}
			if (triggerD1 || triggerD2)
			{
				hPtRapMass_trig->Fill(pt, rap, mass);
			}

			hDen_PtEtaPhi->Fill(ptD1, etaD1, phiD1);
			hDen_PtEtaPhi->Fill(ptD2, etaD2, phiD2);
		}

		// hnTracks_generalTracks->Fill(csTree.nTracks_generalTracks());
	}
	cout << "total_softMuon = " << total_softMuon << endl;
	cout << "total_trigMuon = " << total_trigMuon << endl;

	TH1D *hEff_x = cal_eff(hNum_PtEtaPhi, hDen_PtEtaPhi, "x");
	TH1D *hEff_y = cal_eff(hNum_PtEtaPhi, hDen_PtEtaPhi, "y");
	TH1D *hEff_z = cal_eff(hNum_PtEtaPhi, hDen_PtEtaPhi, "z");
	// plot_eff(hEff_x);
	// plot_eff(hEff_y);
	// plot_eff(hEff_z);
	// plot_hist2D_compare(hNum_PtEtaPhi, hDen_PtEtaPhi, "xy");
	// plot_hist_projection(hNum_PtEtaPhi);
	// plot_hist_projection(hPtRapMass);
	plot_hist_projection(hPtRapMass_trig);

	auto c = new TCanvas("c", "", 800, 600);
	hRunNb->Draw();
	c->SaveAs("outFigures/hRunNb.png");
}