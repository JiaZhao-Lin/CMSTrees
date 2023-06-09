from CRABAPI.RawCommand import crabCommand
from CRABClient.ClientExceptions import ClientException
from httplib import HTTPException

# We want to put all the CRAB project directories from the tasks we submit here into one common directory.
# That's why we need to set this parameter (here or above in the configuration file, it does not matter, we will not overwrite it).
from CRABClient.UserUtilities import config
config = config()

config.section_("General")
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'step1_STARlight_LHE_GenSim_cfg.py'
config.JobType.numCores = 1
config.JobType.allowUndistributedCMSSW = True

config.section_('Data')
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 2
config.Data.publication = True

config.section_('Site')
config.Data.ignoreLocality = True
config.Site.whitelist = ['T2_CH_CERN']
config.Site.storageSite = 'T2_CH_CERN'

def submit(config):
    try:
        crabCommand('submit', config = config, dryrun=False)
    except HTTPException as hte:
        print "Failed submitting task: %s" % (hte.headers)
    except ClientException as cle:
        print "Failed submitting task: %s" % (cle)

#############################################################################################
## From now on that's what users should modify: this is the a-la-CRAB2 configuration part. ##
#############################################################################################

dataMap = {
        "STARlight_CohPhi2KK_PbPb5TeV_LHE_GenSim": { "InputFiles": "lheFileList/CohPhi_200k_lhe.txt", "PrimaryDataset": "STARlight_CohPhi2KK_PbPb5TeV_GenFilter", "Memory": 2000, "RunTime": 1000 },
        }

## Submit job for different datasets 
for key, val in dataMap.items():
    config.General.requestName = key
#    config.JobType.maxMemoryMB = val["Memory"]
#    config.JobType.maxJobRuntimeMin = val["RunTime"]
#    config.Data.inputDataset = val["Dataset"]
    config.Data.userInputFiles = open(val["InputFiles"]).readlines() 
    config.Data.outputPrimaryDataset = val["PrimaryDataset"]
    config.Data.outputDatasetTag = config.General.requestName
#    config.Data.outLFNDirBase = '/store/user/shuaiy/RiceHIN/STARlight/%s' %  config.General.requestName
    config.Data.outLFNDirBase = '/store/group/phys_heavyions/jiazhao/STARlight/GenSim/%s' %  config.General.requestName
    print("Submitting CRAB job for: "+val["InputFiles"])
    submit(config)