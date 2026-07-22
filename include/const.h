#pragma once

#define sim_w 128
#define sim_h 128

#define window_w 1080
#define window_h 1080

//B3 / S23 --- Conway's Game of Life
#define BORN (n==3) 
#define SURVIVE (n==2 || n==3)

#define ALIVE_COLOR 100, 200, 255 //R, G, B
#define DEAD_COLOR 0, 0, 0 