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

muplusfilter = cms.EDFilter("MCSingleParticleFilter",
        Status = cms.untracked.vint32(1, 1),
        # MinPt = cms.untracked.vdouble(0.7, 0.7),
        # MinEta = cms.untracked.vdouble(1.28, -2.42),
        # MaxEta = cms.untracked.vdouble(2.42, -1.28),
        ParticleID = cms.untracked.vint32(-13, -13),
        )

muminusfilter = cms.EDFilter("MCSingleParticleFilter",
        Status = cms.untracked.vint32(1, 1),
        # MinPt = cms.untracked.vdouble(0.7, 0.7),
        # MinEta = cms.untracked.vdouble(1.28, -2.42),
        # MaxEta = cms.untracked.vdouble(2.42, -1.28),
        ParticleID = cms.untracked.vint32(13, 13),
        )

ProductionFilterSequence = cms.Sequence(generator*muplusfilter*muminusfilter)