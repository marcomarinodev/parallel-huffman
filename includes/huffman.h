#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#define INTERNAL_NODE_SYMBOL '$'

#include "./utils.h"

#include <bits/stdc++.h>

using namespace std;

/// @brief huffman tree node
struct min_heap_node
{
    // input character
    char data;

    // frequency
    unsigned freq;

    min_heap_node *left, *right;

    min_heap_node(char data, unsigned freq)
    {
        left = right = NULL;
        this->data = data;
        this->freq = freq;
    }
};

/// @brief used to compare two huffman tree nodes
struct compare
{
    bool operator()(min_heap_node* left, min_heap_node* right)
    {
        return left->freq > right->freq;
    }
};

/// @brief recursive print a huffman tree given its root
/// @param root
/// @param str accumulative string
void print_huffman_tree(struct min_heap_node* root, string str);

/// @brief it builds the huffman tree given the map with characters count
/// @param chars_count characters count
/// @return huffman tree root node
min_heap_node* build_huffman_tree(map<char, int> chars_count);

/// @brief it builds the encoding table using an existing huffman tree. Then you can
/// access to a character count by getting its value from the output map
/// @param huffman_tree huffman tree root node
/// @return encoding map
unordered_map<char, string> build_encoding_table(min_heap_node* huffman_tree);

/// @brief helper function of build_encoding_table function that  stores the encodes into 
/// the encoding table
/// @param encoding_table encoding table to fill
/// @param huffman_tree huffman root tree node
/// @param code accumulatore code string
void store_codes(unordered_map<char, string>& encoding_table, min_heap_node* huffman_tree, string code);

#endif