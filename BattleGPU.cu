#include <cuda.h>
#include <cuda_runtime_api.h>
#include <device_launch_parameters.h>
#include <iostream>

#ifndef COMMON_H
#include "common.h"
#endif

const int TILE_SIZE = 16;
const int NUM_POKEMON = 2;
// GPU-compatible random number generation
int getRandom(int seed, int offset) {
    curandState state;
    curand_init(seed, offset, 0, &state);
    return curand(&state) % 100;
}

Pokemon battleGPU(Pokemon& pokemon1, Pokemon& pokemon2, int seed) {
    int moveIndex;
    Move selected;

    while (pokemon1.getHP() >= 1 && pokemon2.getHP() >= 1) {
        if (pokemon1.getSpe() > pokemon2.getSpe()) {
            moveIndex = getRandom(seed, 1) % pokemon1.getMovesNum();
            selected = pokemon1.getMove(moveIndex);

            if (selected.getCat() == 3) {
                statusCalc(pokemon1, pokemon2, selected);
            }
            else {
                damageCalc(pokemon1, pokemon2, selected);
            }

            if (pokemon2.getHP() < 1) break;

            moveIndex = getRandom(seed, 2) % pokemon2.getMovesNum();
            selected = pokemon2.getMove(moveIndex);

            if (selected.getCat() == 3) {
                statusCalc(pokemon2, pokemon1, selected);
            }
            else {
                damageCalc(pokemon2, pokemon1, selected);
            }
        }
        else {
            moveIndex = getRandom(seed, 3) % pokemon2.getMovesNum();
            selected = pokemon2.getMove(moveIndex);

            if (selected.getCat() == 3) {
                statusCalc(pokemon2, pokemon1, selected);
            }
            else {
                damageCalc(pokemon2, pokemon1, selected);
            }

            if (pokemon1.getHP() < 1) break;

            moveIndex = getRandom(seed, 4) % pokemon1.getMovesNum();
            selected = pokemon1.getMove(moveIndex);

            if (selected.getCat() == 3) {
                statusCalc(pokemon1, pokemon2, selected);
            }
            else {
                damageCalc(pokemon1, pokemon2, selected);
            }
        }
    }

    if (pokemon2.getHP() < 1) {
        return pokemon1;
    }
    else {
        return pokemon2;
    }
}

// Kernel to simulate many battles
__global__ void battleKernel(Pokemon* p1Array, Pokemon* p2Array, Pokemon* results, int numBattles) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < numBattles) {
        results[idx] = pokeBattleGPU(p1Array[idx], p2Array[idx], idx + 1234); // Unique seed per thread
    }
}

bool pokeBattleGPU(Pokemon* pokemon1, Pokemon* pokemon2) {
    Pokemon* d_p1, *d_p2;

    // Allocate memory on the device
    cudaMalloc((void**)&d_p1, sizeof(Pokemon) * NUM_POKEMON);
    cudaMalloc((void**)&d_p2, sizeof(Pokemon) * NUM_POKEMON);

    // Copy input Pokémon from host to device
    cudaMemcpy(d_p1, &pokemon1, sizeof(Pokemon), cudaMemcpyHostToDevice);
    cudaMemcpy(d_p2, &pokemon2, sizeof(Pokemon), cudaMemcpyHostToDevice);

    dim3 blockSize(TILE_SIZE);
    dim3 gridSize((size + TILE_SIZE - 1) / TILE_SIZE);

    // Launch 1 thread to simulate 1 battle
    battleKernel << <dimGrid, dimBlock >> > (d_p1, d_p2, 1);
    cudaDeviceSynchronize(); // Wait for GPU to finish

    // Copy updated Pokémon back to host
    cudaMemcpy(&pokemon1, d_p1, sizeof(Pokemon), cudaMemcpyDeviceToHost);
    cudaMemcpy(&pokemon2, d_p2, sizeof(Pokemon), cudaMemcpyDeviceToHost);

    // Free GPU memory
    cudaFree(d_p1);
    cudaFree(d_p2);

    // Determine winner
    return pokemon2.getHP() < 1;
}

