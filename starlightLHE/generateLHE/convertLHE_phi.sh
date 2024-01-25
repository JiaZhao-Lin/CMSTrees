#!/bin/bash
#date

if [ $# != 1 ]; then
    echo "Please provide particle species name: \"LowMassGammaGamma\" OR \"CohPhi\" OR \"CohPhi_0n0n\" OR \"CohPhi_0nXn\" OR \"CohPhi_XnXn\" OR \"InCohPhi\"!!!"
    exit
fi

dir=$1

lheFileDir="lheFiles"

if [ ! -d $dir/$lheFileDir ]; then
    mkdir -p $dir/$lheFileDir
fi

if [ "`ls -A $dir/$lheFileDir`" != "" ]; then
    rm -rf $dir/$lheFileDir/*
fi

cmsEnergyDiv2=2680.0

for inputFile in `ls $dir/splitFiles/slight*`
do
    echo $inputFile

    baseFileName=`basename $inputFile`

    #outputFile=${baseFileName/.out/.lhe}
    if [[ $baseFileName =~ ".out" ]]; then
        outputFile=${baseFileName%.out}  # remove `.out`
    fi

    if [[ $baseFileName =~ ".tx" ]]; then
        outputFile=${baseFileName%.tx}  # remove `.tx`
    fi

    idx=${outputFile:$strLen-4}

    modIdx=$(echo $idx | sed 's/^0*//')  # remove leading zeroes
    #modIdx=$((10#$idx))  # remove leading zeroes

    modOutputFile=${outputFile/$idx/$modIdx}

    echo $outputFile
    #echo $modOutputFile
    #echo ""

    root -l -b << EOF
    .x convert_SL2LHE_phi.C+("$inputFile","$dir/$lheFileDir/$outputFile",$cmsEnergyDiv2,$cmsEnergyDiv2)
    .q
EOF

done