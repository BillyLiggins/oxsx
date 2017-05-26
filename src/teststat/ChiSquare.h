/********************************************************************/
/* A routine to calculate a binned chi squared test.                */
/* NOTE: this can only make sense if all pdfs have the same binning */
/********************************************************************/
#ifndef __OXSX_CHI_SQUARED__
#define __OXSX_CHI_SQUARED__
#include <SystematicManager.h>
#include <BinnedEDManager.h>
#include <TestStatistic.h>
#include <ComponentManager.h>

//FIXME::Enforce equal binning on the pdfs
class DataSet;
class ChiSquare : public TestStatistic{
 public:
    ChiSquare() : fCalculatedDataDist(false),fVerboseMode(false) {}

    void SetVerboseMode(){fVerboseMode=true;}
    void SetDataSet(DataSet* d);
    DataSet* GetDataSet();

    // Fit Component interface
    double Evaluate();
    void   RegisterFitComponents();
    int    GetParameterCount() const;
    //This method will print the current parameters to screen in verbose mode.  
    void PrintParameters(const std::vector<double>&);
    void   SetParameters(const std::vector<double>& params_);
    std::vector<double> GetParameters() const;
    std::vector<std::string> GetParameterNames() const;
    
 private:
    bool              fCalculatedDataDist;
    bool              fVerboseMode;
    BinnedED          fDataDist;
    void              BinData();

    DataSet*          fDataSet;
    BinnedEDManager   fPdfManager;
    SystematicManager fSystematicManager;
    ComponentManager  fComponentManager;
};
#endif
