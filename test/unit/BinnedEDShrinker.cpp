#include <catch.hpp>
#include <BinnedEDShrinker.h>
#include <iostream>

TEST_CASE("Shrinking a single uniform BinED Axis"){
    BinAxis axis("test", 0, 100, 100, "latexnametest");
    BinnedEDShrinker shrink;
    BinAxis shrunkAxis = shrink.ShrinkAxis(axis, 2, 3);
   
    REQUIRE(shrunkAxis.GetNBins() == axis.GetNBins() - 5);

    std::vector<double> oldLowEdges(axis.GetNBins());
    for(size_t i = 0; i < oldLowEdges.size(); i++)
        oldLowEdges[i] = axis.GetBinLowEdge(i);


    std::vector<double> newLowEdges(shrunkAxis.GetNBins());
    for(size_t i = 0; i < newLowEdges.size(); i++)
        newLowEdges[i] = shrunkAxis.GetBinLowEdge(i);
    
    
    REQUIRE(newLowEdges == std::vector<double>(oldLowEdges.begin() + 2,
                                               oldLowEdges.end() - 3));    
}

TEST_CASE("Shrinking a 1D pdf"){
    BinAxis axis1("test", 0 , 100, 100, "latexnametest");
    AxisCollection axes; 
    axes.AddAxis(axis1);

    BinnedED inputBinED(axes);
    //fill with linearly increasing some data
    for(size_t i = 0; i < inputBinED.GetNBins(); i++)
        inputBinED.SetBinContent(i, i);
    inputBinED.SetObservables(0);

    BinnedEDShrinker shrinker;
    shrinker.SetBuffer(0, 3, 5); // buffer of 5 above, 3 below in dimension 0

    SECTION("With overflow bins"){
        shrinker.SetUsingOverflows(true);

        BinnedED shrunkBinED = shrinker.ShrinkDist(inputBinED);
        REQUIRE(shrunkBinED.GetNBins() == inputBinED.GetNBins() - 5 - 3);

        // check the over flow bins and the middle bin
        REQUIRE(shrunkBinED.GetBinContent(0) == 6);

        // 94 is already there, then add 99, 98, 97, 96, 95 buffer bins = 579
        REQUIRE(shrunkBinED.GetBinContent(shrunkBinED.GetNBins() - 1) == 579); 
        REQUIRE(shrunkBinED.GetBinContent(50) == 53); // just gets offset by 3
       
    }
    
    SECTION("With truncation"){
        shrinker.SetUsingOverflows(false);
        BinnedED shrunkBinED = shrinker.ShrinkDist(inputBinED);
        REQUIRE(shrunkBinED.GetNBins() == inputBinED.GetNBins() - 5 - 3);

        // check the over flow bins and the middle bin
        REQUIRE(shrunkBinED.GetBinContent(0) == 3);

        REQUIRE(shrunkBinED.GetBinContent(shrunkBinED.GetNBins() - 1) == 94); 
        REQUIRE(shrunkBinED.GetBinContent(50) == 53);
    }
    
}

TEST_CASE("2D pdf, only have buffer in one direction"){
    
    BinAxis axis1("test", 0, 100, 100, "latexnametest");
    BinAxis axis2("test2", 0, 100, 100, "latexnametest");

    AxisCollection axes;
    axes.AddAxis(axis1);
    axes.AddAxis(axis2);

    BinnedED inputBinED(axes);
    // flat spectrum
    for (size_t i = 0; i < inputBinED.GetNBins(); i++)
        inputBinED.SetBinContent(i, 1);

    std::vector<size_t> relevantIndicies;
    relevantIndicies.push_back(0);
    relevantIndicies.push_back(1);

    inputBinED.SetObservables(ObsSet(relevantIndicies));


    BinnedEDShrinker shrinker;
    shrinker.SetBuffer(1, 3, 5); // five, three from above on dim 1
    
    SECTION("With Overflow bins"){
        BinnedED shrunk = shrinker.ShrinkDist(inputBinED);

        REQUIRE(shrunk.GetAxes().GetAxis(0).GetNBins() == inputBinED.GetAxes().GetAxis(0).GetNBins());
        REQUIRE(shrunk.GetAxes().GetAxis(1).GetNBins() == 
                inputBinED.GetAxes().GetAxis(1).GetNBins() - 5 -3 );


        // 0, 92 .. etc should be over flows 
        REQUIRE(shrunk.GetBinContent(0) == 4);
        REQUIRE(shrunk.GetBinContent(1) == 1);
        REQUIRE(shrunk.GetBinContent(91) == 6);
            

        REQUIRE(shrunk.GetBinContent(92) == 4);
        REQUIRE(shrunk.GetBinContent(93) == 1);
        REQUIRE(shrunk.GetBinContent(183) == 6);
        

    }
}




