#include <Piror.h>
#include <math.h>
#include <PirorManager.h>
#include <iostream>

PirorManager::PirorManager(const PirorManager& other_ ){
    pirorList =  other_.pirorList;
}

PirorManager& PirorManager::operator=(const PirorManager& other_)
    {
        pirorList =  other_.pirorList;
        return *this;
    }

void
PirorManager::AddPiror(Piror piror){
    pirorList.push_back(piror);
}

std::vector<Piror>
PirorManager::GetPirors(){
    return pirorList;
}

double PirorManager::GetProbabilities( const ParameterDict& params_){
    double result=0;
    //Loop over all pirors
    for (std::vector<Piror>::iterator iter = pirorList.begin(); iter != pirorList.end(); ++iter ) {
        result += (*iter).Probability(params_);
    }
    return result;

}


double PirorManager::GetLogProbabilities( const ParameterDict& params_){
    double result=0;
    for (std::vector<Piror>::iterator iter = pirorList.begin(); iter != pirorList.end(); ++iter ) {
        result += log((*iter).Probability(params_));
    }
    return result;

}
