#include "ParticleLife.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int game_running = FALSE;
int randomize = FALSE;

particle* population = NULL;

float* min_distances_table = NULL;
float* radiuses_table = NULL;
float* force_table = NULL;
float N_repulsion_multiplier = 3;
int numParticles = 1000;
int numTypes = 3;


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
            if (event.key.keysym.sym == SDLK_LSHIFT) {
                randomize = TRUE;
            } 
            break;
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_LSHIFT) {
                randomize = FALSE;
            }
            break;
    }
}

void setup() {
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
        min_distances_table[i] = 1;
        radiuses_table[i] = 3;
        force_table[i] = 1;
    }

}

void update() {
    // float debug = 0;
    // float debug2 = 0;
    // float debug3 = 0;
    for (int i = 0; i < numParticles; ++i) {
        for (int j = 0; j < numParticles; ++j) {
            //  int distance = 
            //  ((population[j].position_x - population[i].position_x)(population[j].position_x - population[i].position_x) + (population[j].position_y - population[i].position_y)(population[j].position_y - population[i].position_y))
            if (i != j){
                float x_dist = population[j].position_x - population[i].position_x;
                float y_dist = population[j].position_y - population[i].position_y;
                float dist = sqrt(x_dist*x_dist + y_dist*y_dist);
                for (int row = 0; row < numTypes; ++row){
                    for (int col = 0; col < numTypes; ++col){
                        float min_distance = min_distances_table[row*numTypes+col]; //FROM TABLE
                        float radius = radiuses_table[row*numTypes+col]; //FROM TABLE
                        float force = force_table[row*numTypes+col]; //FROM TABLE
                        // debug = force;
                        // debug2 = radius;
                        // debug3 = min_distance;
                    }
                }
                
            }
        }
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
    while (game_running) {
        process_input();
        update();
        render();
        if (randomize){
            setup();
        }
    }
    return EXIT_SUCCESS;
}

// void display(){
    
// }