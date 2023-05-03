// #include "../common/headers.h"
#include "./common/function.h"
// #include "../common/funUtil.h"

const Bool_t  mStorePDF = kTRUE;

const Double_t mTinyNum = 1.e-6;

Int_t    mGenMarker = 20;
Int_t    mGenColor  = 1;
Int_t    mGenWidth  = 2;
Double_t mGenSize   = 0.8;
const double mEtaLow = -4;
const double mEtaHig = 4;
const double mPtLow = 0.0;
const double mPtHig = 0.3;

TH2D* Get2DEff(TH2D* hNumerator, TH2D* hDenominator)
{
	TH2D* hEff = (TH2D *)hNumerator->Clone(Form("%s_Eff", hNumerator->GetName()));
	hEff->Divide(hNumerator, hDenominator, 1, 1, "B");
	return hEff;
}
TH1D* Get1DEff(TH1D* hNumerator, TH1D* hDenominator)
{
	TH1D* hEff = (TH1D *)hNumerator->Clone(Form("%s_Eff", hNumerator->GetName()));
	hEff->Divide(hNumerator, hDenominator, 1, 1, "B");
	return hEff;
}
void DrawDiKaonHist(TH1D* hPosKaon, TH1D* hNegKaon, const double yRange, const int nRebin, const TString draw = "", const bool norm = true)
{
	hPosKaon->Rebin(nRebin);
	hNegKaon->Rebin(nRebin);
	if (norm)
	{
		hPosKaon->Scale(1.0 / hPosKaon->GetEntries());
		hNegKaon->Scale(1.0 / hNegKaon->GetEntries());
	}
	hPosKaon->GetYaxis()->SetRangeUser(0.0, yRange);
	setHisto(hPosKaon, 20, 0.8, 1, 1, 2);
	setHisto(hNegKaon, 24, 0.8, 4, 4, 2);
	hPosKaon->Draw(draw);
	hNegKaon->Draw(draw+"same");
}
void Plot2DEff(TH3D* hMthPosKaonPhivsEtavsPt_Gen, TH3D* hMthNegKaonPhivsEtavsPt_Gen,
				TH3D* hPosKaonPhivsEtavsPt_Gen, TH3D* hNegKaonPhivsEtavsPt_Gen,
				const TString outFileDir)
{
	// * when there is a reco track matched to a gen track, we use the gen info of the matched track to calculate the efficiency

	TH2D *hMthPosKaonEtavsPt_Gen = (TH2D *)hMthPosKaonPhivsEtavsPt_Gen->Project3D("hMthPosKaonEtavsPt_Gen_yx");
	TH2D *hMthNegKaonEtavsPt_Gen = (TH2D *)hMthNegKaonPhivsEtavsPt_Gen->Project3D("hMthNegKaonEtavsPt_Gen_yx");
	TH2D *hMthPosKaonPhivsPt_Gen = (TH2D *)hMthPosKaonPhivsEtavsPt_Gen->Project3D("hMthPosKaonPhivsPt_Gen_zx");
	TH2D *hMthNegKaonPhivsPt_Gen = (TH2D *)hMthNegKaonPhivsEtavsPt_Gen->Project3D("hMthNegKaonPhivsPt_Gen_zx");

	TH2D *hPosKaonEtavsPt_Gen = (TH2D *)hPosKaonPhivsEtavsPt_Gen->Project3D("hPosKaonEtavsPt_Gen_yx");
	TH2D *hNegKaonEtavsPt_Gen = (TH2D *)hNegKaonPhivsEtavsPt_Gen->Project3D("hNegKaonEtavsPt_Gen_yx");
	TH2D *hPosKaonPhivsPt_Gen = (TH2D *)hPosKaonPhivsEtavsPt_Gen->Project3D("hPosKaonPhivsPt_Gen_zx");
	TH2D *hNegKaonPhivsPt_Gen = (TH2D *)hNegKaonPhivsEtavsPt_Gen->Project3D("hNegKaonPhivsPt_Gen_zx");

	TH2D *hPosKaon2DMthEff_EtavsPt = Get2DEff(hMthPosKaonEtavsPt_Gen, hPosKaonEtavsPt_Gen);
	TH2D *hNegKaon2DMthEff_EtavsPt = Get2DEff(hMthNegKaonEtavsPt_Gen, hNegKaonEtavsPt_Gen);
	TH2D *hPosKaon2DMthEff_PhivsPt = Get2DEff(hMthPosKaonPhivsPt_Gen, hPosKaonPhivsPt_Gen);
	TH2D *hNegKaon2DMthEff_PhivsPt = Get2DEff(hMthNegKaonPhivsPt_Gen, hNegKaonPhivsPt_Gen);

	TCanvas* c1 = new TCanvas();
	c1->Divide(2, 2);

	Double_t xPos = 0.15;
	Double_t yPos = 0.84;

	c1->cd(1);
	hPosKaon2DMthEff_EtavsPt->GetXaxis()->SetRangeUser(mPtLow+mTinyNum, mPtHig-mTinyNum);
	hPosKaon2DMthEff_EtavsPt->GetYaxis()->SetRangeUser(mEtaLow+mTinyNum, mEtaHig-mTinyNum);
	hPosKaon2DMthEff_EtavsPt->Draw("colz");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency (K^{+})", 22, 0.06, 4);
	drawLatex(xPos, yPos, "(a)", 22, 0.08, 1);

	c1->cd(2);
	hPosKaon2DMthEff_PhivsPt->GetXaxis()->SetRangeUser(mPtLow+mTinyNum, mPtHig-mTinyNum);
	hPosKaon2DMthEff_PhivsPt->Draw("colz");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency (K^{+})", 22, 0.06, 4);
	drawLatex(xPos, yPos, "(b)", 22, 0.08, 1);

	c1->cd(3);
	hNegKaon2DMthEff_EtavsPt->GetXaxis()->SetRangeUser(mPtLow+mTinyNum, mPtHig-mTinyNum);
	hNegKaon2DMthEff_EtavsPt->GetYaxis()->SetRangeUser(mEtaLow+mTinyNum, mEtaHig-mTinyNum);
	hNegKaon2DMthEff_EtavsPt->Draw("colz");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency (K^{-})", 22, 0.06, 4);
	drawLatex(xPos, yPos, "(c)", 22, 0.08, 1);

	c1->cd(4);
	hNegKaon2DMthEff_PhivsPt->GetXaxis()->SetRangeUser(mPtLow+mTinyNum, mPtHig-mTinyNum);
	hNegKaon2DMthEff_PhivsPt->Draw("colz");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency (K^{-})", 22, 0.06, 4);
	drawLatex(xPos, yPos, "(d)", 22, 0.08, 1);

	if(mStorePDF) c1->SaveAs(outFileDir);
}

