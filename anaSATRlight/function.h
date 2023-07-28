//TH1D *rebHisto(TH1D *oldHisto, TString name, int nBinsX, double *BinX, TString NorX = "X");
//TH2D *rebHisto(TH2D *oldHisto, TString name, int nBinsX, double *BinX, int nBinsY, double *BinY, TString RebXY="XY", TString NorXY="Y");
//TH1D *calGeoMean(TH1D *hNum1, TH1D *hNum2, TString name);
//TH2D *calGeoMean(TH2D *hNum1, TH2D *hNum2, TString name);
//TH1D *calRatio(TH1D *hNum, TH1D *hDen, TString name, TString Correlation = "NOCORR");
//TH2D *calRatio(TH2D *hNum, TH2D *hDen, TString name, TString Correlation = "NOCORR");
//pair<double, double> calRatio(double Num, double NumErr, double Den, double DenErr, TString Correlation = "NOCORR");
//TH1D *calMult(TH1D *hLGeo, TH1D *hAcc, TString name);
//TH2D *calMult(TH2D *hLGeo, TH2D *hAcc, TString name);
//pair<double, double> calMult(double Num1, double NumErr1, double Num2, double NumErr2);
//TH2D* histo(TString name, double xlow, double xup, double ylow, double yup, TString xTitle, TString yTitle);
//TH2D* histo(TString name, int nBinsX, double xlow, double xup, int nBinsY, double ylow, double yup, TString xTitle, TString yTitle);
//TMarker* drawMarker(double x, double y, int markerStyle, double markerSize, int markerColor);
//void drawOverSizeErr(TGraphErrors *gr, double scale, double arrowSize, double xmin, double xmax, TString opt="pzsame", double endErr=0, double yTh = 1.e-15);
//void drawOverSizeSysErr(TGraphErrors *gr, double scale, double bandFrac, double arrowFrac, int bandWidth, int bandColor, double arrowSize, double xmin, double xmax, double yTh=1.e-15);
//TArrow*  drawArrow(double x1, double y1, double x2, double y2, double arrowSize=0.05, TString arrowOption="|>", int lineColor=1, int lineWidth=1, int lineStyle=1);
//TBox*  drawBox(double x, double y, double xErrLow, double yErrLow, double xErrHi, double yErrHi, int color, int lineWidth=1, int fillStyle=0, double colorAlpha=1);
//TLatex* drawLatex(double x, double y, TString text, int textFont, double textSize, int colorIndex, double textAngle=0);
//TLine* drawLine(double xlow,double ylow, double xup, double yup, int lineColor, int lineWidth,int lineStyle);
//void drawLines(double xlow,double ylow, double xup, double yup, int lineColor, int lineWidth,int lineStyle);
//void setHisto(TH1D *h,int markerStyle, double markerSize, int markerColor,int lineColor, int lineWidth=1);
//void setProfile(TProfile *h,int markerStyle, double markerSize, int markerColor,int lineColor, int lineWidth=1);
//void setGraph(TGraph *g, int markerStyle, double markerSize, int markerColor, int lineColor, int lineWidth=1);
//void setGraph(TGraphErrors *g, int markerStyle, double markerSize, int markerColor, int lineColor, int lineWidth=1);
//void setGraph(TGraphAsymmErrors *g, int markerStyle, double markerSize, int markerColor, int lineColor, int lineWidth=1);
//void setFun(TF1 *f, int lineColor, int lineWidth=2, int lineStyle=1);
//void setLegend(TLegend *leg,double xlow, double ylow, double xup, double yup, double textSize);
//void setPad(double left, double right, double top, double bottom);
//void clearPad(TCanvas *c, int nPads);
//void pdfAction(TCanvas *c, TPDF *ps, Bool_t isClosePDF);

const double maxDiff = 1.e-10;

//__________________________________________________
TH1D *rebHisto(TH1D *oldHisto, TString name, int nBinsX, const double *BinX, TString NorX = "X")
{
	TH1D *newHisto = new TH1D(name.Data(),name.Data(),nBinsX,BinX);

	int oldNBins = oldHisto->GetNbinsX();
	int newNBins = newHisto->GetNbinsX();
	
	if(newNBins>oldNBins)
	{
		cout<<" Failed to rebin \""<<oldHisto->GetName()<<"\"! --- NBinsX are more than original histogram!"<<endl;
		return NULL;
	}

	newHisto->GetXaxis()->SetTitle(oldHisto->GetXaxis()->GetTitle());

	Bool_t NorFlag = kTRUE;
	double oldBinWidth = 1.e-3;
	for(int i=0; i<oldNBins; i++)
	{
		if(oldBinWidth>oldHisto->GetBinWidth(i+1)) oldBinWidth = oldHisto->GetBinWidth(i+1);
	}
	
	for(int i=0;i<newNBins;i++)
	{
		double newBinCenter = newHisto->GetBinCenter(i+1);
		double newBinWidth  = newHisto->GetBinWidth(i+1);
		double newBinLow    = newBinCenter-newBinWidth/2.+oldBinWidth/2.;
		double newBinHi     = newBinCenter+newBinWidth/2.+oldBinWidth/2.;

		int oldBinLow       = oldHisto->FindBin(newBinLow);
		int oldBinHi        = oldHisto->FindBin(newBinHi)-1;
		int oldBinDiff      = oldBinHi-oldBinLow+1;

		if(
				TMath::Abs(newBinCenter-newBinWidth/2.-oldHisto->GetXaxis()->GetBinLowEdge(oldBinLow)) > maxDiff
				|| TMath::Abs(newBinCenter+newBinWidth/2.-oldHisto->GetXaxis()->GetBinUpEdge(oldBinHi)) > maxDiff
		  ){
			cout<<" Failed to rebin \""<<oldHisto->GetName()<<"\"! --- X-axis bin boundary is different from original histogram!"<<endl;
			return NULL;
		}

		double newBinContent  = oldHisto->Integral(oldBinLow,oldBinHi);
		double newBinErr = 0.;
		
		for(int j=oldBinLow;j<=oldBinHi;j++)
		{
			newBinErr += pow(oldHisto->GetBinError(j),2);
		}
		
		newBinErr = sqrt(newBinErr);
		
		if(NorX.CompareTo("X")==0) // if no difference
		{
			newHisto->SetBinContent(i+1, newBinContent/oldBinDiff);
			newHisto->SetBinError(  i+1, newBinErr/oldBinDiff    );
		}
		else
		{
			newHisto->SetBinContent(i+1, newBinContent);
			newHisto->SetBinError(  i+1, newBinErr    );
			NorFlag = kFALSE;
		}
	}

	if(!NorFlag)
	{
		cout<<"The \""<<newHisto->GetName()<<"\" is not normalized!"<<endl;
		cout<<"If you want to normalize the \""<<newHisto->GetName()<<"\", the NorXY argument should be \"X\"or\"\" !"<<endl;
	}
	return newHisto;
}

