#ifndef __OXSX_PARAMETER_MANAGER__
#define __OXSX_PARAMETER_MANAGER__
#include <vector>
#include <string>
class FitParameter;
class ParameterManager{
 public:
    ~ParameterManager(); // frees fParamPtrs
    void Add(FitParameter*, const std::string&); // copies 
    template<typename Container>
    void AddContainer(Container&,
                      const std::string& nameBase_);
    
    std::vector<std::string> GetParameterNames() const;
    size_t GetParameterCount() const;
    std::vector<double> GetParameters() const;
    void SetParameters(const std::vector<double>&);

 private:
    std::vector<FitParameter*> fParamPtrs;
    std::vector<std::string>   fNames;
};

// implements AddContainerOfParameters(-)
#include <ParameterManager.hpp> 
#endif
