//
// Created by marco.marino on 5/20/2023.
//

#include "../includes/utils.h"

using namespace std;

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

void read_file_chunk(const string &filename, streampos start, streampos end, vector<char> &buffer)
{
    ifstream file(filename, ios::binary);
    if (!file)
        cerr << "err while opening file" << endl;

    file.seekg(start);
    streamsize chunk_size = end - start;
    buffer.resize(chunk_size);

    file.read(buffer.data(), chunk_size);

    file.close();
}

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

    for (int i = 0; i < num_threads; i++)
    {
        chunk_starts[i] = i * chunk_size;
        chunk_ends[i] = chunk_starts[i] + chunk_size;

        if (i == num_threads - 1)
            chunk_ends[i] += remainder;

        threads.emplace_back(read_file_chunk, filename, chunk_starts[i], chunk_ends[i], ref(thread_buffers[i]));
    }

    for (int i = 0; i < num_threads; i++)
    {
        threads[i].join();

        const vector<char> &buffer = thread_buffers[i];
        memcpy(file_data.data() + chunk_starts[i], buffer.data(), buffer.size());
    }

    return file_data;
}

void write_file_chunk(const string &filename, const string &data, streampos start)
{
    ofstream file(filename, ios::binary | ios::app);
    if (!file)
        cerr << "err while opening file" << endl;

    file.seekp(start);
    file.write(data.c_str(), data.size());
    file.close();
}

void par_write_file(const string &filename, const string &data, int num_threads)
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

    // truncating the file ensuring a clean write
    file.close();
    file.open(filename, ios::binary | ios::app);

    for (int i = 0; i < num_threads; i++)
    {
        chunk_starts[i] = i * chunk_size;
        chunk_ends[i] = chunk_starts[i] + chunk_size;

        if (i == num_threads - 1)
            chunk_ends[i] += remainder;

        string data_chunk = data.substr(chunk_starts[i], chunk_ends[i] - chunk_starts[i]);
        threads.emplace_back(write_file_chunk, filename, move(data_chunk), chunk_starts[i]);
    }

    for (auto& thread: threads) thread.join();
}