#include "common/headers.h"
#include "common/funUtil.h"
#include "common/VertexCompositeTree.h"
#include "Selections.C"

const std::vector<int> trigIdxes = {0, 1, 2, 3, 4, 5, 6, 7};
const int nTrig = trigIdxes.size();

TH1D *hnEvts;
TH3D* hPhivsEtavsPt_Gen;
TH3D* hMvsPtvsRap_Gen;
TH3D* hPosMuPhivsEtavsPt_Gen;
TH3D* hMthPosMuPhivsEtavsPt_Gen;
TH3D* hMthPosMuPhivsEtavsPt;
TH3D* hNegMuPhivsEtavsPt_Gen;
TH3D* hMthNegMuPhivsEtavsPt_Gen;
TH3D* hMthNegMuPhivsEtavsPt;

std::vector<TH3D*> hTrigPosMuPhivsEtavsPt_vec(nTrig);
std::vector<TH3D*> hTrigNegMuPhivsEtavsPt_vec(nTrig);

void bookHistos()
{
	const Int_t    mHistRapBins     = 60;
	const Double_t mHistRapLow      = -3;
	const Double_t mHistRapHi       = 3;

	const Int_t    mHistPtBins      = 800;
	const Double_t mHistPtLow       = 0;
	const Double_t mHistPtHi        = 4;
	
	const Int_t    mHistMassBins    = 300;
	const Double_t mHistMassLow     = 2;
	const Double_t mHistMassHi      = 5;

	const Int_t nPtBins = 25;
	Double_t    Pt[nPtBins+1] = {0, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 3.0, 3.2, 3.6, 4.0};

	const Int_t nEtaBins = 100;
	Double_t Eta[nEtaBins+1];
	Double_t mEtaLow = -2.5, mEtaHi = 2.5;
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

	// event level
	hnEvts = new TH1D("hnEvts", "hnEvts;", 5, 0, 5);
	hnEvts->GetXaxis()->SetBinLabel(1, "trigEvt");
	hnEvts->GetXaxis()->SetBinLabel(2, "validVtx & !Beam-halo");
	hnEvts->GetXaxis()->SetBinLabel(3, "HFMaxE <= 7.6(7.3) GeV");
	hnEvts->GetXaxis()->SetBinLabel(4, "N_{trk}^{HP} == 2");
	hnEvts->GetXaxis()->LabelsOption("d");
	hnEvts->GetXaxis()->SetLabelSize(0.06);

	hPhivsEtavsPt_Gen   	 	= new TH3D("hPhivsEtavsPt_Gen", "hPhivsEtavsPt_Gen; p_{T} (GeV/c); #eta; #phi", 500, 0, 5, 300, -3, 3, 180, -PI, PI);;
	hMvsPtvsRap_Gen				= new TH3D("hMvsPtvsRap_Gen", "hMvsPtvsRap_Gen; Rapidity; p_{T} (GeV/c); M_{#mu#mu} (GeV/c^{2})", mHistRapBins, mHistRapLow, mHistRapHi, mHistPtBins, mHistPtLow, mHistPtHi, mHistMassBins, mHistMassLow, mHistMassHi);

	hPosMuPhivsEtavsPt_Gen    	= new TH3D("hPosMuPhivsEtavsPt_Gen",    "hPosMuPhivsEtavsPt_Gen;    p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
	hMthPosMuPhivsEtavsPt_Gen 	= new TH3D("hMthPosMuPhivsEtavsPt_Gen", "hMthPosMuPhivsEtavsPt_Gen; p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
	hMthPosMuPhivsEtavsPt     	= new TH3D("hMthPosMuPhivsEtavsPt",     "hMthPosMuPhivsEtavsPt;     p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
	hNegMuPhivsEtavsPt_Gen    	= new TH3D("hNegMuPhivsEtavsPt_Gen",    "hNegMuPhivsEtavsPt_Gen;    p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
	hMthNegMuPhivsEtavsPt_Gen 	= new TH3D("hMthNegMuPhivsEtavsPt_Gen", "hMthNegMuPhivsEtavsPt_Gen; p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
	hMthNegMuPhivsEtavsPt     	= new TH3D("hMthNegMuPhivsEtavsPt",     "hMthNegMuPhivsEtavsPt;     p_{T} (GeV/c); #eta; #phi", nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);

	for (int i = 0; i < nTrig; i++)
	{
		hTrigPosMuPhivsEtavsPt_vec[i] = new TH3D(Form("hTrigPosMuPhivsEtavsPt_trigIdx%d", trigIdxes[i]), Form("hTrigPosMuPhivsEtavsPt_trigIdx%d; p_{T} (GeV/c); #eta; #phi", trigIdxes[i]), nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
		hTrigNegMuPhivsEtavsPt_vec[i] = new TH3D(Form("hTrigNegMuPhivsEtavsPt_trigIdx%d", trigIdxes[i]), Form("hTrigNegMuPhivsEtavsPt_trigIdx%d; p_{T} (GeV/c); #eta; #phi", trigIdxes[i]), nPtBins, Pt, nEtaBins, Eta, nPhiBins, Phi);
	}
}

void SkimTree_TrigEff()
{
	TH1::SetDefaultSumw2(kTRUE);
	const TString inputFileDir = "inFiles/dimuana_mc_CohJpsi_test.root";
	TFile *inputFile = new TFile(inputFileDir, "READ");

	const TString TreeDir = "dimucontana_mc/VertexCompositeNtuple";

	// ===================================================================================
	// = Set up the TTreeReader and TTreeReaderValue objects.
	TTreeReader treeReader(TreeDir, inputFile);

	TTreeReaderValue<Int_t>					NtrkHP(treeReader,				"NtrkHP");
	TTreeReaderArray<Bool_t>				evtSel(treeReader,				"evtSel");
	
	TTreeReaderValue<UInt_t>				candSize_gen(treeReader,		"candSize_gen");
	TTreeReaderArray<Short_t>				chargeD1_gen(treeReader,		"chargeD1_gen");
	TTreeReaderArray<Short_t>				chargeD2_gen(treeReader,		"chargeD2_gen");
	TTreeReaderArray<Float_t>				pTD1_gen(treeReader,			"pTD1_gen");
	TTreeReaderArray<Float_t>				pTD2_gen(treeReader,			"pTD2_gen");
	TTreeReaderArray<Float_t>				EtaD1_gen(treeReader,			"EtaD1_gen");
	TTreeReaderArray<Float_t>				EtaD2_gen(treeReader,			"EtaD2_gen");
	TTreeReaderArray<Float_t>				PhiD1_gen(treeReader,			"PhiD1_gen");
	TTreeReaderArray<Float_t>				PhiD2_gen(treeReader,			"PhiD2_gen");

	TTreeReaderValue<UInt_t>				candSize_mu(treeReader,				"candSize_mu");
	TTreeReaderArray<Bool_t>				softMuon_mu(treeReader,				"softMuon_mu");
	TTreeReaderArray<Float_t>				pT_mu(treeReader,					"pT_mu");
	TTreeReaderArray<Float_t>				eta_mu(treeReader,					"eta_mu");
	TTreeReaderArray<Float_t>				phi_mu(treeReader,					"phi_mu");

	TTreeReaderValue<std::vector< std::vector<UChar_t> >>
											trigMuon_mu(treeReader,				"trigMuon_mu");

	// =======================================================================================


	bookHistos();

	int jentry = -1;
	while (treeReader.Next())
	{
		jentry++;
		if (jentry % (treeReader.GetEntries() / 10) == 0)
			cout << "begin " << jentry << "th entry...." << endl;

		Int_t   nTrkHP       = *NtrkHP;

		hnEvts               ->Fill(0.5);

		// Select Event - require this event has a valid vertex and is not beam-halo event
		//if(!csTree.evtSel()[2] || !csTree.evtSel()[3]) continue;
		Bool_t goodVtx = (evtSel[2] && evtSel[3]);
		if(goodVtx) hnEvts->Fill(1.5);

		//evtSel[4-15]
		//[4]=0: HFPlusMaxTower < 3 GeV;  [4]=1: HFPlusMaxTower > 3 GeV
		//[5]=0: HFMinusMaxTower < 3 GeV;  [5]=1: HFMinusMaxTower > 3 GeV
		//[6] is for Plus & [7] is for Minus; Threshold = 4 GeV
		//[8] is for Plus & [9] is for Minus; Threshold = 5 GeV
		//[10] is for Plus & [11] is for Minus; Threshold = 6 GeV
		//[12] is for Plus & [13] is for Minus; Threshold = 7 GeV
		//[14] is for Plus & [15] is for Minus; Threshold = 8 GeV
		//[16] is for Plus (Th = 7.6 GeV) & [17] is for Minus (Th = 7.3 GeV);

		if(evtSel[16] || evtSel[17]) continue;
		Bool_t goodHFVeto = (!evtSel[16] && !evtSel[17]);
		if(goodVtx & goodHFVeto) hnEvts->Fill(2.5);

		//if(nTrkHP != 2) continue;

		Bool_t passEvtSel = goodVtx && goodHFVeto && (nTrkHP==2);

		if(passEvtSel) hnEvts->Fill(3.5);

		// Loop over the correct-sign candidates
		Int_t nSoftMuon = 0;
		for (UInt_t icand = 0; icand < *candSize_gen; icand++)
		{
			Double_t posPt_gen  = chargeD1_gen[icand] > 0 ? pTD1_gen[icand]  : pTD2_gen[icand];
			Double_t posEta_gen = chargeD1_gen[icand] > 0 ? EtaD1_gen[icand] : EtaD2_gen[icand];
			Double_t posPhi_gen = chargeD1_gen[icand] > 0 ? PhiD1_gen[icand] : PhiD2_gen[icand];
			Double_t negPt_gen  = chargeD1_gen[icand] < 0 ? pTD1_gen[icand]  : pTD2_gen[icand];
			Double_t negEta_gen = chargeD1_gen[icand] < 0 ? EtaD1_gen[icand] : EtaD2_gen[icand];
			Double_t negPhi_gen = chargeD1_gen[icand] < 0 ? PhiD1_gen[icand] : PhiD2_gen[icand];

			hPhivsEtavsPt_Gen      ->Fill(posPt_gen, posEta_gen, posPhi_gen);
			hPhivsEtavsPt_Gen      ->Fill(negPt_gen, negEta_gen, negPhi_gen);

			hPosMuPhivsEtavsPt_Gen ->Fill(posPt_gen, posEta_gen, posPhi_gen);
			hNegMuPhivsEtavsPt_Gen ->Fill(negPt_gen, negEta_gen, negPhi_gen);

			TLorentzVector posFourMom_gen, negFourMom_gen, pairFourMom_gen;
			posFourMom_gen.SetPtEtaPhiM(posPt_gen, posEta_gen, posPhi_gen, MASS_MUON);
			negFourMom_gen.SetPtEtaPhiM(negPt_gen, negEta_gen, negPhi_gen, MASS_MUON);
			pairFourMom_gen = posFourMom_gen + negFourMom_gen;

			Double_t pt_gen   = pairFourMom_gen.Pt();
			Double_t eta_gen  = pairFourMom_gen.Eta();
			Double_t phi_gen  = pairFourMom_gen.Phi();
			Double_t mass_gen = pairFourMom_gen.M();
			Double_t y_gen    = pairFourMom_gen.Rapidity();

			hMvsPtvsRap_Gen        ->Fill(y_gen, pt_gen,     mass_gen);

			Double_t posMthDeltaR = 99999999.;
			Double_t negMthDeltaR = 99999999.;
			Int_t    posRecoIdx   = -1;
			Int_t    negRecoIdx   = -1;

			for(UInt_t imu=0; imu < *candSize_mu; imu++)
			{
				if( !softMuon_mu[imu] ) continue;

				if( icand==0 ) nSoftMuon++;

				Double_t muPt  = pT_mu[imu];
				Double_t muEta = eta_mu[imu];
				Double_t muPhi = phi_mu[imu];

				TVector3 Mom_rec; Mom_rec.SetPtEtaPhi(muPt, muEta, muPhi);
				TVector3 posMom_gen = posFourMom_gen.Vect();
				TVector3 negMom_gen = negFourMom_gen.Vect();

				if ( RecGenMatched(Mom_rec, posMom_gen, posMthDeltaR) )
				{
					posMthDeltaR = posMom_gen.DeltaR(Mom_rec);
					posRecoIdx   = imu;
				}
				if (RecGenMatched(Mom_rec, negMom_gen, negMthDeltaR))
				{
					negMthDeltaR = negMom_gen.DeltaR(Mom_rec);
					negRecoIdx   = imu;
				}
			}

			if(posRecoIdx>=0 && negRecoIdx>=0 && posRecoIdx == negRecoIdx)
			{
				cout<<"One reco-track is matched to multiple gen-tracks !"<<endl;
			}

			if( posRecoIdx>=0 )
			{
				Double_t muPt     = pT_mu[posRecoIdx];
				Double_t muEta    = eta_mu[posRecoIdx];
				Double_t muPhi    = phi_mu[posRecoIdx];

				hMthPosMuPhivsEtavsPt_Gen ->Fill(posPt_gen, posEta_gen, posPhi_gen);
				hMthPosMuPhivsEtavsPt     ->Fill(muPt,      muEta,      muPhi     );


				cout<<trigMuon_mu->size()<<endl;
				// for (int iTrig = 0; iTrig < nTrig; ++iTrig)
				// {
				// 	if (trigMuon_mu->at(trigIdxes[iTrig]).at(posRecoIdx))
				// 	{
				// 		hTrigPosMuPhivsEtavsPt_vec[iTrig]->Fill(muPt, muEta, muPhi);
				// 	}
				// }
			}

			// if(negRecoIdx>=0)
			// {
			// 	Double_t muPt     = pT_mu()[negRecoIdx];
			// 	Double_t muEta    = eta_mu()[negRecoIdx];
			// 	Double_t muPhi    = phi_mu()[negRecoIdx];

			// 	hMthNegMuPhivsEtavsPt_Gen->Fill(negPt_gen, negEta_gen, negPhi_gen);
			// 	hMthNegMuPhivsEtavsPt->Fill(muPt, muEta, muPhi);

			// 	for (int iTrig = 0; iTrig < nTrig; ++iTrig)
			// 	{
			// 		if (trigMuon_mu()[iTrig][negRecoIdx])
			// 		{
			// 			hTrigNegMuPhivsEtavsPt_vec[iTrig]->Fill(muPt, muEta, muPhi);
			// 		}
			// 	}
			// }
		}
	}
}