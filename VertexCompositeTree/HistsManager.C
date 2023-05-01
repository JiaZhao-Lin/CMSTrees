/*
 A class for managing histograms produced in SkimTree
*/

struct HistsManager
{
	TFile* inFile = nullptr;

	TH1D *hM_Gen;
	TH1D *hPt_Gen;
	TH1D *hRap_Gen;
	TH1D *hM;
	TH1D *hPt;
	TH1D *hRap;
	TH1D *hM_InPair;
	TH1D *hPt_InPair;
	TH1D *hRap_InPair;
	TH1D *hnEvts;
	TH3D *hVzvsVyvsVx;
	TH2D *hHFMinusvsHFPlus;
	TH3D *hNCanvsNtrkvsCen;
	TH2D *hNtrkofflinevsNtrkHP;

	TH3D *hPhivsEtavsPt_Gen;
	TH3D *hForward_PhivsEtavsPt_Gen;
	TH2D *hNegPtvsPosPt_Gen;
	TH2D *hNegEtavsPosEta_Gen;
	TH2D *hNegPhivsPosPhi_Gen;
	TH1D *hDeltaR;
	TH1D *hDeltaPt;
	TH2D *hPtResvsGenPt;
	TH2D *hEtaResvsGenEta;
	TH2D *hPhiResvsGenPhi;

	TH2D *hNRcMuvsNGenMu;
	TH3D *hPhivsEtavsPt;

	TH2D *hRcPairPtvsGenPairPt;
	TH2D *hRcPairEtavsGenPairEta;
	TH2D *hRcPairPhivsGenPairPhi;
	TH2D *hMassResvsGenMass;
	TH2D *hPairPtResvsGenPairPt;
	TH2D *hRapResvsGenRap;

	TH3D *hMvsPtvsRap_Gen;
	TH3D *hMvsAsyPhivsRap_Gen;
	TH2D *hDeltaPhivsM_Gen;

	TH3D *hMvsPtvsRap_woEvtSel_woSmear;
	TH3D *hMvsPtvsRap_woSmear;

	TH3D *hMvsPtvsRap_woEvtSel;
	TH3D *hMvsPtvsRap;

	TH3D *hMvsAsyPhivsRap;
	TH2D *hDeltaPhivsM;

	// To calculate 3D efficiency
	TH3D *hPosKaonPhivsEtavsPt_Gen;
	TH3D *hMthPosKaonPhivsEtavsPt_Gen;
	TH3D *hMthPosKaonPhivsEtavsPt;
	TH3D *hTrigPosKaonPhivsEtavsPt;

	TH3D *hNegKaonPhivsEtavsPt_Gen;
	TH3D *hMthNegKaonPhivsEtavsPt_Gen;
	TH3D *hMthNegKaonPhivsEtavsPt;
	TH3D *hTrigNegKaonPhivsEtavsPt;

	TH3D *hMthPosKaonPhivsEtavsPtInpair;
	TH3D *hTrigPosKaonPhivsEtavsPtInpair;
	TH3D *hMthNegKaonPhivsEtavsPtInpair;
	TH3D *hTrigNegKaonPhivsEtavsPtInpair;

