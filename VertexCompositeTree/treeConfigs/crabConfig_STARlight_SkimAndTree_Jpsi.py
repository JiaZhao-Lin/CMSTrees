from datetime import datetime
from CRABClient.UserUtilities import config

config = config()

config.section_("General")
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False

config.section_('JobType')
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'PbPbSkimAndTree2023_DiMuContBoth_mc_cfg.py'
# config.JobType.inputFiles = ['HeavyIonRPRcd_PbPb2018_offline.db']
#config.JobType.numCores = 1
config.JobType.allowUndistributedCMSSW = True

config.section_('Data')
config.Data.inputDBS = 'phys03'
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = False

config.section_('Site')
config.Data.ignoreLocality = True
config.Site.whitelist = ['T2_CH_CERN']
config.Site.storageSite = 'T2_CH_CERN'


#############################################################################################
#############################################################################################

request_name = "STARlight_CohJpsi_SkimAndTree"
request_name += "_%s" % datetime.now().strftime("%y%m%d_%H%M%S")

# dataset = "/STARlight_CohJpsi2MuMu_PbPb5TeV_GenFilter/phys_heavyions-STARlight_CohJpsi_Digi-e40e02507aab18498ef2ae53eec7f44b/USER"
input_filelist = "/afs/cern.ch/user/j/jiazhao/CMSSW_13_0_5_patch2/src/submitJob/lheFileList/test_Reco.txt"

config.General.requestName = request_name
# config.JobType.maxMemoryMB = 2000
# config.JobType.maxJobRuntimeMin = 1000
# config.Data.inputDataset = dataset
config.Data.userInputFiles = open(input_filelist).readlines() 
config.Data.outputDatasetTag = config.General.requestName
config.Data.outLFNDirBase = '/store/group/phys_heavyions/jiazhao/STARlight/SkimAndTree/%s' %  config.General.requestName

# print("Submitting CRAB job for: "+dataset)
print("Submitting CRAB job for: "+input_filelist)
print('OutputDirectory: '+config.Data.outLFNDirBase)
