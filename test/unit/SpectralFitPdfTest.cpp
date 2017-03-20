#include <catch.hpp>
#include <SpectralFitBinED.h>

TEST_CASE(""){    
    AxisCollection axes;
    axes.AddAxis(BinAxis("test1", 0, 10, 10));
    axes.AddAxis(BinAxis("test2", 0, 10, 10));

    SpectralFitBinED fitBinED(axes);
    
    SECTION("Behaves like a binned pdf"){
        REQUIRE(fitBinED.GetNDims() == 2);
        REQUIRE(fitBinED.GetNBins() == 100);
        REQUIRE(fitBinED.GetBinContents() == std::vector<double>(100, 0));
    }
    SECTION("Bin contents are adjustable"){
        fitBinED.MakeFittable();
        REQUIRE(fitBinED.GetParameterCount() == 100);
        REQUIRE(fitBinED.GetParameters() == std::vector<double>(100, 0));

        std::vector<double> testConts;
        testConts.reserve(100);
        for(int i = 0; i < 100; i++){
            testConts.push_back(i);                        
        }
        fitBinED.SetParameters(testConts);
        REQUIRE(fitBinED.GetParameters() == testConts);
        REQUIRE(fitBinED.GetBinContents() == testConts);        
    }
    
}
