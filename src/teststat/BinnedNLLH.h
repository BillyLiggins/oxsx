#ifndef __BinnedNLLH__
#define __BinnedNLLH__
#include <TestStatistic.h>
#include <BinnedEDManager.h>
#include <SystematicManager.h>
#include <BinnedEDShrinker.h>
#include <ComponentManager.h>
#include <DataSet.h>
#include <CutCollection.h>
#include <CutLog.h>
#include <QuadraticConstraint.h>
#include <PriorManager.h>
#include <map>
#include <vector>

class DataSet;
class BinnedNLLH : public TestStatistic{
 public:
    BinnedNLLH() : fCalculatedDataDist(false), fAlreadyShrunk(false), fDataSet(NULL), fSignalCutEfficiency(1){}

    void   SetPdfManager(const BinnedEDManager&);
    void   SetSystematicManager(const SystematicManager&);

    void   AddPdf(const BinnedED&);
    void   AddSystematic(Systematic*);

    void   AddPdfs(const std::vector<BinnedED>&);
    void   AddSystematics(const std::vector<Systematic*>);

    void   SetConstraint(const std::string& paramName_, double mean_, double sigma_);
    
    void SetNormalisations(const std::vector<double>& norms_);
    std::vector<double> GetNormalisations() const;

    void  BinData();

    void SetDataDist(const BinnedED&);
    BinnedED GetDataDist() const;

    void SetDataSet(DataSet*);
    DataSet* GetDataSet();

    void SetBuffer(size_t dim_, unsigned lower_, unsigned upper_);
    std::pair<unsigned, unsigned> GetBuffer(size_t dim_) const;
    void SetBufferAsOverflow(bool b_); // true by default
    bool GetBufferAsOverflow() const;

    void AddCut(const Cut&);
    void SetCuts(const CutCollection&);

    double GetSignalCutEfficiency() const;
    void   SetSignalCutEfficiency(double);

    CutLog GetSignalCutLog() const;
    void   SetSignalCutLog(const CutLog&);

    // Test statistic interface
    void RegisterFitComponents(); 
    void SetParameters(const ParameterDict&);
    ParameterDict GetParameters() const;
    int  GetParameterCount() const;
    std::set<std::string> GetParameterNames() const;
    double Evaluate();

 private:
    BinnedEDManager      fPdfManager;
    SystematicManager    fSystematicManager;
    BinnedEDShrinker     fPdfShrinker;
    PriorManager         fPriorManager;
    DataSet*             fDataSet;
    CutCollection        fCuts;
    std::map<std::string, QuadraticConstraint> fConstraints;

    double  fSignalCutEfficiency;
    CutLog  fSignalCutLog;

    BinnedED         fDataDist;
    bool             fCalculatedDataDist;
    bool             fAlreadyShrunk;
    ComponentManager fComponentManager;    
};
#endif
