# READ ME

This repository contains code for opening ROOT trees.

# STARlight Simulation

## 1. Install STARlight
One can either execute "./compileSTARlight.sh 313" to automatically compile the STARlight v3.13 
or compile STARlight step by step following the instruction in 'https://starlight.hepforge.org'
       
1. Setup the configuration/input file (slight.in)
	* `cd starlightTrunk_v313/build`
    * `cp ../config/slight.in .` (In 'starlightLHE/cohJpsiConfigAt5p02TeV' directory, there is a pre-defined input file for coherent Jpsi in PbPb@5.02 TeV with interference turned on)
       c) set the parameters according to your need. Please see details in p6-12 of Readme.pdf under 'starlightTrunk_v313' directory or section 7 of the STAR manual [Comput. Phys. Commun. 212 (2017) 258].

1. Generate STARlight output: 
    * one can directly execute `./starlight` under 'starlightTrunk_v313/build' directory to generate STARlight output (slight.out) after finishing previous steps.Please see the desciption of output in 'https://starlight.hepforge.org' or section 8 of the STAR manual [Comput. Phys. Commun. 212 (2017) 258].
    * there is a simple macro, sitting in 'starlightLHE/anaSTARlightOutput' directory, to analyze STARlight output
    

## 2. Convert plain STARlight output file to CMS LHE file

(take coherent Jpsi as an example)

To save computing time, the plain STARlight output file with large statistics should be splitted into many small files for job submission in lxplus cluster. The most time-consuming step in CMS simulation chain (GENSIM->DIGIRAW->RECO) is DIGIRAW, the input statistics of DIGIRAW step is better not to exceed 2000 events for UPC simulation without embedding. One could apply filters in 'STARlight output file --> LHE file' and GENSIM steps, therefore, one needs to evaluate the statistics for each small plain STARlight output file.

1. make a directory (e.g., `mkdir CohJpsi`) under generateLHE directory, copy the generated STARlight output file to 'CohJpsi' directory, and rename the STARlight output file (e.g., `mv slight.out slight.CohJpsi.out`). It is better to also copy the STARlight configuration file (slight.in) to CohJpsi directory, which can remind people the exact settings in the future.

1. under 'generateLHE' directory, execute `./splitSTARlight.sh 4000 CohJpsi` to split the large STARlight output file (4000 means each small file has 1000 STARlight events). This script simply call the 'split' command. -- NOTE, if one runs this script in macOS, please install gsplit, comment out the split command, and then use the gsplit command.

1. under 'generateLHE' directory, execute `./convertLHE.sh CohJpsi` to convert the splitted STARlight output files to CMS LHE files. This script calls 'convert_SL2LHE.C' macro, one can directly run this macro by using 'root -l -b -q convert_SL2LHE.C', of course, a valid input file is needed. In this step, one can apply some additional filters in convert_SL2LHE.C, e.g., pair rapidity filter, kinematic filter of daughters etc. The kinematic filter of daughter particle is not recommended, because one needs to evaluate detector acceptance in Jpsi analysis. **NOTE: Please edit convert_SL2LHE.C since there might have some acceptance cuts**


# $J/\psi$ CMS Simulation

## 1. Set up CMS environment 
1. In lxplus work area (In general, the exactly same CMSSW package as that in official real data production should be used.) Do `cmsrel CMSSW_13_0_X`. Only needs to be done once
1. `cd CMSSW_13_0_X/src` and `cmsenv`
1. execute `scram b -j 8`

## 2. Run CMS Simulation
**NOTE: Please edit the python file under the Configureation directory since there might have some acceptance cuts and energy setting**
1. execute `./genPythonCfg_cmsDriver.sh` to generate the python configration files for GENSIM(mc_step1), DIGIRAW(mc_step2), and RECO(mc_step3). One can replace the lhe file right after '--filein' by a new generated LHE file.
1. move these three python files (step*.py) to 'submitJob' directory and 'cd submitJob'
1. add manually (open the resulting python file) in step 2 the following lines at the end of the file:
`
process.HcalTPGCoderULUT.FG_HF_thresholds = cms.vuint32(14, 19)
process.GlobalTag.toGet.extend([cms.PSet(record = cms.string('L1TUtmTriggerMenuRcd'), tag = cms.string('L1Menu_CollisionsHeavyIons2022_v1_1_0-d1_xml'))])
`
1. do some local test by executing `cmsRun step1_STARlight_LHE_GenSim_cfg.py`, `cmsRun step2_STARlight_Digi_cfg.py`, and `cmsRun step3_STARlight_Reco_cfg.py` for GENSIM, DIGIRAW, and RECO steps, respectively. 

## 3. Submit crab job
1. `crab submit crabConfig_STARlight_GenSim.py` and copy the directory after the "OutputDirectory" output message
1. `./getFileList.sh OutputDirectory lheFileList/CohJpsi_100k_GenSim.txt` OutputDirectory is the file directory output by GenSim step. the second command is the output file list text file. the file list is to be read by the next crab job as the input file list.
1. `crab submit crabConfig_STARlight_Digi.py`
1. `./getFileList.sh OutputDirectory lheFileList/CohJpsi_100k_Digi.txt`
1. `crab submit crabConfig_STARlight_Reco.py`
1. `./getFileList.sh OutputDirectory lheFileList/CohJpsi_100k_Reco.txt`