void Plot1DEff(TH3D* hMthPosKaonPhivsEtavsPt_Gen, TH3D* hMthNegKaonPhivsEtavsPt_Gen,
				TH3D* hPosKaonPhivsEtavsPt_Gen, TH3D* hNegKaonPhivsEtavsPt_Gen,
				const TString outFileDir)
{
	const int nPhiRebin = 4;
	const int nEtaRebin = 2;
	const int nPtRebin  = 1;

	auto hPosKaonPhi_Gen = (TH1D*) hPosKaonPhivsEtavsPt_Gen->ProjectionZ("hPosKaonPhi_Gen")->Rebin(nPhiRebin);
	auto hNegKaonPhi_Gen = (TH1D*) hNegKaonPhivsEtavsPt_Gen->ProjectionZ("hNegKaonPhi_Gen")->Rebin(nPhiRebin);
	auto hPosKaonEta_Gen = (TH1D*) hPosKaonPhivsEtavsPt_Gen->ProjectionY("hPosKaonEta_Gen")->Rebin(nEtaRebin);
	auto hNegKaonEta_Gen = (TH1D*) hNegKaonPhivsEtavsPt_Gen->ProjectionY("hNegKaonEta_Gen")->Rebin(nEtaRebin);
	auto hPosKaonPt_Gen  = (TH1D*) hPosKaonPhivsEtavsPt_Gen->ProjectionX("hPosKaonPt_Gen")->Rebin(nPtRebin);
	auto hNegKaonPt_Gen  = (TH1D*) hNegKaonPhivsEtavsPt_Gen->ProjectionX("hNegKaonPt_Gen")->Rebin(nPtRebin);

	auto hPosKaonPhi = (TH1D*) hMthPosKaonPhivsEtavsPt_Gen->ProjectionZ("hPosKaonPhi")->Rebin(nPhiRebin);
	auto hNegKaonPhi = (TH1D*) hMthNegKaonPhivsEtavsPt_Gen->ProjectionZ("hNegKaonPhi")->Rebin(nPhiRebin);
	auto hPosKaonEta = (TH1D*) hMthPosKaonPhivsEtavsPt_Gen->ProjectionY("hPosKaonEta")->Rebin(nEtaRebin);
	auto hNegKaonEta = (TH1D*) hMthNegKaonPhivsEtavsPt_Gen->ProjectionY("hNegKaonEta")->Rebin(nEtaRebin);
	auto hPosKaonPt  = (TH1D*) hMthPosKaonPhivsEtavsPt_Gen->ProjectionX("hPosKaonPt")->Rebin(nPtRebin);
	auto hNegKaonPt  = (TH1D*) hMthNegKaonPhivsEtavsPt_Gen->ProjectionX("hNegKaonPt")->Rebin(nPtRebin);

	auto hPosKaonPhiEff = Get1DEff(hPosKaonPhi, hPosKaonPhi_Gen);
	auto hNegKaonPhiEff = Get1DEff(hNegKaonPhi, hNegKaonPhi_Gen);
	auto hPosKaonEtaEff = Get1DEff(hPosKaonEta, hPosKaonEta_Gen);
	auto hNegKaonEtaEff = Get1DEff(hNegKaonEta, hNegKaonEta_Gen);
	auto hPosKaonPtEff  = Get1DEff(hPosKaonPt,  hPosKaonPt_Gen);
	auto hNegKaonPtEff  = Get1DEff(hNegKaonPt,  hNegKaonPt_Gen);

	TCanvas* c1 = new TCanvas();
	c1->Divide(2, 2);

	Double_t xPos = 0.15;
	Double_t yPos = 0.84;

	c1->cd(1);
	auto leg = new TLegend(0.7, 0.65, 0.9, 0.9);
	leg->SetFillStyle(0);
	leg->SetTextSize(0.08);
	DrawDiKaonHist(hPosKaonPhiEff, hNegKaonPhiEff, 0.1, 1, "histe", false);
	leg->AddEntry(hPosKaonPhiEff, "K^{+}", "l");
	leg->AddEntry(hNegKaonPhiEff, "K^{-}", "l");
	leg->Draw("same");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency", 22, 0.06, 4);
	drawLatex(xPos, yPos, "(a)", 22, 0.08, 1);

	c1->cd(2);
	DrawDiKaonHist(hPosKaonEtaEff, hNegKaonEtaEff, 0.2, 1, "histe", false);
	drawLatex(0.28, 0.945, "Reconstruted Efficiency", 22, 0.06, 4);
	drawLatex(xPos, yPos, "(b)", 22, 0.08, 1);

	c1->cd(3);
	DrawDiKaonHist(hPosKaonPtEff, hNegKaonPtEff, 0.2, 1, "histe", false);
	drawLatex(0.28, 0.945, "Reconstruted Efficiency", 22, 0.06, 4);
	drawLatex(xPos, yPos, "(c)", 22, 0.08, 1);

	if(mStorePDF) c1->SaveAs(outFileDir);
}

