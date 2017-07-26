/***************************************************************************************/
/* Light weight class to represent a particular combination of the data 
   for a single event 
*/
/**************************************************************************************/
#ifndef __OBS_SET__
#define __OBS_SET__
#include <vector>
#include <string>
#include <utility>

class ObsSet{
 public:    
    ObsSet(){}
    ~ObsSet(){}
    ObsSet(const std::vector<size_t>& indices_);
    ObsSet(size_t index_);
    ObsSet(const ObsSet&);   

    std::vector<size_t> GetIndices() const; 
    void SetIndices(const std::vector<size_t>& indices_); 

    std::string GetName() const;
    void SetName(const std::string& name_);

    size_t GetIndex(size_t indexNum_) const;
    size_t GetNObservables() const;
    
    size_t GetDataIndexPos(size_t dataIndex_) const; // where is data index x in this representation
    // Call this something else
    std::vector<size_t> GetRelativeIndices(const ObsSet&) const; 
    
    // comparision
    bool operator==(const ObsSet& other_) const;
    bool operator!=(const ObsSet& other_) const;

 private:
    std::vector<size_t> fIndices;
    std::vector< std::pair<size_t, size_t> > fInverse;
    std::string fName;
    size_t fNObservables;
};
#endif
