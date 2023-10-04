#!/bin/bash
shopt -s expand_aliases

SCRAM_ARCH=el8_amd64_gcc11
cd $1
eval `/cvmfs/cms.cern.ch/common/scram runtime -sh`

cd $CMSSW_BASE/src/HLTrigger/Configuration/test/workstation

#wget https://adiflori.web.cern.ch/adiflori/hlt_hion.py
#cp hlt_hion.py test_pset.py
source $2 > test_pset.py
HLTCONFIGFILE="test_pset.py"

sed -i "/add\ specific/,+16d" $HLTCONFIGFILE

echo '
# Define the output
process.output = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring("drop *", "keep *_TriggerResults_*_*", "keep *_hltPixelTracksPPOnAA_*_*", "keep *_hltPixelTracksLowPtForUPCPPOnAA_*_*", "keep *_hltPixelTracksForUPCPPOnAA_*_*", "*_hltSiPixelClustersPPOnAA_*_*"),
    fileName = cms.untracked.string("output_130.root"),
)
process.output_path = cms.EndPath(process.output)

process.schedule.append( process.output_path )  # enable this line if you want to get an output containing trigger info to be used for further analysis, e.g. "TriggerResults", digis etc

#process.GlobalTag.toGet = cms.VPSet(
#  cms.PSet(record = cms.string("SiPixelQualityFromDbRcd"),
#           tag = cms.string("SiPixelQuality_phase1_2023_v5_mc"),
#           label = cms.untracked.string("forRawToDigi"),
#           connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
#           ),
#  cms.PSet(record = cms.string("SiPixelQualityFromDbRcd"),
#           tag = cms.string("SiPixelQuality_phase1_2023_v5_mc"),
#           label = cms.untracked.string(""),
#           connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
#           )
#)

#process.siPixelQualityESProducer.siPixelQualityLabel = "forDigitizer"
' >> $HLTCONFIGFILE

isData=${3:-0}
if [ $isData -eq 2023 ]
then
echo '
# use rawDataRepacker and skip zero suppression
from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag
massSearchReplaceAnyInputTag(process.SimL1Emulator, cms.InputTag("rawDataCollector","","@skipCurrentProcess"), cms.InputTag("rawDataRepacker","","@skipCurrentProcess"))
' >> $HLTCONFIGFILE
fi

if [ $isData -eq 2022 ]
then
echo '
# use rawDataRepacker and skip zero suppression
from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag
massSearchReplaceAnyInputTag(process.SimL1Emulator, cms.InputTag("rawDataCollector","","@skipCurrentProcess"), cms.InputTag("rawDataRepacker","","@skipCurrentProcess"))
' >> $HLTCONFIGFILE
fi

if [ $isData -eq 2018 ]
then
echo '
# adapt the HCAL configuration to run over 2018 data
from HLTrigger.Configuration.customizeHLTforCMSSW import customiseFor2018Input
process = customiseFor2018Input(process)
process.hltHcalDigis.saveQIE10DataNSamples = cms.untracked.vint32(10)
process.hltHcalDigis.saveQIE10DataTags = cms.untracked.vstring("MYDATA")

from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag
massSearchReplaceAnyInputTag(process.SimL1Emulator, cms.InputTag("rawDataCollector","","@skipCurrentProcess"), cms.InputTag("rawDataRepacker","","@skipCurrentProcess"))
#massSearchReplaceAnyInputTag(process.HLTFullIterativeTrackingPPOnAAForDmeson, cms.InputTag("highPurity","",""), cms.InputTag("loose","",""))
process.hltHITrackingSiStripRawToClustersFacilityZeroSuppression.DigiProducersList= cms.VInputTag("hltSiStripRawToDigi:ZeroSuppressed")
process.hltHITrackingSiStripRawToClustersFacilityFullZeroSuppression.DigiProducersList = cms.VInputTag("hltSiStripRawToDigi:ZeroSuppressed")
for lbl in ["hltSiStripZeroSuppression", "hltSiStripDigiToZSRaw", "rawDataRepacker", "hltSiStripClusterizerForRawPrime","hltSiStripClusters2ApproxClusters"]:
    delattr(process, lbl)

from HLTrigger.Configuration.common import producers_by_type
#for producer in producers_by_type(process, "PoolDBESSource"):
process.GlobalTag.toGet = cms.VPSet(
  cms.PSet( record = cms.string( "BeamSpotObjectsRcd" ),
    refreshTime = cms.uint64( 2 )
  )
)
' >> $HLTCONFIGFILE
fi
