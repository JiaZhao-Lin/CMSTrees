#include "../VertexCompositeTree/VCTree_utilities.C"

//# Constants #############################################################################################################
const int nbin_track = 5;
const int nbin_hit = 30;


//# Histograms ##############################################################################################################
TH1D *h_hltPixelTracksPPOnAA = new TH1D("h_hltPixelTracksPPOnAA", "h_hltPixelTracksPPOnAA;N_{track}^{HLT pixel}", nbin_track, 0, nbin_track);
TH1D *h_hltPixelTracksForUPCPPOnAA = new TH1D("h_hltPixelTracksForUPCPPOnAA", "h_hltPixelTracksForUPCPPOnAA;N_{track}^{HLT pixel UPC}", nbin_track, 0, nbin_track);
TH1D *h_hltPixelTracksLowPtForUPCPPOnAA = new TH1D("h_hltPixelTracksLowPtForUPCPPOnAA", "h_hltPixelTracksLowPtForUPCPPOnAA;N_{track}^{HLT pixel LowPtUPC}", nbin_track, 0, nbin_track);
TH1D *h_generalTracks = new TH1D("h_generalTracks", "h_generalTracks;N_{track}^{general}", nbin_track, 0, nbin_track);
TH1D *h_hiConformalPixelTracks = new TH1D("h_hiConformalPixelTracks", "h_hiConformalPixelTracks;N_{track}^{conformal pixel}", nbin_track, 0, nbin_track);

TH1D *h_hltPixelTracksPPOnAA_nHits = new TH1D("h_hltPixelTracksPPOnAA_nHits", "h_hltPixelTracksPPOnAA_nHits;N_{hit}^{HLT pixel}", nbin_hit, 0, nbin_hit);
TH1D *h_hltPixelTracksForUPCPPOnAA_nHits = new TH1D("h_hltPixelTracksForUPCPPOnAA_nHits", "h_hltPixelTracksForUPCPPOnAA_nHits;N_{hit}^{HLT pixel UPC}", nbin_hit, 0, nbin_hit);
TH1D *h_hltPixelTracksLowPtForUPCPPOnAA_nHits = new TH1D("h_hltPixelTracksLowPtForUPCPPOnAA_nHits", "h_hltPixelTracksLowPtForUPCPPOnAA_nHits;N_{hit}^{HLT pixel LowPtUPC}", nbin_hit, 0, nbin_hit);
TH1D *h_generalTracks_nHits = new TH1D("h_generalTracks_nHits", "h_generalTracks_nHits;N_{hit}^{general}", nbin_hit, 0, nbin_hit);
TH1D *h_hiConformalPixelTracks_nHits = new TH1D("h_hiConformalPixelTracks_nHits", "h_hiConformalPixelTracks_nHits;N_{hit}^{conformal pixel}", nbin_hit, 0, nbin_hit);

TH2D *h_hltPixelTracksPPOnAA_Vs_generalTracks = new TH2D("h_hltPixelTracksPPOnAA_Vs_generalTracks", "hltPixelTracksPPOnAA_Vs_generalTracks;N_{track}^{HLT pixel};N_{track}^{general}", nbin_track, 0, nbin_track, nbin_track, 0, nbin_track);
TH2D *h_hltPixelTracksForUPCPPOnAA_Vs_generalTracks = new TH2D("h_hltPixelTracksForUPCPPOnAA_Vs_generalTracks", "hltPixelTracksForUPCPPOnAA_Vs_generalTracks;N_{track}^{HLT pixel UPC};N_{track}^{general}", nbin_track, 0, nbin_track, nbin_track, 0, nbin_track);
TH2D *h_hltPixelTracksLowPtForUPCPPOnAA_Vs_generalTracks = new TH2D("h_hltPixelTracksLowPtForUPCPPOnAA_Vs_generalTracks", "hltPixelTracksLowPtForUPCPPOnAA_Vs_generalTracks;N_{track}^{HLT pixel LowPtUPC};N_{track}^{general}", nbin_track, 0, nbin_track, nbin_track, 0, nbin_track);

//# Functions ###############################################################################################################
TH1D* draw_frame_dist(TString title, int nbin, int y_max)
{
	TH1D *hFrame = new TH1D("hFrame", title, nbin, 0, nbin);
	for (int i = 0; i < nbin; ++i)
	{
		hFrame->GetXaxis()->SetBinLabel(i + 1, Form("%d", i));
	}
	//set the y axis range
	hFrame->GetYaxis()->SetRangeUser(0, y_max);
	hFrame->Draw();

	return hFrame;
}

