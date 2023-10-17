# RAW - AOD and MINIAOD, pp reco
# cmsDriver.py reco  --conditions 132X_dataRun3_Express_v4 -s RAW2DIGI,L1Reco,RECO,PAT --datatier RECO,MINIAOD --eventcontent AOD,MINIAOD --data  --process RECO --scenario pp --customise Configuration/DataProcessing/RecoTLR.customisePostEra_Run3 --no_exec --era Run3_2023 --repacked

# RAW - AOD, pp reco
# cmsDriver.py reco  --conditions 132X_dataRun3_Express_v4 -s RAW2DIGI,L1Reco,RECO --datatier RECO --eventcontent AOD --data  --process RECO --scenario pp --customise Configuration/DataProcessing/RecoTLR.customisePostEra_Run3 --no_exec --era Run3_2023 --repacked --filein file:/eos/cms/tier0/store/hidata/HIRun2023A/HIForward0/RAW/v1/000/374/666/00000/010de9da-f6e1-41d0-9d7e-d17c0d726b6f.root --nThreads 4 -n 10

# RAW - AOD, HI reco
cmsDriver.py reco  --conditions 132X_dataRun3_Express_v4 -s RAW2DIGI,L1Reco,RECO --datatier RECO --eventcontent AOD --data  --process RECO --scenario pp --customise Configuration/DataProcessing/RecoTLR.customisePostEra_Run3 --no_exec --era Run3_pp_on_PbPb_2023 --repacked --filein file:/eos/cms/store/t0streamer/Data/PhysicsHIForward0/000/374/681/run374681_ls0100_streamPhysicsHIForward0_StorageManager.dat --nThreads 4 -n 10

# RAW - AOD, pp reco from root file
cmsDriver.py reco  --conditions 132X_dataRun3_Express_v4 -s RAW2DIGI,L1Reco,RECO --datatier RECO --eventcontent AOD --data  --process RECO --scenario pp --customise Configuration/DataProcessing/RecoTLR.customisePostEra_Run3 --no_exec --era Run3_2023 --repacked --filein file:/eos/cms/store/t0streamer/Data/PhysicsHIForward0/000/374/803/run374803_ls0284_streamPhysicsHIForward0_StorageManager.dat --nThreads 4 -n 10 

cmsDriver.py reco  --conditions 132X_dataRun3_Prompt_v4 -s RAW2DIGI,L1Reco,RECO --datatier RECO --eventcontent AOD --data  --process RECO --scenario pp --customise Configuration/DataProcessing/RecoTLR.customisePostEra_Run3 --no_exec --era Run3_2023 --repacked --filein file:/afs/cern.ch/user/j/jiazhao/RecoProductions/CMSSW_13_2_5_patch1/src/Reco/skim_RAW_ForUPCPhi.root --nThreads 4 -n 10



# change the following line if using .dat
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
