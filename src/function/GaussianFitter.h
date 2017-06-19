#include <Gaussian.h>
#include <vector>      
#include <string>



class GaussianFitter
{
public:
    GaussianFitter (GaussianFitter* func_);
    virtual ~GaussianFitter ();

    void SetMeanNames(const std::vector<double> means_,const std::string& baseName_);
    void SetStdDevNames(const std::vector<double> stdDevs_,const std::string& baseName_);

private:
    Gaussian* fOrignalFunc;
    std::vector<std::string> names;
    std::vector<std::string> fMeans;
    std::vector<std::string> fStdDevs;
};
