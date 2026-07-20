#pragma once

#define sim_w 128
#define sim_h 72

#define window_w 1280
#define window_h 720

//B3 / S23 --- Conway's Game of Life
#define BORN (n==3) 
#define SURVIVE (n==2 || n==3)

#define ALIVE_COLOR 255, 10, 255 //R, G, B
#define DEAD_COLOR 0, 0, 0 