#ifndef __OXSX_CUT__
#define __OXSX_CUT__
class Event;

class Cut{
 public:
  virtual bool PassesCut(const Event& ev_) const = 0;
  virtual Cut* Clone() const = 0;
  virtual ~Cut() {}
};
#endif
