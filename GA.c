// Include everything necessary here 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "functions.c"

int main(int argc, char *argv[])
{
    // <YOUR CODE: Handle the possible errors in input data given by the user and say how to execute the code>
    if (argc != 6) {
        printf("Incorrect number of inputs. Expected 6.\n");
        printf("Number of variables: %d", argc);
        return 1;
    }
    // <YOUR CODE: Assign all inputs given by the user argv[i]> like
    // POPULATION_SIZE, MAX_GENERATIONS, crossover_rate, mutate_rate, stop_criteria
    int POPULATION_SIZE = atoi(argv[1]);
    int MAX_GENERATIONS = atoi(argv[2]);
    float crossover_rate = atof(argv[3]);
    float mutate_rate = atof(argv[4]);
    float stop_criteria = strtod(argv[5], NULL);
    int NUM_VARIABLES = 2;
    double Lbound[] = {-5.0, -5.0};
    double Ubound[] = {5.0, 5.0};


    // <YOUR CODE: Here make all the initial print outs>
    printf("\nGenetic algorithm has been initiated.\n");
    printf("---------------------------------------\n");
    printf("The number of variables: %i\n", argc);
    printf("Lower bounds: [%f, %f]\n", Lbound[0], Lbound[1]);
    printf("Upper bounds: [%f, %f]\n\n", Ubound[0], Ubound[1]);
    printf("Population size: %d\n", POPULATION_SIZE);
    printf("Max generations: %d\n", MAX_GENERATIONS);
    printf("Crossover rate: %f\n", crossover_rate);
    printf("Mutation rate: %f\n", mutate_rate);
    printf("Stopping criteria: %f\n\n", stop_criteria);
    printf("Results: \n");
    printf("---------------------------------------\n");

    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();

    // <YOUR CODE: Declare all the arrays you need here>
    double population[POPULATION_SIZE][NUM_VARIABLES];
    double new_population[POPULATION_SIZE][NUM_VARIABLES];
    double fitness[POPULATION_SIZE];
    double fitness_probs[POPULATION_SIZE];

    // <YOUR CODE: Call generate_population function to initialize the "population"> like:
    // generate_population(POPULATION_SIZE, NUM_VARIABLES, population, Lbound, Ubound);
    generate_population(POPULATION_SIZE, NUM_VARIABLES, population, Lbound, Ubound);

    double best_fitness = 0;
    int best_fitness_index = 0;

    for (int generation = 0; generation < MAX_GENERATIONS; generation++)
    {
        // <YOUR CODE: Compute the fitness values using objective function for
        // each row in "population" (each set of variables)> like:
        // compute_objective_function(POPULATION_SIZE, NUM_VARIABLES, population, fitness);
        
        compute_objective_function(POPULATION_SIZE,NUM_VARIABLES, population, fitness);

        // <YOUR CODE: Here implement the logic of finding best solution with minimum fitness value
        // and the stopping criteria>

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

        // <YOUR CODE: Here call the crossover function>

        crossover(POPULATION_SIZE, NUM_VARIABLES, fitness, new_population, population, crossover_rate);

        // <YOUR CODE: Here call the mutation function>

        mutate(POPULATION_SIZE, NUM_VARIABLES, new_population, population, Lbound, Ubound, mutate_rate);

        for (int i = 0; i < POPULATION_SIZE; i++) {
            if (fitness_probs[i] < fitness_probs[i + 1]) {
                best_fitness = fitness_probs[i];
                best_fitness_index = i;
            }
        }

        if (best_fitness < stop_criteria) {
            break;
        }


        // Now you have the a new population, and it goes to the beginning of loop to re-compute all again
    }

    // <YOUR CODE: Jump to this part of code if the stopping criteria is met before MAX_GENERATIONS is met>

    // ###################################################################################
    // You dont need to change anything here
    // Here we print the CPU time taken for your code
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("CPU time: %f seconds\n", cpu_time_used);
    // ###################################################################################

    // <Here print out the best solution and objective function value for the best solution like the format>
    printf("best fitness: %f\n", best_fitness);
    printf("best solution: x1 = %f and x2 = %f", new_population[best_fitness_index][0], new_population[best_fitness_index][1]);
    return 0;
}
