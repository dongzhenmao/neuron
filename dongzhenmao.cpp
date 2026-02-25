#include <stdio.h>
#include <vector> 
#include <math.h>

#include "neuron.hpp"

void view_DA_v() {
    printf("Time,DA_v\n");
    DA.burst();
    for (int i = 0; i < 2000; ++i) { // 200 ms
        if (i % 10 == 0) {
            printf("%f,%.6f\n", i / 10.0, DA.v());
        }
        DA.t_run();
    }    
}

void view_Ca_v() {

}

PN_neuron a, b;

void view_enhance() {
    printf("Time,W\n");
    link_random(&a, &b);
    printf("%lf\n", b.den.front().w());
}

int main() {
    view_enhance();
    
    return 0;
}

/*


*/