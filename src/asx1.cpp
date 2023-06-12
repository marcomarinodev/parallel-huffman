//
// Created by marco.marino on 5/20/2023.
//

#include "../includes/asx1.h"

using namespace std;

void asx1::rand_init(vector<double>* input)
{
    int seed = 123;
    const int max = 10;
    srand(seed);

    for (size_t i = 0; i < (*input).size(); i++)
        (*input)[i] = (double) (rand() % max);
}

void asx1::print_vector(const string& vec_name, vector<double> input)
{
    cout << vec_name << ": ";
    for (size_t i = 0; i < input.size(); i++)
        cout << input[i] << " ";
    cout << endl;
}

void asx1::execute(int vlen, int nw)
{
    vector<double> v(vlen);

    rand_init(&v);
    print_vector("Initial input vector", v);

    auto f = [] (double x) { return x * 2; };

    apply_func_to_vec<double>(&v, f, vlen, nw);
}

