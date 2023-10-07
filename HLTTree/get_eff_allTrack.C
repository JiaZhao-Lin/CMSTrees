/*
	For plotting efficiency from the HLT Re emulated trigger
	Also getting the general track from a AOD file

	For pixel track efficiency, use the following trigger:
		"HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v8",
		"HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v8",
		"HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v8"

	JiaZhao Lin, Sep 2023
*/

#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TF1.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TLorentzVector.h"
#include "TCanvas.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"


void update_progress(int ientry, int total_entries, int percentage_increment)
{
	if (ientry % (total_entries / percentage_increment) == 0)
	{
		std::cout << "Processing " << ientry << "th entry... (" << (int)((double)ientry / total_entries * 100) << "%)" << std::endl;
	}
}

std::vector<TFile *>get_file(TString directory_path)
{
	// Get a list of all files in the specified directory
	TSystemDirectory dir(directory_path, directory_path);
	TList *files = dir.GetListOfFiles();

	// Loop over all files in the directory
	std::vector<TFile *> file_list;
	if (files)
	{
		TSystemFile *file;
		TString fname;
		TIter next(files);
		while ((file=(TSystemFile*)next()))
		{
			fname = file->GetName();
			if (!file->IsDirectory() && fname.EndsWith(".root"))
			{
				if (fname.Contains("junk")) continue;
				cout << "file name: " << fname << endl;
				TFile *file = new TFile(directory_path + fname);
				file_list.push_back(file);
			}
		}
	}
	return file_list;
}

std::vector<TFile *>get_file_fromList(std::vector<TString> file_name_list)
{
	std::vector<TFile *> file_list;
	for ( auto file_name: file_name_list)
	{
		cout << "file name: " << file_name << endl;
		TFile *file = new TFile(file_name);
		file_list.push_back(file);
	}
	return file_list;
}

bool pipeline_eff_track(fwlite::Event &ev, int &nTracks, TString module)
{
	fwlite::Handle<vector<reco::Track> > objs;
	objs.getByLabel(ev, module.Data());

	//Check if the product is valid (data is available)
	if (!objs.isValid())
	{
		cout << Form("Invalid product: %s, skipping event", module.Data()) << endl;
		nTracks = -1;
		return kFALSE;
	}

	nTracks = objs.ptr()->size();
	// for (int i = 0; i < objs.ptr()->size(); ++i)
	// {
	// 	auto o = objs.ptr()->at(i);
	// 	if (o.quality(reco::TrackBase::highPurity))
	// 	{
	// 		nTracks++;
	// 		// cout << "general track: " << o.pt() << " " << o.eta() << " " << o.phi() << " " << o.charge() << " " << o.quality(reco::TrackBase::highPurity) << endl;
	// 	}
	// }
	return kTRUE;
}

bool pipeline_eff_track_HP(fwlite::Event &ev, int &nTracks, int &nTracks_pT0p3, int &nTracks_pT0p4, int &nTracks_pT0p5, TString module)
{
	fwlite::Handle<vector<reco::Track> > objs;
	objs.getByLabel(ev, module.Data());

	//Check if the product is valid (data is available)
	if (!objs.isValid())
	{
		cout << "Invalid product, skipping event" << endl;
		nTracks = -1;
		nTracks_pT0p3 = -1;
		nTracks_pT0p4 = -1;
		nTracks_pT0p5 = -1;
		return kFALSE;
	}

	nTracks = 0;
	nTracks_pT0p3 = 0;
	nTracks_pT0p4 = 0;
	nTracks_pT0p5 = 0;
	for (int i = 0; i < objs.ptr()->size(); ++i)
	{
		auto o = objs.ptr()->at(i);
		//high purity
		int pass_HP = o.quality(reco::TrackBase::highPurity);
		//vertex cut
		int pass_vertex = fabs(o.dz()) < 15;
		//eta cut
		int pass_eta = fabs(o.eta()) < 2.4;

		if (!pass_HP) continue;
		if (!pass_vertex) continue;
		if (!pass_eta) continue;
		
		nTracks++;
		//pt cut
		if (o.pt() > 0.3) nTracks_pT0p3++;
		if (o.pt() > 0.4) nTracks_pT0p4++;
		if (o.pt() > 0.5) nTracks_pT0p5++;
	}
	return kTRUE;
}

