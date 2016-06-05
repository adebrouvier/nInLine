#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rand.h"

void 
randomize(void)
{
srand(time(NULL));
}

double 
randNormalize(void)
{
return rand() / ( (double) RAND_MAX + 1);
}


int 
randInt(int izq, int der)
{
return (int)(randNormalize() * (der - izq + 1) + izq);
}

double 
randReal(double izq, double der)
{
return (randNormalize() * (der - izq) + izq);
}
