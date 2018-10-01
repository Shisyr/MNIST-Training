#include <stdio.h>
#include <math.h>
#include "randlib.h"
#include "mnist/mnist.h"

#define numInputNodes 785
#define numOutputNodes 10

void initializeTarget(float target[], int numberOnPicture);
void createRandomWeightMatrix(float weightMatrix[][numInputNodes]);
void getOutput(float output[], int inputVec[], float weightMatrix[][numInputNodes]);
void squashOutput(float output[]) ;
void getError(float error[], float target[], float output[]);
float getAverageError(float error[]);
void updateWights(float weightMatrix[][numInputNodes], int inputVec[], float error[], float learningRate);

int main(int argc, char const *argv[]) {
  // --- an example for working with random numbers
  seed_randoms();
  // float sampNoise = rand_frac()/2.0; // sets default sampNoise
  float sampNoise = 0;

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
  // int pictureIndex = 0;

  float learningRate = 0.5;
  // number of training loops
  // int trainLoopNum = 20;

  float target[numOutputNodes];

  // create array to store outputs
  float output[numOutputNodes];

  float error[numOutputNodes];
  // 0 = 1
  // 1 = 3
  // 2 = 0
  // 3 = 4
  // 4 = 6
  // ...

  float weightMatrix[numOutputNodes][numInputNodes];
  createRandomWeightMatrix(weightMatrix);

  // float averageError;

  // // load MNIST picture and convert it to a vector
  // get_input(inputVec, zData, pictureIndex, sampNoise);
  //
  // // draw the picture with specified label
  // draw_input(inputVec, zData[pictureIndex].label);


  // loadType = 0, 60k training images
  // loadType = 1, 10k testing images
  // loadType = 2, 10 specific images from training set
  // printf("number of training patterns = %d\n", sizeData);
for(float noise = 0.0; noise < 0.6; noise += 0.1) {

  // printf("sampNoise = %.02f\n", noise);

  createRandomWeightMatrix(weightMatrix);

  for(int simulation = 0; simulation < 20; simulation++) {

    // printf("\nSimulation #%d\n\n", simulation);
    float theError = 0;
    for(int pictureIndex = 0; pictureIndex < sizeData; pictureIndex++){
        get_input(inputVec, zData, pictureIndex, noise);
        // draw_input(inputVec, zData[pictureIndex].label);

        initializeTarget(target, zData[pictureIndex].label);
          // printf("\nTraining loop #%d for pictureIndex #%d\n\n", i, pictureIndex);

        getOutput(output, inputVec, weightMatrix);

        squashOutput(output);

        getError(error, target, output);
        //
        // printf("%f ", getAverageError(error));
        //
        theError += getAverageError(error);

        updateWights(weightMatrix, inputVec, error, learningRate);
    }
    printf("%f, ", (theError / sizeData));
  }

  printf("\n");

}






  // int outPic = 6;
  // get_input(inputVec, zData, outPic, 0);
  // draw_input(inputVec, zData[outPic].label);
  //
  // getOutput(output, inputVec, weightMatrix);
  //
  // squashOutput(output);
  //
  // for(int i = 0; i < 10; i++) {
  //   printf("squashedOutput[%d] = %f\n", i, output[i]);
  // }

  return 0;

}


// create matrix to store all weights for implementing matrix multiplication instead of dot product
void createRandomWeightMatrix(float weightMatrix[][numInputNodes]) {

    for (int i = 0; i < numOutputNodes; i++)
    {
      for (int j = 0; j < numInputNodes; j++)
      {
        // set random values for weights between -1 to 1
        weightMatrix[i][j] = rand_weight();
      }
    }
  }


void initializeTarget(float target[], int numberOnPicture) {

  for(int i = 0; i < numOutputNodes; i++) {
    if (i == numberOnPicture) {
      target[i] = 1;
    } else {
      target[i] = 0;
    }
    // printf("target[%d] = %f\n", i, target[i]);
  }
}

// calculate output by matrix multiplication of inputs and weights
// and then squash output
void getOutput(float output[], int inputVec[], float weightMatrix[][numInputNodes]) {

  // variable to store results of matrix multiplication
  float sumMultiplicationResult;

  for(int i = 0; i < numOutputNodes; i++) {
    sumMultiplicationResult = 0;
    for(int j = 0; j < numInputNodes; j++) {

      float multiplicationResult = inputVec[j] * weightMatrix[i][j];
      sumMultiplicationResult += multiplicationResult;
    }

    output[i] = sumMultiplicationResult;
    // printf("output[%d] = %f\n", i, sumMultiplicationResult);
  }
}

void squashOutput(float output[]) {

  for(int i = 0; i < numOutputNodes; i++) {
    output[i] = 1.0 / (1.0 + pow(M_E, -1 * output[i]));
    // printf("squashed output[%d] = %f\n", i, output[i]);
  }
}


// get error
void getError(float error[], float target[], float output[]) {

  for (int i = 0; i < numOutputNodes; i++) {
    error[i] = target[i] - output[i];

    // printf("error[%d] = %f\n", i, error[i]);
  }
}

float getAverageError(float error[]) {

  float allErrorSum = 0;
  for (int i = 0; i < numOutputNodes; i++) {
    allErrorSum += fabs(error[i]);
  }

  return (allErrorSum / numOutputNodes);
}

// calculate deltaWeight
// and update weight
void updateWights(float weightMatrix[][numInputNodes], int inputVec[], float error[], float learningRate) {

  float deltaWeightMatrix[numOutputNodes][numInputNodes];
  for(int i = 0; i < numOutputNodes; i++) {
    for (int j = 0; j < numInputNodes; j++) {
      deltaWeightMatrix[i][j] = inputVec[j] * error[i] * learningRate;
      // float temp = weightMatrix[i][j];
      weightMatrix[i][j] += deltaWeightMatrix[i][j];

    }
  }
}
