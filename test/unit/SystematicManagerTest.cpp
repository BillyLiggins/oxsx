#include <catch.hpp>
#include <BinnedEDManager.h>
#include <DistTools.h>
#include <Gaussian.h>
#include <SystematicManager.h>
#include <Convolution.h>
#include <Scale.h>
#include <iostream>

TEST_CASE("SystematicManager"){

    Gaussian gaus(0,1);

    SparseMatrix id(50,50);
    id.SetToIdentity();

    SparseMatrix zero(50,50);
    zero.SetZeros();

    AxisCollection axes;
    axes.AddAxis(BinAxis("axis1", -10, 10 ,50));

    BinnedED pdf1("pdf1",axes);
    BinnedED pdf2("pdf2",axes);

    pdf1.SetObservables(0);
    pdf2.SetObservables(0);    

    pdf1.SetBinContent(10,0.5);
    pdf2.SetBinContent(10,0.5);

    pdf1.SetBinContent(40,0.5);
    pdf2.SetBinContent(40,0.5);

    std::vector<BinnedED> pdfs;
    pdfs.push_back(pdf1);
    pdfs.push_back(pdf2);

    Scale scale1("scale1");
    scale1.SetAxes(axes);
    scale1.SetScaleFactor(1.0);
    scale1.SetTransformationObs(0);
    scale1.SetDistributionObs(0);
    scale1.Construct();

    Scale scale2("scale2");
    scale2.SetAxes(axes);
    scale2.SetScaleFactor(0.5);
    scale2.SetTransformationObs(0);
    scale2.SetDistributionObs(0);
    scale2.Construct();

    Convolution conv1("smear1");
    conv1.SetFunction(&gaus);
    conv1.SetAxes(axes);
    conv1.SetTransformationObs(0);
    conv1.SetDistributionObs(0);
    conv1.Construct();
    // conv1.SetResponse(id);

    // Convolution conv2("smear2");
    // conv2.SetFunction(&gaus);
    // conv2.SetAxes(axes);
    // conv2.SetResponse(id);
    // conv2.SetTransformationObs(0);
    // conv2.SetDistributionObs(0);
    // conv2.Construct();
    
    SystematicManager sys; 

    sys.Add(&conv1);
    sys.Add(&scale2);
    // sys.Add(&conv1,"all");
    // sys.Add(&scale1,"all");
    // sys.AddPdfToGroup("all",pdf1);
    // sys.AddPdfToGroup("all",pdf2);

    BinnedEDManager edMan;
    edMan.AddPdf(pdf1);
    edMan.AddPdf(pdf2);
    edMan.SetNormalisations(std::vector<double>(2, 1));

    // BinnedEDManager edMan;
    // edMan.AddPdf(pdf1);
    // edMan.AddPdf(pdf2);
    // edMan.SetNormalisations(std::vector<double>(2, 1));

    
    SECTION("Applying identity does nothing"){

        edMan.ApplySystematics(sys);

        for (int i = 0; i <50; ++i) 
            std::cout <<"i = "<<std::setprecision(3)<<i<< " " << std::setprecision(3)<<edMan.BinProbability(i)<< " "<< std::endl;

        REQUIRE( edMan.BinProbability(10) == Approx(1.0) );
        REQUIRE( edMan.BinProbability(40) == Approx(1.0) );
    }

    SECTION("Applying Scale to PDF1"){
        sys.Add(&scale2,"groupName");
        sys.AddPdfToGroup("groupName",pdf1);
        edMan.ApplySystematics(sys);

        for (int i = 0; i <50; ++i)
            std::cout <<"i = "<<i<< " " << edMan.BinProbability(i)<< " "<< std::endl;

        REQUIRE( edMan.BinProbability(10) == Approx(1).margin(0.5) );
        REQUIRE( edMan.BinProbability(5) == Approx(1).margin(0.5) );
        REQUIRE( edMan.BinProbability(40) == Approx(1).margin(0.5) );
        REQUIRE( edMan.BinProbability(20) == Approx(1).margin(0.5) );
    }
    //
    // sys.Add(&conv2,"groupName");
    // sys.AddPdfToGroup("groupName",pdf2);
    // edMan.ApplySystematics(sys);
    //
    // SECTION("Applying Scale to PDF1"){
    //     REQUIRE( edMan.BinProbability(10) == Approx(1).epsilon(1) );
    //     REQUIRE( edMan.BinProbability(5) == Approx(1).epsilon(1) );
    //     REQUIRE( edMan.BinProbability(40) == Approx(1).epsilon(1) );
    //     REQUIRE( edMan.BinProbability(20) == Approx(1).epsilon(1) );
    // }

}
