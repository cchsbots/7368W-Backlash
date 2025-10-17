//This file is named vex.h, and it includes all of the necessary libraries to run the program.
//This is a standard file that comes with the VEX coding programs.
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "v5.h"
#include "v5_vcs.h"

#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define WaitUntilorTimeout(condition)                                          \
  do {                                                                         \
    wait(5, msec)                                                              \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)