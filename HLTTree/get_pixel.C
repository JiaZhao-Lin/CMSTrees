#include "DataFormats/FWLite/interface/Handle.h"

void update_progress(int ientry, int total_entries, int percentage_increment)
{
	if (ientry % (total_entries / percentage_increment) == 0)
	{
		std::cout << "Processing " << ientry << "th entry... (" << (int)((double)ientry / total_entries * 100) << "%)" << std::endl;
	}
}

void get_pixel()
{
	TFile file("/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/HLTTree/HLT_STARlight_CohJpsiToMuMu_132X_230831_220632/CRAB_UserFiles/HLT_STARlight_CohJpsiToMuMu_132X_230831_220632/230831_200651/output_132X.root");

	fwlite::Event ev(&file);

	TFile *outfile = new TFile("Pixel.root", "RECREATE");
	std::vector<int> nHit;
	int nTrack;

	TTree *myTree = new TTree("Pixel", "My Tree");
	myTree->Branch("nHit", &nHit);
	myTree->Branch("nTrack", &nTrack, "nTrack/I"); // "I" indicates integer data type

	int ientry = -1;
	int total_entries = ev.size();

	for( ev.toBegin(); ! ev.atEnd(); ++ev) {
		// clear the vector
		nHit.clear();
		
		ientry++;
		update_progress(ientry, total_entries, 10);

		fwlite::Handle<vector<reco::Track> > objs;
		objs.getByLabel(ev,"hltPixelTracksPPOnAA");
		// now can access data
		// std::cout <<" size "<<objs.ptr()->size()<<std::endl;
		nTrack = objs.ptr()->size();
		for ( auto o: *objs)
		{
			// cout<<o.numberOfValidHits()<<endl;
			nHit.push_back( o.numberOfValidHits() );
		}
		myTree->Fill();
	}

	myTree->Write();
	outfile->Close();
}