void Plot2DEff_PhivsEta(TH3D* hMthPosKaonPhivsEtavsPt_Gen, TH3D* hMthNegKaonPhivsEtavsPt_Gen,
				TH3D* hPosKaonPhivsEtavsPt_Gen, TH3D* hNegKaonPhivsEtavsPt_Gen,
				const TString outFileDir)
{
	TH2D *hMthPosKaonPhivsEta_Gen = (TH2D *)hMthPosKaonPhivsEtavsPt_Gen->Project3D("hMthPosKaonPhivsEta_Gen_zy");
	TH2D *hPosKaonPhivsEta_Gen    = (TH2D *)hPosKaonPhivsEtavsPt_Gen->Project3D("hPosKaonPhivsEta_Gen_zy");
	TH2D *hMthNegKaonPhivsEta_Gen = (TH2D *)hMthNegKaonPhivsEtavsPt_Gen->Project3D("hMthNegKaonPhivsEta_Gen_zy");
	TH2D *hNegKaonPhivsEta_Gen    = (TH2D *)hNegKaonPhivsEtavsPt_Gen->Project3D("hNegKaonPhivsEta_Gen_zy");

	TH1D *hMthPosKaonPhi_Gen      = (TH1D *)hMthPosKaonPhivsEtavsPt_Gen->ProjectionZ("hMthPosKaonPhi_Gen");
	TH1D *hMthNegKaonPhi_Gen      = (TH1D *)hMthNegKaonPhivsEtavsPt_Gen->ProjectionZ("hMthNegKaonPhi_Gen");
	TH1D *hPosKaonPhi_Gen         = (TH1D *)hPosKaonPhivsEtavsPt_Gen->ProjectionZ("hPosKaonPhi_Gen");
	TH1D *hNegKaonPhi_Gen         = (TH1D *)hNegKaonPhivsEtavsPt_Gen->ProjectionZ("hNegKaonPhi_Gen");

	TH2D *hPosKaon2DMthEff_PhivsEta = Get2DEff(hMthPosKaonPhivsEta_Gen, hPosKaonPhivsEta_Gen);
	TH2D *hNegKaon2DMthEff_PhivsEta = Get2DEff(hMthNegKaonPhivsEta_Gen, hNegKaonPhivsEta_Gen);
	TH1D *hPosKaonMthEffvsPhi = Get1DEff(hMthPosKaonPhi_Gen, hPosKaonPhi_Gen);
	TH1D *hNegKaonMthEffvsPhi = Get1DEff(hMthNegKaonPhi_Gen, hNegKaonPhi_Gen);

	TCanvas* c1 = new TCanvas();
	c1->Divide(2, 2);

	Double_t xPos = 0.15;
	Double_t yPos = 0.84;

	c1->cd(1);
	hPosKaon2DMthEff_PhivsEta->GetXaxis()->SetRangeUser(mEtaLow+mTinyNum, mEtaHig-mTinyNum);
	hPosKaon2DMthEff_PhivsEta->Draw("colz");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency (K^{+})", 22, 0.06, 4);
	drawLatex(0.48, 0.16, "(a)", 22, 0.08, 1);

	c1->cd(2);
	hNegKaon2DMthEff_PhivsEta->GetXaxis()->SetRangeUser(mEtaLow+mTinyNum, mEtaHig-mTinyNum);
	hNegKaon2DMthEff_PhivsEta->Draw("colz");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency (K^{-})", 22, 0.06, 4);
	drawLatex(0.48, 0.16, "(b)", 22, 0.08, 1);

	c1->cd(3);
	TLegend* leg = new TLegend(0.15, 0.65, 0.6, 0.9);
	leg->SetFillStyle(0);
	leg->SetTextSize(0.1);
	setHisto(hPosKaonMthEffvsPhi, 20, 0.5, 1, 1, 2);
	setHisto(hNegKaonMthEffvsPhi, 24, 0.5, 4, 4, 2);
	hPosKaonMthEffvsPhi->GetYaxis()->SetTitle("Reconstruted Efficiency");
	// if(filename.EqualTo("LowMassGammaGamma")) hPosKaonMthEffvsPhi->GetYaxis()->SetRangeUser(0.45, 0.65);
	// else hPosKaonMthEffvsPhi->GetYaxis()->SetRangeUser(0.0, 0.02);
	hPosKaonMthEffvsPhi->GetYaxis()->SetRangeUser(0.0, 0.1);
	hPosKaonMthEffvsPhi->Draw("histe");
	hNegKaonMthEffvsPhi->Draw("histesame");
	leg->AddEntry(hPosKaonMthEffvsPhi, "K^{+}", "l");
	leg->AddEntry(hNegKaonMthEffvsPhi, "K^{-}", "l");
	leg->Draw("same");
	drawLatex(0.48, 0.16, "(c)", 22, 0.08, 1);

	c1->cd(4);
	TH1D *hKaonMthEffRatiovsPhi = (TH1D *)hPosKaonMthEffvsPhi->Clone("hKaonMthEffRatiovsPhi");
	hKaonMthEffRatiovsPhi->Divide(hPosKaonMthEffvsPhi, hNegKaonMthEffvsPhi, 1, 1);
	setHisto(hKaonMthEffRatiovsPhi, 20, 0.8, 1, 1, 2);
	hKaonMthEffRatiovsPhi->GetYaxis()->SetRangeUser(0.0, 1.7);
	hKaonMthEffRatiovsPhi->GetYaxis()->SetTitle("K^{+}/K^{-} Efficiency Ratio");
	hKaonMthEffRatiovsPhi->Fit("pol0", "", "", -TMath::Pi(), TMath::Pi());
	hKaonMthEffRatiovsPhi->Draw("psame");
	drawLatex(0.48, 0.16, "(d)", 22, 0.08, 1);
	if(mStorePDF) c1->SaveAs(outFileDir);
}



