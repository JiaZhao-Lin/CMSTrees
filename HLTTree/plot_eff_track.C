#include "../VertexCompositeTree/VCTree_utilities.C"
#include "./get_weight.C"

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

std::vector<TH2D*> v_hNum_nTracksMaxPt;
std::vector<TH2D*> v_hDen_nTracksMaxPt;

std::vector<TH1D*> v_hNum_nPixelTracks;
std::vector<TH1D*> v_hDen_nPixelTracks;

TH2D *hNum_nTracksMaxPt;
TH2D *hDen_nTracksMaxPt;

TH1D *hNum_nPixelTracks;
TH1D *hDen_nPixelTracks;

TH2D* hNum_OR_nTracksMaxPt;
TH2D* hDen_OR_nTracksMaxPt;

TH1D* hNum_OR_nPixelTracks;
TH1D* hDen_OR_nPixelTracks;

TH2D* hEtaPt = new TH2D("hEtaPt", "hEtaPt; #eta; p_{T}", 100, -2.5, 2.5, 100, 0, 1.0);

int triggers_Idx_OR1 = 0;
int triggers_Idx_OR2 = 1;


//# Functions ###############################################################################################################

void init_hist()
{
	for (int i = 0; i < trigger_name_track.size(); ++i)
	{
		v_hNum_nTracksMaxPt.push_back(new TH2D(Form("hNum_nTracksMaxPt_%s", trigger_name_track.at(i).Data()), Form("hNum_nTracksMaxPt_%s; N_{track}^{generalTrkHP}; p_{T}^{max}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track, 100, 0, 1));
		v_hDen_nTracksMaxPt.push_back(new TH2D(Form("hDen_nTracksMaxPt_%s", trigger_name_track.at(i).Data()), Form("hDen_nTracksMaxPt_%s; N_{track}^{generalTrkHP}; p_{T}^{max}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track, 100, 0, 1));
		v_hNum_nPixelTracks.push_back(new TH1D(Form("hNum_nPixelTracks_%s", trigger_name_track.at(i).Data()), Form("hNum_nPixelTracks_%s; N_{pixelTrack}^{hiConformal}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
		v_hDen_nPixelTracks.push_back(new TH1D(Form("hDen_nPixelTracks_%s", trigger_name_track.at(i).Data()), Form("hDen_nPixelTracks_%s; N_{pixelTrack}^{hiConformal}", trigger_name_track.at(i).Data()), nbin_track, 0, nbin_track));
	}

	hNum_OR_nTracksMaxPt = new TH2D("hNum_OR_nTracksMaxPt", "hNum_OR_nTracksMaxPt; N_{track}^{generalTrkHP}; p_{T}^{max}", nbin_track, 0, nbin_track, 100, 0, 1);
	hDen_OR_nTracksMaxPt = new TH2D("hDen_OR_nTracksMaxPt", "hDen_OR_nTracksMaxPt; N_{track}^{generalTrkHP}; p_{T}^{max}", nbin_track, 0, nbin_track, 100, 0, 1);
	hNum_OR_nPixelTracks = new TH1D("hNum_OR_nPixelTracks", "hNum_OR_nPixelTracks; N_{pixelTrack}^{hiConformal}", nbin_track, 0, nbin_track);
	hDen_OR_nPixelTracks = new TH1D("hDen_OR_nPixelTracks", "hDen_OR_nPixelTracks; N_{pixelTrack}^{hiConformal}", nbin_track, 0, nbin_track);
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

	c->SaveAs(Form("outFigures/eff/%s.png", name.Data()));
	delete c;
}

void plot_eff(std::vector<TH2D*> v_hNum, std::vector<TH2D*> v_hDen, double max_pt = -1, TString name = "eff")
{
	auto c = new TCanvas("c", "", 800, 600);

	std::vector<TH1D*> v_hNum_1D;
	std::vector<TH1D*> v_hDen_1D;

	if (max_pt == -1)
	{
		for (int i = 0; i < trigger_name_track.size(); ++i)
		{
			v_hNum_1D.push_back((TH1D*) v_hNum.at(i)->ProjectionX(Form("hNum_%s", trigger_name_track.at(i).Data()))->Clone());
			v_hDen_1D.push_back((TH1D*) v_hDen.at(i)->ProjectionX(Form("hDen_%s", trigger_name_track.at(i).Data()))->Clone());
		}
	}
	else
	{
		for (int i = 0; i < trigger_name_track.size(); ++i)
		{
			int bin_max_pt = v_hNum.at(i)->GetYaxis()->FindBin(max_pt);
			int total_bin = v_hNum.at(i)->GetNbinsX();

			v_hNum_1D.push_back((TH1D*) v_hNum.at(i)->ProjectionX(Form("hNum_%s", trigger_name_track.at(i).Data()), 1, bin_max_pt)->Clone());
			v_hDen_1D.push_back((TH1D*) v_hDen.at(i)->ProjectionX(Form("hDen_%s", trigger_name_track.at(i).Data()), 1, bin_max_pt)->Clone());
		}
	}

	auto v_hEff = cal_eff(v_hNum_1D, v_hDen_1D);

	std::vector<int> color = { 1, 2, 4 };

	for (int i = 0; i < trigger_name_track.size() - 1; ++i)
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
	for (int i = 0; i < trigger_name_track.size() - 1; ++i)
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

	//draw latex
	TLatex *latex = new TLatex();
	latex->SetNDC();
	latex->SetTextSize(0.03);
	latex->DrawLatex(0.4, 0.6, Form("N_{track}^{generalTrkHP} = 2, Eff = %.3f", v_hEff.at(1)->GetBinContent(3)));

	c->SaveAs(Form("outFigures/eff/%s.png", name.Data()));
	delete c;
}


// void plot_eff_comb(std::vector<TH1D*> v_hNum, std::vector<TH1D*> v_hDen, int index_1, int index_2, int color)
// {
// 	auto c = new TCanvas("c", "", 800, 600);

// 	//combining the entries of the two histograms
// 	TH1D *hNum_comb = (TH1D*)v_hNum.at(index_1)->Clone();
// 	TH1D *hDen_comb = (TH1D*)v_hDen.at(index_1)->Clone();
// 	hNum_comb->Add(v_hNum.at(index_2));

// 	TH1D *hEff_comb = (TH1D*)hNum_comb->Clone();
// 	hEff_comb->Divide(hNum_comb, hDen_comb, 1, 1, "B");

// 	// hEff_comb->SetMarkerStyle(21);
// 	hEff_comb->SetMarkerColor(color);
// 	format_hist_dist(hEff_comb);
// 	hEff_comb->Draw("same");

// 	//draw a line at 1
// 	TLine *line = new TLine(hEff_comb->GetXaxis()->GetXmin(), 1, hEff_comb->GetXaxis()->GetXmax(), 1);
// 	line->SetLineColor(kRed);
// 	line->SetLineStyle(2);
// 	line->SetLineWidth(2);
// 	line->Draw("same");

// 	c->SaveAs(Form("outFigures/eff/hEff_comb_tracking.png"));
// }

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

	c->SaveAs(Form("outFigures/eff/%s.png", name.Data()));
}

void plot_eff_OR(TH2D* hNum, TH2D* hDen, double max_pt = -1, TString name = "eff")
{
	auto c = new TCanvas("c", "", 800, 600);

	std::vector<TH1D*> v_hNum_1D;
	std::vector<TH1D*> v_hDen_1D;

	if (max_pt == -1)
	{
		v_hNum_1D.push_back((TH1D*) hNum->ProjectionX(Form("hNum_OR"))->Clone());
		v_hDen_1D.push_back((TH1D*) hDen->ProjectionX(Form("hDen_OR"))->Clone());
	}
	else
	{
		int bin_max_pt = hNum->GetYaxis()->FindBin(max_pt);
		int total_bin = hNum->GetNbinsX();

		v_hNum_1D.push_back((TH1D*) hNum->ProjectionX(Form("hNum_OR"), 1, bin_max_pt)->Clone());
		v_hDen_1D.push_back((TH1D*) hDen->ProjectionX(Form("hDen_OR"), 1, bin_max_pt)->Clone());
	}

	auto v_hEff = cal_eff(v_hNum_1D, v_hDen_1D);

	std::vector<int> color = { 1, 2, 4 };

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
	legend->AddEntry(v_hEff.at(0), "OR", "lp");
	legend->SetTextSize(0.025);
	legend->Draw("same");

	//draw a line at 1
	TLine *line = new TLine(v_hEff.at(0)->GetXaxis()->GetXmin(), 1, v_hEff.at(0)->GetXaxis()->GetXmax(), 1);
	line->SetLineColor(kRed);
	line->SetLineStyle(2);
	line->SetLineWidth(2);
	line->Draw("same");

	//draw latex
	TLatex *latex = new TLatex();
	latex->SetNDC();
	latex->SetTextSize(0.03);
	latex->DrawLatex(0.4, 0.6, Form("N_{track}^{generalTrkHP} = 2, Eff = %.3f", v_hEff.at(0)->GetBinContent(3)));

	c->SaveAs(Form("outFigures/eff/%s.png", name.Data()));
	delete c;
}

//# Main ###################################################################################################################
void plot_eff_track()
{
	//= Read the Tree ================================================================================
	// TFile *f = new TFile("inFiles/eff/efficiency_mc_NgeneralTrk.root");
	TFile *f = new TFile("inFiles/eff/efficiency_ZB.root");
	// TFile *f = new TFile("inFiles/eff/efficiency_mc.root");
	// TFile *f = new TFile("inFiles/eff/efficiency_mc_doublediff.root");
	TTree *myTree = (TTree*)f->Get("efficiency");

	std::vector<int> *trigger_bit_track = 0;

	double m_generalTracks_pT_max;
	int m_generalTracks_HP_nTracks;
	int m_hiConformalPixelTracks_nTracks;
	
	std::vector<double> *m_generalTracks_pT = 0;
	std::vector<double> *m_generalTracks_eta = 0;


	myTree->SetBranchAddress("trigger_bit_track", &trigger_bit_track);
	myTree->SetBranchAddress("m_generalTracks_pT_max", &m_generalTracks_pT_max);
	myTree->SetBranchAddress("m_generalTracks_HP_nTracks", &m_generalTracks_HP_nTracks);
	myTree->SetBranchAddress("m_hiConformalPixelTracks_nTracks", &m_hiConformalPixelTracks_nTracks);
	myTree->SetBranchAddress("m_generalTracks_pT", &m_generalTracks_pT);
	myTree->SetBranchAddress("m_generalTracks_eta", &m_generalTracks_eta);

	//= Initialize histograms =========================================================================
	init_hist();

	//= Loop over the Tree ===========================================================================
	int nentries = myTree->GetEntries();
	for (int i = 0; i < nentries; ++i)
	{
		myTree->GetEntry(i);
		// cout << "Entry: " << i << "pT: " << m_generalTracks_pT_max << endl;

		// vector<double> v_abs_eta;
		// for (int j = 0; j < m_generalTracks_eta->size(); ++j)
		// {
		// 	double eta = m_generalTracks_eta->at(j);
		// 	v_abs_eta.push_back(abs(eta));
		// }
		// if (v_abs_eta.size() == 0) continue;
		// double max_pt = *std::max_element(m_generalTracks_pT->begin(), m_generalTracks_pT->end());
		// double min_pt = *std::min_element(m_generalTracks_pT->begin(), m_generalTracks_pT->end());
		// double max_eta = *std::max_element(std::begin(v_abs_eta), std::end(v_abs_eta));
		// double min_eta = *std::min_element(std::begin(v_abs_eta), std::end(v_abs_eta));

		// // cout << max_pt << " " << m_generalTracks_pT_max << endl;
		// if (max_eta > 2.4 || min_eta<1.6) continue;
		// if (max_pt > 0.20) continue;

		double weight = get_weight(*m_generalTracks_eta, *m_generalTracks_pT);
		// double weight = 1.0;

		for (int j = 0; j < trigger_bit_track->size(); ++j)
		{
			if (trigger_bit_track->at(j))
			{
				v_hNum_nTracksMaxPt.at(j)->Fill(m_generalTracks_HP_nTracks, m_generalTracks_pT_max, weight);

				v_hNum_nPixelTracks.at(j)->Fill(m_hiConformalPixelTracks_nTracks, weight);
			}
			v_hDen_nTracksMaxPt.at(j)->Fill(m_generalTracks_HP_nTracks, m_generalTracks_pT_max, weight);
			
			v_hDen_nPixelTracks.at(j)->Fill(m_hiConformalPixelTracks_nTracks, weight);

		}

		// OR
		if (trigger_bit_track->at(triggers_Idx_OR1) || trigger_bit_track->at(triggers_Idx_OR2))
		{
			hNum_OR_nTracksMaxPt->Fill(m_generalTracks_HP_nTracks, m_generalTracks_pT_max, weight);
			hNum_OR_nPixelTracks->Fill(m_hiConformalPixelTracks_nTracks, weight);
		}
		hDen_OR_nTracksMaxPt->Fill(m_generalTracks_HP_nTracks, m_generalTracks_pT_max, weight);
		hDen_OR_nPixelTracks->Fill(m_hiConformalPixelTracks_nTracks, weight);

		for (int j = 0; j < m_generalTracks_pT->size(); ++j)
		{
			double eta = m_generalTracks_eta->at(j);
			double pt = m_generalTracks_pT->at(j);
			// double w = get_weight(eta, pt);
			// double w = 1.0;
			hEtaPt->Fill( eta, pt, weight);
		}
	}

	//= Plotting ====================================================================================
	// plot_eff(v_hNum_nTracksMaxPt, v_hDen_nTracksMaxPt, 0.1, "hEff_nTracksMaxPt_0p1");
	// plot_eff(v_hNum_nTracksMaxPt, v_hDen_nTracksMaxPt, 0.2, "hEff_nTracksMaxPt_0p2");
	// plot_eff(v_hNum_nTracksMaxPt, v_hDen_nTracksMaxPt, 0.3, "hEff_nTracksMaxPt_0p3");
	// plot_eff(v_hNum_nTracksMaxPt, v_hDen_nTracksMaxPt, 0.4, "hEff_nTracksMaxPt_0p4");
	plot_eff(v_hNum_nTracksMaxPt, v_hDen_nTracksMaxPt, -1, "hEff_nTracksMaxPt_weight");

	plot_eff_OR(hNum_OR_nTracksMaxPt, hDen_OR_nTracksMaxPt, -1, "hEff_OR_nTracksMaxPt_weight");

	plot_eff(v_hNum_nPixelTracks, v_hDen_nPixelTracks, "hEff_nPixelTracks_weight");
	plot_eff_OR({hNum_OR_nPixelTracks}, {hDen_OR_nPixelTracks}, {"OR"}, "hEff_OR_nPixelTracks_weight");

	auto c = new TCanvas("cc", "", 800, 600);
	c->SetRightMargin(0.12);
	hEtaPt->Draw("colz");
	//set right margin
	// gStyle->SetPadRightMargin(0.15);
	c->SetLogz();
	c->SaveAs("outFigures/eff/hEtaPt_weight.png");
}