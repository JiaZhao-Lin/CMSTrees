cmsDriver.py Configuration/GenProduction/python/HINPbPbAutumn18GS_STARlight_fragment_phi.py --filein file:/eos/cms/store/group/phys_heavyions/jiazhao/starlightLHE_OG/generateLHE/CohPhi/lheFiles/slight_CohPhi_0001.lhe --filetype LHE --fileout file:step1_STARlight_LHE_GenSim.root --mc --eventcontent RAWSIM --no_exec --datatier GEN-SIM --conditions 103X_upgrade2018_realistic_HI_v11 --beamspot RealisticPbPbCollision2018 --step GEN,SIM --nThreads 1 --scenario HeavyIons --geometry DB:Extended --era Run2_2018_pp_on_AA --python_filename step1_STARlight_LHE_GenSim_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n -1

cmsDriver.py step2 --filein file:step1_STARlight_LHE_GenSim.root --fileout step2_STARlight_Digi.root --mc --eventcontent RAWSIM --pileup HiMixNoPU --datatier GEN-SIM-RAW --conditions 103X_upgrade2018_realistic_HI_v11 --step DIGI:pdigi_hi_nogen,L1,DIGI2RAW,HLT:HIon --nThreads 1 --geometry DB:Extended --era Run2_2018_pp_on_AA --python_filename step2_STARlight_Digi_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n -1

cmsDriver.py step3 --filein file:step2_STARlight_Digi.root --fileout step3_STARlight_Reco.root --mc --eventcontent AODSIM --runUnscheduled --datatier AODSIM --conditions 103X_upgrade2018_realistic_HI_v11 --step RAW2DIGI,L1Reco,RECO --nThreads 1 --era Run2_2018_pp_on_AA --python_filename step3_STARlight_Reco_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n -1