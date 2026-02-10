#include <stdio.h>
#include <vector> 
#include <math.h>

const double min_dt = 0.1; // 时间步 0.1 ms

double Ca_f(double x) { // min = -0.087, max -> 0.05
    return x < 0.2 ? 0 : 0.05 * (x - 0.2) * (x - 0.8) / (x * x + 0.3);
} 

const int max_n = 1e2;

struct neuron;
struct axon;
struct dendrite;

struct dendrite {
    static constexpr double Ca_rest = 0.05;

    neuron *from;
    double w; 
    double Ca_v;
    double h; // 已激活的 NMDA 受体占比

    void get_release();

    void get_bap() {
        Ca_v += 3.5 * h + 0.2;
    }

    void t_run() { // 随时间 min_dt 的自然损失
        Ca_v = std::max(Ca_rest, Ca_v + 0.003 * (Ca_rest - Ca_v) - 0.003); // 前面是流出, 后面是离子泵
        h *= 0.994;
        w += min_dt * Ca_f(Ca_v);
        w = std::max(std::min(w, 1.0), -1.0);
    }

};

struct axon {
    neuron *from;
    std::vector<dendrite*> to; 
    void release();
};

struct neuron {
    static constexpr double a = 0.02;
    static constexpr double b = 0.2;
    static constexpr double c = -67.0; // rest
    static constexpr double d = 8.0;

    int type; // 

    std::vector<dendrite> den;
    axon ax;

    double v, u, I;

    void release();

    void t_run();
    
} _neuron[max_n];

void dendrite::get_release() {
    from->I += w;
    h += (1 - h) * 0.5;
}

void axon::release() {
    for (dendrite *den : to) {
        den->get_release();
    }
}

void neuron::release() {
    ax.release();
    for (dendrite &d : den) {
        d.get_bap();
    }
    v = c;
    u = u + d;
}

void neuron::t_run() {
    double _v = v;
    v += min_dt * (0.04 * v * v + 5 * v + 140 - u + I);
    u += min_dt * a * (b * _v - u);
    I *= 0.1;
}

void solve() {
    constexpr double Ca_rest = 0.05;
    double Ca_v = 2;
    printf("Time,Ca_v\n");
    for (int i = 0; i < 1000; ++i) {
        Ca_v = std::max(Ca_rest, Ca_v + 0.004 * (Ca_rest - Ca_v) - 0.002);
        printf("%lf,%lf\n", 0.1 * i, Ca_f(Ca_v));
    }
}

int main() {
    solve();
    
    return 0;
}

/*


*/