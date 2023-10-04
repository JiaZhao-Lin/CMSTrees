#include "../VertexCompositeTree/VCTree_utilities.C"

//# Constants #############################################################################################################
std::vector<TString> trigger_name_track = 
{
	"HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v8",
	"HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v8",
	"HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v8"
};
const int nbin_track = 10;

//# Histograms ##############################################################################################################
std::vector<TH1D*> v_hNum_hltPixelTracksForUPCPPOnAA_nTracks;
std::vector<TH1D*> v_hDen_hltPixelTracksForUPCPPOnAA_nTracks;
std::vector<TH1D*> v_hNum_hltPixelTracksLowPtForUPCPPOnAA_nTracks;
std::vector<TH1D*> v_hDen_hltPixelTracksLowPtForUPCPPOnAA_nTracks;
std::vector<TH1D*> v_hNum_hltPixelTracksPPOnAA_nTracks;
std::vector<TH1D*> v_hDen_hltPixelTracksPPOnAA_nTracks;


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
	}
}

std::vector<TH1D*> cal_eff(std::vector<TH1D*> v_hNum, std::vector<TH1D*> v_hDen)
{
	std::vector<TH1D*> v_hEff;
	for (int i = 0; i < v_hNum.size(); ++i)
	{
		TH1D *hEff = (TH1D*)v_hNum.at(i)->Clone();
		hEff->Divide(v_hNum.at(i), v_hDen.at(i), 1, 1, "B");
		v_hEff.push_back(hEff);
	}
	return v_hEff;
}

void plot_eff(std::vector<TH1D*> v_hNum, std::vector<TH1D*> v_hDen, TString name)
{
	auto c = new TCanvas("c", "", 800, 600);
	auto v_hEff = cal_eff(v_hNum, v_hDen);

	std::vector<int> color = { 1, 2, 4 };

	for (int i = 1; i < v_hEff.size(); ++i)
	{
		v_hEff.at(i)->GetYaxis()->SetRangeUser(0, 1.1);
		v_hEff.at(i)->SetMarkerColor(color.at(i));
		v_hEff.at(i)->SetLineColor(color.at(i));
		v_hEff.at(i)->SetMarkerStyle(21);
		v_hEff.at(i)->Draw("same");
	}

	// legend
	auto legend = new TLegend(0.3, 0.3, 0.5, 0.5);
	for (int i = 1; i < v_hEff.size(); ++i)
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

//# Main ###################################################################################################################
void plot_eff_pixelTrack()
{
	//= Read the Tree ================================================================================
	TFile *f = new TFile("inFiles/efficiency.root");
	TTree *myTree = (TTree*)f->Get("efficiency");

	std::vector<int> *trigger_bit_track = 0;

	int m_hltPixelTracksForUPCPPOnAA_nTracks;
	int m_hltPixelTracksLowPtForUPCPPOnAA_nTracks;
	int m_hltPixelTracksPPOnAA_nTracks;
	int m_hltSiPixelClustersPPOnAA_nClusters;

	myTree->SetBranchAddress("trigger_bit_track", &trigger_bit_track);
	myTree->SetBranchAddress("m_hltPixelTracksForUPCPPOnAA_nTracks", &m_hltPixelTracksForUPCPPOnAA_nTracks);
	myTree->SetBranchAddress("m_hltPixelTracksLowPtForUPCPPOnAA_nTracks", &m_hltPixelTracksLowPtForUPCPPOnAA_nTracks);
	myTree->SetBranchAddress("m_hltPixelTracksPPOnAA_nTracks", &m_hltPixelTracksPPOnAA_nTracks);
	myTree->SetBranchAddress("m_hltSiPixelClustersPPOnAA_nClusters", &m_hltSiPixelClustersPPOnAA_nClusters);

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
			}
			v_hDen_hltPixelTracksForUPCPPOnAA_nTracks.at(j)->Fill(m_hltPixelTracksForUPCPPOnAA_nTracks);
			v_hDen_hltPixelTracksLowPtForUPCPPOnAA_nTracks.at(j)->Fill(m_hltPixelTracksLowPtForUPCPPOnAA_nTracks);
			v_hDen_hltPixelTracksPPOnAA_nTracks.at(j)->Fill(m_hltPixelTracksPPOnAA_nTracks);
		}
	}

	//= Plotting ====================================================================================
	// plot_eff(v_hNum_hltPixelTracksForUPCPPOnAA_nTracks, v_hDen_hltPixelTracksForUPCPPOnAA_nTracks);
	// plot_eff(v_hNum_hltPixelTracksLowPtForUPCPPOnAA_nTracks, v_hDen_hltPixelTracksLowPtForUPCPPOnAA_nTracks);

	
	plot_eff(v_hNum_hltPixelTracksPPOnAA_nTracks, v_hDen_hltPixelTracksPPOnAA_nTracks, "hEff_hltPixelTracksPPOnAA_nTracks");
	plot_eff(v_hNum_hltPixelTracksForUPCPPOnAA_nTracks, v_hDen_hltPixelTracksForUPCPPOnAA_nTracks, "hEff_hltPixelTracksForUPCPPOnAA_nTracks");
	plot_eff(v_hNum_hltPixelTracksLowPtForUPCPPOnAA_nTracks, v_hDen_hltPixelTracksLowPtForUPCPPOnAA_nTracks, "hEff_hltPixelTracksLowPtForUPCPPOnAA_nTracks");
}