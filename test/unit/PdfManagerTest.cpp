#include <catch.hpp>
#include <BinEDManager.h>
#include <Gaussian.h>
#include <AnalyticBinED.h>
#include <Event.h>

TEST_CASE("Add a couple of analytic pdfs"){
    BinEDManager pdfMan;
    Gaussian gaus1(0, 1);
    Gaussian gaus2(0, 1);

    AnalyticBinED pdf1(&gaus1);
    AnalyticBinED pdf2(&gaus2);
    pdf1.SetObservables(0);
    pdf2.SetObservables(0);
    
    SECTION("initialised correctly"){
        REQUIRE(pdfMan.GetNBinEDs() == 0);
        REQUIRE(pdfMan.GetNDims() == 0);
        
    }
    
    SECTION("add pdfs one at a time"){
        pdfMan.AddBinED(&pdf1);
        pdfMan.AddBinED(&pdf2);
        
        REQUIRE(pdfMan.GetNDims() == 1);
        REQUIRE(pdfMan.GetNBinEDs() == 2);
        REQUIRE(pdfMan.GetNormalisations() == std::vector<double>(2, 0));
    }
    
    SECTION("add pdfs in one go"){
        std::vector<BinED*> pdfs;
        pdfs.push_back(&pdf1);
        pdfs.push_back(&pdf2);
        pdfMan.AddBinEDs(pdfs);
        
        REQUIRE(pdfMan.GetNDims() == 1);
        REQUIRE(pdfMan.GetNBinEDs() == 2);
        REQUIRE(pdfMan.GetNormalisations() == std::vector<double>(2, 0));
    }

    SECTION("correct probability"){
        pdfMan.AddBinED(&pdf1);
        pdfMan.AddBinED(&pdf2);

        Event event(std::vector<double>(1, 0));
        REQUIRE(pdfMan.Probability(event) == 0); // norms are zero

        pdfMan.SetNormalisations(std::vector<double>(2, 1));
        REQUIRE(pdfMan.Probability(event) == Approx(0.7978845607));
        // 0.7978845607 = 2 /sqrt(2 * pi)
    }

    SECTION("works as fit component"){
        pdfMan.AddBinED(&pdf1);
        pdfMan.AddBinED(&pdf2);
        pdfMan.MakeFittable();

        REQUIRE(pdfMan.GetParameterCount() == 2);
        REQUIRE(pdfMan.GetParameters() == std::vector<double>(2, 0));
        
        std::vector<std::string> expectedNames;
        expectedNames.push_back("BinED Normalisation 0");
        expectedNames.push_back("BinED Normalisation 1");
        REQUIRE(pdfMan.GetParameterNames() == expectedNames);
        
        std::vector<double> newParameters(2, 10);
        pdfMan.SetParameters(newParameters);
        REQUIRE(pdfMan.GetParameters()     == newParameters);
        REQUIRE(pdfMan.GetNormalisations() == newParameters);
    }
}
