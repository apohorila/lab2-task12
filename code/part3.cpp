#include <iostream>
#include <iomanip>
#include <functional>

#include <execution>
#include <algorithm>

using namespace std;
#include "timeit.h"
#include "constants.h"

#include <thread>
#include <random>

using InputIterator = vector<long long>::const_iterator;
using ResultIterator = vector<long long>::iterator;
vector<long long> generate_sequence (size_t size, int min_val, int max_val);

template<typename InputIterator, typename ResultIterator>
void thread_local_scan(InputIterator data_first, InputIterator data_last, ResultIterator result_first){
    inclusive_scan(data_first, data_last, result_first);
}

void merge_results(std::vector<long long>& result, size_t K, size_t N)
{
    if (K <= 1) return;

    size_t block_size = N / K;
    
    for (size_t k = 1; k < K; ++k)
    {
        size_t start_of_current_block = k * block_size;
        long long prefix_sum_to_add = result[start_of_current_block - 1]; 
        size_t end_of_current_block;

        if (k == K - 1) {
            end_of_current_block = N;
        } else {
            end_of_current_block = start_of_current_block + block_size;
        }
        for (size_t i = start_of_current_block; i < end_of_current_block; ++i)
        {
            result[i] += prefix_sum_to_add;
        }
    }
}

void parallel_inclusive_scan(const vector<long long>& data, vector<long long>& result, size_t K){
    size_t data_size = data.size();
    if (K <= 0 || data_size == 0) return;
    if (K > data_size) K = data_size;

    size_t block_size = (data_size + K - 1) / K;
    vector <thread> threads;

    for (size_t i = 0; i < K; ++i){

        size_t start = i * block_size;
        size_t end = std::min(start + block_size, data_size);


        threads.emplace_back([&data, &result, start, end](){
            thread_local_scan(data.begin() + start, 
                              data.begin() + end,
                              result.begin() + start);
        });

    }
    for (auto& t : threads){
            t.join();
        }
    merge_results(result, K, data_size);
}

void print_result(size_t K, double time_ms){
    std::cout << std::fixed << std::setprecision(4)
              << std::setw(15) << K 
              << std::setw(15) << time_ms 
              << std::endl;
}
void analyze_par_inclusive_scan(){

    const size_t N = N_LARGE;
    const int repeat = REPEAT_TIMES;
    int max_threads = thread::hardware_concurrency();

    auto data = generate_sequence(N, 1, 1000);
    vector<long long> result(N);

    double best_time = numeric_limits<double>::max();
    size_t best_K = 0;

    std::vector<size_t> K_values = {1, 2, 4};
    if (max_threads > 4) K_values.push_back(max_threads);
    K_values.push_back(max_threads * 2);

    std::sort(K_values.begin(), K_values.end());
    K_values.erase(std::unique(K_values.begin(), K_values.end()), K_values.end());

    std::cout << "Data Size: N=" << N << ", Hardware Threads: " << max_threads << "\n";
    std::cout << "Measuring time (time with " << repeat << " repeats) for different K:\n";
    std::cout << std::setw(15) << "K (Threads)"
          << std::setw(15) << "Time (ms)"
          << "\n";
    std::cout << "--------------------------------\n";

    for (size_t K : K_values){
        double time_ms = timeit([&data, &result, K]() {
            parallel_inclusive_scan(data, result, K);
        }, repeat);

        print_result(K, time_ms);

        if (time_ms < best_time){
            best_time = time_ms;
            best_K = K;
        }
        cerr<<"Check sum:" << result.back()<<endl;
    }

    std::cout << "\n--- RESULTS ANALYSIS ---\n";
    std::cout << "Best K (Fastest Time) found at K = " << best_K 
          << " (Time: "<< std::fixed << std::setprecision(4) << best_time << " ms)\n";
    std::cout << "Ratio K_best / P_hardware: " << (double)best_K / max_threads << "\n";
    std::cout << "Time growth law for K > K_best: Time increases due to increased thread overhead and the dominating sequential merge phase (Amdahl's Law).\n";

    }

    void part3(){

        analyze_par_inclusive_scan();
    }