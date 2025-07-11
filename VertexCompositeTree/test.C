// #include <filesystem>
// #include "./common/VertexCompositeTree.h"

void test()
{
	TH3D * hNum = new TH3D("hNum", "hNum", 100, 0, 10, 100, -2.5, 2.5, 100, -3.15, 3.15);
	TH3D * hDen = new TH3D("hDen", "hDen", 100, 0, 10, 100, -2.5, 2.5, 100, -3.15, 3.15);

	// fill hNum and hDen with some random numbers
	for (int i = 0; i < 100000; i++)
	{
		double pt = gRandom->Uniform(0, 10);
		double eta = gRandom->Uniform(-2.5, 2.5);
		double phi = gRandom->Uniform(-3.15, 3.15);

		hDen->Fill(pt, eta, phi);
		if (gRandom->Uniform(0, 1) > 0.5)	hNum->Fill(pt, eta, phi);
	}


	// project 2D histograms
	TH2D * hNum2D = (TH2D*)hNum->Project3D("xy");
	TH2D * hDen2D = (TH2D*)hDen->Project3D("xy");

	// divide 2D histograms
	TH2D * h1 = (TH2D*)hNum2D->Clone("h1");
	h1->Divide( hNum2D, hDen2D, 1, 1, "B" );

	// print out the efficiency
	for (int i = 0; i < h1->GetNbinsX(); i++)
	{

		cout << "bin: " << i << " eff: " << h1->GetBinContent(i+1, i+1) << " Den: " << hDen2D->GetBinContent(i+1, i+1) << " Num: " << hNum2D->GetBinContent(i+1, i+1) << endl;
	}
}