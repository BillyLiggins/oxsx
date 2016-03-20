#include <LineCut.h>
#include <EventData.h>
#include <PdfExceptions.h>

LineCut::LineCut(size_t dim_, double val_, 
                 const std::string& side_){
    fDim = dim_;
    fVal = val_;
    SetSidedness(side_);
}
Cut* 
LineCut::Clone() const{
    return new LineCut(*this);
}

bool 
LineCut::PassesCut(const EventData& ev_) const{
    double val = 0;
    try{
        val = ev_.GetDatum(fDim);
    }
    catch(const DimensionError&){
        throw DimensionError("Cut::Cut to non-existent data observable requested!");
    }
    
    if (fSidedness == "lower")
        return val > fVal;
    else
        return val < fVal;
}
    
void   
LineCut::SetCutValue(double val_){
    fVal = val_;
}

double 
LineCut::GetCutValue() const{
    return fVal;
}

void 
LineCut::SetSidedness(const std::string& side_){
    if(side_ == "upper" || side_ == "lower")
        fSidedness = side_;
    else
        throw std::runtime_error("LineCut::Invalid sidedness! should be 'upper' or 'lower'");
} 

std::string 
LineCut::GetSidedness() const{
    return fSidedness;
}
