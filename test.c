#include <stdio.h>
#include <math.h>
#include "randlib.h"
#include "mnist/mnist.h"

#define numInputNodes 785
#define numOutputNodes 10

int initializeTarget(float target[], int numberOnPicture);
void createRandomWeightMatrix(float weightMatrix[][numInputNodes]);
void getSquashedOutput(float output[], int inputVec[], float weightMatrix[][numInputNodes]);
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
  int pictureIndex = 2;

  float learningRate = 0.1;
  // number of training loops
  int trainLoopNum = 20;

  float weightMatrix[numOutputNodes][numInputNodes];

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

  for (int i = 0; i < 6; i++) {

    printf("\n\nImage noise = %.02f\n\n", sampNoise);

    // loadType = 0, 60k training images
    // loadType = 1, 10k testing images
    // loadType = 2, 10 specific images from training set
    // printf("number of training patterns = %d\n", sizeData);

    // inspect the training samples we will work with:
    // for(int pictureIndex = 0; pictureIndex < sizeData; pictureIndex++){

      get_input(inputVec, zData, pictureIndex, sampNoise);
      draw_input(inputVec, zData[pictureIndex].label);

      createRandomWeightMatrix(weightMatrix);

      int whichNumber = initializeTarget(target, zData[pictureIndex].label);

      for (int i = 1; i <= trainLoopNum; i++) {

        printf("\nTraining loop #%d for pictureIndex #%d\n\n", i, pictureIndex);

        getSquashedOutput(output, inputVec, weightMatrix);

        getError(error, target, output);

        // printf("average Error = %f\n", getAverageError(error));

        printf("\ntargetted error[%d] = %f\n", whichNumber, error[whichNumber]);

        updateWights(weightMatrix, inputVec, error, learningRate);
      }

      sampNoise += 0.1;
  }

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

    printf("Created random weigths\n\n");
  }


int initializeTarget(float target[], int numberOnPicture) {

  int targetedNumber = 0;
  for(int i = 0; i < numOutputNodes; i++) {
    if (i == numberOnPicture) {
      target[i] = 1;
      targetedNumber = i;
    } else {
      target[i] = 0;
    }
    printf("target[%d] = %f\n", i, target[i]);
  }

  return targetedNumber;
}

// calculate output by matrix multiplication of inputs and weights
// and then squash output
void getSquashedOutput(float output[], int inputVec[], float weightMatrix[][numInputNodes]) {

  // variable to store results of matrix multiplication
  float multiplicationResult = 0;

  for (int i = 0; i < numOutputNodes; i++) {
    for (int j = 0; j < numInputNodes; j++) {
      multiplicationResult += weightMatrix[i][j] * inputVec[j];
    }
    output[i] = multiplicationResult;
    output[i] = 1.0 / (1.0 + pow(M_E, -1 * output[i]));
    printf("squashed output[%d] = %f\n", i, output[i]);

    multiplicationResult = 0;
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
    allErrorSum += error[i];
  }

  return (allErrorSum/numOutputNodes);
}

// calculate deltaWeight
// and update weight
void updateWights(float weightMatrix[][numInputNodes], int inputVec[], float error[], float learningRate) {

  float deltaWeightMatrix[numOutputNodes][numInputNodes];
  for(int i = 0; i < numOutputNodes; i++) {
    for (int j = 0; j < numInputNodes; j++) {
      deltaWeightMatrix[i][j] = inputVec[j] * error[i] * learningRate;
      weightMatrix[i][j] += deltaWeightMatrix[i][j];
    }
  }
}
