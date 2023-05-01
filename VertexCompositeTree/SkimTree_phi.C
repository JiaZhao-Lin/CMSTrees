#include "Selections.C"
#include "HistsManager.C"

void SkimTree_phi(TString fileName = "inFiles/phi_tree_highBetaStar.root")
{

	cout<<"running with: "<<fileName<<endl;
	TH1::SetDefaultSumw2(kTRUE);

	// Open the file containing the tree.
	TFile *myFile = TFile::Open(fileName);

	if (!myFile) {
		std::cerr << "Error: failed to open file " << fileName << std::endl;
		return;
	}

	HistsManager cHM = HistsManager();
	cHM.InitHists();


	// # -----------------------------------------------------------------------------------
	// # Set up the TTreeReader and TTreeReaderValue objects.
	TTreeReader treeReader("generalanaNew/ParticleTree", myFile);
	// TreeValues cTV = TreeValues(treeReader, TreeValuesList, TreeValuesVectorList);

	TTreeReaderValue<std::vector<Float_t>>		gen_pT(treeReader,		"gen_pT");
	TTreeReaderValue<std::vector<Float_t>>		gen_eta(treeReader,		"gen_eta");
	TTreeReaderValue<std::vector<Float_t>>		gen_phi(treeReader,		"gen_phi");
	TTreeReaderValue<std::vector<char>>			gen_charge(treeReader,	"gen_charge");
	TTreeReaderValue<std::vector<Float_t>>		gen_mass(treeReader,	"gen_mass");
	TTreeReaderValue<std::vector<Int_t>>		gen_pdgId(treeReader,	"gen_pdgId");

	TTreeReaderValue<std::vector<Bool_t>>		evtSel(treeReader,		"evtSel");
	TTreeReaderValue<std::vector<Float_t>>		cand_mass(treeReader,	"cand_mass");
	TTreeReaderValue<std::vector<Int_t>>		cand_pdgId(treeReader,	"cand_pdgId");
	TTreeReaderValue<std::vector<Float_t>>		cand_pT(treeReader,		"cand_pT");
	TTreeReaderValue<std::vector<Float_t>>		cand_eta(treeReader,	"cand_eta");
	TTreeReaderValue<std::vector<Float_t>>		cand_phi(treeReader,	"cand_phi");
	TTreeReaderValue<std::vector<Float_t>>		cand_y(treeReader,		"cand_y");
	TTreeReaderValue<std::vector<char>>			cand_charge(treeReader,	"cand_charge");



	// TTreeReaderValue<unsigned char>		centrality(treeReader,		"centrality");
	TTreeReaderValue<Float_t>			bestvtxX(treeReader,		"bestvtxX");
	TTreeReaderValue<Float_t>			bestvtxY(treeReader,		"bestvtxY");
	TTreeReaderValue<Float_t>			bestvtxZ(treeReader,		"bestvtxZ");
	// TTreeReaderValue<Float_t>			HFsumETPlus(treeReader,		"HFsumETPlus");
	// TTreeReaderValue<Float_t>			HFsumETMinus(treeReader,	"HFsumETMinus");
	// TTreeReaderValue<unsigned short>	Ntrkoffline(treeReader,		"Ntrkoffline");
	TTreeReaderValue<Int_t>				Ntrkgen(treeReader,			"Ntrkgen");
	// # -----------------------------------------------------------------------------------


	int jentry = 0;
	int total_entries = treeReader.GetEntries();
	while (treeReader.Next()) {
		jentry++;
		if (jentry % (total_entries / 10) == 0)
			cout << "begin " << jentry << "th entry...." << endl;

		// # EVENT START--------------------------------------------------------------------------
		// # ------------------------------------------------------------------------------------
		//evtSel[4-15]
		//[4]=0: HFPlusMaxTower < 3 GeV;  [4]=1: HFPlusMaxTower > 3 GeV
		//[5]=0: HFMinusMaxTower < 3 GeV;  [5]=1: HFMinusMaxTower > 3 GeV
		//[6] is for Plus & [7] is for Minus; Threshold = 4 GeV
		//[8] is for Plus & [9] is for Minus; Threshold = 5 GeV
		//[10] is for Plus & [11] is for Minus; Threshold = 6 GeV
		//[12] is for Plus & [13] is for Minus; Threshold = 7 GeV
		//[14] is for Plus & [15] is for Minus; Threshold = 8 GeV
		//[16] is for Plus (Th = 7.6 GeV) & [17] is for Minus (Th = 7.3 GeV); 

		cHM.hnEvts->Fill(0.5);

		// * Select Event - require this event has a valid vertex and is not beam-halo event
		Bool_t goodVtx = (evtSel->at(2) && evtSel->at(3));
		if(goodVtx) cHM.hnEvts->Fill(1.5);

		// ! Skip HF veto for now due to lack of HF info in MC
		// Bool_t goodHFVeto = (!evtSel->at(16) && !evtSel->at(17));
		Bool_t goodHFVeto = true;
		if(goodVtx & goodHFVeto) cHM.hnEvts->Fill(2.5);

		// ! Skip HF veto for now due to lack of HF info in MC
		// Bool_t passEvtSel = goodVtx && goodHFVeto && (nTrkHP==2);
		Bool_t passEvtSel = goodVtx;
		if(passEvtSel) cHM.hnEvts->Fill(3.5);

		// # ------------------------------------------------------------------------------------
		

		// # ----------------------------------------------------------------------------------------------------
		// # Gen START------------------------------------------------------------------------------------------
		int ngen = gen_mass->size();

		if (!GoodNtrk(ngen)) continue;
		if (!IsKaonPair(gen_pdgId->at(0), gen_pdgId->at(1))) continue;

		Double_t posPt_gen		= gen_charge->at(0) > 0 ? gen_pT->at(0)  : gen_pT->at(1);
		Double_t posEta_gen		= gen_charge->at(0) > 0 ? gen_eta->at(0) : gen_eta->at(1);
		Double_t posPhi_gen		= gen_charge->at(0) > 0 ? gen_phi->at(0) : gen_phi->at(1);
		Double_t posMass_gen 	= gen_charge->at(0) > 0 ? gen_mass->at(0) : gen_mass->at(1);
		Double_t negPt_gen		= gen_charge->at(0) < 0 ? gen_pT->at(0)  : gen_pT->at(1);
		Double_t negEta_gen		= gen_charge->at(0) < 0 ? gen_eta->at(0) : gen_eta->at(1);
		Double_t negPhi_gen		= gen_charge->at(0) < 0 ? gen_phi->at(0) : gen_phi->at(1);
		Double_t negMass_gen 	= gen_charge->at(0) < 0 ? gen_mass->at(0) : gen_mass->at(1);

		cHM.hPhivsEtavsPt_Gen      ->Fill(posPt_gen, posEta_gen, posPhi_gen);
		cHM.hPhivsEtavsPt_Gen      ->Fill(negPt_gen, negEta_gen, negPhi_gen);

		cHM.hPosKaonPhivsEtavsPt_Gen ->Fill(posPt_gen, posEta_gen, posPhi_gen);
		cHM.hNegKaonPhivsEtavsPt_Gen ->Fill(negPt_gen, negEta_gen, negPhi_gen);

		cHM.hNegPtvsPosPt_Gen      ->Fill(posPt_gen,  negPt_gen);
		cHM.hNegEtavsPosEta_Gen    ->Fill(posEta_gen, negEta_gen);
		cHM.hNegPhivsPosPhi_Gen    ->Fill(posPhi_gen, negPhi_gen);

		TLorentzVector posFourMom_gen, negFourMom_gen, pairFourMom_gen;
		posFourMom_gen.SetPtEtaPhiM(posPt_gen, posEta_gen, posPhi_gen, posMass_gen);
		negFourMom_gen.SetPtEtaPhiM(negPt_gen, negEta_gen, negPhi_gen, negMass_gen);
		pairFourMom_gen = posFourMom_gen + negFourMom_gen;

		Double_t pt_gen   = pairFourMom_gen.Pt();
		Double_t eta_gen  = pairFourMom_gen.Eta();
		Double_t phi_gen  = pairFourMom_gen.Phi();
		Double_t mass_gen = pairFourMom_gen.M();
		Double_t y_gen    = pairFourMom_gen.Rapidity();

		cHM.hMvsPtvsRap_Gen			->Fill(y_gen, pt_gen,     mass_gen);
		cHM.hM_Gen->Fill(mass_gen);
		cHM.hPt_Gen->Fill(pt_gen);
		cHM.hRap_Gen->Fill(y_gen);
		// # Gen END--------------------------------------------------------------------------------------------

		
		// # Rec Gen Matching START-----------------------------------------------------------------------------
		int nrec = cand_mass->size();
		Double_t posMthDeltaR = 99999999.;
		Double_t negMthDeltaR = 99999999.;
		Int_t    posRecoIdx   = -1;
		Int_t    negRecoIdx   = -1;
		// if (nrec == 3)
		// {
			for (int irec = 0; irec < nrec; irec++)
			{
				if (!IsKaon(cand_pdgId->at(irec))) continue;
				Double_t Pt_temp  = cand_pT->at(irec);
				Double_t Eta_temp = cand_eta->at(irec);
				Double_t Phi_temp = cand_phi->at(irec);

				TVector3 Mom_rec; Mom_rec.SetPtEtaPhi(Pt_temp, Eta_temp, Phi_temp);
				TVector3 Mom_gen = posFourMom_gen.Vect();

				if ( RecGenMatched(Mom_rec, Mom_gen, posMthDeltaR) )
				{
					posMthDeltaR = Mom_rec.DeltaR(Mom_gen);
					posRecoIdx   = irec;
				}
				if (RecGenMatched(Mom_rec, Mom_gen, negMthDeltaR))
				{
					negMthDeltaR = Mom_rec.DeltaR(Mom_gen);
					negRecoIdx   = irec;
				}
			}
			if(posRecoIdx>=0 && negRecoIdx>=0 && posRecoIdx == negRecoIdx)
			{
				cout<<"One reco-track is matched to multiple gen-tracks !"<<endl;
			}

			if( posRecoIdx>=0 )
			{
				Double_t Pt_temp     = cand_pT->at(posRecoIdx);
				Double_t Eta_temp    = cand_eta->at(posRecoIdx);
				Double_t Phi_temp    = cand_phi->at(posRecoIdx);
				// Bool_t   isTrigKaon = trigMuon_mu()[trigIdx][posRecoIdx];

				cHM.hDeltaR  ->Fill( posMthDeltaR );
				cHM.hDeltaPt ->Fill( (Pt_temp-posPt_gen)/posPt_gen );

				cHM.hPtResvsGenPt   ->Fill( posPt_gen, (Pt_temp-posPt_gen)/posPt_gen );
				cHM.hEtaResvsGenEta ->Fill( posEta_gen, Eta_temp-posEta_gen          );
				cHM.hPhiResvsGenPhi ->Fill( posPhi_gen, Phi_temp-posPhi_gen          );

				cHM.hMthPosKaonPhivsEtavsPt_Gen ->Fill(posPt_gen, posEta_gen, posPhi_gen);
				cHM.hMthPosKaonPhivsEtavsPt     ->Fill(Pt_temp,      Eta_temp,      Phi_temp     );

				// if(isTrigKaon)
				// {
				// 	hTrigPosMuPhivsEtavsPt->Fill(muPt, muEta, Phi_temp);
				// }
			}

			if(negRecoIdx>=0)
			{
				Double_t Pt_temp     = cand_pT->at(negRecoIdx);
				Double_t Eta_temp    = cand_eta->at(negRecoIdx);
				Double_t Phi_temp    = cand_phi->at(negRecoIdx);
				// Bool_t   isTrigKaon = trigMuon_mu()[trigIdx][negRecoIdx];

				cHM.hDeltaR  ->Fill( negMthDeltaR );
				cHM.hDeltaPt ->Fill( (Pt_temp-negPt_gen)/negPt_gen );

				cHM.hPtResvsGenPt   ->Fill( negPt_gen, (Pt_temp-negPt_gen)/negPt_gen );
				cHM.hEtaResvsGenEta ->Fill( negEta_gen, Eta_temp-negEta_gen          );
				cHM.hPhiResvsGenPhi ->Fill( negPhi_gen, Phi_temp-negPhi_gen          );

				cHM.hMthNegKaonPhivsEtavsPt_Gen ->Fill(negPt_gen, negEta_gen, negPhi_gen);
				cHM.hMthNegKaonPhivsEtavsPt     ->Fill(Pt_temp,      Eta_temp,      Phi_temp     );

				// if(isTrigMu){
				// 	hTrigNegMuPhivsEtavsPt->Fill(muPt, muEta, muPhi);
				// }
			}


		// }

		// # Rec Gen Matching END-------------------------------------------------------------------------------


		// # ---------------------------------------------------------------------------------------------------
		// # Rec START------------------------------------------------------------------------------------------
		
		// ! throw away rec events with more than 3 tracks
		if (nrec > 3)	continue;

		Double_t posPt_rec;
		Double_t posEta_rec;
		Double_t posPhi_rec;
		Double_t posMass_rec;
		Double_t negPt_rec;
		Double_t negEta_rec;
		Double_t negPhi_rec;
		Double_t negMass_rec;

		bool Kaon_pair_filled = false;
		for (int irec = 0; irec < nrec; irec++)
		{
			// * catch kaon pair rec
			if (IsKaon(cand_pdgId->at(irec)))
			{
				if (cand_charge->at(irec) > 0)
				{
					posPt_rec = cand_pT->at(irec);
					posEta_rec = cand_eta->at(irec);
					posPhi_rec = cand_phi->at(irec);
					posMass_rec = cand_mass->at(irec);
				}
				else
				{
					negPt_rec = cand_pT->at(irec);
					negEta_rec = cand_eta->at(irec);
					negPhi_rec = cand_phi->at(irec);
					negMass_rec = cand_mass->at(irec);
				}
			}

			if (posMass_rec && negMass_rec && !Kaon_pair_filled)
			{
				TLorentzVector posFourMom_rec, negFourMom_rec, pairFourMom_rec;
				posFourMom_rec.SetPtEtaPhiM(posPt_rec, posEta_rec, posPhi_rec, posMass_rec);
				negFourMom_rec.SetPtEtaPhiM(negPt_rec, negEta_rec, negPhi_rec, negMass_rec);
				pairFourMom_rec = posFourMom_rec + negFourMom_rec;

				Double_t pt_rec   = pairFourMom_rec.Pt();
				Double_t eta_rec  = pairFourMom_rec.Eta();
				Double_t phi_rec  = pairFourMom_rec.Phi();
				Double_t mass_rec = pairFourMom_rec.M();
				Double_t y_rec    = pairFourMom_rec.Rapidity();

				cHM.hM_InPair->Fill(mass_rec);
				cHM.hPt_InPair->Fill(pt_rec);
				cHM.hRap_InPair->Fill(y_rec);

				Kaon_pair_filled = true;
			}


			// * catch phi rec
			if (!IsPhi(cand_pdgId->at(irec))) continue;

			Double_t pt_rec   = cand_pT->at(irec);
			Double_t eta_rec  =	cand_eta->at(irec);
			Double_t phi_rec  =	cand_phi->at(irec);
			Double_t mass_rec =	cand_mass->at(irec);
			Double_t y_rec    =	cand_y->at(irec);

			// * Collecting Selection 
			// if (!passEvtSel) continue;
			cHM.hMvsPtvsRap->Fill(y_rec, pt_rec, mass_rec);
			cHM.hM->Fill(mass_rec);
			cHM.hPt->Fill(pt_rec);
			cHM.hRap->Fill(y_rec);
		}
		// # Rec END--------------------------------------------------------------------------------------------
   	
	
		
	}

	cHM.WriteHists();
	gStyle->SetOptStat();
	auto c = new TCanvas("c", "c", 800, 600);
	cHM.hRap->Draw("L");
	c->SaveAs("outFigures/rap.png");
	cHM.hRap_Gen->Draw();
	c->SaveAs("outFigures/rap_gen.png");
	cHM.hM ->Draw();
	c->SaveAs("outFigures/mass.png");
	cHM.hM_Gen ->Draw();
	c->SaveAs("outFigures/mass_gen.png");
	cHM.hPt ->Draw();
	c->SaveAs("outFigures/pt.png");
	cHM.hPt_Gen ->Draw();
	c->SaveAs("outFigures/pt_gen.png");
	cHM.hnEvts->Draw();
	c->SaveAs("outFigures/nEvts.png");


	// std::string inputFile;

	// const auto& csTreeDir = "dimucontana_mc";

	// for (Long64_t jentry = 1; jentry < csTree.GetEntries(); jentry++)
	// {
	// 	if (jentry % (csTree.GetEntries() / 10) == 0)
	// 		cout << "begin " << jentry << "th entry...." << endl;

	// 	// Get the entry
	// 	if (csTree.GetEntry(jentry) < 0)
	// 	{
	// 		cout << "Invalid correct-sign entry!" << endl;
	// 		return;
	// 	}

	// 	Int_t   cen          = csTree.centrality();
	// 	Float_t vtxX         = csTree.bestvtxX();
	// 	Float_t vtxY         = csTree.bestvtxY();
	// 	Float_t vtxZ         = csTree.bestvtxZ();
	// 	Float_t hfsumETPlus  = csTree.HFsumETPlus();
	// 	Float_t hfsumETMinus = csTree.HFsumETMinus();
	// 	Int_t   nTrkoffline  = csTree.Ntrkoffline();
	// 	Int_t   nTrkHP       = csTree.NtrkHP();

	// 	hnEvts               ->Fill(0.5);
	// 	hVzvsVyvsVx          ->Fill(vtxX, vtxY, vtxZ);
	// 	hHFMinusvsHFPlus     ->Fill(hfsumETPlus, hfsumETMinus);
	// 	hNCanvsNtrkvsCen     ->Fill(cen, nTrkoffline, csTree.candSize());
	// 	hNtrkofflinevsNtrkHP ->Fill(nTrkHP, nTrkoffline);

	// 	// Select Event - require this event has a valid vertex and is not beam-halo event
	// 	//if(!csTree.evtSel()[2] || !csTree.evtSel()[3]) continue;
	// 	Bool_t goodVtx = (csTree.evtSel()[2] && csTree.evtSel()[3]);
	// 	if(goodVtx) hnEvts->Fill(1.5);

	// 	//evtSel[4-15]
	// 	//[4]=0: HFPlusMaxTower < 3 GeV;  [4]=1: HFPlusMaxTower > 3 GeV
	// 	//[5]=0: HFMinusMaxTower < 3 GeV;  [5]=1: HFMinusMaxTower > 3 GeV
	// 	//[6] is for Plus & [7] is for Minus; Threshold = 4 GeV
	// 	//[8] is for Plus & [9] is for Minus; Threshold = 5 GeV
	// 	//[10] is for Plus & [11] is for Minus; Threshold = 6 GeV
	// 	//[12] is for Plus & [13] is for Minus; Threshold = 7 GeV
	// 	//[14] is for Plus & [15] is for Minus; Threshold = 8 GeV
	// 	//[16] is for Plus (Th = 7.6 GeV) & [17] is for Minus (Th = 7.3 GeV);

	// 	//if(csTree.evtSel()[16] || csTree.evtSel()[17]) continue;
	// 	Bool_t goodHFVeto = (!csTree.evtSel()[16] && !csTree.evtSel()[17]);
	// 	if(goodVtx & goodHFVeto) hnEvts->Fill(2.5);

	// 	//if(nTrkHP != 2) continue;

	// 	Bool_t passEvtSel = goodVtx && goodHFVeto && (nTrkHP==2);

	// 	if(passEvtSel) hnEvts->Fill(3.5);

	// 	// Loop over the correct-sign candidates
	// 	Int_t nSoftMuon = 0;
	// 	for (UInt_t icand = 0; icand < csTree.candSize_gen(); icand++)
	// 	{
	// 		Double_t posPt_gen  = csTree.chargeD1_gen()[icand] > 0 ? csTree.pTD1_gen()[icand]  : csTree.pTD2_gen()[icand];
	// 		Double_t posEta_gen = csTree.chargeD1_gen()[icand] > 0 ? csTree.EtaD1_gen()[icand] : csTree.EtaD2_gen()[icand];
	// 		Double_t posPhi_gen = csTree.chargeD1_gen()[icand] > 0 ? csTree.PhiD1_gen()[icand] : csTree.PhiD2_gen()[icand];
	// 		Double_t negPt_gen  = csTree.chargeD1_gen()[icand] < 0 ? csTree.pTD1_gen()[icand]  : csTree.pTD2_gen()[icand];
	// 		Double_t negEta_gen = csTree.chargeD1_gen()[icand] < 0 ? csTree.EtaD1_gen()[icand] : csTree.EtaD2_gen()[icand];
	// 		Double_t negPhi_gen = csTree.chargeD1_gen()[icand] < 0 ? csTree.PhiD1_gen()[icand] : csTree.PhiD2_gen()[icand];

	// 		hPhivsEtavsPt_Gen      ->Fill(posPt_gen, posEta_gen, posPhi_gen);
	// 		hPhivsEtavsPt_Gen      ->Fill(negPt_gen, negEta_gen, negPhi_gen);

	// 		hPosMuPhivsEtavsPt_Gen ->Fill(posPt_gen, posEta_gen, posPhi_gen);
	// 		hNegMuPhivsEtavsPt_Gen ->Fill(negPt_gen, negEta_gen, negPhi_gen);

	// 		hNegPtvsPosPt_Gen      ->Fill(posPt_gen,  negPt_gen);
	// 		hNegEtavsPosEta_Gen    ->Fill(posEta_gen, negEta_gen);
	// 		hNegPhivsPosPhi_Gen    ->Fill(posPhi_gen, negPhi_gen);

	// 		TLorentzVector posFourMom_gen, negFourMom_gen, pairFourMom_gen;
	// 		posFourMom_gen.SetPtEtaPhiM(posPt_gen, posEta_gen, posPhi_gen, Mmuon);
	// 		negFourMom_gen.SetPtEtaPhiM(negPt_gen, negEta_gen, negPhi_gen, Mmuon);
	// 		pairFourMom_gen = posFourMom_gen + negFourMom_gen;

	// 		Double_t pt_gen   = pairFourMom_gen.Pt();
	// 		Double_t eta_gen  = pairFourMom_gen.Eta();
	// 		Double_t phi_gen  = pairFourMom_gen.Phi();
	// 		Double_t mass_gen = pairFourMom_gen.M();
	// 		Double_t y_gen    = pairFourMom_gen.Rapidity();

	// 		if(y_gen>1.5)
	// 		{
	// 			hForward_PhivsEtavsPt_Gen->Fill(posPt_gen, posEta_gen, posPhi_gen);
	// 			hForward_PhivsEtavsPt_Gen->Fill(negPt_gen, negEta_gen, negPhi_gen);
	// 		}

	// 		Double_t asyPhi_gen = 1 - TMath::Abs(shiftDeltaPhi(posFourMom_gen.DeltaPhi(negFourMom_gen))) / PI; //acoplanarity

	// 		TVector3 muMomDiff_gen = posFourMom_gen.Vect() - negFourMom_gen.Vect();
	// 		TVector3 pairMom_gen   = pairFourMom_gen.Vect();
	// 		Double_t phiDiff_gen   = shiftDeltaPhi(pairMom_gen.DeltaPhi(muMomDiff_gen));

	// 		hMvsPtvsRap_Gen        ->Fill(y_gen, pt_gen,     mass_gen);
	// 		hMvsAsyPhivsRap_Gen    ->Fill(y_gen, asyPhi_gen, mass_gen);
	// 		hDeltaPhivsM_Gen       ->Fill(mass_gen, phiDiff_gen);

	// 		Double_t posMthDeltaR = 99999999.;
	// 		Double_t negMthDeltaR = 99999999.;
	// 		Int_t    posRecoIdx   = -1;
	// 		Int_t    negRecoIdx   = -1;

	// 		for(UInt_t imu=0; imu<csTree.candSize_mu(); imu++)
	// 		{
	// 			if( !csTree.softMuon_mu()[imu] ) continue;

	// 			if( icand==0 ) nSoftMuon++;

	// 			Double_t muPt  = csTree.pT_mu()[imu];
	// 			Double_t muEta = csTree.eta_mu()[imu];
	// 			Double_t muPhi = csTree.phi_mu()[imu];

	// 			TVector3 recoMom; recoMom.SetPtEtaPhi(muPt, muEta, muPhi);
	// 			Double_t posDeltaR = posFourMom_gen.Vect().DeltaR(recoMom);
	// 			Double_t negDeltaR = negFourMom_gen.Vect().DeltaR(recoMom);

	// 			// DeltaPt/genPt < 0.15 ensures to keep 99.9842% events
	// 			// DeltaR < 0.05 ensures to keep 99.9763% events
	// 			if(
	// 					TMath::Abs((muPt-posPt_gen)/posPt_gen) < 0.15
	// 					&& posDeltaR < 0.05
	// 					&& posDeltaR < posMthDeltaR
	// 			  )
	// 			{
	// 				posMthDeltaR = posDeltaR;
	// 				posRecoIdx = imu;
	// 			}

	// 			if(
	// 					TMath::Abs((muPt-negPt_gen)/negPt_gen) < 0.15
	// 					&& negDeltaR < 0.05
	// 					&& negDeltaR < negMthDeltaR
	// 			  )
	// 			{
	// 				negMthDeltaR = negDeltaR;
	// 				negRecoIdx = imu;
	// 			}
	// 		}

	// 		if(posRecoIdx>=0 && negRecoIdx>=0 && posRecoIdx == negRecoIdx)
	// 		{
	// 			cout<<"One reco-track is matched to multiple gen-tracks !"<<endl;
	// 		}

	// 		if( posRecoIdx>=0 )
	// 		{
	// 			Double_t muPt     = csTree.pT_mu()[posRecoIdx];
	// 			Double_t muEta    = csTree.eta_mu()[posRecoIdx];
	// 			Double_t muPhi    = csTree.phi_mu()[posRecoIdx];
	// 			Bool_t   isTrigMu = csTree.trigMuon_mu()[trigIdx][posRecoIdx];

	// 			hDeltaR  ->Fill( posMthDeltaR );
	// 			hDeltaPt ->Fill( (muPt-posPt_gen)/posPt_gen );

	// 			hPtResvsGenPt   ->Fill( posPt_gen, (muPt-posPt_gen)/posPt_gen );
	// 			hEtaResvsGenEta ->Fill( posEta_gen, muEta-posEta_gen          );
	// 			hPhiResvsGenPhi ->Fill( posPhi_gen, muPhi-posPhi_gen          );

	// 			hMthPosMuPhivsEtavsPt_Gen ->Fill(posPt_gen, posEta_gen, posPhi_gen);
	// 			hMthPosMuPhivsEtavsPt     ->Fill(muPt,      muEta,      muPhi     );

	// 			if(isTrigMu)
	// 			{
	// 				hTrigPosMuPhivsEtavsPt->Fill(muPt, muEta, muPhi);
	// 			}
	// 		}

	// 		if(negRecoIdx>=0)
	// 		{
	// 			Double_t muPt     = csTree.pT_mu()[negRecoIdx];
	// 			Double_t muEta    = csTree.eta_mu()[negRecoIdx];
	// 			Double_t muPhi    = csTree.phi_mu()[negRecoIdx];
	// 			Bool_t   isTrigMu = csTree.trigMuon_mu()[trigIdx][negRecoIdx];

	// 			hDeltaR   ->Fill( negMthDeltaR );
	// 			hDeltaPt  ->Fill( (muPt-negPt_gen)/negPt_gen );

	// 			hPtResvsGenPt->Fill(negPt_gen, (muPt-negPt_gen)/negPt_gen);
	// 			hEtaResvsGenEta->Fill(negEta_gen, muEta-negEta_gen);
	// 			hPhiResvsGenPhi->Fill(negPhi_gen, muPhi-negPhi_gen);

	// 			hMthNegMuPhivsEtavsPt_Gen->Fill(negPt_gen, negEta_gen, negPhi_gen);
	// 			hMthNegMuPhivsEtavsPt->Fill(muPt, muEta, muPhi);

	// 			if(isTrigMu){
	// 				hTrigNegMuPhivsEtavsPt->Fill(muPt, muEta, muPhi);
	// 			}
	// 		}

	// 		Int_t recoIdx = csTree.RecIdx_gen()[icand];

	// 		if(recoIdx<0 || csTree.candSize()<=0) continue;

	// 		Double_t posPt     = csTree.chargeD1()[recoIdx] > 0 ? csTree.pTD1()[recoIdx] : csTree.pTD2()[recoIdx];
	// 		Double_t posEta    = csTree.chargeD1()[recoIdx] > 0 ? csTree.EtaD1()[recoIdx] : csTree.EtaD2()[recoIdx];
	// 		Double_t posPhi    = csTree.chargeD1()[recoIdx] > 0 ? csTree.PhiD1()[recoIdx] : csTree.PhiD2()[recoIdx];
	// 		Double_t isPosTrig = csTree.chargeD1()[recoIdx] > 0 ? csTree.trigMuon1()[trigIdx][recoIdx] : csTree.trigMuon2()[trigIdx][recoIdx];
	// 		Double_t negPt     = csTree.chargeD1()[recoIdx] < 0 ? csTree.pTD1()[recoIdx] : csTree.pTD2()[recoIdx];
	// 		Double_t negEta    = csTree.chargeD1()[recoIdx] < 0 ? csTree.EtaD1()[recoIdx] : csTree.EtaD2()[recoIdx];
	// 		Double_t negPhi    = csTree.chargeD1()[recoIdx] < 0 ? csTree.PhiD1()[recoIdx] : csTree.PhiD2()[recoIdx];
	// 		Double_t isNegTrig = csTree.chargeD1()[recoIdx] < 0 ? csTree.trigMuon1()[trigIdx][recoIdx] : csTree.trigMuon2()[trigIdx][recoIdx];

	// 		if( !csTree.softCand(recoIdx) ) continue;

	// 		// shift mean
	// 		Double_t shiftPosPt = posPt - funPtMeanShift->Eval(posPt_gen)*posPt_gen;
	// 		Double_t shiftNegPt = negPt - funPtMeanShift->Eval(negPt_gen)*negPt_gen;

	// 		// smear width
	// 		Double_t posPtNew = posPt_gen + (shiftPosPt - posPt_gen)/funRawPtRes->Eval(posPt_gen)*funTunedPtRes->Eval(posPt_gen);
	// 		Double_t negPtNew = negPt_gen + (shiftNegPt - negPt_gen)/funRawPtRes->Eval(negPt_gen)*funTunedPtRes->Eval(negPt_gen);

	// 		hMthPosMuPhivsEtavsPtInpair->Fill(posPtNew, posEta, posPhi);
	// 		if(isPosTrig) hTrigPosMuPhivsEtavsPtInpair->Fill(posPtNew, posEta, posPhi);

	// 		hMthNegMuPhivsEtavsPtInpair->Fill(negPtNew, negEta, negPhi);
	// 		if(isNegTrig) hTrigNegMuPhivsEtavsPtInpair->Fill(negPtNew, negEta, negPhi);

	// 		TLorentzVector posFourMom, negFourMom, pairFourMom;
	// 		posFourMom.SetPtEtaPhiM(posPtNew, posEta, posPhi, Mmuon);
	// 		negFourMom.SetPtEtaPhiM(negPtNew, negEta, negPhi, Mmuon);
	// 		pairFourMom = posFourMom + negFourMom;
	// 		//cout<<"posPhi: "<<posPhi<<"   negPhi: "<<negPhi<<"    phiAngel:"<<posFourMom.DeltaPhi(negFourMom)<<endl;

	// 		// without pt smear
	// 		Double_t pt_woSmear   = csTree.pT()[recoIdx];
	// 		Double_t eta_woSmear  = csTree.eta()[recoIdx];
	// 		Double_t phi_woSmear  = csTree.phi()[recoIdx];
	// 		Double_t mass_woSmear = csTree.mass()[recoIdx];
	// 		Double_t y_woSmear    = csTree.y()[recoIdx];

	// 		// with pt smear
	// 		Double_t pt   = pairFourMom.Pt();
	// 		Double_t eta  = pairFourMom.Eta();
	// 		Double_t phi  = pairFourMom.Phi();
	// 		Double_t mass = pairFourMom.M();
	// 		Double_t y    = pairFourMom.Rapidity();
	// 		//cout<<"pt: "<<pt<<"   eta: "<<eta<<"   phi: "<<phi<<"   M: "<<mass<<"   y: "<<y<<endl;

	// 		hRcPairPtvsGenPairPt     ->Fill(pt_gen,  pt );
	// 		hRcPairEtavsGenPairEta   ->Fill(phi_gen, phi);
	// 		hRcPairPhivsGenPairPhi   ->Fill(eta_gen, eta);
	// 		hMassResvsGenMass        ->Fill(mass_gen, (mass-mass_gen)/mass_gen);
	// 		hPairPtResvsGenPairPt    ->Fill(pt_gen,  pt - pt_gen);
	// 		hRapResvsGenRap          ->Fill(y_gen,   y-y_gen    );

	// 		Double_t asyPhi = 1 - TMath::Abs(shiftDeltaPhi(posFourMom.DeltaPhi(negFourMom))) / PI; //acoplanarity

	// 		// already required soft muon pair before

	// 		//----------------------------------------------------------------------------------------
	// 		// without pt smear
	// 		if(goodMuPair(csTree, recoIdx))
	// 		{
	// 			hMvsPtvsRap_woEvtSel_woSmear->Fill(y_woSmear, pt_woSmear, mass_woSmear);

	// 			if( passEvtSel )
	// 			{
	// 				hMvsPtvsRap_woSmear->Fill(y_woSmear, pt_woSmear, mass_woSmear);
	// 			}
	// 		}
	// 		//----------------------------------------------------------------------------------------

	// 		//----------------------------------------------------------------------------------------
	// 		// with pt smear
	// 		if( !goodMuPair(posFourMom, isPosTrig, negFourMom, isNegTrig) ) continue;
	// 		hMvsPtvsRap_woEvtSel->Fill(y, pt, mass);
	// 		//----------------------------------------------------------------------------------------

	// 		if(!passEvtSel) continue;

	// 		hMvsPtvsRap        ->Fill(y, pt,     mass);
	// 		hMvsAsyPhivsRap    ->Fill(y, asyPhi, mass);

	// 		TVector3 muMomDiff = posFourMom.Vect() - negFourMom.Vect();
	// 		TVector3 pairMom   = pairFourMom.Vect();

	// 		Double_t phiDiff   = shiftDeltaPhi(pairMom.DeltaPhi(muMomDiff));
	// 		hDeltaPhivsM       ->Fill(mass, phiDiff);
	// 	}

	// 	hNRcMuvsNGenMu->Fill( csTree.candSize_gen()*2, nSoftMuon);
	// }

	// system("mkdir -p mcHistos");
	// writeHistos(Form("mcHistos/dimuonHistos.%s", fileName.Data()));
}