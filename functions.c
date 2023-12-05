// Include everything necessary here 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "functions.h"
#include "OF.c"


double generate_random(double min, double max)
{ //WORKS FULLY
    double doubleInRange = ((double)rand() / RAND_MAX) * (max - min) + min;
    return doubleInRange;
}

unsigned int generate_int()
{ //WORKS FULLY
    // implement the function to return a random integer value
    srand(time(NULL));
    int r = rand();
    return r;
}

// Function to initialize a random population
void generate_population(int POPULATION_SIZE, int NUM_VARIABLES, double population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES])
{ //WORKS FULLY

    for (int i = 0; i < POPULATION_SIZE; ++i) {
        for (int j = 0; j < NUM_VARIABLES; j++) {
            // double randomValue = ((double)rand() / RAND_MAX) * (Ubound[0] - Lbound[0]) + Lbound[0];
            double randomValue = generate_random(Lbound[0], Ubound[0]);
            population[i][j] = randomValue;
        }
    }
}

// Function to compute the objective function for each member of the population
void compute_objective_function(int POPULATION_SIZE, int NUM_VARIABLES, double population[POPULATION_SIZE][NUM_VARIABLES], double fitness[POPULATION_SIZE])
{ //WORKS FULLY
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness[i] = Objective_function(NUM_VARIABLES, population[i]);
    }
}

void crossover(int POPULATION_SIZE, int NUM_VARIABLES, double fitness[POPULATION_SIZE], double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double crossover_rate)
{ //WORKS FULLY
    double fitness_probs[POPULATION_SIZE];
    double sum = 0;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness_probs[i] = 1/(fitness[i] + 1e-6);
        sum += fitness_probs[i];
    }
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness_probs[i] /= sum;
    }

    int* selected_indices = (int*)malloc(10 * sizeof(int));
    srand(time(NULL));

    for (int i = 0; i < 10; ++i) {
        double rand_num = ((double)rand() / RAND_MAX);
        double cumulative_prob = 0.0;

        for (int j = 0; j < POPULATION_SIZE; ++j) {
            cumulative_prob += fitness_probs[j];

            if (rand_num <= cumulative_prob) {
                selected_indices[i] = j;
                break;
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 2; j++) {
            new_population[i][j] = population[selected_indices[i]][j];
        }
    }

    
}

void mutate(int POPULATION_SIZE, int NUM_VARIABLES, double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES], double mutate_rate)
{
    /*Implement the logic of mutation on "new_population" and then copy everything into "population"*/
    int total_gen = POPULATION_SIZE * NUM_VARIABLES;
    int total_gen_mutate = (int)(total_gen*mutate_rate);
    int genes_to_mutate_indices[total_gen_mutate];

    for (int i = 0; i < total_gen_mutate; i++) {
        int index = rand() % total_gen;
        genes_to_mutate_indices[i] = index;
    }

    for (int i = 0; i < total_gen_mutate; i++) {
        int row = (floor)(genes_to_mutate_indices[i] / NUM_VARIABLES);
        int col = (floor)(genes_to_mutate_indices[i] % NUM_VARIABLES);

        new_population[row][col] = generate_random(Lbound[0], Ubound[0]);
    }
}