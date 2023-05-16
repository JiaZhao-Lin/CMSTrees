#ifndef CONSTANTS_H
#define CONSTANTS_H


const double Mmuon = 0.1056583745;
const double Mpion = 0.13957018;
const double Mkaon = 0.493677;
const double Mproton = 0.938272081;
const double Melectron = 0.0005109989461;

const double PI = TMath::Pi();

// 0 HLT_HIL1DoubleMuOpen_OS_Centrality_40_100 (HIDoubleMuon, HIDoubleMuonPsiPeri) -> Events with at least two opposite electric-charge L1 muons with loose trigger quality (requires at least two muon stations with measurements) within a centrality percentile range between 40-100%
// 1 HLT_HIL1DoubleMuOpen_Centrality_50_100 (HIDoubleMuon, HIDoubleMuonPsiPeri) -> Events with at least two L1 muons with loose trigger quality (requires at least two muon stations with measurements) within a centrality percentile range between 50-100%
// 4 HLT_HIUPC_DoubleMu0_NotMBHF2AND (HIForward) -> Events with at least two L1 muons with medium trigger quality  (requires at least three muon stations with measurements) with low energy in the HF calorimeters (NotMBHF2AND means !(HF+ > 15 ADC Counts  AND  HF- > 19 ADC Counts))
// 5 HLT_HIL1MuOpen_Centrality_80_100 (HIForward) -> Events with at least one L1 muon with loose trigger quality  (requires at least two muon stations with measurements) within a centrality percentile range between 80-100%
// 7 HLT_HIUPC_SingleMuOpen_NotMBHF2AND -> Events with at least one L1 muon with loose trigger quality  (requires at least two muon stations with measurements) within low energy in the HF calorimeters (NotMBHF2AND means !(HF+ > 15 ADC Counts  AND  HF- > 19 ADC Counts))
const int nTrigs = 8;

//const int   trigIdx = 4;
//const TString trigName = "DoubleMuUPC";

const int   trigIdx = 7;
const TString trigName = "SingleMuUPC";

const int mRunNbCut = 326776;     // get this number from Quan (L = 1570.6796 ub^{-1} for runID >= 326776)
// const double mCMSLum = 1570.6796;  // ub^{-1} for HLT_HIUPC_SingleMuOpen_NotMBHF2AND_v with runID >= 326776
const double mCMSLum = 1520.3;  // ub^{-1} for HLT_HIUPC_SingleMuOpen_NotMBHF2AND_v with runID >= 326776
const double Lumi_Uncer = 1.5;    // 1.5% uncertainty on luminosity

const int  mCentralityCut = 180;
const int  mNtrkofflineCut = 2;

const int    nDirs = 2;        // 0: ZDC-Plus; 1: ZDC-Minus;
const TString  mDir[nDirs]        = {"Plus", "Minus"};
const double mHFsumETCut[nDirs] = {12,    12    };
const double mZdcFitLow[nDirs]  = {4.2e3, 6.e3  };
const double mZdcFitHi[nDirs]   = {25.e3, 37.5e3};

//const int nNeus = 3;
const int nNeus = 2;
const int nNeuMults = nNeus*(nNeus+1)/2;

//const double mNeuZDCLow[nDirs][nNeus] = {
//    {0, 4.2e3, 10.e3},
//    {0, 6.0e3, 16.e3}
//};
//const double mNeuZDCHi[nDirs][nNeus] = {
//    {4.2e3, 10.e3, 1.e6},
//    {6.0e3, 16.e3, 1.e6}
//};

const double mNeuZDCLow[nDirs][nNeus] = 
{
    {0, 4.2e3},
    {0, 6.0e3}
};

const double mNeuZDCHi[nDirs][nNeus] = 
{
    {4.2e3, 1.e6},
    {6.0e3, 1.e6}
};

const double mTwoNeuZDCLow[nDirs] = {11e3, 17.5e3};
const double mTwoNeuZDCHi[nDirs]  = {17e3, 27.0e3};

//const int    nRapBins = 16;
//const double mRapLow[nRapBins] = {-2.4, -2.3, -2.2, -2.1, -2.0, -1.9, -1.8, -1.7, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3};
//const double mRapHi[nRapBins]  = {-2.3, -2.2, -2.1, -2.0, -1.9, -1.8, -1.7, -1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4};

const int    nRapBins          = 12;
const double mRapLow[nRapBins] = {-2.4, -2.2, -2.1, -2.0, -1.9, -1.8, 1.6, 1.8, 1.9, 2.0, 2.1, 2.2};
const double mRapHi[nRapBins]  = {-2.2, -2.1, -2.0, -1.9, -1.8, -1.6, 1.8, 1.9, 2.0, 2.1, 2.2, 2.4};

const int    nDiffRapBins = 6;
const double mDiffRapLow[nDiffRapBins] = {-2.4, -2.1, -1.9, 1.6, 1.9, 2.1};
const double mDiffRapHi[nDiffRapBins]  = {-2.1, -1.9, -1.6, 1.9, 2.1, 2.4};

// const int    nDiffRapBins                = 4;
// const double mDiffRapLow[nDiffRapBins]   = {-2.4, -2.0, 1.6, 2.0};
// const double mDiffRapHi[nDiffRapBins]    = {-2.0, -1.6, 2.0, 2.4};

// const double mDiffRapBds[nDiffRapBins+2] = {-2.4, -2.0, -1.6, 1.6, 2.0, 2.4};
const double mDiffRapBds[nDiffRapBins+2] = {-2.4, -2.1, -1.9, -1.6, 1.6, 1.9, 2.1, 2.4};

const double mMassLow4MuonAccStudy = 2;
const double mMassHi4MuonAccStudy  = 5;

//const double mJpsiMassLow = 2.85;
//const double mJpsiMassHi  = 3.35;
const double mJpsiMassLow   = 2.95;
const double mJpsiMassHi    = 3.25;
const double mLowMassBandLow = 2.75; //= 2.75;
const double mLowMassBandHi  = 2.90; //= 2.9;
const double mHiMassBandLow  = 3.30; //= 3.3;
const double mHiMassBandHi   = 3.45; //= 3.45;
const double mPsiMassLow     = 3.50;
const double mPsiMassHi      = 3.80;

const double mPairYCut   = 2.4;
const double mAlphaCut   = 6.e-3;
const double mPtCut4Coh  = 0.20; //Coh signals only dominate for low pt region, so set a cutoff

const int    nSmearScan  = 200;
const double mInitPar0   = 0.01;
const double mSmearStep  = 5.e-5;
const double mPar0       = 0.0145;

const double mJpsi_PDG = 3.0969; const double  mPsi_PDG = 3.686097;
//Decay branch ratios from PDG
const double br_Jpsi2uu  = 0.05961;
const double br_Psi2uu   = 0.0080;
const double br_Psi2Jpsi = 0.6140;
const double br_Jpsi2uu_Uncer = 0.033/5.961 * 100.0;    // branching ratio uncertainty in percentage


#endif