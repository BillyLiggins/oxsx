/****************************************************************************/
/* Test Statistics must implement the Evaluate method for optimisation,     */
/* as well as registering any components that should be adjusted in the fit */
/*     using AddFitComponent() in the method RegisterFitComponents()        */
/****************************************************************************/
#ifndef __OXSX_TEST_STATISTIC__
#define __OXSX_TEST_STATISTIC__
#include <vector>
#include <string>
#include <stddef.h>

class TestStatistic{
 public:
    virtual ~TestStatistic() {}

    virtual double Evaluate() = 0;    
    virtual void   SetVerboseMode() = 0;
    virtual void   PrintParameters(const std::vector<double>& params_) = 0;
    virtual void   SetParameters(const std::vector<double>& params_) = 0;
    virtual std::vector<double> GetParameters() const = 0;
    virtual int    GetParameterCount() const = 0;

    virtual std::vector<std::string> GetParameterNames() const = 0;
    
    // Set up all the components for a fit
    virtual void RegisterFitComponents() = 0;
};
#endif
