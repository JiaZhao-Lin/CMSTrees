from datetime import datetime
from CRABClient.UserUtilities import config

config = config()

config.section_("General")
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False
request_name = "HIForward0_ppReco"
run_number = "374666"
request_name += "_Run" + run_number + "_%s" % datetime.now().strftime("%y%m%d_%H%M%S")
config.General.requestName = request_name

#############################################################################################
config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'reco_RAW2DIGI_L1Reco_RECO.py'
config.JobType.numCores = 4
config.JobType.allowUndistributedCMSSW = True

#############################################################################################
config.section_('Data')
config.Data.inputDBS = 'global'
#* Using Dataset from DAS *******************************
# config.Data.inputDataset = '/CohJPsi_STARLIGHT_5p36TeV_2023Run3/phys_heavyions-CohJPsi_STARLIGHT_5p36TeV_2023Run3_RAW_20230812-e859c292782622da41d6dad2411c0a90/USER'
# config.Data.splitting = 'FileBased'
# config.Data.unitsPerJob = 1
#* Using FileList ***************************************
input_filelist = "/afs/cern.ch/user/j/jiazhao/Trigger/CMSSW_13_2_5_patch1/src/HLTrigger/Configuration/test/workstation/fileList/HIForward0_374666.txt"
config.Data.userInputFiles = open(input_filelist).readlines() 
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
#********************************************************

config.JobType.maxMemoryMB = 5000
# config.JobType.maxJobRuntimeMin = 1000
# config.Data.outputPrimaryDataset = "STARlight"
# config.Data.outputDatasetTag = config.General.requestName
config.Data.outputDatasetTag = run_number
config.Data.outLFNDirBase = '/store/group/phys_heavyions/jiazhao/Data_Run3/Express/%s' %  config.General.requestName

config.section_('Site')
config.Data.ignoreLocality = True
config.Site.whitelist = ['T2_CH_CERN']
config.Site.storageSite = 'T2_CH_CERN'

#############################################################################################
print('OutputDirectory: '+config.Data.outLFNDirBase)

