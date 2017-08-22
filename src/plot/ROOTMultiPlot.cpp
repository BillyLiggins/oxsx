#include <ROOTMultiPlot.h>
#include <DistTools.h>
#include <Exceptions.h>
#include <BinnedED.h>
#include <iostream>

//FIXME: add marginalisations for higher D pdfs
const unsigned 
ROOTMultiPlot::fNcolors = 7;
const int
ROOTMultiPlot::fColorScheme[fNcolors] = {kRed, kBlue, kViolet, 
                     kOrange, kBlue, kBlack,
                     kMagenta};
                     

void
ROOTMultiPlot::AddPdf(const BinnedED& pdf_, const std::string& name_){
  size_t nDims = pdf_.GetNDims();
  if (nDims != 1)
    throw DimensionError("ROOTMultiPlot::Added dim != 1 binned pdf!");
  
  TH1D rootHist = DistTools::ToTH1D(pdf_);
  rootHist.SetDirectory(0);
  rootHist.GetXaxis()->SetTitle(pdf_.GetAxes().GetAxis(0).GetLatexName().c_str());
  AddPdf(rootHist, name_);
}

void 
ROOTMultiPlot::AddPdf(const TH1D& pdf_, const std::string& name_){
  fHists.push_back(pdf_);
  fNames.push_back(name_);
  fConstructed = false;
}

void
ROOTMultiPlot::SaveAs(const std::string& filename_){
  if(!fConstructed)
    Construct();
  
  fCanvas.SaveAs(filename_.c_str());
}


TCanvas&
ROOTMultiPlot::GetCanvas(){
  if(!fConstructed)
    Construct();
  
  return fCanvas;
}


void
ROOTMultiPlot::Construct(){
  if(!fHists.size())
    return;

  // Setsizes
  double maxX = 0;
  double minX = 0;

  for(size_t i = 0; i < fHists.size(); i++){
    TAxis* axis = fHists[i].GetXaxis();
    double xUp  = axis -> GetBinLowEdge(axis -> GetFirst());
    double xLow = axis -> GetBinUpEdge(axis  -> GetLast());

    if(xUp > maxX || !i)
      maxX = xUp;
    if(xLow < minX || !i)
      maxX = xUp;
  }
  double span = maxX - minX;
  for(size_t i = 0; i < fHists.size(); i++){
    //      fHists[i].GetXaxis() -> SetRangeUser(minX, maxX);
  }

  // draw with legend
  fCanvas.Clear();
  fLegend.Clear();
  fCanvas.cd();
  
  if(fStacked)
    ConstructStacked();
  else
    ConstructOverlay();

  for(size_t i = 0; i < fNames.size(); i++){
      fLegend.AddEntry(&fHists.at(i), fNames.at(i).c_str());
      fHists[i].SetLineColor(fColorScheme[i %fNcolors]);
  }
  
  if(fDrawLegend)
    fLegend.Draw("same");
  
  fConstructed = true;
}

void
ROOTMultiPlot::SetStacked(bool b_){
  fStacked = b_;
  fConstructed = false;
}

void
ROOTMultiPlot::ConstructOverlay(){
  for(size_t i = 0; i < fNames.size(); i++){
    fHists[i].SetFillColor(0);
    fHists.at(i).Draw("same");  
  }
}

void
ROOTMultiPlot::ConstructStacked(){
  fStack.Clear();
  for(size_t i = 0; i < fNames.size(); i++){
    fStack.Add(&fHists.at(i));
    fHists[i].SetFillColor(fColorScheme[i %fNcolors]);
  }
  fStack.Draw();
}

void
ROOTMultiPlot::SetDrawLegend(bool b_){
    fDrawLegend = b_;
}
