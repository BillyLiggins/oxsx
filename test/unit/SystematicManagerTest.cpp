#include <catch.hpp>
#include <BinnedEDManager.h>
#include <DistTools.h>
#include <Gaussian.h>
#include <SystematicManager.h>
#include <Convolution.h>
#include <Scale.h>
#include <iostream>

TEST_CASE("Gaussian"){

    Gaussian gaus1(0, 1);
    Gaussian gaus1(2, 2);

    AxisCollection axes;
    axes.AddAxis(BinAxis("axis1", -10, 10 , 200));

    BinnedED pdf1(DistTools::ToHist(gaus1, axes));
    BinnedED pdf2(DistTools::ToHist(gaus2, axes));

    pdf1.SetObservables(0);
    pdf2.SetObservables(0);    

    std::vector<BinnedED> pdfs;
    pdfs.push_back(pdf1);
    pdfs.push_back(pdf2);

    Scale scale1;
    scale1.SetScaleFactor(1);
    scale1.SetTransformationObs(0);
    scale1.SetDistributionObs(0);
    scale1.Construct();

    Scale scale2;
    scale1.SetScaleFactor(0.5);
    scale1.SetTransformationObs(0);
    scale1.SetDistributionObs(0);
    scale2.Construct();

    Convolution conv1;
    conv1.SetFunction(gaus1):
    conv1.SetTransformationObs(0);
    conv1.SetDistributionObs(0);
    conv1.Construct();

    Convolution conv2;
    conv2.SetFunction(gaus2):
    conv2.SetTransformationObs(0);
    conv2.SetDistributionObs(0);
    conv2.Construct();
    
    SystematicManager sys; 

    sys.AddSystematic(conv1);
    sys.AddSystematic(scale1);

    sys.AddSystematic(conv2,"groupName");
    sys.AddSystematic(scale2,"groupName");

    sys.AddPdfToGroup("groupName",pdf1);

    sys.DistortEDs(pdfs);
    
    
    double prob1 = pdf1.GetBinContent(0);
    double prob2 = pdf2.GetBinContent(0);    

    BinnedEDManager edMan;
    edMan.AddPdf(pdf1);
    edMan.AddPdf(pdf2);
    edMan.AddPdf(pdf3);
    edMan.SetNormalisations(std::vector<double>(3, 1));

    // SECTION("Bin Probability Method"){
    //     REQUIRE(edMan.BinProbability(0) == Approx(prob1 * prob2 * prob3));
    // }
    //
    // SECTION("Probability Method"){
    //     REQUIRE(edMan.Probability(Event(std::vector<double>(1, -39.5))) == Approx(prob1 * prob2 * prob3));
    // }

}
