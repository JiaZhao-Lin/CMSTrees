#ifndef CONSTANTS_H
#define CONSTANTS_H

const double PI					= TMath::Pi();

const double MASS_KAON			= 0.493677;
const double PDG_KAON			= 321;
const double PDG_KAON_MINUS		= -321;
const double PDG_KAON_PLUS		= 321;

const double MASS_K0			= 0.497614;
const double PDG_K0				= 311;

const double MASS_PHI			= 1.019455;
const double PDG_PHI			= 333;

const double MASS_JPSI			= 3.096916;
const double PDG_JPSI			= 443;

const double MASS_PSI2S			= 3.686109;
const double PDG_PSI2S			= 100443;

const double MASS_MUON			= 0.105658;
const double PDG_MUON			= 13;
const double PDG_MUON_MINUS		= -13;
const double PDG_MUON_PLUS		= 13;

const TString HLT_TRIG_LIST[]	= 
{
	"HLT_HIUPC_SingleMuOpen_NotMBHF2AND_v", // UPC muons
    "HLT_HIUPC_SingleMuOpen_NotMBHF2AND_MaxPixelTrack_v",   //2
    "HLT_HIUPC_SingleMuOpen_NotMBHF2OR_v",
    "HLT_HIUPC_SingleMuOpen_NotMBHF2OR_MaxPixelTrack_v",    //4
    "HLT_HIUPC_SingleMuOpen_BptxAND_MaxPixelTrack_v",
    "HLT_HIUPC_SingleMuOpen_OR_SingleMuCosmic_EMTF_NotMBHF2AND_v",  //6
    "HLT_HIUPC_SingleMuOpen_OR_SingleMuCosmic_EMTF_NotMBHF2AND_MaxPixelTrack_v",
    "HLT_HIUPC_SingleMuOpen_OR_SingleMuCosmic_EMTF_NotMBHF2OR_v",   //8
    "HLT_HIUPC_SingleMuOpen_OR_SingleMuCosmic_EMTF_NotMBHF2OR_MaxPixelTrack_v",
    "HLT_HIUPC_SingleMuOpen_OR_SingleMuCosmic_EMTF_BptxAND_MaxPixelTrack_v",    //10
    "HLT_HIUPC_SingleMuCosmic_NotMBHF2AND_v",
    "HLT_HIUPC_SingleMuCosmic_NotMBHF2AND_MaxPixelTrack_v",   //12
    "HLT_HIUPC_SingleMuCosmic_NotMBHF2OR_v",
    "HLT_HIUPC_SingleMuCosmic_NotMBHF2OR_MaxPixelTrack_v",  //14
    "HLT_HIUPC_SingleMuCosmic_BptxAND_MaxPixelTrack_v",
    "HLT_HIUPC_DoubleMuOpen_NotMBHF2AND_v",
    "HLT_HIUPC_DoubleMuOpen_NotMBHF2AND_MaxPixelTrack_v",   //18
    "HLT_HIUPC_DoubleMuOpen_BptxAND_MaxPixelTrack_v",
    "HLT_HIUPC_DoubleMuCosmic_NotMBHF2AND_v",   //20
    "HLT_HIUPC_DoubleMuCosmic_NotMBHF2AND_MaxPixelTrack_v",
    "HLT_HIUPC_DoubleMuCosmic_BptxAND_MaxPixelTrack_v",   //22
};

const TString HLT_TRIG_LIST_2018[]	= 
{
	"HLT_HIL1DoubleMuOpen_OS_Centrality_40_100_v", // Peripheral OS dimuons
	"HLT_HIL1DoubleMuOpen_Centrality_50_100_v", // Peripheral dimuons
	"HLT_HIL3Mu2p5NHitQ10_L2Mu2_M7toinf_v", // Bottomonia
	"HLT_HIL1DoubleMu10_v", // Z bosons
	"HLT_HIUPC_DoubleMu0_NotMBHF2AND_v", // UPC dimuons
	// Single muon triggers
	"HLT_HIL1MuOpen_Centrality_80_100_v", // Peripheral muons
	"HLT_HIL3Mu12_v", // Electroweak bosons
	"HLT_HIUPC_SingleMuOpen_NotMBHF2AND_v", // UPC muons
	"HLT_HIL3Mu3_NHitQ10_v1", // Low pT muons
};

#endif