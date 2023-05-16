//
// Created by marco.marino on 5/16/2023.
//
#include <vector>
#include <algorithm>
#include <future>
#include "utils.hpp"

#ifndef ASX2_HPP
#define ASX2_HPP

namespace asx2
{
    template<typename Tin, typename Tout>
    Tout dc(Tin input, bool (*basecase)(Tin), Tout (*solve)(Tin), std::vector<Tin> (*divide)(Tin), Tout (*conquer)(std::vector<Tout>))
    {
        if (basecase(input)) return solve(input);

        std::vector<Tin> subproblems = divide(input);

        std::vector<std::future<Tout>> futures(subproblems.size());

        transform(subproblems.begin(), subproblems.end(), futures.begin(), [&](Tin x) {
            return async(std::launch::async, dc<Tin, Tout>, x, basecase, solve, divide, conquer);
        });

        std::vector<Tout> final;
        final.reserve(futures.size());

        for (auto &future: futures)
            final.push_back(future.get());

        auto result = conquer(final);

        return result;
    }

    bool testVecsIfEquals(std::vector<int> vec1, std::vector<int> vec2);

    // lambdas
    auto basecase = [](std::vector<int> vec) {
        return vec.size() < 64;
    };

    auto solve = [](std::vector<int> vec) {
        std::vector<int> doubled_vec;
        doubled_vec.reserve(vec.size());
        for (auto el: vec) {
            active_wait(200);
            doubled_vec.push_back(el * 2);
        }

        return doubled_vec;
    };

    auto divide = [](std::vector<int> vec) {
        const int half_size = vec.size() / 2;
        std::vector<int> split_lo(vec.begin(), vec.begin() + half_size);
        std::vector<int> split_hi(vec.begin() + half_size, vec.end());

        return std::vector<std::vector<int>> {split_lo, split_hi};
    };

    auto conquer = [](std::vector<std::vector<int>> sums) {
        std::vector<int> result;
        size_t alloc_size = 0;

        for (const auto & sum : sums)
            alloc_size += sum.size();

        result.reserve(alloc_size);

        for (const auto & sum : sums)
            result.insert(result.end(), sum.begin(), sum.end());

        return result;
    };

    void execute(int size);
}

#endif
