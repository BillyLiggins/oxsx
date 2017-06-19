#include <Gaussian.h>



class GaussianFitter
{
public:
    GaussianFitter (GaussianFitter* func_);
    virtual ~GaussianFitter ();

private:
    std::vector<std::string> names;
    std::map<std::string,double> means;
    std::vector<std::string> stddevs;
    /* data */
};
