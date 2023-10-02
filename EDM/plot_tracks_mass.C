#include "../VertexCompositeTree/VCTree_utilities.C"

//# Constants #############################################################################################################
const int nbin_track = 5;
const int nbin_hit = 30;
const int nbin_mass = 80;


//# Histograms ##############################################################################################################
TH1D *h_generalTracks_nTrakcs = new TH1D("h_generalTracks_nTrakcs", "h_generalTracks_nTrakcs;N_{track}^{generalTrk}", nbin_track, 0, nbin_track);
TH1D *h_generalTracks_nHit = new TH1D("h_generalTracks_nHit", "h_generalTracks_nHit;N_{hit}^{generalTrk}", nbin_hit, 0, nbin_hit);
TH1D *h_generalTracks_mass = new TH1D("h_generalTracks_mass", "h_generalTracks_mass;Mass [GeV]", nbin_mass, 2.2, 5);

TH1D *h_globalMuons_nTrakcs = new TH1D("h_globalMuons_nTrakcs", "h_globalMuons_nTrakcs;N_{track}^{globalMu}", nbin_track, 0, nbin_track);
TH1D *h_globalMuons_nHit = new TH1D("h_globalMuons_nHit", "h_globalMuons_nHit;N_{hit}^{globalMu}", nbin_hit, 0, nbin_hit);
TH1D *h_globalMuons_mass = new TH1D("h_globalMuons_mass", "h_globalMuons_mass;Mass [GeV]", nbin_mass, 0, 5);


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

void format_hist_dist(TH1D* h)
{
	for (int i = 0; i < h->GetNbinsX(); ++i)
	{
		h->GetXaxis()->SetBinLabel(i + 1, Form("%d", i));
	}
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
	auto c = new TCanvas("c", "", 800, 600);

	//= offline ntrack distribution =================================================================
	format_hist_dist(h_generalTracks_nTrakcs);
	h_generalTracks_nTrakcs->Draw();

	//legend
	// auto legend2 = new TLegend(0.65, 0.7, 0.80, 0.9);
	// legend2->SetTextSize(0.03);
	// legend2->AddEntry(h_generalTracks, "generalTracks", "l");
	// legend2->Draw("same");
	c->SaveAs("outFigures/nTracks_generalTracks.png");

	//= offline nhit distribution ===================================================================
	// format_hist_dist(h_generalTracks_nHit);
	// h_generalTracks_nHit->Draw();

	// //legend
	// // auto legend3 = new TLegend(0.65, 0.7, 0.80, 0.9);
	// // legend3->SetTextSize(0.03);
	// // legend3->AddEntry(h_generalTracks_nHit, "generalTracks", "l");
	// // legend3->Draw("same");
	// c->SaveAs("outFigures/nHits_generalTracks.png");

	//= offline mass distribution ===================================================================
	h_generalTracks_mass->Draw();

	//legend
	// auto legend4 = new TLegend(0.65, 0.7, 0.80, 0.9);
	// legend4->SetTextSize(0.03);
	// legend4->AddEntry(h_generalTracks_mass, "generalTracks", "l");
	// legend4->Draw("same");
	c->SaveAs("outFigures/mass_generalTracks.png");

	delete c;
}

void pipeline_fill_tracks(TH1D *hTracks, TH1D* hHits, TH1D* hMass, int &mTrack, std::vector<int> *mHits, double &mMass)
{
	hTracks->Fill(mTrack);
	// for (int i = 0; i < mHits->size(); ++i)
	// {
	// 	hHits->Fill(mHits->at(i));
	// }
	hMass->Fill(mMass);
}

//# Main ###################################################################################################################
void plot_tracks_mass()
{
	// //= Read the Tree ================================================================================
	TFile *f = new TFile("inFiles/Tracks_mass_Jpsi_374345.root");

	TTree *myTree = (TTree*)f->Get("Tracks_mass");

	int m_generalTracks_nTracks;
	double m_generalTracks_mass;
	std::vector<int> *m_generalTracks_nHit = 0;

	myTree->SetBranchAddress("generalTracks_nTracks", &m_generalTracks_nTracks);
	// myTree->SetBranchAddress("generalTracks_nHit", &m_generalTracks_nHit);
	myTree->SetBranchAddress("generalTracks_mass", &m_generalTracks_mass);

	//= Loop over the Tree ===========================================================================
	int nentries = myTree->GetEntries();
	for (int i = 0; i < nentries; ++i)
	{
		myTree->GetEntry(i);

		pipeline_fill_tracks(h_generalTracks_nTrakcs, h_generalTracks_nHit, h_generalTracks_mass, m_generalTracks_nTracks, m_generalTracks_nHit, m_generalTracks_mass);
		pipeline_fill_tracks(h_globalMuons_nTrakcs, h_globalMuons_nHit, h_globalMuons_mass, m_generalTracks_nTracks, m_generalTracks_nHit, m_generalTracks_mass);
	}


	// //= save the histograms ==========================================================================
	// TFile *outfile = new TFile("outFiles/hist_mass.root", "RECREATE");
	// h_generalTracks_nTrakcs->Write("h_generalTracks_nTrakcs");
	// h_generalTracks_nHit->Write("h_generalTracks_nHit");
	// h_generalTracks_mass->Write("h_generalTracks_mass");
	// outfile->Close();
	// cout << "Histograms saved!" << endl;

	// //= read the histograms ==========================================================================
	// TFile *infile = new TFile("outFiles/hist_mass.root");
	// h_generalTracks_nTrakcs = (TH1D*)infile->Get("h_generalTracks_nTrakcs");
	// h_generalTracks_nHit = (TH1D*)infile->Get("h_generalTracks_nHit");
	// h_generalTracks_mass = (TH1D*)infile->Get("h_generalTracks_mass");


	//= fit the mass distribution with a crystal ball function within a jpis mass window =================
	TF1 *f1 = new TF1("f1", "crystalball", 2.85, 3.3);
	f1->SetParameters(20, 3.1, 0.1, 1, 1);
	TF1 *f2 = new TF1("f2", "pol2", 2.85, 3.3);
	f2->SetParNames("pol0", "pol1", "pol2");

	TF1 *func = new TF1("func", "f1+f2", 2.85, 3.3);

	// h_generalTracks_mass->Rebin(2);
	h_generalTracks_mass->Fit("func", "");

	//= Plotting ====================================================================================
	plot_1D();

}