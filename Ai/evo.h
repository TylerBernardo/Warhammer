//
// Created by tpixe on 5/3/2022.
//

#ifndef EVOLUTIONARYALGORITHIM_EVO_H
#define EVOLUTIONARYALGORITHIM_EVO_H
#include <Eigen/Dense>
#include<cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <random>
#include <thread>
#include <chrono>
#include <fstream>
#include <sstream>
#include "newNetwork.h"
using namespace Eigen;

template <typename T>
inline std::string arrayToString(T* toConvert,int length){
    std::string output = "[";
    for(int i = 0; i < length; i++){
        output += std::to_string(toConvert[i]);
        if(i != length-1){
            output += ",";
        }
    }
    output += "]";
    return output;
}

//This function works
template <typename T>
inline T* stringToArray(std::string *toConvert,int length){
    std::stringstream convert(*toConvert);
    T* output = new T[length];
    char dummyChar = ' ';
    for(int i = 0; i < length; i++){
        convert >> dummyChar;
        convert >> output[i];
    }
    return output;
}

//pure virtual class to allow scene specific actions to be made.
class Agent{
    protected:
        int inputSpaceLength, outputSpaceLength, hiddenLayerCount;
    public:
        Network *network = nullptr;
        int reward = 0;
        //should the simulation end? Returns true when it should
        virtual bool endState(){return false;};

        virtual void printHeatMap(){};

        void save(std::string* reference){
            //this signifies the start of a new ai in the list of ais
            std::string output = "|-------------|\n";
            output += *(this->network->networkToString());
            //convert network into string here
            //agent variables will be handled by derived class
            *reference = output;
        };

        ~Agent(){
            //delete network;
        }
};
//pure virtual class
class EvoController{

    public:
    int inputSpaceLength, outputSpaceLength, hiddenLayerCount;
    int *hiddenLayers;
    Agent **agents;
    int agentCount;

        /* base variables required for evoController;
        EvoController(int _inputSpaceLength, int _outputSpaceLength, int _hiddenLayerCount){
            this->inputSpaceLength = _inputSpaceLength;
            this->outputSpaceLength = _outputSpaceLength;
            this ->hiddenLayerCount = _hiddenLayerCount;
            this->hiddenLayers = new int[this->hiddenLayerCount];
        }*/
        //should return an array of length "inputSpaceLength" that represents all the inputs for the net of the agent located at agents[i]
        virtual double* genInputSpace(int agentNumber){return nullptr;};
        //should take in the output of the neural net and determine what action to take. Should take that action, then return score. also in charge of termination the simulation.
        virtual int state(double *output, int agentNumber){return 0;};
        //creates an agent
        virtual Agent* createAgent(){return nullptr;};
        //reset agent at agents[agentNum]
        virtual void reset(){};
        //save the state of training to a text file for later use
        virtual void save(std::string const &path){
            std::ofstream outputFile;
            outputFile.open(path);
            outputFile << "ISL:" << inputSpaceLength << std::endl << "OSL:" << outputSpaceLength << std::endl << "HLC:" << hiddenLayerCount << std::endl << "AC:" << agentCount << std::endl << "HL:" << arrayToString(hiddenLayers, hiddenLayerCount) << std::endl;
            std::string* outputText = new std::string ;
            for(int i = 0; i < this->agentCount; i++){
                this->agents[i]->save(outputText);
                outputFile << *outputText << std::endl;
            }
            delete outputText;
            outputFile.close();
        };
};



Agent* learn(EvoController *controller, int populationSize, int generations, int threads );

#endif //EVOLUTIONARYALGORITHIM_EVO_H
