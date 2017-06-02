#ifndef __OXSX__CONTAINER_TOOLS__
#define __OXSX__CONTAINER_TOOLS__
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <cassert>

namespace ContainerTools{

template <typename T1, typename T2>
std::map<T1, T2> VecsToMap(const std::vector<T1>& v1_, const std::vector<T2>& v2_){
    assert(v1_.size() == v2_.size());
    
    std::map<T1, T2> map;
    for(size_t i = 0; i < v1_.size(); i++)
        map[v1_.at(i)] = v2_.at(i);
    return map;
}

template<typename T1, typename T2>
std::vector<T1> GetKeys(const std::map<T1, T2> & mp_){
    typedef typename std::map<T1, T2>::const_iterator mapIt;

    std::vector<T1> vec;
    vec.reserve(mp_.size());
    for(mapIt it = mp_.begin(); it != mp_.end(); ++it){
        vec.push_back(it->first);        
    }
    return vec;
}

template<typename T1, typename T2>
std::vector<T2> GetValues(const std::map<T1, T2>& mp_, const std::vector<T1>& keys_){
    // throws std::out_of_range if there is no matching key
    std::vector<T2> vals;
    vals.reserve(keys_.size());
    for(size_t i = 0; i < keys_.size(); i++){    
            vals.push_back(mp_.at(keys_.at(i)));
    }

    return vals;
}

template<typename T1, typename T2>
std::vector<T2> GetValues(const std::map<T1, T2>& mp_){
    typedef typename std::map<T1, T2>::const_iterator mapIt;

    std::vector<T2> vec;
    vec.reserve(mp_.size());
    for(mapIt it = mp_.begin(); it != mp_.end(); ++it){
        vec.push_back(it->second);        
    }
    return vec;
}
template<typename T1, typename T2>
void SetValues(std::map<T1, T2> mp_, const std::vector<T1>& keys_, const std::vector<T2>& values_){
    typedef typename std::map<T1, T2>::iterator mapIt;

    assert(mp_.size() == keys_.size());
    assert(values_.size() == keys_.size());
    size_t index = 0;
    for(size_t i = 0; i < keys_.size(); i++)
        mp_[keys_.at(i)] = values_.at(i);
}


template<typename T1, typename T2>
bool HasSameKeys(const T1& m1_, const T2& m2_){
    return GetKeys(m1_) == GetKeys(m2_);
}

template<typename T1, typename T2>
bool HasKey(const std::map<T1, T2>& mp_, const T1& key_){    
    return mp_.find(key_) != mp_.end();
}

template<typename T1, typename T2, typename T3>
std::string CompareKeys(const std::map<T1, T2>& m1_, const std::map<T1, T3>& m2_, const std::string name1_ = "p1", const std::string& name2_ = "p2"){
    // nothing doing
    if(HasSameKeys(m1_, m2_))
        return name1_ + " and " + name2_ + " have the same keys";

    typename std::vector<T1> missingFrom2;
    
    typedef typename std::map<T1, T2>::const_iterator mapIt;

    for(mapIt it = m1_.begin(); it != m1_.end(); ++it){
        if(!HasKey(m2_, it->first))
            missingFrom2.push_back(it->first);
    }

    std::string returnStr =  "Keys in " + name1_ + "Missing from " + name2_ + " : \n";
    for(size_t i = 0; i < missingFrom2.size(); i++){
        returnStr += std::string(missingFrom2.at(i));
        returnStr += ",";
    }

    returnStr += "\n";
    return returnStr;
}

template<typename T1>
std::string ToString(const T1& c_, const std::string& delimit_ = ", "){
    std::string retStr = "";
    for(typename T1::const_iterator it = c_.begin(); it != c_.end(); ++it)
        retStr += std::string(*it) + delimit_;
    return retStr;
}

} // namespace
#endif
