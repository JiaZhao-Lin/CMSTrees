/*
	For plotting mass distribution from the combined tracks

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

std::vector<TString> triggers =
{
	"HLT_HIUPC_SingleMuOpen_NotMBHF2AND_v8"
	// "HLT_HIUPC_SingleMuCosmic_NotMBHF2AND_v6"
	// "HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v4",
	// "HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v6"
	// "HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v4"
};
std::vector<int> triggers_Idx;
int trigger_size = triggers.size();

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

void get_tracks_mass()
{
	//read the EDM files under the directory
	TString directory_path = "/eos/cms/store/group/phys_heavyions/wangj/RECO2023/aod_PhysicsHIForward0_374345/";
	// std::vector<TString> file_name_list =
	// {
	// 	"/eos/cms/store/group/phys_heavyions/wangj/RECO2023/aod_PhysicsHIForward0_374345/reco_run374345_ls0100_streamPhysicsHIForward0_StorageManager.root",
	// 	"/eos/cms/store/group/phys_heavyions/wangj/RECO2023/aod_PhysicsHIForward0_374345/reco_run374345_ls0199_streamPhysicsHIForward0_StorageManager.root"
	// };

	// Get a list of all files in the specified directory
	std::vector<TFile *> files = get_file(directory_path);
	// std::vector<TFile *> files = get_file_fromList(file_name_list);

	//# Setting up the Tree ############################################################################
	int m_generalTracks_nTracks;
	double m_generalTracks_mass;
	std::vector<int> m_generalTracks_nHits;
	// int m_generalTracks_nHits[nHits];

	// int m_hiConformalPixelTracks_nTracks;
	// double m_hiConformalPixelTracks_mass;
	// // std::vector<int> m_hiConformalPixelTracks_nHits;
	// int m_hiConformalPixelTracks_nHits[nHits];
	
	int m_standAloneMuons_nTracks;
	double m_standAloneMuons_mass;
	std::vector<int> m_standAloneMuons_nHits;
	// int m_standAloneMuons_nHits[nHits];

	int m_refittedStandAloneMuons_nTracks;
	double m_refittedStandAloneMuons_mass;
	std::vector<int> m_refittedStandAloneMuons_nHits;
	// int m_refittedStandAloneMuons_nHits[nHits];

	int m_globalMuons_nTracks;
	double m_globalMuons_mass;
	std::vector<int> m_globalMuons_nHits;
	// int m_globalMuons_nHits[nHits];

	int m_displacedTracks_nTracks;
	double m_displacedTracks_mass;
	std::vector<int> m_displacedTracks_nHits;
	// int m_displacedTracks_nHits[nHits];

	int m_displacedGlobalMuons;
	double m_displacedGlobalMuons_mass;
	std::vector<int> m_displacedGlobalMuons_nHits;
	// int m_displacedGlobalMuons_nHits[nHits];

	int m_displacedStandAloneMuons;
	double m_displacedStandAloneMuons_mass;
	std::vector<int> m_displacedStandAloneMuons_nHits;
	// int m_displacedStandAloneMuons_nHits[nHits];

	//tree
	TFile *outfile = new TFile("Tracks_mass.root", "RECREATE");
	TTree *myTree = new TTree("Tracks_mass", "Tree for tracking study");
	myTree->Branch("generalTracks_nTracks", &m_generalTracks_nTracks, "generalTracks_nTracks/I");
	myTree->Branch("generalTracks_mass", &m_generalTracks_mass, "generalTracks_mass/D");
	myTree->Branch("generalTracks_nHits", &m_generalTracks_nHits);
	// myTree->Branch("generalTracks_nHits", m_generalTracks_nHits, "generalTracks_nHits[100]/I");
	// myTree->Branch("hiConformalPixelTracks_nTracks", &m_hiConformalPixelTracks_nTracks, "hiConformalPixelTracks_nTracks/I");
	// myTree->Branch("hiConformalPixelTracks_mass", &m_hiConformalPixelTracks_mass, "hiConformalPixelTracks_mass/D");
	// myTree->Branch("hiConformalPixelTracks_nHits", &m_hiConformalPixelTracks_nHits);
	// myTree->Branch("hiConformalPixelTracks_nHits", m_hiConformalPixelTracks_nHits, "hiConformalPixelTracks_nHits[100]/I");
	myTree->Branch("standAloneMuons_nTracks", &m_standAloneMuons_nTracks, "standAloneMuons_nTracks/I");
	myTree->Branch("standAloneMuons_mass", &m_standAloneMuons_mass, "standAloneMuons_mass/D");
	myTree->Branch("standAloneMuons_nHits", &m_standAloneMuons_nHits);
	// myTree->Branch("standAloneMuons_nHits", m_standAloneMuons_nHits, "standAloneMuons_nHits[100]/I");
	myTree->Branch("refittedStandAloneMuons_nTracks", &m_refittedStandAloneMuons_nTracks, "refittedStandAloneMuons_nTracks/I");
	myTree->Branch("refittedStandAloneMuons_mass", &m_refittedStandAloneMuons_mass, "refittedStandAloneMuons_mass/D");
	myTree->Branch("refittedStandAloneMuons_nHits", &m_refittedStandAloneMuons_nHits);
	// myTree->Branch("refittedStandAloneMuons_nHits", m_refittedStandAloneMuons_nHits, "refittedStandAloneMuons_nHits[100]/I");
	myTree->Branch("globalMuons_nTracks", &m_globalMuons_nTracks, "globalMuons_nTracks/I");
	myTree->Branch("globalMuons_mass", &m_globalMuons_mass, "globalMuons_mass/D");
	myTree->Branch("globalMuons_nHits", &m_globalMuons_nHits);
	// myTree->Branch("globalMuons_nHits", m_globalMuons_nHits, "globalMuons_nHits[100]/I");
	myTree->Branch("displacedTracks_nTracks", &m_displacedTracks_nTracks, "displacedTracks_nTracks/I");
	myTree->Branch("displacedTracks_mass", &m_displacedTracks_mass, "displacedTracks_mass/D");
	myTree->Branch("displacedTracks_nHits", &m_displacedTracks_nHits);
	// myTree->Branch("displacedTracks_nHits", m_displacedTracks_nHits, "displacedTracks_nHits[100]/I");
	myTree->Branch("displacedGlobalMuons", &m_displacedGlobalMuons, "displacedGlobalMuons/I");
	myTree->Branch("displacedGlobalMuons_mass", &m_displacedGlobalMuons_mass, "displacedGlobalMuons_mass/D");
	myTree->Branch("displacedGlobalMuons_nHits", &m_displacedGlobalMuons_nHits);
	// myTree->Branch("displacedGlobalMuons_nHits", m_displacedGlobalMuons_nHits, "displacedGlobalMuons_nHits[100]/I");
	myTree->Branch("displacedStandAloneMuons", &m_displacedStandAloneMuons, "displacedStandAloneMuons/I");
	myTree->Branch("displacedStandAloneMuons_mass", &m_displacedStandAloneMuons_mass, "displacedStandAloneMuons_mass/D");
	myTree->Branch("displacedStandAloneMuons_nHits", &m_displacedStandAloneMuons_nHits);
	// myTree->Branch("displacedStandAloneMuons_nHits", m_displacedStandAloneMuons_nHits, "displacedStandAloneMuons_nHits[100]/I");

	int ientry = -1;
	int total_UPC_events = 0;
	// Loop over all files in the directory
	for (auto file: files)
	{
		//# Starting  ##########################################################################################
		cout << "====================> Processing file: " << file->GetName() << endl;
		fwlite::Event ev_Reco(file);

		fwlite::Handle<edm::TriggerResults> triggerResults;
		triggerResults.getByLabel(ev_Reco, "TriggerResults", "", "HLT");
		edm::TriggerNames triggerNames = ev_Reco.triggerNames(*triggerResults);

		// triggers_Idx.clear();
		if (triggers_Idx.size() == 0)
		{
			// find the index of the trigger
			for( int i = 0; i < triggers.size(); i++)
			{
				int index = triggerNames.triggerIndex(triggers[i]);
				if (index >= triggerNames.size())
				{
					throw std::runtime_error( Form("Cannot find trigger name %s", triggers[i].Data()) );
				}

				cout << "trigger: " << triggers[i] << " index: " << index << endl;
				triggers_Idx.push_back(index);
			}
		}

		int total_entries = ev_Reco.size();
		cout << "total entries: " << total_entries << endl;
		for( ev_Reco.toBegin(); ! ev_Reco.atEnd(); ++ev_Reco)
		{
			ientry++;
			// update_progress(ientry, total_entries, 10);
			// cout << "====================> Event: " << ientry << endl;

			//= skip the event without the trigger fired
			triggerResults.getByLabel(ev_Reco, "TriggerResults", "", "HLT");
			if (!triggerResults->accept(triggers_Idx[0])) continue;

			//= skip the event doesn't have exactly 2 tracks
			fwlite::Handle<vector<reco::Track> > temp_objs;
			temp_objs.getByLabel(ev_Reco, "generalTracks");
			if (temp_objs.ptr()->size() != 2) continue;


			pipeline_track(ev_Reco, m_generalTracks_nTracks, m_generalTracks_mass, m_generalTracks_nHits, "generalTracks");
			// pipeline_track(ev_Reco, m_hiConformalPixelTracks_nTracks, m_hiConformalPixelTracks_mass, m_hiConformalPixelTracks_nHits, "hiConformalPixelTracks");
			pipeline_track(ev_Reco, m_standAloneMuons_nTracks, m_standAloneMuons_mass, m_standAloneMuons_nHits, "standAloneMuons");
			pipeline_track(ev_Reco, m_globalMuons_nTracks, m_globalMuons_mass, m_globalMuons_nHits, "globalMuons");
			pipeline_track(ev_Reco, m_refittedStandAloneMuons_nTracks, m_refittedStandAloneMuons_mass, m_refittedStandAloneMuons_nHits, "refittedStandAloneMuons");
			pipeline_track(ev_Reco, m_displacedTracks_nTracks, m_displacedTracks_mass, m_displacedTracks_nHits, "displacedTracks");
			pipeline_track(ev_Reco, m_displacedGlobalMuons, m_displacedGlobalMuons_mass, m_displacedGlobalMuons_nHits, "displacedGlobalMuons");
			pipeline_track(ev_Reco, m_displacedStandAloneMuons, m_displacedStandAloneMuons_mass, m_displacedStandAloneMuons_nHits, "displacedStandAloneMuons");

			cout << "m_standAloneMuons_nTracks: " << m_standAloneMuons_nTracks << endl;
			cout << "===> m_standAloneMuons_mass: " << m_standAloneMuons_mass << endl;
			cout << "m_globalMuons_nTracks: " << m_globalMuons_nTracks << endl;
			cout << "===> m_globalMuons_mass: " << m_globalMuons_mass << endl;
			cout << "m_generalTracks_nTracks: " << m_generalTracks_nTracks << endl;
			cout << "===> m_generalTracks_mass: " << m_generalTracks_mass << endl;
			
			total_UPC_events++;
			myTree->Fill();
		}
		file->Close();
	}

	myTree->Write();
	outfile->Close();

	cout << endl << "===> total entries looped: " << ientry << " total UPC events: " << total_UPC_events << endl;
}
