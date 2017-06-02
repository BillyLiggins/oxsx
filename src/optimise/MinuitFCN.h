/*************************************************/
/* Wrap an evaluation routine for minuit to call */
/*************************************************/
#ifndef __MINUIT_FCN__
#define __MINUIT_FCN__
#include <TestStatistic.h>
#include <Minuit2/FCNBase.h>
#include <ContainerTools.hpp>
#include <Exceptions.h>

class MinuitFCN : public ROOT::Minuit2::FCNBase{
 public:
    MinuitFCN() :pTestStatistic(NULL), fUp(0.5), fFlipSign(false) {}
    MinuitFCN(TestStatistic* statistic_, const std::vector<std::string>& names_) : pTestStatistic(statistic_), 
                                                                                   fUp(0.5), fFlipSign(false),
                                                                                   fParameterNames(names_){}
    // the second argument above is to absolutely ensure that the parameters are interpreted in the right order

    //these two required by minit
    double Up() const {return fUp;} 
    double operator()(const std::vector<double>& paramVals_) const {
        if(!pTestStatistic)
            throw NULLPointerAccessError("Minuit is trying to optimise a NULL TestStatistic* !");
        
        ContainerTools::SetValues(fSetParameters, fParameterNames, paramVals_);
        pTestStatistic->SetParameters(fSetParameters);
        if(fFlipSign)
            return -1 * pTestStatistic->Evaluate();

        return pTestStatistic -> Evaluate();
    }


    void   SetUp(double up_) {fUp = up_;}
    double GetUp() const {return Up();}

    void SetSignFlip(bool b_) {fFlipSign = b_;}
    bool GetSignFlip() const {return fFlipSign;}

 private:
    TestStatistic* pTestStatistic;
    double fUp;
    bool   fFlipSign;  // if true, result is multiplied by -ve 1. changes minimisation to maximisation
    ParameterDict fSetParameters; // edit this map in place to avoid recreating it constantly
    std::vector<std::string> fParameterNames;

    void Initialise(){fSetParameters = pTestStatistic -> GetParameters();}
    
};
#endif
