/******************************************************************************************************/
/* Manages a set of pdfs, recieves a list of parameters and passes them out to each of the systematics*/
/* and triggeres their reconstruction. Systematics inside are passed to a set of pdfs to change       */
/* them                                                                                               */
/******************************************************************************************************/

#ifndef __SYSTEMATIC_MANAGER__
#define __SYSTEMATIC_MANAGER__
#include <vector>
#include <Systematic.h>
#include <SparseMatrix.h>

class SystematicManager{
 public:
    SystematicManager(): fNSystematics(0) {;}
    ~SystematicManager() {}

    void AddSystematic(Systematic*,const std::string groupName_ = "all");
    void AddPdfToGroup(const std::string& groupName_, BinnedED& ED_);

    const std::map<std::string,std::vector<Systematic*> >& GetSystematicsGroup() const;

    const size_t& GetNSystematicsInGroup(std::string & name) const;
    const std::vector<Systematic*>& GetSystematicsInGroup(std::string & name) const;

    const std::vector<std::string>& GetGroupNames(std::string & name) const;
    
    size_t GetNSystematics() const;

    const SparseMatrix& GetTotalResponse(const std::string groupName_ = "all" );

    void DistortEDs(std::vector<BinnedED>& fWorkingEDs);

    void Construct();
    
 private:
    std::vector<Systematic*> fSystematics;
    SparseMatrix fTotalResponse;
    size_t fNSystematics;
    size_t fNGroups;
    std::map<std::string,SparseMatrix> allResponses;
    std::map<std::string,std::vector<Systematic*> > groups;
    std::map<std::string,std::vector<std::string> > EDnames;
    std::vector<std::string> sysOrder; // This isn't used.
};
#endif
