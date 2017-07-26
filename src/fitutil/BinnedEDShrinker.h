/***************************************************************************************************/
/* This class is responsible for shrinking pdfs to a subset of their normal range by truncating or */
/* rebinning into over/underflows. This is typically used to create a buffer around the fit        */
/* region. So that systematics can affect the boundary in a smooth physical way                    */
/***************************************************************************************************/

#ifndef __OXSX_BINNED_ED_SHRINKER__
#define __OXSX_BINNED_ED_SHRINKER__
#include <BinnedED.h>
#include <map>
#include <utility> // std::pair


class BinnedEDShrinker{
 public:
    BinnedEDShrinker();
    ~BinnedEDShrinker(){}

    BinAxis ShrinkAxis(const BinAxis&, const unsigned lowerBuff_, 
                       const unsigned upperBuff_) const;
    BinnedED ShrinkDist(const BinnedED& dist_) const;
    
    void SetBuffer(size_t dim_, unsigned lowerBuf_, unsigned upperBuf_);
    std::pair<unsigned, unsigned> GetBuffer(size_t dim_) const;
    
    std::map<size_t, std::pair<unsigned, unsigned> > GetBuffers() const;
     
    void SetUsingOverflows(bool b_);
    bool GetUsingOverflows() const;

 private:
    // Pairs of lower/upper buffer sizes in number of bins, keyed by diminension to shrink
    std::map<size_t, std::pair<unsigned, unsigned> > fBuffers; 
    bool fUsingOverflows; // false at initialisation
};
#endif
