#include <stdio.h>
#include <vector> 
// #include <>

const int max_n = 1e2;

struct neuron;
struct axon;
struct dendrite;

struct dendrite {
    neuron *from;
    double w; 
} _dendrite[max_n];

struct axon {
    neuron *from;
    std::vector<dendrite*> to; 
    void release();
} _axon[max_n];

struct neuron {
    int type; // 

    std::vector<dendrite> den;
    axon ax;

    double Ca_v; 
    double K_v;
    double Na_v;

    double v;
    
} _neuron[max_n];
 
void axon::release() {
    for (dendrite *den : to) {
        den->from->v += den->w;
    }
}







void solve() {

}

int main() {
    solve();
    
    return 0;
}

/*


*/