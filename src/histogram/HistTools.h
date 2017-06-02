#ifndef __OXSX_HIST_TOOLS__
#define __OXSX_HIST_TOOLS__
#include <vector>
#include <map>
#include <string>


class AxisCollection;
class Histogram;

class HistTools{
 public:
    static std::vector<Histogram> MakeAllHists(const AxisCollection&, 
                                               const std::vector<std::pair<std::string, std::string> >& combinations_);

    static std::vector<Histogram> MakeAllHists(const AxisCollection&, 
                                               const std::vector<std::string>& names_);

    static void FillAllHists(std::vector<Histogram>&, const std::map<std::string, double>& fillvals);
};
#endif
