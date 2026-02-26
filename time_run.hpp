#pragma once

#include "overall/overall.hpp"
#include "neuron.hpp"



void t_run() {
    
    for (neuron &n : neu) n.t_run();
    DA.t_run();
    next_time();
}

