#include "common/constants.h"

bool GoodNtrk(int Ntrk)
{
	// ! require 2 tracks
	return Ntrk == 2;
}

bool IsKaonPair(int pdgId1, int pdgId2)
{
	// ! require 2 Kaons
	return (abs(pdgId1) == PDG_KAON && abs(pdgId2) == PDG_KAON);
}

bool IsKaon(int pdgId)
{
	// ! require Kaon
	return abs(pdgId) == PDG_KAON;
}

bool IsPhi(int pdgId)
{
	// ! require phi
	return pdgId == PDG_PHI;
}

bool RecGenMatched(TVector3 Mom_rec, TVector3 Mom_gen, double BestDeltaR)
{
	// check if the reconstructed particle is matched to the generated one
	// DeltaPt/genPt < 0.15 ensures to keep 99.9842% events
	// DeltaR < 0.05 ensures to keep 99.9763% events
	// DeltaR < BestDeltaR ensures to keep only the best match
	double DeltaR	= Mom_gen.DeltaR(Mom_rec);
	double PtRes	= TMath::Abs((Mom_rec.Pt()-Mom_gen.Pt())/Mom_gen.Pt());

	return (PtRes < 0.15) && (DeltaR < 0.05) && (DeltaR < BestDeltaR);
}