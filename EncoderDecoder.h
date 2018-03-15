#include <locale>
#include <codecvt>
#include "huffman.h"
#include "fano.h"
#include "lz77.h"


namespace EncDec
{

    // format: all_size.size.offssize.offs.lensize.len.char
//    void write_lz77(std::vector<Node>& v, std::string destination_path)
//    {
//        std::ofstream fout(destination_path, ios::out);
////        std::fstream fout;
////        fout.open(destination_path, std::ios::out);
//        int all_size = 0, offset_size = 0, len_size = 0;
////        fout << v.size() << "\n";
//
//
//        std::string all_str = std::to_string(v.size());
//        all_size = all_str.length();
//        char const *pchar = all_str.c_str();
//
//        fout.put((uchar)all_size);
//        for (int i = 0; i < all_size; ++i)
//        {
//            uchar ch = pchar[i];
//            fout.put(ch);
//        }
//
//
//        for (int i = 0; i < v.size(); i++)
//        {
//            std::string offs_str = std::to_string(v[i].offs);
//            offset_size = offs_str.length();
//            char const *offs_char = offs_str.c_str();
//
//            fout.put((uchar)offset_size);
//            for (int j = 0; j < offset_size; ++j)
//                fout.put(offs_char[j]);
//
//
//
//            std::string len_str = std::to_string(v[i].len);
//            len_size = len_str.length();
//            char const *len_char = len_str.c_str();
//
//            fout.put((uchar)len_size);
//            for (int j = 0; j < len_size; ++j)
//                fout.put(len_char[j]);
//
//
//
//            fout.put(v[i].ch);
////            fout << v[i].offs<< v[i].len << v[i].ch;
//        }
//
//        fout.close();
//
//    }


    // allsize.div.mod.ch.div.mod.ch.......allsize times
    void write_lz77(std::vector<Node>& v, std::string destination_path)
    {
        std::ofstream fout(destination_path, ios::out);
//        std::fstream fout;
//        fout.open(destination_path, std::ios::out);
        uchar div = 0, mod = 0;
//        fout << v.size() << "\n";


//        div = v.size() / 256;
//        mod = v.size() % 256;
//        fout.put(div);
//        fout.put(mod);
        int sz = v.size();
        int szof = sizeof(sz);

        fout.write(reinterpret_cast<const char *>(&sz), sizeof(sz));
        for (int i = 0; i < v.size(); i++)
        {
            div = v[i].offs / 256;
            mod = v[i].offs % 256;
            fout.put(div);
            fout.put(mod);

            div = v[i].len / 256;
            mod = v[i].len % 256;
            fout.put(div);
            fout.put(mod);

            fout.put(v[i].ch);
//            fout << v[i].offs<< v[i].len << v[i].ch;
        }

        fout.close();

    }


    vector<uchar> decLZ77(vector<Node> arr, int n)
    {
        vector<uchar> res;
        for (int i = 0; i < n; i++)
        {
            const Node& node = arr[i];
            int ind = res.size() - node.offs;

            // add offset symbol len times
            for (int j = 0; j < node.len; ++j)
                res.emplace_back(res[ind + j]);

            // add next symbol
            res.emplace_back(node.ch);
        }
        return res;
    }

    void decode_lz77(std::string source_path, std::string destination_path)
    {
        if (access(source_path.c_str(), F_OK) == -1)
            throw std::runtime_error("File does't exist (decode_lz77)");


        std::ifstream in(source_path, std::ios::in | std::ios::binary);
        int div = 0, mod = 0, n = 0, offset, len;


        in.read(reinterpret_cast<char *>(&n), sizeof(n));
//        div = in.get();
//        mod = in.get();
//        n = div * 256 + mod;
        uchar ch;
        vector<Node> nodes;
        for (int i = 0; i < n; ++i)
        {
            div = in.get();
            mod = in.get();
            offset = div * 256 + mod;

            div = in.get();
            mod = in.get();
            len = div * 256 + mod;

            ch = in.get();

            nodes.emplace_back(Node(offset, len, ch));
        }
        in.close();

        vector<uchar> result = decLZ77(nodes, n);


        std::ofstream fout(destination_path, ios::out);
        for (int i = 0; i < result.size(); ++i)
        {
            fout.put(result[i]);
        }

        fout.close();
    }


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
