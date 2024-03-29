import FWCore.ParameterSet.Config as cms

externalLHEProducer = cms.EDProducer("ExternalLHEProducer",
    args = cms.vstring('../starlight_slight_CohPhiToKK_Run3_el8_amd64_gcc11_CMSSW_13_2_10_tarball.tgz'),
    nEvents = cms.untracked.uint32(1),
    numberOfParameters = cms.uint32(1),
    outputFile = cms.string('cmsgrid_final.lhe'),
    scriptName = cms.FileInPath('GeneratorInterface/LHEInterface/data/run_generic_tarball_cvmfs.sh')
)


generator = cms.EDFilter("Pythia8HadronizerFilter",
                         maxEventsToPrint = cms.untracked.int32(1),
                         pythiaPylistVerbosity = cms.untracked.int32(1),
                         filterEfficiency = cms.untracked.double(1.0),
                         pythiaHepMCVerbosity = cms.untracked.bool(False),
                         comEnergy = cms.double(5360.),
                         PythiaParameters = cms.PSet(
                                parameterSets = cms.vstring('skip_hadronization'),
                                skip_hadronization = cms.vstring('ProcessLevel:all = off',
                                        'Check:event = off')
                        )
)


# diKafilter = cms.EDFilter("PythiaFilter",
#     MaxAbsRapidity = cms.untracked.double(3.0),
#     MinAbsRapidity = cms.untracked.double(1.3),
#     ParticleID = cms.untracked.int32(333)
# )

# ProductionFilterSequence = cms.Sequence(generator+diKafilter)
ProductionFilterSequence = cms.Sequence(generator)