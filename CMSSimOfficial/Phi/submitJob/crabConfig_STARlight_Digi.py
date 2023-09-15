from datetime import datetime
from CRABClient.UserUtilities import config

config = config()

config.section_("General")
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False
request_name = "STARlight_CohPhiToKK_Digi_132X"
request_name += "_%s" % datetime.now().strftime("%y%m%d_%H%M%S")
config.General.requestName = request_name

#############################################################################################
config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'step2_STARlight_Digi_cfg.py'
config.JobType.numCores = 1
config.JobType.allowUndistributedCMSSW = True

#############################################################################################
config.section_('Data')
config.Data.inputDBS = 'phys03'
#* Using Dataset from DAS *******************************
# config.Data.inputDataset = ''
# config.Data.splitting = 'FileBased'
# config.Data.unitsPerJob = 1
#* Using FileList ***************************************
input_filelist = "fileList/STARlight_CohPhiToKK_GenSim.txt"
config.Data.userInputFiles = open(input_filelist).readlines() 
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
#********************************************************

# config.JobType.maxMemoryMB = 2000
# config.JobType.maxJobRuntimeMin = 1000
# config.Data.outputPrimaryDataset = "STARlight"
config.Data.outputDatasetTag = config.General.requestName
config.Data.outLFNDirBase = '/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/Digi/%s' %  config.General.requestName

config.section_('Site')
config.Data.ignoreLocality = True
config.Site.whitelist = ['T2_CH_CERN']
config.Site.storageSite = 'T2_CH_CERN'

#############################################################################################
print('OutputDirectory: '+config.Data.outLFNDirBase)

