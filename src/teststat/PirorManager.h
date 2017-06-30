#ifndef __OXSX_PIROR_MANAGER__
#define __OXSX_PIROR_MANAGER__
#include <vector>
#include <map>
#include <string>
#include <stddef.h>
#include <Function.h>
#include <Piror.h>
#include <ParameterDict.h>

class PirorManager{
    public:
        PirorManager(){;}
        PirorManager& operator=(const PirorManager& other_);
        PirorManager(const PirorManager&);

        PirorManager(std::vector<Piror>& initList){
            pirorList=initList;
        }

        void AddPiror(Piror piror);
        std::vector<Piror> GetPirors();
        // size_t GetNPirors();
        double GetProbabilities( const ParameterDict& params_);
        double GetLogProbabilities( const ParameterDict& params_);

    private:
        std::vector<Piror> pirorList;
};
#endif

