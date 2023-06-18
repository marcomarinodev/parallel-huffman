
#include "../includes/huffman.h"

using namespace std;

// print huffman tree
void print_huffman_tree(struct min_heap_node *root, string str)
{
    if (!root)
        return;

    if (root->data != INTERNAL_NODE_SYMBOL)
    {
        print_ascii_html_value(root->data);
        cout << " : " << str << endl;
    }

    print_huffman_tree(root->left, str + "0");
    print_huffman_tree(root->right, str + "1");
}

min_heap_node *build_huffman_tree(map<char, int> chars_count)
{
    struct min_heap_node *left, *right, *top;

    // min heap insertion
    priority_queue<min_heap_node *, vector<min_heap_node *>, compare> min_heap;

    for (auto entry : chars_count)
        min_heap.push(new min_heap_node(entry.first, entry.second));

    while (min_heap.size() != 1)
    {
        // extract two minimum freq items from min heap
        left = min_heap.top();
        min_heap.pop();

        right = min_heap.top();
        min_heap.pop();

        // now let's insert the internal node
        // with the sum of the frequences of both left and right node
        top = new min_heap_node(INTERNAL_NODE_SYMBOL, left->freq + right->freq);

        // link with the least frequent and the second least frequent
        top->left = left;
        top->right = right;

        // add that into the min_heap
        min_heap.push(top);
    }

    return top;
}

unordered_map<char, string> build_encoding_table(min_heap_node *huffman_tree)
{
    unordered_map<char, string> encoding_table;

    store_codes(encoding_table, huffman_tree, "", 0);

    return encoding_table;
}

void store_codes(unordered_map<char, string> &encoding_table, min_heap_node *huffman_tree, string code, int index)
{
    if (huffman_tree == NULL)
        return;
    if (huffman_tree->data != INTERNAL_NODE_SYMBOL)
        encoding_table[huffman_tree->data] = code;

    index++;

    store_codes(encoding_table, huffman_tree->left, code + "0", index);
    store_codes(encoding_table, huffman_tree->right, code + "1", index);
}
