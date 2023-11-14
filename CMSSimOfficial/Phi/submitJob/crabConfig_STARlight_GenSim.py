from datetime import datetime
from CRABClient.UserUtilities import config

config = config()

## User Input ##############################################################################
pset_name       = 'step1_STARlight_LHE_GenSim_cfg.py'

request_name    = 'STARlight_CohPhiToKK_GenSim_132X'
channel         = 'CohPhiToKK'
request_name    += '_%s' % datetime.now().strftime('%y%m%d_%H%M%S')

# input_filelist	= './fileList/t0streamer_PhysicsHIForward0_374778.txt'
input_file      = 'starlight_slight_CohPhiToKK_Run3_el8_amd64_gcc11_CMSSW_13_2_6_patch2_tarball.tgz'
# input_dataset = ''

output_pd       = 'STARlight'
output_dir      = '/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/GenSim/%s' %  request_name

## General #################################################################################
config.section_('General')
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False
config.General.requestName = request_name

## JobType ##################################################################################
config.section_('JobType')
config.JobType.pluginName = 'PrivateMC'
config.JobType.inputFiles = [input_file]
config.JobType.psetName = pset_name
config.JobType.numCores = 1
# config.JobType.maxMemoryMB = 4000
# config.JobType.maxJobRuntimeMin = 1000
config.JobType.allowUndistributedCMSSW = True

## Data #####################################################################################
config.section_('Data')
# config.Data.inputDBS = 'global'
#* Using Dataset from DAS *******************************
# config.Data.inputDataset = input_dataset
# config.Data.splitting = 'FileBased'
# config.Data.unitsPerJob = 1
#* Using FileList ***************************************
# config.Data.userInputFiles = open(input_filelist).readlines() 
# config.Data.splitting = 'FileBased'
# config.Data.unitsPerJob = 1
# config.Data.publication = False

config.Data.splitting	= 'EventBased'
config.Data.unitsPerJob = 1000
NJOBS = 1000
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.publication = False
#********************************************************

config.Data.outputPrimaryDataset = output_pd
# config.Data.outputDatasetTag = config.General.requestName
config.Data.outputDatasetTag = channel
config.Data.outLFNDirBase = output_dir
config.Data.ignoreLocality = True

## Site #####################################################################################
config.section_('Site')
config.Site.whitelist = ['T2_CH_CERN']
config.Site.storageSite = 'T2_CH_CERN'

#############################################################################################
print('OutputDirectory: '+config.Data.outLFNDirBase)

