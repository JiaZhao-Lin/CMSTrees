import FWCore.ParameterSet.Config as cms


process = cms.Process("SKIM")

process.load("Configuration.StandardSequences.Services_cff")

# Load the RAW data file
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring("file:/eos/cms/store/group/phys_heavyions/jiazhao/Data_Run3/HIForward/7efdd2e1-1597-424f-b77f-22935545155d.root")
)
# process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

# Limit the output messages
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 200
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.options.numberOfThreads=cms.untracked.uint32(1)

# Configure the HLT filter
process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
process.hltHighLevel = cms.EDFilter("HLTHighLevel",
    TriggerResultsTag = cms.InputTag("TriggerResults", "", "HLT"),
    HLTPaths = cms.vstring(
		"HLT_HIZeroBias_v*",
		"HLT_HIZeroBias_HighRate_v*",
		# "HLT_HIUPC_ZeroBias_SinglePixelTrack_MaxPixelTrack_v*",
    	# "HLT_HIUPC_ZeroBias_SinglePixelTrackLowPt_MaxPixelCluster400_v*",
    	# "HLT_HIUPC_ZeroBias_MinPixelCluster400_MaxPixelCluster10000_v*",
		# "HLT_HIUPC_ZDC1nOR_SinglePixelTrack_MaxPixelTrack_v*",
		# "HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_v*",
		# "HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_v*",
	),
    andOr = cms.bool(True),  # The filter will pass if any of the specified trigger conditions are satisfied
	throw = cms.bool(False)  # True if an exception should be thrown if the specified paths are not found
)


# Configure the output file with the skimmed data
# process.TFileService = cms.Service("TFileService", fileName = cms.string("skim_RAW_ForUPCPhi.root"))

# Define the path that applies the HLT filter
process.hltTrigPath = cms.Path(process.hltHighLevel)

# Configure the output file with the skimmed data
process.output = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string("skim_Trig.root"),
	SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('hltTrigPath')
    ),
)

# Endpath saves the skimmed data to the output file
process.out = cms.EndPath(process.output)

# Schedule the path and endpath
process.schedule = cms.Schedule(
    process.hltTrigPath,
    process.out
)