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

const int pass_pdgId = 13;

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

void get_tracks_mass_miniAOD()
{
	//read the EDM files under the directory
	TString directory_path = "/eos/cms/store/group/phys_heavyions/wangj/RECO2023/miniaod_PhysicsHIForward0_374354/";
	std::vector<TString> file_name_list =
	{
		"/eos/cms/store/group/phys_heavyions/wangj/RECO2023/miniaod_PhysicsHIForward0_374354/reco_run374354_ls0100_streamPhysicsHIForward0_StorageManager.root",
		"/eos/cms/store/group/phys_heavyions/wangj/RECO2023/miniaod_PhysicsHIForward0_374354/reco_run374354_ls0101_streamPhysicsHIForward0_StorageManager.root"
	};

	// Get a list of all files in the specified directory
	std::vector<TFile *> files = get_file(directory_path);
	// std::vector<TFile *> files = get_file_fromList(file_name_list);

	//# Setting up the Tree ############################################################################
	int m_packedPFCandidates_nTracks;
	double m_packedPFCandidates_mass;
	double m_packedPFCandidates_mu_pt;
	double m_packedPFCandidates_mu_eta;
	double m_packedPFCandidates_mu_phi;
	std::vector<int> m_packedPFCandidates_mu_charge;
	std::vector<int> m_packedPFCandidates_nHits;

	//tree
	TFile *outfile = new TFile("Tracks_mass_miniAOD.root", "RECREATE");
	TTree *myTree = new TTree("Tracks_mass", "Tree for tracking study");
	myTree->Branch("packedPFCandidates_nTracks", &m_packedPFCandidates_nTracks, "packedPFCandidates_nTracks/I");
	myTree->Branch("packedPFCandidates_mass", &m_packedPFCandidates_mass, "packedPFCandidates_mass/D");
	myTree->Branch("packedPFCandidates_mu_pt", &m_packedPFCandidates_mu_pt, "packedPFCandidates_mu_pt/D");
	myTree->Branch("packedPFCandidates_mu_eta", &m_packedPFCandidates_mu_eta, "packedPFCandidates_mu_eta/D");
	myTree->Branch("packedPFCandidates_mu_phi", &m_packedPFCandidates_mu_phi, "packedPFCandidates_mu_phi/D");
	myTree->Branch("packedPFCandidates_mu_charge", &m_packedPFCandidates_mu_charge);
	myTree->Branch("packedPFCandidates_nHits", &m_packedPFCandidates_nHits);
	
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
			fwlite::Handle<vector<pat::PackedCandidate> > temp_objs;
			temp_objs.getByLabel(ev_Reco, "packedPFCandidates");
			int pass_count = 0;
			for (int i = 0; i < temp_objs.ptr()->size(); ++i)
			{
				auto o = temp_objs.ptr()->at(i);
				if ( abs(o.pdgId()) != pass_pdgId ) continue;
				pass_count++;
			}
			if (pass_count != 2) continue;

			cout << "====================> Event: " << ientry << endl;
			fwlite::Handle<vector<pat::PackedCandidate> > objs;
			objs.getByLabel(ev_Reco, "packedPFCandidates");

			m_packedPFCandidates_nTracks = 0;

			TLorentzVector particle;
			m_packedPFCandidates_nHits.clear();
			m_packedPFCandidates_mu_charge.clear();
			for (int i = 0; i < objs.ptr()->size(); ++i)
			{
				//counting muons only
				auto o = objs.ptr()->at(i);
				if ( abs(o.pdgId()) != pass_pdgId ) continue;
				
				m_packedPFCandidates_nTracks++;
				int hits = o.numberOfHits();
				m_packedPFCandidates_mu_pt = o.pt();
				m_packedPFCandidates_mu_eta = o.eta();
				m_packedPFCandidates_mu_phi = o.phi();
				m_packedPFCandidates_mu_charge.push_back( o.charge() );
				m_packedPFCandidates_nHits.push_back( hits );

				TLorentzVector track;
				track.SetPtEtaPhiM(o.pt(), o.eta(), o.phi(), o.mass());
				
				//print out the muon information
				cout << "Muon " << i << " pt: " << o.pt() << " eta: " << o.eta() << " phi: " << o.phi() << " mass: " << o.mass() << " charge: " << o.charge() << " hits: " << hits << endl;

				particle += track;
			}
			m_packedPFCandidates_mass = particle.M();

			cout << "m_packedPFCandidates_nTracks: " << m_packedPFCandidates_nTracks << endl;
			cout << "m_packedPFCandidates_mass: " << m_packedPFCandidates_mass << endl;

			total_UPC_events++;
			myTree->Fill();
		}
		file->Close();
	}

	myTree->Write();
	outfile->Close();

	cout << endl << "===> total entries looped: " << ientry << " total UPC events: " << total_UPC_events << endl;
}
