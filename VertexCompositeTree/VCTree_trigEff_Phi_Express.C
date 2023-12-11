/*
	Use to plot online express data trigger efficiency
	Should use looser cuts when skimming the RECO data when producing VC tree

	JiaZhao Lin, Sep 2023
*/

// #include "./VCTree_routines.C"
// #include "Selections.C"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TSystemFile.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TMath.h"
#include "TLine.h"
#include "TLatex.h"
#include "TLegend.h"

const double PDG_PHI			= 333;
bool IsPhi(int pdgId)
{
	// ! require phi
	return pdgId == PDG_PHI;
}

//# Global Variables #######################################################################################################
const int triggers_Idx = 0;

const int nbin_pt = 100;
const double min_pt = 0;
const double max_pt = 1;
const int nbin_eta = 100;
const double min_eta = -3;
const double max_eta = 3;
const int nbin_phi = 100;
const double min_phi = -3.14;
const double max_phi = 3.14;
const int nbin_mass = 100;
const double min_mass = 0.9;
const double max_mass = 1.2;
const int nbin_rap = 100;
const double min_rap = -2.6;
const double max_rap = 2.6;

//# Histograms ##############################################################################################################
TH1D *hRunNb = new TH1D("hRunNb", "hRunNb; Run Number", 200, 374700, 374900);
TH3D *hPosKaonPtEtaPhi = new TH3D("hPosKaonPtEtaPhi", "hPosKaonPtEtaPhi; p_{T} [GeV/c]; #eta; #phi", nbin_pt, min_pt, max_pt, nbin_eta, min_eta, max_eta, nbin_phi, min_phi, max_phi);
TH3D *hNegKaonPtEtaPhi = new TH3D("hNegKaonPtEtaPhi", "hNegKaonPtEtaPhi; p_{T} [GeV/c]; #eta; #phi", nbin_pt, min_pt, max_pt, nbin_eta, min_eta, max_eta, nbin_phi, min_phi, max_phi);
TH3D *hKaonPtEtaPhi = new TH3D("hKaonPtEtaPhi", "hKaonPtEtaPhi; p_{T} [GeV/c]; #eta; #phi", nbin_pt, min_pt, max_pt, nbin_eta, min_eta, max_eta, nbin_phi, min_phi, max_phi);

TH3D *hPtRapMass = new TH3D("hPtRapMass", "hPtRapMass; p_{T} [GeV/c]; Rapidity; Mass [GeV/c^{2}]", nbin_pt, min_pt, max_pt, nbin_rap, min_rap, max_rap, nbin_mass, min_mass, max_mass);

