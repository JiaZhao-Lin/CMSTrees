#include "../VertexCompositeTree/VCTree_utilities.C"

TH1D *hPixelHit		= new TH1D("hPixelHit","hPixelHit;N_{hit}",10,0,10);
TH1D *hPixelTrack	= new TH1D("hPixelTrack","hPixelTrack;N_{track}",5,0,5);
TH2D *hPixelTrack_OnlineVsOffline = new TH2D("hPixelTrack_OnlineVsOffline","hPixelTrack_OnlineVsOffline;N_{track}^{online};N_{track}^{offline}",6,0,6,6,0,6);


void plot()
{
	auto c = new TCanvas("c", "", 800, 600);
	//stat box and disable title
	gStyle->SetOptStat(0);
	gStyle->SetOptTitle(0);

	hPixelHit->Draw();
	for (int i = 0; i < hPixelHit->GetNbinsX(); ++i)
	{
		hPixelHit->GetXaxis()->SetBinLabel(i + 1, Form("%d", i));
		draw_latex(0.68, 0.75-i*0.05, Form("N_{pixel}^{hit} = %d: %.0f", i, hPixelHit->GetBinContent(i+1)));
	}
	c->SaveAs("outFigures/hPixelHit.png");


	hPixelTrack->Draw();
	for (int i = 0; i < hPixelTrack->GetNbinsX(); ++i)
	{
		hPixelTrack->GetXaxis()->SetBinLabel(i + 1, Form("%d", i));
		draw_latex(0.68, 0.75-i*0.05, Form("N_{pixel}^{track} = %d: %.0f", i, hPixelTrack->GetBinContent(i+1)));
	}	
	c->SaveAs("outFigures/hPixelTrack.png");

	// draw scatter plot
	for (int i = 0; i < hPixelTrack_OnlineVsOffline->GetNbinsX(); ++i)
	{
		hPixelTrack_OnlineVsOffline->GetXaxis()->SetBinLabel(i + 1, Form("%d", i));
		hPixelTrack_OnlineVsOffline->GetYaxis()->SetBinLabel(i + 1, Form("%d", i));
	}
	hPixelTrack_OnlineVsOffline->Draw("colz");
	// offset the graph a little bit
	// gPad->SetLeftMargin(0.15);
	gPad->SetRightMargin(0.13);
	gPad->SetBottomMargin(0.13);

	//draw percentage on each bin
	for (int i = 0; i < hPixelTrack_OnlineVsOffline->GetNbinsX(); ++i)
	{
		for (int j = 0; j < hPixelTrack_OnlineVsOffline->GetNbinsY(); ++j)
		{
			draw_latex(0.13 + i*0.13, 0.15 + j*0.13, Form("%.2f %%", hPixelTrack_OnlineVsOffline->GetBinContent(i+1, j+1)/hPixelTrack_OnlineVsOffline->GetEntries()*100));
		}
	}
	c->SaveAs("outFigures/hPixelTrack_OnlineVsOffline.png");
}

void plot_pixel()
{
	//# Read the online info from Pixel.root file ##################################################
	TFile *f = new TFile("inFiles/Pixel_test.root");
	TTree *tree = (TTree*)f->Get("Pixel");
	
	std::vector<int> *nHit = 0;
	int nTrack;

	std::vector<int> v_nTrack;
	tree->Print();

	tree->SetBranchAddress("nHit", &nHit);
	tree->SetBranchAddress("nTrack", &nTrack);

	int nentries = tree->GetEntries();
	cout << nentries << endl;
	for (int i = 0; i < nentries; ++i)
	{
		tree->GetEntry(i);
		v_nTrack.push_back(nTrack);
		hPixelTrack->Fill(nTrack);
		for (int j = 0; j < nHit->size(); ++j)
		{
			hPixelHit->Fill(nHit->at(j));
		}
	}

	//# Now read the offline reco info from VCTree ##################################################

	// std::string inputFileDir = "../VertexCompositeTree/inFiles/2023/VCTree_STARLIGHT_CohJpsiToMuMu_5p36TeV_132X_1M.root";
	std::string inputFileDir = "../VertexCompositeTree/inFiles/2023/VCTree_STARLIGHT_CohJpsiToMuMu_5p36TeV_132X_10.root";
	const auto& csTreeDir = "dimucontana_mc";

	// Extract the tree
	VertexCompositeTree csTree;
	if (!csTree.GetTree(inputFileDir, csTreeDir)) 
	{
		cout << "Invalid Correct-Sign tree!" << endl;
		return;
	}

	int total_entries = csTree.GetEntries();
	cout << "max_entries Online = " << tree->GetEntries() << endl;
	cout << "max_entries Offline = " << total_entries << endl;
	for (Long64_t jentry = 0; jentry < total_entries; jentry++) 
	{
		update_progress(jentry, total_entries, 10);

		// Get the entry
		if (csTree.GetEntry(jentry) < 0) 
		{
			cout << "Invalid correct-sign entry!" << endl;
			return;
		}

		Bool_t goodVtx				= (csTree.evtSel()[2] && csTree.evtSel()[3]);
		Bool_t goodHFVeto 			= (!csTree.evtSel()[16] && !csTree.evtSel()[17]);
		Bool_t passNTrkHP 			= csTree.NtrkHP() == 2;	// contain only two high-purity tracks and nothing else
		Bool_t passEvtSel 			= goodVtx && goodHFVeto && passNTrkHP;

		// if (!passEvtSel) continue;
		// cout << "offline NpixelTracks = " << csTree.NpixelTracks() << ", online NpixelTracks = " << v_nTrack[jentry] << endl;
		hPixelTrack_OnlineVsOffline->Fill(v_nTrack[jentry], csTree.NpixelTracks());
	}

	//# Plotting ####################################################################################
	plot();
}