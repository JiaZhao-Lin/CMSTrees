// a class to hold the values of TTreeReaderValue
// can't use map because TTreeReaderValue automatically deletes after assignment
struct TreeValues
{
	TreeValues(TTreeReader& treeReader, std::vector<TString> varNames, std::vector<TString> varNamesV)
	:	myTreeReader{treeReader},	vTRVNames{varNames},	vTRVVNames{varNamesV}
	{
		// myTreeReader = treeReader;
		for (auto varName : varNames)
		{
			vTRV.push_back(TTreeReaderValue<Float_t> (treeReader, varName));
			mIndex[varName] = vTRV.size() - 1;
		}
		for (auto varNameV : varNamesV)
		{
			vTRVV.push_back(TTreeReaderValue<std::vector<Float_t>>(treeReader, varNameV));
			mIndexV[varNameV] = vTRVV.size() - 1;
		}
	}

	TTreeReaderValue< Float_t >* GetTRV(TString varName)
	{
		//find index of varName in vTRVNames and return the value
		auto it = std::find(vTRVNames.begin(), vTRVNames.end(), varName);
		if (it != vTRVNames.end())
		{
			auto index = std::distance(vTRVNames.begin(), it);
			return &vTRV.at(index);
		}
		else
		{
			throw std::runtime_error(Form("Error: %s not found in vTRVNames", varName.Data()));
		}
	}

	Float_t Get(TString varName, int i)
	{
		return vTRVV.at(mIndexV.at(varName))->at(i);
	}

	Int_t GetSize(TString varName)
	{
		return vTRVV.at(mIndexV.at(varName))->size();
	}

	Float_t Get(TString varName)
	{
		return *vTRV.at(mIndex.at(varName));
	}

	// void CheckValue(ROOT::Internal::TTreeReaderValueBase* value) {
	// 	if (value->GetSetupStatus() < 0) {
	// 		throw std::runtime_error(Form("Error %d setting up reader for %s", value->GetSetupStatus(), value->GetBranchName()));
   	// 	}
	// }

	const TTreeReader& myTreeReader;
	std::vector< TTreeReaderValue< Float_t > > 				vTRV;
	std::vector< TTreeReaderValue< std::vector<Float_t> > > vTRVV;
	std::vector< TString > 									vTRVNames;
	std::vector< TString > 									vTRVVNames;
	std::map< TString, int> 								mIndex;
	std::map< TString, int> 								mIndexV;
};