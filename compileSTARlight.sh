#!/bin/sh

if [ $# != 1 ]; then
    echo "- Please provide the STARlight version parameter: '313' OR '300' !"
    echo "- STARlight v3.13 (313) and v3.00 (300) are the two available versions in 'https://starlight.hepforge.org/downloads'."
    echo "- v3.13 was used for the official CMS STARlight MC production for Coherent/Incoherent Jpsi(Psi2S) analysis."
    exit
fi

if [ $1 != "313" -a $1 != "300" ]; then
    echo "- Please provide the STARlight version parameter: '313' OR '300' !"
    exit
fi

slVer=$1

wget 'https://starlight.hepforge.org/downloads?f=starlight_r'${slVer}'.tar'
mv 'downloads?f=starlight_r'${slVer}'.tar' starlight_r${slVer}.tar
mkdir -p starlightTrunk_v${slVer}/build
mv starlight_r${slVer}.tar starlightTrunk_v${slVer}/
cd starlightTrunk_v${slVer}
tar xvf starlight_r${slVer}.tar
rm -rf starlight_r${slVer}.tar
cd build
cmake ..
make