void Plot1DKinematics(TH3D* hPosKaonPhivsEtavsPt_Gen, TH3D* hNegKaonPhivsEtavsPt_Gen,
					TH3D* hPosKaonPhivsEtavsPt, TH3D* hNegKaonPhivsEtavsPt,
					const TString outFileDir)
{
	auto hPosKaonPhi_Gen = (TH1D*) hPosKaonPhivsEtavsPt_Gen->ProjectionZ("hPosKaonPhi_Gen");
	auto hNegKaonPhi_Gen = (TH1D*) hNegKaonPhivsEtavsPt_Gen->ProjectionZ("hNegKaonPhi_Gen");
	auto hPosKaonEta_Gen = (TH1D*) hPosKaonPhivsEtavsPt_Gen->ProjectionY("hPosKaonEta_Gen");
	auto hNegKaonEta_Gen = (TH1D*) hNegKaonPhivsEtavsPt_Gen->ProjectionY("hNegKaonEta_Gen");
	auto hPosKaonPt_Gen  = (TH1D*) hPosKaonPhivsEtavsPt_Gen->ProjectionX("hPosKaonPt_Gen");
	auto hNegKaonPt_Gen  = (TH1D*) hNegKaonPhivsEtavsPt_Gen->ProjectionX("hNegKaonPt_Gen");

	auto hPosKaonPhi = (TH1D*) hPosKaonPhivsEtavsPt->ProjectionZ("hPosKaonPhi");
	auto hNegKaonPhi = (TH1D*) hNegKaonPhivsEtavsPt->ProjectionZ("hNegKaonPhi");
	auto hPosKaonEta = (TH1D*) hPosKaonPhivsEtavsPt->ProjectionY("hPosKaonEta");
	auto hNegKaonEta = (TH1D*) hNegKaonPhivsEtavsPt->ProjectionY("hNegKaonEta");
	auto hPosKaonPt  = (TH1D*) hPosKaonPhivsEtavsPt->ProjectionX("hPosKaonPt");
	auto hNegKaonPt  = (TH1D*) hNegKaonPhivsEtavsPt->ProjectionX("hNegKaonPt");

	TCanvas* c1 = new TCanvas();
	c1->Divide(2, 3);

	c1->cd(1);
	// gPad->SetMargin(0.1, 0.1, 0.08, 0.1);  // left, right, bottom, top
	TLegend* leg = new TLegend(0.15, 0.6, 0.6, 0.85);
	leg->SetFillStyle(0);
	leg->SetTextSize(0.1);
	const int nPosKaon_Gen = hPosKaonPhi_Gen->GetEntries();
	const int nNegKaon_Gen = hNegKaonPhi_Gen->GetEntries();
	DrawDiKaonHist(hPosKaonPhi_Gen, hNegKaonPhi_Gen, 0.2, 4);
	leg->AddEntry(hPosKaonPhi_Gen, "K^{+}", "p");
	leg->AddEntry(hNegKaonPhi_Gen, "K^{-}", "p");
	leg->Draw("same");
	drawLatex(0.4, 0.945, "Gen level", 22, 0.05, 4);
	drawLatex(0.65, 0.80, Form("N_{K^{+}} = %d", nPosKaon_Gen), 22, 0.08, 1);
	drawLatex(0.65, 0.70, Form("N_{K^{-}} = %d", nNegKaon_Gen), 22, 0.08, 4);

	c1->cd(2);
	const int nPosKaon = hPosKaonPhi->GetEntries();
	const int nNegKaon = hNegKaonPhi->GetEntries();
	DrawDiKaonHist(hPosKaonPhi, hNegKaonPhi, 0.2, 4);
	drawLatex(0.5, 0.945, "Rec level", 22, 0.05, 4);
	drawLatex(0.65, 0.80, Form("N_{K^{+}} = %d", nPosKaon), 22, 0.08, 1);
	drawLatex(0.65, 0.70, Form("N_{K^{-}} = %d", nNegKaon), 22, 0.08, 4);


	c1->cd(3);
	DrawDiKaonHist(hPosKaonEta_Gen, hNegKaonEta_Gen, 0.2, 2);
	hNegKaonEta_Gen->Draw("same");

	c1->cd(4);
	DrawDiKaonHist(hPosKaonEta, hNegKaonEta, 0.2, 2);

	c1->cd(5);
	DrawDiKaonHist(hPosKaonPt_Gen, hNegKaonPt_Gen, 0.5, 1);

	c1->cd(6);
	DrawDiKaonHist(hPosKaonPt, hNegKaonPt, 0.5, 1);

	if(mStorePDF) c1->SaveAs(outFileDir);
}

