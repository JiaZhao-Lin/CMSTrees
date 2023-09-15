#include "./VCTree_utilities.C"


//== Main =========================================================================================
void VCTree_TrigEff()
{
	TH1::SetDefaultSumw2(kTRUE);
	// std::string inputFileDir = "inFiles/2023/VCTree_STARLIGHT_CohJpsiToMuMu_5p36TeV_130X_1M.root";
	std::string inputFileDir = "inFiles/2023/VCTree_STARLIGHT_CohJpsiToMuMu_5p36TeV_132X_1M.root";
	// std::string inputFileDir = "inFiles/2018/VertexCompositeTree_STARLIGHT_CohJpsiToMuMu_GenFilter_DiMuMC_20200906.root";
	// std::string inputFileDir = "inFiles/2018/VertexCompositeTree_STARLIGHT_GGToMuMu_woPtCut_DiMuMC_20191122.root";

	const auto& csTreeDir = "dimucontana_mc";

	// Extract the tree
	VertexCompositeTree csTree;
	if (!csTree.GetTree(inputFileDir, csTreeDir)) 
	{
		cout << "Invalid Correct-Sign tree!" << endl;
		return;
	}

	init_smear_input();

	bookHistos();

	int multiple_matched_tracks = 0;
	int total_tracks = 0;
	int total_entries = csTree.GetEntries();
	int max_entries = 100000;
	cout << "max_entries = " << total_entries << endl;
	for (Long64_t jentry = 0; jentry < total_entries; jentry++) 
	{
		if (jentry > max_entries) break;

		update_progress(jentry, max_entries, 10);

		// Get the entry
		if (csTree.GetEntry(jentry) < 0) 
		{
			cout << "Invalid correct-sign entry!" << endl;
			return;
		}

		hnEvts->Fill(0.5);

		//= Event Level Selections ============================================================
		// Select Event - require this event has a valid vertex and is not beam-halo event
		//[2] = Flag_primaryVertexFilter: events have valid vertex
		//[3] = Flag_clusterCompatibilityFilter: the shapes of the clusters in the pixel detector are required to be compatible with the shapes expected from a heavy ion collision
		Bool_t goodVtx				= (csTree.evtSel()[2] && csTree.evtSel()[3]);

		//evtSel[4-15]
		//[4]=0: HFPlusMaxTower < 3 GeV;  [4]=1: HFPlusMaxTower > 3 GeV
		//[5]=0: HFMinusMaxTower < 3 GeV;  [5]=1: HFMinusMaxTower > 3 GeV
		//[6] is for Plus & [7] is for Minus; Threshold = 4 GeV
		//[8] is for Plus & [9] is for Minus; Threshold = 5 GeV
		//[10] is for Plus & [11] is for Minus; Threshold = 6 GeV
		//[12] is for Plus & [13] is for Minus; Threshold = 7 GeV
		//[14] is for Plus & [15] is for Minus; Threshold = 8 GeV
		//[16] is for Plus (Th = 7.6 GeV) & [17] is for Minus (Th = 7.3 GeV);
		Bool_t goodHFVeto 			= (!csTree.evtSel()[16] && !csTree.evtSel()[17]);
		Bool_t passNTrkHP 			= csTree.NtrkHP() == 2;	// contain only two high-purity tracks and nothing else
		Bool_t passEvtSel 			= goodVtx && goodHFVeto && passNTrkHP;
		Bool_t passNRecoCandidate 	= csTree.candSize() > 0;

		if (goodVtx)				hnEvts->Fill(1.5);
		if (goodVtx && goodHFVeto)	hnEvts->Fill(2.5);
		if (passEvtSel) 			hnEvts->Fill(3.5);
		if (passNRecoCandidate)		hnEvts->Fill(4.5);

		//* fill Ntrk distributions
		hNtrkofflinevsNtrkHP->Fill(csTree.Ntrkoffline(), csTree.NtrkHP());
		hNpixelvsNpixelTracks->Fill(csTree.Npixel(), csTree.NpixelTracks());

		//= Fill Dimuon Reco Eff Hist ===========================================================================
		for (UInt_t icand_gen = 0; icand_gen < csTree.candSize_gen(); icand_gen++)
		{
			//* Get the four-momentum of the pair
			//* Gen level pt_gen and ptD1_gen + ptD2_gen are the same, but there is no phi_gen branch
			const std::vector<TLorentzVector> fourMom_gen = GetFourMom(csTree.chargeD1_gen()[icand_gen], csTree.chargeD2_gen()[icand_gen], csTree.pTD1_gen()[icand_gen], csTree.pTD2_gen()[icand_gen], csTree.EtaD1_gen()[icand_gen], csTree.EtaD2_gen()[icand_gen], csTree.PhiD1_gen()[icand_gen], csTree.PhiD2_gen()[icand_gen]);
			TLorentzVector pairFourMom_gen = fourMom_gen[2];

			Double_t pt_gen		= pairFourMom_gen.Pt();
			Double_t eta_gen	= pairFourMom_gen.Eta();
			Double_t phi_gen	= pairFourMom_gen.Phi();
			Double_t mass_gen	= pairFourMom_gen.M();
			Double_t rap_gen	= pairFourMom_gen.Rapidity();

			//* fill denominator of Paired Muon reconstruction eff
			hPtvsEtavsPhi_gen	->Fill(pt_gen,	eta_gen,	phi_gen);
			hPtvsRapvsPhi_gen	->Fill(pt_gen,	rap_gen,	phi_gen);

			//* fill numerator of Paired Muon reconstruction eff
			Short_t recoIdx = csTree.RecIdx_gen()[icand_gen]; // recoIdx = -1 if not reconstructed

			if (csTree.candSize() > 0)
			{
				hPtvsEtavsPhi_gen_reconstructed	->Fill(pt_gen,	eta_gen,	phi_gen);
				hPtvsRapvsPhi_gen_reconstructed	->Fill(pt_gen,	rap_gen,	phi_gen);
			}
		}

		//= Fill Single Muon Reconstruction and L1Trigger Eff Hist ===================================================
		for (UInt_t icand_gen = 0; icand_gen < csTree.candSize_gen(); icand_gen++)
		{
			//* Get the four-momentum of the pair
			const std::vector<TLorentzVector> fourMom_gen = GetFourMom(csTree.chargeD1_gen()[icand_gen], csTree.chargeD2_gen()[icand_gen], csTree.pTD1_gen()[icand_gen], csTree.pTD2_gen()[icand_gen], csTree.EtaD1_gen()[icand_gen], csTree.EtaD2_gen()[icand_gen], csTree.PhiD1_gen()[icand_gen], csTree.PhiD2_gen()[icand_gen]);

			TVector3 Mom_gen_mu_pos 		= fourMom_gen[0].Vect();
			TVector3 Mom_gen_mu_neg 		= fourMom_gen[1].Vect();
			TLorentzVector pairFourMom_gen	= fourMom_gen[2];

			Double_t pt_gen_mu_pos		= fourMom_gen[0].Pt();
			Double_t eta_gen_mu_pos		= fourMom_gen[0].Eta();
			Double_t phi_gen_mu_pos		= fourMom_gen[0].Phi();
			Double_t mass_gen_mu_pos	= fourMom_gen[0].M();
			Double_t rap_gen_mu_pos		= fourMom_gen[0].Rapidity();

			Double_t pt_gen_mu_neg		= fourMom_gen[1].Pt();
			Double_t eta_gen_mu_neg		= fourMom_gen[1].Eta();
			Double_t phi_gen_mu_neg		= fourMom_gen[1].Phi();
			Double_t mass_gen_mu_neg	= fourMom_gen[1].M();
			Double_t rap_gen_mu_neg		= fourMom_gen[1].Rapidity();

			//* fill denominator of single muon reconstruction eff
			hPtvsEtavsPhi_gen_mu_pos	->Fill(pt_gen_mu_pos,	eta_gen_mu_pos,	phi_gen_mu_pos);
			hPtvsEtavsPhi_gen_mu_neg	->Fill(pt_gen_mu_neg,	eta_gen_mu_neg,	phi_gen_mu_neg);

			total_tracks += 2;

			//= Using muon from the reco single muon branch =================================================
			Double_t BestDeltaR_mu_pos = 99999999.;
			Double_t BestDeltaR_mu_neg = 99999999.;
			Int_t    RecoIdx_mu_pos   = -1;
			Int_t    RecoIdx_mu_neg   = -1;

			for (UInt_t icand_mu = 0; icand_mu < csTree.candSize_mu(); icand_mu++)
			{
				//! Muon Level Selections !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				if (!csTree.softMuon_mu()[icand_mu]) continue;
				//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

				TVector3 Mom_mu; Mom_mu.SetPtEtaPhi(csTree.pT_mu()[icand_mu], csTree.eta_mu()[icand_mu], csTree.phi_mu()[icand_mu]);

				//* Check if the current reconstructed muon (pos or neg) is matched to a generated one
				if ( RecGenMatched(Mom_mu, Mom_gen_mu_pos, BestDeltaR_mu_pos) )
				{
					BestDeltaR_mu_pos = Mom_gen_mu_pos.DeltaR(Mom_mu);
					RecoIdx_mu_pos = icand_mu;
				}
				if ( RecGenMatched(Mom_mu, Mom_gen_mu_neg, BestDeltaR_mu_neg) )
				{
					BestDeltaR_mu_neg = Mom_gen_mu_neg.DeltaR(Mom_mu);
					RecoIdx_mu_neg = icand_mu;
				}
			}

			//* warning if one reconstructed muon is matched to multiple generated muons
			if(RecoIdx_mu_pos>=0 && RecoIdx_mu_neg>=0 && RecoIdx_mu_pos == RecoIdx_mu_neg)
			{
				multiple_matched_tracks++;
				// cout<<"One reco-track is matched to multiple gen-tracks !"<<endl;
			}

			if (RecoIdx_mu_pos >= 0)
			{
				//* get the matched muon
				Double_t pt_mu_pos	= csTree.pT_mu()[RecoIdx_mu_pos];
				Double_t eta_mu_pos	= csTree.eta_mu()[RecoIdx_mu_pos];
				Double_t phi_mu_pos	= csTree.phi_mu()[RecoIdx_mu_pos];

				//* fill numerator of single muon reconstruction eff
				hPtvsEtavsPhi_gen_mu_pos_reconstructed	->Fill(pt_gen_mu_pos,	eta_gen_mu_pos,	phi_gen_mu_pos);
				//* fill denominator of single muon L1 trigger eff
				hPtvsEtavsPhi_mu_pos					->Fill(pt_mu_pos,	eta_mu_pos,	phi_mu_pos);

				//* fill numerator of L1 trigger eff
				if (bool( csTree.trigMuon_mu()[L1_TrigIdx][RecoIdx_mu_pos]) )	
				{
					hPtvsEtavsPhi_mu_pos_trigL1->Fill(pt_mu_pos, eta_mu_pos, phi_mu_pos);
				}
			}
			if (RecoIdx_mu_neg >= 0)
			{
				//* get the matched muon
				Double_t pt_mu_neg	= csTree.pT_mu()[RecoIdx_mu_neg];
				Double_t eta_mu_neg	= csTree.eta_mu()[RecoIdx_mu_neg];
				Double_t phi_mu_neg	= csTree.phi_mu()[RecoIdx_mu_neg];

				//* fill numerator of single muon reconstruction eff
				hPtvsEtavsPhi_gen_mu_neg_reconstructed	->Fill(pt_gen_mu_neg,	eta_gen_mu_neg,	phi_gen_mu_neg);
				//* fill denominator of single muon L1 trigger eff
				hPtvsEtavsPhi_mu_neg		->Fill(pt_mu_neg,	eta_mu_neg,	phi_mu_neg);

				//* fill numerator of L1 trigger eff
				if (bool(csTree.trigMuon_mu()[L1_TrigIdx][RecoIdx_mu_neg]))
				{
					hPtvsEtavsPhi_mu_neg_trigL1->Fill(pt_mu_neg, eta_mu_neg, phi_mu_neg);
				}
			}
			//=============================================================================================

			// //= Using muon from the paired muon branch =================================================
			Int_t recoIdx = csTree.RecIdx_gen()[icand_gen];
			if ( recoIdx<0 || csTree.candSize() <= 0 ) continue;

			//! Muon Level Selections !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			if( !csTree.softCand(recoIdx) ) continue;
			//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			//* fill numerator of paired single muon reconstruction eff
			hPtvsEtavsPhi_gen_mu_pos_reconstructed_pair	->Fill(pt_gen_mu_pos,	eta_gen_mu_pos,	phi_gen_mu_pos);
			hPtvsEtavsPhi_gen_mu_neg_reconstructed_pair	->Fill(pt_gen_mu_neg,	eta_gen_mu_neg,	phi_gen_mu_neg);

			//* if you do smearing, you need to use the individual muon's kinematics
			//* However if you don't do smearing, the combined kinematics already saved in the tree is good enough
			
			// Without smearing
			//* get the four-momentum of the pair
			const std::vector<TLorentzVector> fourMom_woSmear = GetFourMom(csTree.chargeD1()[recoIdx], csTree.chargeD2()[recoIdx], csTree.pTD1()[recoIdx], csTree.pTD2()[recoIdx], csTree.EtaD1()[recoIdx], csTree.EtaD2()[recoIdx], csTree.PhiD1()[recoIdx], csTree.PhiD2()[recoIdx]);
			TLorentzVector FourMom_mu_pos_woSmear	= fourMom_woSmear[0];
			TLorentzVector FourMom_mu_neg_woSmear	= fourMom_woSmear[1];
			TLorentzVector pairFourMom_woSmear		= fourMom_woSmear[2];

			//* fill denominator of paired single muon L1 trigger eff
			hPtvsEtavsPhi_mu_pos_pair ->Fill(FourMom_mu_pos_woSmear.Pt(), FourMom_mu_pos_woSmear.Eta(), FourMom_mu_pos_woSmear.Phi());
			hPtvsEtavsPhi_mu_neg_pair ->Fill(FourMom_mu_neg_woSmear.Pt(), FourMom_mu_neg_woSmear.Eta(), FourMom_mu_neg_woSmear.Phi());

			//* fill numerator of paired single muon L1 trigger eff
			Double_t isPosTrig 		= csTree.chargeD1()[recoIdx] > 0 ? csTree.trigMuon1()[L1_TrigIdx][recoIdx] : csTree.trigMuon2()[L1_TrigIdx][recoIdx];
			Double_t isNegTrig		= csTree.chargeD2()[recoIdx] > 0 ? csTree.trigMuon1()[L1_TrigIdx][recoIdx] : csTree.trigMuon2()[L1_TrigIdx][recoIdx];
			if (bool(isPosTrig))	hPtvsEtavsPhi_mu_pos_pair_trigL1->Fill(FourMom_mu_pos_woSmear.Pt(), FourMom_mu_pos_woSmear.Eta(), FourMom_mu_pos_woSmear.Phi());
			if (bool(isNegTrig))	hPtvsEtavsPhi_mu_neg_pair_trigL1->Fill(FourMom_mu_neg_woSmear.Pt(), FourMom_mu_neg_woSmear.Eta(), FourMom_mu_neg_woSmear.Phi());


			// With smearing
			Double_t pt_mu_pos 		= csTree.chargeD1()[recoIdx] > 0 ? csTree.pTD1()[recoIdx] : csTree.pTD2()[recoIdx];
			Double_t eta_mu_pos 	= csTree.chargeD1()[recoIdx] > 0 ? csTree.EtaD1()[recoIdx] : csTree.EtaD2()[recoIdx];
			Double_t phi_mu_pos 	= csTree.chargeD1()[recoIdx] > 0 ? csTree.PhiD1()[recoIdx] : csTree.PhiD2()[recoIdx];
			Double_t pt_mu_neg 		= csTree.chargeD1()[recoIdx] < 0 ? csTree.pTD1()[recoIdx] : csTree.pTD2()[recoIdx];
			Double_t eta_mu_neg 	= csTree.chargeD1()[recoIdx] < 0 ? csTree.EtaD1()[recoIdx] : csTree.EtaD2()[recoIdx];
			Double_t phi_mu_neg 	= csTree.chargeD1()[recoIdx] < 0 ? csTree.PhiD1()[recoIdx] : csTree.PhiD2()[recoIdx];

			auto temp_pair	= smear_pt(pt_mu_pos, pt_gen_mu_pos, pt_mu_neg, pt_gen_mu_neg);
			Double_t pt_mu_pos_Smearted  = temp_pair.first;
			Double_t pt_mu_neg_Smearted  = temp_pair.second;

			TLorentzVector FourMom_mu_pos, FourMom_mu_neg, pairFourMom;
			FourMom_mu_pos.SetPtEtaPhiM(pt_mu_pos_Smearted, eta_mu_pos, phi_mu_pos, MASS_MUON);
			FourMom_mu_neg.SetPtEtaPhiM(pt_mu_neg_Smearted, eta_mu_neg, phi_mu_neg, MASS_MUON);
			pairFourMom = FourMom_mu_pos + FourMom_mu_neg;

			// // Fill Hist for Without Smearing
			// if(goodMuPair(csTree, recoIdx, L1_TrigIdx))
			// {
			// 	hRapvsPtvsM_woEvtSel_woSmear->Fill(y_woSmear, pt_woSmear, mass_woSmear);

			// 	if( passEvtSel )
			// 	{
			// 		hRapvsPtvsM_woSmear->Fill(y_woSmear, pt_woSmear, mass_woSmear);
			// 	}
			// }

			// // Fill Hist for With Smearing
			// if( goodMuPair(fourMom_Smeared[0], isPosTrig, fourMom_Smeared[1], isNegTrig) )
			// {
			// 	//* fill numerator of paired single muon reconstruction eff
			// 	hPtvsEtavsPhi_gen_mu_pos_reconstructed_pair	->Fill(pt_gen_mu_pos,	eta_gen_mu_pos,	phi_gen_mu_pos);
			// 	hPtvsEtavsPhi_gen_mu_neg_reconstructed_pair	->Fill(pt_gen_mu_neg,	eta_gen_mu_neg,	phi_gen_mu_neg);

			// 	hRapvsPtvsM_woEvtSel->Fill(y_Smeared, pt_Smeared, mass_Smeared);
			// }
		}


		//! Event Level Selections !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (!passEvtSel) 			continue;
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


		//= Fill HLT Trig Eff Hist =======================================================================
		for (UInt_t icand = 0; icand < csTree.candSize(); icand++)
		{
			//* Using the combined kinematics such as pT(), eta(), phi() since it's more accurate
			//* the difference between using the combined kinematics and summing up the individual kinematics is small
			//* but the combined kinematics is more accurate due to the vertex fitting

			Double_t pt		= csTree.pT()[icand];
			Double_t eta	= csTree.eta()[icand];
			Double_t phi	= csTree.phi()[icand];
			Double_t mass	= csTree.mass()[icand];
			Double_t rap	= csTree.y()[icand];

			//* fill denominator of HLT trig eff
			hPtvsEtavsPhi	->Fill(pt,	eta,	phi);
			hPtvsRapvsPhi	->Fill(pt,	rap,	phi);

			//* fill numerator of HLT trig eff for each trigger
			for (int iTrig = 0; iTrig < nTrig; ++iTrig)
			{
				if (csTree.trigHLT()[HLT_TrigIdx[iTrig]])
				{
					vPtvsEtavsPhi_trigHLT[iTrig]->Fill(pt, eta, phi);
					vPtvsRapvsPhi_trigHLT[iTrig]->Fill(pt, rap, phi);
				}
			}
		}
	}

	cout<<"multiple_matched_tracks: "<<multiple_matched_tracks<<endl;
	cout<<"Percentage of multiple_matched_tracks: "<<multiple_matched_tracks*100./total_tracks<<"%"<<endl;

	draw_EvtSel();
	draw_Ntrk_distribution();
	draw_Npixel_distribution();

	//= Calculate HLT Trig Eff ========================================================================
	pipeline_draw_HLT_TrigEff(vPtvsRapvsPhi_trigHLT, hPtvsRapvsPhi, "x", "");
	pipeline_draw_HLT_TrigEff(vPtvsRapvsPhi_trigHLT, hPtvsRapvsPhi, "y", "");
	pipeline_draw_HLT_TrigEff(vPtvsRapvsPhi_trigHLT, hPtvsRapvsPhi, "z", "");

	//= Draw Reco Eff in Pt for Dimuon =================================================================
	// pipeline_draw_RecoEff(hPtvsRapvsPhi_gen_reconstructed->ProjectionY(), hPtvsRapvsPhi_gen->ProjectionY(), "");

	//= Draw Reco Eff in Pt for Single Muon ============================================================
	// pipeline_draw_RecoEff_SingleMu(hPtvsEtavsPhi_gen_mu_pos_reconstructed, hPtvsEtavsPhi_gen_mu_pos, "y", "./outFigures/Eff_RecoSingleMu.pdf");

	//= Draw L1 Trig Eff in Pt =========================================================================
	// pipeline_draw_L1_TrigEff(hPtvsEtavsPhi_mu_pos_trigL1, hPtvsEtavsPhi_mu_pos, "x");
	// pipeline_draw_L1_TrigEff(hPtvsEtavsPhi_mu_pos_pair_trigL1, hPtvsEtavsPhi_mu_pos_pair, "x", "./outFigures/Eff_L1_Trig_pair.pdf");

	// pipeline_draw_L1_TrigEff_inEtaBins(hPtvsEtavsPhi_mu_pos_trigL1, hPtvsEtavsPhi_mu_pos, "x", "./outFigures/Eff_L1_Trig_BD.pdf");
	// pipeline_draw_L1_TrigEff_inEtaBins(hPtvsEtavsPhi_mu_pos_pair_trigL1, hPtvsEtavsPhi_mu_pos_pair, "x", "./outFigures/Eff_L1_Trig_BD_pair.pdf");
}