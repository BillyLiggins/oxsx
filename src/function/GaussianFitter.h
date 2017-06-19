#ifndef __OXSX_GAUSSIANFITTER__
#define __OXSX_GAUSSIANFITTER__
#include <Gaussian.h>
#include <vector>      
#include <string>
#include <ParameterDict.h>

class Gaussian;

class GaussianFitter{
public:
    GaussianFitter(){;}
    GaussianFitter(Gaussian * func_);
    ~GaussianFitter ();

    void SetOriginalFunction(Gaussian* func_);
    void SetMeanNames(const std::vector<double> means_,const std::string& baseName_);
    void SetStdDevNames(const std::vector<double> stdDevs_,const std::string& baseName_);

    void   SetParameter(const std::string& name_, double value);
    double GetParameter(const std::string& name_) const;

    void   SetParameters(const ParameterDict&);
    ParameterDict GetParameters() const;
    size_t GetParameterCount() const;

    std::set<std::string> GetParameterNames() const;
    void   RenameParameter(const std::string& old_, const std::string& new_);

private:
    Gaussian* fOrignalFunc;
    std::vector<std::string> fMeans;
    std::vector<std::string> fStdDevs;
};
#endif
