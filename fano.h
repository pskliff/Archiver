#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <map>


typedef unsigned char uchar;
using namespace std;


class ShannonFano
{

    class Node
    {
        friend class ShannonFano;


        uchar ch;
        int frequency;

        // save index to keep codes in the right order
        int index;


        Node(uchar ch, int freq, int index)
        {
            this->ch = ch;
            this->frequency = freq;
            this->index = index;
        }


        Node()
        {}
    };


public:

    ShannonFano(int n)
    {
        this->n = n;
        nodes = new Node[n];
        str_codes = new string[n];
    }


    ~ShannonFano()
    {
        delete[] str_codes;
        delete[] nodes;
    }



    void make_final_codes()
    {
        for (int i = 0; i < n; ++i)
            codes[nodes[i].ch] = str_codes[i];
    }


    void addChance(uchar ch, int chance)
    {
        nodes[ind] = *(new Node(ch, chance, ind));
        ++ind;
    }


    string get(int i)
    {
        return str_codes[i];
    }


    void encode(int l, int r)
    {
        int i, split_ind;
        int in_sum;
        int full;

        if (l == r)
            return;
        else if (r - l == 1)
        {
            //  if there are only 2 elements
            str_codes[nodes[l].index] += '0';
            str_codes[nodes[r].index] += '1';
        } else
        {
            // sum of all frequencies
            full = 0;
            for (i = l; i <= r; ++i)
            {
                full += nodes[i].frequency;
            }

            in_sum = 0;
            split_ind = -1;
            for (i = l; i <= r; ++i)
            {
                // minimize difference between left and right part
                if (abs(2 * (in_sum + nodes[i].frequency) - full) < abs(2 * in_sum - full))
                    str_codes[nodes[i].index] += '0';
                else
                {
                    str_codes[nodes[i].index] += '1';
                    if (split_ind < 0) split_ind = i;
                }
                in_sum += nodes[i].frequency;
            }

            if (split_ind < 0) split_ind = l + 1;

            encode(l, split_ind - 1);
            encode(split_ind, r);
        }

        make_final_codes();
    }



    void build()
    {
        encode(0, n - 1);
    }


    // size of the array
    int n;

    // variable that helps setting indexes
    int ind = 0;

    // buffer codes
    string* str_codes;

    // keeps final codes
    map<uchar, string> codes;

    Node* nodes;

};
