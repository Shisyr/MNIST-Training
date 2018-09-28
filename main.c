/**************************************************************************
*
*   SOME STARTER CODE FOR WORKING WITH NMIST, © MICHAEL BRADY 2018
*
**************************************************************************/
#include <stdio.h>
#include "randlib.h"
#include "mnist/mnist.h"

#define numInputNodes 785
#define numOutputNodes 10

int main(int argc, char* argv[])
{
    // --- an example for working with random numbers
    seed_randoms();
    float sampNoise = rand_frac()/2.0; // sets default sampNoise

    // --- a simple example of how to set params from the command line
    if(argc == 2){ // if an argument is provided, it is SampleNoise
        sampNoise = atof(argv[1]);
        if (sampNoise < 0 || sampNoise > .5){
            printf("Error: sample noise should be between 0.0 and 0.5\n");
            return 0;
        }
    }

    // --- an example for how to work with the included mnist library:
    mnist_data *zData;      // each image is 28x28 pixels
    unsigned int sizeData; // depends on loadType
    int loadType = 2; // loadType may be: 0, 1, or 2
    if (mnistLoad(&zData, &sizeData, loadType)){
        printf("something went wrong loading data set\n");
        return -1;
    }
    unsigned int numWeights = numInputNodes * numOutputNodes;
    float theWeights[numWeights];

    // initializing weights with random numbers between -1 to 1
    for(int i = 0; i < numWeights; i++) {
      theWeights[i] = rand_weight();
    }

    // print weights
    for(int i = 0; i < numOutputNodes; i++) {
      for(int j = 0; j < numInputNodes; j++) {
        int theIndex = (i*numOutputNodes) + j;
        // printf("weight i-j =  %f\n", i, j, theWeights[theIndex]);
      }
    }

    // create memory for an output vector
    int output[numOutputNodes];
    for(int i = 0; i < numOutputNodes; i++) {
      output[i] = 0;
    }

    // create memory for input vector and load the vector

    int inputVec[numInputNodes];
    int pictureNum = 1; // index of a picture

    // pictures are stored in zData
    get_input(inputVec, zData, pictureNum, 0); //loads a picture

    draw_input(inputVec, zData[pictureNum].label);

    for (int i = 0; i < numOutputNodes; i++) {
      for (int j = 0; j < numInputNodes; j++) {
        // int input = inputVec[j];

        // outPut[i] += theWeights[(i * numOutputNodes) + j] * inputVec[j];
        // printf("weight %d ")
      }
    }

    //
    // for (int k = 0; k < numInputNodes; k++) {
    //   printf("%d", inputVec[k]);
    // }
    //
    // printf("\n");


    // // loadType = 0, 60k training images
    // // loadType = 1, 10k testing images
    // // loadType = 2, 10 specific images from training set
    // printf("number of training patterns = %d\n", sizeData);
    //
    // // inspect the training samples we will work with:
    // int inputVec[785]; //(28*28)+1 = 785
    // for(int i = 0; i < sizeData; i++){
    //     get_input(inputVec, zData, i, sampNoise);
    //     draw_input(inputVec, zData[i].label);
    // }

    free(zData);
    return 0;
}
