#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
// #include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

TH1D *hNum_nTracks = new TH1D("hNum_nTracks", "hNum_nTracks; nTracks", 20, 0, 20);
TH1D *hDen_nTracks = new TH1D("hDen_nTracks", "hDen_nTracks; nTracks", 20, 0, 20);
TH1D *hEff_nTracks = new TH1D("hEff_nTracks", "hEff_nTracks; nTracks; Efficiency", 20, 0, 20);

void test1()
{
	TFile file("output_132X_correct.root");

	TString trigger_name_ZB = "HLT_HIZeroBias_HighRate_v1";
	TString trigger_name = "HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v8";
	fwlite::Event ev(&file);
	int ientry = -1;
	for( ev.toBegin(); ! ev.atEnd(); ++ev)
	{
		ientry++;
		// if (ientry > 10) break;
		cout << "====================> Processing event: " << ientry << endl;

		fwlite::Handle<edm::TriggerResults> triggerResults;
		triggerResults.getByLabel(ev, "TriggerResults", "", "MyHLT");
		edm::TriggerNames triggerNames = ev.triggerNames(*triggerResults);

		//print all trigger names
		// for (unsigned int i = 0; i < triggerNames.size(); ++i)
		// {
		// 	cout << triggerNames.triggerName(i) << endl;
		// }

		int index_ZB = triggerNames.triggerIndex(trigger_name_ZB);
		int index = triggerNames.triggerIndex(trigger_name);

		// cout << "trigger: " << trigger_name_ZB << " index: " << index_ZB << endl;
		// cout << "trigger: " << trigger_name << " index: " << index << endl;

		// skip if the ZB trigger is not fired
		if (!triggerResults->accept(index_ZB)) continue;

		// if (triggerResults->accept(index)) cout << "============================>trigger fired!" << endl;
		//get number of pixel tracks
		fwlite::Handle<vector<reco::Track>> nTracks;
		nTracks.getByLabel(ev, "hltPixelTracksLowPtForUPCPPOnAA", "", "MyHLT");
		if (!nTracks.isValid())
		{
			cout << "Invalid product, skipping event" << endl;
			continue;
		}
		cout << "nTracks: " << nTracks.ptr()->size() << endl;

		hDen_nTracks->Fill(nTracks.ptr()->size());
		if (triggerResults->accept(index)) hNum_nTracks->Fill(nTracks.ptr()->size());
	}
}