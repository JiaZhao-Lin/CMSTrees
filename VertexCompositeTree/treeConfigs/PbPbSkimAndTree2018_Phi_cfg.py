import FWCore.ParameterSet.Config as cms
from Configuration.StandardSequences.Eras import eras
process = cms.Process('ANASKIM',eras.Run2_2018_pp_on_AA)

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')
process.load("CondCore.CondDB.CondDB_cfi")
process.load('Configuration.EventContent.EventContent_cff')


# Limit the output messages
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 200
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
# process.options.SkipEvent = cms.untracked.vstring('ProductNotFound')

# Define the input source
process.source = cms.Source("PoolSource",
#   fileNames = cms.untracked.vstring('/store/himc/HINPbPbAutumn18DR/DYJetsToLL_MLL-50_TuneCP5_HydjetDrumMB_5p02TeV-amcatnloFXFX-pythia8/AODSIM/mva98_103X_upgrade2018_realistic_HI_v11-v1/70003/FABA33C0-C8C5-9E4D-AEDB-48A33CC53415.root'),
   fileNames = cms.untracked.vstring('file:/eos/cms/store/group/phys_heavyions/jiazhao/STARlight/Reco/STARlight_CohPhi2KK_PbPb5TeV_Reco/STARlight_CohPhi2KK_PbPb5TeV_GenFilter/STARlight_CohPhi2KK_PbPb5TeV_Reco/230416_192957/0000step3_STARlight_Reco_highBetaStar_1.root'),
   inputCommands=cms.untracked.vstring('keep *', 'drop *_hiEvtPlane_*_*')
)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

#Setup FWK for multithreaded
#process.options.numberOfThreads=cms.untracked.uint32(8)
#process.options.numberOfStreams=cms.untracked.uint32(0)

# Set the global tag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')
process.GlobalTag.globaltag = cms.string('103X_upgrade2018_realistic_HI_v12')

# Add PbPb centrality
process.load("RecoHI.HiCentralityAlgos.CentralityBin_cfi")
process.centralityBin.Centrality = cms.InputTag("hiCentrality")
process.centralityBin.centralityVariable = cms.string("HFtowers")
process.GlobalTag.snapshotTime = cms.string("9999-12-31 23:59:59.000")
process.GlobalTag.toGet.extend([
    cms.PSet(record = cms.string("HeavyIonRcd"),
        tag = cms.string("CentralityTable_HFtowers200_HydjetDrum5F_v1032x02_mc"),
        connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS"),
        label = cms.untracked.string("HFtowers")
        ),
    ])
process.cent_seq = cms.Sequence(process.centralityBin)

# Add PbPb event plane
process.load("RecoHI.HiEvtPlaneAlgos.HiEvtPlane_cfi")
process.load("RecoHI.HiEvtPlaneAlgos.hiEvtPlaneFlat_cfi")
process.hiEvtPlane.trackTag = cms.InputTag("generalTracks")
process.hiEvtPlane.vertexTag = cms.InputTag("offlinePrimaryVerticesRecovery")
process.hiEvtPlane.loadDB = cms.bool(True)
process.hiEvtPlane.useNtrk = cms.untracked.bool(False)
process.hiEvtPlane.caloCentRef = cms.double(-1)
process.hiEvtPlane.caloCentRefWidth = cms.double(-1)
process.hiEvtPlaneFlat.caloCentRef = cms.double(-1)
process.hiEvtPlaneFlat.caloCentRefWidth = cms.double(-1)
process.hiEvtPlaneFlat.vertexTag = cms.InputTag("offlinePrimaryVerticesRecovery")
process.hiEvtPlaneFlat.useNtrk = cms.untracked.bool(False)
process.CondDB.connect = "sqlite_file:HeavyIonRPRcd_PbPb2018_offline.db"
process.PoolDBESSource = cms.ESSource("PoolDBESSource",
                                       process.CondDB,
                                       toGet = cms.VPSet(cms.PSet(record = cms.string('HeavyIonRPRcd'),
                                                                  tag = cms.string('HeavyIonRPRcd_PbPb2018_offline')
                                                                  )
                                                         )
                                      )
process.es_prefer_flatparms = cms.ESPrefer('PoolDBESSource','')
process.evtplane_seq = cms.Sequence(process.hiEvtPlane * process.hiEvtPlaneFlat)


# Phi candidate rereco
process.load("VertexCompositeAnalysis.VertexCompositeProducer.generalParticles_cff")
process.generalPhiCandidatesNew = process.generalParticles.clone(
    pdgId = cms.int32(333),
    mass = cms.double(1.019),
    charge = cms.int32(0),
    doSwap = cms.bool(False),
    width = cms.double(1.3),

    # daughter information
    daughterInfo = cms.VPSet([
        cms.PSet(pdgId = cms.int32(321), charge = cms.int32(-1)),
        cms.PSet(pdgId = cms.int32(321), charge = cms.int32(+1))
    ])
  )