void format_hist_dist2D(TH2D* h, int nbin)
{
	for (int i = 0; i < nbin; ++i)
	{
		h->GetXaxis()->SetBinLabel(i + 1, Form("%d", i));
		h->GetYaxis()->SetBinLabel(i + 1, Form("%d", i));
	}
}

void plot_1D()
{
	//= online ntrack distribution ==================================================================
	auto c = new TCanvas("c", "", 800, 600);

	auto hFrame = draw_frame_dist("h_hltPixelTracksPPOnAA;N_{track}^{Online}", nbin_track, 1e3);
	h_hltPixelTracksPPOnAA->Draw("same");
	h_hltPixelTracksForUPCPPOnAA->SetLineColor(kRed);
	h_hltPixelTracksForUPCPPOnAA->Draw("same");
	h_hltPixelTracksLowPtForUPCPPOnAA->SetLineColor(kBlue);
	h_hltPixelTracksLowPtForUPCPPOnAA->Draw("same");

	//legend
	auto legend = new TLegend(0.65, 0.7, 0.80, 0.9);
	legend->SetTextSize(0.02);
	legend->AddEntry(h_hltPixelTracksPPOnAA, "hltPixelTracksPPOnAA", "l");
	legend->AddEntry(h_hltPixelTracksForUPCPPOnAA, "hltPixelTracksForUPCPPOnAA", "l");
	legend->AddEntry(h_hltPixelTracksLowPtForUPCPPOnAA, "hltPixelTracksLowPtForUPCPPOnAA", "l");
	legend->Draw("same");
	c->SaveAs("outFigures/online_nTracks.png");
	delete hFrame;

	//= online nhit distribution ====================================================================
	hFrame = draw_frame_dist("h_hltPixelTracksPPOnAA_nHits;N_{hit}^{Online}", nbin_hit, 1.5e3);
	h_hltPixelTracksPPOnAA_nHits->Draw("same");
	h_hltPixelTracksForUPCPPOnAA_nHits->SetLineColor(kRed);
	h_hltPixelTracksForUPCPPOnAA_nHits->Draw("same");
	h_hltPixelTracksLowPtForUPCPPOnAA_nHits->SetLineColor(kBlue);
	h_hltPixelTracksLowPtForUPCPPOnAA_nHits->Draw("same");

	//legend
	auto legend1 = new TLegend(0.65, 0.7, 0.80, 0.9);
	legend1->SetTextSize(0.02);
	legend1->AddEntry(h_hltPixelTracksPPOnAA_nHits, "hltPixelTracksPPOnAA", "l");
	legend1->AddEntry(h_hltPixelTracksForUPCPPOnAA_nHits, "hltPixelTracksForUPCPPOnAA", "l");
	legend1->AddEntry(h_hltPixelTracksLowPtForUPCPPOnAA_nHits, "hltPixelTracksLowPtForUPCPPOnAA", "l");
	legend1->Draw("same");
	c->SaveAs("outFigures/online_nHits.png");
	delete hFrame;

	//= offline ntrack distribution =================================================================
	hFrame = draw_frame_dist("h_generalTracks;N_{track}^{Offline}", nbin_track, 0.03e3);
	h_generalTracks->Draw("same");
	h_hiConformalPixelTracks->SetLineColor(kRed);
	h_hiConformalPixelTracks->Draw("same");

	//legend
	auto legend2 = new TLegend(0.65, 0.7, 0.80, 0.9);
	legend2->SetTextSize(0.03);
	legend2->AddEntry(h_generalTracks, "generalTracks", "l");
	legend2->AddEntry(h_hiConformalPixelTracks, "hiConformalPixelTracks", "l");
	legend2->Draw("same");
	c->SaveAs("outFigures/offline_nTracks.png");
	delete hFrame;

	//= offline nhit distribution ===================================================================
	hFrame = draw_frame_dist("h_generalTracks_nHits;N_{hit}^{Offline}", nbin_hit, 4e6);
	h_generalTracks_nHits->Draw("same");
	h_hiConformalPixelTracks_nHits->SetLineColor(kRed);
	h_hiConformalPixelTracks_nHits->Draw("same");

	//legend
	auto legend3 = new TLegend(0.65, 0.7, 0.80, 0.9);
	legend3->SetTextSize(0.03);
	legend3->AddEntry(h_generalTracks_nHits, "generalTracks", "l");
	legend3->AddEntry(h_hiConformalPixelTracks_nHits, "hiConformalPixelTracks", "l");
	legend3->Draw("same");
	c->SaveAs("outFigures/offline_nHits.png");
	delete hFrame;
	delete c;
}

