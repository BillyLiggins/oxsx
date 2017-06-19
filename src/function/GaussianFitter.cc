#include <GaussianFitter.h>
#include <iostream>
#include <sstream>
#include <Formatter.h>
#include <Exceptions.h>


GaussianFitter::GaussianFitter(Gaussian * func_){
   fOrignalFunc = func_; 
}

void
GaussianFitter::SetMeanNames(const std::vector<double> means_,const std::string& baseName_){

    std::stringstream ss;
    for (int i = 0; i < means_.size(); ++i) {
        ss << baseName_<< "_" << i;
        fMeans.push_back(ss.str());
        ss.str("");
    }

}
void
GaussianFitter::SetStdDevNames(const std::vector<double> stdDevs_,const std::string& baseName_){

}

// void
// GaussianFitter::RenameParameter(const std::string& old_, const std::string& new_){
//     fFitter.RenameParameter(old_, new_);
// }
//
void
GaussianFitter::SetParameter(const std::string& name_, double value_){
    std::vector<double> means = fOrignalFunc->GetMeans();
    std::vector<std::string>::iterator it
    try{
        it=find(vec.begin(),vec.end(), name_);
    }catch(const std::out_of_range& e_){
        throw NotFoundError("GaussianFitter:: div");
    }
    means.at(it-vec.end())=value_;
    fOrignalFunc->SetStdDevNames(means, fOrignalFunc->GetStDevs());
}

void
GaussianFitter::SetParameter(const std::string& name_, double value_){
    std::vector<double> means = fOrignalFunc->GetMeans();
    std::vector<double> stddevs= fOrignalFunc->GetStDevs();
    std::vector<std::string>::iterator it

    it=find(fMeans.begin(),fMeans.end(), name_);

    if(it==fMeans.end()){
        it=find(fStdDevs.begin(),fStdDevs.end(), name_);

        if(it==fStdDevs.end())
            throw NotFoundError("GaussianFitter:: div");

        stddevs.at(it-fStdDevs.end())=value_;
        fOrignalFunc->SetStdDevNames(fOrignalFunc->GetMeans(),stddevs);
        return;

    }
    means.at(it-vec.end())=value_;
    fOrignalFunc->SetStdDevNames(means, fOrignalFunc->GetStDevs());
         
}

double
GaussianFitter::GetParameter(const std::string& name_) const{
    std::vector<double> means = fOrignalFunc->GetMeans();
    std::vector<double> stddevs= fOrignalFunc->GetStDevs();
    std::vector<std::string>::iterator it

    it=find(fMeans.begin(),fMeans.end(), name_);

    if(it==fMeans.end()){
        it=find(fStdDevs.begin(),fStdDevs.end(), name_);

        if(it==fStdDevs.end())
            throw NotFoundError("GaussianFitter:: idiot");

        return stddevs.at(it-fStdDevs.end());

    }
    return means.at(it-vec.end())}
}

void
GaussianFitter::SetParameters(const ParameterDict& ps_){
    for (ParameterDict::const_iterator i = ps_.begin(); i != ps_.end(); ++i) {
        SetParameter(i->first,i->second);
    }
    
}

// ParameterDict
// GaussianFitter::GetParameters() const{
//     return fFitter.GetParameters();
// }
//
// size_t
// GaussianFitter::GetParameterCount() const{
//     return fFitter.GetParameterCount();
// }
//
// std::set<std::string>
// GaussianFitter::GetParameterNames() const{
//     return fFitter.GetParameterNames();
// }
