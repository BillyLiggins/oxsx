#include <SystematicManager.h>
#include <Exceptions.h>
#include <Formatter.hpp>

const size_t
SystematicManager::GetNSystematicsInGroup(const std::string& name) const{
    try{
        return groups.at(name).size();
    }
    catch(const std::out_of_range& e_){
        throw NotFoundError(Formatter()<<
                "SystematicManager :: name : "<< 
                name <<
                " not found in systematic map");
    }
}

const std::vector<Systematic*>&
SystematicManager::GetSystematicsInGroup(const std::string& name) const{
    try{
        return groups.at(name);
    }
    catch(const std::out_of_range& e_){
        throw NotFoundError(Formatter()<<
                "SystematicManager :: name : "<< 
                name <<
                " not found in systematic map");
    }
}

const std::vector<std::string>
SystematicManager::GetGroups(const std::string& name) const{
    std::vector<std::string> names;
    for (std::map<std::string,std::vector<Systematic*> >::const_iterator group =groups.begin(); group != groups.end(); ++group ) {
        names.push_back(group->first);
    }
    return names;
}


const std::map<std::string, std::vector<Systematic*> >& 
SystematicManager::GetSystematicsGroup() const{
    return groups;
}

void
SystematicManager::Construct(){
    // Don't do anything if there are no systematics
    if(!groups.size())
        return;

    bool containsAll_ = false;

    //Construct the response matrices.
    for (std::map<std::string,std::vector<Systematic*> >::const_iterator group=groups.begin(); group!= groups.end(); ++group ) {
        //Over all systematics in each group
        for(size_t i = 0; i < group->second.size(); i++)
            groups[group->first].at(i) -> Construct();
        
        if(group->first =="all"){
            SparseMatrix resp = groups[group->first].at(0) -> GetResponse();
            for(size_t i = 1; i < group->second.size(); i++)
                  resp *= groups[group->first].at(i) -> GetResponse();
            totalResponses[group->first]=resp;
            containsAll_ = true;
        }
    }

    //This loop should construct groups other than the "all".
    for (std::map<std::string,std::vector<Systematic*> >::const_iterator group =groups.begin(); group != groups.end(); ++group ) {
        // If the "all" group do nothing.
        if (group->first == "all") {
            continue;
        // construct if the all group exists then build upon that
        }else if ( containsAll_ ) {
            SparseMatrix resp = totalResponses["all"];
            for(size_t i = 0; i < group->second.size(); i++)
                  resp *= groups[group->first].at(i) -> GetResponse();
            totalResponses[group->first]=resp;
        // else construct the group on its own.
        }else{
            SparseMatrix resp = groups[group->first].at(0) -> GetResponse();
            for(size_t i = 1; i < group->second.size(); i++)
                  resp *= groups[group->first].at(i) -> GetResponse();
            totalResponses[group->first]=resp;

        }
    }
}


const SparseMatrix&
SystematicManager::GetTotalResponse(const std::string& groupName_) const{
    try{
        return totalResponses.at(groupName_);
    }
    catch(const std::out_of_range& e_){
        throw NotFoundError(Formatter()<<
                "SystematicManager :: name : "<< groupName_<<
                " not found in systematic map");
    }
}
                                        
const size_t
SystematicManager::GetNSystematics() const{
    return fNSystematics;
}

void
SystematicManager::Add(Systematic* sys_, const std::string& groupName_){
     groups[groupName_].push_back(sys_);
     fNGroups++;
     fNSystematics++;
}

void 
SystematicManager::AddPdfToGroup(const std::string& groupName_, BinnedED& ED_){
    //Can you have a pdf in different groups?
    //Does the group exist?
    EDnames[groupName_].push_back(ED_.GetName());
}

void
SystematicManager::DistortEDs(std::vector<BinnedED>& fWorkingEDs_) {
    Construct();
    for(size_t j = 0; j < fWorkingEDs_.size(); j++){
        const std::string name = fWorkingEDs_.at(j).GetName();

        //If all group exist then apply that set of systematics first.
        if ( EDnames.find("all") == EDnames.end() ){
            std::vector<std::string> v = EDnames["all"];
            if (std::find(v.begin(), v.end(), name) != v.end())
                fWorkingEDs_[j].SetBinContents(GetTotalResponse("all").operator()(fWorkingEDs_.at(j).GetBinContents()));
        }

        for (std::map<std::string,std::vector<std::string> >::const_iterator group = EDnames.begin(); group != EDnames.end(); ++group) {
            if (group->first == "all")
                continue;
            std::vector<std::string> v = group->second;
            if (std::find(v.begin(), v.end(), name) != v.end())
                fWorkingEDs_[j].SetBinContents(GetTotalResponse(group->first).operator()(fWorkingEDs_.at(j).GetBinContents()));
        }
    }
}
