/*
 * CopyRight (c) 2019 gcj
 * File: tic_toc.hpp
 * Project: algorithm
 * Author: gcj
 * Date: 2019/5/30
 * Description: static time consumption
 * License: see the LICENSE.txt file
 * github: https://github.com/saber/algorithm
 */

#ifndef GLIB_TIC_TOC_H_
#define GLIB_TIC_TOC_H_

#pragma once

#include <ctime>
#include <cstdlib>
#include <chrono>

// 记录每个程序段使用的时间
class TicToc {
public:

    TicToc() {
        tic();
    }

    void tic() {
        start_ = std::chrono::system_clock::now();
    }

    double toc() {
        end_ = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end_ - start_;
        return elapsed_seconds.count() * 1000;
    }

private:
    std::chrono::time_point<std::chrono::system_clock> start_, end_;
};

#endif
