
const Double_t mRapLow = 1.45, mRapHi = 2.45;
const Double_t mKaonPtTh = 0.06,  mKaonEtaLow = 1.28, mKaonEtaHig = 4.00;
const Double_t PI = TMath::Pi();

void anaSTARlight(TString parSpec = "CohPhi_200k", const Int_t nAnaEvts=1000000)
{
	TString inFileName = "../starlightLHE/generateLHE/" + parSpec + "/slight." + parSpec + ".out";

	TH1D *hnEvts                = new TH1D("hnEvts", "hnEvts", 5, -0.5, 4.5);
	TH3D *hMvsPtvsRap_STARlight = new TH3D("hMvsPtvsRap_STARlight", "hMvsPtvsRap_STARlight; y; p_{T} (GeV); M_{#mu#mu} (GeV); Entries", 100, -2.5, 2.5, 2000, 0, 2, 300, 0, 3);
	TH3D *hMvsPtvsRap_KaonFilter  = new TH3D("hMvsPtvsRap_KaonFilter", "hMvsPtvsRap_KaonFilter; y; p_{T} (GeV); M_{#mu#mu} (GeV); Entries", 100, -2.5, 2.5, 2000, 0, 2, 300, 0, 3);
	TH2D *hNegPtvsPosPt         = new TH2D("hNegPtvsPosPt", "hNegPtvsPosPt; #mu^{+} p_{T} (GeV); #mu^{-} p_{T} (GeV)", 200, 0, 10, 200, 0, 10);
	TH2D *hNegEtavsPosEta       = new TH2D("hNegEtavsPosEta", "hNegEtavsPosEta; #mu^{+} #eta; #mu^{-} #eta", 200, -10, 10, 200, -10, 10);
	TH2D *hNegPhivsPosPhi       = new TH2D("hNegPhivsPosPhi", "hNegPhivsPosPhi; #mu^{+} #phi; #mu^{-} #phi", 120, -PI, PI, 120, -PI, PI);

	ifstream infile(inFileName.Data());
	if (!infile.is_open()) { cout << "\t ERROR: I can not open \"" << inFileName << "\"" << endl; return; }

	string temp_string, temp;
	istringstream curstring;

	int nEvts=0; // event_counter
	int evtIdx = 0, NTrk = 0, useless = 0;
	int pdg_id_temp = -1;
	double px_temp = 0, py_temp = 0, pz_temp = 0;

	std::vector<double> px, py, pz, e;
	std::vector<int>    pdg_id;
	TLorentzVector      posFourMom, negFourMom;
	const int KAON_PDG = 321;
	while (getline(infile, temp_string)) 
	{
		curstring.clear(); // needed when using several times istringstream::str(string)
		curstring.str(temp_string);

		if(strstr(temp_string.c_str(), "EVENT")) 
		{
			nEvts++;

			if(nEvts > nAnaEvts) break;
			if(nEvts%100000 == 0) cout<<"Working on "<< nEvts/100000 <<"-th 100k event ..."<<endl;

			// EVENT:          1       2       1
			curstring >> temp >> evtIdx >> NTrk >> useless; //assuming that EVENT always preceeds VERTEX/TRACK so that NTrk is set correctly

			pdg_id.clear();
			px.clear();
			py.clear();
			pz.clear();
			e.clear();

			posFourMom.SetPxPyPzE(0, 0, 0, 0);
			negFourMom.SetPxPyPzE(0, 0, 0, 0);
		}
		else if(strstr(temp_string.c_str(), "TRACK")) 
		{
			curstring >> temp >> useless >> px_temp >> py_temp >> pz_temp >> useless >> useless >> useless >> pdg_id_temp;

			TParticle particle_temp(pdg_id_temp, 0, 0, 0, 0, 0, px_temp, py_temp, pz_temp, 0.0, 0.0, 0.0, 0.0, 0.0);
			double mass_temp = particle_temp.GetMass();
			double e_temp    = TMath::Sqrt(pow(mass_temp, 2) + pow(particle_temp.P(), 2));

			px.push_back(px_temp);
			py.push_back(py_temp);
			pz.push_back(pz_temp);
			e.push_back(e_temp);
			pdg_id.push_back(pdg_id_temp);
		}

		if(NTrk == (int)px.size())
		{
			if(TMath::Abs(pdg_id[0])==KAON_PDG && TMath::Abs(pdg_id[1])==KAON_PDG)
			{
				hnEvts->Fill(0);

				if(pdg_id[0] < 0){
					posFourMom.SetPxPyPzE(px[0], py[0], pz[0], e[0]);
					negFourMom.SetPxPyPzE(px[1], py[1], pz[1], e[1]);
				}
				else{
					posFourMom.SetPxPyPzE(px[1], py[1], pz[1], e[1]);
					negFourMom.SetPxPyPzE(px[0], py[0], pz[0], e[0]);
				}

				TLorentzVector motherFourMom = posFourMom + negFourMom;

				double pt   = motherFourMom.Pt();
				double mass = motherFourMom.M();
				double y    = motherFourMom.Rapidity();

				if(TMath::Abs(y) < mRapLow || TMath::Abs(y) > mRapHi) continue;

				hnEvts->Fill(1);
				hMvsPtvsRap_STARlight->Fill(y, pt, mass);

				// mKaonPtTh, mKaonEtaLow, and mKaonEtaHig are used to filter muon kinematics during (private/official) MC production
				// see details in https://github.com/mumuustc/starlightLHE/blob/master/scriptForSimulation/Configuration/GenProduction/python/HINPbPbAutumn18GS_STARlight_fragment.py
				// see details in https://twiki.cern.ch/twiki/bin/viewauth/CMS/MCFor2018PbPb5TeV: the LHE-MuonCUT-fragment.py file in "Photo-production of charmonia in ultraperipheral PbPb collisions (Shuai Yang)" request
				
				// ! no filter for now -----------------------------------------------
				if( posFourMom.Pt() >= mKaonPtTh && TMath::Abs(posFourMom.Eta()) >= mKaonEtaLow && TMath::Abs(posFourMom.Eta()) <= mKaonEtaHig
						&& negFourMom.Pt() >= mKaonPtTh && TMath::Abs(negFourMom.Eta()) >= mKaonEtaLow && TMath::Abs(negFourMom.Eta()) <= mKaonEtaHig
				  ){
					if(TMath::Abs(y) >= mRapLow && TMath::Abs(y) <= mRapHi) hnEvts->Fill(2);

					hMvsPtvsRap_KaonFilter->Fill(y, pt, mass);
					hNegPtvsPosPt->Fill(posFourMom.Pt(), negFourMom.Pt());
					hNegEtavsPosEta->Fill(posFourMom.Eta(), negFourMom.Eta());
					hNegPhivsPosPhi->Fill(posFourMom.Phi(), negFourMom.Phi());
				}
				// !-------------------------------------------------------------------

				// hMvsPtvsRap_KaonFilter->Fill(y, pt, mass);
				// hNegPtvsPosPt->Fill(posFourMom.Pt(), negFourMom.Pt());
				// hNegEtavsPosEta->Fill(posFourMom.Eta(), negFourMom.Eta());
				// hNegPhivsPosPhi->Fill(posFourMom.Phi(), negFourMom.Phi());

			}
			else{
				cout<<"The first two tracks are not muons !"<<endl;
			}
		}

	} // reading loop of the input file

	infile.close();

	system("mkdir -p outFiles/AccHist");

	TFile *fOut = new TFile(Form("outFiles/AccHist/%s.root", parSpec.Data()), "recreate");
	fOut->cd();
	hnEvts->Write();
	hMvsPtvsRap_STARlight->Write();
	hMvsPtvsRap_KaonFilter->Write();
	hNegPtvsPosPt->Write();
	hNegEtavsPosEta->Write();
	hNegPhivsPosPhi->Write();
	fOut->Close();
}
