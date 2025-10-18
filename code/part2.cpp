#include "timeit.h"
#include "constants.h"
#include "sequence_generator.h"

#include <vector>
#include <random>

#include <execution>
#include <algorithm>

#include <iostream>
#include<iomanip>

using namespace std;

vector<long long> generate_sequence();

void library_experiments(){
    const vector<size_t> N_values = {N_SMALL, N_MEDIUM, N_LARGE};

    std::cout << std::setw(10) << "N (Size)" 
              << std::setw(10) << "R (Runs)"
              << std::setw(15) << "1. std::seq"     
              << std::setw(15) << "2. std::par"    
              << std::setw(15) << "3. std::unseq"   
              << std::setw(15) << "4. std::par_unseq"
              << "\n";
    std::cout << "-------------------------------------------------------------------------------------------------------------------\n";

    for (size_t N : N_values){


        auto data = generate_sequence(N,1,1000);
        vector<long long> result(N);

        double time_seq = 0.0;
        double time_par = 0.0;
        double time_unseq = 0.0;
        double time_par_unseq = 0.0;


        time_seq = timeit([&data, &result]() {
        std::inclusive_scan(execution::seq,data.begin(), data.end(), result.begin());
        }, REPEAT_TIMES);

        time_par = timeit([&data, &result]() {
        std::inclusive_scan(execution::par,data.begin(), data.end(), result.begin());
        }, REPEAT_TIMES);

        time_unseq = timeit([&data, &result]() {
        std::inclusive_scan(execution::unseq,data.begin(), data.end(), result.begin());
        }, REPEAT_TIMES);

        time_par_unseq = timeit([&data, &result]() {
        std::inclusive_scan(execution::par_unseq,data.begin(), data.end(), result.begin());
        },  REPEAT_TIMES);

        std::cout << std::fixed << std::setprecision(4)
                  << std::setw(10) << N
                  << std::setw(10) << REPEAT_TIMES
                  << std::setw(15) << time_seq
                  << std::setw(15) << time_par
                  << std::setw(15) << time_unseq
                  << std::setw(15) << time_par_unseq
                  << "\n";
                 cout <<"\n";

        std::cerr << " Check sum: " << result.back() << std::endl;
        std::cout << "-------------------------------------------------------------------------------------------------------------------\n";
    }

}

void part2(){ 
library_experiments();
}