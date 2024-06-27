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

bool get_max_pt(fwlite::Event &ev, double &max_pt, TString module)
{
	fwlite::Handle<vector<reco::Track> > objs;
	objs.getByLabel(ev, module.Data());

	//Check if the product is valid (data is available)
	if (!objs.isValid())
	{
		cout << "Invalid product, skipping event" << endl;
		max_pt = -1;
		return kFALSE;
	}

	max_pt = 0.0;
	for (int i = 0; i < objs.ptr()->size(); ++i)
	{
		auto o = objs.ptr()->at(i);
		if (o.pt() > max_pt)
		{
			max_pt = o.pt();
		}
	}
	return kTRUE;
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

bool pipeline_eff_track_HP(fwlite::Event &ev, int &nTracks, TString module, std::vector<double> &v_pT, std::vector<double> &v_eta)
{
	fwlite::Handle<vector<reco::Track> > objs;
	objs.getByLabel(ev, module.Data());

	//Check if the product is valid (data is available)
	if (!objs.isValid())
	{
		cout << "Invalid product, skipping event" << endl;
		nTracks = -1;
		return kFALSE;
	}

	// get beamspot 
	fwlite::Handle<reco::BeamSpot> beamspot;
	beamspot.getByLabel(ev, "offlineBeamSpot");

	nTracks = 0;
	v_pT.clear();
	v_eta.clear();
	for (int i = 0; i < objs.ptr()->size(); ++i)
	{
		auto o = objs.ptr()->at(i);
		//high purity
		bool pass_HP = o.quality(reco::TrackBase::highPurity);
		//vertex cut
		// bool pass_vertex = fabs(o.dz(beamspot->position())) < 15;
		bool pass_vertex = fabs(o.dz()) < 15;
		//eta cut
		bool pass_eta = fabs(o.eta()) < 2.4;
		// // dxy cut
		// double dxy = o.dxy(beamspot->position());
		// double dxyError = o.dxyError();
		// bool pass_dxy = fabs(dxy/dxyError) < 3;
		// // dz cut
		// double dz = o.dz(beamspot->position());
		// double dzError = o.dzError();
		// bool pass_dz = fabs(dz/dzError) < 3;

		// if (!pass_HP) continue;
		// if (!pass_vertex) continue;
		// if (!pass_eta) continue;
		// if (!pass_dxy) continue;
		// if (!pass_dz) continue;
		
		v_pT.push_back(o.pt());
		v_eta.push_back(o.eta());
		nTracks++;
	}
	return kTRUE;
}

void get_eff_track()
{
	//read the EDM files under the directory
	//* MC
	// TString directory_path_Reco = "/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/Reco/STARlight_CohPhiToKK_Reco_132X_240430_154103/STARlight/CohPhiToKK/240430_134201/0000/";
	//* HIZeroBias Skim
	TString directory_path_Reco = "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/";
	//* HIForward
	// TString directory_path_Reco = "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIForward/skim_Trig_HIForward1_ZB_240614_213330/PbPb2023/HIForward_ZB/240614_193334/0000/";


	std::vector<TString> file_name_list_Reco =
	{
		// "/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/Reco/STARlight_CohPhiToKK_Reco_132X_240430_154103/STARlight/CohPhiToKK/240430_134201/0000/step3_STARlight_Reco_10.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/Reco/STARlight_CohPhiToKK_Reco_132X_240430_154103/STARlight/CohPhiToKK/240430_134201/1_10.root",
		
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIForward/skim_Trig_HIForward_ZB_240613_182519/PbPb2023/HIForward_ZB/240613_162523/0000/skim_Trig_526.root",
		
		//* HIForward
		//"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIForward/7efdd2e1-1597-424f-b77f-22935545155d.root",
		//"/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIForward/83c7d224-4d6f-489c-9189-018ff5b52db9.root",

		//* ZB
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/0213a101-9c2d-48a8-acfe-6f4ab594aad1.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/0425f6d3-cac4-4968-b8d4-8f1186af09e8.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/049686ae-2e41-49d9-9f95-97c8364d478e.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/0525a80b-52b4-457a-8ca0-fc4c217ba75c.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_103.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_113.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_115.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_350.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_441.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_572.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_180.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_406.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_29.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_38.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_639.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_558.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_26.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_238.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_88.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_167.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_448.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_259.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_357.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIZeroBias/skim_Trig_HIZeroBias0_ZB_240614_230424/PbPb2023/HIZeroBias_ZB/240614_210427/0000/skim_Trig_113.root",
		
		//* Double Diffractive
		// "/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/andre/STARLIGHT_double_diff_RECO_1.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/andre/STARLIGHT_double_diff_RECO_10.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/andre/STARLIGHT_double_diff_RECO_100.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/andre/STARLIGHT_double_diff_RECO_101.root",
		// "/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/andre/STARLIGHT_double_diff_RECO_102.root",

		//* Single Diffractive
		"/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/andre/STARLIGHT_single_diff_RECO_1.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/andre/STARLIGHT_single_diff_RECO_10.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/andre/STARLIGHT_single_diff_RECO_100.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/andre/STARLIGHT_single_diff_RECO_101.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/andre/STARLIGHT_single_diff_RECO_102.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/andre/STARLIGHT_single_diff_RECO_104.root",
		"/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/andre/STARLIGHT_single_diff_RECO_105.root",
	};

	TH1D *hNum_nTracks = new TH1D("hNum_nTracks", "hNum_nTracks; nTracks; Efficiency", 20, 0, 20);
	TH1D *hDen_nTracks = new TH1D("hDen_nTracks", "hDen_nTracks; nTracks; Efficiency", 20, 0, 20);
	TH1D *hEff_nTracks = new TH1D("hEff_nTracks", "hEff_nTracks; nTracks; Efficiency", 20, 0, 20);

	TH2D *hNum_nTracksMaxPt = new TH2D("hNum_nTracksMaxPt", "hNum_nTracksMaxPt; nTracks; pT", 20, 0, 20, 200, 0, 2);
	TH2D *hDen_nTracksMaxPt = new TH2D("hDen_nTracksMaxPt", "hDen_nTracksMaxPt; nTracks; pT", 20, 0, 20, 200, 0, 2);
	TH2D *hEff_nTracksMaxPt = new TH2D("hEff_nTracksMaxPt", "hEff_nTracksMaxPt; nTracks; pT; Efficiency", 20, 0, 20, 200, 0, 2);

	// Get a list of all files in the specified directory
	// std::vector<TFile *> files = get_file(directory_path);
	// std::vector<TFile *> files = get_file_fromList(file_name_list);

	std::vector<TFile *> files_Reco = get_file(directory_path_Reco);

	// std::vector<TFile *> files_Reco = get_file_fromList(file_name_list_Reco);

	//# Setting up the Tree ############################################################################
	std::vector<TString> trigger_name_track = 
	{
		"HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v8",
		"HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v8",
		"HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v8"
	};

	// TString trigger_name_ZB = "HLT_HIZeroBias_HighRate_v3";
	TString trigger_name_ZB = "HLT_HIZeroBias_v10";
	// TString trigger_name_ZB = "HLT_HIZeroBias_HighRateRAW_v1";
	
	std::vector<int> triggers_Idx_track;
	std::vector<int> trigger_bit_track;

	double m_generalTracks_pT_max;
	int m_generalTracks_HP_nTracks;
	int m_generalTracks_HP_nTracks_pT0p2;
	int m_hiConformalPixelTracks_nTracks;

	std::vector<double> m_generalTracks_pT;
	std::vector<double> m_generalTracks_eta;


	std::vector<int> m_generalTracks_cuts;

	TFile *outfile = new TFile("efficiency.root", "RECREATE");
	TTree *myTree = new TTree("efficiency", "Tree for efficiency study");

	myTree->Branch("trigger_bit_track", &trigger_bit_track);
	myTree->Branch("m_generalTracks_pT", &m_generalTracks_pT);
	myTree->Branch("m_generalTracks_eta", &m_generalTracks_eta);

	myTree->Branch("m_generalTracks_pT_max", &m_generalTracks_pT_max, "m_generalTracks_pT_max/D");
	myTree->Branch("m_generalTracks_HP_nTracks", &m_generalTracks_HP_nTracks, "m_generalTracks_HP_nTracks/I");
	myTree->Branch("m_hiConformalPixelTracks_nTracks", &m_hiConformalPixelTracks_nTracks, "m_hiConformalPixelTracks_nTracks/I");

	int ientry = -1;
	int total_processed_events = -1;
	int count_file = 0;
	// Loop over all files in the directory
	for (int i = 0; i < files_Reco.size(); ++i)
	{
		// if (i > 100) break;

		count_file++;
		TFile * file_Reco = files_Reco.at(i);

		//# Starting  ##########################################################################################
		cout << "====================> Processing Reco file: " << file_Reco->GetName() << endl;

		fwlite::Event ev_Reco(file_Reco);

		fwlite::Handle<edm::TriggerResults> triggerResults;
		triggerResults.getByLabel(ev_Reco, "TriggerResults", "", "HLT");
		edm::TriggerNames triggerNames = ev_Reco.triggerNames(*triggerResults);

		// print out all the trigger names
		for (int j = 0; j < triggerNames.size(); j++)
		{
			cout << "trigger: " << triggerNames.triggerName(j) << " index: " << j << endl;
		}

		// triggers_Idx_track.clear();
		if (triggers_Idx_track.size() == 0)
		{
			// find the index of the trigger
			for( int j = 0; j < trigger_name_track.size(); j++)
			{
				int index = triggerNames.triggerIndex(trigger_name_track[j]);
				if (index >= triggerNames.size())
				{
					throw std::runtime_error( Form("Cannot find trigger name %s", trigger_name_track[i].Data()) );
				}

				cout << "trigger: " << trigger_name_track[j] << " index: " << index << endl;
				triggers_Idx_track.push_back(index);
			}
		}

		int total_entries = ev_Reco.size();
		cout << "total entries: " << total_entries << endl;
		for( ev_Reco.toBegin(); ! ev_Reco.atEnd(); ++ev_Reco)
		{
			ientry++;

			triggerResults.getByLabel(ev_Reco, "TriggerResults", "", "HLT");

			//skip events that do not pass the ZB trigger
			if (!triggerResults->accept(triggerNames.triggerIndex(trigger_name_ZB.Data()))) continue;

			if (! get_max_pt(ev_Reco, m_generalTracks_pT_max, "generalTracks") ) continue;
			// if (m_generalTracks_pT_max > 0.2) continue;

			if (! pipeline_eff_track_HP(ev_Reco, m_generalTracks_HP_nTracks, "generalTracks", m_generalTracks_pT, m_generalTracks_eta) ) continue;
			if (! pipeline_eff_track(ev_Reco, m_hiConformalPixelTracks_nTracks, "hiConformalPixelTracks") ) continue;

			trigger_bit_track.clear();
			for (int i = 0; i < triggers_Idx_track.size(); i++)
			{
				trigger_bit_track.push_back( triggerResults->accept(triggers_Idx_track[i]) );
			}

			// fill the histogram
			hDen_nTracksMaxPt->Fill(m_generalTracks_HP_nTracks, m_generalTracks_pT_max);
			if (triggerResults->accept(triggers_Idx_track[1])) hNum_nTracksMaxPt->Fill(m_generalTracks_HP_nTracks, m_generalTracks_pT_max);
			// hDen_nTracks->Fill(m_generalTracks_HP_nTracks);
			// if (triggerResults->accept(triggers_Idx_track[1])) hNum_nTracks->Fill(m_generalTracks_HP_nTracks);

			total_processed_events++;
			myTree->Fill();
		}
		file_Reco->Close();
	}

	myTree->Write();
	outfile->Close();

	cout << endl << "===> total entries looped: " << ientry << " total processed events: " << total_processed_events << endl;

	// quickly check the efficiency
	int pt_bin = hDen_nTracksMaxPt->GetYaxis()->FindBin(0.2);
	hNum_nTracks = (TH1D *) hNum_nTracksMaxPt->ProjectionX("hNum_nTracks");
	hDen_nTracks = (TH1D *) hDen_nTracksMaxPt->ProjectionX("hDen_nTracks");
	hEff_nTracks->Divide(hNum_nTracks, hDen_nTracks, 1, 1, "B");
	for (int i = 0; i < hEff_nTracks->GetNbinsX(); ++i)
	{
		cout << "bin: " << i << " eff: " << hEff_nTracks->GetBinContent(i+1) << " Den: " << hDen_nTracks->GetBinContent(i+1) << " Num: " << hNum_nTracks->GetBinContent(i+1) << endl;
	}
}
