#include <catch.hpp>
#include <HistTools.h>
#include <BinAxis.h>
#include <Histogram.h>
#include <Combinations.hpp>
#include <Formatter.hpp>

TEST_CASE("Producing  histograms from a set of axes"){    
    // make the axes
    AxisCollection axes;
    std::stringstream ss;
    for(int i = 0; i < 4; i++)
        axes.AddAxis(BinAxis(Formatter() << i, 0, 1, 1));
    
    std::vector<size_t> dimensionList = SequentialElements<size_t>(size_t(0), axes.GetNDimensions());


    SECTION("2D"){
        std::vector<std::vector<size_t> > l2combinations = FixedLengthCombinationsNoDuplicates<size_t>(dimensionList, 2);
        
        std::vector<Histogram> hists = HistTools::MakeAllHists(axes, l2combinations);
        for(size_t i = 0; i < hists.size(); i++){
            for(size_t j = 0; j < hists.at(i).GetNDims(); j++){
                REQUIRE(hists.at(i).GetAxes().GetAxis(j).GetName() == std::string(Formatter() << l2combinations.at(i).at(j)));
            }
        }
    }
    
    SECTION("1D"){
        std::vector<std::vector<size_t> > l1combinations = FixedLengthCombinationsNoDuplicates<size_t>(dimensionList, 1);
        
        std::vector<Histogram> hists = HistTools::MakeAllHists(axes, l1combinations);
        for(size_t i = 0; i < hists.size(); i++){
            REQUIRE(hists.at(i).GetAxes().GetAxis(0).GetName() == std::string(Formatter() << l1combinations.at(i).at(0)));
        }
    }
        
}
    

