#include <iostream>
#include <stdlib.h>
#include <vector>

typedef unsigned char uchar;
using namespace std;

class Node
{
public:
    //everything public
    int offs;
    int len;
    uchar ch;

    Node(int o, int l, uchar c) : offs(o), len(l), ch(c)
    {};

    Node()
    {};

};

Node find_biggest_match(vector<uchar>& s, int current_position, int window_size, int lookahead_size)
{
//    std::cout << "************ Entered  find_biggest_match   ****************\n";
    // end position of the buffer
    int buffer_end = min(current_position + lookahead_size, (int)s.size());
    int best_offset = 0;
    int best_length = 0;
    int maxLen = 0;

    // start position of the dictionary
    int start = max(0, current_position - window_size);

    // goes through the dictionary
    for (int i = start; i < current_position; ++i)
    {

        // found elements equal to the first element in buffer
        if (s[i] == s[current_position])
        {

            // continues to compare elements
            int j = i + 1;
            for (int curr_pos = current_position + 1;
                 curr_pos < buffer_end; ++curr_pos)
            {

                if (s[j] != s[curr_pos])
                    break;
                ++j;
            }

            best_length = j - i;

            // if found prefix of bigger length => update length and offset
            if (best_length > maxLen)
            {
                best_offset = current_position - i;
                maxLen = best_length;
            }

        }
    }

//    std::cout << "************ Entered  find_biggest_match   ****************\n";
    return Node(best_offset, maxLen, s[current_position + maxLen]);
}


/**
 * encodes array of chars via lz77 algorithm
 */
void encode_LZ77(vector<uchar>& s, vector<Node>& res, int histBufMax, int prevBufMax)
{
    std::cout << "************ Entered  encode_LZ77   ****************\n";
    // first element will always have a 0 length and 0 offset
    int i = 1;
    res.emplace_back(Node(0, 0, s[0]));


    while (i < s.size())
    {
        Node match = find_biggest_match(s, i, histBufMax, prevBufMax);
        res.emplace_back(match);

        // skip elements that are already seen
        i += match.len + 1;
        if(i % 100000 == 0)
            cout << i << endl;
    }

    std::cout << "************ Exit  encode_LZ77   ****************\n";
}
