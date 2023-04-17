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
config.JobType.psetName = 'step2_STARlight_Digi_cfg.py'
config.JobType.numCores = 1
config.JobType.allowUndistributedCMSSW = True

config.section_('Data')
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
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
            "STARlight_CohPhi2KK_PbPb5TeV_Digi": { "Dataset": "/STARlight_CohPhi2KK_PbPb5TeV_GenFilter/phys_heavyions-STARlight_CohPhi2KK_PbPb5TeV_LHE_GenSim-f936895bd353c91c07137f485198850d/USER", "Memory": 2500, "RunTime": 1000 },
            }

## Submit job for different datasets 
for key, val in dataMap.items():
    config.General.requestName = key
    # config.JobType.maxMemoryMB = val["Memory"]
    # config.JobType.maxJobRuntimeMin = val["RunTime"]
    config.Data.inputDataset = val["Dataset"]
    config.Data.outputDatasetTag = config.General.requestName
    config.Data.outLFNDirBase = '/store/group/phys_heavyions/jiazhao/STARlight/DigiRaw/%s' %  config.General.requestName
    print("Submitting CRAB job for: "+val["Dataset"])
    submit(config)