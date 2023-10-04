#include "../VertexCompositeTree/VCTree_utilities.C"

//# Constants #############################################################################################################
std::vector<TString> trigger_name_mu = 
{
	"HLT_HIUPC_SingleMuOpen_NotMBHF2AND_v8"
};

std::vector<TString> trigger_name_track = 
{
	// "HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v8",
	"HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v8",
	"HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v8"
};
const int nbin_track = 5;
const int nbin_hit = 30;

const int nbin_pt = 30;
const double min_pt = 0;
const double max_pt = 5;
const int nbin_eta = 30;
const double min_eta = -2.6;
const double max_eta = 2.6;
const int nbin_phi = 30;
const double min_phi = -3.2;
const double max_phi = 3.2;

//# Histograms ##############################################################################################################
std::vector<TH3D*> v_hNum_PtEtaPhi_globalMuons;
std::vector<TH3D*> v_hDen_PtEtaPhi_globalMuons;

TH1D * hNum_generalTracks_nTracks = new TH1D("hNum_generalTracks_nTracks", "hNum_generalTracks_nTracks; N_{track}^{generalTrk}", nbin_track, 0, nbin_track);
TH1D * hDen_generalTracks_nTracks = new TH1D("hDen_generalTracks_nTracks", "hDen_generalTracks_nTracks; N_{track}^{generalTrk}", nbin_track, 0, nbin_track);

//# Functions ###############################################################################################################

void init_hist()
{
	for (int i = 0; i < trigger_name_mu.size(); ++i)
	{
		v_hNum_PtEtaPhi_globalMuons.push_back(new TH3D(Form("hNum_PtEtaPhi_globalMuons_%s", trigger_name_mu.at(i).Data()), Form("hNum_PtEtaPhi_globalMuons_%s; p_{T} [GeV]; #eta; #phi", trigger_name_mu.at(i).Data()), nbin_pt, min_pt, max_pt, nbin_eta, min_eta, max_eta, nbin_phi, min_phi, max_phi));
		v_hDen_PtEtaPhi_globalMuons.push_back(new TH3D(Form("hDen_PtEtaPhi_globalMuons_%s", trigger_name_mu.at(i).Data()), Form("hDen_PtEtaPhi_globalMuons_%s; p_{T} [GeV]; #eta; #phi", trigger_name_mu.at(i).Data()), nbin_pt, min_pt, max_pt, nbin_eta, min_eta, max_eta, nbin_phi, min_phi, max_phi));
	}
}

std::vector<TH1D*> cal_eff(std::vector<TH3D*> v_hNum, std::vector<TH3D*> v_hDen, TString projection)
{
	std::vector<TH1D*> v_hEff;
	for (int i = 0; i < v_hNum.size(); ++i)
	{
		TH1D *hNum = (TH1D*)v_hNum.at(i)->Project3D(projection);
		TH1D *hDen = (TH1D*)v_hDen.at(i)->Project3D(projection);
		TH1D *hEff = (TH1D*)hNum->Clone();
		hEff->Divide(hNum, hDen, 1, 1, "B");
		v_hEff.push_back(hEff);
	}
	return v_hEff;
}

std::vector<TH2D*> cal_eff2D(std::vector<TH3D*> v_hNum, std::vector<TH3D*> v_hDen, TString projection)
{
	std::vector<TH2D*> v_hEff;
	for (int i = 0; i < v_hNum.size(); ++i)
	{
		TH2D *hNum = (TH2D*)v_hNum.at(i)->Project3D(projection);
		TH2D *hDen = (TH2D*)v_hDen.at(i)->Project3D(projection);
		TH2D *hEff = (TH2D*)hNum->Clone(Form("hEff_%s_%s", projection.Data(), trigger_name_mu.at(i).Data()));
		hEff->Divide(hNum, hDen, 1, 1, "B");
		v_hEff.push_back(hEff);
	}
	return v_hEff;
}



// void plot_hist2D(TH2D* h2)
// {
// 	//= hltPixelTracksPPOnAA_Vs_generalTracks ===============================================================
// 	auto c = new TCanvas("c", "", 800, 600);
// 	format_hist_dist2D(h2, nbin_track);
// 	h2->Draw("colz");
// 	// offset the graph a little bit
// 	// gPad->SetLeftMargin(0.15);
// 	gPad->SetRightMargin(0.13);
// 	gPad->SetBottomMargin(0.13);

// 	//draw percentage on each bin
// 	for (int i = 0; i < h2->GetNbinsX(); ++i)
// 	{
// 		for (int j = 0; j < h2->GetNbinsY(); ++j)
// 		{
// 			draw_latex(0.14 + i*0.15, 0.17 + j*0.16, Form("%.2f %%", h2->GetBinContent(i+1, j+1)/h2->GetEntries()*100.0));
// 		}
// 	}
// 	c->SaveAs(Form("outFigures/%s.png", h2->GetTitle()));
// 	delete c;
// }

void plot_eff(std::vector<TH3D*> v_hNum, std::vector<TH3D*> v_hDen, TString projection)
{
	auto c = new TCanvas("c", "", 800, 600);
	
	std::vector<TH1D*> v_hEff = cal_eff(v_hNum, v_hDen, projection);

	for (int i = 0; i < v_hEff.size(); ++i)
	{
		v_hEff.at(i)->Draw();
		c->SaveAs(Form("outFigures/hEff_%d.png", i));
	}
}

