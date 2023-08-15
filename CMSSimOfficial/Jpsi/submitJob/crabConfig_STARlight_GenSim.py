from datetime import datetime
from CRABClient.UserUtilities import config

config = config()

config.section_("General")
config.General.workArea = 'crab_projects'
config.General.transferOutputs = True
config.General.transferLogs = False
request_name = "STARlight_CohJpsiToMuMu_GenSim"
request_name += "_%s" % datetime.now().strftime("%y%m%d_%H%M%S")
config.General.requestName = request_name

#############################################################################################
config.section_('JobType')
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'step1_STARlight_LHE_GenSim_cfg.py'
config.JobType.numCores = 1
config.JobType.allowUndistributedCMSSW = True

#############################################################################################
config.section_('Data')
config.JobType.inputFiles = ['starlight_slight_CohJpsiToMuMu_Run3_el8_amd64_gcc11_CMSSW_13_0_12_tarball.tgz']
config.Data.splitting	= 'EventBased'
config.Data.unitsPerJob = 1000
NJOBS = 800
config.Data.totalUnits = config.Data.unitsPerJob * NJOBS
config.Data.publication = False
# config.JobType.maxMemoryMB = 2000
# config.JobType.maxJobRuntimeMin = 1000
config.Data.outputPrimaryDataset = "STARlight_CohJpsiToMuMu_PbPb5p36TeV_2023Run3"
config.Data.outputDatasetTag = config.General.requestName
config.Data.outLFNDirBase = '/store/group/phys_heavyions/jiazhao/STARlight/2023Run3/GenSim/%s' %  config.General.requestName

config.section_('Site')
config.Data.ignoreLocality = True
config.Site.whitelist = ['T2_CH_CERN']
config.Site.storageSite = 'T2_CH_CERN'

#############################################################################################
for job_name in config.JobType.inputFiles:
	print("Submitting CRAB job for: "+job_name)
print('OutputDirectory: '+config.Data.outLFNDirBase)

