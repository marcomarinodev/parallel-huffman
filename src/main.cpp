#include <cstring>
#include "../includes/asx1.h"
#include "../includes/asx2.h"
#include "asx3.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    cout << "starting" << endl;
    // cli arguments
    const bool print_res = argc > 1 && !strcmp(argv[1], "p");
    const string filename = argc > 2 ? argv[2] : "sample";

    vector<string> words = read_file("./inputs/" + filename + ".txt");
    map<string, int> par_words_count, seq_words_count;

    Asx3 asx3(2, 2);

    int map_nw = asx3.def_map_nw;
    int red_nw = asx3.def_red_nw;

    auto mapper = [](string w) { return make_pair(w, 1); };
    auto reduce = [](int a, int b) { return a + b; };

    { utimer t0("par_words_count");
        par_words_count = asx3.par_gmr<string, int, string>(words, map_nw, red_nw);
    }

    { utimer t1("seq_words_count");
        seq_words_count = asx3.gmr<string, int, string>(words, mapper, reduce);
    }

    if (print_res) {
        for (auto word: par_words_count)
            cout << "(" << word.first << ", " << word.second << ")" << endl;
        cout << "================" << endl;

        for (auto word: seq_words_count)
            cout << "(" << word.first << ", " << word.second << ")" << endl;
    }

    if (par_words_count == seq_words_count)
        cout << "The result is correct!" << endl;
    else cout << "Invalid result!" << endl;

    // asx1::execute(vlen, nw);
    // asx2::execute(size);

    return 0;
}