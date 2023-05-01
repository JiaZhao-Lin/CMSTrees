// #include "../common/headers.h"
#include "./common/function.h"
// #include "../common/funUtil.h"

const Bool_t  mStorePDF = kTRUE;

const Double_t mTinyNum = 1.e-6;

Int_t    mGenMarker = 20;
Int_t    mGenColor  = 1;
Int_t    mGenWidth  = 2;
Double_t mGenSize   = 0.8;

void genKaonEff(TString filename="test")
{
	gStyle->SetOptFit(1111);

	TFile* f = TFile::Open(Form("outFiles/%s.root", filename.Data()));

	TH3D *hPosKaonPhivsEtavsPt_Gen    = (TH3D *)f->Get("hPosKaonPhivsEtavsPt_Gen");
	TH3D *hMthPosKaonPhivsEtavsPt_Gen = (TH3D *)f->Get("hMthPosKaonPhivsEtavsPt_Gen");
	TH3D *hMthPosKaonPhivsEtavsPt     = (TH3D *)f->Get("hMthPosKaonPhivsEtavsPt");
	TH3D *hNegKaonPhivsEtavsPt_Gen    = (TH3D *)f->Get("hNegKaonPhivsEtavsPt_Gen");
	TH3D *hMthNegKaonPhivsEtavsPt_Gen = (TH3D *)f->Get("hMthNegKaonPhivsEtavsPt_Gen");
	TH3D *hMthNegKaonPhivsEtavsPt     = (TH3D *)f->Get("hMthNegKaonPhivsEtavsPt");

	TString dir = Form("outFigures/KaonEffPlots/%s", filename.Data());
	system(Form("mkdir -p %s", dir.Data()));
	// system(Form("rm -rf %s/*", dir.Data()));

	TH2D *hPosKaonEtavsPt_Gen = (TH2D *)hPosKaonPhivsEtavsPt_Gen->Project3D("hPosKaonEtavsPt_Gen_yx");
	TH2D *hPosKaonPhivsPt_Gen = (TH2D *)hPosKaonPhivsEtavsPt_Gen->Project3D("hPosKaonPhivsPt_Gen_zx");
	TH2D *hMthPosKaonEtavsPt_Gen = (TH2D *)hMthPosKaonPhivsEtavsPt_Gen->Project3D("hMthPosKaonEtavsPt_Gen_yx");
	TH2D *hMthPosKaonPhivsPt_Gen = (TH2D *)hMthPosKaonPhivsEtavsPt_Gen->Project3D("hMthPosKaonPhivsPt_Gen_zx");

	TH2D *hPosKaonPhivsEta_Gen    = (TH2D *)hPosKaonPhivsEtavsPt_Gen->Project3D("hPosKaonPhivsEta_Gen_zy");
	TH2D *hMthPosKaonPhivsEta_Gen = (TH2D *)hMthPosKaonPhivsEtavsPt_Gen->Project3D("hMthPosKaonPhivsEta_Gen_zy");
	TH1D *hPosKaonPhi_Gen         = (TH1D *)hPosKaonPhivsEtavsPt_Gen->ProjectionZ("hPosKaonPhi_Gen");
	TH1D *hMthPosKaonPhi_Gen      = (TH1D *)hMthPosKaonPhivsEtavsPt_Gen->ProjectionZ("hMthPosKaonPhi_Gen");

	TH2D *hMthPosKaonEtavsPt  = (TH2D *)hMthPosKaonPhivsEtavsPt->Project3D("hMthPosKaonEtavsPt_yx");
	TH2D *hMthPosKaonPhivsPt  = (TH2D *)hMthPosKaonPhivsEtavsPt->Project3D("hMthPosKaonPhivsPt_zx");

	TH2D *hMthPosKaonPhivsEta  = (TH2D *)hMthPosKaonPhivsEtavsPt->Project3D("hMthPosKaonPhivsEta_zy");
	TH1D *hMthPosKaonPhi       = (TH1D *)hMthPosKaonPhivsEtavsPt->ProjectionZ("hMthPosKaonPhi");

	TH2D *hNegKaonEtavsPt_Gen = (TH2D *)hNegKaonPhivsEtavsPt_Gen->Project3D("hNegKaonEtavsPt_Gen_yx");
	TH2D *hNegKaonPhivsPt_Gen = (TH2D *)hNegKaonPhivsEtavsPt_Gen->Project3D("hNegKaonPhivsPt_Gen_zx");
	TH2D *hMthNegKaonEtavsPt_Gen = (TH2D *)hMthNegKaonPhivsEtavsPt_Gen->Project3D("hMthNegKaonEtavsPt_Gen_yx");
	TH2D *hMthNegKaonPhivsPt_Gen = (TH2D *)hMthNegKaonPhivsEtavsPt_Gen->Project3D("hMthNegKaonPhivsPt_Gen_zx");

	TH2D *hNegKaonPhivsEta_Gen    = (TH2D *)hNegKaonPhivsEtavsPt_Gen->Project3D("hNegKaonPhivsEta_Gen_zy");
	TH2D *hMthNegKaonPhivsEta_Gen = (TH2D *)hMthNegKaonPhivsEtavsPt_Gen->Project3D("hMthNegKaonPhivsEta_Gen_zy");
	TH1D *hNegKaonPhi_Gen         = (TH1D *)hNegKaonPhivsEtavsPt_Gen->ProjectionZ("hNegKaonPhi_Gen");
	TH1D *hMthNegKaonPhi_Gen      = (TH1D *)hMthNegKaonPhivsEtavsPt_Gen->ProjectionZ("hMthNegKaonPhi_Gen");

	TH2D *hMthNegKaonEtavsPt  = (TH2D *)hMthNegKaonPhivsEtavsPt->Project3D("hMthNegKaonEtavsPt_yx");
	TH2D *hMthNegKaonPhivsPt  = (TH2D *)hMthNegKaonPhivsEtavsPt->Project3D("hMthNegKaonPhivsPt_zx");

	TH2D *hMthNegKaonPhivsEta  = (TH2D *)hMthNegKaonPhivsEtavsPt->Project3D("hMthNegKaonPhivsEta_zy");
	TH1D *hMthNegKaonPhi       = (TH1D *)hMthNegKaonPhivsEtavsPt->ProjectionZ("hMthNegKaonPhi");

	Int_t mEtaBinLow = 1;
	Int_t mEtaBinMid = hMthPosKaonPhivsEtavsPt->GetYaxis()->FindBin(0.);
	Int_t mEtaBinHi  = hMthPosKaonPhivsEtavsPt->GetNbinsY();
	TH1D *hMthPosKaonPhi_NegEta   = (TH1D *)hMthPosKaonPhivsEtavsPt->ProjectionZ("hMthPosKaonPhi_NegEta", 0, -1, mEtaBinLow, mEtaBinMid);
	TH1D *hMthPosKaonPhi_PosEta   = (TH1D *)hMthPosKaonPhivsEtavsPt->ProjectionZ("hMthPosKaonPhi_PosEta", 0, -1, mEtaBinMid, mEtaBinHi);
	TH1D *hMthNegKaonPhi_NegEta   = (TH1D *)hMthNegKaonPhivsEtavsPt->ProjectionZ("hMthNegKaonPhi_NegEta", 0, -1, mEtaBinLow, mEtaBinMid);
	TH1D *hMthNegKaonPhi_PosEta   = (TH1D *)hMthNegKaonPhivsEtavsPt->ProjectionZ("hMthNegKaonPhi_PosEta", 0, -1, mEtaBinMid, mEtaBinHi);

	TH2D *hPosKaon2DMthEff_EtavsPt = (TH2D *)hMthPosKaonEtavsPt_Gen->Clone("hPosKaon2DMthEff_EtavsPt");
	hPosKaon2DMthEff_EtavsPt->Divide(hMthPosKaonEtavsPt_Gen, hPosKaonEtavsPt_Gen, 1, 1, "B");

	TH2D *hPosKaon2DMthEff_PhivsPt = (TH2D *)hMthPosKaonPhivsPt_Gen->Clone("hPosKaon2DMthEff_PhivsPt");
	hPosKaon2DMthEff_PhivsPt->Divide(hMthPosKaonPhivsPt_Gen, hPosKaonPhivsPt_Gen, 1, 1, "B");

	TH2D *hPosKaon2DMthEff_PhivsEta = (TH2D *)hMthPosKaonPhivsEta_Gen->Clone("hPosKaon2DMthEff_PhivsEta");
	hPosKaon2DMthEff_PhivsEta->Divide(hMthPosKaonPhivsEta_Gen, hPosKaonPhivsEta_Gen, 1, 1, "B");

	TH1D *hPosKaonMthEffvsPhi = (TH1D *)hMthPosKaonPhi_Gen->Clone("hPosKaonMthEffvsPhi");
	hPosKaonMthEffvsPhi->Divide(hMthPosKaonPhi_Gen, hPosKaonPhi_Gen, 1, 1, "B");

	TH2D *hNegKaon2DMthEff_EtavsPt = (TH2D *)hMthNegKaonEtavsPt_Gen->Clone("hNegKaon2DMthEff_EtavsPt");
	hNegKaon2DMthEff_EtavsPt->Divide(hMthNegKaonEtavsPt_Gen, hNegKaonEtavsPt_Gen, 1, 1, "B");

	TH2D *hNegKaon2DMthEff_PhivsPt = (TH2D *)hMthNegKaonPhivsPt_Gen->Clone("hNegKaon2DMthEff_PhivsPt");
	hNegKaon2DMthEff_PhivsPt->Divide(hMthNegKaonPhivsPt_Gen, hNegKaonPhivsPt_Gen, 1, 1, "B");

	TH2D *hNegKaon2DMthEff_PhivsEta = (TH2D *)hMthNegKaonPhivsEta_Gen->Clone("hNegKaon2DMthEff_PhivsEta");
	hNegKaon2DMthEff_PhivsEta->Divide(hMthNegKaonPhivsEta_Gen, hNegKaonPhivsEta_Gen, 1, 1, "B");

	TH1D *hNegKaonMthEffvsPhi = (TH1D *)hMthNegKaonPhi_Gen->Clone("hNegKaonMthEffvsPhi");
	hNegKaonMthEffvsPhi->Divide(hMthNegKaonPhi_Gen, hNegKaonPhi_Gen, 1, 1, "B");

	TH1D *hKaonMthEffRatiovsPhi = (TH1D *)hPosKaonMthEffvsPhi->Clone("hKaonMthEffRatiovsPhi");
	hKaonMthEffRatiovsPhi->Divide(hPosKaonMthEffvsPhi, hNegKaonMthEffvsPhi, 1, 1);

	TCanvas* c1 = new TCanvas("c1", "c1", 0, 0, 800, 600);
	c1->Divide(2, 2);

	Double_t xPos = 0.15;
	Double_t yPos = 0.84;

	c1->cd(1);
	gPad->SetLogy(0);
	gPad->SetLogz(0);
	hPosKaon2DMthEff_EtavsPt->GetYaxis()->SetRangeUser(-4+mTinyNum, 4-mTinyNum);
	hPosKaon2DMthEff_EtavsPt->Draw("colz");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency (K^{+})", 22, 0.06, 4);
	drawLatex(xPos, yPos, "(a)", 22, 0.08, 1);
	c1->cd(2);
	gPad->SetLogy(0);
	gPad->SetLogz(0);
	hPosKaon2DMthEff_PhivsPt->Draw("colz");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency (K^{+})", 22, 0.06, 4);
	drawLatex(xPos, yPos, "(b)", 22, 0.08, 1);
	c1->cd(3);
	gPad->SetLogy(0);
	gPad->SetLogz(0);
	hNegKaon2DMthEff_EtavsPt->GetYaxis()->SetRangeUser(-4+mTinyNum, 4-mTinyNum);
	hNegKaon2DMthEff_EtavsPt->Draw("colz");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency (K^{-})", 22, 0.06, 4);
	drawLatex(xPos, yPos, "(c)", 22, 0.08, 1);
	c1->cd(4);
	gPad->SetLogy(0);
	gPad->SetLogz(0);
	hNegKaon2DMthEff_PhivsPt->Draw("colz");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency (K^{-})", 22, 0.06, 4);
	drawLatex(xPos, yPos, "(d)", 22, 0.08, 1);
	c1->SaveAs(Form("%s/2DMthEff.png", dir.Data()));
	if(mStorePDF) c1->SaveAs(Form("%s/2DMthEff.pdf", dir.Data()));

	TLegend* leg = new TLegend(0.18, 0.18, 0.36, 0.4);
	leg->SetFillStyle(0);
	leg->SetTextSize(0.1);

	c1->cd(1);
	gPad->SetLogy(0);
	gPad->SetLogz(0);
	hPosKaon2DMthEff_PhivsEta->GetXaxis()->SetRangeUser(-4+mTinyNum, 4-mTinyNum);
	hPosKaon2DMthEff_PhivsEta->Draw("colz");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency (K^{+})", 22, 0.06, 4);
	drawLatex(0.48, 0.16, "(a)", 22, 0.08, 1);
	c1->cd(2);
	gPad->SetLogy(0);
	gPad->SetLogz(0);
	hNegKaon2DMthEff_PhivsEta->GetXaxis()->SetRangeUser(-4+mTinyNum, 4-mTinyNum);
	hNegKaon2DMthEff_PhivsEta->Draw("colz");
	drawLatex(0.28, 0.945, "Reconstruted Efficiency (K^{-})", 22, 0.06, 4);
	drawLatex(0.48, 0.16, "(b)", 22, 0.08, 1);
	c1->cd(3);
	gPad->SetLogy(0);
	gPad->SetLogz(0);
	setHisto(hPosKaonMthEffvsPhi, 20, 0.5, 1, 1, 2);
	setHisto(hNegKaonMthEffvsPhi, 24, 0.5, 4, 4, 2);
	hPosKaonMthEffvsPhi->GetYaxis()->SetTitle("Reconstruted Efficiency");
	if(filename.EqualTo("LowMassGammaGamma")) hPosKaonMthEffvsPhi->GetYaxis()->SetRangeUser(0.45, 0.65);
	else hPosKaonMthEffvsPhi->GetYaxis()->SetRangeUser(0.0, 0.02);
	hPosKaonMthEffvsPhi->Draw("histe");
	hNegKaonMthEffvsPhi->Draw("histesame");
	leg->AddEntry(hPosKaonMthEffvsPhi, "K^{+}", "l");
	leg->AddEntry(hNegKaonMthEffvsPhi, "K^{-}", "l");
	leg->Draw("same");
	drawLatex(0.48, 0.16, "(c)", 22, 0.08, 1);
	c1->cd(4);
	gPad->SetLogy(0);
	gPad->SetLogz(0);
	setHisto(hKaonMthEffRatiovsPhi, 20, 0.8, 1, 1, 2);
	hKaonMthEffRatiovsPhi->GetYaxis()->SetRangeUser(0.8, 1.2);
	hKaonMthEffRatiovsPhi->GetYaxis()->SetTitle("K^{+}/K^{-} Efficiency Ratio");
	hKaonMthEffRatiovsPhi->Fit("pol0", "", "", -TMath::Pi(), TMath::Pi());
	hKaonMthEffRatiovsPhi->Draw("psame");
	drawLatex(0.48, 0.16, "(d)", 22, 0.08, 1);
	c1->SaveAs(Form("%s/2DMthEff_PhivsEta.png", dir.Data()));
	if(mStorePDF) c1->SaveAs(Form("%s/2DMthEff_PhivsEta.pdf", dir.Data()));

	// if(ipad%nPads != 0) pdfAction(c2, ps);

	// ps->On();
	// ps->Close();

	cout << "End of program !" << endl;
}
