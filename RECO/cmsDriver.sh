# RAW - AOD and MINIAOD
# cmsDriver.py reco  --conditions 132X_dataRun3_Express_v4 -s RAW2DIGI,L1Reco,RECO,PAT --datatier RECO,MINIAOD --eventcontent AOD,MINIAOD --data  --process RECO --scenario pp --customise Configuration/DataProcessing/RecoTLR.customisePostEra_Run3 --no_exec --era Run3_2023 --repacked

# RAW - AOD
cmsDriver.py reco  --conditions 132X_dataRun3_Express_v4 -s RAW2DIGI,L1Reco,RECO,PAT --datatier RECO --eventcontent AOD --data  --process reRECO --scenario pp --customise Configuration/DataProcessing/RecoTLR.customisePostEra_Run3 --no_exec --era Run3_2023 --repacked --filein file:/eos/cms/store/t0streamer/Data/PhysicsHIForward0/000/374/588/run374588_ls0133_streamPhysicsHIForward0_StorageManager.dat -n 10

# process.source = cms.Source("NewEventStreamFileReader",
    # fileNames = cms.untracked.vstring('file:/eos/cms/store/t0streamer/Data/PhysicsHIPhysicsRawPrime0/000/373/870/run373870_ls0037_streamPhysicsHIPhysicsRawPrime0_StorageManager.dat')
# )

# process.GlobalTag.toGet = cms.VPSet(
#   cms.PSet(record = cms.string("HeavyIonRPRcd"),
#            tag = cms.string("HeavyIonRPRcd_75x_v0_prompt"),
#            label = cms.untracked.string(''),
#            connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
#            )
# )
