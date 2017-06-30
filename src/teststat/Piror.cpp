#include <Piror.h>
#include <iostream>
#include <Exceptions.h>
#include <algorithm>

Piror::Piror(const Piror& other_){
    //What do you need to copy over?
    parameterList= other_.parameterList;
    function = dynamic_cast<Function*>(other_.function->Clone());
}

Piror& Piror::operator=(const Piror& other_)
    {
        parameterList= other_.parameterList;
        function = dynamic_cast<Function*>(other_.function->Clone());
        return *this;
    }

Piror::~Piror(){
    delete function;
}

void 
Piror::SetParameterList(std::vector<std::string> parameters){
    parameterList = parameters;
}

std::vector<std::string>
Piror::GetParameterList(){
    return parameterList;
}

void
Piror::SetFunction(Function* func){
    function=func;
}

Function*
Piror::GetFunction(){
    return function;
}

double
Piror::Probability(const ParameterDict& param_){
    std::vector<double> parameters;
    for (std::vector<std::string>::const_iterator para_ = parameterList.begin(); para_ != parameterList.end(); ++para_) {
        std::vector<std::string>::iterator it;
        it = find(parameterList.begin(),parameterList.end(),*para_);
        if(it!=parameterList.end())
            // parameters.push_back(*para_);
            parameters.push_back(param_.at(*it));
    }
    if (parameters.size()!=parameterList.size())
        throw std::runtime_error("Piror:: Number of parameters provided not equal to the number expected.");
    return (*function)(parameters);
}
