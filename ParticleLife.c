#include "ParticleLife.h"

int numTypes = 6;
int numParticles = 1000;

void main(){
    int colorStep = 360/numTypes;
    srand(7);
    particle* population = malloc(sizeof(particle) * numParticles);
    //initialize all particles
    for (int i = 0; i < numParticles; ++i){
        population[i].position_x = rand() % 800;
        population[i].position_y = rand() % 1000;
        population[i].velocity_x = 0;
        population[i].velocity_y = 0;
        population[i].type = rand() % numTypes;
    }

}

void make_particle(particle* cur_particle){
    //Pass in a pointer to the current particle
    cur_particle[0].position_x = rand() % 800;
    cur_particle[0].position_y = rand() % 1000;
    cur_particle[0].velocity_x = 0;
    cur_particle[0].velocity_y = 0;
    cur_particle[0].type = rand() % numTypes;
}

// void display(){
    
// }