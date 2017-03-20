#include <catch.hpp>
#include <BinnedEDGenerator.h>
#include <DistTools.h>
#include <Gaussian.h>
#include <cmath>

TEST_CASE("Recover a 1D gaussian"){
    Gaussian gaus(0, 10);
    AxisCollection axes;
    axes.AddAxis(BinAxis("test", -100, 100, 200));
    
    BinnedED inBinED(DistTools::ToHist(gaus, axes));
    
    BinnedEDGenerator edGen;
    edGen.SetRates(std::vector<double> (1, 1000000));
    edGen.SetBinEDs(std::vector<BinnedED> (1, inBinED));

    BinnedED outBinED = edGen.ExpectedRatesBinED();
    REQUIRE(outBinED.Integral() == 1000000);
    outBinED.Normalise();

    double outMean = outBinED.Means().at(0);
    double inMean = inBinED.Means().at(0);

    REQUIRE(std::abs(outMean) < 0.01);
}
