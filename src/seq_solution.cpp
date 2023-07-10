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

    int remainder = encoded_string.length() % 8;

    if (remainder != 0)
        encoded_string.append(8 - remainder, '0');

    return encoded_string;
}

string seq_solution::compress(string encoded_string)
{
    string compressed;

    for (size_t i = 0; i < encoded_string.length(); i += 8)
    {
        bitset<8> bits(encoded_string.substr(i, 8));
        compressed += char(bits.to_ulong());
    }

    return compressed;
}
