#include "ParticleLife.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int game_running = FALSE;
int randomize = FALSE;

particle* population = NULL;

float* min_distances_table = NULL;
float* radiuses_table = NULL;
float* force_table = NULL;
float N_repulsion_multiplier = 5;
float Base_force_multiplier = .05;
float Base_friction = .85;
int numParticles = 1000;
int numTypes = 3;
int frame;

int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stdout, "Error initializing SDL.\n");
        return FALSE;
    }
    window = SDL_CreateWindow(
    "Particle Life", 
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    WIDTH,
    HEIGHT,
    SDL_WINDOW_SHOWN
    );
    if (!window) {
        fprintf(stdout, "Error creating SDL Window\n");
        return FALSE;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        fprintf(stdout, "Error creating SDL Renderer.\n");
        return FALSE;
    }
    return TRUE;
}

void process_input() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            game_running = FALSE;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                game_running = FALSE;
            }
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_LSHIFT) {
                randomize = TRUE;
            }
            break;
    }
}

int random_z(int lower, int upper){
    return (rand() % (upper - lower + 1)) + lower;
}

float random_float_z(float left, float right) {
    float randomNumber = sin(rand() * rand());
    return left + (right - left) * fabs(randomNumber);
}

void print_run_data() {
    FILE *fptr;

    // Open a file in writing mode
    fptr = fopen("initial_conditions.txt", "w");
    for (int i = 0; i < numTypes * numTypes; i++){
        fprintf(fptr, "min distances %f, radiuses %f, forces %f\n", min_distances_table[i], radiuses_table[i], force_table[i]);
    }
    for (int i = 0; i < numParticles; ++i){
        fprintf(fptr, "Particle %d, at %f, %f\n", i, population[i].position_x, population[i].position_y);
    }
    // Close the file
    fclose(fptr);
}

void setup() {
    free(population);
    free(min_distances_table);
    free(radiuses_table);
    free(force_table);
    // int colorStep = 360/numTypes;
    printf("Placing particles...\n");
    // srand(7);
    population = malloc(sizeof(particle) * numParticles);
    //initialize all particles
    for (int i = 0; i < numParticles; ++i){
        population[i].position_x = rand() % 800;
        population[i].position_y = rand() % 1000;
        population[i].velocity_x = 0;
        population[i].velocity_y = 0;
        population[i].radius = 1;
        population[i].type = rand() % numTypes;
        // printf("X:%f, Y:%f\n", population[i].position_x,population[i].position_y);
    }
    min_distances_table = malloc(numTypes * numTypes * sizeof(float));
    radiuses_table = malloc(numTypes * numTypes * sizeof(float));
    force_table = malloc(numTypes * numTypes * sizeof(float));
    for (int i = 0; i < numTypes*numTypes; ++i){
        min_distances_table[i] = random_z(30,50);
        // radiuses_table[i] = rand() % 15 + min_distances_table[i] + 5;
        radiuses_table[i] = random_z(70,250);
        // force_table[i] = (rand() % 3 - 1.1);
        force_table[i] = random_float_z(0.3, 1.0);
        if (rand() < RAND_MAX / 2){
            force_table[i] = force_table[i] * -1;
        }
    }
    print_run_data();
}

void update() {
    // float debug = 0;
    // float debug2 = 0;
    // float debug3 = 0;
    int first = 0;
    for (int i = 0; i < numParticles; ++i) {
        for (int j = 0; j < numParticles; ++j) {
            //Calculates the forces on j caused by i and accelerates j
            if (i != j){
                //Calculates the distance, direction, between i and j as well as the constants defining the forces
                float x_dist = population[j].position_x - population[i].position_x;
                if (x_dist > 0.5 * WIDTH){x_dist -= WIDTH;}
                if (x_dist < -0.5 * WIDTH){x_dist += WIDTH;}
                float y_dist = population[j].position_y - population[i].position_y;
                if (y_dist > 0.5 * HEIGHT){y_dist -= HEIGHT;}
                if (y_dist < -0.5 * HEIGHT){y_dist += HEIGHT;}
                float dist = sqrt(x_dist*x_dist + y_dist*y_dist);
                float min_distance = min_distances_table[population[i].type*numTypes+population[j].type]; //FROM TABLE
                float radius = radiuses_table[population[i].type*numTypes+population[j].type]; //FROM TABLE
                float force = force_table[population[i].type*numTypes+population[j].type]; //FROM TABLE
                      
                //Calculates the force applied on j by i
                float primary_force = (-force/radius) * dist + force;
                float repulsive_force = ((fabs(force) * N_repulsion_multiplier)/min_distance) * dist - (fabs(force) * N_repulsion_multiplier);
                if (first == 0 && frame % 5 == 0){printf("NOCLAMP repl force %f prim force %f dist %f\n", repulsive_force, primary_force, dist); first = -1;}
                if (dist > radius || dist < min_distance){primary_force = 0;}
                if (dist > min_distance){repulsive_force = 0;}
                float net_force = (primary_force + repulsive_force) * Base_force_multiplier;
                //accelerates j in the proper direction according to force, where positve would be an attractive mechanism
                if (dist != 0){
                population[j].velocity_y += net_force * (y_dist/dist);
                population[j].velocity_x += net_force * (x_dist/dist);
                } else {
                    population[j].velocity_y += net_force;
                    population[j].velocity_x += net_force;
                    printf("BREAKING EVENT\n");
                }

            }
        }
    }
    for (int i = 0; i < numParticles; ++i) {
        //apply friction
        population[i].velocity_x = population[i].velocity_x * Base_friction;
        population[i].velocity_y = population[i].velocity_y * Base_friction;

        //moves each particle according to its velocity
        population[i].position_x += population[i].velocity_x;
        population[i].position_y += population[i].velocity_y;
        
        //Apply wrapping for particle positions in all four bounds of the screen
        population[i].position_x = fmod(population[i].position_x + WIDTH, WIDTH);
        population[i].position_y = fmod(population[i].position_y + HEIGHT, HEIGHT);
    }
    // printf("%f, %f, %f\n", debug, debug2, debug3);
    // printf("--%d-\n", rand());
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    //background fill
    SDL_RenderClear(renderer);

    //render drawing
    for (int i = 0; i < numParticles; i++){
        particle cur_particle = population[i];

        SDL_Rect cur_rect = {cur_particle.position_x, cur_particle.position_y, cur_particle.radius*2, cur_particle.radius*2};
        switch(cur_particle.type) {
            case 0:
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                break;
            case 1:
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                break;
            case 2:
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                break;
        }
        SDL_RenderFillRect(renderer, &cur_rect);
    }
        
    //swap rendered drawing into view
    SDL_RenderPresent(renderer);
}

void destroy_window() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(){
    //initialize window and setup
    printf("Initializing window...\n");
    game_running = initialize_window();
    srand(7); //move to setup to ensure each placement results in the same particle positions.
    setup();
    //game loop
    frame = 0;
    while (game_running) {
        if (frame %5 == 0) {
            printf("Interactions running tracking first particle, x=%f, y=%f, vx=%f\n", population[0].position_x, population[0].position_y, population[0].velocity_x);
        }
        process_input();
        update();
        render();
        if (randomize){
            setup();
            randomize = FALSE;
        }
        ++frame;
    }
    return EXIT_SUCCESS;
}

// void display(){
    
// }