#include "../VertexCompositeTree/VCTree_utilities.C"

//# Constants #############################################################################################################
const int nbin_track = 5;
const int nbin_hit = 40;
const int nbin_mass = 100;


//# Histograms ##############################################################################################################
TH1D *h_packedPFCandidates_nTrakcs = new TH1D("h_packedPFCandidates_nTrakcs", "h_packedPFCandidates_nTrakcs;N_{track}^{PFCandidates}", nbin_track, 0, nbin_track);
TH1D *h_packedPFCandidates_nHits = new TH1D("h_packedPFCandidates_nHits", "h_packedPFCandidates_nHits;N_{hit}^{PFCandidates}", nbin_hit, 0, nbin_hit);
TH1D *h_packedPFCandidates_mass = new TH1D("h_packedPFCandidates_mass", "h_packedPFCandidates_mass;Mass [GeV]", nbin_mass, 2.2, 5);

TH2D *h_packedPFCandidates_EtaVsPt = new TH2D("h_packedPFCandidates_EtaVsPt", "h_packedPFCandidates_EtaVsPt;#eta;p_{T} [GeV]", 30, -2.6, 2.6, 30, 0, 5);

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
	format_hist_dist(h_packedPFCandidates_nTrakcs);
	h_packedPFCandidates_nTrakcs->Draw();

	//legend
	// auto legend2 = new TLegend(0.65, 0.7, 0.80, 0.9);
	// legend2->SetTextSize(0.03);
	// legend2->AddEntry(h_packedPFCandidates, "packedPFCandidates", "l");
	// legend2->Draw("same");
	c->SaveAs("outFigures/nTracks_packedPFCandidates.png");

	//= offline nhit distribution ===================================================================
	format_hist_dist(h_packedPFCandidates_nHits);
	h_packedPFCandidates_nHits->Draw();

	// //legend
	// // auto legend3 = new TLegend(0.65, 0.7, 0.80, 0.9);
	// // legend3->SetTextSize(0.03);
	// // legend3->AddEntry(h_packedPFCandidates_nHits, "packedPFCandidates", "l");
	// // legend3->Draw("same");
	c->SaveAs("outFigures/nHits_packedPFCandidates.png");

	//= offline mass distribution ===================================================================
	h_packedPFCandidates_mass->Draw();

	//legend
	// auto legend4 = new TLegend(0.65, 0.7, 0.80, 0.9);
	// legend4->SetTextSize(0.03);
	// legend4->AddEntry(h_packedPFCandidates_mass, "packedPFCandidates", "l");
	// legend4->Draw("same");
	c->SaveAs("outFigures/mass_packedPFCandidates.png");

	//= offline Pt vs Eta distribution ================================================================
	h_packedPFCandidates_EtaVsPt->Draw("colz");
	//set right margin
	gPad->SetRightMargin(0.12);

	//legend
	// auto legend5 = new TLegend(0.65, 0.7, 0.80, 0.9);
	// legend5->SetTextSize(0.03);
	// legend5->AddEntry(h_packedPFCandidates_EtaVsPt, "packedPFCandidates", "l");
	// legend5->Draw("same");
	c->SaveAs("outFigures/EtaVsPt_packedPFCandidates.png");

	delete c;
}


//# Main ###################################################################################################################
void plot_tracks_mass_miniAOD()
{
	// //= Read the Tree ================================================================================
	TFile *f = new TFile("inFiles/Tracks_mass_miniAOD_Jpsi_374354.root");

	TTree *myTree = (TTree*)f->Get("Tracks_mass");

	int m_packedPFCandidates_nTracks;
	double m_packedPFCandidates_mass;
	double m_packedPFCandidates_mu_pt;
	double m_packedPFCandidates_mu_eta;
	double m_packedPFCandidates_mu_phi;
	std::vector<int> *m_packedPFCandidates_mu_charge = 0;
	std::vector<int> *m_packedPFCandidates_nHits = 0;

	myTree->SetBranchAddress("packedPFCandidates_nTracks", &m_packedPFCandidates_nTracks);
	myTree->SetBranchAddress("packedPFCandidates_nHits", &m_packedPFCandidates_nHits);
	myTree->SetBranchAddress("packedPFCandidates_mass", &m_packedPFCandidates_mass);
	myTree->SetBranchAddress("packedPFCandidates_mu_pt", &m_packedPFCandidates_mu_pt);
	myTree->SetBranchAddress("packedPFCandidates_mu_eta", &m_packedPFCandidates_mu_eta);
	myTree->SetBranchAddress("packedPFCandidates_mu_phi", &m_packedPFCandidates_mu_phi);
	myTree->SetBranchAddress("packedPFCandidates_mu_charge", &m_packedPFCandidates_mu_charge);

	//= Loop over the Tree ===========================================================================
	int nentries = myTree->GetEntries();
	for (int i = 0; i < nentries; ++i)
	{
		myTree->GetEntry(i);

		//skip incorrect muon charge
		if (m_packedPFCandidates_mu_charge->at(0) == m_packedPFCandidates_mu_charge->at(1)) continue;

		h_packedPFCandidates_nTrakcs->Fill(m_packedPFCandidates_nTracks);
		for (int i = 0; i < m_packedPFCandidates_nHits->size(); ++i)
		{
			h_packedPFCandidates_nHits->Fill(m_packedPFCandidates_nHits->at(i));
		}
		h_packedPFCandidates_mass->Fill(m_packedPFCandidates_mass);
		h_packedPFCandidates_EtaVsPt->Fill(m_packedPFCandidates_mu_eta, m_packedPFCandidates_mu_pt);
	}


	// //= save the histograms ==========================================================================
	// TFile *outfile = new TFile("outFiles/hist_mass.root", "RECREATE");
	// h_packedPFCandidates_nTrakcs->Write("h_packedPFCandidates_nTrakcs");
	// h_packedPFCandidates_nHit->Write("h_packedPFCandidates_nHit");
	// h_packedPFCandidates_mass->Write("h_packedPFCandidates_mass");
	// outfile->Close();
	// cout << "Histograms saved!" << endl;

	// //= read the histograms ==========================================================================
	// TFile *infile = new TFile("outFiles/hist_mass.root");
	// h_packedPFCandidates_nTrakcs = (TH1D*)infile->Get("h_packedPFCandidates_nTrakcs");
	// h_packedPFCandidates_nHit = (TH1D*)infile->Get("h_packedPFCandidates_nHit");
	// h_packedPFCandidates_mass = (TH1D*)infile->Get("h_packedPFCandidates_mass");


	//= fit the mass distribution with a crystal ball function within a jpis mass window =================
	TF1 *f1 = new TF1("f1", "crystalball", 2.85, 3.3);
	f1->SetParameters(20, 3.1, 0.1, 1, 1);
	TF1 *f2 = new TF1("f2", "pol2", 2.85, 3.3);
	f2->SetParNames("pol0", "pol1", "pol2");

	TF1 *func = new TF1("func", "f1+f2", 2.85, 3.3);

	// h_packedPFCandidates_mass->Rebin(2);
	h_packedPFCandidates_mass->Fit("func", "");

	//= Plotting ====================================================================================
	plot_1D();

}