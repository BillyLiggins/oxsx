#include <DataSet.h>
#include <ObsSet.h>
#include <Exceptions.h>
#include <string>
#include <vector>
#include <algorithm>
typedef std::vector<std::string> StringVec;

ObsSet 
DataSet::MakeDataRep(const StringVec observableNames_) const{
    std::vector<size_t> indicies;
    indicies.reserve(observableNames_.size());
    
    StringVec availableNames = GetObservableNames();
    for(size_t i = 0; i < observableNames_.size(); i++){
        StringVec::iterator pos = std::find(availableNames.begin(),
                                            availableNames.end(),
                                            observableNames_.at(i)
                                            );
        if(pos == availableNames.end())
            throw RepresentationError(Formatter() << "DataSet::Asked for representation containing non-existent variable " << observableNames_.at(i));
        else
            indicies.push_back(pos - availableNames.begin());
    }
    return ObsSet(indicies);
}


