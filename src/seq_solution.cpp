#include "../includes/seq_solution.h"

using namespace std;

map<char, int> seq_solution::count_chars(const vector<char> &chars)
{
    map<char, int> counts;
    long seq_count_elapsed;
    {
        utimer seq_count("seq count", &seq_count_elapsed);
        for (char c : chars)
            counts[c]++;
    }

#ifdef DEBUG
    cout << "[SEQ] seq count elapsed: " << seq_count_elapsed << endl;
#endif

    return counts;
}

string seq_solution::encode(vector<char> chars, unordered_map<char, string> encoding_table)
{
    string encoded_string;
    long seq_encoding_elapsed;
    {
        utimer seq_encoding("seq encoding", &seq_encoding_elapsed);
        for (int i = 0; i < chars.size(); i++)
        {
            try
            {
                string code = encoding_table.at(chars[i]);
                encoded_string += code;
            }
            catch (const out_of_range &e)
            {
                cout << "[SEQ] Char is: " << chars[i] << endl;
                cerr << "[SEQ] Exception at " << e.what() << endl;
            }
        }
    }

#ifdef DEBUG
    cout << "[SEQ] seq encoding elapsed: " << seq_encoding_elapsed << endl;
#endif

    return encoded_string;
}

vector<bitset<8>> seq_solution::compress(string encoded_string)
{
    size_t string_size = encoded_string.size();
    size_t num_chunks = string_size / 8 + (string_size % 8 != 0);
    vector<bitset<8>> byte_chunks;
    byte_chunks.reserve(num_chunks);

    for (size_t i = 0; i < string_size; i += 8)
    {
        bitset<8> bits(encoded_string.substr(i, 8));
        byte_chunks.push_back(bits);
    }

    return byte_chunks;
}

vector<bitset<8>> seq_solution::encode_and_compress(const vector<char> &chars, const unordered_map<char, string> &encoding_table)
{
    string code_buffer;
    vector<bitset<8>> compressed_chunk;

    for (int i = 0; i < chars.size(); i++)
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
            cerr << "[SEQ] exception at " << e.what() << endl;
        }
    }

    return compressed_chunk;
}