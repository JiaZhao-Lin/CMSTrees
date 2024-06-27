
TH2D *hWeight = nullptr;

void init_weight()
{
	// TString inFile_name_data = "./inFiles/eff/efficiency_ZB.root";
	// TString inFile_name_data = "./inFiles/eff/efficiency_ZB_NgeneralTrk.root";
	// TString inFile_name_mc = "./inFiles/eff/efficiency_mc_NgeneralTrk.root";
	TString inFile_name_mc = "./inFiles/eff/efficiency_ZB.root";
	TString inFile_name_data = "./inFiles/eff/efficiency_mc.root";
	// TString inFile_name_mc = "./inFiles/eff/efficiency_mc_doublediff.root";

	TFile *inFile_data = new TFile(inFile_name_data);
	TFile *inFile_mc = new TFile(inFile_name_mc);

	TTree *tree_data = (TTree*)inFile_data->Get("efficiency");
	TTree *tree_mc = (TTree*)inFile_mc->Get("efficiency");

	TH2D *hEtaPt_data = new TH2D("hEtaPt_data", "hEtaPt_data", 100, -2.4, 2.4, 500, 0, 5);
	TH2D *hEtaPt_mc = new TH2D("hEtaPt_mc", "hEtaPt_mc", 100, -2.4, 2.4, 500, 0, 5);

	std::vector<double> *m_generalTracks_pT = 0;
	std::vector<double> *m_generalTracks_eta = 0;

	// fill hEtaPt_data
	tree_data->SetBranchAddress("m_generalTracks_pT", &m_generalTracks_pT);
	tree_data->SetBranchAddress("m_generalTracks_eta", &m_generalTracks_eta);

	for (int i = 0; i < tree_data->GetEntries(); i++)
	{
		tree_data->GetEntry(i);
		for (int j = 0; j < m_generalTracks_pT->size(); j++)
		{
			hEtaPt_data->Fill(m_generalTracks_eta->at(j), m_generalTracks_pT->at(j));
		}
	}

	// fill hEtaPt_mc
	tree_mc->SetBranchAddress("m_generalTracks_pT", &m_generalTracks_pT);
	tree_mc->SetBranchAddress("m_generalTracks_eta", &m_generalTracks_eta);

	for (int i = 0; i < tree_mc->GetEntries(); i++)
	{
		tree_mc->GetEntry(i);
		for (int j = 0; j < m_generalTracks_pT->size(); j++)
		{
			hEtaPt_mc->Fill(m_generalTracks_eta->at(j), m_generalTracks_pT->at(j));
		}
	}

	// normalize
	hEtaPt_data->Scale(1.0 / hEtaPt_data->Integral());
	hEtaPt_mc->Scale(1.0 / hEtaPt_mc->Integral());

	hWeight = (TH2D*) hEtaPt_data->Clone("hWeight");
	hWeight->Divide(hEtaPt_mc);
}

double get_weight(double eta, double pt)
{
	if (hWeight == nullptr)
	{
		init_weight();
	}

	int bin_eta = hWeight->GetXaxis()->FindBin(eta);
	int bin_pt = hWeight->GetYaxis()->FindBin(pt);

	// if out of range, return 0
	if (bin_eta == 0 || bin_eta == hWeight->GetNbinsX() + 1 || bin_pt == 0 || bin_pt == hWeight->GetNbinsY() + 1)
	{
		return 0.0;
	}
	
	return hWeight->GetBinContent(bin_eta, bin_pt);
}

double get_weight(vector<double> eta, vector<double> pt)
{
	if (hWeight == nullptr)
	{
		init_weight();
	}

	// double weight = 1.0;
	// for (int i = 0; i < eta.size(); i++)
	// {
	// 	weight *= get_weight(eta[i], pt[i]);
	// }

	// find the leading pT track
	if (eta.size() == 0)
	{
		return 1.0;
	}
	int max_pt_index = std::distance(pt.begin(), std::max_element(pt.begin(), pt.end()));
	double weight = get_weight(eta[max_pt_index], pt[max_pt_index]);


	return weight;
}