//__________________________________________________
TH2D *rebHisto(TH2D *oldHisto, TString name, int nBinsX, double *BinX, int nBinsY, double *BinY, TString RebXY="XY", TString NorXY="Y")
{
	TH2D *newHisto;
	if(RebXY.CompareTo("X")==0){//rebinX
		nBinsY = oldHisto->GetNbinsY();
		double LowY = oldHisto->GetYaxis()->GetBinLowEdge(1);	
		double UpY = oldHisto->GetYaxis()->GetBinUpEdge(nBinsY);
		newHisto = new TH2D(name.Data(),name.Data(),nBinsX,BinX,nBinsY,LowY,UpY);
	}
	else if(RebXY.CompareTo("Y")==0 || RebXY.CompareTo("")==0){//rebinY
		nBinsX = oldHisto->GetNbinsX();
		double LowX = oldHisto->GetXaxis()->GetBinLowEdge(1);
		double UpX = oldHisto->GetXaxis()->GetBinUpEdge(nBinsX);
		newHisto = new TH2D(name.Data(),name.Data(),nBinsX,LowX,UpX,nBinsY,BinY);
	}
	else if(RebXY.CompareTo("XY")==0){//rebinXY
		newHisto = new TH2D(name.Data(),name.Data(),nBinsX,BinX,nBinsY,BinY);
	}
	else{
		cout<<"Failed to rebin \""<<oldHisto->GetName()<<"\" --- The RebXY parameter should be \"X\", \"Y\"or\"\", \"XY\"!"<<endl;
		return NULL;
	}

	newHisto->GetXaxis()->SetTitle(oldHisto->GetXaxis()->GetTitle());
	newHisto->GetYaxis()->SetTitle(oldHisto->GetYaxis()->GetTitle());

	int oldNBinsX = oldHisto->GetNbinsX();
	int oldNBinsY = oldHisto->GetNbinsY();
	int newNBinsX = newHisto->GetNbinsX();
	int newNBinsY = newHisto->GetNbinsY();
	if(newNBinsX>oldNBinsX){
		cout<<" Failed to rebin \""<<oldHisto->GetName()<<"\"! --- NBinsX are more than original histogram!"<<endl;
		return NULL;
	}
	if(newNBinsY>oldNBinsY){
		cout<<" Failed to rebin \""<<oldHisto->GetName()<<"\"! --- NBinsY are more than original histogram!"<<endl;
		return NULL;
	}

	Bool_t NorFlag = kTRUE;
	double oldBinWidthX = 1.e-3;
	for(int i=0; i<oldNBinsX; i++){
		if(oldBinWidthX>oldHisto->GetXaxis()->GetBinWidth(i+1))
			oldBinWidthX = oldHisto->GetXaxis()->GetBinWidth(i+1);
	}
	double oldBinWidthY = 1.e-3;
	for(int i=0; i<oldNBinsY; i++){
		if(oldBinWidthY>oldHisto->GetYaxis()->GetBinWidth(i+1))
			oldBinWidthY = oldHisto->GetYaxis()->GetBinWidth(i+1);
	}
	for(int i=0;i<newNBinsX;i++){
		for(int j=0;j<newNBinsY;j++){
			double newBinCenterX = newHisto->GetXaxis()->GetBinCenter(i+1);
			double newBinWidthX = newHisto->GetXaxis()->GetBinWidth(i+1);
			double newBinLowX = newBinCenterX-newBinWidthX/2.+oldBinWidthX/2.;
			double newBinHiX  = newBinCenterX+newBinWidthX/2.+oldBinWidthX/2.;
			double newBinCenterY = newHisto->GetYaxis()->GetBinCenter(j+1);
			double newBinWidthY = newHisto->GetYaxis()->GetBinWidth(j+1);
			double newBinLowY = newBinCenterY-newBinWidthY/2.+oldBinWidthY/2.;
			double newBinHiY  = newBinCenterY+newBinWidthY/2.+oldBinWidthY/2.;

			int oldBinLowX = oldHisto->GetXaxis()->FindBin(newBinLowX);
			int oldBinHiX  = oldHisto->GetXaxis()->FindBin(newBinHiX)-1;
			int oldBinDiffX = oldBinHiX-oldBinLowX+1;
			int oldBinLowY = oldHisto->GetYaxis()->FindBin(newBinLowY);
			int oldBinHiY  = oldHisto->GetYaxis()->FindBin(newBinHiY)-1;
			int oldBinDiffY = oldBinHiY-oldBinLowY+1;

			if(
					TMath::Abs(newBinCenterX-newBinWidthX/2.-oldHisto->GetXaxis()->GetBinLowEdge(oldBinLowX)) > maxDiff 
					|| TMath::Abs(newBinCenterX+newBinWidthX/2.-oldHisto->GetXaxis()->GetBinUpEdge(oldBinHiX)) > maxDiff 
			  ){
				cout<<" Failed to rebin \""<<oldHisto->GetName()<<"\"! --- X-axis bin boundary is different from original histogram!"<<endl;
				return NULL;
			}

			if(
					TMath::Abs(newBinCenterY-newBinWidthY/2.-oldHisto->GetYaxis()->GetBinLowEdge(oldBinLowY)) > maxDiff 
					|| TMath::Abs(newBinCenterY+newBinWidthY/2.-oldHisto->GetYaxis()->GetBinUpEdge(oldBinHiY)) > maxDiff 
			  ){
				cout<<" Failed to rebin \""<<oldHisto->GetName()<<"\"! --- Y-axis bin boundary is different from original histogram!"<<endl;
				return NULL;
			}

			double newBinContent  = oldHisto->Integral(oldBinLowX,oldBinHiX,oldBinLowY,oldBinHiY);
			double newBinErr = 0.;
			for(int binx=oldBinLowX;binx<=oldBinHiX;binx++){
				for(int biny=oldBinLowY;biny<=oldBinHiY;biny++){
					newBinErr += pow(oldHisto->GetBinError(binx,biny),2.);
				}
			}
			newBinErr = sqrt(newBinErr);

			if(NorXY.CompareTo("X")==0){
				newHisto->SetBinContent(i+1,j+1,newBinContent/oldBinDiffX);
				newHisto->SetBinError(i+1,j+1,newBinErr/oldBinDiffX);
			}
			else if(NorXY.CompareTo("Y")==0 || NorXY.CompareTo("")==0){
				newHisto->SetBinContent(i+1,j+1,newBinContent/oldBinDiffY);
				newHisto->SetBinError(i+1,j+1,newBinErr/oldBinDiffY);
			}
			else if(NorXY.CompareTo("XY")==0){
				newHisto->SetBinContent(i+1,j+1,newBinContent/oldBinDiffX/oldBinDiffY);
				newHisto->SetBinError(i+1,j+1,newBinErr/oldBinDiffX/oldBinDiffY);
			}
			else{
				newHisto->SetBinContent(i+1,j+1,newBinContent);
				newHisto->SetBinError(i+1,j+1,newBinErr);
				NorFlag = kFALSE;
			}
		}
	}	

	if(!NorFlag){
		cout<<"The \""<<newHisto->GetName()<<"\" is not normalized!"<<endl;
		cout<<"If you want to normalize the \""<<newHisto->GetName()<<"\", the NorXY argument should be \"X\", \"Y\"or\"\", \"XY\"!"<<endl;
	}
	return newHisto;
}

