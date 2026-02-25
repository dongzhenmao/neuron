#include <stdio.h>
#include <vector> 
#include <math.h>

#include "neuron.hpp"

void view_DA() {
    DA.burst();
    for (int i = 0; i < 2000; ++i) { // 200 ms
        if (i % 10 == 0) {
            printf("%f %.6f\n", i / 10.0, DA.v());
        }
        DA.t_run();
    }    
}

int main() {
    view_DA();
    
    return 0;
}

/*


*/