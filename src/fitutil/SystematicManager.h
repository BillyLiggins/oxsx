/******************************************************************************************************/
/* Manages a set of pdfs, recieves a list of parameters and passes them out to each of the systematics*/
/* and triggeres their reconstruction. Systematics inside are passed to a set of pdfs to change       */
/* them                                                                                               */
/******************************************************************************************************/
/* What you have decided to do:
 *  it has become appearent that it is hard to enforce that a pdf can not be acted upon with the same systematic twice.
 *  It is end Jack said that we should have something like  AddDist("nameOfPdf", {"sys1GroupName","sys2GroupName"}). then you can go through and on adding you say is a systamatic given twice"  
 *  If it is then that is a problem.
 *
 *
 *  You then apply this by looping over pdfs and applying each group.
 */

#ifndef __SYSTEMATIC_MANAGER__
#define __SYSTEMATIC_MANAGER__
#include <vector>
#include <Systematic.h>
#include <SparseMatrix.h>

class SystematicManager{
 public:
    SystematicManager(): fNSystematics(0) {;}
    ~SystematicManager() {}

    void Add(Systematic*,const std::string& groupName_ = "all");
    void AddPdfToGroup(const std::string& groupName_, BinnedED& ED_);

    const std::map<std::string,std::vector<Systematic*> >& GetSystematicsGroup() const;

    const size_t& GetNSystematicsInGroup(std::string & name) const;
    const std::vector<Systematic*>& GetSystematicsInGroup(std::string & name) const;

    const std::vector<std::string>& GetGroupNames(const std::string & name) const;
    
    const size_t GetNSystematics() const;

    const size_t GetNSystematicsInGroup(const std::string& name) const;

    const std::vector<Systematic*>& GetSystematicsInGroup(const std::string& name) const;

    const std::vector<std::string> GetGroups(const std::string& name) const;

    void UniqueSys(const std::vector<std::string>&);

    void AddDist(const BinnedED& pdf, const std::vector<std::string>& syss_);

    const SparseMatrix& GetTotalResponse(const std::string& groupName_ = "all" ) const;

    void DistortEDs(std::vector<BinnedED>& fWorkingEDs);

    void Construct();
    
 private:
    size_t fNSystematics;
    size_t fNGroups;
    std::map<std::string,SparseMatrix> totalResponses;
    std::map<std::string,std::vector<Systematic*> > groups;
    std::map<std::string,std::vector<std::string> > EDGroups;
    std::vector<std::string> sysOrder; // This isn't used.
};
#endif
