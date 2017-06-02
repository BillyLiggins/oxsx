#include <Minuit.h>
#include <Minuit2/MnMigrad.h>
#include <Minuit2/MnMinimize.h>
#include <Minuit2/MnSimplex.h>
#include <Minuit2/FunctionMinimum.h>
#include <Minuit2/MnUserParameters.h>
#include <FitResult.h>
#include <Exceptions.h>
#include <Formatter.hpp>

using ContainerTools::GetValues;
using ContainerTools::HasSameKeys;
using ContainerTools::GetKeys;
using ContainerTools::ToString;


using ROOT::Minuit2::MnMigrad;
using ROOT::Minuit2::MnMinimize;
using ROOT::Minuit2::MnSimplex;
using ROOT::Minuit2::MnUserParameters;

// FIXME:: CHeck the length of the vectors as you set them match fNparams

Minuit::~Minuit(){
    delete fMinimiser;
}

void
Minuit::SetMethod(const std::string& method_){
    fMethod = method_;   
}

std::string
Minuit::GetMethod() const{
    return fMethod;
}

void
Minuit::SetInitialErrors(const ParameterDict& errs_){
    fInitialErrors = errs_;
}

void
Minuit::SetInitialValues(const ParameterDict& vals_){
    fInitialValues = vals_;
}

void
Minuit::SetUpperContourEdge(double val_){
    fMinuitFCN.SetUp(val_);
}

double
Minuit::GetUpperContourEdge() const{
    return fMinuitFCN.GetUp();
}

void
Minuit::SetMaxima(const ParameterDict& maxima_) {fMaxima = maxima_;}

ParameterDict 
Minuit::GetMaxima() const {return fMaxima;}

void
Minuit::SetMinima(const ParameterDict& minima_) {fMinima = minima_;}

ParameterDict 
Minuit::GetMinima() const {return fMinima;}

void 
Minuit::Initialise(){
    delete fMinimiser;
    fMinimiser = NULL;

    // max or min?
    if(fMaximising)
        fMinuitFCN.SetSignFlip(true);

    // check everything makes sense
    if( !HasSameKeys(fMinima, fMaxima))
        throw LogicError(Formatter()
                         << "Minuit initialisation error "
                         << " minima/maxima parameters dont't match:\n"
                         << "Minima for :\n" << ToString(GetKeys(fMinima)) << "\n"
                         << "Maxima for :\n" << ToString(GetKeys(fMaxima)) << "\n"
                         );

    if( !HasSameKeys(fInitialErrors, fInitialValues))
        throw LogicError(Formatter()
                         << "Minuit initialisation error "
                         << "Initial value/error parameters dont't match:\n"
                         << "Initial Values for :\n" << ToString(GetKeys(fInitialValues)) << "\n"
                         << "Initial Errors for :\n" << ToString(GetKeys(fInitialErrors)) << "\n"
                         );
    
    // impose a set ordering on the parameters, so they can be reliably referenced (minuit does vectors we do maps)
    fParameterNames = GetKeys(fInitialErrors);
    
    // Create parameters and set limits
    MnUserParameters params(GetValues(fInitialValues, fParameterNames), GetValues(fInitialErrors, fParameterNames));

    if(fMinima.size() && fMaxima.size())
        for(size_t i = 0; i < fParameterNames.size(); i++)
            params.SetLimits(i, fMinima[fParameterNames.at(i)], fMaxima[fParameterNames.at(i)]);
    
    if("Migrad" == fMethod)
        fMinimiser = new MnMigrad(fMinuitFCN, params);
    
    else if ("Minimize" == fMethod)
        fMinimiser = new MnMinimize(fMinuitFCN, params);
    
    else if ("Simplex" == fMethod)
        fMinimiser = new MnSimplex(fMinuitFCN, params);
    else 
        throw ValueError(Formatter() << "Minuit::Unrecognised method - "
                         << fMethod
                         );
}

void
Minuit::Fix(const std::string& name_){
    fFixedParameters.insert(name_);
}

void
Minuit::Release(const std::string& name_){
    fFixedParameters.erase(name_);
}

void
Minuit::SetMaxCalls(unsigned max_) {
    fMaxCalls = max_;
}

unsigned
Minuit::GetMaxCalls() const {
    return fMaxCalls;
}

const FitResult&
Minuit::Optimise(TestStatistic* testStat_){
    testStat_ -> RegisterFitComponents();
    
    fMinuitFCN = MinuitFCN(testStat_, fParameterNames);
    Initialise();

    if(testStat_->GetParameterNames() != fParameterNames)
        throw LogicError(Formatter() << "Minuit config parameters don't match the test statistic!\n" 
                         << "TestStatistic:\n" << ToString(testStat_->GetParameterNames()) 
                         << "Minuit:\n" << ToString(fParameterNames)
                         );
    

    // fix the requested parameters
    // first work out which minuit index this refers to
    std::set<std::string>::iterator it = fFixedParameters.begin();
    for(; it != fFixedParameters.end(); ++it){
        size_t pos = std::distance(std::find(fParameterNames.begin(), fParameterNames.end(), *it), fParameterNames.begin());
        fMinimiser -> Fix(pos);
    }
    
    // defaults are same as ROOT defaults
    ROOT::Minuit2::FunctionMinimum fnMin  = fMinimiser -> operator()(fMaxCalls, fTolerance); 

    fFitResult.SetBestFit(ContainerTools::VecsToMap(fParameterNames, fMinimiser -> Params()));
    fFitResult.SetValid(fnMin.IsValid());
    return fFitResult;
}

void 
Minuit::SetTolerance(double tol_) {
  fTolerance = tol_;
}

double
Minuit::GetTolerance() const {
  return fTolerance;
}

FitResult
Minuit::GetFitResult() const{
    return fFitResult;
    
}
