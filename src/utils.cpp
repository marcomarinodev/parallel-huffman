//
// Created by marco.marino on 5/20/2023.
//

#include "../includes/utils.h"

using namespace std;

vector<char> par_read_file(const string &filename, int num_threads)
{
    ifstream file(filename, ios::binary);
    if (!file)
        cerr << "Err while opening file" << endl;

    file.seekg(0, ios::end);
    streampos file_size = file.tellg();
    file.seekg(0);

    vector<char> file_data(file_size);
    streampos chunk_size = file_size / num_threads;
    streampos remainder = file_size % num_threads;
    vector<thread> threads;
    vector<vector<char>> thread_buffers(num_threads);
    vector<streampos> chunk_starts(num_threads);
    vector<streampos> chunk_ends(num_threads);

    auto read_file_worker = [&](streampos start, streampos end, vector<char> &buffer)
    {
        ifstream file(filename, ios::binary);
        if (!file)
            cerr << "err while opening file" << endl;

        file.seekg(start);
        streamsize chunk_size = end - start;
        buffer.resize(chunk_size);

        file.read(buffer.data(), chunk_size);

        file.close();
    };

    for (int i = 0; i < num_threads; i++)
    {
        chunk_starts[i] = i * chunk_size;
        chunk_ends[i] = chunk_starts[i] + chunk_size;

        if (i == num_threads - 1)
            chunk_ends[i] += remainder;

        threads.emplace_back(read_file_worker, chunk_starts[i], chunk_ends[i], ref(thread_buffers[i]));
    }

    for (int i = 0; i < num_threads; i++)
    {
        threads[i].join();

        const vector<char> &buffer = thread_buffers[i];
        memcpy(file_data.data() + chunk_starts[i], buffer.data(), buffer.size());
    }

    return file_data;
}

void par_write_file_string(const string &filename, const string &data, int num_threads)
{
    streamsize data_size = data.size();
    streampos chunk_size = data_size / num_threads;
    streampos remainder = data_size % num_threads;
    ofstream file(filename, ios::binary);
    if (!file)
        cerr << "Err while opening file" << endl;
    vector<thread> threads;
    vector<streampos> chunk_starts(num_threads);
    vector<streampos> chunk_ends(num_threads);
    mutex file_mtx;

    // truncating the file ensuring a clean write
    file.close();
    file.open(filename, ios::binary | ios::app);

    auto write_file_string_worker = [&](const string& data, streampos start)
    {
        lock_guard<mutex> lock(file_mtx);
        file.seekp(start);
        file.write(data.c_str(), data.size());
    };

    for (int i = 0; i < num_threads; i++)
    {
        chunk_starts[i] = i * chunk_size;
        chunk_ends[i] = chunk_starts[i] + chunk_size;

        if (i == num_threads - 1)
            chunk_ends[i] += remainder;

        string data_chunk = data.substr(chunk_starts[i], chunk_ends[i] - chunk_starts[i]);
        threads.emplace_back(write_file_string_worker, move(data_chunk), chunk_starts[i]);
    }

    for (auto &thread : threads)
        thread.join();

    file.close();
}

vector<vector<bitset<8>>> par_compress(const vector<char> &chars, const unordered_map<char, string> &encoding_table)
{
    const int num_threads = 2;
    vector<vector<bitset<8>>> compressed_chunks(num_threads);
    mutex mtx;
    condition_variable cv;
    atomic<int> processed_chunks(0);
    atomic<bool> all_chunks_processed(false);

    auto compress_chunk_worker = [&](int index)
    {
        int chunk_size = chars.size() / num_threads;
        int start_index = index * chunk_size;
        int end_index = (index == num_threads - 1) ? chars.size() - 1 : start_index + chunk_size;
        string code_buffer;
        vector<bitset<8>> compressed_chunk;

        for (int i = start_index; i <= end_index; i++)
        {
            try
            {
                string code = encoding_table.at(chars[i]);
                code_buffer += code;

                while (code_buffer.size() >= 8)
                {
                    bitset<8> bits(code_buffer.substr(0, 8));
                    compressed_chunk.push_back(bits);
                    code_buffer = code_buffer.substr(8);
                }
            }
            catch (const out_of_range &e)
            {
                cerr << "Exception at " << e.what() << endl;
            }
        }

        // locking compressed_chunks
        unique_lock<mutex> lock(mtx);
        compressed_chunks[index] = compressed_chunk;
        processed_chunks++;

        if (processed_chunks == num_threads)
        {
            all_chunks_processed = true;
            cv.notify_all();
        }
        else
        {
            cv.wait(lock, [&]()
                    { return all_chunks_processed.load(); });
        }
    };

    vector<thread> encoding_threads;

    for (int i = 0; i < num_threads; i++)
        encoding_threads.emplace_back(compress_chunk_worker, i);

    for (auto &thread : encoding_threads)
        thread.join();

    return compressed_chunks;
}

void par_write_bits_chunks(vector<vector<bitset<8>>> compressed_chunks, const string &output_file)
{
    ofstream out_file(output_file, ios::binary | ios::out);
    mutex out_file_mtx;

    auto write_bits_worker = [&](int index, const vector<bitset<8>> &chunk)
    {
        lock_guard<mutex> lock(out_file_mtx);
        out_file.seekp(index * chunk.size());
        for (const auto &bits : chunk)
            out_file.put(static_cast<unsigned char>(bits.to_ulong()));
    };

    vector<thread> chunk_writer_threads;
    int j = 0;
    for (const auto &chunk : compressed_chunks)
    {
        chunk_writer_threads.emplace_back(
            write_bits_worker,
            j, chunk);
        j++;
    }

    for (auto &thread : chunk_writer_threads)
        thread.join();

    out_file.close();
}

void seq_write_bits_chunks(vector<vector<bitset<8>>> compressed_chunks, const string &output_file)
{
    ofstream out_file(output_file, ios::binary | ios::app);

    for (const auto &chunk : compressed_chunks)
    {
        for (const auto &bits : chunk)
            out_file.put(static_cast<unsigned char>(bits.to_ulong()));
    }
    
    out_file.close();
}


void active_wait(int usecs)
{
    auto start = chrono::high_resolution_clock::now();
    auto end = false;
    while (!end)
    {
        auto elapsed = chrono::high_resolution_clock::now() - start;
        auto usec = chrono::duration_cast<chrono::microseconds>(elapsed).count();
        if (usec > usecs)
            end = true;
    }
}

vector<char> read_file(string file_path)
{
    vector<char> characters;
    ifstream input_file(file_path);

    if (input_file.is_open())
    {
        char c;
        while (input_file.get(c))
            characters.push_back(c);

        input_file.close();
        return characters;
    }

    cout << "Unable to open a file named " << file_path << endl;
    return characters;
}

template <typename VecType>
bool vec_contains(vector<VecType> v, VecType element)
{
    return find(v.begin(), v.end(), element) != v.end();
}

void print_ascii_html_value(char c)
{
    cout << "&#" << dec << static_cast<int>(c) << ";";
}

string vec_to_string(vector<char> vec)
{
    string str(vec.begin(), vec.end());
    return str;
}

void print_string_vector(vector<std::string> vec)
{
    for (const auto &str : vec)
        cout << str << std::endl;
}