//__________________________________________________
TH1D *calGeoMean(TH1D *hNum1, TH1D *hNum2, TString name)
{
	if(
			hNum1->GetNbinsX() != hNum2->GetNbinsX()
	  ){
		cout<<"Failed to calculate the Geometry Mean: The bin numbers of \""<<hNum1->GetName()<<"\" and \""<<hNum2->GetName()<<"\""<<" are not the same !"<<endl;
		return NULL;
	}

	int nBinsX = hNum1->GetNbinsX();

	for(int i=0;i<nBinsX;i++){
		if(
				TMath::Abs(hNum1->GetXaxis()->GetBinLowEdge(i+1) - hNum2->GetXaxis()->GetBinLowEdge(i+1)) > maxDiff
				|| TMath::Abs(hNum1->GetXaxis()->GetBinUpEdge(i+1) - hNum2->GetXaxis()->GetBinUpEdge(i+1)) > maxDiff
		  ){
			cout<<"Failed to calculate the Geometry Mean: The X-axis bin boundaries of \""<<hNum1->GetName()<<"\" and \""<<hNum2->GetName()<<"\""<<" are not the same !"<<endl;
			return NULL;
		}
	}

	TH1D *hGeo = (TH1D *)hNum1->Clone(name.Data());
	hGeo->SetTitle(name.Data());
	hGeo->GetXaxis()->SetTitle(hNum1->GetXaxis()->GetTitle());
	hGeo->Reset();
	for(int i=0;i<nBinsX;i++){
		double nNum1 = hNum1->GetBinContent(i+1);
		double nNumErr1 = hNum1->GetBinError(i+1);
		double nNum2 = hNum2->GetBinContent(i+1);
		double nNumErr2 = hNum2->GetBinError(i+1);
		double nGeoMean = 0.;
		double nGeoMeanErr = 0.;
		if(nNum1*nNum2>0.){
			nGeoMean = 2*sqrt(nNum1*nNum2);
			nGeoMeanErr = nGeoMean*sqrt(pow(nNumErr1/nNum1,2)/4.+pow(nNumErr2/nNum2,2)/4.);
		}
		hGeo->SetBinContent(i+1,nGeoMean);
		hGeo->SetBinError(i+1,nGeoMeanErr);
	}
	return hGeo;
}

