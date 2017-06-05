#ifndef __OXSX_SUMMED_STATISTIC__
#define __OXSX_SUMMED_STATISTIC__
#include <TestStatistic.h>

class StatisticSum : public TestStatistic{
 public:
    StatisticSum() {}
    StatisticSum(TestStatistic&, TestStatistic&);
    void AddStat(TestStatistic&);

    // Test Statistic Interface
    virtual double Evaluate();
    virtual void   SetParameters(const ParameterDict& params_);
    virtual ParameterDict GetParameters() const;
    virtual int    GetParameterCount() const;

    virtual std::vector<std::string> GetParameterNames() const;
    
    // Set up all the components for a fit
    virtual void RegisterFitComponents();
    
 private:
    std::vector<TestStatistic*> fStats;
};

StatisticSum operator + (TestStatistic&, TestStatistic&);
StatisticSum operator + (const StatisticSum&, TestStatistic&);
StatisticSum Sum(const std::vector<TestStatistic*>&);
#endif
