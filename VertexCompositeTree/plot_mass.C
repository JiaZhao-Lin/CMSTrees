"""
For plotting the dielectron mass distribution
"""

void fit_mass(TH1D *h1)
{
	//= fit the mass distribution with a crystal ball function within a jpis mass window =================
	// TF1 *f1 = new TF1("f1", "crystalball", 2.85, 3.3);
	// f1->SetParameters(20, 3.1, 0.1, 1, 1);
	// TF1 *f2 = new TF1("f2", "pol2", 2.85, 3.3);
	// f2->SetParNames("pol0", "pol1", "pol2");

	// TF1 *func = new TF1("func", "f1+f2", 2, 5);
	// // TF1 *func = new TF1("func", "f1", 2.70, 3.4);

	// h1->Fit("func", "R");

	//= use roofit to fit two crystal ball functions, one at the Jpsi mass and one at the psi(2S) mass, and a pol2 background function =================
	RooRealVar x("x", "m_{e^{+}e^{-}} [GeV]", 2.0, 5);
	RooRealVar  JpsiMean("JpsiMean", "JpsiMean", 3.1, 2.9, 3.3);
	RooRealVar  JpsiSigma("JpsiSigma", "JpsiSigma", 0.05, 0.01, 0.2);
	RooRealVar  JpsiAlpha("JpsiAlpha", "JpsiAlpha", 1, 0.01, 5);
	RooRealVar  JpsiN("JpsiN", "JpsiN", 1, 0.01, 10);
	RooCBShape Jpsi("Jpsi", "Jpsi", x, JpsiMean, JpsiSigma, JpsiAlpha, JpsiN);


	// RooConstVar massRatio = RooConstVar(  "massRatio",   "massRatio",   3.686/3.097 );
	// psiMu = jpsiMu * massRatio; psiSigma = jpsiSigma * massRatio
	// RooFormulaVar Psi2SMean("Psi2SMean", "Psi2SMean", "@0*@1", RooArgList(JpsiMean, massRatio));
	// RooFormulaVar Psi2SSigma("Psi2SSigma", "Psi2SSigma", "@0*@1", RooArgList(JpsiSigma, massRatio));
	// RooCBShape Psi2S("Psi2S", "Psi2S", x, Psi2SMean, Psi2SSigma, JpsiAlpha, JpsiN);

	RooRealVar  pol0("pol0", "pol0", 10, 0, 400);
	RooRealVar  pol1("pol1", "pol1", -4, -30, 30);
	RooRealVar  pol2("pol2", "pol2", 2, -30, 30);
	// RooPolynomial Bkg("Bkg", "Bkg", x, RooArgList(pol0));
	// RooPolynomial Bkg("Bkg", "Bkg", x, RooArgList(pol0, pol1));
	RooPolynomial Bkg("Bkg", "Bkg", x, RooArgList(pol0, pol1, pol2));

	// double nJpsi_init = h1->Integral(h1->FindBin(3.0), h1->FindBin(3.2));
	double nJpsi_init = h1->Integral(h1->FindBin(3.0), h1->FindBin(3.2));
	// double nPsi2S_init = nJpsi_init * 0.05;
	//use sideband to estimate the background
	double nBkg_init = h1->Integral(h1->FindBin(2.), h1->FindBin(2.9)) + h1->Integral(h1->FindBin(3.2), h1->FindBin(4.0));
	// double nBkg_init = 1000;

	RooRealVar nJpsi("nJpsi", "nJpsi", nJpsi_init, 0, nJpsi_init * 10);
	// RooRealVar nPsi2S("nPsi2S", "nPsi2S", nPsi2S_init, 0, nPsi2S_init * 10);
	RooRealVar nBkg("nBkg", "nBkg", nBkg_init, 0, nBkg_init * 10);

	// RooAddPdf totMassPdf("totMassPdf", "totMassPdf", RooArgList(Jpsi, Psi2S, Bkg), RooArgList(nJpsi, nPsi2S, nBkg));
	RooAddPdf totMassPdf("totMassPdf", "totMassPdf", RooArgList(Jpsi, Bkg), RooArgList(nJpsi, nBkg));

	// h1->Rebin(2);
	RooDataHist dataMass("dataMass", "dataMass", x, h1);
	auto fitResult = totMassPdf.fitTo(dataMass, RooFit::Extended(kTRUE), RooFit::SumW2Error(kTRUE), RooFit::Hesse(kTRUE), RooFit::Minos(kFALSE), RooFit::PrintLevel(-1), RooFit::Save(kTRUE));

	RooPlot *frame = x.frame();
	dataMass.plotOn(frame, RooFit::MarkerStyle(20), RooFit::MarkerSize(1), RooFit::MarkerColor(2), RooFit::LineColor(2), RooFit::LineWidth(2), RooFit::DrawOption("pz"));
	totMassPdf.plotOn(frame, RooFit::LineColor(1), RooFit::LineStyle(1), RooFit::LineWidth(2));
	totMassPdf.plotOn(frame, RooFit::Components("Jpsi"), RooFit::LineColor(kRed), 		RooFit::LineStyle(5), RooFit::LineWidth(2));
	// totMassPdf.plotOn(frame, RooFit::Components("Psi2S"), RooFit::LineColor(kGreen), 	RooFit::LineStyle(6), RooFit::LineWidth(2));
	totMassPdf.plotOn(frame, RooFit::Components("Bkg"), RooFit::LineColor(kBlue), 		RooFit::LineStyle(2), RooFit::LineWidth(3));
	// totMassPdf.paramOn(frame, RooFit::Layout(0.5, 0.9, 0.9));
	//Set y axis label
	frame->GetYaxis()->SetTitle(Form("Events / (%.2f GeV)", h1->GetBinWidth(1)));
	frame->Draw();
	//draw chi2/ndf on the plot
	double chi2 = frame->chiSquare("totMassPdf_Norm[x]", "h_dataMass", fitResult->floatParsFinal().getSize());

	// add legend
	TLegend *leg = new TLegend(0.65, 0.55, 0.9, 0.85);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->AddEntry(frame->findObject("h_dataMass"), "Data", "pe");
	leg->AddEntry(frame->findObject("totMassPdf_Norm[x]"), "Fit", "l");
	leg->AddEntry(frame->findObject("totMassPdf_Norm[x]_Comp[Jpsi]"), "J/#psi", "l");
	// leg->AddEntry(frame->findObject("totMassPdf_Norm[x]_Comp[Psi2S]"), "#psi(2S)", "l");
	leg->AddEntry(frame->findObject("totMassPdf_Norm[x]_Comp[Bkg]"), "Bkg", "l");
	leg->Draw("same");

	TLatex *t = new TLatex();
	t->SetNDC();
	t->SetTextFont(42);
	t->SetTextSize(0.05);
	// cout << "chi2/ndf = " << chi2 << endl;
	// t->DrawLatex(0.15, 0.85, Form("#chi^{2}/ndf = %.2f", chi2));
	t->DrawLatex(0.13, 0.94, Form("#bf{CMS} #it{Preliminary}"));
	t->DrawLatex(0.62, 0.94, Form("PbPb (2023, 5.36 TeV)"));


	
}

void plot_mass()
{
	TString inputFileName = "inFiles/2023/dielectron_Mass_30thNov.root";
	TFile *inputFile = new TFile(inputFileName.Data(), "READ");

	TH1D *h1 = (TH1D *)inputFile->Get("hdieleMass1Data_2023");

	auto c = new TCanvas("c", "", 800, 600);

	fit_mass(h1);


	c->SaveAs("outFigures/dielectron_mass.png");
}