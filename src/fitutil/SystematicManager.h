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
    SystematicManager() {;}
    ~SystematicManager() {}

    void Add(Systematic*,const std::string& groupName_ = "default");

    const std::map<std::string,std::vector<Systematic*> >& GetSystematicsGroup() const;

    const std::vector<Systematic*>& GetSystematicsInGroup(std::string & name) const;

    const std::vector<std::string> GetGroupNames() const;
    
    const size_t GetNSystematics() const;
    const size_t GetNGroups() const;

    const size_t GetNSystematicsInGroup(const std::string& name_) const;

    const std::vector<Systematic*>& GetSystematicsInGroup(const std::string& name) const;

    const std::vector<std::string>& GetSystematicsNamesInGroup(const std::string& name) const;

    const std::vector<std::string> GetGroups(const std::string& name) const;

    void AddDist(const BinnedED& pdf, const std::vector<std::string>& syss_);

    const SparseMatrix& GetTotalResponse(const std::string& groupName_ = "default" ) const;

    void DistortEDs(std::vector<BinnedED>& fWorkingEDs) const;

    void Construct();
    
 private:
    size_t fNGroups;
    std::map<std::string,SparseMatrix> fTotalReponses;
    std::map<std::string,std::vector<Systematic*> > fGroups;

    std::map<std::string,std::vector<std::string> > fEDGroups;
    void UniqueSystematics(const std::vector<std::string>&);
    const size_t CountNSystematics() const;
};
#endif
