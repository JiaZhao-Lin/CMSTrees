from datetime import datetime
from CRABClient.UserUtilities import config

config = config()

## User Input ##############################################################################
pset_name 		= 'reco_RAW2DIGI_L1Reco_RECO.py'

request_name 	= 'AOD_t0streamer_PhysicsHIForward0_ppReco'
# request_name = 'AOD_HIRun2023A_HIForward0_ppReco'
run_number 		= '374778'
request_name 	+= '_Run' + run_number + '_%s' % datetime.now().strftime('%y%m%d_%H%M%S')

input_filelist 	= './fileList/t0streamer_PhysicsHIForward0_374778.txt'
# input_dataset	= ''

output_pd		= 'PhysicsHIForward0'
output_dir 		= '/store/group/phys_heavyions/jiazhao/Data_Run3/Express/%s' %  request_name

## General #################################################################################
config.section_('General')
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False
config.General.requestName = request_name

## JobType ##################################################################################
config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = pset_name
config.JobType.numCores = 4
config.JobType.maxMemoryMB = 8000
# config.JobType.maxJobRuntimeMin = 1000
config.JobType.allowUndistributedCMSSW = True

## Data #####################################################################################
config.section_('Data')
config.Data.inputDBS = 'global'
#* Using Dataset from DAS *******************************
# config.Data.inputDataset = input_dataset
# config.Data.splitting = 'FileBased'
# config.Data.unitsPerJob = 1
#* Using FileList ***************************************
config.Data.userInputFiles = open(input_filelist).readlines() 
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
#********************************************************

config.Data.outputPrimaryDataset = output_pd
# config.Data.outputDatasetTag = config.General.requestName
config.Data.outputDatasetTag = run_number
config.Data.outLFNDirBase = output_dir
config.Data.ignoreLocality = True

## Site #####################################################################################
config.section_('Site')
config.Site.whitelist = ['T2_CH_CERN']
config.Site.storageSite = 'T2_CH_CERN'

#############################################################################################
print('OutputDirectory: '+config.Data.outLFNDirBase)

