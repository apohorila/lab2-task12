#pragma once
#include <vector>
#include <random>

using namespace std;

vector<long long> generate_sequence (size_t size, int min_val, int max_val){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> distribution(min_val, max_val);

     vector<long long> data;
     data.reserve(size);

    for (size_t i =0; i < size; i++){
        long long rnd = distribution(gen);
        data.push_back(rnd);
    }
        
    return data;

}