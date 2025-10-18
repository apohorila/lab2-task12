#include "timeit.h"
#include "constants.h"

#include <vector>
#include <random>

#include <execution>
#include <algorithm>

#include <iostream>
#include <iomanip>

vector<long long> generate_sequence (size_t size, int min_val, int max_val);

void part1(){
    const vector<size_t> N_values = {N_SMALL, N_MEDIUM, N_LARGE};
    std::cout << std::setw(10) << "N (Size)" 
              << std::setw(10) << "R (Runs)"
              << std::setw(15) << "Time"  
              <<"\n";

     for (size_t N : N_values){

        auto data = generate_sequence(N,1,1000);
        vector<long long> result(N);

        double time_implicit_seq;

     time_implicit_seq = timeit([&data, &result]() {
        std::inclusive_scan(data.begin(), data.end(), result.begin());
        }, REPEAT_TIMES);

        std::cout << std::fixed << std::setprecision(4)
                  << std::setw(10) << N
                  << std::setw(10) << REPEAT_TIMES
                  << std::setw(15) << time_implicit_seq
                  <<"\n";
        std::cerr << " Check sum: " << result.back() << std::endl;
        std::cout << "-------------------------------------------------------------------------------------------------------------------\n";
    }

}