#!/bin/bash
shopt -s expand_aliases

export SCRAM_ARCH=el8_amd64_gcc11 # ensure you use the right arch
CMSSWV="CMSSW_13_2_5_patch1" 
/cvmfs/cms.cern.ch/common/scramv1 project CMSSW $CMSSWV # cmsrel $CMSSWV
cd $CMSSWV/src
eval `/cvmfs/cms.cern.ch/common/scram runtime -sh` # cmsenv

git cms-init

# For HLT analyzers
git cms-addpkg HLTrigger/Configuration
git cms-addpkg L1Trigger/L1TGlobal
mkdir -p L1Trigger/L1TGlobal/data/Luminosity/startup/ && cd L1Trigger/L1TGlobal/data/Luminosity/startup/

# For HLT analyzers
git cms-addpkg HLTrigger/Configuration
git cms-addpkg L1Trigger/L1TGlobal
mkdir -p L1Trigger/L1TGlobal/data/Luminosity/startup/ && cd L1Trigger/L1TGlobal/data/Luminosity/startup/

wget https://raw.githubusercontent.com/mitaylor/HIMenus/main/Menus/L1Menu_CollisionsHeavyIons2023_v1_0_0.xml
wget https://raw.githubusercontent.com/mitaylor/HIMenus/main/Menus/L1Menu_CollisionsHeavyIons2023_v1_1_0.xml
wget https://raw.githubusercontent.com/mitaylor/HIMenus/main/Menus/L1Menu_CollisionsHeavyIons2023_v1_1_1.xml
wget https://raw.githubusercontent.com/mitaylor/HIMenus/main/Menus/L1Menu_CollisionsHeavyIons2023_v1_1_2.xml
wget https://raw.githubusercontent.com/mitaylor/HIMenus/main/Menus/L1Menu_CollisionsHeavyIons2023_v1_1_3.xml

cd $CMSSW_BASE/src

git cms-addpkg HLTrigger/HLTanalyzers

git cms-merge-topic silviodonato:customizeHLTFor2023

# Build
scram b -j 8
cd HLTrigger/Configuration/test && mkdir workstation && cd workstation
