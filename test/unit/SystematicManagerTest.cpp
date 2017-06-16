//The point of this is to test the interace no do a fit.
#include <catch.hpp>
#include <BinnedEDManager.h>
#include <DistTools.h>
#include <Gaussian.h>
#include <SystematicManager.h>
#include <Convolution.h>
#include <Scale.h>
#include <iostream>

class FakeSystematic : public Systematic{
public:
    FakeSystematic(const std::string& name_)  {name = name_; }
    ~FakeSystematic()  {}

    BinnedED 
    operator()(const BinnedED& pdf_) const{;}
        
    void SetResponse(const SparseMatrix& responseMatrix_){resp= responseMatrix_;}
    const SparseMatrix& GetResponse() const{return resp;}
        
    void   SetTransformationObs(const ObsSet&){;}

    ObsSet GetTransformationObs() const{;}

    void   SetDistributionObs(const ObsSet&){;}
    ObsSet GetDistributionObs() const{;}

    const AxisCollection& GetAxes() const{}
    void  SetAxes(const AxisCollection& axes_){}

    void Construct() {;}

    void   SetParameter(const std::string& name_, double value) {;};
    double GetParameter(const std::string& name_) const { return 2;};

    void   SetParameters(const ParameterDict&) {;};
    ParameterDict GetParameters() const {return ParameterDict temp;};
    size_t GetParameterCount() const  {;};

    std::set<std::string> GetParameterNames() const {return std:set;};
    void   RenameParameter(const std::string& old_, const std::string& new_) {;};

    std::string GetName() const {return name;};
    void SetName(const std::string& name_) {name = name_;};
private:
    std::string name;
    SparseMatrix resp;
};

TEST_CASE("SystematicManager"){

    SparseMatrix id(50,50);
    id.SetToIdentity();
    SparseMatrix zero(50,50);
    zero.SetZeros();

    FakeSystematic sys1("sys1");
    sys1.SetResponse(id);
    FakeSystematic sys2("sys2");
    sys2.SetResponse(zero);

    AxisCollection axes;
    axes.AddAxis(BinAxis("axis1", -10, 10 ,50));

    BinnedED pdf1("pdf1",axes);
    BinnedED pdf2("pdf2",axes);

    pdf1.SetBinContent(10,0.5);
    pdf1.SetBinContent(40,0.5);

    pdf2.SetBinContent(10,0.5);
    pdf2.SetBinContent(40,0.5);

    std::vector<BinnedED&> pdfs;
    pdfs.push_back(pdf1);
    pdfs.push_back(pdf2);

    SystematicManager man;

    man.Add(sys1);
    man.Add(sys2,"groupName");

    std::vector<std::string> appliedGroups;
    appliedGroups.push_back("groupName");
    man.AddDist(pdf1,appliedGroups); 

    man.DistortEDs(pdfs);

    SECTION("Set group names correctly"){
        std::vector<std::string> name =  man.GetGroupNames();
        // REQUIRE( name == "groupName" );
    }

    SECTION("Counting systematics"){
        int n =  man.GetNSystematics();
        // REQUIRE( n == 2);
        n =  man.GetNSystematicsInGroup("groupName");
        // REQUIRE( n == 1);
    }

    SECTION("Applying identity does nothing"){

        // REQUIRE( pdf1.GetBinContent(10) == 0.5 );
        // REQUIRE( pdf2.GetBinContent(10) == 0 );
    }

    // SECTION("Applying Scale to PDF1"){
    //     sys.Add(&scale2,"groupName");
    //     sys.AddPdfToGroup("groupName",pdf1);
    //     edMan.ApplySystematics(sys);
    //
    //     for (int i = 0; i <50; ++i)
    //         std::cout <<"i = "<<i<< " " << edMan.BinProbability(i)<< " "<< std::endl;
    //
    //     REQUIRE( edMan.BinProbability(10) == Approx(1).margin(0.5) );
    //     REQUIRE( edMan.BinProbability(5) == Approx(1).margin(0.5) );
    //     REQUIRE( edMan.BinProbability(40) == Approx(1).margin(0.5) );
    //     REQUIRE( edMan.BinProbability(20) == Approx(1).margin(0.5) );
    // }
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
