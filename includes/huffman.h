#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <bits/stdc++.h>
#include <bitset>
#include "./utils.h"

#define INTERNAL_NODE_SYMBOL '$'

using namespace std;

// A Huffman tree node
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

// to compare two 'min_heap_node's 
struct compare
{
    bool operator()(min_heap_node* left, min_heap_node* right)
    {
        return left->freq > right->freq;
    }
};

void print_huffman_tree(struct min_heap_node* root, string str);

min_heap_node* build_huffman_tree(map<char, int> chars_count);

unordered_map<char, string> build_encoding_table(min_heap_node* huffman_tree);

void store_codes(unordered_map<char, string>& encoding_table, min_heap_node* huffman_tree, string code, int index);

#endif