cmsDriver.py Configuration/GenProduction/python/HINPbPb_Run3_STARlight_fragment.py --filein file:/eos/cms/store/group/phys_heavyions/jiazhao/CMSTrees/generateLHE/CohPhi/lheFiles/slight_CohPhi_0001.lhe --filetype LHE --fileout file:step1_STARlight_LHE_GenSim.root --mc --eventcontent RAWSIM --no_exec --datatier GEN-SIM --conditions 132X_mcRun3_2023_realistic_HI_v9 --beamspot Realistic2023PbPbCollision --step GEN,SIM --nThreads 1 --scenario pp --geometry DB:Extended --era Run3_2023_UPC --python_filename step1_STARlight_LHE_GenSim_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 100

cmsDriver.py step2 --filein file:step1_STARlight_LHE_GenSim.root --fileout step2_STARlight_Digi.root --mc --eventcontent RAWSIM --pileup HiMixNoPU --datatier GEN-SIM-RAW --conditions 132X_mcRun3_2023_realistic_HI_v9 --step DIGI:pdigi_hi_nogen,L1,DIGI2RAW,HLT:HIon --nThreads 1 --geometry DB:Extended --era Run3_2023_UPC --python_filename step2_STARlight_Digi_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n -1

cmsDriver.py step3 --filein file:step2_STARlight_Digi.root --fileout step3_STARlight_Reco.root --mc --eventcontent AODSIM --datatier AODSIM --conditions 132X_mcRun3_2023_realistic_HI_v9 --step RAW2DIGI,L1Reco,RECO --nThreads 1 --era Run3_2023_UPC --python_filename step3_STARlight_Reco_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n -1