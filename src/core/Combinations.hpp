#ifndef __OXSX_COMBINATIONS__
#define __OXSX_COMBINATIONS__
#include <Exceptions.h>
#include <vector>
#include <utility>


namespace Combinations{

template<typename T1>
std::vector<std::pair<T1, T1> > AllCombsNoDiag(const std::vector<T1>& v1_){
    std::vector<std::pair<T1, T1> > pairs;
    for(size_t i = 0; i < v1_.size(); i++){
        for(size_t j = 0; j < i; j++){
            pairs.push_back(make_pair(v1_.at(i), v1_.at(j)));
        }
    }
    return pairs;
}

template<typename T1>
std::vector<T1> Range(const T1& N, const T1& start = 0){
    std::vector<T1> ret;
    ret.reserve(N - start);
    for(T1 i = start; i < N; i++)
        ret.push_back(i);
    return ret;
}


}
#endif