//__________________________________________________
TH2D *calGeoMean(TH2D *hNum1, TH2D *hNum2, TString name)
{
	if(
			hNum1->GetNbinsX() != hNum2->GetNbinsX()
			|| hNum1->GetNbinsY() != hNum2->GetNbinsY()
	  ){
		cout<<"Failed to calculate the Geometry Mean: The bin numbers of \""<<hNum1->GetName()<<"\" and \""<<hNum2->GetName()<<"\""<<" are not the same !"<<endl;
		return NULL;
	}

	int nBinsX = hNum1->GetNbinsX();
	int nBinsY = hNum1->GetNbinsY();

	for(int i=0;i<nBinsX;i++){
		if(
				TMath::Abs(hNum1->GetXaxis()->GetBinLowEdge(i+1) - hNum2->GetXaxis()->GetBinLowEdge(i+1)) > maxDiff
				|| TMath::Abs(hNum1->GetXaxis()->GetBinUpEdge(i+1) - hNum2->GetXaxis()->GetBinUpEdge(i+1)) > maxDiff
		  ){
			cout<<"Failed to calculate the Geometry Mean: The X-axis bin boundaries of \""<<hNum1->GetName()<<"\" and \""<<hNum2->GetName()<<"\""<<" are not the same !"<<endl;
			return NULL;
		}
	}

	for(int i=0;i<nBinsY;i++){
		if(
				TMath::Abs(hNum1->GetYaxis()->GetBinLowEdge(i+1) - hNum2->GetYaxis()->GetBinLowEdge(i+1)) > maxDiff
				|| TMath::Abs(hNum1->GetYaxis()->GetBinUpEdge(i+1) - hNum2->GetYaxis()->GetBinUpEdge(i+1)) > maxDiff
		  ){
			cout<<"Failed to calculate the Geometry Mean: The Y-axis bin boundaries of \""<<hNum1->GetName()<<"\" and \""<<hNum2->GetName()<<"\""<<" are not the same !"<<endl;
			return NULL;
		}
	}

	TH2D *hGeo = (TH2D *)hNum1->Clone(name.Data());
	hGeo->SetTitle(name.Data());
	hGeo->GetXaxis()->SetTitle(hNum1->GetXaxis()->GetTitle());
	hGeo->GetYaxis()->SetTitle(hNum1->GetYaxis()->GetTitle());
	hGeo->Reset();
	for(int i=0;i<nBinsX;i++){
		for(int j=0;j<nBinsY;j++){
			double nNum1 = hNum1->GetBinContent(i+1,j+1);
			double nNumErr1 = hNum1->GetBinError(i+1,j+1);
			double nNum2 = hNum2->GetBinContent(i+1,j+1);
			double nNumErr2 = hNum2->GetBinError(i+1,j+1);
			double nGeoMean = 0.;
			double nGeoMeanErr = 0.;
			if(nNum1*nNum2>0.){
				nGeoMean = 2*sqrt(nNum1*nNum2);
				nGeoMeanErr = nGeoMean*sqrt(pow(nNumErr1/nNum1,2)/4.+pow(nNumErr2/nNum2,2)/4.);
			}
			hGeo->SetBinContent(i+1,j+1,nGeoMean);
			hGeo->SetBinError(i+1,j+1,nGeoMeanErr);
		}
	}
	return hGeo;
}

//__________________________________________________
TH1D *calRatio(TH1D *hNum, TH1D *hDen, TString name, TString Correlation = "NOCORR")
{
	if(
			hNum->GetNbinsX() != hDen->GetNbinsX()
	  ){
		cout<<"Failed to calculate the Ratio: The bin numbers of \""<<hNum->GetName()<<"\" and \""<<hDen->GetName()<<"\""<<" are not the same !"<<endl;
		return NULL;
	}

	int nBinsX = hNum->GetNbinsX();

	for(int i=0;i<nBinsX;i++){
		if(
				TMath::Abs(hNum->GetXaxis()->GetBinLowEdge(i+1) - hDen->GetXaxis()->GetBinLowEdge(i+1)) > maxDiff
				|| TMath::Abs(hNum->GetXaxis()->GetBinUpEdge(i+1) - hDen->GetXaxis()->GetBinUpEdge(i+1)) > maxDiff
		  ){
			cout<<"Failed to calculate the Ratio: The X-axis bin boundaries of \""<<hNum->GetName()<<"\" and \""<<hDen->GetName()<<"\""<<" are not the same !"<<endl;
			return NULL;
		}
	}

	TH1D *hRatio = (TH1D *)hNum->Clone(name.Data());
	hRatio->SetTitle(name.Data());
	hRatio->GetXaxis()->SetTitle(hNum->GetXaxis()->GetTitle());
	hRatio->Reset();
	for(int i=0;i<nBinsX;i++){
		double nNum = hNum->GetBinContent(i+1);
		double nNumErr = hNum->GetBinError(i+1);
		double nDen = hDen->GetBinContent(i+1);
		double nDenErr = hDen->GetBinError(i+1);
		double ratio = 0.;
		double ratioErr = 0.;
		if(TMath::Abs(nDen)>0){
			ratio = nNum/nDen;
			if(Correlation.CompareTo("NOCORR")==0){
				ratioErr = sqrt( (pow(nDen*nNumErr,2)+pow(nNum*nDenErr,2))/pow(nDen,4) );
			}
			else if(Correlation.CompareTo("CORR")==0){ //using binomial statistics, here the nNum & nDen must be large equal 0, ratio must be less equal than 1 and larger equal than 0
				if(nDen<0){
					cout<<"The bin content should be larger than 0 !"<<endl;
					return NULL;
				}
				if(ratio>=0 && ratio<=1){
					ratioErr = TMath::Abs( ((1.-2.*ratio)*pow(nNumErr,2)+pow(ratio*nDenErr,2))/pow(nDen,2) );
					ratioErr = sqrt(ratioErr);
				}
				else{
					cout<<"The ratio should be less equal than 1 and larger euqal than 0 !"<<endl;
					return NULL;
				}
			}
			else{
				cout<<"The option is wrong ! Please input \"NOCORR\" or \"CORR\" !"<<endl;
				return NULL;
			}
		}
		hRatio->SetBinContent(i+1,ratio);
		hRatio->SetBinError(i+1,ratioErr);
	}
	return hRatio;
};

