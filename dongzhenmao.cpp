#include <stdio.h>
#include <vector> 
#include <math.h>

#include "neuron.hpp"

void view() {
    constexpr double Ca_rest = 0.05;
    double Ca_v = 2.2;
    printf("Time,Ca_v\n");
    for (int i = 0; i < 500; ++i) {
        Ca_v = std::max(Ca_rest, Ca_v + 0.003 * (Ca_rest - Ca_v) - 0.003);
        printf("%lf,%lf\n", 0.1 * i, Ca_f(Ca_v));
    }
}

void build() {

}

void solve() {

}

int main() {
    // view();
    
    return 0;
}

/*


*/