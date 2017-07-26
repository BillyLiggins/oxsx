#ifndef __OXSX_BOX_CUT__
#define __OXSX_BOX_CUT__
#include <Event.h>
#include <Cut.h>

class BoxCut : public Cut{
 public:
    BoxCut(size_t dim_, double lower_, double upper_) : fDim(dim_), fUpperLim(upper_), 
                                                        fLowerLim(lower_) {}

    virtual Cut* Clone() const;

    virtual bool PassesCut(const Event& ev_) const;
    
    void    SetLowerLimit(double lower_) {fLowerLim = lower_;} 
    double  GetLowerLimit() const {return fLowerLim;}

    void    SetUpperLimit(double upper_) {fUpperLim = upper_;} 
    double  GetUpperLimit() const {return fUpperLim;}
   

 private:
    size_t fDim;
    double fUpperLim;
    double fLowerLim;
    
};
#endif
