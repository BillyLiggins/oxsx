#include <catch.hpp>
#include <BinEDMapping.h>
#include <BinnedED.h>

TEST_CASE("Check Initialisation of 3x3 -> 3x3"){
    AxisCollection ax;
    ax.AddAxis(BinAxis("axis 1", 2.3, 4.5, 90));
    ax.AddAxis(BinAxis("axis 2", 1.3, 4.7, 11));
    
    BinEDMapping map;
    REQUIRE(map.GetNBins() == 0);
    map.SetAxes(ax);
    
    REQUIRE(map.GetNBins() == 90 * 11);
    
    std::vector<double> initalVals;
    for(size_t i = 0; i < map.GetNBins(); i++)
        for(size_t j = 0; j < map.GetNBins(); j++)
            initalVals.push_back(map.GetComponent(i, j));
    
    // should all be initialised to zero
    REQUIRE(initalVals == std::vector<double>((90*11) * (90 * 11), 0));    
}


TEST_CASE("Identity Matrix Multiplication on 100 long vector"){
    AxisCollection axes;
    axes.AddAxis(BinAxis("", 0, 100, 100));

    BinnedED binnedBinED(axes);
    for(size_t i = 0; i < 100; i++)
        binnedBinED.SetBinContent(i, 1.1);
    
    BinEDMapping map;
    map.SetAxes(axes);

    for(size_t i = 0; i < 100; i++)
        map.SetComponent(i, i, 1);

    BinnedED mappedBinED = map(binnedBinED);

    REQUIRE(mappedBinED.GetBinContents() == std::vector<double> (100, 1.1));
}

