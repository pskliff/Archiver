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


class Huffman
{

    class Node
    {
        friend class Huffman;

        uchar ch;

        int frequency;

        // save index to keep codes in the right order
//        int index;

        // Left and right child
        Node* left, * right;


        Node(uchar ch, int freq)
        {
            left = right = nullptr;
            this->frequency = freq;
//            this->index = index;
            this->ch = ch;
        }
    };


    // To compare nodes via frequency and indexes (to follow the task config)
    class Comparator
    {
    public:
        bool operator()(Node* l, Node* r)
        {
            return
//                    l->frequency == r->frequency
//                   ? l->index >= 0 && r->index >= 0
//                     ? l->index < r->index
//                     : l->index > r->index
//                   :
                   l->frequency > r->frequency;
        }
    };


    // fulfills the codes array going down the tree
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


    void build()
    {
        ind = 0;
        int n = queue.size();
//        codes = new string[n];
//        for (int i = 0; i < n; ++i)
//        {
//            codes[]
//        }

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

    }


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

    // variable that helps setting indexes
    int ind = 0;

    // keeps final codes
    map <uchar, string> codes;
};

//class Huffman
//{
//
//    class Node
//    {
//        friend class Huffman;
//
//        wchar_t ch;
//
//        int frequency;
//
//        // save index to keep codes in the right order
////        int index;
//
//        // Left and right child
//        Node* left, * right;
//
//
//        Node(wchar_t ch, int freq)
//        {
//            left = right = nullptr;
//            this->frequency = freq;
////            this->index = index;
//            this->ch = ch;
//        }
//    };
//
//
//    // To compare nodes via frequency and indexes (to follow the task config)
//    class Comparator
//    {
//    public:
//        bool operator()(Node* l, Node* r)
//        {
//            return
////                    l->frequency == r->frequency
////                   ? l->index >= 0 && r->index >= 0
////                     ? l->index < r->index
////                     : l->index > r->index
////                   :
//                    l->frequency > r->frequency;
//        }
//    };
//
//
//    // fulfills the codes array going down the tree
//    void getCodes(Node* node, string str)
//    {
//        if (!node)
//            return;
//
//        if (node->left == nullptr && node->right == nullptr)
//            codes[node->ch] = str;
//
//        getCodes(node->left, str + "0");
//        getCodes(node->right, str + "1");
//    }
//
//
//public:
//
//
//    void build()
//    {
//        ind = 0;
//        int n = queue.size();
////        codes = new string[n];
////        for (int i = 0; i < n; ++i)
////        {
////            codes[]
////        }
//
//        Node* left, * right, * parent;
//
//
//        // while there are still elements to sum
//        while (queue.size() != 1)
//        {
//
//            // get 2 minimum nodes according the comparator
//            left = queue.top();
//            queue.pop();
//
//            right = queue.top();
//            queue.pop();
//
//            // create a new node => that equals the sum of it's children
//            // give it negative index to keep higher priority in queue
//            parent = new Node('$', left->frequency + right->frequency);
//
//            parent->left = left;
//            parent->right = right;
//
//            queue.push(parent);
//        }
//
//        getCodes(parent, "");
//
//    }
//
//
//    void addChance(wchar_t ch, int chance)
//    {
//        queue.push(new Node(ch, chance));
//    }
//
//
//    string get(char i)
//    {
//        return codes[i];
//    }
//
//
//    priority_queue<Node*, vector<Node*>, Comparator> queue;
//
//    // size of the array
//    int n;
//
//    // variable that helps setting indexes
//    int ind = 0;
//
//    // keeps final codes
//    map <wchar_t, string> codes;
//};

//
//#include <iostream>
//#include <map>
//#include <string>
//#include <zconf.h>
//#include <stdexcept>
//#include <fstream>
//#include <locale>
//#include <codecvt>
//#include "HuffmanEncoderDecoder.h"
//
//
//
//
///**
//* Gets map with file chars frequency
//*
//* @param
//* string fileName
//*
//* @return map with key - wchar_t, and value - fileContent.Count(key)
//*
//* @throws runtime_error if file does not exist
//*/
//std::map<wchar_t, int> GetFrequencyMap(std::string fileName)
//{
//
//    // check file existence
//    if (access(fileName.c_str(), F_OK) == -1)
//    {
//        throw std::runtime_error("File does not exist (from GetFrequencyMap())");
//    }
//    std::wifstream f(fileName, std::ios::binary);
//    f.imbue(std::locale(""));
//    std::map<wchar_t, int> result;
//    while (!f.eof())
//    {
//        wchar_t ch = f.get();
//        if (ch != -1)
//            // increase number of occurrences for current char
//            result[ch] += 1;
//    }
//    f.close();
//    return result;
//}
//
//
//
//
//
///**
//* Prepares and archive text file via Haffman algorithm
//*
//*
//* @param
//* string fileName - decoding file
//*
//* @throws invalid_argument, if file name is invalid (do not have suffix ".txt" or if its len <= 4)
//* @throws throw logic_error if file is empty
//*
//*/
//void ArchiveHuffman(std::string fileName)
//{
////    if (fileName.length() <= 4 || fileName.substr(fileName.length()-4, 4) != ".txt") throw invalid_argument("Invalid filename");
//    auto frequencyMap = GetFrequencyMap(fileName);
////    map <wchar_t, string> codes;
////    std::priority_queue<pair<int, std::vector<wchar_t>>, vector<pair<int, std::vector<wchar_t>>>, CompareInts> queue;
//
//    Huffman* huff = new Huffman();
//    for (auto elem : frequencyMap)
//        huff->addChance(elem.first, elem.second);
//
//    //TODO move to class and test how it works for now
////    if (huff->queue.size() == 1) { // if there is only one char type
////        huff->codes[huff->queue.top()->ch] = "0";
////    } else if (huff->queue.size() == 0) { // empty file error
////        throw logic_error("Archiving file is empty");
////    }
//
//    huff->build();
//
////    fileName.erase(fileName.length()-4, 4);
//    huffEncDec::EncodeMapToFile(fileName, fileName+"haff.txt",  huff->codes); // encoding map and file content
//}
//
//
//
//int main()
//{
////    std::map<char, int> freqs = GetFrequencyMap("samples/01");
////    for (auto f : freqs)
////    {
////        std::cout << f.first << " " << f.second << std::endl;
////    }
//    ArchiveHuffman("samples/01");
//    huffEncDec::DecodeFile("samples/01haff.txt", "samples/01res");
//
//    return 0;
//}