#!/bin/bash
#date

if [ $# != 2 ]; then
    echo "Please provide two paramters for this script !!!"
    echo "1st - Total line number (must be a multiple of 4) for each small STARlight output file. '100000' is recommended for UPC Jpsi analysis."
    echo "2nd - particle species name: \"LowMassGammaGamma\" OR \"CohJpsi\" OR \"CohJpsi_0n0n\" OR \"CohJpsi_0nXn\" OR \"CohJpsi_XnXn\" OR \"InCohJpsi\" OR \"CohPsi2S\" OR \"InCohPsi2S\"."
    exit
fi

num=`expr $1 % 4`
if [ ${num} != 0 ]; then
    echo "The 1st paramter should be a multiple of 4 !"
    exit
fi

if [ $2 != "LowMassGammaGamma" -a $2 != "CohJpsi" -a $2 != "CohJpsi_0n0n" -a $2 != "CohJpsi_0nXn" -a $2 != "CohJpsi_XnXn" -a $2 != "InCohJpsi" -a $2 != "CohPsi2S" -a $2 != "InCohPsi2S" ]; then
    echo "The second paramter should be: \"LowMassGammaGamma\" OR \"CohJpsi\" OR \"CohJpsi_0n0n\" OR \"CohJpsi_0nXn\" OR \"InCohJpsi\" OR \"CohPsi2S\" OR \"InCohPsi2S\" !"
    exit
fi

totalLines=$1
dir=$2
specName=$2

mkdir -p $dir/splitFiles

split -l ${totalLines} ${dir}/slight.${specName}.out  --numeric-suffixes=1  -a 4   ${dir}/splitFiles/slight_${specName}_  --additional-suffix=.out
#gsplit -l ${totalLines} ${dir}/slight.${specName}.out  --numeric-suffixes=1  -a 4   ${dir}/splitFiles/slight_${specName}_  --additional-suffix=.out
#gsplit -l 100000 slight.CohJpsi.out  --numeric-suffixes=1  -a 4   splitFiles/slight_CohJpsi_  --additional-suffix=.out