void get_eff_allTrack()
{
	//read the EDM files under the directory
	TString directory_path_HLT = "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HLT/HLT_132X_Run374666_231004_013822/CRAB_UserFiles/374666/231003_233846/0000/";
	TString directory_path_Reco = "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_HIReco_Run374666_231004_115615/CRAB_UserFiles/374666/231004_095625/0000/";
	std::vector<TString> file_name_list =
	{
		"output_132X_correct.root"
	};

	std::vector<TString> file_name_list_HLT =
	{
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HLT/HLT_132X_Run374666_231004_013822/CRAB_UserFiles/374666/231003_233846/0000/output_132X_100.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HLT/HLT_132X_Run374666_231004_013822/CRAB_UserFiles/374666/231003_233846/0000/output_132X_101.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HLT/HLT_132X_Run374666_231004_013822/CRAB_UserFiles/374666/231003_233846/0000/output_132X_102.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HLT/HLT_132X_Run374666_231004_013822/CRAB_UserFiles/374666/231003_233846/0000/output_132X_103.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HLT/HLT_132X_Run374666_231004_013822/CRAB_UserFiles/374666/231003_233846/0000/output_132X_104.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HLT/HLT_132X_Run374666_231004_013822/CRAB_UserFiles/374666/231003_233846/0000/output_132X_105.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HLT/HLT_132X_Run374666_231004_013822/CRAB_UserFiles/374666/231003_233846/0000/output_132X_106.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HLT/HLT_132X_Run374666_231004_013822/CRAB_UserFiles/374666/231003_233846/0000/output_132X_107.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HLT/HLT_132X_Run374666_231004_013822/CRAB_UserFiles/374666/231003_233846/0000/output_132X_108.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HLT/HLT_132X_Run374666_231004_013822/CRAB_UserFiles/374666/231003_233846/0000/output_132X_109.root",
	};
	std::vector<TString> file_name_list_Reco =
	{
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_HIReco_Run374666_231004_115615/CRAB_UserFiles/374666/231004_095625/0000/reco_RAW2DIGI_L1Reco_RECO_100.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_HIReco_Run374666_231004_115615/CRAB_UserFiles/374666/231004_095625/0000/reco_RAW2DIGI_L1Reco_RECO_101.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_HIReco_Run374666_231004_115615/CRAB_UserFiles/374666/231004_095625/0000/reco_RAW2DIGI_L1Reco_RECO_102.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_HIReco_Run374666_231004_115615/CRAB_UserFiles/374666/231004_095625/0000/reco_RAW2DIGI_L1Reco_RECO_103.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_HIReco_Run374666_231004_115615/CRAB_UserFiles/374666/231004_095625/0000/reco_RAW2DIGI_L1Reco_RECO_104.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_HIReco_Run374666_231004_115615/CRAB_UserFiles/374666/231004_095625/0000/reco_RAW2DIGI_L1Reco_RECO_105.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_HIReco_Run374666_231004_115615/CRAB_UserFiles/374666/231004_095625/0000/reco_RAW2DIGI_L1Reco_RECO_106.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_HIReco_Run374666_231004_115615/CRAB_UserFiles/374666/231004_095625/0000/reco_RAW2DIGI_L1Reco_RECO_107.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_HIReco_Run374666_231004_115615/CRAB_UserFiles/374666/231004_095625/0000/reco_RAW2DIGI_L1Reco_RECO_108.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_HIReco_Run374666_231004_115615/CRAB_UserFiles/374666/231004_095625/0000/reco_RAW2DIGI_L1Reco_RECO_109.root",
	};

	TH1D *hNum_nTracks = new TH1D("hNum_nTracks", "hNum_nTracks; nTracks", 20, 0, 20);
	TH1D *hDen_nTracks = new TH1D("hDen_nTracks", "hDen_nTracks; nTracks", 20, 0, 20);
	TH1D *hEff_nTracks = new TH1D("hEff_nTracks", "hEff_nTracks; nTracks; Efficiency", 20, 0, 20);

	// Get a list of all files in the specified directory
	// std::vector<TFile *> files = get_file(directory_path);
	// std::vector<TFile *> files = get_file_fromList(file_name_list);

	std::vector<TFile *> files_HLT = get_file(directory_path_HLT);
	std::vector<TFile *> files_Reco = get_file(directory_path_Reco);

	// std::vector<TFile *> files_HLT = get_file_fromList(file_name_list_HLT);
	// std::vector<TFile *> files_Reco = get_file_fromList(file_name_list_Reco);

	//# Setting up the Tree ############################################################################
	std::vector<TString> trigger_name_track = 
	{
		"HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v8",
		"HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v8",
		"HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v8"
	};

	TString trigger_name_ZB = "HLT_HIZeroBias_HighRate_v1";
	
	std::vector<int> triggers_Idx_track;
	std::vector<int> trigger_bit_track;

	int m_hltPixelTracksForUPCPPOnAA_nTracks;
	int m_hltPixelTracksLowPtForUPCPPOnAA_nTracks;
	int m_hltPixelTracksPPOnAA_nTracks;
	int m_hltSiPixelClustersPPOnAA_nClusters;

	int m_generalTracks_HP_nTracks;
	int m_generalTracks_HP_nTracks_pT0p3;
	int m_generalTracks_HP_nTracks_pT0p4;
	int m_generalTracks_HP_nTracks_pT0p5;

	std::vector<int> m_generalTracks_cuts;

	TFile *outfile = new TFile("efficiency.root", "RECREATE");
	TTree *myTree = new TTree("efficiency", "Tree for efficiency study");

	myTree->Branch("trigger_bit_track", &trigger_bit_track);
	myTree->Branch("m_hltPixelTracksForUPCPPOnAA_nTracks", &m_hltPixelTracksForUPCPPOnAA_nTracks, "m_hltPixelTracksForUPCPPOnAA_nTracks/I");
	myTree->Branch("m_hltPixelTracksLowPtForUPCPPOnAA_nTracks", &m_hltPixelTracksLowPtForUPCPPOnAA_nTracks, "m_hltPixelTracksLowPtForUPCPPOnAA_nTracks/I");
	myTree->Branch("m_hltPixelTracksPPOnAA_nTracks", &m_hltPixelTracksPPOnAA_nTracks, "m_hltPixelTracksPPOnAA_nTracks/I");
	myTree->Branch("m_hltSiPixelClustersPPOnAA_nClusters", &m_hltSiPixelClustersPPOnAA_nClusters, "m_hltSiPixelClustersPPOnAA_nClusters/I");

	myTree->Branch("m_generalTracks_HP_nTracks", &m_generalTracks_HP_nTracks, "m_generalTracks_HP_nTracks/I");
	myTree->Branch("m_generalTracks_HP_nTracks_pT0p3", &m_generalTracks_HP_nTracks_pT0p3, "m_generalTracks_HP_nTracks_pT0p3/I");
	myTree->Branch("m_generalTracks_HP_nTracks_pT0p4", &m_generalTracks_HP_nTracks_pT0p4, "m_generalTracks_HP_nTracks_pT0p4/I");
	myTree->Branch("m_generalTracks_HP_nTracks_pT0p5", &m_generalTracks_HP_nTracks_pT0p5, "m_generalTracks_HP_nTracks_pT0p5/I");

	int ientry = -1;
	int total_processed_events = -1;
	int count_file = 0;
	// Loop over all files in the directory
	for (int i = 0, j = 0; i < files_HLT.size() && j < files_Reco.size(); ++i, ++j)
	{
		count_file++;
		if ( count_file < 100 || count_file > 130) continue;
		// TFile * file_HLT = new TFile("/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HLT/HLT_132X_Run374666_231004_013822/CRAB_UserFiles/374666/231003_233846/0000/output_132X_100.root");
		// TFile * file_Reco= new TFile("/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_HIReco_Run374666_231004_115615/CRAB_UserFiles/374666/231004_095625/0000/reco_RAW2DIGI_L1Reco_RECO_100.root");
		TFile * file_HLT = files_HLT.at(i);
		TFile * file_Reco = files_Reco.at(j);

		//# Starting  ##########################################################################################
		cout << "====================> Processing HLT file: " << file_HLT->GetName() << endl;
		cout << "====================> Processing Reco file: " << file_Reco->GetName() << endl;

		fwlite::Event ev_HLT(file_HLT);
		fwlite::Event ev_Reco(file_Reco);

		fwlite::Handle<edm::TriggerResults> triggerResults;
		triggerResults.getByLabel(ev_HLT, "TriggerResults", "", "MyHLT");
		edm::TriggerNames triggerNames = ev_HLT.triggerNames(*triggerResults);

		// triggers_Idx_track.clear();
		if (triggers_Idx_track.size() == 0)
		{
			// find the index of the trigger
			for( int i = 0; i < trigger_name_track.size(); i++)
			{
				int index = triggerNames.triggerIndex(trigger_name_track[i]);
				if (index >= triggerNames.size())
				{
					throw std::runtime_error( Form("Cannot find trigger name %s", trigger_name_track[i].Data()) );
				}

				cout << "trigger: " << trigger_name_track[i] << " index: " << index << endl;
				triggers_Idx_track.push_back(index);
			}
		}

		int total_entries = ev_HLT.size();
		cout << "total entries: " << total_entries << endl;
		for( ev_HLT.toBegin(); ! ev_HLT.atEnd(); ++ev_HLT)
		{
			ientry++;
			ev_Reco.to(ev_HLT.id());
			int matched_run = ev_HLT.id().run() == ev_Reco.id().run();
			int matched_lumi = ev_HLT.id().luminosityBlock() == ev_Reco.id().luminosityBlock();
			int matched_event = ev_HLT.id().event() == ev_Reco.id().event();
			if (!matched_run || !matched_lumi || !matched_event)
			{
				cout << "event not match!" << endl;
				continue;
			}

			// cout << "====================> Processing event: HLT: " << ev_HLT.id().event() << " Reco: " << ev_Reco.id().event() << endl;

			triggerResults.getByLabel(ev_HLT, "TriggerResults", "", "MyHLT");

			//skip events that do not pass the ZB trigger
			if (!triggerResults->accept(triggerNames.triggerIndex(trigger_name_ZB.Data()))) continue;

			// for track efficiency
			// if (! pipeline_eff_track(ev_HLT, m_hltPixelTracksPPOnAA_nTracks, "hltPixelTracksPPOnAA")) continue;
			if (! pipeline_eff_track(ev_HLT, m_hltPixelTracksForUPCPPOnAA_nTracks, "hltPixelTracksForUPCPPOnAA")) continue;
			if (! pipeline_eff_track(ev_HLT, m_hltPixelTracksLowPtForUPCPPOnAA_nTracks, "hltPixelTracksLowPtForUPCPPOnAA")) continue;
			
			if (! pipeline_eff_track_HP(ev_Reco, m_generalTracks_HP_nTracks, m_generalTracks_HP_nTracks_pT0p3, m_generalTracks_HP_nTracks_pT0p4, m_generalTracks_HP_nTracks_pT0p5, "generalTracks") ) continue;

			// for cluster efficiency
			fwlite::Handle<edmNew::DetSetVector<SiPixelCluster>> pixelClusters;
			pixelClusters.getByLabel(ev_HLT, "hltSiPixelClustersPPOnAA");
			if (!pixelClusters.isValid())
			{
				cout << "Invalid hltSiPixelClustersPPOnAA product, skipping event" << endl;
				continue;
			}
			m_hltSiPixelClustersPPOnAA_nClusters = pixelClusters->size();

			trigger_bit_track.clear();
			for (int i = 0; i < triggers_Idx_track.size(); i++)
			{
				trigger_bit_track.push_back( triggerResults->accept(triggers_Idx_track[i]) );
			}

			// fill the histogram
			hDen_nTracks->Fill(m_generalTracks_HP_nTracks);
			if (triggerResults->accept(triggers_Idx_track[1])) hNum_nTracks->Fill(m_generalTracks_HP_nTracks);
			
			
			total_processed_events++;
			myTree->Fill();
		}
		// file->Close();
		file_HLT->Close();
		file_Reco->Close();
	}

	myTree->Write();
	outfile->Close();

	cout << endl << "===> total entries looped: " << ientry << " total processed events: " << total_processed_events << endl;

	// quickly check the efficiency
	hEff_nTracks->Divide(hNum_nTracks, hDen_nTracks, 1, 1, "B");
	for (int i = 0; i < hEff_nTracks->GetNbinsX(); ++i)
	{
		cout << "bin: " << i << " eff: " << hEff_nTracks->GetBinContent(i+1) << " Den: " << hDen_nTracks->GetBinContent(i+1) << " Num: " << hNum_nTracks->GetBinContent(i+1) << endl;
	}
	
}
