# Note

## For Run 3
1. Change the "--era" to Run3_pp_on_PbPb
2. Change the "--conditions" to 130X_mcRun3_2023_realistic_HI_v8
3. Change the "--beamspot" to  Realistic2022PbPbCollision
4. Change the beam energy to 5.36 TeV (this might require to remake the LHE files you used as input and change the fragment file)


## Configurations
* Edit the name of the input hole file
* “fileNames = cms.untracked.vstring()”
* Use file: follow by the location
* Please change the Configuration python file when modifying collision energy and filter