void pipeline_fill_eff_mu(std::vector<int> *trigger_bit_mu, std::vector<double> *m_pt = 0, std::vector<double> *m_eta = 0, std::vector<double> *m_phi = 0, std::vector<int> *m_charge = 0)
{
	for (int i = 0; i < trigger_bit_mu->size(); ++i)
	{
		for (int j = 0; j < m_pt->size(); ++j)
		{
			if (trigger_bit_mu->at(i) == 1)
			{
				v_hNum_PtEtaPhi_globalMuons.at(i)->Fill(m_pt->at(j), m_eta->at(j), m_phi->at(j));
			}
			v_hDen_PtEtaPhi_globalMuons.at(i)->Fill(m_pt->at(j), m_eta->at(j), m_phi->at(j));
		}
	}
}

//# Main ###################################################################################################################
void plot_eff()
{
	//= Read the Tree ================================================================================
	TFile *f = new TFile("inFiles/efficiency.root");
	TTree *myTree = (TTree*)f->Get("efficiency");

	std::vector<int> *trigger_bit_mu = 0;
	std::vector<int> *trigger_bit_track = 0;

	std::vector<double> *m_standAloneMuons_pt = 0;
	std::vector<double> *m_standAloneMuons_eta = 0;
	std::vector<double> *m_standAloneMuons_phi = 0;
	std::vector<int> *m_standAloneMuons_charge = 0;

	std::vector<double> *m_globalMuons_pt = 0;
	std::vector<double> *m_globalMuons_eta = 0;
	std::vector<double> *m_globalMuons_phi = 0;
	std::vector<int> *m_globalMuons_charge = 0;

	std::vector<double> *m_refittedStandAloneMuons_pt = 0;
	std::vector<double> *m_refittedStandAloneMuons_eta = 0;
	std::vector<double> *m_refittedStandAloneMuons_phi = 0;
	std::vector<int> *m_refittedStandAloneMuons_charge = 0;

	int m_generalTracks_nTracks;

	myTree->SetBranchAddress("trigger_bit_mu", &trigger_bit_mu);
	myTree->SetBranchAddress("standAloneMuons_pt", &m_standAloneMuons_pt);
	myTree->SetBranchAddress("standAloneMuons_eta", &m_standAloneMuons_eta);
	myTree->SetBranchAddress("standAloneMuons_phi", &m_standAloneMuons_phi);
	myTree->SetBranchAddress("standAloneMuons_charge", &m_standAloneMuons_charge);

	myTree->SetBranchAddress("globalMuons_pt", &m_globalMuons_pt);
	myTree->SetBranchAddress("globalMuons_eta", &m_globalMuons_eta);
	myTree->SetBranchAddress("globalMuons_phi", &m_globalMuons_phi);
	myTree->SetBranchAddress("globalMuons_charge", &m_globalMuons_charge);

	myTree->SetBranchAddress("refittedStandAloneMuons_pt", &m_refittedStandAloneMuons_pt);
	myTree->SetBranchAddress("refittedStandAloneMuons_eta", &m_refittedStandAloneMuons_eta);
	myTree->SetBranchAddress("refittedStandAloneMuons_phi", &m_refittedStandAloneMuons_phi);
	myTree->SetBranchAddress("refittedStandAloneMuons_charge", &m_refittedStandAloneMuons_charge);

	myTree->SetBranchAddress("trigger_bit_track", &trigger_bit_track);
	myTree->SetBranchAddress("generalTracks_nTracks", &m_generalTracks_nTracks);

	//= Initialize histograms =========================================================================
	init_hist();

	//= Loop over the Tree ===========================================================================
	int nentries = myTree->GetEntries();
	for (int i = 0; i < nentries; ++i)
	{
		myTree->GetEntry(i);

		pipeline_fill_eff_mu(trigger_bit_mu, m_globalMuons_pt, m_globalMuons_eta, m_globalMuons_phi, m_globalMuons_charge);
		if (trigger_bit_track->at(0)) cout << "trigger_bit_track->at(1) = " << trigger_bit_track->at(1) << endl;
		if (trigger_bit_track->at(0))
		{
			hNum_generalTracks_nTracks->Fill(m_generalTracks_nTracks);
		}
		hDen_generalTracks_nTracks->Fill(m_generalTracks_nTracks);

	}

	//= Plotting ====================================================================================
	// plot_eff(v_hNum_PtEtaPhi_globalMuons, v_hDen_PtEtaPhi_globalMuons, "x");

	auto c = new TCanvas("c", "", 800, 600);
	TH1D *hEff = (TH1D*) hNum_generalTracks_nTracks->Clone();
	hEff->Divide(hNum_generalTracks_nTracks, hDen_generalTracks_nTracks, 1, 1, "B");
	// hDen_generalTracks_nTracks->Draw();
	hNum_generalTracks_nTracks->Draw("same");
	hNum_generalTracks_nTracks->SetLineColor(kRed);
	c->SaveAs("outFigures/h_generalTracks_nTracks.png");

}