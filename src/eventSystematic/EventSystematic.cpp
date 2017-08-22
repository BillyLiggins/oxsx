#include <EventSystematic.h>

void
EventSystematic::SetObservables(const ObsSet& rep_){
    fObservables = rep_;
}

ObsSet
EventSystematic::GetObservables() const{
    return fObservables;
}