//__________________________________________________
TH2D *calRatio(TH2D *hNum, TH2D *hDen, TString name, TString Correlation = "NOCORR")
{
	if(
			hNum->GetNbinsX() != hDen->GetNbinsX()
			|| hNum->GetNbinsY() != hDen->GetNbinsY()
	  ){
		cout<<"Failed to calculate the Ratio: The bin numbers of \""<<hNum->GetName()<<"\" and \""<<hDen->GetName()<<"\""<<" are not the same !"<<endl;
		return NULL;
	}

	int nBinsX = hNum->GetNbinsX();
	int nBinsY = hNum->GetNbinsY();

	for(int i=0;i<nBinsX;i++){
		if(
				TMath::Abs(hNum->GetXaxis()->GetBinLowEdge(i+1) - hDen->GetXaxis()->GetBinLowEdge(i+1)) > maxDiff
				|| TMath::Abs(hNum->GetXaxis()->GetBinUpEdge(i+1) - hDen->GetXaxis()->GetBinUpEdge(i+1)) > maxDiff
		  ){
			cout<<"Failed to calculate the Ratio: The X-axis bin boundaries of \""<<hNum->GetName()<<"\" and \""<<hDen->GetName()<<"\""<<" are not the same !"<<endl;
			return NULL;
		}
	}

	for(int i=0;i<nBinsY;i++){
		if(
				TMath::Abs(hNum->GetYaxis()->GetBinLowEdge(i+1) - hDen->GetYaxis()->GetBinLowEdge(i+1)) > maxDiff
				|| TMath::Abs(hNum->GetYaxis()->GetBinUpEdge(i+1) - hDen->GetYaxis()->GetBinUpEdge(i+1)) > maxDiff
		  ){
			cout<<"Failed to calculate the Ratio: The Y-axis bin boundaries of \""<<hNum->GetName()<<"\" and \""<<hDen->GetName()<<"\""<<" are not the same !"<<endl;
			return NULL;
		}
	}

	TH2D *hRatio = (TH2D *)hNum->Clone(name.Data());
	hRatio->SetTitle(name.Data());
	hRatio->GetXaxis()->SetTitle(hNum->GetXaxis()->GetTitle());
	hRatio->GetYaxis()->SetTitle(hNum->GetYaxis()->GetTitle());
	hRatio->Reset();
	for(int i=0;i<nBinsX;i++){
		for(int j=0;j<nBinsY;j++){
			double nNum = hNum->GetBinContent(i+1,j+1);
			double nNumErr = hNum->GetBinError(i+1,j+1);
			double nDen = hDen->GetBinContent(i+1,j+1);
			double nDenErr = hDen->GetBinError(i+1,j+1);
			double ratio = 0.;
			double ratioErr = 0.;
			if(TMath::Abs(nDen)>0){
				ratio = nNum/nDen;
				if(Correlation.CompareTo("NOCORR")==0){
					ratioErr = sqrt( (pow(nDen*nNumErr,2)+pow(nNum*nDenErr,2))/pow(nDen,4) );
				}
				else if(Correlation.CompareTo("CORR")==0){ //using binomial statistics, here the nNum & nDen must be large than 0, ratio must be less equal than 1 and larger equal than 0
					if(nDen<0){
						cout<<"The bin content should be larger than 0 !"<<endl;
						return NULL;
					}
					if(ratio>=0 && ratio<=1){
						ratioErr = TMath::Abs( ((1.-2.*ratio)*pow(nNumErr,2)+pow(ratio*nDenErr,2))/pow(nDen,2) );
						ratioErr = sqrt(ratioErr);
					}
					else{
						cout<<"The ratio should be less equal than 1 and larger euqal than 0 !"<<endl;
						return NULL;
					}
				}
				else{
					cout<<"The option is wrong ! Please input \"NOCORR\" or \"CORR\" !"<<endl;
					return NULL;
				}
			}

			hRatio->SetBinContent(i+1,j+1,ratio);
			hRatio->SetBinError(i+1,j+1,ratioErr);
		}
	}
	return hRatio;
};

//__________________________________________________
pair<double, double> calRatio(double Num, double NumErr, double Den, double DenErr, TString Correlation = "NOCORR")
{
	double ratio = 0;
	double ratioErr = 0;
	pair<double, double> Quotients(0, 0);

	if(TMath::Abs(Den)>0){
		ratio = Num/Den;
		if(Correlation.EqualTo("NOCORR")){
			ratioErr = sqrt( (pow(Den*NumErr,2)+pow(Num*DenErr,2))/pow(Den,4) );
		}
		else if(Correlation.EqualTo("CORR")){ //using binomial statistics, here the Num & Den must be large equal 0, ratio must be less equal than 1 and larger equal than 0
			if(Den<0){
				cout<<"The denorminator should be larger than 0 !"<<endl;
				return Quotients;
			}
			if(ratio>=0 && ratio<=1){
				ratioErr = TMath::Abs( ((1.-2.*ratio)*pow(NumErr,2)+pow(ratio*DenErr,2))/pow(Den,2) );
				ratioErr = sqrt(ratioErr);
			}
			else{
				cout<<"The ratio should be less equal than 1 and larger euqal than 0 !"<<endl;
				return Quotients;
			}
		}
		else{
			cout<<"The option is wrong ! Please input \"NOCORR\" or \"CORR\" !"<<endl;
			return Quotients;
		}
	}
	else{
		cout<<"The denorminator is 0 !"<<endl;
		return Quotients;
	}

	Quotients.first = ratio;
	Quotients.second = ratioErr;

	return Quotients;
}

//__________________________________________________
TH1D *calMult(TH1D *hNum1, TH1D *hNum2, TString name)
{
	if(
			hNum1->GetNbinsX() != hNum2->GetNbinsX()
	  ){
		cout<<"Failed to calculate the Multiply: The bin numbers of \""<<hNum1->GetName()<<"\" and \""<<hNum2->GetName()<<"\""<<" are not the same!"<<endl;
		return NULL;
	}

	int nBinsX = hNum1->GetNbinsX();

	for(int i=0;i<nBinsX;i++){
		if(
				TMath::Abs(hNum1->GetXaxis()->GetBinLowEdge(i+1) - hNum2->GetXaxis()->GetBinLowEdge(i+1)) > maxDiff
				|| TMath::Abs(hNum1->GetXaxis()->GetBinUpEdge(i+1) - hNum2->GetXaxis()->GetBinUpEdge(i+1)) > maxDiff
		  ){
			cout<<"Failed to calculate the Multiply: The X-axis bin boundaries of \""<<hNum1->GetName()<<"\" and \""<<hNum2->GetName()<<"\""<<" are not the same !"<<endl;
			return NULL;
		}
	}

	TH1D *hMult = (TH1D *)hNum1->Clone(name.Data());
	hMult->SetTitle(name.Data());
	hMult->GetXaxis()->SetTitle(hNum1->GetXaxis()->GetTitle());
	hMult->Reset();
	for(int i=0;i<nBinsX;i++){
		double nNum1 = hNum1->GetBinContent(i+1);
		double nNumErr1 = hNum1->GetBinError(i+1);
		double nNum2 = hNum2->GetBinContent(i+1);
		double nNumErr2 = hNum2->GetBinError(i+1);
		double nMult = 0.; 
		double nMultErr = 0.;

		nMult = nNum1*nNum2;
		nMultErr = sqrt( pow(nNum2*nNumErr1,2)+pow(nNum1*nNumErr2,2) );

		hMult->SetBinContent(i+1,nMult);
		hMult->SetBinError(i+1,nMultErr);
	}
	return hMult;
};

