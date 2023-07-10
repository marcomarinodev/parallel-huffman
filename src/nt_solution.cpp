#include "../includes/nt_solution.h"

using namespace std;

map<char, int> nt_solution::count_chars(const vector<char> &chars, int num_threads)
{
    vector<map<char, int>> chunk_counts(num_threads);
    map<char, int> result;
    long par_count_chars_elapsed;
    {
        utimer par_count_chars_timer("par count chars timer", &par_count_chars_elapsed);
        int chunk_size = chars.size() / num_threads;
        vector<thread> threads;

        mutex counts_mtx;

        auto worker = [&](int index)
        {
            int start = index * chunk_size;
            int end = index == num_threads - 1 ? chars.size() : start + chunk_size;
            map<char, int> local_counts;

            for (int i = start; i < end; i++)
                local_counts[chars[i]]++;

            chunk_counts[index] = local_counts;
        };

        for (int i = 0; i < num_threads; i++)
            threads.emplace_back(worker, i);

        for (int i = 0; i < num_threads; i++)
        {
            threads[i].join();
            for (auto entry: chunk_counts[i])
                result[entry.first] += entry.second;
        }
    }

#ifdef DEBUG
    cout << "[PAR] par count chars elapsed: " << par_count_chars_elapsed << endl;
#endif

    return result;
}

nt_solution::Gmr::Gmr(int map_nw, int red_nw)
{
    this->map_nw = map_nw;
    this->red_nw = red_nw;

    mappers_outputs.resize(this->map_nw);
    reducers_queue.resize(this->red_nw);

    vector<mutex> tmp_rq_mutexes(this->red_nw);
    reducers_queue_mutexes.swap(tmp_rq_mutexes);
}

map<char, int> nt_solution::Gmr::count_chars(vector<char> input)
{
    long map_reduce_elapsed;
    {
        utimer map_reduce_timer("map reducer timer", &map_reduce_elapsed);
        // mapper's phase
        // mapper function body (i-th mapper)
        auto mapper_body = [&](int i)
        {
            int chunk_size = input.size() / map_nw;
            int lo_bound = i * chunk_size;
            int hi_bound = i == map_nw - 1 ? input.size() : (i + 1) * chunk_size;

            // executing the mapper on that input's chunk
            for (int j = lo_bound; j < hi_bound; j++)
            {
                auto key = input[j];
                if (mappers_outputs[i].count(key))
                    mappers_outputs[i][key]++;
                else
                    mappers_outputs[i][key] = 1;
            }

            // bringing the mapper results to the reducers queue
            for (const auto &entry : mappers_outputs[i])
            {
                // we use for each entry a hash function
                // to decide which is the recipient queue
                auto reducer_queue_index = (hash<char>{}(entry.first)) % red_nw;

                // locking the recipient queue to push back the mapper result
                unique_lock<mutex> u_lock(reducers_queue_mutexes[reducer_queue_index]);

                reducers_queue[reducer_queue_index].emplace_back(entry);
            }

            return;
        };

        // starting the mappers
        thread mapper_threads_ids[map_nw];
        for (int i = 0; i < map_nw; ++i)
            mapper_threads_ids[i] = thread(mapper_body, i);

        for (int i = 0; i < map_nw; ++i)
            mapper_threads_ids[i].join();

        // reducer's phase
        auto reducer_body = [&](int i)
        {
            // internal map
            map<char, int> internal_map;
            // taking entries from its own queue and feeding the internal map
            // by reducing the entries
            for (auto entry : reducers_queue[i])
            {
                auto key = entry.first;
                auto value = entry.second;

                if (internal_map.count(key))
                    internal_map[key] += value;
                else
                    internal_map[key] = value;
            }

            // merging the internal map with the shared result map
            for (auto entry : internal_map)
            {
                auto key = entry.first;
                auto value = entry.second;

                unique_lock<mutex> rr_lock(reducers_result_mutex);
                if (reducers_result.count(key))
                    reducers_result[key] += value;
                else
                    reducers_result[key] = value;
            }
        };

        // starting reducers
        thread reducer_threads_ids[red_nw];
        for (int i = 0; i < red_nw; ++i)
            reducer_threads_ids[i] = thread(reducer_body, i);

        for (int i = 0; i < red_nw; ++i)
            reducer_threads_ids[i].join();
    }

#ifdef DEBUG
    cout << "[PAR] Map Reduce Counting characters elapsed: " << map_reduce_elapsed << endl;
#endif

    return reducers_result;
}

string nt_solution::encode(const vector<char> &chars, const unordered_map<char, string> &encoding_table, int num_threads)
{
    string encoded_result;
    long par_encoding_elapsed;
    {
        utimer par_encoding_timer("par encoding timer", &par_encoding_elapsed);
        vector<string> encoded_chunks(num_threads);

        auto encode_chunk_worker = [&](int index)
        {
            int chunk_size = chars.size() / num_threads;
            int start_index = index * chunk_size;
            int end_index = (index == num_threads - 1) ? chars.size() - 1 : start_index + chunk_size - 1;

            string encoded_chunk;

            for (int i = start_index; i <= end_index; i++)
            {
                try
                {
                    string code = encoding_table.at(chars[i]);
                    encoded_chunk += code;
                }
                catch (const out_of_range &e)
                {
                    cerr << "[PAR] Exception at " << e.what() << endl;
                }
            }

            // now you have a string with encoded bits
            encoded_chunks[index] = encoded_chunk;
        };

        vector<thread> encoding_threads;

        for (int i = 0; i < num_threads; i++)
            encoding_threads.emplace_back(encode_chunk_worker, i);

        for (int i = 0; i < num_threads; i++)
        {
            encoding_threads[i].join();
            encoded_result += encoded_chunks[i];
        }
    }

#ifdef DEBUG
    cout << "[PAR] par encoding elapsed: " << par_encoding_elapsed << endl;
#endif

    int remainder = encoded_result.length() % 8;

    if (remainder != 0)
        encoded_result.append(8 - remainder, '0');

    return encoded_result;
}

string nt_solution::compress(string encoded_string, int num_threads)
{
    vector<string> compressed_chunks(num_threads);
    string result;
    vector<thread> threads;
    int chunk_size = encoded_string.length() / num_threads;
    chunk_size = chunk_size - chunk_size % 8;

    auto compress_worker = [&](int index)
    {
        int start = index * chunk_size;
        int end = index == num_threads - 1 ? encoded_string.length() : start + chunk_size;
        string compressed_chunk;

        // start - end % 8 = 0
        for (int i = start; i < end; i += 8)
        {
            bitset<8> bits(encoded_string.substr(i, 8));
            compressed_chunk += char(bits.to_ulong());
        }

        compressed_chunks[index] = compressed_chunk;
    };

    for (int i = 0; i < num_threads; i++)
        threads.emplace_back(compress_worker, i);

    // merging the vector<bitset<8>>
    for (int i = 0; i < num_threads; i++)
        threads[i].join();

    for (int i = 0; i < num_threads; i++)
        result += compressed_chunks[i];

    return result;
}
