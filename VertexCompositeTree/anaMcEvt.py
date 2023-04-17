# %%
# # Import modules
import ROOT
import numpy as np
import matplotlib.pyplot as plt
import json
from dataclasses import dataclass, field
from deco_clock import clock
ROOT.gInterpreter.ProcessLine('#include "../Ultra-Peripheral Collisions/UPC Code/upcJpsiDimu/common/funUtil.h"')
ROOT.gInterpreter.ProcessLine('#include "../Ultra-Peripheral Collisions/UPC Code/upcJpsiDimu/common/VertexCompositeTree.h"')
import numba as nb



# %%
# # Define classes
@dataclass
class EfficiencyData:
	"""
	Container for the histograms
	"""
	data: dict = field(default_factory=dict)

	def __post_init__(self):
		"""
		Initialize the histograms
		"""
		mHistRapBins     = 60;	mHistRapLow      = -3;	mHistRapHi       = 3;
		mHistPtBins      = 800;	mHistPtLow       = 0;	mHistPtHi        = 4;
		mHistMassBins    = 300;	mHistMassLow     = 2;	mHistMassHi      = 5;
		mHistAsyPhiBins  = 300;	mHistAsyPhiLow   = 0;	mHistAsyPhiHi    = 0.3;
		mHistAsyPtBins   = 500;	mHistAsyPtLow    = 0;	mHistAsyPtHi     = 0.5;
		Pt = np.array([0, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 3.0, 3.2, 3.6, 4.0], dtype="d")
		Eta = np.arange(-2.5, 2.5, 0.05, dtype = "d")
		Phi = np.arange(0, np.pi, 0.1, dtype = "d")
		# print(f"Pt = {Pt}")
		# print(f"Eta = {Eta}")
		# print(f"Phi = {Phi}")

		self.data["hnEvts"] = ROOT.TH1D("hnEvts", "hnEvts;", 5, 0, 5)
		self.data["hnEvts"].GetXaxis().SetBinLabel(1, "trigEvt")
		self.data["hnEvts"].GetXaxis().SetBinLabel(2, "validVtx & !Beam-halo")
		self.data["hnEvts"].GetXaxis().SetBinLabel(3, "HFMaxE <= 7.6(7.3) GeV")
		self.data["hnEvts"].GetXaxis().SetBinLabel(4, "N_{trk}^{HP} == 2")
		self.data["hnEvts"].GetXaxis().LabelsOption("d")
		self.data["hnEvts"].GetXaxis().SetLabelSize(0.06)

		self.data["hVzvsVyvsVx"]				= ROOT.TH3D("hVzvsVyvsVx",          "hVzvsVyvsVx; V_{x} (cm); V_{y} (cm); V_{z} (cm)", 100, -1, 1, 100, -1, 1, 300, -30, 30);
		self.data["hHFMinusvsHFPlus"]			= ROOT.TH2D("hHFMinusvsHFPlus",     "hHFMinusvsHFPlus; HFsumETPlus; HFsumETMinus",      200, 0, 20, 200, 0, 20);
		self.data["hNCanvsNtrkvsCen"]			= ROOT.TH3D("hNCanvsNtrkvsCen",     "hNCanvsNtrkvsCen; Centrality; N_{trk}^{offline}; # CS pair", 50, 150, 200, 10, 0, 10, 5, 0, 5);
		self.data["hNtrkofflinevsNtrkHP"]		= ROOT.TH2D("hNtrkofflinevsNtrkHP", "hNtrkofflinevsNtrkHP; N_{trk}^{HP}; N_{trk}^{offline}", 10, 0, 10, 10, 0, 10);

		self.data["hPhivsEtavsPt_Gen"]			= ROOT.TH3D("hPhivsEtavsPt_Gen", "hPhivsEtavsPt_Gen; p_{T} (GeV/c); #eta; #phi", 500, 0, 5, 300, -3, 3, 180, -np.pi, np.pi);
		self.data["hForward_PhivsEtavsPt_Gen"]	= ROOT.TH3D("hForward_PhivsEtavsPt_Gen", "hForward_PhivsEtavsPt_Gen; p_{T} (GeV/c); #eta; #phi", 500, 0, 5, 300, -3, 3, 180, -np.pi, np.pi);
		self.data["hNegPtvsPosPt_Gen"]			= ROOT.TH2D("hNegPtvsPosPt_Gen", "hNegPtvsPosPt_Gen; #mu^{+} p_{T} (GeV/c); #mu^{-} p_{T} (GeV/c);", 500, 0, 5, 500, 0, 5);
		self.data["hNegEtavsPosEta_Gen"]		= ROOT.TH2D("hNegEtavsPosEta_Gen", "hNegEtavsPosEta_Gen; #mu^{+} #eta; #mu^{-} #eta;", 300, -3, 3, 300, -3, 3);
		self.data["hNegPhivsPosPhi_Gen"]		= ROOT.TH2D("hNegPhivsPosPhi_Gen", "hNegPhivsPosPhi_Gen; #mu^{+} #phi; #mu^{-} #phi;", 180, -np.pi, np.pi, 180, -np.pi, np.pi);
		self.data["hDeltaR"]					= ROOT.TH1D("hDeltaR", "hDeltaR; #DeltaR", 2000, 0, 0.2);
		self.data["hDeltaPt"]					= ROOT.TH1D("hDeltaPt", "hDeltaPt; (p_{T}^{Rc}-p_{T}^{Gen})/p_{T}^{Gen}", 5000, -0.5, 0.5);
		self.data["hPtResvsGenPt"]				= ROOT.TH2D("hPtResvsGenPt", "hPtResvsGenPt; p_{T}^{Gen} (GeV/c); (p_{T}^{Rc}-p_{T}^{Gen})/p_{T}^{Gen};", 500, 0, 5, 300, -0.15, 0.15);
		self.data["hEtaResvsGenEta"]			= ROOT.TH2D("hEtaResvsGenEta", "hEtaResvsGenEta; #eta^{Gen}; #eta^{Rc}-#eta^{Gen};", 300, -3, 3, 200, -0.01, 0.01);
		self.data["hPhiResvsGenPhi"]			= ROOT.TH2D("hPhiResvsGenPhi", "hPhiResvsGenPhi; #phi^{Gen}; #phi^{Rc}-#phi^{Gen};", 180, -np.pi, np.pi, 200, -0.01, 0.01);

		self.data["hNRcMuvsNGenMu"]			= ROOT.TH2D("hNRcMuvsNGenMu", "hNRcMuvsNGenMu; # of muon (GEN); # of muon (Soft)", 10, 0, 10, 10, 0, 10);
		self.data["hPhivsEtavsPt"]			= ROOT.TH3D("hPhivsEtavsPt", "hPhivsEtavsPt; p_{T} (GeV/c); #eta; #phi", 500, 0, 5, 300, -3, 3, 180, -np.pi, np.pi);

		self.data["hRcPairPtvsGenPairPt"]	= ROOT.TH2D("hRcPairPtvsGenPairPt", "hRcPairPtvsGenPairPt; pair p_{T}^{Gen} (GeV/c); pair p_{T}^{Rc} (GeV/c);", mHistPtBins, mHistPtLow, mHistPtHi, mHistPtBins, mHistPtLow, mHistPtHi);
		self.data["hRcPairEtavsGenPairEta"]	= ROOT.TH2D("hRcPairEtavsGenPairEta", "hRcPairEtavsGenPairEta; pair #eta^{Gen}; pair #eta^{Rc};", 160, -4, 4, 160, -4, 4);
		self.data["hRcPairPhivsGenPairPhi"]	= ROOT.TH2D("hRcPairPhivsGenPairPhi", "hRcPairPhivsGenPairPhi; pair #phi^{Gen}; pair #phi^{Rc};", 120, -np.pi, np.pi, 120, -np.pi, np.pi);
		self.data["hMassResvsGenMass"]		= ROOT.TH2D("hMassResvsGenMass", "hMassResvsGenMass; M_{#mu#mu}^{Gen} (GeV/c^{2}); (M_{#mu#mu}^{Rc} - M_{#mu#mu}^{Gen})/M_{#mu#mu}^{Gen};", mHistMassBins, mHistMassLow, mHistMassHi, 300, -0.15, 0.15);
		self.data["hPairPtResvsGenPairPt"]	= ROOT.TH2D("hPairPtResvsGenPairPt", "hPairPtResvsGenPairPt; J/#psi p_{T}^{Gen} (GeV/c); J/#psi p_{T}^{Rc} - p_{T}^{Gen};", 300, 0, 0.3, 300, -0.3, 0.3);
		self.data["hRapResvsGenRap"]		= ROOT.TH2D("hRapResvsGenRap", "hRapResvsGenRap; pair y^{Gen}; y^{Rc} - y^{Gen};", 600, -3, 3, 1000, -0.1, 0.1);

		self.data["hMvsPtvsRap_Gen"]		= ROOT.TH3D("hMvsPtvsRap_Gen", "hMvsPtvsRap_Gen; Rapidity; p_{T} (GeV/c); M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistPtBins, mHistPtLow, mHistPtHi, mHistMassBins, mHistMassLow, mHistMassHi);
		self.data["hMvsAsyPhivsRap_Gen"]	= ROOT.TH3D("hMvsAsyPhivsRap_Gen", "hMvsAsyPhivsRap_Gen; Rapidity; #alpha; M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistAsyPhiBins, mHistAsyPhiLow, mHistAsyPhiHi, mHistMassBins, mHistMassLow, mHistMassHi);
		self.data["hDeltaPhivsM_Gen"]		= ROOT.TH2D("hDeltaPhivsM_Gen", "hDeltaPhivsM_Gen; M_{#mu#mu} (GeV/c^{2}); #phi_{#mu^{+}+#mu^{-}} - #phi_{#mu^{+}-#mu^{-}}", mHistMassBins, mHistMassLow, mHistMassHi, 120, -np.pi, np.pi);

		self.data["hMvsPtvsRap_woEvtSel_woSmear"]	= ROOT.TH3D("hMvsPtvsRap_woEvtSel_woSmear", "hMvsPtvsRap_woEvtSel_woSmear; Rapidity; p_{T} (GeV/c); M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistPtBins, mHistPtLow, mHistPtHi, mHistMassBins, mHistMassLow, mHistMassHi);
		self.data["hMvsPtvsRap_woSmear"]			= ROOT.TH3D("hMvsPtvsRap_woSmear", "hMvsPtvsRap_woSmear; Rapidity; p_{T} (GeV/c); M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistPtBins, mHistPtLow, mHistPtHi, mHistMassBins, mHistMassLow, mHistMassHi);

		self.data["hMvsPtvsRap_woEvtSel"]			= ROOT.TH3D("hMvsPtvsRap_woEvtSel", "hMvsPtvsRap_woEvtSel; Rapidity; p_{T} (GeV/c); M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistPtBins, mHistPtLow, mHistPtHi, mHistMassBins, mHistMassLow, mHistMassHi);
		self.data["hMvsPtvsRap"]					= ROOT.TH3D("hMvsPtvsRap", "hMvsPtvsRap; Rapidity; p_{T} (GeV/c); M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistPtBins, mHistPtLow, mHistPtHi, mHistMassBins, mHistMassLow, mHistMassHi);
		self.data["hMvsAsyPhivsRap"]				= ROOT.TH3D("hMvsAsyPhivsRap", "hMvsAsyPhivsRap; Rapidity; #alpha; M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistAsyPhiBins, mHistAsyPhiLow, mHistAsyPhiHi, mHistMassBins, mHistMassLow, mHistMassHi);
		self.data["hDeltaPhivsM"]					= ROOT.TH2D("hDeltaPhivsM", "hDeltaPhivsM; M_{#mu#mu} (GeV/c^{2}); #phi_{#mu^{+}+#mu^{-}} - #phi_{#mu^{+}-#mu^{-}}", mHistMassBins, mHistMassLow, mHistMassHi, 120, -np.pi, np.pi);

		self.data["hPosMuPhivsEtavsPt_Gen"]			= ROOT.TH3D("hPosMuPhivsEtavsPt_Gen",		"hPosMuPhivsEtavsPt_Gen;	p_{T} (GeV/c); #eta; #phi", len(Pt)-1, Pt, len(Eta)-1, Eta, len(Phi)-1, Phi)
		self.data["hMthPosMuPhivsEtavsPt_Gen"]		= ROOT.TH3D("hMthPosMuPhivsEtavsPt_Gen",	"hMthPosMuPhivsEtavsPt_Gen;	p_{T} (GeV/c); #eta; #phi", len(Pt)-1, Pt, len(Eta)-1, Eta, len(Phi)-1, Phi)
		self.data["hMthPosMuPhivsEtavsPt"]			= ROOT.TH3D("hMthPosMuPhivsEtavsPt",		"hMthPosMuPhivsEtavsPt;		p_{T} (GeV/c); #eta; #phi", len(Pt)-1, Pt, len(Eta)-1, Eta, len(Phi)-1, Phi)
		self.data["hTrigPosMuPhivsEtavsPt"]			= ROOT.TH3D("hTrigPosMuPhivsEtavsPt",		"hTrigPosMuPhivsEtavsPt;	p_{T} (GeV/c); #eta; #phi", len(Pt)-1, Pt, len(Eta)-1, Eta, len(Phi)-1, Phi)
		self.data["hNegMuPhivsEtavsPt_Gen"]			= ROOT.TH3D("hNegMuPhivsEtavsPt_Gen",		"hNegMuPhivsEtavsPt_Gen;	p_{T} (GeV/c); #eta; #phi", len(Pt)-1, Pt, len(Eta)-1, Eta, len(Phi)-1, Phi)
		self.data["hMthNegMuPhivsEtavsPt_Gen"]		= ROOT.TH3D("hMthNegMuPhivsEtavsPt_Gen",	"hMthNegMuPhivsEtavsPt_Gen;	p_{T} (GeV/c); #eta; #phi", len(Pt)-1, Pt, len(Eta)-1, Eta, len(Phi)-1, Phi)
		self.data["hMthNegMuPhivsEtavsPt"]			= ROOT.TH3D("hMthNegMuPhivsEtavsPt",		"hMthNegMuPhivsEtavsPt;		p_{T} (GeV/c); #eta; #phi", len(Pt)-1, Pt, len(Eta)-1, Eta, len(Phi)-1, Phi)
		self.data["hTrigNegMuPhivsEtavsPt"]			= ROOT.TH3D("hTrigNegMuPhivsEtavsPt",		"hTrigNegMuPhivsEtavsPt;	p_{T} (GeV/c); #eta; #phi", len(Pt)-1, Pt, len(Eta)-1, Eta, len(Phi)-1, Phi)

		self.data["hMthPosMuPhivsEtavsPtInpair"]	= ROOT.TH3D("hMthPosMuPhivsEtavsPtInpair",	"hMthPosMuPhivsEtavsPtInpair;	p_{T} (GeV/c); #eta; #phi", len(Pt)-1, Pt, len(Eta)-1, Eta, len(Phi)-1, Phi)
		self.data["hTrigPosMuPhivsEtavsPtInpair"]	= ROOT.TH3D("hTrigPosMuPhivsEtavsPtInpair",	"hTrigPosMuPhivsEtavsPtInpair;	p_{T} (GeV/c); #eta; #phi", len(Pt)-1, Pt, len(Eta)-1, Eta, len(Phi)-1, Phi)
		self.data["hMthNegMuPhivsEtavsPtInpair"]	= ROOT.TH3D("hMthNegMuPhivsEtavsPtInpair",	"hMthNegMuPhivsEtavsPtInpair;	p_{T} (GeV/c); #eta; #phi", len(Pt)-1, Pt, len(Eta)-1, Eta, len(Phi)-1, Phi)
		self.data["hTrigNegMuPhivsEtavsPtInpair"]	= ROOT.TH3D("hTrigNegMuPhivsEtavsPtInpair",	"hTrigNegMuPhivsEtavsPtInpair;	p_{T} (GeV/c); #eta; #phi", len(Pt)-1, Pt, len(Eta)-1, Eta, len(Phi)-1, Phi)

	def __getitem__(self, key):
		if key not in self.data:
			raise KeyError(f"Key {key} not found in EfficiencyData")
		return self.data[key]
	def __setitem__(self, key, value):
		self.data[key] = value


class EfficiencyAnalyzer:
	def __init__(self, file_name = "inFiles/dimuana_mc_CohJpsi_sample_1000.root"):
		self.file_name 		= file_name
		self.file 			= ROOT.TFile(file_name)
		self.csTreeDir 		= "dimucontana_mc"
		self.csTree 		= ROOT.VertexCompositeTree()
		self.csTree.GetTree(self.file_name, self.csTreeDir)
		self.eff_data		= EfficiencyData()
		self.Mmuon = 0.1056583745
		self.trigIdx = 7

		self.init_pt_smear_func()


	def good_Vtx(self):
		# Select Event - require this event has a valid vertex and is not beam-halo event
		goodVtx = (self.csTree.evtSel()[2]) and (self.csTree.evtSel()[3])

		return goodVtx

	def good_HFVeto(self):
		"""
		evtSel[4-15]
		[4]=0: HFPlusMaxTower < 3 GeV;  [4]=1: HFPlusMaxTower > 3 GeV
		[5]=0: HFMinusMaxTower < 3 GeV;  [5]=1: HFMinusMaxTower > 3 GeV
		[6] is for Plus & [7] is for Minus; Threshold = 4 GeV
		[8] is for Plus & [9] is for Minus; Threshold = 5 GeV
		[10] is for Plus & [11] is for Minus; Threshold = 6 GeV
		[12] is for Plus & [13] is for Minus; Threshold = 7 GeV
		[14] is for Plus & [15] is for Minus; Threshold = 8 GeV
		[16] is for Plus (Th = 7.6 GeV) & [17] is for Minus (Th = 7.3 GeV);
		"""
		goodHFVeto = (not self.csTree.evtSel()[16]) and (not self.csTree.evtSel()[17])

		return goodHFVeto

	def good_Ntrk(self):
		return self.csTree.NtrkHP() == 2

	def pass_EvtSel(self):
		return self.good_Vtx() and self.good_HFVeto() and self.good_Ntrk()

	def init_pt_smear_func(self):
		mPar0       = 0.0145
		fRawPtRes = ROOT.TFile.Open("inFiles/rawPtRes.root")
		self.funPtMeanShift   = fRawPtRes.Get("funPtMeanShift")
		self.funRawPtRes      = fRawPtRes.Get("funRawPtRes")

		self.funTunedPtRes    = ROOT.TF1("funTunedPtRes", "sqrt([0]*[0]/x/x+[1]*[1])", 0, 5)
		self.funTunedPtRes.SetParameters(mPar0, self.funRawPtRes.GetParameter(1))

	def shiftDeltaPhi(self, dPhi: float)	-> float:
		if (np.isnan(dPhi)):
			return -999;

		while (dPhi < -np.pi):
			dPhi += 2 * np.pi
		while (dPhi >= np.pi):
			dPhi -= 2 * np.pi

		return dPhi

	def good_recoMuon(self, recoIdx):

		if( recoIdx < 0 or self.csTree.candSize() <= 0): 
			return False
		if not self.csTree.softCand(recoIdx):
			return False

		return True

	def goodMuPairIdx(self, icand_rec):
		mTrkPtTh1  = ROOT.fTrkAcc.Eval( self.csTree.EtaD1()[icand_rec] )
		mTrkPtTh2  = ROOT.fTrkAcc.Eval( self.csTree.EtaD2()[icand_rec] )
		mTrigPtTh1 = ROOT.fTrigAcc.Eval( self.csTree.EtaD1()[icand_rec] )
		mTrigPtTh2 = ROOT.fTrigAcc.Eval( self.csTree.EtaD2()[icand_rec] )

		if(self.csTree.pTD1()[icand_rec] < mTrkPtTh1 or self.csTree.pTD2()[icand_rec] < mTrkPtTh2): return False

		isTrigAcc1 = False
		isTrigAcc2 = False

		print(bool(self.csTree.trigMuon1()[self.trigIdx][icand_rec]))

		if(self.csTree.trigMuon1()[self.trigIdx][icand_rec] and self.csTree.pTD1()[icand_rec] >= mTrigPtTh1):
			isTrigAcc1 = True

		if(self.csTree.trigMuon2()[self.trigIdx][icand_rec] and self.csTree.pTD2()[icand_rec] >= mTrigPtTh2): 
			isTrigAcc2 = True

		if( (not isTrigAcc1) and (not isTrigAcc2) ): return False

		return True

	def goodMuPair(self, posFourMom, isPosTrig,	negFourMom, isNegTrig):
		mPosTrkPtTh  = ROOT.fTrkAcc .Eval( posFourMom.Eta() )
		mNegTrkPtTh  = ROOT.fTrkAcc .Eval( negFourMom.Eta() )
		mPosTrigPtTh = ROOT.fTrigAcc.Eval( posFourMom.Eta() )
		mNegTrigPtTh = ROOT.fTrigAcc.Eval( negFourMom.Eta() )

		if(posFourMom.Pt() < mPosTrkPtTh or negFourMom.Pt() < mNegTrkPtTh): return False

		isPosTrigAcc = False
		isNegTrigAcc = False
		if(isPosTrig and posFourMom.Pt() >= mPosTrigPtTh): isPosTrigAcc = True
		if(isNegTrig and negFourMom.Pt() >= mNegTrigPtTh): isNegTrigAcc = True

		if( not isPosTrigAcc and not isNegTrigAcc ): return False

		return True

	def fill_mu(self, RecoIdx, Pt_gen, Eta_gen, Phi_gen, MthDeltaR):
		muPt     = self.csTree.pT_mu()[RecoIdx]
		muEta    = self.csTree.eta_mu()[RecoIdx]
		muPhi    = self.csTree.phi_mu()[RecoIdx]
		isTrigMu = self.csTree.trigMuon_mu()[self.trigIdx][RecoIdx]

		self.eff_data["hDeltaR"]  .Fill( MthDeltaR )
		self.eff_data["hDeltaPt"] .Fill( (muPt-Pt_gen)/Pt_gen )

		self.eff_data["hPtResvsGenPt"]   .Fill( Pt_gen, (muPt-Pt_gen)/Pt_gen )
		self.eff_data["hEtaResvsGenEta"] .Fill( Eta_gen, muEta-Eta_gen          )
		self.eff_data["hPhiResvsGenPhi"] .Fill( Phi_gen, muPhi-Phi_gen          )

		self.eff_data["hMthPosMuPhivsEtavsPt_Gen"] .Fill(Pt_gen, Eta_gen, Phi_gen)
		self.eff_data["hMthPosMuPhivsEtavsPt"]     .Fill(muPt,      muEta,      muPhi     )

		if(isTrigMu):
			self.eff_data["hTrigPosMuPhivsEtavsPt"].Fill(muPt, muEta, muPhi)

	@clock
	def analyze(self):
		"""
		Analyze the data and fill the histograms
		"""

		for ientry in range(1,	self.csTree.GetEntries()):
			if (ientry % (self.csTree.GetEntries() / 10) == 0):
				print(f"Processing entry {ientry} of {self.csTree.GetEntries()}")

			if (self.csTree.GetEntry(ientry) < 0):
				raise RuntimeError(f"Invalid correct-sign entry! Error reading entry {ientry} of {self.csTree.GetEntries()}")

			cen				= self.csTree.centrality()
			vtxX			= self.csTree.bestvtxX()
			vtxY			= self.csTree.bestvtxY()
			vtxZ			= self.csTree.bestvtxZ()
			hfsumETPlus		= self.csTree.HFsumETPlus()
			hfsumETMinus	= self.csTree.HFsumETMinus()
			nTrkoffline		= self.csTree.Ntrkoffline()
			nTrkHP			= self.csTree.NtrkHP()

			self.eff_data["hnEvts"]					.Fill(0.5)
			self.eff_data["hVzvsVyvsVx"]			.Fill(vtxX, vtxY, vtxZ)
			self.eff_data["hHFMinusvsHFPlus"]		.Fill(hfsumETPlus, hfsumETMinus)
			self.eff_data["hNCanvsNtrkvsCen"]		.Fill(cen, nTrkoffline, self.csTree.candSize())
			self.eff_data["hNtrkofflinevsNtrkHP"]	.Fill(nTrkHP, nTrkoffline)

			if self.good_Vtx(): self.eff_data["hnEvts"].Fill(1.5)

			if self.good_Vtx() and self.good_HFVeto(): self.eff_data["hnEvts"].Fill(2.5)

			if self.pass_EvtSel(): self.eff_data["hnEvts"].Fill(3.5)

			#Loop over the correct-sign candidates
			nSoftMuon = 0
			for icand_gen in range(self.csTree.candSize_gen()):
		
				# -----------------------------------------------------------------------------------------------------
				# Gen candidates START--------------------------------------------------------------------------------------
				posPt_gen  =	self.csTree.pTD1_gen()	[icand_gen]	if self.csTree.chargeD1_gen()[icand_gen] > 0  else	self.csTree.pTD2_gen()	[icand_gen]
				posEta_gen =	self.csTree.EtaD1_gen()	[icand_gen]	if self.csTree.chargeD1_gen()[icand_gen] > 0  else	self.csTree.EtaD2_gen()	[icand_gen]
				posPhi_gen =	self.csTree.PhiD1_gen()	[icand_gen]	if self.csTree.chargeD1_gen()[icand_gen] > 0  else	self.csTree.PhiD2_gen()	[icand_gen]
				negPt_gen  =	self.csTree.pTD1_gen()	[icand_gen]	if self.csTree.chargeD1_gen()[icand_gen] < 0  else	self.csTree.pTD2_gen()	[icand_gen]
				negEta_gen =	self.csTree.EtaD1_gen()	[icand_gen]	if self.csTree.chargeD1_gen()[icand_gen] < 0  else	self.csTree.EtaD2_gen()	[icand_gen]
				negPhi_gen =	self.csTree.PhiD1_gen()	[icand_gen]	if self.csTree.chargeD1_gen()[icand_gen] < 0  else	self.csTree.PhiD2_gen()	[icand_gen]

				self.eff_data["hPhivsEtavsPt_Gen"]		.Fill(posPt_gen, posEta_gen, posPhi_gen)
				self.eff_data["hPhivsEtavsPt_Gen"]		.Fill(negPt_gen, negEta_gen, negPhi_gen)

				self.eff_data["hPosMuPhivsEtavsPt_Gen"]	.Fill(posPt_gen, posEta_gen, posPhi_gen)
				self.eff_data["hNegMuPhivsEtavsPt_Gen"]	.Fill(negPt_gen, negEta_gen, negPhi_gen)

				self.eff_data["hNegPtvsPosPt_Gen"]		.Fill(posPt_gen,  negPt_gen)
				self.eff_data["hNegEtavsPosEta_Gen"]	.Fill(posEta_gen, negEta_gen)
				self.eff_data["hNegPhivsPosPhi_Gen"]	.Fill(posPhi_gen, negPhi_gen)

				posFourMom_gen	= ROOT.TLorentzVector();	negFourMom_gen	= ROOT.TLorentzVector();	pairFourMom_gen	= ROOT.TLorentzVector()

				posFourMom_gen.SetPtEtaPhiM(posPt_gen, posEta_gen, posPhi_gen, self.Mmuon)
				negFourMom_gen.SetPtEtaPhiM(negPt_gen, negEta_gen, negPhi_gen, self.Mmuon)
				pairFourMom_gen = posFourMom_gen + negFourMom_gen

				pt_gen   = pairFourMom_gen.Pt()
				eta_gen  = pairFourMom_gen.Eta()
				phi_gen  = pairFourMom_gen.Phi()
				mass_gen = pairFourMom_gen.M()
				y_gen    = pairFourMom_gen.Rapidity()

				if(y_gen>1.5):
					self.eff_data["hForward_PhivsEtavsPt_Gen"].Fill(posPt_gen, posEta_gen, posPhi_gen)
					self.eff_data["hForward_PhivsEtavsPt_Gen"].Fill(negPt_gen, negEta_gen, negPhi_gen)

				asyPhi_gen = 1 - ROOT.TMath.Abs(self.shiftDeltaPhi(posFourMom_gen.DeltaPhi(negFourMom_gen))) / np.pi #acoplanarity

				muMomDiff_gen = posFourMom_gen.Vect() - negFourMom_gen.Vect()
				pairMom_gen   = pairFourMom_gen.Vect()
				phiDiff_gen   = self.shiftDeltaPhi(pairMom_gen.DeltaPhi(muMomDiff_gen))

				self.eff_data["hMvsPtvsRap_Gen"]		.Fill(y_gen, pt_gen,     mass_gen)
				self.eff_data["hMvsAsyPhivsRap_Gen"]	.Fill(y_gen, asyPhi_gen, mass_gen)
				self.eff_data["hDeltaPhivsM_Gen"]		.Fill(mass_gen, phiDiff_gen)

				# Gen candidates END----------------------------------------------------------------------------------------
				# -----------------------------------------------------------------------------------------------------
				

				# -----------------------------------------------------------------------------------------------------
				# Reco mu START-------------------------------------------------------------------------------------
				posMthDeltaR = 99999999.
				negMthDeltaR = 99999999.
				posRecoIdx   = -1
				negRecoIdx   = -1
				
				for imu in range(0, self.csTree.candSize_mu()):

					if( not self.csTree.softMuon_mu()[imu] ): continue

					if( icand_gen == 0 ): nSoftMuon += 1

					muPt  = self.csTree.pT_mu()[imu]
					muEta = self.csTree.eta_mu()[imu]
					muPhi = self.csTree.phi_mu()[imu]

					recoMom = ROOT.TVector3(); recoMom.SetPtEtaPhi(muPt, muEta, muPhi)
					posDeltaR = posFourMom_gen.Vect().DeltaR(recoMom)
					negDeltaR = negFourMom_gen.Vect().DeltaR(recoMom)

					# // DeltaPt/genPt < 0.15 ensures to keep 99.9842% events
					# // DeltaR < 0.05 ensures to keep 99.9763% events
					if(	np.abs((muPt-posPt_gen)/posPt_gen) < 0.15
						and posDeltaR < 0.05 
						and posDeltaR < posMthDeltaR
					):
						posMthDeltaR = posDeltaR
						posRecoIdx = imu
					if(	np.abs((muPt-negPt_gen)/negPt_gen) < 0.15
						and negDeltaR < 0.05
						and negDeltaR < negMthDeltaR
					):
						negMthDeltaR = negDeltaR
						negRecoIdx = imu
				
				if(posRecoIdx >= 0 and negRecoIdx>=0 and posRecoIdx == negRecoIdx):
					print("One reco-track is matched to multiple gen-tracks !")

				if( posRecoIdx >= 0 ):
					self.fill_mu( posRecoIdx, posPt_gen, posEta_gen, posPhi_gen, posMthDeltaR)

				if( negRecoIdx >= 0 ):
					self.fill_mu( negRecoIdx, negPt_gen, negEta_gen, negPhi_gen, negMthDeltaR)

				# Reco mu END----------------------------------------------------------------------------------------
				# -----------------------------------------------------------------------------------------------------


				# -----------------------------------------------------------------------------------------------------
				# Reco candidates START-------------------------------------------------------------------------------------
				recoIdx = self.csTree.RecIdx_gen()[icand_gen]

				if not self.good_recoMuon(recoIdx):
					continue

				posPt     = self.csTree.pTD1()[recoIdx]					if	self.csTree.chargeD1()[recoIdx] > 0	else	self.csTree.pTD2()[recoIdx]; 
				posEta    = self.csTree.EtaD1()[recoIdx]				if	self.csTree.chargeD1()[recoIdx] > 0	else	self.csTree.EtaD2()[recoIdx]; 
				posPhi    = self.csTree.PhiD1()[recoIdx]				if	self.csTree.chargeD1()[recoIdx] > 0	else	self.csTree.PhiD2()[recoIdx]; 
				isPosTrig = self.csTree.trigMuon1()[self.trigIdx][recoIdx]	if	self.csTree.chargeD1()[recoIdx] > 0	else	self.csTree.trigMuon2()[self.trigIdx][recoIdx]; 
				negPt     = self.csTree.pTD1()[recoIdx]					if	self.csTree.chargeD1()[recoIdx] < 0	else	self.csTree.pTD2()[recoIdx]; 
				negEta    = self.csTree.EtaD1()[recoIdx]				if	self.csTree.chargeD1()[recoIdx] < 0	else	self.csTree.EtaD2()[recoIdx]; 
				negPhi    = self.csTree.PhiD1()[recoIdx]				if	self.csTree.chargeD1()[recoIdx] < 0	else	self.csTree.PhiD2()[recoIdx]; 
				isNegTrig = self.csTree.trigMuon1()[self.trigIdx][recoIdx]	if	self.csTree.chargeD1()[recoIdx] < 0	else	self.csTree.trigMuon2()[self.trigIdx][recoIdx]; 

				# shift mean
				shiftPosPt = posPt - self.funPtMeanShift.Eval(posPt_gen)*posPt_gen
				shiftNegPt = negPt - self.funPtMeanShift.Eval(negPt_gen)*negPt_gen

				# smear width
				posPtNew = posPt_gen + (shiftPosPt - posPt_gen)/self.funRawPtRes.Eval(posPt_gen)*self.funTunedPtRes.Eval(posPt_gen)
				negPtNew = negPt_gen + (shiftNegPt - negPt_gen)/self.funRawPtRes.Eval(negPt_gen)*self.funTunedPtRes.Eval(negPt_gen)

				self.eff_data["hMthPosMuPhivsEtavsPtInpair"].Fill(posPtNew, posEta, posPhi)
				if(isPosTrig):	self.eff_data["hTrigPosMuPhivsEtavsPtInpair"].Fill(posPtNew, posEta, posPhi)

				self.eff_data["hMthNegMuPhivsEtavsPtInpair"].Fill(negPtNew, negEta, negPhi)
				if(isNegTrig):	self.eff_data["hTrigNegMuPhivsEtavsPtInpair"].Fill(negPtNew, negEta, negPhi)

				posFourMom	= ROOT.TLorentzVector();	negFourMom	= ROOT.TLorentzVector();	pairFourMom	= ROOT.TLorentzVector()
				posFourMom.SetPtEtaPhiM(posPtNew, posEta, posPhi, self.Mmuon)
				negFourMom.SetPtEtaPhiM(negPtNew, negEta, negPhi, self.Mmuon)
				pairFourMom = posFourMom + negFourMom

				# without pt smear
				pt_woSmear   = self.csTree.pT()[recoIdx]
				eta_woSmear  = self.csTree.eta()[recoIdx]
				phi_woSmear  = self.csTree.phi()[recoIdx]
				mass_woSmear = self.csTree.mass()[recoIdx]
				y_woSmear    = self.csTree.y()[recoIdx]

				# with pt smear
				pt   = pairFourMom.Pt()
				eta  = pairFourMom.Eta()
				phi  = pairFourMom.Phi()
				mass = pairFourMom.M()
				y    = pairFourMom.Rapidity()

				self.eff_data["hRcPairPtvsGenPairPt"]     .Fill(pt_gen,  pt )
				self.eff_data["hRcPairPhivsGenPairPhi"]   .Fill(phi_gen, phi)
				self.eff_data["hRcPairEtavsGenPairEta"]   .Fill(eta_gen, eta)
				self.eff_data["hMassResvsGenMass"]        .Fill(mass_gen, (mass-mass_gen)/mass_gen)
				self.eff_data["hPairPtResvsGenPairPt"]    .Fill(pt_gen,  pt - pt_gen)
				self.eff_data["hRapResvsGenRap"]          .Fill(y_gen,   y-y_gen    )

				asyPhi = 1 - np.abs(self.shiftDeltaPhi(posFourMom.DeltaPhi(negFourMom))) / np.pi; #//acoplanarity
	
				# // without pt smear
				if(self.goodMuPairIdx(recoIdx)):
					# print(icand_gen)
					self.eff_data["hMvsPtvsRap_woEvtSel_woSmear"].Fill(y_woSmear, pt_woSmear, mass_woSmear)
					if( self.pass_EvtSel() ):
						self.eff_data["hMvsPtvsRap_woSmear"].Fill(y_woSmear, pt_woSmear, mass_woSmear)

				# // with pt smear
				if( not self.goodMuPair(posFourMom, isPosTrig, negFourMom, isNegTrig) ): 
					continue
				
				self.eff_data["hMvsPtvsRap_woEvtSel"].Fill(y, pt, mass)
				if(self.pass_EvtSel()):
					self.eff_data["hMvsPtvsRap"]        .Fill(y, pt,     mass)
					self.eff_data["hMvsAsyPhivsRap"]    .Fill(y, asyPhi, mass)

					muMomDiff = posFourMom.Vect() - negFourMom.Vect()
					pairMom   = pairFourMom.Vect()

					phiDiff   = self.shiftDeltaPhi(pairMom.DeltaPhi(muMomDiff))
					self.eff_data["hDeltaPhivsM"]       .Fill(mass, phiDiff)
				
				# Reco level END----------------------------------------------------------------------------------------
				# -----------------------------------------------------------------------------------------------------
		
			self.eff_data["hNRcMuvsNGenMu"].Fill( self.csTree.candSize_gen()*2, nSoftMuon)



# %%
# # test
ana = EfficiencyAnalyzer()
# t = timeit.timeit(stmt=ana.analyze, number=10000)
# print(f"Execution time: {t:.6f} seconds")
ana.analyze()
c1 = ROOT.TCanvas("c1", "c1", 800, 600)
ana.eff_data["hMvsPtvsRap_woEvtSel"].Draw()
c1.SaveAs("test.pdf")

