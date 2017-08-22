#include <ObsSet.h>
#include <Exceptions.h>
#include <iostream>

ObsSet::ObsSet(const std::vector<size_t>& indices_){
    SetIndices(indices_);
}

ObsSet::ObsSet(size_t index_){
    SetIndices(std::vector<size_t>(1, index_));
}

ObsSet::ObsSet(const ObsSet& other_){
    fName = other_.fName;
    SetIndices(other_.fIndices);
}


std::vector<size_t>
ObsSet::GetIndices() const {return fIndices;}

void 
ObsSet::SetIndices(const std::vector<size_t>& indices_)  {
    fIndices = indices_;
    fNObservables = indices_.size();

    // build inverse map
    fInverse.resize(0);
    for(size_t i = 0; i < fIndices.size(); i++)
        fInverse.push_back(std::pair<size_t,size_t>(fIndices.at(i), i));
    
}

std::string
ObsSet::GetName() const {return fName;}

void
ObsSet::SetName(const std::string& name_) {fName = name_;}

// FIXME -  needs bounds handling!
size_t
ObsSet::GetIndex(size_t indexNum_) const{return fIndices.at(indexNum_);}

size_t
ObsSet::GetNObservables() const {return fNObservables;}

size_t
ObsSet::GetDataIndexPos(size_t dataIndex_) const{
    for(size_t i = 0; i < fInverse.size(); i++){
        if (dataIndex_ == fInverse.at(i).first)
            return fInverse.at(i).second;
    }
    throw RepresentationError("Incompatible data representation"); // 
}

std::vector<size_t> 
ObsSet::GetRelativeIndices(const ObsSet& otherRep_) const{
    // given a longer represetation containing the indices of this rep, where are the shared indices
    // in the big rep

    if(otherRep_.fNObservables < fNObservables)
        throw RepresentationError("Tried to take relatice indices in compatible representations ");
        
    std::vector<size_t> relativeIndices(fIndices.size(), 0);
    for(size_t i = 0; i < fIndices.size(); i++)
        relativeIndices[i] = otherRep_.GetDataIndexPos(fIndices.at(i));
    
    return relativeIndices;
} 

bool
ObsSet::operator==(const ObsSet& other_) const{
    return fIndices == other_.fIndices;
}

bool
ObsSet::operator!=(const ObsSet& other_) const{
    return !(*this == other_);
}
