#include "DataFormats/FWLite/interface/Handle.h"

std::vector<TString> triggers =
{
	"HLT_HIZeroBias_v6",
	"HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v4",
	"HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v4",
	"HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v4"
};
std::vector<int> triggers_Idx;
int trigger_size = triggers.size();
TH2D *hPass_TrigVsPixelTracks = new TH2D("hPass_TrigVsPixelTracks", "hPass_TrigVsPixelTracks", trigger_size, 0, trigger_size, 5, 0, 5);
TH2D *hTotal_TrigVsPixelTracks = new TH2D("hTotal_TrigVsPixelTracks", "hTotal_TrigVsPixelTracks", trigger_size, 0, trigger_size, 5, 0, 5);

void get_RecoTrack()
{
	TFile file("0000/step3_STARlight_Reco_10.root");

	fwlite::Event ev(&file);

	fwlite::Handle<vector<reco::Track>> pixel_tracks;
	fwlite::Handle<edm::TriggerResults> triggerResults;
	
	pixel_tracks.getByLabel(ev, "hiConformalPixelTracks");
	triggerResults.getByLabel(ev, "TriggerResults", "", "HLT");

	edm::TriggerNames triggerNames = ev.triggerNames(*triggerResults);

	//find the index of the trigger
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

	for( ev.toBegin(); ! ev.atEnd(); ++ev)
	{
		//skip the event without the track
		if (pixel_tracks->size() == 0) continue;
		// cout << "pixel track size: " << pixel_tracks->size() << endl;
		// cout << "pass trigger: " << triggerResults->accept(triggers_Idx[0]) << endl;
		for( int i = 0; i < triggers_Idx.size(); i++)
		{
			//fill the total number of pixel tracks
			hTotal_TrigVsPixelTracks->Fill(i, pixel_tracks->size());

			//fill the number of pixel tracks that pass the trigger
			if( triggerResults->accept(triggers_Idx[i]) )
			{
				hPass_TrigVsPixelTracks->Fill(i, pixel_tracks->size());
			}
		}
	}

	//print the pass rate for each trigger
	for (int i = 0; i < trigger_size; i++)
	{
		cout << "trigger: " << triggers[i] << " 0 track pass rate: " << hPass_TrigVsPixelTracks->GetBinContent(i+1, 1) / hTotal_TrigVsPixelTracks->GetBinContent(i+1, 1) << endl;
		cout << "trigger: " << triggers[i] << " 1 track pass rate: " << hPass_TrigVsPixelTracks->GetBinContent(i+1, 2) / hTotal_TrigVsPixelTracks->GetBinContent(i+1, 2) << endl;
		cout << "trigger: " << triggers[i] << " 2 track pass rate: " << hPass_TrigVsPixelTracks->GetBinContent(i+1, 3) / hTotal_TrigVsPixelTracks->GetBinContent(i+1, 3) << endl;
		cout << "trigger: " << triggers[i] << " 3 track pass rate: " << hPass_TrigVsPixelTracks->GetBinContent(i+1, 4) / hTotal_TrigVsPixelTracks->GetBinContent(i+1, 4) << endl;
		cout << "trigger: " << triggers[i] << " 4 track pass rate: " << hPass_TrigVsPixelTracks->GetBinContent(i+1, 5) / hTotal_TrigVsPixelTracks->GetBinContent(i+1, 5) << endl;
	}
}