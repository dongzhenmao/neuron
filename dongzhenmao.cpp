#include <stdio.h>
#include <vector> 
#include <math.h>

#include <neuron.hpp>

void view() {
    constexpr double Ca_rest = 0.05;
    double Ca_v = 2;
    printf("Time,Ca_v\n");
    for (int i = 0; i < 1000; ++i) {
        Ca_v = std::max(Ca_rest, Ca_v + 0.004 * (Ca_rest - Ca_v) - 0.002);
        printf("%lf,%lf\n", 0.1 * i, Ca_f(Ca_v));
    }
}

void build() {

}

void solve() {

}

int main() {
    solve();
    
    return 0;
}

/*


*/