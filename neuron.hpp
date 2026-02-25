#pragma once

#include <stdio.h>
#include <vector> 
#include <math.h>
#include <list>
#include <random>

#include "my_algorithm.hpp"
#include "overall/DA.hpp"
#include "overall/const_v.hpp"

struct neuron;

struct neuron {

    struct dendrite;
    struct axon;

    struct dendrite {
        static double Ca_f(double x);

        double Ca_rest() { return 0.05; };

        neuron *from;
        axon *link;

        double __w; 
        double __Ca_v;
        double __h; // 已激活的 NMDA 受体占比

        double &w() { return __w; }
        double &Ca_v() { return __Ca_v; }
        double &h() { return __h; }

        // dendrite() = default;
        dendrite(double l, double r);

        void get_release();
        void get_bap();
        void t_run();

    };

    struct axon {
        neuron *from;
        std::list<dendrite*> to; // 防止地址变动
        void release();
    };    

    virtual double a() = 0;
    virtual double b() = 0;
    virtual double c() = 0; // 复极化电位
    virtual double d() = 0; // 
    virtual int type() = 0;

    std::list<dendrite> den;
    axon ax;

    double __v; // 电位
    double __u; // 疲劳度
    double __I; // 输入电流

    double &v() { return __v; }
    double &u() { return __u; }
    double &I() { return __I; }

    void release();
    void t_run();

    dendrite *build_a_den(int pre_type);

    neuron() { 
        ax.from = this; 
    }
    
};

double neuron::dendrite::Ca_f(double x) {
    return x < 0.1 ? 0 : 0.006 * (x - 0.1) * (x - 0.8) / (x * x + 0.3);
}

neuron::dendrite::dendrite(double l, double r) { 
    h() = 0, Ca_v() = Ca_rest();
    w() = rand_double(l, r);
}

void neuron::dendrite::get_release() {
    from->I() += 100 * w();
    h() += (1 - h()) * 0.5;
}

void neuron::dendrite::get_bap() {
    Ca_v() += 4.5 * h() + 0.3;
}

void neuron::dendrite::t_run() {              // 随时间 min_dt 的自然损失
    // Ca_v = std::max(Ca_rest(), Ca_v + 0.003 * (Ca_rest() - Ca_v) - 0.003); // 前面是树突棘颈流出, 后面是离子泵
    Ca_v() = Ca_v() + 0.004 * (Ca_rest() - Ca_v()); // 树突棘颈流出

    h() *= 0.994;
    w() += min_dt * neuron::dendrite::Ca_f(Ca_v()) * DA.f();

    if (link->from->type() == 1) {            // 突触前神经元是兴奋性
        w() = std::max(std::min(w(), 2.0), 0.0);
    } else {                                  // 突触前神经元是抑制性
        w() = std::max(std::min(w(), 0.0), -2.0);
    }
}

neuron::dendrite *neuron::build_a_den(int pre_type) {
    if (pre_type == 1) {
        den.push_back(neuron::dendrite(0.2, 0.8));
    } else {
        den.push_back(neuron::dendrite(-0.8, 0.2));
    }
    den.back().from = this;
    return &den.back();
}

void link_random(neuron *pre, neuron *post) {
    neuron::dendrite *den = post->build_a_den(pre->type());
    pre->ax.to.push_back(den);
    den->link = &pre->ax;
}

void neuron::axon::release() {
    for (dendrite *den : to) {
        den->get_release();
    }
}

void neuron::release() {
    printf("sp!\n");

    ax.release();
    for (dendrite &d : den) d.get_bap();
    
    v() = c();
    u() = u() + d();
}

void neuron::t_run() {
    for (dendrite &d : den) d.t_run();

    double _v = v();
    v() += min_dt * (0.04 * v() * v() + 5 * v() + 140 - u() + I());
    u() += min_dt * a() * (b() * _v - u());
    I() *= 0.1;

    if (v() > 30.0) release();
}


struct PN_neuron : neuron {
    double a() override { return 0.02; }
    double b() override { return 0.2; }
    double c() override { return -70.0; }
    double d() override { return 8.0; }
    int type() override { return 1; }
    PN_neuron() : neuron() {
        __v = c();
        __u = c() * b();
        __I = 0;
    }
};

struct SST_neuron : neuron {
    double a() override { return 0.02; }
    double b() override { return 0.2; }
    double c() override { return -70.0; }
    double d() override { return 8.0; }
    int type() override { return -1; }
    SST_neuron() : neuron() {
        __v = c();
        __u = c() * b();
        __I = 0;
    }
};

