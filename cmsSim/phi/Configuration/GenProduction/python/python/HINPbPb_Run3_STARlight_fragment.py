import FWCore.ParameterSet.Config as cms

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

ProductionFilterSequence = cms.Sequence(generator)