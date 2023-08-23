//
// Created by tpixe on 5/3/2022.
//

#ifndef EVOLUTIONARYALGORITHIM_NEWNETWORK_H
#define EVOLUTIONARYALGORITHIM_NEWNETWORK_H
#include <Eigen/Dense>
#include<cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace Eigen;

class Network {
    //1d array, index 0 = weights between layer 0 and 1



    //DNA
    //contains the weights and biases for each. Value encoding acording to this link : https://www.obitko.com/tutorials/genetic-algorithms/encoding.php
    //mutation can take random parts of each parent and make a new one.
    //matrix is fixed shape
    public:
        MatrixXd* weights;
        MatrixXd* bias;
        VectorXd* inputs;
        int *height;
        int length;
        int size = 0;
        Network();
        Network(int length, int *height);
        Network(std::string *data);
        ~Network();
        void print();
        void calc(VectorXd &input, MatrixXd &output);
        void calc(double *input, int inputLength, double *output, int outputLength);
        void clearMemory();
        std::string* networkToString();

};

#endif //EVOLUTIONARYALGORITHIM_NEWNETWORK_H
