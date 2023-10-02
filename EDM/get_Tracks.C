/*
	For plotting the number of tracks and number of hits in each EDM event
	The information is stored in the tree "Tracks"
	Only to be run on the lxplus

	JiaZhao Lin, Sep 2023
*/

#include "DataFormats/FWLite/interface/Handle.h"

void update_progress(int ientry, int total_entries, int percentage_increment)
{
	if (ientry % (total_entries / percentage_increment) == 0)
	{
		std::cout << "Processing " << ientry << "th entry... (" << (int)((double)ientry / total_entries * 100) << "%)" << std::endl;
	}
}

void pipeline_track(fwlite::Event &ev, int &nTracks, std::vector<int> &nHit, TString module)
{
	// clear the vector
	nHit.clear();

	fwlite::Handle<vector<reco::Track> > objs;
	objs.getByLabel(ev, module.Data());

	//Check if the product is valid (data is available)
	if (!objs.isValid())
	{
		cout << "Invalid product, skipping event" << endl;
		nTracks = -1;
		return;
	}

	nTracks = objs.ptr()->size();
	int total_hits = 0;
	for ( auto o: *objs)
	{
		auto hits = o.numberOfValidHits();
		nHit.push_back( hits );
		total_hits += hits;
	}

	cout << module << " size: " << nTracks << endl;
	cout << "===> nHit: " << total_hits << endl;
}

void get_Tracks()
{
	//Jpsi
	// TFile file_HLT("./output_132X_Jpsi.root");
	// TFile file_Reco("/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/Reco/STARlight_CohJpsiToMuMu_Reco_132X_230824_045129/CRAB_UserFiles/STARlight_CohJpsiToMuMu_Reco_132X_230824_045129/230824_025138/0000/step3_STARlight_Reco_10.root");

	//Phi
	// TFile file_HLT("./output_132X_Phi.root");
	// TFile file_Reco("/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/Reco/STARlight_CohPhiToKK_Reco_132X_230916_064532/CRAB_UserFiles/STARlight_CohPhiToKK_Reco_132X_230916_064532/230916_044540/0000/step3_STARlight_Reco_10.root");

	//hydjet
	// TFile *file_HLT = TFile::Open("./output_132X_hydjet.root");
	// TFile *file_Reco = TFile::Open("root://cms-xrd-global.cern.ch///store/himc/Run3Winter22PbPbNoMixRECOMiniAOD/MinBias_Hydjet_Drum5F_5p02TeV/AODSIM/122X_mcRun3_2021_realistic_HI_v10-v3/2430000/9c9e7fa3-4d63-40da-a168-d87c6765cbae.root");

	TFile *file_HLT = TFile::Open("./output_132X_hydjet.root");
	TFile *file_Reco = TFile::Open("root://cms-xrd-global.cern.ch///store/user/subehera/MinBias_PbPb_5p36TeV_Hydjet_v1/MinBias_PbPb_5p36TeV_Hydjet_RECODEBUG_v1/230921_112329/0000/recostep_10.root");



	//# Setting up the Tree ############################################################################
	int m_hltPixelTracksPPOnAA;
	int m_hltPixelTracksForUPCPPOnAA;
	int m_hltPixelTracksLowPtForUPCPPOnAA;
	int m_generalTracks;
	int m_hiConformalPixelTracks;
	std::vector<int> m_hltPixelTracksPPOnAA_nHits;
	std::vector<int> m_hltPixelTracksForUPCPPOnAA_nHits;
	std::vector<int> m_hltPixelTracksLowPtForUPCPPOnAA_nHits;
	std::vector<int> m_generalTracks_nHits;
	std::vector<int> m_hiConformalPixelTracks_nHits;

	//tree
	TFile *outfile = new TFile("Tracks.root", "RECREATE");
	TTree *myTree = new TTree("Tracks", "Tree for tracking study");
	myTree->Branch("hltPixelTracksPPOnAA", &m_hltPixelTracksPPOnAA, "hltPixelTracksPPOnAA/I");
	myTree->Branch("hltPixelTracksForUPCPPOnAA", &m_hltPixelTracksForUPCPPOnAA, "hltPixelTracksForUPCPPOnAA/I");
	myTree->Branch("hltPixelTracksLowPtForUPCPPOnAA", &m_hltPixelTracksLowPtForUPCPPOnAA, "hltPixelTracksLowPtForUPCPPOnAA/I");
	myTree->Branch("generalTracks", &m_generalTracks, "generalTracks/I");
	myTree->Branch("hiConformalPixelTracks", &m_hiConformalPixelTracks, "hiConformalPixelTracks/I");
	myTree->Branch("hltPixelTracksPPOnAA_nHits", &m_hltPixelTracksPPOnAA_nHits);
	myTree->Branch("hltPixelTracksForUPCPPOnAA_nHits", &m_hltPixelTracksForUPCPPOnAA_nHits);
	myTree->Branch("hltPixelTracksLowPtForUPCPPOnAA_nHits", &m_hltPixelTracksLowPtForUPCPPOnAA_nHits);
	myTree->Branch("generalTracks_nHits", &m_generalTracks_nHits);
	myTree->Branch("hiConformalPixelTracks_nHits", &m_hiConformalPixelTracks_nHits);

	//# Starting  ##########################################################################################
	fwlite::Event ev_HLT(file_HLT);
	fwlite::Event ev_Reco(file_Reco);

	int ientry = -1;
	int total_entries = ev_HLT.size();
	for( ev_HLT.toBegin(), ev_Reco.toBegin(); ! ev_HLT.atEnd() && ! ev_Reco.atEnd(); ++ev_HLT, ++ev_Reco)
	{
		ientry++;
		update_progress(ientry, total_entries, 10);

		cout << "====================> Event: " << ientry << endl;
		pipeline_track(ev_HLT, m_hltPixelTracksPPOnAA, m_hltPixelTracksPPOnAA_nHits, "hltPixelTracksPPOnAA");
		pipeline_track(ev_HLT, m_hltPixelTracksForUPCPPOnAA, m_hltPixelTracksForUPCPPOnAA_nHits, "hltPixelTracksForUPCPPOnAA");
		pipeline_track(ev_HLT, m_hltPixelTracksLowPtForUPCPPOnAA, m_hltPixelTracksLowPtForUPCPPOnAA_nHits, "hltPixelTracksLowPtForUPCPPOnAA");

		pipeline_track(ev_Reco, m_generalTracks, m_generalTracks_nHits, "generalTracks");
		pipeline_track(ev_Reco, m_hiConformalPixelTracks, m_hiConformalPixelTracks_nHits, "hiConformalPixelTracks");

		edm::Handle<edmNew::DetSetVector<SiPixelCluster>> pixelClusters;
		ev_HLT.getByLabel("hltSiPixelClustersPPOnAA", pixelClusters);
		cout << "hltSiPixelClustersPPOnAA size: " << pixelClusters->size() << endl;

		myTree->Fill();
	}
	myTree->Write();
	outfile->Close();
}
