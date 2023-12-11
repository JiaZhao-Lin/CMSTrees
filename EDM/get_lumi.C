// #include "FWCore/FWLite/FWLite.h"
// #include "DataFormats/Luminosity/interface/OnlineLuminosityRecord.h"

// int get_lumi() {
//   TString inputFileName = "./0000/reco_RAW2DIGI_L1Reco_RECO_100.root";

//   // Initialize FWLite
//   FWLite::AutoLibraryLoader loader;

//   // Open the root file
//   TFile* file = TFile::Open(inputFileName);
//   if (!file->IsOpen()) {
//     std::cerr << "Error opening root file: " << inputFileName << std::endl;
//     return 1;
//   }

//   // Get the luminosity record
//   edm::LumiInfo lumiInfo;
//   edm::Handle<OnlineLuminosityRecord> lumiRec;
//   edm::InputTag lumiTag("lumiProducer", "online");
//   file->GetObject(lumiTag.label(), lumiRec);
//   if (!lumiRec.isValid()) {
//     std::cerr << "Error getting luminosity record: " << lumiTag << std::endl;
//     return 1;
//   }
//   lumiInfo = lumiRec->get(1);

//   // Calculate the total luminosity
//   double totalLuminosity = lumiInfo.integratedLuminosity();

//   // Print the total luminosity
//   std::cout << "Total luminosity: " << totalLuminosity << " pb^-1" << std::endl;

//   // Close the root file
//   file->Close();

//   return 0;
// }