#include <MinuitFCN.h>

double
MinuitFCN::operator()(const std::vector<double>& paramVals_) const {
    if(!pTestStatistic)
        throw NULLPointerAccessError("Minuit is trying to optimise a NULL TestStatistic* !");

    ContainerTools::SetValues(fSetParameters, fParameterNames, paramVals_);
    std::cout <<"minuit setting ";
    for (ParameterDict::const_iterator i =fSetParameters.begin(); i !=fSetParameters.end(); ++i) {
        std::cout<< i->first<< " = " << i->second << " " ;
    }
    std::cout  << std::endl;
    pTestStatistic->SetParameters(fSetParameters);
    if(fFlipSign)
            return -1 * pTestStatistic->Evaluate();

    return pTestStatistic -> Evaluate();
}

MinuitFCN&
MinuitFCN::operator=(const MinuitFCN& other_){
    if(this == &other_)
        return *this;
    
    pTestStatistic = other_.pTestStatistic;
    fUp = other_.fUp;
    fFlipSign = other_.fFlipSign;
    fParameterNames = other_.fParameterNames;
    
    // don't bother copying fSetParameters        
    return *this;
}

MinuitFCN::MinuitFCN(const MinuitFCN& other_){
    pTestStatistic = other_.pTestStatistic;
    fUp = other_.fUp;
    fFlipSign = other_.fFlipSign;
    fParameterNames = other_.fParameterNames;

    // don't bother copying fSetParameters    
}
