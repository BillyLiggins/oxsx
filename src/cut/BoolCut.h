#ifndef __OXSX_BOOL_CUT__
#define __OXSX_BOOL_CUT__
#include <Cut.h>
#include <stddef.h>

class BoolCut : public Cut{
 public:
    BoolCut(size_t dim_, double value_): fDim(dim_),  fVal(value_){}
    BoolCut(const BoolCut& other_);
    virtual bool PassesCut(const Event& ev_) const;
    virtual Cut* Clone() const;
 private:
    size_t fDim;
    double fVal;
};
#endif
