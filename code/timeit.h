#pragma once

#include <iostream>

#include <chrono>
#include <format>

using namespace std;

template <class F>
auto timeit(F &&f, int repeat=1)
{
    auto time1 = chrono::high_resolution_clock::now();
    f();
    auto time2 = chrono::high_resolution_clock::now();
    auto tdiff = time2 - time1;
    return std::chrono::duration<double, std::milli>(tdiff).count();
    
}