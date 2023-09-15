from datetime import datetime
from CRABClient.UserUtilities import config

config = config()

config.section_("General")
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False
request_name = "HLT_STARlight_CohJpsiToMuMu_132X"
request_name += "_%s" % datetime.now().strftime("%y%m%d_%H%M%S")
config.General.requestName = request_name

#############################################################################################
config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'test_pset.py'
config.JobType.numCores = 1
config.JobType.allowUndistributedCMSSW = True

#############################################################################################
config.section_('Data')
config.Data.inputDBS = 'phys03'
#* Using Dataset from DAS *******************************
# config.Data.inputDataset = '/CohJPsi_STARLIGHT_5p36TeV_2023Run3/phys_heavyions-CohJPsi_STARLIGHT_5p36TeV_2023Run3_RAW_20230812-e859c292782622da41d6dad2411c0a90/USER'
# config.Data.splitting = 'FileBased'
# config.Data.unitsPerJob = 1
#* Using FileList ***************************************
input_filelist = "fileList/Digi_132X.txt"
config.Data.userInputFiles = open(input_filelist).readlines() 
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False
#********************************************************

config.JobType.maxMemoryMB = 3000
# config.JobType.maxJobRuntimeMin = 1000
# config.Data.outputPrimaryDataset = "STARlight"
config.Data.outputDatasetTag = config.General.requestName
config.Data.outLFNDirBase = '/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/HLTTree/%s' %  config.General.requestName

config.section_('Site')
config.Data.ignoreLocality = True
config.Site.whitelist = ['T2_CH_CERN']
config.Site.storageSite = 'T2_CH_CERN'

#############################################################################################
print('OutputDirectory: '+config.Data.outLFNDirBase)

