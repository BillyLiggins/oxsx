/* Example shows how to create Prior objects which can be used to constrain
 * floated variable by applying a penalty term to the fitting function. The
 * magnitude of the penalty term is given by the by a class inheriting from the
 * Function class (interface).
 *
 * Once a Prior is set up it may be added to a fitting function by calling the
 * AddPrior function.
 *
*
 */

#include <iostream>
#include <iomanip>
#include <Prior.h>
#include <Function.h>
#include <PriorManager.h>
#include <Gaussian.h>
#include <ParameterDict.h>
#include <math.h>
#include <TRandom3.h>
class Func: public Function
{
    public:
        Func(int num):Function(), fNdims(num){;}
        double operator() (const std::vector<double>& params) const{
          double exponent = 0;
          double mean;
          double stdDev =1;
          double nDevs;

          double diff = params[0];
          for (int i = 1; i < fNdims; ++i) {
            diff -= params[i];
          }
          nDevs = (diff)/stdDev;
          exponent += nDevs * nDevs;

          double norm = 1;
          norm *= sqrt(2*M_PI)*stdDev;

          return exp(- 0.5* exponent) / norm; 
        }
        void SetParameter(const std::string&, double){;}
        double GetParameter(const std::string&) const{;}
        void SetParameters(const ParameterDict&){;}
        ParameterDict GetParameters() const{;}
        size_t GetParameterCount() const{;}
        void RenameParameter(const std::string&, const std::string&){;}
        std::string GetName() const{;}
        void SetName(const std::string&){;}
        Function* Clone() const{
          return static_cast<Function*> (new Func(fNdims));
        }
        int GetNDims() const{;}
        std::set<std::string> GetParameterNames() const{;}

    private:
        int fNdims;
};

int main(int argc, char *argv[])
{
    std::cout << "Starting prior example" << std::endl; 

    ParameterDict dict;
    dict["alpha"]  =10;
    dict["beta"]   =2;
    dict["gamma"]  =4;
    dict["delta"]  =5;
    dict["epsilon"]=9;

    // The function relating the primary variable value to its dependent can be
    // any user defined function that inherits from Function. 
    //
    // The vector that the function acts on must be of the form {fPrimary,dep_1,dep_2,...} 
    //
    Func* func= new Func(2);
    Prior* pir = new Prior();
    pir->SetFunction(func);
    // Set the primary variable of by name. 
    pir->SetPrimary("alpha");
    // Set the dependent variable by passing a vector of parameter names. It
    // is important that the order matchs that of the dependants in the function
    // definition.

    std::vector<std::string> deps;
    deps.push_back("beta");
    pir->SetDependents(deps);
    

    //Now everything is setup
    //To add to a fitting function e.g. BinnedNLLH. Use
    //the AddPrior function. e.g. lh.AddPrior(&pir); 

    {
      // This is a simple optimisation loop. Just to give some kind of output
      // showing that changing the ParameterDict passed to the probability
      // method returns 
  
      double perviousProb = 0;
      bool Qup =false;
      TRandom3 rand;

      std::cout<<"\nStarting dummy optimisation"<<std::endl;
      for (int i = 0; i < 25; ++i) {
        if(i==0){
          double randd= rand.Uniform(-1,1);
          dict["alpha"] = dict["alpha"]+randd;
          if(randd<0)
            Qup = false;
          else
            Qup = true;
        }
        double prob =pir->Probability(dict); 
        std::cout<<"alpha = "<<std::setprecision(3)<< dict["alpha"]<< std::fixed 
        << std::setw(10)<< " beta = "<< dict["beta"] <<std::setprecision(6)
        << " prob = "<< prob << std::setprecision(3)
        <<"\t => Term added to a BinnedNLLH = "<< log(prob)<<std::endl;
        if(prob < perviousProb){
          if(Qup == true){
            dict["alpha"]  =dict["alpha"]-rand.Uniform();
            Qup= false;
          }else{
            dict["alpha"]  =dict["alpha"]+rand.Uniform();
            Qup= true;
          }

        }else{
          if(Qup == true){
            dict["alpha"]  =dict["alpha"]+rand.Uniform();
          }else{
            dict["alpha"]  =dict["alpha"]-rand.Uniform();
          }
        }
        perviousProb = prob;
      }
    }
    

    Func* func_2= new Func(3);
    std::vector<std::string> deps_2;
    deps_2.push_back("gamma");
    deps_2.push_back("delta");
    Prior* pir2 = new Prior("beta",deps_2,func_2);

    {
      double perviousProb = 0;
      bool Qup =false;
      TRandom3 rand;
      std::cout<<"\nStarting second dummy optimisation"<<std::endl;
      for (int i = 0; i < 25; ++i) {
        if(i==0){
          double randd= rand.Uniform(-1,1);
          dict["beta"] = dict["beta"]+randd;
          if(randd<0)
            Qup = false;
          else
            Qup = true;
        }
        double prob =pir2->Probability(dict); 
        std::cout<<"beta = "<<std::setprecision(3)<< dict["beta"]<< std::fixed 
        << std::setw(10)<< " gamma = "<< dict["gamma"] << " delta = "<< dict["delta"] <<std::setprecision(6)
        << " prob = "<< prob << std::setprecision(3)
        <<"\t => Term added to a BinnedNLLH = "<< log(prob)<<std::endl;
        if(prob < perviousProb){
          if(Qup == true){
            dict["beta"]  =dict["beta"]-rand.Uniform();
            Qup= false;
          }else{
            dict["beta"]  =dict["beta"]+rand.Uniform();
            Qup= true;
          }
          
        }else{
          if(Qup == true){
            dict["beta"]  =dict["beta"]+rand.Uniform();
          }else{
            dict["beta"]  =dict["beta"]-rand.Uniform();
          }
        }
        perviousProb = prob;
      }
    }

    return 0;
}
