#include <iostream>
#include <map>
#include <string>
#include <zconf.h>
#include <stdexcept>
#include <fstream>
#include <locale>
#include <codecvt>
#include "huffman.h"



namespace huffEncDec
{


/**
 * converts string of 0 and 1 to a byte
 */
    static uchar to_byte(string byteStr)
    {
        uchar byte = 0;
        for (uchar bit : byteStr)
            byte = (byte << 1) | (bit - '0');
        return byte;
    }


/**
 * converts byte to string of 0 and 1
 */
    string to_string(uchar byte)
    {
        string res = "";
        for (int i = 128; i > 0; i /= 2)
        {
            uchar b = byte / 128;
            res += std::to_string(b);
            byte = (byte << 1) % 256;
        }
        return res;
    }


/**
* Encodes file to the compressed binary file
*
* @throws runtime_error, if source file doesn't exist
*/
    static void encode_file(std::string source_path, std::string destination_path, std::map<uchar, string> codes)
    {
        if (access(source_path.c_str(), 0) != 0)
            throw runtime_error("File does not exist (encode_file)");

        std::ofstream file(destination_path, ios::out);

        file.put(codes.size());

        // encoding map
        for (auto ch : codes)
        {
            file.put(ch.first);
            for (uchar c : ch.second)
                file.put(c);
            file.put('\n');     // splits each map line
        }


        // collects binary code as string of 0 and 1
        string proxy = "";
        std::ifstream in(source_path, std::ios::in | std::ios::binary);

        // collects encoded bytes
        vector<uchar> encoded_bytes;
        char ch_buf;
        while (in.get(ch_buf))
        {
            uchar ch = ch_buf;
            proxy += codes[ch];

            // while in proxy there is > 1 byte
            while (proxy.length() > 8)
            {
                // crop next byte as string
                string byteStr = proxy.substr(0, 8);

                // append to the final vector with encoded bytes
                encoded_bytes.push_back(to_byte(byteStr));
                proxy.erase(0, 8);
            }
        }

        // number of bits left in buffer
        const int len = proxy.length();
        if (len == 0)
        {
            // if no bits available
            file.put(0);
        } else
        {

            // if some bits are left => adding sensless bits
            for (int i = 0; i < 8 - len; ++i)
                proxy += "0";
            encoded_bytes.push_back(to_byte(proxy));
            file.put(8 - len);
        }


        // write into the destination file
        for (int i = 0; i < encoded_bytes.size(); ++i)
            file.put(encoded_bytes[i]);

        encoded_bytes.clear();
        file.close();
        in.close();
    }


    struct string_len_comparator
    {
        bool operator()(const std::string& a, const std::string& b) const
        {
            if (a.length() == b.length()) return a > b;
            return a.length() < b.length();
        }
    };


/**
* Decodes compressed file
*
* @throws logic_error, if compressed file format is incorrect
*
*/
    void decode_file(string source_path, string destination_path)
    {
        ifstream in(source_path, std::ios::in);
        map<string, uchar, string_len_comparator> byte_map;
        if (in.eof())
            throw logic_error("Decoding file is empty");


        // if map consists of 256 chars
        uchar len = in.get();
        int ml = len == '\0' ? 256 : len;

        uchar value;
        char value_buf;

        int ctr = 0;


        // reads map from compressed file
        while (in.get(value_buf) && ctr < ml)
        {
            value = value_buf;
            string key = "";
            char ch_buf;
            while (in.get(ch_buf))
            {
                uchar ch = ch_buf;
                if (ch == '\n')
                {
                    if (key.length() > 0)
                        byte_map[key] = value;
                    else
                        throw logic_error("Decoding file format is incorrect");
                    ctr += 1;
                    break;
                }
                key += ch;
            }
        }


        if (in.eof())
            throw logic_error("Decoding file is invalid");


        // number of meaningfull bits in the final byte
        uchar num_of_bits = value_buf;

        string source_content = "";
        char buf;

        // read contents from source file and convert it to string with 0 and 1
        while (in.get(buf))
        {
            uchar ch = buf;

            source_content += to_string(ch);
        }
        source_content.erase(source_content.length() - num_of_bits, source_content.length());


        ofstream file(destination_path, ios::out);

        // go through the source file content and convert 0s and 1s to decompressed chars
        unsigned long ind = 0;
        while (ind < source_content.length())
        {
            bool found = false;
            for (auto byte : byte_map)
            {
                if (!source_content.compare(ind, byte.first.size(), byte.first))
                {
                    ind += byte.first.size();
                    file.put(byte.second);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                throw logic_error("Invalid decoding file");
            }
        }
        file.close();
    }

}
