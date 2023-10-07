#include "../VertexCompositeTree/VCTree_utilities.C"

//# Constants #############################################################################################################
std::vector<TString> trigger_name_track = 
{
	"HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v8",
	"HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v8",
	"HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v8"
};
const int nbin_track = 20;
const int nbin_cluster = 50;

//# Histograms ##############################################################################################################
std::vector<TH1D*> v_hNum_hltPixelTracksForUPCPPOnAA_nTracks;
std::vector<TH1D*> v_hDen_hltPixelTracksForUPCPPOnAA_nTracks;
std::vector<TH1D*> v_hNum_hltPixelTracksLowPtForUPCPPOnAA_nTracks;
std::vector<TH1D*> v_hDen_hltPixelTracksLowPtForUPCPPOnAA_nTracks;
std::vector<TH1D*> v_hNum_hltPixelTracksPPOnAA_nTracks;
std::vector<TH1D*> v_hDen_hltPixelTracksPPOnAA_nTracks;
std::vector<TH1D*> v_hNum_hltSiPixelClustersPPOnAA_nClusters;
std::vector<TH1D*> v_hDen_hltSiPixelClustersPPOnAA_nClusters;

std::vector<TH1D*> v_hNum_generalTracks_HP_nTracks;
std::vector<TH1D*> v_hDen_generalTracks_HP_nTracks;
std::vector<TH1D*> v_hNum_generalTracks_HP_nTracks_pT0p3;
std::vector<TH1D*> v_hDen_generalTracks_HP_nTracks_pT0p3;
std::vector<TH1D*> v_hNum_generalTracks_HP_nTracks_pT0p4;
std::vector<TH1D*> v_hDen_generalTracks_HP_nTracks_pT0p4;
std::vector<TH1D*> v_hNum_generalTracks_HP_nTracks_pT0p5;
std::vector<TH1D*> v_hDen_generalTracks_HP_nTracks_pT0p5;

TH1D* v_hNum_OR_generalTracks_HP_nTracks = new TH1D("v_hNum_OR_generalTracks_HP_nTracks", "v_hNum_OR_generalTracks_HP_nTracks; N_{track}^{generalTrkHP}", nbin_track, 0, nbin_track);
TH1D* v_hDen_OR_generalTracks_HP_nTracks = new TH1D("v_hDen_OR_generalTracks_HP_nTracks", "v_hDen_OR_generalTracks_HP_nTracks; N_{track}^{generalTrkHP}", nbin_track, 0, nbin_track);
TH1D* v_hNum_OR_generalTracks_HP_nTracks_pT0p3 = new TH1D("v_hNum_OR_generalTracks_HP_nTracks_pT0p3", "v_hNum_OR_generalTracks_HP_nTracks_pT0p3; N_{track}^{generalTrkHP}", nbin_track, 0, nbin_track);
TH1D* v_hDen_OR_generalTracks_HP_nTracks_pT0p3 = new TH1D("v_hDen_OR_generalTracks_HP_nTracks_pT0p3", "v_hDen_OR_generalTracks_HP_nTracks_pT0p3; N_{track}^{generalTrkHP}", nbin_track, 0, nbin_track);
TH1D* v_hNum_OR_generalTracks_HP_nTracks_pT0p4 = new TH1D("v_hNum_OR_generalTracks_HP_nTracks_pT0p4", "v_hNum_OR_generalTracks_HP_nTracks_pT0p4; N_{track}^{generalTrkHP}", nbin_track, 0, nbin_track);
TH1D* v_hDen_OR_generalTracks_HP_nTracks_pT0p4 = new TH1D("v_hDen_OR_generalTracks_HP_nTracks_pT0p4", "v_hDen_OR_generalTracks_HP_nTracks_pT0p4; N_{track}^{generalTrkHP}", nbin_track, 0, nbin_track);
TH1D* v_hNum_OR_generalTracks_HP_nTracks_pT0p5 = new TH1D("v_hNum_OR_generalTracks_HP_nTracks_pT0p5", "v_hNum_OR_generalTracks_HP_nTracks_pT0p5; N_{track}^{generalTrkHP}", nbin_track, 0, nbin_track);
TH1D* v_hDen_OR_generalTracks_HP_nTracks_pT0p5 = new TH1D("v_hDen_OR_generalTracks_HP_nTracks_pT0p5", "v_hDen_OR_generalTracks_HP_nTracks_pT0p5; N_{track}^{generalTrkHP}", nbin_track, 0, nbin_track);

