#include <catch.hpp>
#include <ObsSet.h>
#include <OXSXDataSet.h>

TEST_CASE("Build from one index"){
     ObsSet drep(19);
    REQUIRE(drep.GetNObservables() == 1);
    REQUIRE(drep.GetIndex(0) == 19);
}

TEST_CASE("Build from vec"){
    std::vector<size_t> indices;
    indices.push_back(5);
    indices.push_back(9);
    indices.push_back(11);

     ObsSet drep(indices);
    REQUIRE(drep.GetNObservables() == 3);
    REQUIRE(drep.GetIndex(0) == 5);
    REQUIRE(drep.GetIndex(1) == 9);
    REQUIRE(drep.GetIndex(2) == 11);

    SECTION("Then copy construct"){
         ObsSet drep2(drep);
        REQUIRE(drep2.GetNObservables() == 3);
        REQUIRE(drep2.GetIndex(0) == 5);
        REQUIRE(drep2.GetIndex(1) == 9);
        REQUIRE(drep2.GetIndex(2) == 11);
        
    }

    SECTION("Testing Inverse Functionality"){
        REQUIRE(drep.GetDataIndexPos(11) == 2);
        REQUIRE(drep.GetDataIndexPos(5) == 0);
        REQUIRE(drep.GetDataIndexPos(9) == 1);
    }


}


TEST_CASE("Looking for the relative indices of 2d rep in 4d rep"){
    std::vector<size_t> smallRepIndices;
    smallRepIndices.push_back(6);
    smallRepIndices.push_back(0);

    std::vector<size_t> bigRepIndices;
    bigRepIndices.push_back(0);
    bigRepIndices.push_back(3);
    bigRepIndices.push_back(6);

     ObsSet bigRep(bigRepIndices);
     ObsSet smallRep(smallRepIndices);

    std::vector<size_t> relativeIndices = smallRep.GetRelativeIndices(bigRep);

    REQUIRE(relativeIndices.size() == 2);
    REQUIRE(relativeIndices.at(0) == 2);
    REQUIRE(relativeIndices.at(1) == 0);
    
}

TEST_CASE("Creating a  ObsSet from a DataSet by observable name"){
    // create a dummy dataset with some observables in it
    std::vector<std::string> observables;
    observables.push_back("obs_1");
    observables.push_back("obs_2");
    observables.push_back("obs_3");
    observables.push_back("obs_4");

    OXSXDataSet dataSet;
    dataSet.SetObservableNames(observables);
    
    // now ask the data set for the representation corresponding to obs_4, obs_3, obs_2
    std::vector<std::string> requestedObs;
    requestedObs.push_back("obs_4");
    requestedObs.push_back("obs_3");
    requestedObs.push_back("obs_2");

     ObsSet generatedRep = dataSet.MakeDataRep(requestedObs);

    //check it did it right
    REQUIRE(generatedRep.GetNObservables() == requestedObs.size());
    for(size_t i = 0; i < requestedObs.size(); i++){
        size_t generatedIndex = generatedRep.GetIndices().at(i);
        std::string generatedName = observables.at(generatedIndex);
        REQUIRE(generatedName == requestedObs.at(i));
        
    }
}

                
