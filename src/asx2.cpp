//
// Created by marco.marino on 5/20/2023.
//

#include "../includes/asx2.h"

using namespace std;


void asx2::execute(int size)
{
    vector<int> v(size, 1);
    vector<int> test_doubled_v;

    { utimer t1("sequential sum");
        vector<int> doubled_v = solve(v);
        test_doubled_v = doubled_v;
    }

    { utimer t0("parallel sum");
        auto doubled_v = dc<vector<int>, vector<int>>(v, basecase, solve, divide, conquer);

        if (testEqualVectors(test_doubled_v, doubled_v))
            cout << "result is correct" << endl;
        else cout << "invalid result" << endl;
    }
}