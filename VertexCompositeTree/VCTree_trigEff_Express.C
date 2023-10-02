/*
	Use to plot online express data trigger efficiency
	Should use looser cuts when skimming the RECO data when producing VC tree

	JiaZhao Lin, Sep 2023
*/

#include "./VCTree_routines.C"

//# Global Variables #######################################################################################################
const int triggers_Idx = 0;

const int nbin_pt = 50;
const double min_pt = 0;
const double max_pt = 5;
const int nbin_eta = 50;
const double min_eta = -3;
const double max_eta = 3;
const int nbin_phi = 50;
const double min_phi = -3.2;
const double max_phi = 3.2;
const int nbin_mass = 100;
const double min_mass = 0;
const double max_mass = 5;
const int nbin_rap = 50;
const double min_rap = -2.6;
const double max_rap = 2.6;

//# Histograms ##############################################################################################################
TH3D *hNum_PtEtaPhi = new TH3D("hNum_PtEtaPhi", "hNum_PtEtaPhi; p_{T} [GeV]; #eta; #phi", nbin_pt, min_pt, max_pt, nbin_eta, min_eta, max_eta, nbin_phi, min_phi, max_phi);
TH3D *hDen_PtEtaPhi = new TH3D("hDen_PtEtaPhi", "hDen_PtEtaPhi; p_{T} [GeV]; #eta; #phi", nbin_pt, min_pt, max_pt, nbin_eta, min_eta, max_eta, nbin_phi, min_phi, max_phi);

TH3D *hPtRapMass = new TH3D("hPtRapMass", "hPtRapMass; p_{T} [GeV]; y; Mass [GeV]", nbin_pt, min_pt, max_pt, nbin_rap, min_rap, max_rap, nbin_mass, min_mass, max_mass);
TH3D *hPtRapMass_trig = new TH3D("hPtRapMass_trig", "hPtRapMass_trig; p_{T} [GeV]; y; Mass [GeV]", nbin_pt, min_pt, max_pt, nbin_rap, min_rap, max_rap, nbin_mass, min_mass, max_mass);

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
	TF1 *f1 = new TF1("f1", "crystalball", 2.85, 3.3);
	f1->SetParameters(20, 3.1, 0.1, 1, 1);
	TF1 *f2 = new TF1("f2", "pol2", 2.85, 3.3);
	f2->SetParNames("pol0", "pol1", "pol2");

	TF1 *func = new TF1("func", "f1+f2", 2.85, 3.3);

	h1->Fit("func", "");
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
	auto legend = new TLegend(0.40, 0.3, 0.60, 0.5);
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

	h1_x->Draw();
	c->SaveAs(Form("outFigures/%s_x.png", h3->GetName()));

	h1_y->Draw();
	c->SaveAs(Form("outFigures/%s_y.png", h3->GetName()));

	h1_z->Draw();
	c->SaveAs(Form("outFigures/%s_z.png", h3->GetName()));

	delete c;
}

//# Main ####################################################################################################################
void VCTree_trigEff_Express()
{
	// TH1::SetDefaultSumw2(kTRUE);
	// std::string inputFile = "inFiles/2023/VCTree_STARLIGHT_CohJpsiToMuMu_5p36TeV_Run3.root";
	std::string inputFile = "inFiles/2023/VCTree_Express_374345.root";
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
		// Bool_t goodHFVeto 			= (!csTree.evtSel()[16] && !csTree.evtSel()[17]);
		Bool_t passNTrkHP 			= csTree.NtrkHP() == 2;	// contain only two high-purity tracks and nothing else
		// Bool_t passEvtSel 			= goodVtx && goodHFVeto && passNTrkHP;
		// Bool_t passNRecoCandidate 	= csTree.candSize() > 0;

		// if (goodVtx)				hnEvts->Fill(1.5);
		// if (goodVtx && goodHFVeto)	hnEvts->Fill(2.5);
		// if (passEvtSel) 			hnEvts->Fill(3.5);
		// if (passNRecoCandidate)		hnEvts->Fill(4.5);

		if (!passNTrkHP) continue;

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
			total_softMuon += 2;

			double mass = csTree.mass()[icand];
			double rap = csTree.y()[icand];
			double pt = csTree.pT()[icand];
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
	}
	cout << "total_softMuon = " << total_softMuon << endl;
	cout << "total_trigMuon = " << total_trigMuon << endl;

	TH1D *hEff_x = cal_eff(hNum_PtEtaPhi, hDen_PtEtaPhi, "x");
	TH1D *hEff_y = cal_eff(hNum_PtEtaPhi, hDen_PtEtaPhi, "y");
	TH1D *hEff_z = cal_eff(hNum_PtEtaPhi, hDen_PtEtaPhi, "z");
	plot_eff(hEff_x);
	plot_eff(hEff_y);
	plot_eff(hEff_z);
	plot_hist2D_compare(hNum_PtEtaPhi, hDen_PtEtaPhi, "xy");
	plot_hist_projection(hPtRapMass_trig);
}