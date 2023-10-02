#include "DataFormats/FWLite/interface/Handle.h"

void get_trigRate()
{
	// TFile file("./output_132X.root");
	TFile file("/eos/cms/store/group/phys_heavyions/jiazhao/TestData/HLTTree/HLT_ZB_132X_test_230917_010802/HITestRaw0/HLT_ZB_132X_test_230917_010802/230916_230836/output_132X.root");

	fwlite::Event ev(&file);

	float total = 0;

	std::vector<TString> triggers =
	{
		"HLT_HIZeroBias_v7",
		"HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v5",
		"HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v5",
		"HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v5"
	};
	std::vector<float> fired(triggers.size(), 0);
	std::vector<int> triggers_Idx;

	fwlite::Handle<edm::TriggerResults> triggerResults;
	triggerResults.getByLabel(ev, "TriggerResults", "", "MyHLT");
	edm::TriggerNames triggerNames = ev.triggerNames(*triggerResults);

	//find the index of the trigger
	for( int i = 0; i < triggers.size(); i++)
	{
		cout << "trigger: " << triggers[i] << " index: " << triggerNames.triggerIndex(triggers[i]) << endl;
		triggers_Idx.push_back(triggerNames.triggerIndex(triggers[i]));
	}
	
	for( ev.toBegin(); ! ev.atEnd(); ++ev)
	{
		total++;
		triggerResults.getByLabel(ev, "TriggerResults", "", "MyHLT");

		for( int i = 0; i < triggers_Idx.size(); i++)
		{
			if( triggerResults->accept(triggers_Idx[i]) ) fired[i]++;
		}
	}

	cout << "total: " << total << endl;
	for( int i = 0; i < triggers.size(); i++)
	{
		cout << "trigger: " << triggers[i] << " fired: " << fired[i] << " pass rate: " << fired[i]/total << endl;
	}

	// int trigger0_index = 25;
	// int trigger1_index = 80;
	// int trigger2_index = 81;
	// int trigger3_index = 82;

	// for( ev.toBegin(); ! ev.atEnd(); ++ev)
	// {
	// 	total++;
	// 	fwlite::Handle<edm::TriggerResults> triggerResults;
	// 	triggerResults.getByLabel(ev, "TriggerResults", "", "MyHLT");

	// 	if( triggerResults->accept(trigger0_index) ) n0++;
	// 	if( triggerResults->accept(trigger1_index) ) n1++;
	// 	if( triggerResults->accept(trigger2_index) ) n2++;
	// 	if( triggerResults->accept(trigger3_index) ) n3++;

	// }

	// cout << "total: " << total << endl;
	// cout << "HLT_HIZeroBias_v7: " << n0 << " pass rate: " << n0/total << endl;
	// cout << "HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v5: " << n1 << " pass rate: " << n1/total << endl;
	// cout << "HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v5: " << n2 << " pass rate: " << n2/total << endl;
	// cout << "HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v5: " << n3 << " pass rate: " << n3/total << endl;
}

