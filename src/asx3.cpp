//
// Created by marco.marino on 5/21/2023.
//

#include <map>
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <iostream>
#include <functional>

using namespace std;

class Asx3
{
public:
    int def_map_nw = 2;
    int def_red_nw = 2;
    // mappers outputs
    vector<map<char, int>> mappers_outputs;
    // reducers queues
    vector<vector<pair<char, int>>> reducers_queue;
    // reducers queues mutexes
    vector<mutex> rq_mutexes;

    mutex reducers_result_mutex;
    map<char, int> reducers_result;

    Asx3(int def_map_nw, int def_red_nw)
    {
        this->def_map_nw = def_map_nw;
        this->def_red_nw = def_red_nw;

        mappers_outputs.resize(this->def_map_nw);
        reducers_queue.resize(this->def_red_nw);

        vector<mutex> tmp_rq_mutexes(this->def_red_nw);
        rq_mutexes.swap(tmp_rq_mutexes);
    }

    template <typename Key, typename Value, typename Input>
    map<Key, Value> par_gmr(
            vector<Input> input,
            int mapper_nw, int reducer_nw)
    {
        // mapper's phase
        // mapper function body (i-th mapper)
        auto mapper_body = [&](int i)
        {
            int chunk_size = input.size() / mapper_nw;
            int lo_bound = i * chunk_size;
            int hi_bound = i == mapper_nw - 1 ? input.size() : (i + 1) * chunk_size;

            // executing the mapper on that input's chunk
            for (int j = lo_bound; j < hi_bound; j++)
            {
                auto key = input[j];
                if (mappers_outputs[i].count(key)) mappers_outputs[i][key]++;
                else mappers_outputs[i][key] = 1;
            }

            // bringing the mapper results to the reducers queue
            for (const auto& entry: mappers_outputs[i])
            {
                // we use for each entry a hash function
                // to decide which is the recipient queue
                auto reducer_queue_index = (hash<char>{}(entry.first)) % reducer_nw;

                // locking the recipient queue to push back the mapper result
                unique_lock<mutex> u_lock(rq_mutexes[reducer_queue_index]);

                reducers_queue[reducer_queue_index].emplace_back(entry);
            }

            return;
        };

        // starting the mappers
        thread mapper_threads_ids[mapper_nw];
        for (int i = 0; i < mapper_nw; ++i)
            mapper_threads_ids[i] = thread(mapper_body, i);

        for (int i = 0; i < mapper_nw; ++i)
            mapper_threads_ids[i].join();

        // reducer's phase
        auto reducer_body = [&](int i)
        {
            // internal map
            map<Key, Value> internal_map;
            // taking entries from its own queue and feeding the internal map
            // by reducing the entries
            for (auto entry: reducers_queue[i])
            {
                auto key = entry.first;
                auto value = entry.second;

                if (internal_map.count(key)) internal_map[key] += value;
                else internal_map[key] = value;
            }

            // merging the internal map with the shared result map
            for (auto entry: internal_map)
            {
                auto key = entry.first;
                auto value = entry.second;

                unique_lock<mutex> rr_lock(reducers_result_mutex);
                if (reducers_result.count(key)) reducers_result[key] += value;
                else reducers_result[key] = value;
            }
        };

        // starting reducers
        thread reducer_threads_ids[reducer_nw];
        for (int i = 0; i < reducer_nw; ++i)
            reducer_threads_ids[i] = thread(reducer_body, i);

        for (int i = 0; i < reducer_nw; ++i)
            reducer_threads_ids[i].join();

        return reducers_result;
    }

    template <typename Key, typename Value, typename Input>
    map<Key, Value> gmr(
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

        return reducer_map;
    }
};