void genKaonEff(TString filename="test")
{
	gStyle->SetOptFit(1111);

	TFile* f = TFile::Open(Form("outFiles/%s.root", filename.Data()));

	TH3D *hPosKaonPhivsEtavsPt_Gen		= (TH3D *)f->Get("hPosKaonPhivsEtavsPt_Gen");
	TH3D *hNegKaonPhivsEtavsPt_Gen		= (TH3D *)f->Get("hNegKaonPhivsEtavsPt_Gen");
	TH3D *hPosKaonPhivsEtavsPt			= (TH3D *)f->Get("hPosKaonPhivsEtavsPt");
	TH3D *hNegKaonPhivsEtavsPt			= (TH3D *)f->Get("hNegKaonPhivsEtavsPt");

	TH3D *hMthPosKaonPhivsEtavsPt_Gen	= (TH3D *)f->Get("hMthPosKaonPhivsEtavsPt_Gen");
	TH3D *hMthNegKaonPhivsEtavsPt_Gen	= (TH3D *)f->Get("hMthNegKaonPhivsEtavsPt_Gen");
	TH3D *hMthPosKaonPhivsEtavsPt		= (TH3D *)f->Get("hMthPosKaonPhivsEtavsPt");
	TH3D *hMthNegKaonPhivsEtavsPt		= (TH3D *)f->Get("hMthNegKaonPhivsEtavsPt");

	TString dir = Form("outFigures/KaonEffPlots/%s", filename.Data());
	system(Form("mkdir -p %s", dir.Data()));
	// system(Form("rm -rf %s/*", dir.Data()));

	Plot2DEff(hMthPosKaonPhivsEtavsPt_Gen, hMthNegKaonPhivsEtavsPt_Gen,
			 hPosKaonPhivsEtavsPt_Gen, hNegKaonPhivsEtavsPt_Gen,
			 dir+"/2DMthEff.pdf");

	Plot1DEff(hMthPosKaonPhivsEtavsPt_Gen, hMthNegKaonPhivsEtavsPt_Gen,
			 hPosKaonPhivsEtavsPt_Gen, hNegKaonPhivsEtavsPt_Gen,
			 dir+"/1DMthEff.pdf");

	Plot2DEff_PhivsEta(hMthPosKaonPhivsEtavsPt_Gen, hMthNegKaonPhivsEtavsPt_Gen,
			 hPosKaonPhivsEtavsPt_Gen, hNegKaonPhivsEtavsPt_Gen,
			 dir+"/2DMthEff_PhivsEta.pdf");

	// * only comparing the kinematics of the rec-gen matched tracks
	Plot1DKinematics(hMthPosKaonPhivsEtavsPt_Gen, hMthNegKaonPhivsEtavsPt_Gen,
					hMthPosKaonPhivsEtavsPt, hMthNegKaonPhivsEtavsPt,
					dir+"/1DMatchedKinematics.pdf");

	// * comparing the kinematics of all the gen and rec tracks
	Plot1DKinematics(hPosKaonPhivsEtavsPt_Gen, hNegKaonPhivsEtavsPt_Gen,
					hPosKaonPhivsEtavsPt, hNegKaonPhivsEtavsPt,
					dir+"/1DKinematics.pdf");


	cout << "End of program !" << endl;
}
