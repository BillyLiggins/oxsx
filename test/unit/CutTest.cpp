#include <catch.hpp>
#include <BoxCut.h>
#include <BoolCut.h>
#include <LineCut.h>
#include <CutCollection.h>

TEST_CASE("1D Box cut"){
    BoxCut cut(0, 1 , 10);
    cut.SetLowerLimit(1);
    cut.SetUpperLimit(10);
    
    SECTION("in the middle of accepted range"){
        Event ev(std::vector<double>(1, 5));
        REQUIRE(cut.PassesCut(ev) == true);
    }

    SECTION("below accepted range"){
        Event ev(std::vector<double>(1, 0));
        REQUIRE(cut.PassesCut(ev) == false);
    }

    SECTION("above accepted range"){
        Event ev(std::vector<double>(1, 11));
        REQUIRE(cut.PassesCut(ev) == false);
    }

    SECTION("on lower boundary"){
        Event ev(std::vector<double>(1, 1));
        REQUIRE(cut.PassesCut(ev) == false);
    }

    SECTION("on upper boundary"){
        Event ev(std::vector<double>(1, 10));
        REQUIRE(cut.PassesCut(ev) == false);
    }
}

TEST_CASE("Several Cuts on MultiD data"){
    //  First val must be more than 2
    LineCut lineCut(0, 2, "lower");

    // cut if second observable is between 5 and 10
    BoxCut boxCut(1, 5, 10);

    // cut if the third value is 1 exactly
    BoolCut boolCut(2, 1);

    // Combine the three cuts
    CutCollection combinedCuts;
    combinedCuts.AddCut(lineCut);
    combinedCuts.AddCut(boxCut);
    combinedCuts.AddCut(boolCut);

    // Create a fake event to test
    std::vector<double> observations;
    observations.push_back(1);
    observations.push_back(6);
    observations.push_back(1);

    Event fakeEvent(observations);

    // Check vals
    REQUIRE(lineCut.PassesCut(fakeEvent) == false);
    REQUIRE(boxCut.PassesCut(fakeEvent)  == true);
    REQUIRE(boolCut.PassesCut(fakeEvent) == true);
    REQUIRE(combinedCuts.PassesCuts(fakeEvent) == false);
}