void plot_hist2D(TH2D* h2)
{
	//= hltPixelTracksPPOnAA_Vs_generalTracks ===============================================================
	auto c = new TCanvas("c", "", 800, 600);
	format_hist_dist2D(h2, nbin_track);
	h2->Draw("colz");
	// offset the graph a little bit
	// gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.13);
	gPad->SetBottomMargin(0.13);

	//draw percentage on each bin
	for (int i = 0; i < h2->GetNbinsX(); ++i)
	{
		for (int j = 0; j < h2->GetNbinsY(); ++j)
		{
			draw_latex(0.14 + i*0.15, 0.17 + j*0.16, Form("%.2f %%", h2->GetBinContent(i+1, j+1)/h2->GetEntries()*100.0));
		}
	}
	c->SaveAs(Form("outFigures/%s.png", h2->GetTitle()));
	delete c;
}

void plot_eff_tracking(TH2D *h2, TString name)
{
	TH1D *hOnline = (TH1D*)h2->ProjectionX();
	TH1D *hOffline = (TH1D*)h2->ProjectionY();
	TH1D *hEff_online = new TH1D("hEff_online", "hEff_online; N_{track}^{online}; Efficiency", nbin_track, 0, nbin_track);
	TH1D *hEff_offline = new TH1D("hEff_offline", "hEff_offline; N_{track}^{offline}; Efficiency", nbin_track, 0, nbin_track);

	//*efficiency online: when there is i track online, the probability of finding at least i track offline
	for (int i = 0; i < h2->GetNbinsX(); i++)
	{
		double total = 0;
		for (int j = i; j < h2->GetNbinsY(); j++)
		{
			total += h2->GetBinContent(i+1, j+1);
		}
		hEff_online->SetBinContent(i+1, total);
	}
	hEff_online->Divide(hEff_online, hOnline, 1, 1, "B");

	//*efficiency offline: when there is i track offline, the probability of not missing any track online
	for (int i = 0; i < h2->GetNbinsY(); i++)
	{
		double total = 0;
		for (int j = 0; j <= i; j++)
		{
			total += h2->GetBinContent(j+1, i+1);
		}
		hEff_offline->SetBinContent(i+1, total);
	}
	hEff_offline->Divide(hEff_offline, hOffline, 1, 1, "B");

	//= plotting======================================================================================
	auto c = new TCanvas("c", "", 800, 600);
	hEff_online->GetYaxis()->SetRangeUser(0, 1.1);
	hEff_online->Draw();

	for (int i = 0; i < hEff_online->GetNbinsX(); i++)
	{
		hEff_online->GetXaxis()->SetBinLabel(i + 1, Form("%d", i));
	}

	//Draw line at y = 1
	TLine *line = new TLine(0, 1, nbin_track, 1);
	line->SetLineColor(kRed);
	line->SetLineStyle(2);
	line->Draw("same");

	c->SaveAs(Form("outFigures/%s_online.png", name.Data()));

	hEff_offline->GetYaxis()->SetRangeUser(0, 1.1);
	hEff_offline->Draw();

	for (int i = 0; i < hEff_offline->GetNbinsX(); i++)
	{
		hEff_offline->GetXaxis()->SetBinLabel(i + 1, Form("%d", i));
	}

	//Draw line at y = 1
	line->Draw("same");

	c->SaveAs(Form("outFigures/%s_offline.png", name.Data()));

	delete c;
	delete hEff_online; delete hEff_offline;
	delete hOnline;
}

void pipeline_fill_tracks(TH1D *hTracks, TH1D* hHits, int &mTrack, std::vector<int> *mHits)
{
	hTracks->Fill(mTrack);
	for (int i = 0; i < mHits->size(); ++i)
	{
		hHits->Fill(mHits->at(i));
	}
}

