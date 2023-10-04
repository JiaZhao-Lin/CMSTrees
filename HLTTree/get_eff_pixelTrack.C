/*
	For plotting efficiency from the HLT Re emulated trigger

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
}

void get_eff_pixelTrack()
{
	//read the EDM files under the directory
	TString directory_path = "/eos/cms/store/group/phys_heavyions/wangj/RECO2023/aod_PhysicsHIForward0_374345/";
	std::vector<TString> file_name_list =
	{
		"output_132X_correct.root"
	};

	TH1D *hNum_nTracks = new TH1D("hNum_nTracks", "hNum_nTracks; nTracks", 20, 0, 20);
	TH1D *hDen_nTracks = new TH1D("hDen_nTracks", "hDen_nTracks; nTracks", 20, 0, 20);
	TH1D *hEff_nTracks = new TH1D("hEff_nTracks", "hEff_nTracks; nTracks; Efficiency", 20, 0, 20);

	// Get a list of all files in the specified directory
	// std::vector<TFile *> files = get_file(directory_path);
	std::vector<TFile *> files = get_file_fromList(file_name_list);

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

	TFile *outfile = new TFile("efficiency.root", "RECREATE");
	TTree *myTree = new TTree("efficiency", "Tree for efficiency study");

	myTree->Branch("trigger_bit_track", &trigger_bit_track);
	myTree->Branch("m_hltPixelTracksForUPCPPOnAA_nTracks", &m_hltPixelTracksForUPCPPOnAA_nTracks, "m_hltPixelTracksForUPCPPOnAA_nTracks/I");
	myTree->Branch("m_hltPixelTracksLowPtForUPCPPOnAA_nTracks", &m_hltPixelTracksLowPtForUPCPPOnAA_nTracks, "m_hltPixelTracksLowPtForUPCPPOnAA_nTracks/I");
	myTree->Branch("m_hltPixelTracksPPOnAA_nTracks", &m_hltPixelTracksPPOnAA_nTracks, "m_hltPixelTracksPPOnAA_nTracks/I");
	myTree->Branch("m_hltSiPixelClustersPPOnAA_nClusters", &m_hltSiPixelClustersPPOnAA_nClusters, "m_hltSiPixelClustersPPOnAA_nClusters/I");

	int ientry = -1;
	int total_processed_events = 0;
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
		triggerResults.getByLabel(ev_Reco, "TriggerResults", "", "MyHLT");
		edm::TriggerNames triggerNames = ev_Reco.triggerNames(*triggerResults);

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

		int total_entries = ev_Reco.size();
		cout << "total entries: " << total_entries << endl;
		for( ev_Reco.toBegin(); ! ev_Reco.atEnd(); ++ev_Reco)
		{
			ientry++;

			triggerResults.getByLabel(ev_Reco, "TriggerResults", "", "MyHLT");

			//skip events that do not pass the ZB trigger
			if (!triggerResults->accept(triggerNames.triggerIndex(trigger_name_ZB.Data()))) continue;

			// for track efficiency
			pipeline_eff_track(ev_Reco, m_hltPixelTracksForUPCPPOnAA_nTracks, "hltPixelTracksForUPCPPOnAA");
			pipeline_eff_track(ev_Reco, m_hltPixelTracksLowPtForUPCPPOnAA_nTracks, "hltPixelTracksLowPtForUPCPPOnAA");
			pipeline_eff_track(ev_Reco, m_hltPixelTracksPPOnAA_nTracks, "hltPixelTracksPPOnAA");

			// for cluster efficiency
			fwlite::Handle<edmNew::DetSetVector<SiPixelCluster>> pixelClusters;
			pixelClusters.getByLabel(ev_Reco, "hltSiPixelClustersPPOnAA");
			if (!pixelClusters.isValid())
			{
				cout << "Invalid product, skipping event" << endl;
				continue;
			}
			m_hltSiPixelClustersPPOnAA_nClusters = pixelClusters->size();

			trigger_bit_track.clear();
			for (int i = 0; i < triggers_Idx_track.size(); i++)
			{
				trigger_bit_track.push_back( triggerResults->accept(triggers_Idx_track[i]) );
			}

			// fill the histogram
			hDen_nTracks->Fill(m_hltPixelTracksLowPtForUPCPPOnAA_nTracks);
			if (triggerResults->accept(triggers_Idx_track[1])) hNum_nTracks->Fill(m_hltPixelTracksLowPtForUPCPPOnAA_nTracks);
			
			
			total_processed_events++;
			myTree->Fill();
		}
		file->Close();
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
