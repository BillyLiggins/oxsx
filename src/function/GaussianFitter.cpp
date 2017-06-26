#include <GaussianFitter.h>
#include <Gaussian.h>
#include <iostream>
#include <sstream>
#include <Formatter.hpp>
#include <ContainerTools.hpp>
#include <Exceptions.h>
#include <algorithm>

GaussianFitter::GaussianFitter(Gaussian* gaus, const size_t& nDims_){
    fOrignalFunc = gaus; 
    std::stringstream ss;
    for (int i = 0; i <nDims_; ++i) {
        ss << "means" << "_" << i;
        fMeansNames.push_back(ss.str());
        ss.str("");
        ss << "stddevs" << "_" << i;
        fStdDevsNames.push_back(ss.str());
        ss.str("");
    }
}

void
GaussianFitter::RenameParameter(const std::string& old_, const std::string& new_){
    std::vector<std::string>::iterator it;
    it=find(fMeansNames.begin(),fMeansNames.end(),old_);

    if(it==fMeansNames.end()){
        it=find(fStdDevsNames.begin(),fStdDevsNames.end(), old_);
        if(it==fStdDevsNames.end())
            throw NotFoundError(Formatter()<<"GaussianFitter:: When renaming the original name wasn't found : "<<
                    old_);
        *it=new_;
        return;
    }
    *it=new_;
}

void
GaussianFitter::SetParameter(const std::string& name_, double value_){
    std::vector<std::string>::iterator it;

    it=find(fMeansNames.begin(),fMeansNames.end(), name_);

    if(it==fMeansNames.end()){

        it=find(fStdDevsNames.begin(),fStdDevsNames.end(), name_);

        if(it==fStdDevsNames.end())
            //This should use compare keys.
            return;

        fOrignalFunc->SetStDev(it-fStdDevsNames.begin(),value_);
        return;
    }
    fOrignalFunc->SetMean(it-fMeansNames.begin(),value_);
}

double
GaussianFitter::GetParameter(const std::string& name_) const{
    // std::vector<double> means = fOrignalFunc->GetMeans();
    // std::vector<double> stddevs= fOrignalFunc->GetStDevs();
    std::vector<std::string>::const_iterator it;

    it=find(fMeansNames.begin(),fMeansNames.end(), name_);

    if(it==fMeansNames.end()){
        it=find(fStdDevsNames.begin(),fStdDevsNames.end(), name_);

        if(it==fStdDevsNames.end())
            throw NotFoundError(Formatter()<<"GaussianFitter:: Parameter : "<<
                                name_<<
                                " was not known to the GaussianFitter.");

        // return stddevs.at(it-fStdDevsNames.end());
        return fOrignalFunc->GetStDev(it-fStdDevsNames.end());
    }
    return fOrignalFunc->GetMean(it-fStdDevsNames.end());
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

    values.reserve( means.size() + stddevs.size() ); // preallocate memory
    values.insert( values.end(), means.begin(), means.end() );
    values.insert( values.end(), stddevs.begin(), stddevs.end() );

    std::vector<std::string> names;
    names.reserve( fMeansNames.size() + fStdDevsNames.size() ); // preallocate memory
    names.insert( names.end(), fMeansNames.begin(), fMeansNames.end() );
    names.insert( names.end(), fStdDevsNames.begin(), fStdDevsNames.end() );

    return ContainerTools::CreateMap(names,values);
}

size_t
GaussianFitter::GetParameterCount() const{
    return fMeansNames.size()+fStdDevsNames.size();
}

std::set<std::string>
GaussianFitter::GetParameterNames() const{
    std::set<std::string> names;
    for (int i = 0; i < fMeansNames.size(); ++i) {
        names.insert(fMeansNames.at(i));
        names.insert(fStdDevsNames.at(i));
    }

    return names;
}

