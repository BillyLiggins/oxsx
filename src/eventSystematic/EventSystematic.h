#ifndef __OXSX_EVENT_SYSTEMATIC__
#define __OXSX_EVENT_SYSTEMATIC__
#include <ObsSet.h>
#include <FitComponent.h>
#include <vector>

class Event;
class EventSystematic : public FitComponent{
 public:
    virtual Event operator()(const Event&) = 0;

    void   SetObservables(const ObsSet&);
    ObsSet GetObservables() const;
    
 protected:   
    ObsSet  fObservables;
};
#endif
