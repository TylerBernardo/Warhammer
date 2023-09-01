//
// Created by tpixe on 5/3/2022.
//

#include "evo.h"

std::uniform_real_distribution<double> unif(0,1);
std::default_random_engine re;

MatrixXd combineMatrixRandom(MatrixXd &matrix1, MatrixXd &matrix2){
    int length = matrix1.rows(), height = matrix1.cols();
    MatrixXd output = MatrixXd::Zero(length,height);
    for(int r = 0; r < length; r++){
        for(int c = 0; c < height; c++){
            double ranNum = unif(re);
            if(ranNum <= .5){
                output(r,c) = matrix1(r,c);
            }else{
                output(r,c) = matrix2(r,c);
            }
        }
    }
    output = output + 0.25 * MatrixXd::Random(length,height);
    return output;
}

bool compareFunction(Agent *a1, Agent *a2){
    return a1->reward >= a2->reward;
}

//take the top quarter of agents and repopulate from there
void crossover(Agent** agents,int length){
    int toKeep = std::round(length/4.0);
    std::uniform_int_distribution<int> iUnif(0,toKeep-1);
    for(int a = 0; a < length-toKeep; a++){
        //determine twoParents TODO: check to make sure parent1 != parent2
        Agent* parent1 = agents[0], *parent2 = agents[iUnif(re)];
        Network* network = (agents[a+toKeep]->network);
        for(int i = 0; i < (network->length)-1; i++){
            //std::cout << network->weights[i] <<std::endl;
            network->weights[i] = combineMatrixRandom(parent1->network->weights[i],parent2->network->weights[i]);
//std::cout << network->weights[i] << std::endl;
            network->bias[i] = combineMatrixRandom(parent1->network->bias[i],parent2->network->bias[i]);
        }
        network->bias[network->length-1] = combineMatrixRandom(parent1->network->bias[network->length-1],parent2->network->bias[network->length-1]);
        agents[a+toKeep]->network = network;
    }
}

void sortAgents(Agent** agents, int length, bool (*compare)(Agent *, Agent *)){
    for(int n = 0; n < length; n++){
        int best = n;
        for(int i = n + 1; i < length; i++){
            if(compare(agents[i],agents[best])){
                best = i;
            }
        }
        Agent* toSwap = agents[best];
        agents[best] = agents[n];
        agents[n] = toSwap;
    }
}

void calcGroup(EvoController *controller, int start, int end){
    //std::cout << "Calculating the agents from " + std::to_string(start) + " to " + std::to_string(end) << std::endl;
    try{
        double *output = nullptr;
        for(int i = start; i <= end; i++) {
            do {
                //evaluate the agent's network on the current state
                output = new double[controller->outputSpaceLength];
                double *input = controller->genInputSpace(i);
                controller->agents[i]->network->calc(input,controller->inputSpaceLength, output,controller->outputSpaceLength);
                int reward = controller->state(output, i);
                //process reward here
                controller->agents[i]->reward += reward;
                delete[] output;
                delete[] input;
            } while (controller->agents[i]->endState());
        }
    }catch(int eCode){
        std::cout << "There was an error with error code " << eCode << " when processing the batch starting at " << start << std::endl;
        throw(eCode);
    }

}

//takes in the length of the input space, a function to generate the input space, the length of the output space, and a function to map net outputs to the gamespace, which then returns reward info.
Agent* learn(EvoController *controller, int populationSize, int generations, int threads){
    /*
     * Might be unneeded
    int networkLength = 2 + controller->hiddenLayerCount;
    int *networkHeight = new int[networkLength];
    networkHeight[0] = controller->inputSpaceLength;
    networkHeight[networkLength-1] = controller->outputSpaceLength;
    for(int i = 1; i < networkLength - 2; i ++){
        networkHeight[i] = controller->hiddenLayers[i - 1];
    }
     */

    //main loop,
    int threadSize = std::floor(populationSize/(1.0 * threads));
    std::thread **threadList = new std::thread*[threads];
    for(int g = 1; g <= generations; g++){
        //loop through each agent TODO: Use multithreading to process this faster
        /*
        for(int i = 0; i < threads ; i++){
            if(i == threads - 1){
                std::cout << "Will make a thread from " << i * threadSize << " to " << populationSize - 1 << std::endl;
                //handle threadsize reaching outside of array
            }else{
                std::cout << "Will make a thread from " << i * threadSize << " to " << (i+1) * threadSize - 1 << std::endl;

            }

        }
         */
        auto start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < threads ; i++){
            if(i == threads - 1){
                threadList[i] = new std::thread(calcGroup, controller,  (i * threadSize), populationSize - 1);
                //handle threadsize reaching outside of array
            }else{
                threadList[i] = new std::thread(calcGroup, controller,  i*threadSize, (i+1) * threadSize - 1);

            }
        }
        for(int t = 0; t < threads; t++){
            threadList[t]->join();
            //std::cout << "Thread " << t << " has finished" << std::endl;
        }

        for(int t = 0; t < threads; t++){
            delete threadList[t];
        }

        //determine top performers
        sortAgents(controller->agents,populationSize,compareFunction);
        //mutation happens here
        //compute stats about generation
        int bestScore = controller->agents[0]->reward;
        double averageScore = bestScore;
        for(int k = 1; k < populationSize; k++){
            averageScore += controller->agents[k]->reward;
        }

        averageScore = averageScore/populationSize;
        std::cout << "The best during generation " << g << " score was " << bestScore << " and the average score was " << averageScore << " in " ;
        crossover(controller->agents,populationSize);
        //possibly multithreading this as well. Maybe rewrite reset to automatically reset all agents, so that multithreading can be implemented on a case to case basis
        /*
        for(int a = 0; a < populationSize; a++){
            controller->reset(a);
        }
         */
        //TODO: Investigate performance of reset function. Over half of runtime per generation dedicated to it
        controller->reset();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds >(stop - start);
        std::cout << duration.count() << " milliseconds" << std::endl;
    }
    delete[] threadList;
    return controller->agents[0];
}