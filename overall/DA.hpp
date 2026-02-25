#pragma once

struct __DA { // umol
    double __v;

    double rest() { return 0.05; }

    __DA() : __v(rest()) {}

    void burst() { 
        __v = std::min(__v + 0.045, 0.3);
    }

    void dip() {
        __v = std::max(__v - 0.045, 0.0);
    }

    double v() { // 获取实时多巴胺浓度
        return __v; 
    }

    double rare_f() {
        if (v() < 0.04) return v() - 0.04;      // 削弱
        else if (v() > 0.06) return v() - 0.06; // 强化
        else return 0;                          // 不改变
    }

    double f() { return rare_f() * 80; }

    void t_run() { // 时间步 0.1 ms
        __v = __v + (rest() - v()) * 0.0011;
    }

} DA;




