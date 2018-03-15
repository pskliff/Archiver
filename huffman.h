#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <zconf.h>
#include <stdexcept>
#include <locale>


typedef unsigned char uchar;
using namespace std;


/**
 * creates codes for bytes with Huffman algorithm
 */
class Huffman
{

    /**
     * Node to keep char and its frequency
     */
    class Node
    {
        friend class Huffman;


        uchar ch;

        int frequency;

        // Left and right child
        Node* left, * right;


        Node(uchar ch, int freq)
        {
            left = right = nullptr;
            this->frequency = freq;
            this->ch = ch;
        }
    };


    /**
     * To compare nodes via frequency
     */
    class Comparator
    {
    public:
        bool operator()(Node* l, Node* r)
        {
            return l->frequency > r->frequency;
        }
    };


    /**
     * fulfills the codes array going down the tree
     */
    void getCodes(Node* node, string str)
    {
        if (!node)
            return;

        if (node->left == nullptr && node->right == nullptr)
            codes[node->ch] = str;

        getCodes(node->left, str + "0");
        getCodes(node->right, str + "1");
    }


public:


    /**
     * builds codes for given bytes
     */
    void build()
    {
        std::cout << "************ Entered  build huffman   ****************\n";
        int n = queue.size();

        Node* left, * right, * parent;


        // while there are still elements to sum
        while (queue.size() != 1)
        {

            // get 2 minimum nodes according the comparator
            left = queue.top();
            queue.pop();

            right = queue.top();
            queue.pop();

            // create a new node => that equals the sum of it's children
            // give it negative index to keep higher priority in queue
            parent = new Node('$', left->frequency + right->frequency);

            parent->left = left;
            parent->right = right;

            queue.push(parent);
        }

        getCodes(parent, "");

        std::cout << "************ Exit  build huffman   ****************\n";

    }


    /**
     * adds new node
     * \param ch byte
     * \param chance frequency
     */
    void addChance(uchar ch, int chance)
    {
        queue.push(new Node(ch, chance));
    }


    string get(uchar i)
    {
        return codes[i];
    }


    priority_queue<Node*, vector<Node*>, Comparator> queue;

    // size of the array
    int n;

    // keeps final codes
    map<uchar, string> codes;
};