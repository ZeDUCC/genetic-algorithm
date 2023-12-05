#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"
#include <time.h>


double Objective_function(int NUM_VARIABLES, double x[NUM_VARIABLES])
{
    // While the objective function could be anything here it is to minimize Ackley function
    double sum1 = 0.0, sum2 = 0.0;
    for (int i = 0; i < NUM_VARIABLES; i++)
    {
        sum1 += x[i] * x[i];
        sum2 += cos(2.0 * M_PI * x[i]);
    }
    return -20.0 * exp(-0.2 * sqrt(sum1 / NUM_VARIABLES)) - exp(sum2 / NUM_VARIABLES) + 20.0 + M_E;
}

double generate_random(double min, double max) //WORKS FULLY
{
    double doubleInRange = ((double)rand() / RAND_MAX) * (max - min) + min;
    return doubleInRange;
}

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

void compute_objective_function(int POPULATION_SIZE, int NUM_VARIABLES, double population[POPULATION_SIZE][NUM_VARIABLES], double fitness[POPULATION_SIZE])
{ //WORKS FULLY
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness[i] = Objective_function(NUM_VARIABLES, population[i]);
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

void crossover(int POPULATION_SIZE, int NUM_VARIABLES, double fitness[POPULATION_SIZE], double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double crossover_rate)
{
    for (int i = 0; i < POPULATION_SIZE; i += 2) {
        if (((double)rand() / RAND_MAX) < crossover_rate) {
            double p1[2];
            double p2[2];

            p1[0] = new_population[i][0];
            p1[1] = new_population[i][1];
            p2[0] = new_population[i + 1][0];
            p2[1] = new_population[i + 1][1];

            double c1[2];
            double c2[2];

            int crosspoint = (rand() % NUM_VARIABLES - 1) + 1;

            for (int e = 0; e < crosspoint; e++) {
                c1[e] = p1[e];
                c2[e] = p2[e];
            }

            for (int e = 0; e < NUM_VARIABLES; e++) {
                c1[e] = p2[e];
                c2[e] = p1[e];
            }

            for (int e = 0; e < NUM_VARIABLES; e++) {
                new_population[i][e] = c1[e];
                new_population[i + 1][e] = c2[e];
            }
        }
    }
}

int main() {

    int POPULATION_SIZE = 10;
    int NUM_VARIABLES = 2;

    double population[POPULATION_SIZE][NUM_VARIABLES];
    double new_population[POPULATION_SIZE][NUM_VARIABLES];
    double fitness[POPULATION_SIZE];
    double Lbound[2] = {-5.0, -5.0};
    double Ubound[2] = {5.0, 5.0};
    double crossover_rate = 0.5;
    double mutate_rate = 0.1;

    generate_population(POPULATION_SIZE, NUM_VARIABLES, population, Lbound, Ubound);
    compute_objective_function(POPULATION_SIZE, NUM_VARIABLES, population, fitness);

    //probability logic starts here
    double fitness_probs[POPULATION_SIZE];
    double sum = 0;
    
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness_probs[i] = 1/(fitness[i] + 1e-6);
        sum += fitness_probs[i];
    }

    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness_probs[i] = fitness_probs[i]/sum;
    }

    int* selected_indices = (int*)malloc(POPULATION_SIZE * sizeof(int));

    srand((unsigned int)time(NULL));

    for (int i = 0; i < POPULATION_SIZE; ++i) {
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

    //probability logic ends here

    //creating new population
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 2; j++) {
            new_population[i][j] = population[selected_indices[i]][j];
        }
    }

    crossover(POPULATION_SIZE, NUM_VARIABLES, fitness, new_population, population, crossover_rate);

    mutate(POPULATION_SIZE, NUM_VARIABLES, new_population, population, Lbound, Ubound, mutate_rate);

    return 0;
}
