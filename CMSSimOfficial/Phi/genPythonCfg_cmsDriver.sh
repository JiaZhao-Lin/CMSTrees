cmsDriver.py Configuration/GenProduction/python/HINPbPb_Run3_STARlight_fragment.py --fileout file:step1_STARlight_LHE_GenSim.root --mc --eventcontent RAWSIM --no_exec --datatier GEN-SIM --conditions 132X_mcRun3_2023_realistic_HI_v2 --beamspot Realistic2022PbPbCollision --step LHE,GEN,SIM --nThreads 1 --scenario HeavyIons --geometry DB:Extended --era Run3_pp_on_PbPb --python_filename step1_STARlight_LHE_GenSim_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 100

cmsDriver.py step2 --filein file:step1_STARlight_LHE_GenSim.root --fileout step2_STARlight_Digi.root --mc --eventcontent RAWSIM --pileup HiMixNoPU --datatier GEN-SIM-RAW --conditions 132X_mcRun3_2023_realistic_HI_v2 --step DIGI:pdigi_hi_nogen,L1,DIGI2RAW,HLT:HIon --nThreads 1 --geometry DB:Extended --era Run3_pp_on_PbPb --python_filename step2_STARlight_Digi_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n -1

cmsDriver.py step3 --filein file:step2_STARlight_Digi.root --fileout step3_STARlight_Reco.root --mc --eventcontent AODSIM --datatier AODSIM --conditions 132X_mcRun3_2023_realistic_HI_v2 --step RAW2DIGI,L1Reco,RECO --nThreads 1 --era Run3_pp_on_PbPb --python_filename step3_STARlight_Reco_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n -1