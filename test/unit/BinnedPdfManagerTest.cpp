#include <catch.hpp>
// #include <BinnedPdfManager.h>
#include <BinnedEDManager.h>
#include <BinAxis.h>
#include <DistTools.h>
// #include <PdfConverter.h>
#include <Gaussian.h>
#include <iostream>

TEST_CASE("Three pdfs no systematics"){
    Gaussian gaus1(0, 1);
    Gaussian gaus2(2, 3);
    Gaussian gaus3(4, 5);

    AxisCollection axes;
    axes.AddAxis(BinAxis("axis1", -40, 40 , 200));

    BinnedED pdf1 = DistTools::ToHist(gaus1, axes);
    BinnedED pdf2 = DistTools::ToHist(gaus2, axes);
    BinnedED pdf3 = DistTools::ToHist(gaus3, axes);

    pdf1.SetObservables(0);
    pdf2.SetObservables(0);    
    pdf3.SetObservables(0);

    double prob1 = pdf1.GetBinContent(0);
    double prob2 = pdf2.GetBinContent(0);    
    double prob3 = pdf3.GetBinContent(0);

    BinnedEDManager pdfMan;
    pdfMan.AddPdf(pdf1);
    pdfMan.AddPdf(pdf2);
    pdfMan.AddPdf(pdf3);
    pdfMan.SetNormalisations(std::vector<double>(3, 1));

    SECTION("Bin Probability Method"){
        REQUIRE(pdfMan.BinProbability(0) == Approx(prob1 * prob2 * prob3));
    }
    
    SECTION("Probability Method"){
        REQUIRE(pdfMan.Probability(Event(std::vector<double>(1, -39.5))) == Approx(prob1 * prob2 * prob3));
    }

}
