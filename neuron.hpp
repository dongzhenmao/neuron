#include <stdio.h>
#include <vector> 
#include <math.h>
#include <list>
#include <random>


const double min_dt = 0.1; // 时间步 0.1 ms

std::mt19937 _gen(713);

double rand_double(double l, double r) {
    return std::uniform_real_distribution<double>(l, r)(_gen);
}

double Ca_f(double x) { // min = -0.087, max -> 0.05
    return x < 0.2 ? 0 : 0.04 * (x - 0.2) * (x - 0.8) / (x * x + 0.3);
} 

const int max_n = 1e2;

struct neuron;

struct neuron {

    struct axon;

    struct dendrite {
        static constexpr double Ca_rest = 0.05;

        neuron *from;
        axon *link;

        double w; 
        double Ca_v;
        double h; // 已激活的 NMDA 受体占比

        dendrite() = default;
        dendrite(double l, double r) { 
            h = 0, Ca_v = Ca_rest;
            w = rand_double(l, r);
        }

        void get_release();

        void get_bap() {
            Ca_v += 3.2 * h + 0.25;
        }

        void t_run();

    };

    struct axon {
        neuron *from;
        std::vector<dendrite*> to; 
        void release();
    };    

    virtual double a() = 0;
    virtual double b() = 0;
    virtual double c() = 0;
    virtual double d() = 0;
    virtual int type() = 0;

    std::list<dendrite> den;
    axon ax;

    double v, u, I;

    void release();

    void t_run();
    
};

template<typename neuron>
void link_random(neuron *pre, neuron *post) {
    if (pre->type() == 0) post->den.push_back(dendrite<neuron>(0, 1));
    else post->den.push_back(dendrite<neuron>(-1, 0));
    pre->ax.to.push_back(&post->den.back());
}

void neuron::dendrite::get_release() {
    from->I += w;
    h += (1 - h) * 0.5;
}

void neuron::axon::release() {
    for (dendrite *den : to) {
        den->get_release();
    }
}

void neuron::release() {
    ax.release();
    for (neuron::dendrite &d : den) {
        d.get_bap();
    }
    v = c();
    u = u + d();
}

void neuron::t_run() {
    double _v = v;
    v += min_dt * (0.04 * v * v + 5 * v + 140 - u + I);
    u += min_dt * a() * (b() * _v - u);
    I *= 0.1;
}


struct PN_neuron : neuron {
    double a() override { return 0.02; }
    double b() override { return 0.2; }
    double c() override { return -67.0; }
    double d() override { return 8.0; }
    int type() override { return 0; }
};

struct up_neuron : neuron {
    double a() override { return 0.02; }
    double b() override { return 0.2; }
    double c() override { return -67.0; }
    double d() override { return 8.0; }
    int type() override { return 1; }    
};

