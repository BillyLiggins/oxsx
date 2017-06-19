#include <GaussianFitter.h>
#include <Gaussian.h>
#include <iostream>
#include <sstream>
#include <Formatter.h>
#include <Exceptions.h>
#include <algorithm>


GaussianFitter::GaussianFitter(Gaussian * func_){
   fOrignalFunc = func_; 
}

void
GaussianFitter::SetOriginalFunction(Gaussian* func_){
    fOrignalFunc =  func_;
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
GaussianFitter::RenameParameter(const std::string& old_, const std::string& new_){
    std::vector<std::string>::iterator it
    it=find(fMeans.begin(),fMeans.end(), name_);

    if(it==fMeans.end()){
        it=find(fStdDevs.begin(),fStdDevs.end(), name_);

        if(it==fStdDevs.end())
            throw NotFoundError("GaussianFitter:: div");

        it=new_;
        return;
    }
    it=new_;
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
    std::vector<std::string>::const_iterator it;

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

ParameterDict
GaussianFitter::GetParameters() const{

    std::vector<double> means = fOrignalFunc->GetMeans();
    std::vector<double> stddevs= fOrignalFunc->GetStDevs();
    std::vector<double> values;
    values.reserve( means.size() +stddevs.size() ); // preallocate memory
    values.insert( values.end(), means.begin(), fMeans.end() );
    values.insert( values.end(), stddevs.begin(), stddevs.end() );

    std::vector<double> names;
    names.reserve( fMeans.size() + fStdDevs.size() ); // preallocate memory
    names.insert( names.end(), fMeans.begin(), fMeans.end() );
    names.insert( names.end(), fStdDevs.begin(), fStdDevs.end() );

    return ContainerTools::CreateMap(names,values);


}

size_t
GaussianFitter::GetParameterCount() const{

    return fMeans.size()+fStdDevs.size();
}

std::set<std::string>
GaussianFitter::GetParameterNames() const{
    std::set<std::string> names;
    for (int i = 0; i < fMeans.size(); ++i) {
        names.insert(fMeans.at(i));
        names.insert(fStdDevs.at(i));
    }
    // for (int i = 0; i <fStdDevs.size(); ++i) {
    //     names.insert(fStdDevs.at(i));
    // }

    return names;
}
