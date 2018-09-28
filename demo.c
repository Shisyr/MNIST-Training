#include <stdio.h>
#include <math.h>
#include "randlib.h"
#include "mnist/mnist.h"


#define numInputNodes 785
#define numOutputNodes 10

int main(int argc, char const *argv[]) {
  // --- an example for working with random numbers
  seed_randoms();
  // float sampNoise = rand_frac()/2.0; // sets default sampNoise
  float sampNoise = 0.0;

  // --- a simple example of how to set params from the command line
  if(argc == 2){ // if an argument is provided, it is SampleNoise
      sampNoise = atof(argv[1]);
      if (sampNoise < 0 || sampNoise > .5){
          printf("Error: sample noise should be between 0.0 and 0.5\n");
          return 0;
      }
  }

  mnist_data *zData;      // each image is 28x28 pixels
  unsigned int sizeData;  // depends on loadType
  int loadType = 2; // loadType may be: 0, 1, or 2
  if (mnistLoad(&zData, &sizeData, loadType)){
      printf("something went wrong loading data set\n");
      return -1;
  }

  int inputVec[numInputNodes]; // creating variable to store input values
  int pictureIndex = 0;

  // 0 = 1
  // 1 = 3
  // 2 = 0
  // 3 = 4
  // 4 = 6
  // ...

  // create matrix to store all weights for implementing matrix multiplication instead of dot product
  float weightMatrix[numOutputNodes][numInputNodes];
  for (int i = 0; i < numOutputNodes; i++)
  {
    for (int j = 0; j < numInputNodes; j++)
    {
      // set random values for weights between -1 to 1
      weightMatrix[i][j] = rand_weight();
    }
  }

  // create array to store outputs
  float output[numOutputNodes];

  // variable to store results of matrix multiplication
  float multiplicationResult = 0;

  // load MNIST picture and convert it to a vector
  get_input(inputVec, zData, pictureIndex, sampNoise);

  // draw the picture with specified label
  draw_input(inputVec, zData[pictureIndex].label);

  // create target
  float target[numOutputNodes];

  // set to 1 the node of the targeted picture
  int numberOnPicture = zData[pictureIndex].label;
  for(int i = 0; i < numOutputNodes; i++) {
    if (i == numberOnPicture) {
      target[i] = 1;
    } else {
      target[i] = 0;
    }
    printf("target[%d] = %f\n", i, target[i]);
  }


  /**************************************************************************
  *
  *   LET'S START TRAINING...
  *
  **************************************************************************/

  // get output by multiplying inputs and weights and summing them
  for (int i = 0; i < numOutputNodes; i++) {
    for (int j = 0; j < numInputNodes; j++) {
      multiplicationResult += weightMatrix[i][j] * inputVec[j];
    }
    output[i] = multiplicationResult;
    printf("output[%d] = %f\n", i, output[i]);
    multiplicationResult = 0;
  }

  // squash outputs to get a value between 0 and 1
  for (int i = 0; i < numOutputNodes; i++) {
    output[i] = 1.0 / (1.0 + pow(M_E, -1 * output[i]));
    printf("squashed output[%d] = %f\n", i, output[i]);
  }


  // create error
  float error[numOutputNodes];

  for (int i = 0; i < numOutputNodes; i++) {
    error[i] = target[i] - output[i];
    printf("error[%d] = %f\n", i, error[i]);
  }

  float learningRate = 0.1;

  // create matrix for deltaWeight
  float deltaWeightMatrix[numOutputNodes][numInputNodes];
  for(int i = 0; i < numOutputNodes; i++) {
    for (int j = 0; j < numInputNodes; j++) {
      deltaWeightMatrix[i][j] = inputVec[j] * error[i] * learningRate;
      // printf("%f ", deltaWeight[i][j]);
    }
    // printf("\n");
  }

  //update weights
  for(int i = 0; i < numOutputNodes; i++) {
    for (int j = 0; j < numInputNodes; j++) {
      weightMatrix[i][j] += deltaWeightMatrix[i][j];
    }
  }

  return 0;
}
