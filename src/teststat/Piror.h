#ifndef __OXSX_PIROR__
#define __OXSX_PIROR__
#include <vector>
#include <string>
#include <stddef.h>
#include <Function.h>
#include <Gaussian.h>
#include <ParameterDict.h>


class Piror{
    public:
        Piror(){;}
        ~Piror();
        Piror(const Piror&);
        Piror& operator=(const Piror&);
        Piror(std::vector<std::string> parameters, Function* func):
            parameterList(parameters), function(func){;}


        void SetParameterList(std::vector<std::string> dependences);
        std::vector<std::string> GetParameterList();
        void SetFunction(Function* func);
        Function* GetFunction();

        double Probability(const ParameterDict&);

    private:
        std::vector<std::string> parameterList;
        Function* function;
};
#endif