int triggers_Idx_OR1 = 0;
int triggers_Idx_OR2 = 1;

TH1D * h_hltSiPixelClustersPPOnAA_nClusters = new TH1D("h_hltSiPixelClustersPPOnAA_nClusters", "h_hltSiPixelClustersPPOnAA_nClusters; N_{cluster}^{pixelClusterPPOnAA}", nbin_cluster, 1, 1000);

//# Functions ###############################################################################################################

void init_hist()
{
	for (int i = 0; i < trigger_name_track.size(); ++i)
	{
		v_hNum_hltPixelTracksForUPCPPOnAA_nTracks.push_back(new TH1D(Form("hNum_hltPixelTracksForUPCPPOnAA_nTracks_%s", trigger_name_track.at(i).Data()), Form("hNum_hltPixelTracksForUPCPPOnAA_nTracks_%s; N_{track}^{pixelTrkUPC}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		v_hDen_hltPixelTracksForUPCPPOnAA_nTracks.push_back(new TH1D(Form("hDen_hltPixelTracksForUPCPPOnAA_nTracks_%s", trigger_name_track.at(i).Data()), Form("hDen_hltPixelTracksForUPCPPOnAA_nTracks_%s; N_{track}^{pixelTrkUPC}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		v_hNum_hltPixelTracksLowPtForUPCPPOnAA_nTracks.push_back(new TH1D(Form("hNum_hltPixelTracksLowPtForUPCPPOnAA_nTracks_%s", trigger_name_track.at(i).Data()), Form("hNum_hltPixelTracksLowPtForUPCPPOnAA_nTracks_%s; N_{track}^{pixelTrkLowPt}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		v_hDen_hltPixelTracksLowPtForUPCPPOnAA_nTracks.push_back(new TH1D(Form("hDen_hltPixelTracksLowPtForUPCPPOnAA_nTracks_%s", trigger_name_track.at(i).Data()), Form("hDen_hltPixelTracksLowPtForUPCPPOnAA_nTracks_%s; N_{track}^{pixelTrkLowPt}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		v_hNum_hltPixelTracksPPOnAA_nTracks.push_back(new TH1D(Form("hNum_hltPixelTracksPPOnAA_nTracks_%s", trigger_name_track.at(i).Data()), Form("hNum_hltPixelTracksPPOnAA_nTracks_%s; N_{track}^{pixelTrkPPOnAA}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		v_hDen_hltPixelTracksPPOnAA_nTracks.push_back(new TH1D(Form("hDen_hltPixelTracksPPOnAA_nTracks_%s", trigger_name_track.at(i).Data()), Form("hDen_hltPixelTracksPPOnAA_nTracks_%s; N_{track}^{pixelTrkPPOnAA}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
	
		v_hNum_hltSiPixelClustersPPOnAA_nClusters.push_back(new TH1D(Form("hNum_hltSiPixelClustersPPOnAA_nClusters_%s", trigger_name_track.at(i).Data()), Form("hNum_hltSiPixelClustersPPOnAA_nClusters_%s; N_{cluster}^{pixelClusterPPOnAA}", trigger_name_track.at(i).Data()), nbin_cluster, 0, nbin_cluster));
		v_hDen_hltSiPixelClustersPPOnAA_nClusters.push_back(new TH1D(Form("hDen_hltSiPixelClustersPPOnAA_nClusters_%s", trigger_name_track.at(i).Data()), Form("hDen_hltSiPixelClustersPPOnAA_nClusters_%s; N_{cluster}^{pixelClusterPPOnAA}", trigger_name_track.at(i).Data()), nbin_cluster, 0, nbin_cluster));

		v_hNum_generalTracks_HP_nTracks.push_back(new TH1D(Form("hNum_generalTracks_HP_nTracks_%s", trigger_name_track.at(i).Data()), Form("hNum_generalTracks_HP_nTracks_%s; N_{track}^{generalTrkHP}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		v_hDen_generalTracks_HP_nTracks.push_back(new TH1D(Form("hDen_generalTracks_HP_nTracks_%s", trigger_name_track.at(i).Data()), Form("hDen_generalTracks_HP_nTracks_%s; N_{track}^{generalTrkHP}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		
		v_hNum_generalTracks_HP_nTracks_pT0p3.push_back(new TH1D(Form("hNum_generalTracks_HP_nTracks_pT0p3_%s", trigger_name_track.at(i).Data()), Form("hNum_generalTracks_HP_nTracks_pT0p3_%s; N_{track}^{generalTrkHP}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		v_hDen_generalTracks_HP_nTracks_pT0p3.push_back(new TH1D(Form("hDen_generalTracks_HP_nTracks_pT0p3_%s", trigger_name_track.at(i).Data()), Form("hDen_generalTracks_HP_nTracks_pT0p3_%s; N_{track}^{generalTrkHP}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		v_hNum_generalTracks_HP_nTracks_pT0p4.push_back(new TH1D(Form("hNum_generalTracks_HP_nTracks_pT0p4_%s", trigger_name_track.at(i).Data()), Form("hNum_generalTracks_HP_nTracks_pT0p4_%s; N_{track}^{generalTrkHP}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		v_hDen_generalTracks_HP_nTracks_pT0p4.push_back(new TH1D(Form("hDen_generalTracks_HP_nTracks_pT0p4_%s", trigger_name_track.at(i).Data()), Form("hDen_generalTracks_HP_nTracks_pT0p4_%s; N_{track}^{generalTrkHP}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		v_hNum_generalTracks_HP_nTracks_pT0p5.push_back(new TH1D(Form("hNum_generalTracks_HP_nTracks_pT0p5_%s", trigger_name_track.at(i).Data()), Form("hNum_generalTracks_HP_nTracks_pT0p5_%s; N_{track}^{generalTrkHP}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		v_hDen_generalTracks_HP_nTracks_pT0p5.push_back(new TH1D(Form("hDen_generalTracks_HP_nTracks_pT0p5_%s", trigger_name_track.at(i).Data()), Form("hDen_generalTracks_HP_nTracks_pT0p5_%s; N_{track}^{generalTrkHP}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
	}
}

std::vector<TH1D*> cal_eff(std::vector<TH1D*> v_hNum, std::vector<TH1D*> v_hDen)
{
	std::vector<TH1D*> v_hEff;
	for (int i = 0; i < v_hNum.size(); ++i)
	{
		TH1D *hEff = (TH1D*)v_hNum.at(i)->Clone();
		hEff->Divide(v_hNum.at(i), v_hDen.at(i), 1, 1, "B");
		// change the y-axis title
		hEff->GetYaxis()->SetTitle("Efficiency");
		v_hEff.push_back(hEff);
	}
	return v_hEff;
}

void format_hist_dist(TH1D* h)
{
	for (int i = 0; i < h->GetNbinsX(); ++i)
	{
		h->GetXaxis()->SetBinLabel(i + 1, Form("%d", i));
	}
}

void plot_eff(std::vector<TH1D*> v_hNum, std::vector<TH1D*> v_hDen, TString name)
{
	auto c = new TCanvas("c", "", 800, 600);
	auto v_hEff = cal_eff(v_hNum, v_hDen);

	std::vector<int> color = { 1, 2, 4 };

	for (int i = 0; i < trigger_name_track.size()-1; ++i)
	{
		v_hEff.at(i)->GetYaxis()->SetRangeUser(0, 1.1);
		v_hEff.at(i)->SetMarkerColor(color.at(i));
		v_hEff.at(i)->SetLineColor(color.at(i));
		v_hEff.at(i)->SetMarkerStyle(21);
		format_hist_dist(v_hEff.at(i));
		v_hEff.at(i)->Draw("same");
	}

	// legend
	auto legend = new TLegend(0.3, 0.3, 0.5, 0.5);
	for (int i = 0; i < trigger_name_track.size()-1; ++i)
	{
		legend->AddEntry(v_hEff.at(i), trigger_name_track.at(i).Data(), "lp");
	}
	legend->SetTextSize(0.025);
	legend->Draw("same");

	//draw a line at 1
	TLine *line = new TLine(v_hEff.at(0)->GetXaxis()->GetXmin(), 1, v_hEff.at(0)->GetXaxis()->GetXmax(), 1);
	line->SetLineColor(kRed);
	line->SetLineStyle(2);
	line->SetLineWidth(2);
	line->Draw("same");

	c->SaveAs(Form("outFigures/%s.png", name.Data()));
	delete c;
}

void plot_eff_comb(std::vector<TH1D*> v_hNum, std::vector<TH1D*> v_hDen, int index_1, int index_2, int color)
{
	auto c = new TCanvas("c", "", 800, 600);

	//combining the entries of the two histograms
	TH1D *hNum_comb = (TH1D*)v_hNum.at(index_1)->Clone();
	TH1D *hDen_comb = (TH1D*)v_hDen.at(index_1)->Clone();
	hNum_comb->Add(v_hNum.at(index_2));

	TH1D *hEff_comb = (TH1D*)hNum_comb->Clone();
	hEff_comb->Divide(hNum_comb, hDen_comb, 1, 1, "B");

	// hEff_comb->SetMarkerStyle(21);
	hEff_comb->SetMarkerColor(color);
	format_hist_dist(hEff_comb);
	hEff_comb->Draw("same");

	//draw a line at 1
	TLine *line = new TLine(hEff_comb->GetXaxis()->GetXmin(), 1, hEff_comb->GetXaxis()->GetXmax(), 1);
	line->SetLineColor(kRed);
	line->SetLineStyle(2);
	line->SetLineWidth(2);
	line->Draw("same");

	c->SaveAs(Form("outFigures/hEff_comb_tracking.png"));
}

TH1D *comb_eff(std::vector<TH1D*> v_hNum, std::vector<TH1D*> v_hDen, int index_1, int index_2)
{
	//combining the entries of the two histograms
	TH1D *hNum_comb = (TH1D*)v_hNum.at(index_1)->Clone();
	TH1D *hDen_comb = (TH1D*)v_hDen.at(index_1)->Clone();
	hNum_comb->Add(v_hNum.at(index_2));

	TH1D *hEff_comb = (TH1D*)hNum_comb->Clone();
	hEff_comb->Divide(hNum_comb, hDen_comb, 1, 1, "B");
	hEff_comb->SetMarkerStyle(24);
	// hEff_comb->SetMarkerSize(2);
	hEff_comb->GetYaxis()->SetRangeUser(0, 1.1);

	format_hist_dist(hEff_comb);

	return hEff_comb;
}

void plot_eff_OR(std::vector<TH1D*> v_hNum, std::vector<TH1D*> v_hDen, std::vector<TString> legend_name, TString name)
{
	auto c = new TCanvas("c", "", 800, 600);

	auto v_hEff = cal_eff(v_hNum, v_hDen);

	std::vector<int> color = { 1, 2, 4, 6 };
	
	for (int i = 0; i < v_hEff.size(); ++i)
	{
		v_hEff.at(i)->SetMarkerColor(color.at(i));
		v_hEff.at(i)->SetLineColor(color.at(i));
		v_hEff.at(i)->SetMarkerStyle(25);
		// v_hEff.at(i)->SetMarkerSize(1.5);
		v_hEff.at(i)->GetYaxis()->SetRangeUser(0, 1.1);
		format_hist_dist(v_hEff.at(i));
		v_hEff.at(i)->Draw("same");
	}

	// legend
	auto legend = new TLegend(0.3, 0.3, 0.5, 0.5);
	for (int i = 0; i < v_hEff.size(); ++i)
	{
		legend->AddEntry(v_hEff.at(i), legend_name.at(i).Data(), "lp");
	}
	legend->SetTextSize(0.025);
	legend->Draw("same");

	//draw a line at 1
	TLine *line = new TLine(v_hEff.at(0)->GetXaxis()->GetXmin(), 1, v_hEff.at(0)->GetXaxis()->GetXmax(), 1);
	line->SetLineColor(kRed);
	line->SetLineStyle(2);
	line->SetLineWidth(2);
	line->Draw("same");

	c->SaveAs(Form("outFigures/%s.png", name.Data()));
}

//# Main ###################################################################################################################
void plot_eff_allTrack()
{
	//= Read the Tree ================================================================================
	TFile *f = new TFile("inFiles/efficiency_UPCPixelValid_100_130.root");
	TTree *myTree = (TTree*)f->Get("efficiency");

	std::vector<int> *trigger_bit_track = 0;

	int m_hltPixelTracksForUPCPPOnAA_nTracks;
	int m_hltPixelTracksLowPtForUPCPPOnAA_nTracks;
	int m_hltPixelTracksPPOnAA_nTracks;
	int m_hltSiPixelClustersPPOnAA_nClusters;
	int m_generalTracks_HP_nTracks;
	int m_generalTracks_HP_nTracks_pT0p3;
	int m_generalTracks_HP_nTracks_pT0p4;
	int m_generalTracks_HP_nTracks_pT0p5;

	myTree->SetBranchAddress("trigger_bit_track", &trigger_bit_track);
	myTree->SetBranchAddress("m_hltPixelTracksForUPCPPOnAA_nTracks", &m_hltPixelTracksForUPCPPOnAA_nTracks);
	myTree->SetBranchAddress("m_hltPixelTracksLowPtForUPCPPOnAA_nTracks", &m_hltPixelTracksLowPtForUPCPPOnAA_nTracks);
	myTree->SetBranchAddress("m_hltPixelTracksPPOnAA_nTracks", &m_hltPixelTracksPPOnAA_nTracks);
	myTree->SetBranchAddress("m_hltSiPixelClustersPPOnAA_nClusters", &m_hltSiPixelClustersPPOnAA_nClusters);
	myTree->SetBranchAddress("m_generalTracks_HP_nTracks", &m_generalTracks_HP_nTracks);
	myTree->SetBranchAddress("m_generalTracks_HP_nTracks_pT0p3", &m_generalTracks_HP_nTracks_pT0p3);
	myTree->SetBranchAddress("m_generalTracks_HP_nTracks_pT0p4", &m_generalTracks_HP_nTracks_pT0p4);
	myTree->SetBranchAddress("m_generalTracks_HP_nTracks_pT0p5", &m_generalTracks_HP_nTracks_pT0p5);

	//= Initialize histograms =========================================================================
	init_hist();

	//= Loop over the Tree ===========================================================================
	int nentries = myTree->GetEntries();
	for (int i = 0; i < nentries; ++i)
	{
		myTree->GetEntry(i);

		for (int j = 0; j < trigger_bit_track->size(); ++j)
		{
			if (trigger_bit_track->at(j))
			{
				v_hNum_hltPixelTracksForUPCPPOnAA_nTracks.at(j)->Fill(m_hltPixelTracksForUPCPPOnAA_nTracks);
				v_hNum_hltPixelTracksLowPtForUPCPPOnAA_nTracks.at(j)->Fill(m_hltPixelTracksLowPtForUPCPPOnAA_nTracks);
				v_hNum_hltPixelTracksPPOnAA_nTracks.at(j)->Fill(m_hltPixelTracksPPOnAA_nTracks);
				v_hNum_hltSiPixelClustersPPOnAA_nClusters.at(j)->Fill(m_hltSiPixelClustersPPOnAA_nClusters);
				v_hNum_generalTracks_HP_nTracks.at(j)->Fill(m_generalTracks_HP_nTracks);
				v_hNum_generalTracks_HP_nTracks_pT0p3.at(j)->Fill(m_generalTracks_HP_nTracks_pT0p3);
				v_hNum_generalTracks_HP_nTracks_pT0p4.at(j)->Fill(m_generalTracks_HP_nTracks_pT0p4);
				v_hNum_generalTracks_HP_nTracks_pT0p5.at(j)->Fill(m_generalTracks_HP_nTracks_pT0p5);
			}
			v_hDen_hltPixelTracksForUPCPPOnAA_nTracks.at(j)->Fill(m_hltPixelTracksForUPCPPOnAA_nTracks);
			v_hDen_hltPixelTracksLowPtForUPCPPOnAA_nTracks.at(j)->Fill(m_hltPixelTracksLowPtForUPCPPOnAA_nTracks);
			v_hDen_hltPixelTracksPPOnAA_nTracks.at(j)->Fill(m_hltPixelTracksPPOnAA_nTracks);
			v_hDen_hltSiPixelClustersPPOnAA_nClusters.at(j)->Fill(m_hltSiPixelClustersPPOnAA_nClusters);
			v_hDen_generalTracks_HP_nTracks.at(j)->Fill(m_generalTracks_HP_nTracks);
			v_hDen_generalTracks_HP_nTracks_pT0p3.at(j)->Fill(m_generalTracks_HP_nTracks_pT0p3);
			v_hDen_generalTracks_HP_nTracks_pT0p4.at(j)->Fill(m_generalTracks_HP_nTracks_pT0p4);
			v_hDen_generalTracks_HP_nTracks_pT0p5.at(j)->Fill(m_generalTracks_HP_nTracks_pT0p5);
		}
		h_hltSiPixelClustersPPOnAA_nClusters->Fill(m_hltSiPixelClustersPPOnAA_nClusters);

		// OR
		if (trigger_bit_track->at(triggers_Idx_OR1) || trigger_bit_track->at(triggers_Idx_OR2))
		{
			v_hNum_OR_generalTracks_HP_nTracks->Fill(m_generalTracks_HP_nTracks);
			v_hNum_OR_generalTracks_HP_nTracks_pT0p3->Fill(m_generalTracks_HP_nTracks_pT0p3);
			v_hNum_OR_generalTracks_HP_nTracks_pT0p4->Fill(m_generalTracks_HP_nTracks_pT0p4);
			v_hNum_OR_generalTracks_HP_nTracks_pT0p5->Fill(m_generalTracks_HP_nTracks_pT0p5);
		}
		v_hDen_OR_generalTracks_HP_nTracks->Fill(m_generalTracks_HP_nTracks);
		v_hDen_OR_generalTracks_HP_nTracks_pT0p3->Fill(m_generalTracks_HP_nTracks_pT0p3);
		v_hDen_OR_generalTracks_HP_nTracks_pT0p4->Fill(m_generalTracks_HP_nTracks_pT0p4);
		v_hDen_OR_generalTracks_HP_nTracks_pT0p5->Fill(m_generalTracks_HP_nTracks_pT0p5);
	}

	//= Plotting ====================================================================================
	// plot_eff(v_hNum_hltPixelTracksForUPCPPOnAA_nTracks, v_hDen_hltPixelTracksForUPCPPOnAA_nTracks);
	// plot_eff(v_hNum_hltPixelTracksLowPtForUPCPPOnAA_nTracks, v_hDen_hltPixelTracksLowPtForUPCPPOnAA_nTracks);

	
	// plot_eff(v_hNum_hltPixelTracksPPOnAA_nTracks, v_hDen_hltPixelTracksPPOnAA_nTracks, "hEff_hltPixelTracksPPOnAA_nTracks");
	plot_eff(v_hNum_hltPixelTracksForUPCPPOnAA_nTracks, v_hDen_hltPixelTracksForUPCPPOnAA_nTracks, "hEff_hltPixelTracksForUPCPPOnAA_nTracks");
	plot_eff(v_hNum_hltPixelTracksLowPtForUPCPPOnAA_nTracks, v_hDen_hltPixelTracksLowPtForUPCPPOnAA_nTracks, "hEff_hltPixelTracksLowPtForUPCPPOnAA_nTracks");
	plot_eff(v_hNum_hltSiPixelClustersPPOnAA_nClusters, v_hDen_hltSiPixelClustersPPOnAA_nClusters, "hEff_hltSiPixelClustersPPOnAA_nClusters");
	plot_eff(v_hNum_generalTracks_HP_nTracks, v_hDen_generalTracks_HP_nTracks, "hEff_generalTracks_HP_nTracks");
	plot_eff(v_hNum_generalTracks_HP_nTracks_pT0p3, v_hDen_generalTracks_HP_nTracks_pT0p3, "hEff_generalTracks_HP_nTracks_pT0p3");
	plot_eff(v_hNum_generalTracks_HP_nTracks_pT0p4, v_hDen_generalTracks_HP_nTracks_pT0p4, "hEff_generalTracks_HP_nTracks_pT0p4");
	plot_eff(v_hNum_generalTracks_HP_nTracks_pT0p5, v_hDen_generalTracks_HP_nTracks_pT0p5, "hEff_generalTracks_HP_nTracks_pT0p5");
	
	plot_eff_OR( {v_hNum_OR_generalTracks_HP_nTracks, v_hNum_OR_generalTracks_HP_nTracks_pT0p3, v_hNum_OR_generalTracks_HP_nTracks_pT0p4, v_hNum_OR_generalTracks_HP_nTracks_pT0p5}, 
				{v_hDen_OR_generalTracks_HP_nTracks, v_hDen_OR_generalTracks_HP_nTracks_pT0p3, v_hDen_OR_generalTracks_HP_nTracks_pT0p4, v_hDen_OR_generalTracks_HP_nTracks_pT0p5}, 
				{"generalTracks_HP_nTracks", "generalTracks_HP_nTracks_pT0p3", "generalTracks_HP_nTracks_pT0p4", "generalTracks_HP_nTracks_pT0p5"},
				"hEff_OR_generalTracks_HP_nTracks");

	auto c = new TCanvas("c", "", 800, 600);
	//log scale
	c->SetLogy();
	c->SetLogx();
	h_hltSiPixelClustersPPOnAA_nClusters->Draw();
	c->SaveAs("outFigures/h_hltSiPixelClustersPPOnAA_nClusters.png");
}