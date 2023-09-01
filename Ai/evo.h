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

/*
 * HOW TO USE EVOLUTIONARY LEARNING FRAMEWORK
 * The basis of this implementation revolves around two classes, Agent and EvoController.
 * EvoController controls the learning of agents, and mutating them. It builds the population, runs simulations, and modifies agents
 * Agent is a single member of a population. It holds the weights and biases of a single network, holds metadata about that network and the agents performance. It also holds information about the state of the environment.
 *
 * When implementing the derived classes for these two classes, implement each of the virtual functions for whatever scenario you need. The only code you will have to write is the code specific to your training environment.
 * More information about each function will be listed by that function
 * NOTE: Ignore any code related to saving for now, it has yet to be fully implemented
 */

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

//Hold information about an individual agent
class Agent{
    protected:
        int inputSpaceLength, outputSpaceLength, hiddenLayerCount;
    public:
        Network *network = nullptr;
        int reward = 0;
        //This function is used to determine if an agent should exit a simulation. This often either means a timer is hit or the agent has succeeded/failed at its task.
        //Should return true if the agent should exit its simulation
        virtual bool endState(){return true;};

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
//
class EvoController{

    public:
        int inputSpaceLength, outputSpaceLength, hiddenLayerCount;
        int *hiddenLayers = nullptr;
        Agent **agents = nullptr;
        int agentCount;

        /* base variables required for evoController;
        EvoController(int _inputSpaceLength, int _outputSpaceLength, int _hiddenLayerCount){
            this->inputSpaceLength = _inputSpaceLength;
            this->outputSpaceLength = _outputSpaceLength;
            this ->hiddenLayerCount = _hiddenLayerCount;
            this->hiddenLayers = new int[this->hiddenLayerCount];
        }*/
        //This function is responsible for being the "eyes" of an agent. It should access the enviorment of agent "agentNumber" and gather all the inputs for the neural network
        //should return an array of length "inputSpaceLength" that represents all the inputs for the net of the agent located at agents[i]
        virtual double* genInputSpace(int agentNumber){return nullptr;};

        //should take in the output of the neural net and determine what action to take. Should take that action, then return score. also in charge of termination the simulation.
        //Returns the reward for agent "agentNumber"
        virtual int state(double *output, int agentNumber){return 0;};

        //Should create a fresh agent for use in the training process
        //returns an agent
        virtual Agent* createAgent(){return nullptr;};

        //Returns an agent to its state before a simulation was run. This includes setting the reward value to 0, and resetting its environment information
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
