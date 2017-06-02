#include <HistTools.h>
#include <Histogram.h>
#include <ContainerTools.hpp>

std::vector<Histogram>
HistTools::MakeAllHists(const AxisCollection& axes_,
                        const std::vector<std::pair<std::string, std::string> >& combinations_){
    std::vector<Histogram> hists;
    hists.reserve(combinations_.size());

    for(size_t i = 0; i < combinations_.size(); i++){
        const std::pair<std::string, std::string>& dims = combinations_.at(i);
        AxisCollection axes;
        axes.AddAxis(axes_.GetAxis(dims.first));
        axes.AddAxis(axes_.GetAxis(dims.second));
            
        hists.push_back(Histogram(axes));
    }
    return hists;
}

void
HistTools::FillAllHists(std::vector<Histogram>& hists_, const std::map<std::string, double>& fillVals_){
    // note this assumes that the histograms have axes that match the keys of fillvals throws otherwise
    for(size_t i = 0; i < hists_.size(); i++){
        hists_.at(i).Fill(fillVals_);
    }    
}


std::vector<Histogram> 
HistTools::MakeAllHists(const AxisCollection& axes_,
                        const std::vector<std::string>& names_){
    std::vector<Histogram> hists;
    hists.reserve(names_.size());
    for(size_t i = 0; i < names_.size(); i++){
        AxisCollection axes;
        axes.AddAxis(axes_.GetAxis(names_.at(i)));
        hists.push_back(Histogram(axes));
    }
    return hists;
}
