#include <map>
#include "./asx/asx1.hpp"
#include "./asx/asx2.hpp"

using namespace std;

template <typename Key, typename Value, typename Input>
vector<pair<Key, Value>> gmr(
        vector<Input> input,
        function<pair<Key, Value>(Input)> mapper,
        function<Value(Value,Value)> reducer)
{
    // mapper output
    vector<pair<Key, Value>> mapper_output(input.size());

    // this may be turned parallel with the execution mode and linking with ltbb
    // execution::par as a first parameter
    transform(input.begin(), input.end(), mapper_output.begin(), mapper);

    // map used by reducer
    map<Key, Value> reducer_map;

    for(auto &entry: mapper_output)
    {
        auto key = entry.first;
        auto value = entry.second;

        if (reducer_map.count(key))
            reducer_map[key] = reducer(reducer_map[key], value);
        else
            reducer_map[key] = value;
    }

    vector<pair<Key, Value>> result;
    for (auto entry: reducer_map) result.push_back(entry);

    return result;
}

int main(int argc, char* argv[])
{
    auto mapper = [](string w) { return make_pair(w, 1); };
    auto reduce = [](int a, int b) { return a + b; };

    vector<string> words = read_file("asx/inputs/sample.txt");

    vector<pair<string, int>> words_count = gmr<string, int, string>(words, mapper, reduce);

    for (auto word: words_count) 
        cout << "(" << word.first << ", " << word.second << ")" << endl;

    // asx1::execute(vlen, nw);
    // asx2::execute(size);

    return 0;
}