/*
	For plotting efficiency from the HiForest AOD
	1. Muon Trigger efficiency as a function of offline pT
	2. Muon Trigger efficiency as a function of offline eta
	3. Muon Trigger efficiency as a function of offline phi
	4. Track efficiency as a function of offline number of Tracks

	The job of this script is to read the HiForest AOD and save all the necessary information into a root file
	The acutal plotting will be done in another script in local computer

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

void pipeline_track(fwlite::Event &ev, int &nTracks, double &mass, std::vector<int> &nHit, TString module)
{

	fwlite::Handle<vector<reco::Track> > objs;
	objs.getByLabel(ev, module.Data());

	//Check if the product is valid (data is available)
	if (!objs.isValid())
	{
		cout << "Invalid product, skipping event" << endl;
		nTracks = -1;
		return ;
	}

	nTracks = objs.ptr()->size();

	int total_hits = 0;
	TLorentzVector particle;

	nHit.clear();
	for (int i = 0; i < objs.ptr()->size(); ++i)
	{
		auto o = objs.ptr()->at(i);
		int hits = o.numberOfValidHits();
		nHit.push_back( hits );
		total_hits += hits;

		//get the mass from the track from the muon
		TLorentzVector track;
		
		// track.SetPtEtaPhiM(o.pt(), o.eta(), o.phi(), 0.4976);	// Kaon mass
		track.SetPtEtaPhiM(o.pt(), o.eta(), o.phi(), 0.1057);	// Muon mass

		if (particle.Pt() == 0) particle = track;
		else particle += track;
	}
	mass = particle.M();
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

void pipeline_eff(fwlite::Event &ev, std::vector<double> &v_pt, std::vector<double> &v_eta, std::vector<double> &v_phi, std::vector<int> &v_charge, TString module)
{
	//empty the vector
	v_pt.clear();
	v_eta.clear();
	v_phi.clear();
	v_charge.clear();

	fwlite::Handle<vector<reco::Track> > objs;
	objs.getByLabel(ev, module.Data());

	//Check if the product is valid (data is available)
	if (!objs.isValid())
	{
		cout << "Invalid product, skipping event" << endl;
		return ;
	}

	for (int i = 0; i < objs.ptr()->size(); ++i)
	{
		auto o = objs.ptr()->at(i);
		v_pt.push_back(o.pt());
		v_eta.push_back(o.eta());
		v_phi.push_back(o.phi());
		v_charge.push_back(o.charge());
	}
}

void pipeline_eff_track(fwlite::Event &ev, int &nTracks, TString module)
{
	fwlite::Handle<vector<reco::Track> > objs;
	objs.getByLabel(ev, module.Data());

	//Check if the product is valid (data is available)
	if (!objs.isValid())
	{
		cout << "Invalid product, skipping event" << endl;
		nTracks = -1;
		return ;
	}

	nTracks = 0;
	for (int i = 0; i < objs.ptr()->size(); ++i)
	{
		auto o = objs.ptr()->at(i);
		if (o.quality(reco::TrackBase::highPurity))
		{
			nTracks++;
			// cout << "general track: " << o.pt() << " " << o.eta() << " " << o.phi() << " " << o.charge() << " " << o.quality(reco::TrackBase::highPurity) << endl;
		}
	}
}

void get_eff()
{
	//read the EDM files under the directory
	TString directory_path = "/eos/cms/store/group/phys_heavyions/wangj/RECO2023/aod_PhysicsHIForward0_374345/";
	std::vector<TString> file_name_list =
	{
		// "/eos/cms/store/group/phys_heavyions/wangj/RECO2023/aod_PhysicsHIForward0_374345/reco_run374345_ls0100_streamPhysicsHIForward0_StorageManager.root",
		// "/eos/cms/store/group/phys_heavyions/wangj/RECO2023/aod_PhysicsHIForward0_374345/reco_run374345_ls0199_streamPhysicsHIForward0_StorageManager.root"
		// "/eos/cms/store/group/phys_heavyions/nalewis/reco_RAW2DIGI_L1Reco_RECO_PAT_run374668_ls0012.root",
		// "/eos/cms/store/group/phys_heavyions/nalewis/reco_RAW2DIGI_L1Reco_RECO_PAT_run374668_ls0013.root",
		// "/eos/cms/store/group/phys_heavyions/nalewis/reco_RAW2DIGI_L1Reco_RECO_PAT_run374668_ls0014.root",
		// "/eos/cms/store/group/phys_heavyions/nalewis/reco_RAW2DIGI_L1Reco_RECO_PAT_run374668_ls0015.root",
		// "/eos/cms/store/group/phys_heavyions/nalewis/reco_RAW2DIGI_L1Reco_RECO_PAT_run374668_ls0016.root",
		// "/eos/cms/store/group/phys_heavyions/nalewis/reco_RAW2DIGI_L1Reco_RECO_PAT_run374668_ls0082.root",
		// "/eos/cms/store/group/phys_heavyions/nalewis/reco_RAW2DIGI_L1Reco_RECO_PAT_run374668_ls0083.root",
		// "/eos/cms/store/group/phys_heavyions/nalewis/reco_RAW2DIGI_L1Reco_RECO_PAT_run374668_ls0084.root",
		// "/eos/cms/store/group/phys_heavyions/nalewis/reco_RAW2DIGI_L1Reco_RECO_PAT_run374668_ls0085.root",
		// "/eos/cms/store/group/phys_heavyions/nalewis/reco_RAW2DIGI_L1Reco_RECO_PAT_run374668_ls0086.root",
		// "/eos/cmsfile:/store/group/phys_heavyions/nalewis/reco_RAW2DIGI_L1Reco_RECO_PAT_run374668_ls0087.root",

		//HION Reco
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_231002_175205/CRAB_UserFiles/HIForward0_231002_175205/231002_155227/0000/reco_RAW2DIGI_L1Reco_RECO_PAT_100.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_231002_175205/CRAB_UserFiles/HIForward0_231002_175205/231002_155227/0000/reco_RAW2DIGI_L1Reco_RECO_PAT_101.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/Express/HIForward0_231002_175205/CRAB_UserFiles/HIForward0_231002_175205/231002_155227/0000/reco_RAW2DIGI_L1Reco_RECO_PAT_102.root",
	};

	// Get a list of all files in the specified directory
	// std::vector<TFile *> files = get_file(directory_path);
	std::vector<TFile *> files = get_file_fromList(file_name_list);

	//# Setting up the Tree ############################################################################
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
	std::vector<int> triggers_Idx_mu;
	std::vector<int> trigger_bit_mu;
	std::vector<int> triggers_Idx_track;
	std::vector<int> trigger_bit_track;

	std::vector<double> m_standAloneMuons_pt;
	std::vector<double> m_standAloneMuons_eta;
	std::vector<double> m_standAloneMuons_phi;
	std::vector<int> m_standAloneMuons_charge;

	std::vector<double> m_globalMuons_pt;
	std::vector<double> m_globalMuons_eta;
	std::vector<double> m_globalMuons_phi;
	std::vector<int> m_globalMuons_charge;

	std::vector<double> m_refittedStandAloneMuons_pt;
	std::vector<double> m_refittedStandAloneMuons_eta;
	std::vector<double> m_refittedStandAloneMuons_phi;
	std::vector<int> m_refittedStandAloneMuons_charge;

	int m_generalTracks_nTracks;
	int m_hiConformalPixelTracks_nTracks;
	// std::vector<int> m_generalTracks_HP;

	TFile *outfile = new TFile("efficiency.root", "RECREATE");
	TTree *myTree = new TTree("efficiency", "Tree for efficiency study");

	myTree->Branch("trigger_bit_mu", &trigger_bit_mu);
	myTree->Branch("standAloneMuons_pt", &m_standAloneMuons_pt);
	myTree->Branch("standAloneMuons_eta", &m_standAloneMuons_eta);
	myTree->Branch("standAloneMuons_phi", &m_standAloneMuons_phi);
	myTree->Branch("standAloneMuons_charge", &m_standAloneMuons_charge);

	myTree->Branch("globalMuons_pt", &m_globalMuons_pt);
	myTree->Branch("globalMuons_eta", &m_globalMuons_eta);
	myTree->Branch("globalMuons_phi", &m_globalMuons_phi);
	myTree->Branch("globalMuons_charge", &m_globalMuons_charge);

	myTree->Branch("refittedStandAloneMuons_pt", &m_refittedStandAloneMuons_pt);
	myTree->Branch("refittedStandAloneMuons_eta", &m_refittedStandAloneMuons_eta);
	myTree->Branch("refittedStandAloneMuons_phi", &m_refittedStandAloneMuons_phi);
	myTree->Branch("refittedStandAloneMuons_charge", &m_refittedStandAloneMuons_charge);

	myTree->Branch("trigger_bit_track", &trigger_bit_track);
	myTree->Branch("generalTracks_nTracks", &m_generalTracks_nTracks, "generalTracks_nTracks/I");
	myTree->Branch("hiConformalPixelTracks_nTracks", &m_hiConformalPixelTracks_nTracks, "hiConformalPixelTracks_nTracks/I");
	// myTree->Branch("generalTracks_HP", &m_generalTracks_HP);

	int ientry = -1;
	int total_UPC_events = 0;
	int count_file = -1;
	// Loop over all files in the directory
	for (auto file: files)
	{
		count_file++;
		if (count_file > 10) break;

		//# Starting  ##########################################################################################
		cout << "====================> Processing file: " << file->GetName() << endl;
		fwlite::Event ev_Reco(file);

		fwlite::Handle<edm::TriggerResults> triggerResults;
		triggerResults.getByLabel(ev_Reco, "TriggerResults", "", "HLT");
		edm::TriggerNames triggerNames = ev_Reco.triggerNames(*triggerResults);

		// triggers_Idx_mu.clear();
		if (triggers_Idx_mu.size() == 0)
		{
			// find the index of the trigger
			for( int i = 0; i < trigger_name_mu.size(); i++)
			{
				int index = triggerNames.triggerIndex(trigger_name_mu[i]);
				if (index >= triggerNames.size())
				{
					throw std::runtime_error( Form("Cannot find trigger name %s", trigger_name_mu[i].Data()) );
				}

				cout << "trigger: " << trigger_name_mu[i] << " index: " << index << endl;
				triggers_Idx_mu.push_back(index);
			}
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

		int total_entries = ev_Reco.size();
		cout << "total entries: " << total_entries << endl;
		for( ev_Reco.toBegin(); ! ev_Reco.atEnd(); ++ev_Reco)
		{
			ientry++;

			//skip 0 or more than 20 tracks
			fwlite::Handle<vector<reco::Track> > temp_objs;
			temp_objs.getByLabel(ev_Reco, "hiConformalPixelTracks");
			if (temp_objs.ptr()->size() > 10) continue;

			// for track efficiency
			pipeline_eff_track(ev_Reco, m_generalTracks_nTracks, "generalTracks");
			pipeline_eff_track(ev_Reco, m_hiConformalPixelTracks_nTracks, "hiConformalPixelTracks");
			trigger_bit_track.clear();
			for (int i = 0; i < triggers_Idx_track.size(); i++)
			{
				trigger_bit_track.push_back( triggerResults->accept(triggers_Idx_track[i]) );
			}

			//skip the event without muon
			// fwlite::Handle<vector<reco::Track> > temp_objs;
			// temp_objs.getByLabel(ev_Reco, "globalMuons");
			// if (temp_objs.ptr()->size() == 0) continue;

			trigger_bit_mu.clear();
			for (int i = 0; i < triggers_Idx_mu.size(); i++)
			{
				trigger_bit_mu.push_back( triggerResults->accept(triggers_Idx_mu[i]) );
			}
			
			pipeline_eff(ev_Reco, m_standAloneMuons_pt, m_standAloneMuons_eta, m_standAloneMuons_phi, m_standAloneMuons_charge, "standAloneMuons");
			pipeline_eff(ev_Reco, m_globalMuons_pt, m_globalMuons_eta, m_globalMuons_phi, m_globalMuons_charge, "globalMuons");
			pipeline_eff(ev_Reco, m_refittedStandAloneMuons_pt, m_refittedStandAloneMuons_eta, m_refittedStandAloneMuons_phi, m_refittedStandAloneMuons_charge, "refittedStandAloneMuons");

			total_UPC_events++;
			myTree->Fill();
		}
		file->Close();
	}

	

	myTree->Write();
	outfile->Close();

	cout << endl << "===> total entries looped: " << ientry << " total UPC events: " << total_UPC_events << endl;
}
