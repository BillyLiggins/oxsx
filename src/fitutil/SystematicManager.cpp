#include <SystematicManager.h>
#include <Exceptions.h>
#include <Formatter.hpp>
#include <ContainerTools.hpp>

const size_t
SystematicManager::GetNSystematicsInGroup(const std::string& name) const{
    try{
        return fGroups.at(name).size();
    }
    catch(const std::out_of_range& e_){
        throw NotFoundError(Formatter()<<
                "SystematicManager :: name : "<< 
                name <<
                " not found in systematic map");
    }
}

const std::vector<std::string>&
SystematicManager::GetGroupNames() const{
    std::vector<std::string> v;
    for(std::map<std::string,std::vector<Systematic*> >::const_iterator it = fGroups.begin(); it !=fGroups.end(); ++it) {
        v.push_back(it->first);
    }
    return v;
}
const std::vector<std::string>&
SystematicManager::GetSystematicsNamesInGroup(const std::string& name) const{
    try{
        std::vector<Systematic*> sys = fGroups.at(name);
        std::vector<std::string> names;
        for (int i = 0; i < sys.size(); ++i) {
            names.push_back(sys.at(i)->GetName());    
        }
        return names;
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
        return fGroups.at(name);
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
    for (std::map<std::string,std::vector<Systematic*> >::const_iterator group =fGroups.begin(); group != fGroups.end(); ++group ) {
        names.push_back(group->first);
    }
    return names;
}


const std::map<std::string, std::vector<Systematic*> >& 
SystematicManager::GetSystematicsGroup() const{
    return fGroups;
}

void
SystematicManager::Construct(){
    // Don't do anything if there are no systematics
    if(!fGroups.size())
        return;


    //Construct the response matrices.
    for (std::map<std::string,std::vector<Systematic*> >::const_iterator group=fGroups.begin(); group!= fGroups.end(); ++group ) {
        //Over default systematics in each group
        for(size_t i = 0; i < group->second.size(); i++)
            fGroups[group->first].at(i) -> Construct();
        
        if(group->first =="default"){
            SparseMatrix resp = fGroups[group->first].at(0) -> GetResponse();
            for(size_t i = 1; i < group->second.size(); i++)
                  resp *= fGroups[group->first].at(i) -> GetResponse();
            fTotalReponses[group->first]=resp;
        }
    }

    //This loop should construct fGroups other than the "default".
    for (std::map<std::string,std::vector<Systematic*> >::const_iterator group =fGroups.begin(); group != fGroups.end(); ++group ) {
        if (group->first == "default") {
            continue;
        }else{
            SparseMatrix resp = fGroups[group->first].at(0) -> GetResponse();
            for(size_t i = 1; i < group->second.size(); i++)
                  resp *= fGroups[group->first].at(i) -> GetResponse();
            fTotalReponses[group->first]=resp;

        }
    }
}


const SparseMatrix&
SystematicManager::GetTotalResponse(const std::string& groupName_) const{
    try{
        return fTotalReponses.at(groupName_);
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
     fGroups[groupName_].push_back(sys_);
     // fNGroups++;
     fNSystematics++;
}
void
SystematicManager::UniqueSystematics(const std::vector<std::string>& syss_){

    std::vector<std::string> allname;
    for (int i = 0; i <syss_.size(); ++i) {
        //What about if group doesn't exist.
        std::vector<Systematic*> group = fGroups[syss_.at(i)];
       for (int j = 0; j < group.size(); ++j) {
           if(std::find(allname.begin(),allname.end(), group.at(i)->GetName() ) == allname.end())
               allname.push_back(group.at(i)->GetName());
           else
               throw NotFoundError(Formatter()<<
                       "SystematicManager :: Systematic: "<< 
                       group.at(i)->GetName()<<
                       " is in more than one group. BAD!!!");
       } 
    }
    
}

void
SystematicManager::AddDist(const BinnedED& pdf, const std::vector<std::string>& syss_){
    UniqueSystematics(syss_);
    fEDGroups[pdf.GetName()] = syss_;
}


void
SystematicManager::DistortEDs(std::vector<BinnedED>& fWorkingEDs_) {
    for(size_t j = 0; j < fWorkingEDs_.size(); j++){
        const std::string name = fWorkingEDs_.at(j).GetName();
        
        //If default group exist then apply that set of systematics first regardless of name.
        if ( fGroups.find("default") != fGroups.end() ){
            std::vector<Systematic*> v = fGroups["default"];
            fWorkingEDs_[j].SetBinContents(GetTotalResponse("default").operator()(fWorkingEDs_.at(j).GetBinContents()));
        }

        //Apply everything else.
        for (int i = 0; i < fEDGroups[name].size(); ++i) {
            fWorkingEDs_[j].SetBinContents(GetTotalResponse(fEDGroups[name].at(i)).operator()(fWorkingEDs_.at(j).GetBinContents()));
        }

    }
}