	void InitHists()
	{
		const Double_t PI = 3.141592;

		const Int_t    mHistRapBins     = 50;
		const Double_t mHistRapLow      = -4;
		const Double_t mHistRapHi       = 4;

		const Int_t    mHistPtBins      = 50;
		const Double_t mHistPtLow       = 0;
		const Double_t mHistPtHi        = 0.5;

		const Int_t    mHistMassBins    = 100;
		const Double_t mHistMassLow     = 0.9;
		const Double_t mHistMassHi      = 1.2;

		const Int_t    mHistAsyPhiBins  = 300;
		const Double_t mHistAsyPhiLow   = 0;
		const Double_t mHistAsyPhiHi    = 0.3;

		const Int_t    mHistAsyPtBins   = 500;
		const Double_t mHistAsyPtLow    = 0;
		const Double_t mHistAsyPtHi     = 0.5;

		// event level
		hnEvts = new TH1D("hnEvts", "hnEvts;", 5, 0, 5);
		hnEvts->GetXaxis()->SetBinLabel(1, "trigEvt");
		hnEvts->GetXaxis()->SetBinLabel(2, "validVtx & !Beam-halo");
		hnEvts->GetXaxis()->SetBinLabel(3, "HFMaxE <= 7.6(7.3) GeV");
		hnEvts->GetXaxis()->SetBinLabel(4, "N_{trk}^{HP} == 2");
		hnEvts->GetXaxis()->LabelsOption("d");
		hnEvts->GetXaxis()->SetLabelSize(0.06);

		hVzvsVyvsVx          = new TH3D("hVzvsVyvsVx",          "hVzvsVyvsVx; V_{x} (cm); V_{y} (cm); V_{z} (cm)", 100, -1, 1, 100, -1, 1, 300, -30, 30);
		hHFMinusvsHFPlus     = new TH2D("hHFMinusvsHFPlus",     "hHFMinusvsHFPlus; HFsumETPlus; HFsumETMinus",      200, 0, 20, 200, 0, 20);
		hNCanvsNtrkvsCen     = new TH3D("hNCanvsNtrkvsCen",     "hNCanvsNtrkvsCen; Centrality; N_{trk}^{offline}; # CS pair", 50, 150, 200, 10, 0, 10, 5, 0, 5);
		hNtrkofflinevsNtrkHP = new TH2D("hNtrkofflinevsNtrkHP", "hNtrkofflinevsNtrkHP; N_{trk}^{HP}; N_{trk}^{offline}", 10, 0, 10, 10, 0, 10);

		hPhivsEtavsPt_Gen    = new TH3D("hPhivsEtavsPt_Gen", "hPhivsEtavsPt_Gen; p_{T} (GeV/c); #eta; #phi", 500, 0, 5, 300, -3, 3, 180, -PI, PI);
		hForward_PhivsEtavsPt_Gen   = new TH3D("hForward_PhivsEtavsPt_Gen", "hForward_PhivsEtavsPt_Gen; p_{T} (GeV/c); #eta; #phi", 500, 0, 5, 300, -3, 3, 180, -PI, PI);
		hNegPtvsPosPt_Gen    = new TH2D("hNegPtvsPosPt_Gen", "hNegPtvsPosPt_Gen; #mu^{+} p_{T} (GeV/c); #mu^{-} p_{T} (GeV/c);", 500, 0, 5, 500, 0, 5);
		hNegEtavsPosEta_Gen  = new TH2D("hNegEtavsPosEta_Gen", "hNegEtavsPosEta_Gen; #mu^{+} #eta; #mu^{-} #eta;", 300, -3, 3, 300, -3, 3);
		hNegPhivsPosPhi_Gen  = new TH2D("hNegPhivsPosPhi_Gen", "hNegPhivsPosPhi_Gen; #mu^{+} #phi; #mu^{-} #phi;", 180, -PI, PI, 180, -PI, PI);
		hDeltaR              = new TH1D("hDeltaR", "hDeltaR; #DeltaR", 2000, 0, 0.2);
		hDeltaPt             = new TH1D("hDeltaPt", "hDeltaPt; (p_{T}^{Rc}-p_{T}^{Gen})/p_{T}^{Gen}", 5000, -0.5, 0.5);
		hPtResvsGenPt        = new TH2D("hPtResvsGenPt", "hPtResvsGenPt; p_{T}^{Gen} (GeV/c); (p_{T}^{Rc}-p_{T}^{Gen})/p_{T}^{Gen};", 500, 0, 5, 300, -0.15, 0.15);
		hEtaResvsGenEta      = new TH2D("hEtaResvsGenEta", "hEtaResvsGenEta; #eta^{Gen}; #eta^{Rc}-#eta^{Gen};", 300, -3, 3, 200, -0.01, 0.01);
		hPhiResvsGenPhi      = new TH2D("hPhiResvsGenPhi", "hPhiResvsGenPhi; #phi^{Gen}; #phi^{Rc}-#phi^{Gen};", 180, -PI, PI, 200, -0.01, 0.01);

		hNRcMuvsNGenMu          = new TH2D("hNRcMuvsNGenMu", "hNRcMuvsNGenMu; # of muon (GEN); # of muon (Soft)", 10, 0, 10, 10, 0, 10);
		hPhivsEtavsPt           = new TH3D("hPhivsEtavsPt", "hPhivsEtavsPt; p_{T} (GeV/c); #eta; #phi", 500, 0, 5, 300, -3, 3, 180, -PI, PI);

		hRcPairPtvsGenPairPt   = new TH2D("hRcPairPtvsGenPairPt", "hRcPairPtvsGenPairPt; pair p_{T}^{Gen} (GeV/c); pair p_{T}^{Rc} (GeV/c);", mHistPtBins, mHistPtLow, mHistPtHi, mHistPtBins, mHistPtLow, mHistPtHi);
		hRcPairEtavsGenPairEta = new TH2D("hRcPairEtavsGenPairEta", "hRcPairEtavsGenPairEta; pair #eta^{Gen}; pair #eta^{Rc};", 160, -4, 4, 160, -4, 4);
		hRcPairPhivsGenPairPhi = new TH2D("hRcPairPhivsGenPairPhi", "hRcPairPhivsGenPairPhi; pair #phi^{Gen}; pair #phi^{Rc};", 120, -PI, PI, 120, -PI, PI);
		hMassResvsGenMass      = new TH2D("hMassResvsGenMass", "hMassResvsGenMass; M_{#mu#mu}^{Gen} (GeV/c^{2}); (M_{#mu#mu}^{Rc} - M_{#mu#mu}^{Gen})/M_{#mu#mu}^{Gen};", mHistMassBins, mHistMassLow, mHistMassHi, 300, -0.15, 0.15);
		hPairPtResvsGenPairPt  = new TH2D("hPairPtResvsGenPairPt", "hPairPtResvsGenPairPt; J/#psi p_{T}^{Gen} (GeV/c); J/#psi p_{T}^{Rc} - p_{T}^{Gen};", 300, 0, 0.3, 300, -0.3, 0.3);
		hRapResvsGenRap        = new TH2D("hRapResvsGenRap", "hRapResvsGenRap; pair y^{Gen}; y^{Rc} - y^{Gen};", 600, -3, 3, 1000, -0.1, 0.1);

		hMvsPtvsRap_Gen     = new TH3D("hMvsPtvsRap_Gen", "hMvsPtvsRap_Gen; Rapidity; p_{T} (GeV/c); M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistPtBins, mHistPtLow, mHistPtHi, mHistMassBins, mHistMassLow, mHistMassHi);
		hM_Gen		     = new TH1D("hM_Gen", "hM_Gen; M_{KK} (GeV/c^{2})", mHistMassBins, mHistMassLow, mHistMassHi);
		hPt_Gen		     = new TH1D("hPt_Gen", "hPt_Gen; p_{T} (GeV/c)", mHistPtBins, mHistPtLow, mHistPtHi);
		hRap_Gen	     = new TH1D("hRap_Gen", "hRap_Gen; Rapidity", mHistRapBins, mHistRapLow, mHistRapHi);
		hM		     = new TH1D("hM", "hM; M_{KK} (GeV/c^{2})", mHistMassBins, mHistMassLow, mHistMassHi);
		hPt		     = new TH1D("hPt", "hPt; p_{T} (GeV/c)", mHistPtBins, mHistPtLow, mHistPtHi);
		hRap	     = new TH1D("hRap", "hRap; Rapidity", mHistRapBins, mHistRapLow, mHistRapHi);
		hM_InPair    = new TH1D("hM_InPair", "hM; M_{KK} (GeV/c^{2})", mHistMassBins, mHistMassLow, mHistMassHi);
		hPt_InPair		     = new TH1D("hPt_InPair", "hPt; p_{T} (GeV/c)", mHistPtBins, mHistPtLow, mHistPtHi);
		hRap_InPair	     = new TH1D("hRap_InPair", "hRap; Rapidity", mHistRapBins, mHistRapLow, mHistRapHi);
		hMvsAsyPhivsRap_Gen = new TH3D("hMvsAsyPhivsRap_Gen", "hMvsAsyPhivsRap_Gen; Rapidity; #alpha; M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistAsyPhiBins, mHistAsyPhiLow, mHistAsyPhiHi, mHistMassBins, mHistMassLow, mHistMassHi);
		hDeltaPhivsM_Gen   = new TH2D("hDeltaPhivsM_Gen", "hDeltaPhivsM_Gen; M_{#mu#mu} (GeV/c^{2}); #phi_{#mu^{+}+#mu^{-}} - #phi_{#mu^{+}-#mu^{-}}", mHistMassBins, mHistMassLow, mHistMassHi, 120, -PI, PI);

		hMvsPtvsRap_woEvtSel_woSmear = new TH3D("hMvsPtvsRap_woEvtSel_woSmear", "hMvsPtvsRap_woEvtSel_woSmear; Rapidity; p_{T} (GeV/c); M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistPtBins, mHistPtLow, mHistPtHi, mHistMassBins, mHistMassLow, mHistMassHi);
		hMvsPtvsRap_woSmear = new TH3D("hMvsPtvsRap_woSmear", "hMvsPtvsRap_woSmear; Rapidity; p_{T} (GeV/c); M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistPtBins, mHistPtLow, mHistPtHi, mHistMassBins, mHistMassLow, mHistMassHi);

		hMvsPtvsRap_woEvtSel = new TH3D("hMvsPtvsRap_woEvtSel", "hMvsPtvsRap_woEvtSel; Rapidity; p_{T} (GeV/c); M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistPtBins, mHistPtLow, mHistPtHi, mHistMassBins, mHistMassLow, mHistMassHi);
		hMvsPtvsRap = new TH3D("hMvsPtvsRap", "hMvsPtvsRap; Rapidity; p_{T} (GeV/c); M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistPtBins, mHistPtLow, mHistPtHi, mHistMassBins, mHistMassLow, mHistMassHi);
		hMvsAsyPhivsRap = new TH3D("hMvsAsyPhivsRap", "hMvsAsyPhivsRap; Rapidity; #alpha; M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistAsyPhiBins, mHistAsyPhiLow, mHistAsyPhiHi, mHistMassBins, mHistMassLow, mHistMassHi);
		hDeltaPhivsM = new TH2D("hDeltaPhivsM", "hDeltaPhivsM; M_{#mu#mu} (GeV/c^{2}); #phi_{#mu^{+}+#mu^{-}} - #phi_{#mu^{+}-#mu^{-}}", mHistMassBins, mHistMassLow, mHistMassHi, 120, -PI, PI);

		const Int_t nPtBins = 25;
		Double_t mPtLow = 0.0, mPtHi = 1;
		Double_t mPtStep = (mPtHi - mPtLow)/nPtBins;
		Double_t Pt[nPtBins+1];

		// Double_t    Pt[nPtBins+1] = {0, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 3.0, 3.2, 3.6, 4.0};
		for(Int_t iPt=0; iPt<=nPtBins; iPt++)
		{
			Pt[iPt] = mPtLow + iPt*mPtStep;
		}

		const Int_t nEtaBins = 100;
		Double_t Eta[nEtaBins+1];
		Double_t mEtaLow = -6, mEtaHi = 6;
		Double_t mEtaStep = (mEtaHi - mEtaLow)/nEtaBins;

		for(Int_t ieta=0; ieta<=nEtaBins; ieta++)
		{
			Eta[ieta] = mEtaLow + ieta*mEtaStep;
		}

		const Int_t nPhiBins = 60;
		Double_t Phi[nPhiBins+1];
		Double_t mPhiLow  = -PI, mPhiHi = PI;
		Double_t mPhiStep = (mPhiHi - mPhiLow)/nPhiBins;

		for(Int_t iphi=0; iphi<=nPhiBins; iphi++)
		{
			Phi[iphi] = mPhiLow + iphi*mPhiStep;
		}

		cout<<endl;
		cout<<"Pt Bin Boundary: "<<endl;
		for(Int_t ibin=0; ibin<nPtBins; ibin++)
		{
			cout<<Pt[ibin]<<", ";
		}
		cout<<Pt[nPtBins]<<endl;
		cout<<endl;

		cout<<"Eta Bin Boundary: "<<endl;
		for(Int_t ibin=0; ibin<nEtaBins; ibin++)
		{
			cout<<Eta[ibin]<<", ";
		}
		cout<<Eta[nEtaBins]<<endl;
		cout<<endl;

		cout<<"Phi Bin Boundary: "<<endl;
		for(Int_t ibin=0; ibin<nPhiBins; ibin++)
		{
			cout<<Phi[ibin]<<", ";
		}
		cout<<Phi[nPhiBins]<<endl;
		cout<<endl;

		hPosKaonPhivsEtavsPt_Gen    = new TH3D("hPosKaonPhivsEtavsPt_Gen",    "hPosKaonPhivsEtavsPt_Gen;    p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
		hMthPosKaonPhivsEtavsPt_Gen = new TH3D("hMthPosKaonPhivsEtavsPt_Gen", "hMthPosKaonPhivsEtavsPt_Gen; p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
		hMthPosKaonPhivsEtavsPt     = new TH3D("hMthPosKaonPhivsEtavsPt",     "hMthPosKaonPhivsEtavsPt;     p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
		hTrigPosKaonPhivsEtavsPt    = new TH3D("hTrigPosKaonPhivsEtavsPt",    "hTrigPosKaonPhivsEtavsPt;    p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
		hNegKaonPhivsEtavsPt_Gen    = new TH3D("hNegKaonPhivsEtavsPt_Gen",    "hNegKaonPhivsEtavsPt_Gen;    p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
		hMthNegKaonPhivsEtavsPt_Gen = new TH3D("hMthNegKaonPhivsEtavsPt_Gen", "hMthNegKaonPhivsEtavsPt_Gen; p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
		hMthNegKaonPhivsEtavsPt     = new TH3D("hMthNegKaonPhivsEtavsPt",     "hMthNegKaonPhivsEtavsPt;     p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
		hTrigNegKaonPhivsEtavsPt    = new TH3D("hTrigNegKaonPhivsEtavsPt",    "hTrigNegKaonPhivsEtavsPt;    p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);

		hMthPosKaonPhivsEtavsPtInpair  = new TH3D("hMthPosKaonPhivsEtavsPtInpair",  "hMthPosKaonPhivsEtavsPtInpair;  p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
		hTrigPosKaonPhivsEtavsPtInpair = new TH3D("hTrigPosKaonPhivsEtavsPtInpair", "hTrigPosKaonPhivsEtavsPtInpair; p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
		hMthNegKaonPhivsEtavsPtInpair  = new TH3D("hMthNegKaonPhivsEtavsPtInpair",  "hMthNegKaonPhivsEtavsPtInpair;  p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
		hTrigNegKaonPhivsEtavsPtInpair = new TH3D("hTrigNegKaonPhivsEtavsPtInpair", "hTrigNegKaonPhivsEtavsPtInpair; p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
	}

	void WriteHists(TString outFileDir = "outFiles/Test.root")
	{
		TFile* fOut = new TFile(outFileDir, "recreate");

		fOut->cd();

		hnEvts->Write();
		// hVzvsVyvsVx->Write();
		// hHFMinusvsHFPlus->Write();
		// hNCanvsNtrkvsCen->Write();
		// hNtrkofflinevsNtrkHP->Write();

		// hPhivsEtavsPt_Gen->Write();
		// hForward_PhivsEtavsPt_Gen->Write();
		// hNegPtvsPosPt_Gen->Write();
		// hNegEtavsPosEta_Gen->Write();
		// hNegPhivsPosPhi_Gen->Write();
		// hDeltaR->Write();
		// hDeltaPt->Write();
		// hPtResvsGenPt->Write();
		// hEtaResvsGenEta->Write();
		// hPhiResvsGenPhi->Write();

		// hNRcMuvsNGenMu->Write();
		// hPhivsEtavsPt->Write();

		// hRcPairPtvsGenPairPt->Write();
		// hRcPairEtavsGenPairEta->Write();
		// hRcPairPhivsGenPairPhi->Write();
		// hMassResvsGenMass->Write();
		// hPairPtResvsGenPairPt->Write();
		// hRapResvsGenRap->Write();

		hMvsPtvsRap_Gen->Write();
		hM_Gen->Write();
		hPt_Gen->Write();
		hRap_Gen->Write();
		// hMvsAsyPhivsRap_Gen->Write();
		// hDeltaPhivsM_Gen->Write();

		// hMvsPtvsRap_woEvtSel_woSmear->Write();
		// hMvsPtvsRap_woSmear->Write();

		// hMvsPtvsRap_woEvtSel->Write();
		hMvsPtvsRap->Write();
		hM->Write();
		hPt->Write();
		hRap->Write();
		hM_InPair->Write();
		hPt_InPair->Write();
		hRap_InPair->Write();
		// hMvsAsyPhivsRap->Write();
		// hDeltaPhivsM->Write();

		hPosKaonPhivsEtavsPt_Gen->Write();
		hMthPosKaonPhivsEtavsPt_Gen->Write();
		hMthPosKaonPhivsEtavsPt->Write();
		hTrigPosKaonPhivsEtavsPt->Write();

		hNegKaonPhivsEtavsPt_Gen->Write();
		hMthNegKaonPhivsEtavsPt_Gen->Write();
		hMthNegKaonPhivsEtavsPt->Write();
		hTrigNegKaonPhivsEtavsPt->Write();

		hMthPosKaonPhivsEtavsPtInpair->Write();
		hTrigPosKaonPhivsEtavsPtInpair->Write();
		hMthNegKaonPhivsEtavsPtInpair->Write();
		hTrigNegKaonPhivsEtavsPtInpair->Write();

		fOut->Close();
	}

	void ReadHists(TString fileName)
	{
		inFile = new TFile(fileName, "read");
	}
};