process.generalPhiCandidatesNew.mva = cms.InputTag("generalTracks","MVAValues") ###cesar:to change iter6 tracking mva cut

# tree producer
from VertexCompositeAnalysis.VertexCompositeAnalyzer.particle_tree_cff import particleAna
process.generalanaNew = particleAna.clone(
  recoParticles = cms.InputTag("generalPhiCandidatesNew"),
#   triggerInfo = cms.untracked.VPSet([
#     cms.PSet(path = cms.string('HLT_HIMinimumBias_*')), # Minimum bias
#   ]),
#   selectEvents = cms.string("ntrkFilterRho_step"),
)

process.generalanaNewSeq = cms.Sequence(process.generalanaNew)
process.generalana_step = cms.EndPath( process.generalanaNewSeq )


# Add PbPb collision event selection
process.load('VertexCompositeAnalysis.VertexCompositeProducer.collisionEventSelection_cff')
process.load('VertexCompositeAnalysis.VertexCompositeProducer.clusterCompatibilityFilter_cfi')
process.load('VertexCompositeAnalysis.VertexCompositeProducer.hfCoincFilter_cff')
process.load("VertexCompositeAnalysis.VertexCompositeProducer.OfflinePrimaryVerticesRecovery_cfi")
process.colEvtSel = cms.Sequence(process.hfCoincFilter2Th4 * process.primaryVertexFilter * process.clusterCompatibilityFilter)

# Define the analysis steps
process.pcentandep_step = cms.Path(process.offlinePrimaryVerticesRecovery * process.cent_seq * process.evtplane_seq)
# process.pcentandep_step = cms.Path(process.offlinePrimaryVerticesRecovery )
process.phirereco_step = cms.Path(process.offlinePrimaryVerticesRecovery * process.generalPhiCandidatesNew)

# Define the output
process.TFileService = cms.Service("TFileService",
                                       fileName = cms.string('phiana_tree_PbPb2018.root')
                                  )

# process.output_HM = cms.OutputModule("PoolOutputModule",
#     fileName = cms.untracked.string('PbPb2018_SKIM_AOD.root'),
#     # SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('eventFilter_HM_step', 'ntrkFilterRho_step')),
#     dataset = cms.untracked.PSet(
#       dataTier = cms.untracked.string('AOD')
#     )
# )
# process.output_HM.outputCommands = cms.untracked.vstring('drop *',
#       'keep *_*_*_PhiPbPb2018SKIM',
#       'keep *_offlinePrimaryVerticesRecovery_*_*',
#       'keep *_hiEvtPlane_*_*',
#       'keep *_centralityBin_*_*',
#       'keep *_hiCentrality_*_PhiPbPb2018SKIM',
# )
# process.output_HM_step = cms.EndPath(process.output_HM)

# Define the process schedule
process.schedule = cms.Schedule(
    # process.eventFilter_HM_step,
    process.pcentandep_step,
    # process.ntrkFilterRho_step,
    process.phirereco_step,
    # process.output_HM_step,
    process.generalana_step
)
# # Define the process schedule
# process.schedule = cms.Schedule(
#     process.pcentandep_step,
#     process.phirereco_step,
#     process.generalana_step
# )

# Add the event selection filters
# process.Flag_colEvtSel = cms.Path(process.offlinePrimaryVerticesRecovery * process.colEvtSel)
# process.Flag_hfCoincFilter2Th4 = cms.Path(process.offlinePrimaryVerticesRecovery * process.hfCoincFilter2Th4)
# process.Flag_primaryVertexFilter = cms.Path(process.offlinePrimaryVerticesRecovery * process.primaryVertexFilter)
# process.Flag_clusterCompatibilityFilter = cms.Path(process.offlinePrimaryVerticesRecovery * process.clusterCompatibilityFilter)
# eventFilterPaths = [ process.Flag_colEvtSel , process.Flag_hfCoincFilter2Th4 , process.Flag_primaryVertexFilter , process.Flag_clusterCompatibilityFilter ]
# for P in eventFilterPaths:
#     process.schedule.insert(0, P)

# Add recovery for offline primary vertex
from HLTrigger.Configuration.CustomConfigs import MassReplaceInputTag
process = MassReplaceInputTag(process,"offlinePrimaryVertices","offlinePrimaryVerticesRecovery")
process.offlinePrimaryVerticesRecovery.oldVertexLabel = "offlinePrimaryVertices"