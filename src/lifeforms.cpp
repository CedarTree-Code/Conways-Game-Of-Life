#include "lifeforms.h"

void randomLife(unsigned char* h_grid, double chance) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::bernoulli_distribution dist(chance);

    for (int i=0; i < sim_w*sim_h; i++) {
        if(dist(gen))
            h_grid[i] = 1; 
        else
            h_grid[i] = 0; 
    }
}