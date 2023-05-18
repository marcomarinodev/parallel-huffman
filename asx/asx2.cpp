//
// Created by marco.marino on 5/16/2023.
//
#include "asx2.hpp"

using namespace std;

bool asx2::testVecsIfEquals(vector<int> vec1, vector<int> vec2)
{
    if (vec1.size() != vec2.size()) return false;

    for (size_t i = 0; i < vec1.size(); i++)
        if (vec1[i] != vec2[i]) return false;

    return true;
}

void asx2::execute(int size)
{
    vector<int> v(size, 1);
    vector<int> test_doubled_v;

    { utimer t1("sequential sum");
        vector<int> doubled_v = asx2::solve(v);
        test_doubled_v = doubled_v;
    }

    { utimer t0("parallel sum");
        auto doubled_v = asx2::dc<vector<int>, vector<int>>(v, asx2::basecase, asx2::solve, asx2::divide, asx2::conquer);

        if (asx2::testVecsIfEquals(test_doubled_v, doubled_v))
            cout << "result is correct" << endl;
        else cout << "invalid result" << endl;
    }
}