//# Main ###################################################################################################################
void plot_Tracks()
{
	//= Read the Tree ================================================================================
	// TFile *f = new TFile("inFiles/Tracks_Jpsi.root");
	TFile *f = new TFile("inFiles/Tracks_Phi.root");
	// TFile *f = new TFile("inFiles/Tracks_hydjet.root");
	TTree *myTree = (TTree*)f->Get("Tracks");

	int m_hltPixelTracksPPOnAA;
	int m_hltPixelTracksForUPCPPOnAA;
	int m_hltPixelTracksLowPtForUPCPPOnAA;
	int m_generalTracks;
	int m_hiConformalPixelTracks;
	std::vector<int> *m_hltPixelTracksPPOnAA_nHits = 0;
	std::vector<int> *m_hltPixelTracksForUPCPPOnAA_nHits = 0;
	std::vector<int> *m_hltPixelTracksLowPtForUPCPPOnAA_nHits = 0;
	std::vector<int> *m_generalTracks_nHits = 0;
	std::vector<int> *m_hiConformalPixelTracks_nHits = 0;

	myTree->SetBranchAddress("hltPixelTracksPPOnAA", &m_hltPixelTracksPPOnAA);
	myTree->SetBranchAddress("hltPixelTracksForUPCPPOnAA", &m_hltPixelTracksForUPCPPOnAA);
	myTree->SetBranchAddress("hltPixelTracksLowPtForUPCPPOnAA", &m_hltPixelTracksLowPtForUPCPPOnAA);
	myTree->SetBranchAddress("generalTracks", &m_generalTracks);
	myTree->SetBranchAddress("hiConformalPixelTracks", &m_hiConformalPixelTracks);
	myTree->SetBranchAddress("hltPixelTracksPPOnAA_nHits", &m_hltPixelTracksPPOnAA_nHits);
	myTree->SetBranchAddress("hltPixelTracksForUPCPPOnAA_nHits", &m_hltPixelTracksForUPCPPOnAA_nHits);
	myTree->SetBranchAddress("hltPixelTracksLowPtForUPCPPOnAA_nHits", &m_hltPixelTracksLowPtForUPCPPOnAA_nHits);
	myTree->SetBranchAddress("generalTracks_nHits", &m_generalTracks_nHits);
	myTree->SetBranchAddress("hiConformalPixelTracks_nHits", &m_hiConformalPixelTracks_nHits);

	//= Loop over the Tree ===========================================================================
	int nentries = myTree->GetEntries();
	for (int i = 0; i < nentries; ++i)
	{
		myTree->GetEntry(i);


		pipeline_fill_tracks(h_hltPixelTracksPPOnAA, h_hltPixelTracksPPOnAA_nHits, m_hltPixelTracksPPOnAA, m_hltPixelTracksPPOnAA_nHits);
		pipeline_fill_tracks(h_hltPixelTracksForUPCPPOnAA, h_hltPixelTracksForUPCPPOnAA_nHits, m_hltPixelTracksForUPCPPOnAA, m_hltPixelTracksForUPCPPOnAA_nHits);
		pipeline_fill_tracks(h_hltPixelTracksLowPtForUPCPPOnAA, h_hltPixelTracksLowPtForUPCPPOnAA_nHits, m_hltPixelTracksLowPtForUPCPPOnAA, m_hltPixelTracksLowPtForUPCPPOnAA_nHits);
		pipeline_fill_tracks(h_generalTracks, h_generalTracks_nHits, m_generalTracks, m_generalTracks_nHits);
		pipeline_fill_tracks(h_hiConformalPixelTracks, h_hiConformalPixelTracks_nHits, m_hiConformalPixelTracks, m_hiConformalPixelTracks_nHits);

		if (m_hltPixelTracksPPOnAA < 0 || m_hltPixelTracksForUPCPPOnAA < 0 || m_hltPixelTracksLowPtForUPCPPOnAA < 0 || m_generalTracks < 0 || m_hiConformalPixelTracks < 0)
		{
			cout << "Error: negative number of tracks" << endl;
			continue;
		} 
		h_hltPixelTracksPPOnAA_Vs_generalTracks->Fill(m_hltPixelTracksPPOnAA, m_generalTracks);
		h_hltPixelTracksForUPCPPOnAA_Vs_generalTracks->Fill(m_hltPixelTracksForUPCPPOnAA, m_generalTracks);
		h_hltPixelTracksLowPtForUPCPPOnAA_Vs_generalTracks->Fill(m_hltPixelTracksLowPtForUPCPPOnAA, m_generalTracks);
	}

	//= Plotting ====================================================================================
	plot_1D();
	plot_eff_tracking(h_hltPixelTracksPPOnAA_Vs_generalTracks, "eff_PixelTracksPPOnAA_Vs_generalTracks");
	plot_eff_tracking(h_hltPixelTracksForUPCPPOnAA_Vs_generalTracks, "eff_PixelTracksForUPCPPOnAA_Vs_generalTracks");
	plot_eff_tracking(h_hltPixelTracksLowPtForUPCPPOnAA_Vs_generalTracks, "eff_PixelTracksLowPtForUPCPPOnAA_Vs_generalTracks");
	plot_hist2D(h_hltPixelTracksPPOnAA_Vs_generalTracks);
	plot_hist2D(h_hltPixelTracksForUPCPPOnAA_Vs_generalTracks);
	plot_hist2D(h_hltPixelTracksLowPtForUPCPPOnAA_Vs_generalTracks);

}