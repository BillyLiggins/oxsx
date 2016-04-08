#include <ChiSquare.h>
#include <DataSet.h>

double
ChiSquare::Evaluate(){
    // the first time this is called, bin data into a pdf
    if (!fCalculatedDataPdf){
        BinData();
        fCalculatedDataPdf = true;
    }
    
    // Construct
    fSystematicManager.Construct();

    // Apply systematics
    fPdfManager.ApplySystematics(fSystematicManager);
    
    // Now calculate the ChiSquared
    double chiSquare = 0;
    std::vector<double> binCentre(fDataPdf.GetNDims());
    for(size_t i = 0; i < fDataPdf.GetNBins(); i++){
        fDataPdf.GetAxes().GetBinCentres(i, binCentre);
        double expected = fPdfManager.Probability(binCentre);
        double deviation = fDataPdf.GetBinContent(i) - expected;
        chiSquare += deviation * deviation / expected; // poisson errors 
    }

    return chiSquare;
}


void
ChiSquare::BinData(){

    BinnedPdf dataPdf(fPdfManager.GetOriginalPdf(0)); // make a copy for same binning and data rep
    dataPdf.Empty();
    
    for(size_t i = 0; i < fDataSet -> GetNEntries(); i++){
        dataPdf.Fill(fDataSet -> GetEntry(i));
    }
    
    fDataPdf = dataPdf;
}


void
ChiSquare::RegisterFitComponents(){
    AddFitComponent(&fPdfManager);
    for(size_t i = 0; i < fSystematicManager.GetSystematics().size(); i++)
        AddFitComponent(fSystematicManager.GetSystematics().at(i));
}