//__________________________________________________
TH2D *calMult(TH2D *hNum1, TH2D *hNum2, TString name)
{
	if(
			hNum1->GetNbinsX() != hNum2->GetNbinsX()
			|| hNum1->GetNbinsY() != hNum2->GetNbinsY()
	  ){
		cout<<"Failed to calMult the Multiply: The bin numbers of \""<<hNum1->GetName()<<"\" and \""<<hNum2->GetName()<<"\""<<" are not the same !"<<endl;
		return NULL;
	}

	int nBinsX = hNum1->GetNbinsX();
	int nBinsY = hNum1->GetNbinsY();

	for(int i=0;i<nBinsX;i++){
		if(
				TMath::Abs(hNum1->GetXaxis()->GetBinLowEdge(i+1) - hNum2->GetXaxis()->GetBinLowEdge(i+1)) > maxDiff
				|| TMath::Abs(hNum1->GetXaxis()->GetBinUpEdge(i+1) - hNum2->GetXaxis()->GetBinUpEdge(i+1)) > maxDiff
		  ){
			cout<<"Failed to calculate the Multiply: The X-axis bin boundaries of \""<<hNum1->GetName()<<"\" and \""<<hNum2->GetName()<<"\""<<" are not the same !"<<endl;
			return NULL;
		}
	}

	for(int i=0;i<nBinsY;i++){
		if(
				TMath::Abs(hNum1->GetYaxis()->GetBinLowEdge(i+1) - hNum2->GetYaxis()->GetBinLowEdge(i+1)) > maxDiff
				|| TMath::Abs(hNum1->GetYaxis()->GetBinUpEdge(i+1) - hNum2->GetYaxis()->GetBinUpEdge(i+1)) > maxDiff
		  ){
			cout<<"Failed to calculate the Multiply: The Y-axis bin boundaries of \""<<hNum1->GetName()<<"\" and \""<<hNum2->GetName()<<"\""<<" are not the same !"<<endl;
			return NULL;
		}
	}

	TH2D *hMult = (TH2D *)hNum1->Clone(name.Data());
	hMult->SetTitle(name.Data());
	hMult->GetXaxis()->SetTitle(hNum1->GetXaxis()->GetTitle());
	hMult->GetYaxis()->SetTitle(hNum1->GetYaxis()->GetTitle());
	hMult->Reset();
	for(int i=0;i<nBinsX;i++){
		for(int j=0;j<nBinsY;j++){
			double nNum1 = hNum1->GetBinContent(i+1,j+1);
			double nNumErr1 = hNum1->GetBinError(i+1,j+1);
			double nNum2 = hNum2->GetBinContent(i+1,j+1);
			double nNumErr2 = hNum2->GetBinError(i+1,j+1);
			double nMult = 0.; 
			double nMultErr = 0.;

			nMult = nNum1*nNum2;
			nMultErr = sqrt( pow(nNum2*nNumErr1,2)+pow(nNum1*nNumErr2,2) );

			hMult->SetBinContent(i+1,j+1,nMult);
			hMult->SetBinError(i+1,j+1,nMultErr);
		}
	}
	return hMult;
};

//__________________________________________________________________________
pair<double, double> calMult(double Num1, double NumErr1, double Num2, double NumErr2)
{
	double Pro = Num1*Num2; 
	double ProErr = sqrt(pow(Num2*NumErr1,2)+pow(Num1*NumErr2,2));
	pair<double, double> Products(Pro, ProErr);
	return Products;
}

//__________________________________________________________________________
TMarker* drawMarker(double x, double y, int markerStyle, double markerSize, int markerColor){
	TMarker *mk = new TMarker(x,y,markerStyle);
	mk->SetNDC();
	mk->SetMarkerStyle(markerStyle);
	mk->SetMarkerSize(markerSize);
	mk->SetMarkerColor(markerColor);
	mk->Draw("psame");
	return mk;
}

