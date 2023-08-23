//
// Created by tpixe on 5/3/2022.
//

#include <iostream>
#include "newNetwork.h"

inline double sigmoid(double x){
    return ((x/(1+abs(x)))+1)/2;
}

inline void sigmoidVector(MatrixXd &input, int height){
    //std::cout << input << std::endl;
    for(int i = 0; i < height; i++){
        input(i) = sigmoid(input(i));
    }
}

VectorXd arrayToVector(double* input, int length){
    VectorXd output(length);
    for(int i = 0; i < length - 1; i++){
        output[i] = input[i];
    }
    return output;
}

Network::Network() {

}

Network::Network(int length, int *height){
        srand (static_cast <unsigned> (time(0)));
        this->length = length;
        this->height = height;
        weights = new MatrixXd[length - 1];
        bias = new MatrixXd[length];
        inputs = new VectorXd[length];
        for(int i = 0; i < length; i++){
            if(i > 0){
                MatrixXd wToAdd = MatrixXd::Random(height[i], height[i-1]) * 5;
                weights[i- 1] = wToAdd;
                MatrixXd bToAdd = MatrixXd::Random(height[i],1) * 5;
                bias[i-1] = bToAdd;
            }
            VectorXd iToAdd = VectorXd::Constant(height[i],0);
            inputs[i] = iToAdd;
        }
    };

Network::~Network(){
    delete[] weights;
    delete[] bias;
    delete[] inputs;
}

void Network::clearMemory(){
    delete[] weights;
    weights = nullptr;
    delete[] bias;
    bias = nullptr;
    delete[] inputs;
    inputs = nullptr;
}

void Network::print(){
        for(int i = 0; i < length-1; i++){
            std::cout << "This is for the area between " << i << " and " << i + 1 << ". " << "Weights : " << std::endl << weights[i] << std::endl << " Bias: " << std::endl << bias[i] << std::endl;
        }
    };

void Network::calc(VectorXd &input, MatrixXd &output){
    inputs[0] = input;
    for(int i = 0; i < length-1; i++){
        MatrixXd result = weights[i] * inputs[i];
        result = result + bias[i];
        sigmoidVector(result,height[i+1]);
        inputs[i+1] = result;
    }
    output = inputs[length-1];
}

void Network::calc(double *input, int inputLength, double *output, int outputLength){
    VectorXd vInput = arrayToVector(input,inputLength);
    MatrixXd vOutput;
    calc(vInput,vOutput);
    for(int i = 0; i < outputLength; i++){
      output[i] = vOutput(i);
    }
    //output = vOutput.data();
}

template <typename T>
inline std::string arrayToString(T* toConvert,int length){
    std::string output = "[";
    for(int i = 0; i < length; i++){
        output += std::to_string(toConvert[i]);
        if(i != length - 1){
            output += ",";
        }else{
            std::cout << "test" << std::endl;
        }
    }
    output += "]";
    return output;
}

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

inline std::string matrixToString(MatrixXd toConvert){
    double* data = toConvert.data();
    int size = toConvert.rows() * toConvert.cols();
    return arrayToString(data,size);
}


inline MatrixXd arrayToMatrixXd(double* input, int rows, int cols){

}

//NOTE: MatrixXd.data() gives a column major order array, not row-major
//dont bother saving inputs, since those are just rewritten every time
//Save all the weights and biases of the network to a string for later use
std::string* Network::networkToString(){
    std::string* output = new std::string();
    *output = "l:" + std::to_string(this->length) + "\nh:" + arrayToString(this->height,this->length);
    for(int i = 0; i < length-1; i++){
        *output += matrixToString(weights[i]) + "\n" + matrixToString(bias[i]) + "\n";
    }
    return output;
}

//constructor for network that loads from a string
//data should be an array of strings, where each entry is a line from the text file. The length of this array will always be 2l + 2, where l is number of layers. l is present in the text file
//TODO: Test this code.
Network::Network(std::string *data) {
    //load length and heights from the data
    char dummyChar;
    std::stringstream(data[0]) >> dummyChar >> dummyChar >> this->length;
    std::string heightS;
    std::stringstream(data[1]) >> dummyChar >> dummyChar >> heightS;
    this->height = stringToArray<int>(&heightS,this->length);
    //make matrix objects
    weights = new MatrixXd[length - 1];
    bias = new MatrixXd[length];
    inputs = new VectorXd[length];
    //load weights and biases from string
    for(int i = 0; i < this->length; i++){
        double* wData = stringToArray<double>(&data[2*i],this->height[i] * this->height[i-1]);
        weights[i] = arrayToMatrixXd(wData,this->height[i], this->height[i-1]);
        delete [] wData;
        double* bData = stringToArray<double>(&data[2*i + 1],this->height[i] * this->height[i-1]);
        bias[i] = arrayToMatrixXd(bData,this->height[i], this->height[i-1]);
        delete bData;
    }
}

