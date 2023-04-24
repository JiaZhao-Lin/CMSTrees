#include "../common/constants.h"

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