//__________________________________________________________________________
void drawOverSizeErr(TGraphErrors *gr, double scale, double arrowSize, double xmin, double xmax, TString opt="pzsame", double endErr=0, double yTh = 1.e-15)
{
	Style_t sty = gr->GetMarkerStyle();
	Size_t  siz = gr->GetMarkerSize();
	Color_t col = gr->GetMarkerColor();
	Color_t lCol = gr->GetLineColor();
	Width_t lWid = gr->GetLineWidth();

	if (!opt.IsNull()) {

		int nPts = 0;
		for (int i=0;i<gr->GetN();i++) {
			double x = gr->GetX()[i];
			double y = gr->GetY()[i];
			if (x<xmin || x>xmax || y<yTh) {  // be careful for the yTh 
				continue;
			}
			else {
				nPts++;
			}
		}

		TGraphAsymmErrors *grClone = new TGraphAsymmErrors(nPts);
		grClone->SetMarkerStyle(sty);
		grClone->SetMarkerSize(siz);
		grClone->SetMarkerColor(col);
		grClone->SetLineColor(lCol);
		grClone->SetLineWidth(lWid);

		for (int i=0;i<gr->GetN();i++) {

			double x,y,ex,ey;
			gr->GetPoint(i,x,y);
			ex=gr->GetErrorX(i);
			ey=gr->GetErrorY(i);

			if (x<xmin || x>xmax || y<yTh) {  // be careful for the yTh 
				continue;
			}
			else { 
				if(ey>=y){
					TArrow *arY = new TArrow(x,y-0.8*scale*y,x,y-scale*y,arrowSize,"---------|>");
					arY->SetLineWidth(lWid);
					arY->SetLineColor(lCol);
					arY->SetFillColor(lCol);
					arY->DrawClone();
					arY->Delete();

					grClone->SetPoint(i, x, y);
					grClone->SetPointError(i, ex, ex, 0.8*scale*y, ey);
				}
				else{
					grClone->SetPoint(i, x, y);
					grClone->SetPointError(i, ex, ex, ey, ey);
				}
			}
		}

		grClone->DrawClone(opt.Data());
		grClone->Delete();
	}
	else {
		for(int i=0;i<gr->GetN();i++){

			double x,y,ex,ey;
			gr->GetPoint(i,x,y);
			ex=gr->GetErrorX(i);
			ey=gr->GetErrorY(i);

			if(x<xmin || x>xmax || y<yTh) continue;  // be careful for the yTh staff

			TMarker *mk = new TMarker(x,y,sty);
			mk->SetMarkerColor(col);
			mk->SetMarkerSize(siz);
			mk->SetMarkerStyle(sty);

			TArrow *arX,*arY;
			if(ey>=y){
				if(endErr>0){
					arY = new TArrow(x,y+ey,x,y-scale*y,arrowSize,"|---------|>");
				}
				else{
					arY = new TArrow(x,y+ey,x,y-scale*y,arrowSize,"---------|>");
				}
				arY->SetLineWidth(lWid);
				arY->SetLineColor(lCol);
				arY->SetFillColor(lCol);
				arY->DrawClone();
			}
			else{
				if(endErr>0){
					arY = new TArrow(x,y+ey,x,y-ey,arrowSize,"|---------|");
				}
				else{
					arY = new TArrow(x,y+ey,x,y-ey,arrowSize,"---------");
				}
				arY->SetLineWidth(lWid);
				arY->SetLineColor(lCol);
				arY->SetFillColor(lCol);
				arY->DrawClone();
			}

			double xlow = x-ex;
			double xhigh = x+ex;
			if(xlow<xmin) xlow = xmin;
			if(xhigh>xmax) xhigh = xmax;
			if(endErr>0){
				arX = new TArrow(xlow,y,xhigh,y,arrowSize,"|---------|");
			}
			else{
				arX = new TArrow(xlow,y,xhigh,y,arrowSize,"---------");
			}
			arX->SetLineWidth(lWid);
			arX->SetLineColor(lCol);
			arX->SetFillColor(lCol);
			arX->DrawClone();

			mk->DrawClone();
			mk->Delete();
			arX->Delete();
			arY->Delete();
		}
	}
}

//__________________________________________________________________________
void drawOverSizeSysErr(TGraphErrors *gr, double scale, double bandFrac, double arrowFrac, int bandWidth, int bandColor, double arrowSize, double xmin, double xmax, double yTh=1.e-15)
{
	for(int i=0;i<gr->GetN();i++){

		//double x,y,ex,ey;
		double x,y,ey;
		gr->GetPoint(i,x,y);
		//ex=gr->GetErrorX(i);
		ey=gr->GetErrorY(i);

		if(x<xmin || x>xmax || y<yTh) continue; //be careful for yTh staff

		TArrow *arY, *arY1;
		arY = NULL;
		arY1 = NULL;
		if(ey>=y){
			arY = new TArrow(x,y+ey,x,y-(scale-scale*bandFrac)*y,arrowSize,"---------");
			arY->SetLineWidth(bandWidth);
			arY->SetLineColor(bandColor);
			arY->SetFillColor(bandColor);
			arY->DrawClone();

			arY1 = new TArrow(x,y-(scale-scale*arrowFrac)*y,x,y-scale*y,arrowSize,"---------|>");
			arY1->SetLineWidth(1);
			arY1->SetLineColor(bandColor);
			arY1->SetFillColor(bandColor);
			arY1->DrawClone();
		}
		else{
			arY = new TArrow(x,y+ey,x,y-ey,arrowSize,"---------");
			arY->SetLineWidth(bandWidth);
			arY->SetLineColor(bandColor);
			arY->SetFillColor(bandColor);
			arY->DrawClone();
		}

		arY->Delete();
		if(arY1) arY1->Delete();
	}
}

//__________________________________________________
TH2D* histo(TString name, double xlow, double xup, double ylow, double yup, TString xTitle, TString yTitle)
{
	TH2D *dd = new TH2D(name.Data(),"",500,xlow,xup,500,ylow,yup);
	dd->GetXaxis()->SetTitle(xTitle.Data());
	dd->GetYaxis()->SetTitle(yTitle.Data());

	dd->GetXaxis()->SetTitleSize(0.055);
	dd->GetXaxis()->SetTitleOffset(0.9);
	dd->GetXaxis()->SetLabelSize(0.045);
	dd->GetYaxis()->SetTitleSize(0.055);
	dd->GetYaxis()->SetTitleOffset(1);
	dd->GetYaxis()->SetLabelSize(0.045);
	dd->GetXaxis()->CenterTitle(kTRUE);
	dd->GetYaxis()->CenterTitle(kTRUE);
	//dd->GetXaxis()->SetNdivisions(512);
	return dd;
}

//__________________________________________________
TH2D* histo(TString name, int nBinsX, double xlow, double xup, int nBinsY, double ylow, double yup, TString xTitle, TString yTitle)
{
	TH2D *dd = new TH2D(name.Data(),"",nBinsX,xlow,xup,nBinsY,ylow,yup);
	dd->GetXaxis()->SetTitle(xTitle.Data());
	dd->GetYaxis()->SetTitle(yTitle.Data());

	dd->GetXaxis()->SetTitleSize(0.055);
	dd->GetXaxis()->SetTitleOffset(0.9);
	dd->GetXaxis()->SetLabelSize(0.045);
	dd->GetYaxis()->SetTitleSize(0.055);
	dd->GetYaxis()->SetTitleOffset(1);
	dd->GetYaxis()->SetLabelSize(0.045);
	dd->GetXaxis()->CenterTitle(kTRUE);
	dd->GetYaxis()->CenterTitle(kTRUE);
	dd->GetXaxis()->SetNdivisions(512);
	return dd;
}