TH3D *hdEdxPPt = new TH3D("hdEdxPPt", "hdEdxPPt; dE/dx [MeV/cm]; p [GeV]; p_{T}", 240, 0, 12, 200, 0, 1, 100, 0, 10);

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
void update_progress(int ientry, int total_entries, int percentage_increment)
{
	if (ientry % (total_entries / percentage_increment) == 0)
	{
		std::cout << "Processing " << ientry << "th entry... (" << (int)((double)ientry / total_entries * 100) << "%)" << std::endl;
	}
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

void plot_hist2D_compare(TH3D *h3Num, TH3D *h3Den, TString projection, TString title)
{
	auto c = new TCanvas("c", "", 800, 600);

	TH2D *h2Num = (TH2D*)h3Num->Project3D(projection.Data());
	TH2D *h2Den = (TH2D*)h3Den->Project3D(projection.Data());

	h2Den->Draw();
	h2Den->SetMarkerSize(1);

	h2Num->Draw("same");
	h2Num->SetMarkerStyle(24);
	h2Num->SetMarkerSize(1);
	h2Num->SetMarkerColor(kRed);

	//legend
	auto legend = new TLegend(0.40, 0.5, 0.60, 0.7);
	// make the legend transparent
	legend->SetFillStyle(0);
	// legend->SetTextSize(0.03);
	legend->AddEntry(h2Num, "Pos Kaon", "p");
	legend->AddEntry(h2Den, "Neg Kaon", "p");
	legend->Draw("same");

	c->SaveAs(Form("outFigures/%s.png", title.Data()));

	delete c;
}

void plot_hist2D(TH3D *h3, TString projection, TString title)
{
	auto c = new TCanvas("c", "", 800, 600);

	gStyle->SetOptStat(0);

	TH2D *h2 = (TH2D*)h3->Project3D(projection.Data());

	// shift the right margin of the histogram to the right by 0.2
	gPad->SetRightMargin(0.15);
	h2->Draw("colz");

	// //set log z axis
	// gPad->SetLogz();

	TLatex *t = new TLatex();
	t->SetNDC();
	t->SetTextFont(42);
	t->SetTextSize(0.05);
	t->DrawLatex(0.13, 0.94, Form("#bf{CMS} #it{Preliminary}"));
	t->DrawLatex(0.52, 0.94, Form("PbPb (2023, 5.36 TeV)"));

	c->SaveAs(Form("outFigures/%s.png", title.Data()));

	delete c;
}

void plot_hist1D_compare(TH3D *h3Num, TH3D *h3Den, TString projection, TString title)
{
	auto c = new TCanvas("c", "", 800, 600);

	TH1D *h1Num = (TH1D*)h3Num->Project3D(projection.Data());
	TH1D *h1Den = (TH1D*)h3Den->Project3D(projection.Data());

	h1Den->Draw("E");
	h1Den->SetMarkerSize(1);

	h1Num->Draw("e same");
	h1Num->SetMarkerStyle(24);
	h1Num->SetMarkerSize(1);
	h1Num->SetMarkerColor(kRed);

	//legend
	auto legend = new TLegend(0.47, 0.6, 0.60, 0.8);
	// make the legend transparent
	legend->SetFillStyle(0);
	// legend->SetTextSize(0.03);
	legend->AddEntry(h1Num, "Reco", "p");
	legend->AddEntry(h1Den, "Gen", "p");
	legend->Draw("same");

	c->SaveAs(Form("outFigures/%s.png", title.Data()));

	delete c;
}

void plot_hist_projection(TH3D *h3)
{
	auto c = new TCanvas("c", "", 800, 600);

	TH1D *h1_x = (TH1D*)h3->Project3D("x");
	TH1D *h1_y = (TH1D*)h3->Project3D("y");
	TH1D *h1_z = (TH1D*)h3->Project3D("z");

	gStyle->SetOptStat(0);
	h1_x->Draw("E");
	c->SaveAs(Form("outFigures/dataPhi/%s_x.png", h3->GetName()));

	h1_y->Draw("E");
	c->SaveAs(Form("outFigures/dataPhi/%s_y.png", h3->GetName()));

	// fit_mass(h1_z);
	h1_z->Draw("E");
	// set lower limit to 0
	gStyle->SetOptStat(1111);
	h1_z->GetYaxis()->SetRangeUser(0, h1_z->GetMaximum() * 1.1);
	c->SaveAs(Form("outFigures/dataPhi/%s_z.png", h3->GetName()));

	delete c;
}

//# Main ####################################################################################################################
void VCTree_trigEff_Phi_Express()
{
	// TH1::SetDefaultSumw2(kTRUE);
	// TString inputFile = "inFiles/2023/phi_ana_highBetaStar.root";
	// TString inputFile = "inFiles/2023/phi_ana_test3.root";
	// TString inputFile = "inFiles/phi_tree_highBetaStar.root";
	// TString inputFile = "inFiles/2023/diKa_ana_107.root";
	// TString inputFile = "/eos/cms/store/group/phys_heavyions/anstahll/CERN/PbPb2023/ParticleAnalyzer/DiKa/2023_10_11/ParticleAnalyzer_DiKa_HIRun2023A_2023_10_11.root";
	TString inputFile = "inFiles/2023/ParticleAnalyzer_DiKa_HIRun2023A_dEdx_2023_10_13_v2.root";

	// Open the file containing the tree.
	TFile *myFile = TFile::Open(inputFile);

	if (!myFile) {
		std::cerr << "Error: failed to open file " << inputFile << std::endl;
		return;
	}
	// TTreeReader treeReader("phiAna/ParticleTree", myFile);
	TTreeReader treeReader("diKaAna/ParticleTree", myFile);
	// TTreeReader treeReader("generalanaNew/ParticleTree", myFile);
	// TreeValues cTV = TreeValues(treeReader, TreeValuesList, TreeValuesVectorList);

	TTreeReaderValue<std::vector<Bool_t>>		evtSel(treeReader,		"evtSel");

	TTreeReaderValue<std::vector<Float_t>>		cand_mass(treeReader,	"cand_mass");
	TTreeReaderValue<std::vector<Int_t>>		cand_pdgId(treeReader,	"cand_pdgId");
	TTreeReaderValue<std::vector<Float_t>>		cand_pT(treeReader,		"cand_pT");
	TTreeReaderValue<std::vector<Float_t>>		cand_eta(treeReader,	"cand_eta");
	TTreeReaderValue<std::vector<Float_t>>		cand_phi(treeReader,	"cand_phi");
	TTreeReaderValue<std::vector<Float_t>>		cand_y(treeReader,		"cand_y");
	TTreeReaderValue<std::vector<char>>			cand_charge(treeReader,	"cand_charge");
	TTreeReaderValue<unsigned short>			NtrkHP(treeReader,		"NtrkHP");
	TTreeReaderValue<vector<unsigned char>>		cand_status(treeReader,		"cand_status");

	TTreeReaderValue<float>							PFHFmaxETMinus(treeReader,	"PFHFmaxEMinus");
	TTreeReaderValue<float>							PFHFmaxETPlus(treeReader,	"PFHFmaxEPlus");

	TTreeReaderValue<std::vector<Bool_t>>		trk_isHP(treeReader,			"trk_isHP");
	TTreeReaderValue<std::vector<unsigned short>>		cand_trkIdx(treeReader,	"cand_trkIdx");
	// TTreeReaderValue<std::vector<float>>		trk_pT(treeReader,			"trk_pT");
	// TTreeReaderValue<std::vector<float>>		trk_eta(treeReader,			"trk_eta");
	// TTreeReaderValue<std::vector<float>>		trk_phi(treeReader,			"trk_phi");

	TTreeReaderValue<vector<vector<unsigned int> >>		cand_dauIdx(treeReader,	"cand_dauIdx");
	TTreeReaderValue<vector<vector<float> >>		cand_pTDau(treeReader,	"cand_pTDau");
	TTreeReaderValue<vector<vector<float> >>		cand_etaDau(treeReader,	"cand_etaDau");
	TTreeReaderValue<vector<vector<float> >>		cand_phiDau(treeReader,	"cand_phiDau");

	TTreeReaderValue<std::vector<Bool_t>>		passHLT(treeReader,		"passHLT");

	TTreeReaderValue<std::vector<float>>	trk_dEdx_dedxPixelHarmonic2(treeReader,		"trk_dEdx_dedxPixelHarmonic2");

	int jentry = -1;
	int total_entries = treeReader.GetEntries();
	int processed_entries = 0;
	int max_entries = 4000000;
	while (treeReader.Next()) {
		jentry++;
		if (jentry > max_entries) break;
		update_progress(jentry, total_entries, 50);

		Bool_t passEvtSel = evtSel->at(0);
		Bool_t pass2HPTrk = *NtrkHP == 2;

		Bool_t passHFVeto = (*PFHFmaxETMinus < 8 && *PFHFmaxETPlus < 8);
		// bool   passHLTtrig = passHLT->at(1) || passHLT->at(4);
		bool   passHLTtrig = passHLT->at(0) || passHLT->at(1);

		if (!passEvtSel) continue;
		if (!pass2HPTrk) continue;
		if (!passHFVeto) continue;
		if (!passHLTtrig) continue;
		// cout<< passHLT->size() << endl;

		// Bool_t passEvtSel = goodVtx && pass2HPTrk;
		// if(passEvtSel) cHM.hnEvts->Fill(3.5);
	
		// fill dedx first
		for (int itrk = 0; itrk < cand_trkIdx->size(); itrk++)
		{
			int trkIdx = cand_trkIdx->at(itrk);
			if (trkIdx > 1000) continue;
			if (cand_status->at(trkIdx) != 1) continue;
			double dEdx = trk_dEdx_dedxPixelHarmonic2->at(trkIdx);
			double pT = cand_pT->at(itrk);
			double eta = cand_eta->at(itrk);
			double p = pT * ROOT::Math::cosh(eta);

			hdEdxPPt->Fill(dEdx, p, pT);
		}

		// # ---------------------------------------------------------------------------------------------------
		// # Rec START------------------------------------------------------------------------------------------

		Double_t posPt_rec;
		Double_t posEta_rec;
		Double_t posPhi_rec;
		Double_t posMass_rec;
		Double_t negPt_rec;
		Double_t negEta_rec;
		Double_t negPhi_rec;
		Double_t negMass_rec;

		for (int irec = 0; irec < cand_mass->size(); irec++)
		{
			// * catch phi rec
			if (IsPhi(cand_pdgId->at(irec)))
			{
				Double_t pt_rec   = cand_pT->at(irec);
				Double_t eta_rec  =	cand_eta->at(irec);
				Double_t phi_rec  =	cand_phi->at(irec);
				Double_t mass_rec =	cand_mass->at(irec);
				Double_t y_rec    =	cand_y->at(irec);

				// if (pt_rec > 0.1) continue;
				// if (abs(y_rec) > 0.4) continue;
				// if (mass_rec < 0.5 || mass_rec > 1.2) continue;

				if (trk_dEdx_dedxPixelHarmonic2->at(0) < 3 ) continue;
				if (trk_dEdx_dedxPixelHarmonic2->at(1) < 3 ) continue;
				if (!trk_isHP->at(0)) continue;
				if (!trk_isHP->at(1)) continue;

				//* Get the daughter kaons index
				auto daughters = cand_dauIdx->at(irec);
				// cout << "EVENT: " << jentry << endl;
				bool passKaonSelection = true;
				for ( int idau = 0; idau < daughters.size(); idau++)
				{
					int change = cand_charge->at(daughters[idau]);
					int pdgId = cand_pdgId->at(daughters[idau]);
					double pt = cand_pT->at(daughters[idau]);
					double eta = cand_eta->at(daughters[idau]);
					double phi = cand_phi->at(daughters[idau]);

					double p = pt * ROOT::Math::cosh(eta);

					if (p < 0.2)
					{
						passKaonSelection = false;
						continue;
					}

					// if (abs(pdgId) != 321)
					// {
					// 	passKaonSelection = false;
					// 	continue;
					// }

					if (pt > 0.2)
					{
						passKaonSelection = false;
						continue;
					}
					// // if (pt < 0.1)
					// // {
					// // 	passKaonSelection = false;
					// // 	continue;
					// // }
					// if (abs(eta) > 2.4 || abs(eta) < 1.4)
					// {
					// 	passKaonSelection = false;
					// 	continue;
					// }

					if (change > 0)
					{
						hPosKaonPtEtaPhi->Fill(pt, eta, phi);
					}
					else if (change < 0)
					{
						hNegKaonPtEtaPhi->Fill(pt, eta, phi);
					}
				}
				if (!passKaonSelection) continue;

				// * Collecting Selection 
				// if (!passEvtSel) continue;
				hPtRapMass->Fill(pt_rec,y_rec, mass_rec);
				processed_entries++;

			}
			// else
			// {
			// 	throw std::runtime_error("unknown particle type!");
			// }

		}
		// # Rec END--------------------------------------------------------------------------------------------
	}
	cout << "Total entries: " << total_entries << " Processed entries: " << processed_entries << endl;

	// plot_hist2D_compare(hPosKaonPtEtaPhi, hNegKaonPtEtaPhi, "xy", "dataPhi/hPosNegKaonPtEtaPhi");

	hKaonPtEtaPhi->Add(hPosKaonPtEtaPhi, hNegKaonPtEtaPhi);
	// plot_hist_projection(hPtRapMass);
	// plot_hist_projection(hKaonPtEtaPhi);
	
	// plot_hist2D(hKaonPtEtaPhi, "xy", "dataPhi/hKaonPtEtaPhi");

	plot_hist2D(hdEdxPPt, "xy", "dataPhi/hdEdxPPt");
}