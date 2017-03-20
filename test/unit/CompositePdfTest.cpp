#include <catch.hpp>
#include <Gaussian.h>
#include <BinnedED.h>
#include <CompositeBinED.h>
#include <AnalyticBinED.h>
#include <iostream>

TEST_CASE("Combining 1D gaussians", "[CompositeBinED]"){
    Gaussian gausF1(0.5, 0.4);
    Gaussian gausF2(0.5, 0.3);

    AnalyticBinED gaus1(&gausF1);
    AnalyticBinED gaus2(&gausF2);

    ObsSet d1(0);
    ObsSet d2(2);
    
    gaus1.SetObservables(d1);

    gaus2.SetObservables(d2);

    CompositeBinED compositeBinED = gaus1 * gaus2;

    // test values
    std::vector<double> vals;
    vals.push_back(1);
    vals.push_back(999);
    vals.push_back(1);
    vals.push_back(1);
    vals.push_back(999);



    SECTION("Check Dimensions"){
        REQUIRE(compositeBinED.GetNDims() == 2);
    }

    SECTION("Check Data Flow to internal pdfs "){

        double prob  = compositeBinED.Probability(Event(vals));
        REQUIRE(prob == Approx(0.15141173681343614));
        
    }

    SECTION("Check Clone Functionality"){
        BinED* clone = compositeBinED.Clone();
        REQUIRE(clone -> GetNDims() == 2);
        REQUIRE(clone -> Probability(Event(vals)) == Approx(0.15141173681343614));
    }

    SECTION("Second level of recursion"){
        Gaussian nextF = Gaussian(0.9, 0.8);

        AnalyticBinED nextBinED(&nextF);
        nextBinED.SetObservables(ObsSet(3));
        CompositeBinED level2 = compositeBinED * nextBinED;
        REQUIRE( level2.GetNDims() == 3 );
        REQUIRE( level2.Probability(Event(vals)) == Approx(0.07491808959564718));
                                                                                         
    }
}

TEST_CASE(" Composite of two Binned BinEDs"){

    // Paired into two couplets
    BinAxis axis1("axis1", -80, 80, 100);
    BinAxis axis2("axis2", -80, 80, 100);

    BinAxis axis3("axis3", -80, 80, 100);
    BinAxis axis4("axis4", -80, 80, 100);

    AxisCollection axes1;
    AxisCollection axes2;
    
    axes1.AddAxis(axis1);
    axes1.AddAxis(axis2);
    
    axes2.AddAxis(axis3);
    axes2.AddAxis(axis4);

    BinnedED pdf1(axes1);
    BinnedED pdf2(axes2);


    // Data, where to look
    std::vector<size_t> indicies1;
    std::vector<size_t> indicies2;

    indicies1.push_back(0);
    indicies1.push_back(2);
    
    indicies2.push_back(1);
    indicies2.push_back(3);

    pdf1.SetObservables(indicies1);
    pdf2.SetObservables(indicies2);

    // Now combine
    CompositeBinED compositeBinED = pdf1 * pdf2;


}