//__________________________________________________
TArrow*  drawArrow(double x1, double y1, double x2, double y2, double arrowSize=0.05, TString arrowOption="|>", int lineColor=1, int lineWidth=1, int lineStyle=1)
{
	TArrow *arrow = new TArrow(x1, y1, x2, y2, arrowSize, arrowOption.Data());
	arrow->SetFillColor(lineColor);
	arrow->SetLineColor(lineColor);
	arrow->SetLineWidth(lineWidth);
	arrow->SetLineStyle(lineStyle);
	arrow->Draw();
	return arrow;
}

//__________________________________________________
TBox*  drawBox(double x, double y, double xErrLow, double yErrLow, double xErrHi, double yErrHi, int color, int lineWidth=1, int fillStyle=0, double colorAlpha=1)
{
	TBox *box = new TBox(x-xErrLow, y-yErrLow, x+xErrHi, y+yErrHi);
	box->SetLineColor(color);
	box->SetLineWidth(lineWidth);
	box->SetFillStyle(fillStyle);
	box->SetFillColorAlpha(color, colorAlpha);
	box->Draw("same");
	return box;
}

//__________________________________________________
TLatex* drawLatex(double x, double y, TString text, int textFont, double textSize, int colorIndex, double textAngle=0)
{
	TLatex *latex = new TLatex(x,y,text.Data());
	latex->SetNDC();
	latex->SetTextFont(textFont);
	latex->SetTextSize(textSize);
	latex->SetTextColor(colorIndex);
	latex->SetTextAngle(textAngle);
	latex->Draw("same");
	return latex;
}

//__________________________________________________
TLine* drawLine(double xlow,double ylow, double xup, double yup, int lineColor, int lineWidth, int lineStyle)
{
	TLine *l1 = new TLine(xlow,ylow,xup,yup);
	l1->SetLineColor(lineColor);
	l1->SetLineWidth(lineWidth);
	l1->SetLineStyle(lineStyle);
	l1->Draw("same");
	return l1;
}

//__________________________________________________
void drawLines(double xlow,double ylow, double xup, double yup, int lineColor, int lineWidth, int lineStyle)
{
	drawLine(xlow,ylow,xup,ylow,lineColor,lineWidth,lineStyle);
	drawLine(xlow,yup,xup,yup,lineColor,lineWidth,lineStyle);
	drawLine(xlow,ylow,xlow,yup,lineColor,lineWidth,lineStyle);
	drawLine(xup,ylow,xup,yup,lineColor,lineWidth,lineStyle);
}

//__________________________________________________
void setHisto(TH1D *h,int markerStyle, double markerSize, int markerColor,int lineColor,int lineWidth=1)
{
	h->SetMarkerStyle(markerStyle);
	h->SetMarkerSize(markerSize);
	h->SetMarkerColor(markerColor);
	h->SetLineColor(lineColor);
	h->SetLineWidth(lineWidth);
};

//__________________________________________________
void setProfile(TProfile *h,int markerStyle, double markerSize, int markerColor,int lineColor,int lineWidth=1)
{
	h->SetMarkerStyle(markerStyle);
	h->SetMarkerSize(markerSize);
	h->SetMarkerColor(markerColor);
	h->SetLineColor(lineColor);
	h->SetLineWidth(lineWidth);
};

//__________________________________________________
void setLegend(TLegend *leg, double xlow, double ylow, double xup, double yup, double textSize){
	leg->Clear();
	leg->SetX1NDC(xlow);
	leg->SetY1NDC(ylow);
	leg->SetX2NDC(xup);
	leg->SetY2NDC(yup);
	leg->SetTextSize(textSize);
}

//__________________________________________________
void setGraph(TGraph *g, int markerStyle, double markerSize, int markerColor, int lineColor, int lineWidth=1)
{
	g->SetMarkerStyle(markerStyle);
	g->SetMarkerSize(markerSize);
	g->SetMarkerColor(markerColor);
	g->SetLineColor(lineColor);
	g->SetLineWidth(lineWidth);
}

//__________________________________________________
void setGraph(TGraphErrors *g, int markerStyle, double markerSize, int markerColor, int lineColor, int lineWidth=1)
{
	g->SetMarkerStyle(markerStyle);
	g->SetMarkerSize(markerSize);
	g->SetMarkerColor(markerColor);
	g->SetLineColor(lineColor);
	g->SetLineWidth(lineWidth);
}

//__________________________________________________
void setGraph(TGraphAsymmErrors *g, int markerStyle, double markerSize, int markerColor, int lineColor, int lineWidth=1)
{
	g->SetMarkerStyle(markerStyle);
	g->SetMarkerSize(markerSize);
	g->SetMarkerColor(markerColor);
	g->SetLineColor(lineColor);
	g->SetLineWidth(lineWidth);
}

//__________________________________________________
void setFun(TF1 *f, int lineColor, int lineWidth=2, int lineStyle=1)
{
	f->SetLineColor(lineColor);
	f->SetLineWidth(lineWidth);
	f->SetLineStyle(lineStyle);
}

//__________________________________________________
void setPad(double left, double right, double top, double bottom)
{
	gPad->SetFillColor(10);
	gPad->SetBorderMode(0);
	gPad->SetBorderSize(0);
	gPad->SetFrameFillColor(10);
	gPad->SetFrameBorderMode(0);
	gPad->SetFrameBorderSize(0);
	gPad->SetLeftMargin(left);
	gPad->SetRightMargin(right);
	gPad->SetTopMargin(top);
	gPad->SetBottomMargin(bottom);
}

//__________________________________________________
void clearPad(TCanvas *c, int nPads)
{
	for(int i=0;i<nPads;i++){
		c->cd(i+1);
		gPad->Clear();
	}
}

//__________________________________________________
void pdfAction(TCanvas *c, TPDF *ps, Bool_t isClosePDF = kFALSE)
{
	c->cd();
	ps->On();
	c->Update();
	if(isClosePDF){
		ps->Close();
	}
	else{
		ps->NewPage();
		ps->Off();
	}
};

////__________________________________________________                               
//void pdfAction(TCanvas *c, TPDF *ps)                                               
//{                                                                                  
//    ps->On();                                                                      
//    c->Update();                                                                   
//    c->cd();                                                                       
//    ps->NewPage();                                                                 
//    ps->Off();                                                                     
//};     
