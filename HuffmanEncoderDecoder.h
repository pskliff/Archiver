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


    static uchar ToByte(string byteStr)
    {
        uchar byte = 0;
        for (uchar bit : byteStr)
        {
            byte = (byte << 1) | (bit - '0');
        }
        return byte;
    }


/**
* Decodes map with char and binary string representation
*
* @param
* string filename - archiving file name
*
* @param
* string archiveName - result archive file name
* @param
* map <wchar_t, string> codes - contains char and its binary string representation
*
* @throws runtime_error, if file filename does not exist
*/
    static void EncodeMapToFile(std::string filename, std::string archiveName, std::map<uchar, string> codes)
    {
        if (access(filename.c_str(), 0) != 0)
        { // check if file exists
            throw runtime_error("File does not exist (from DecodeMapToFile())");
        }
//        locale ulocale(locale(),
//        new codecvt_utf8<char>);
        std::ofstream file(archiveName, ios::out);
//        file.imbue(ulocale);


        int cds = codes.size();
        uchar cc = codes.size();
        std::cout<< cc << std::endl;
        int ml = cc;
        file.put(codes.size());
        for (auto ch : codes)
        { // encoding map of chars and binary strings
            file.put(ch.first);
            for (uchar c : ch.second)
                file.put(c); // right after char putting binary string
            file.put('\n'); // splits each char
        }


        string buffer = ""; // helping string kind of a buffer, that will be collecting binary encoded code
        std::ifstream in(filename, std::ios::in | std::ios::binary);
//        in.imbue(locale(""));
        vector<uchar> fileContent; // it will collect encoded bytes
        char ch_buf;
        while (in.get(ch_buf))
        {
            int wh = ch_buf;
            uchar ch = ch_buf;

//            std::cout<< wh << std::endl;
//            std::cout<< ch << std::endl<< std::endl;

//            if (ch == -1)
//                break;
            buffer += codes[ch];
            while (buffer.length() > 8)
            { // while it contains more than 1 byte
                string byteStr = buffer.substr(0, 8); // getting byte
                fileContent.push_back(ToByte(byteStr)); // appending to vector with new file content (encoded text)
                buffer.erase(0, 8); // erase is quite effective because of short string length
            }
        }

        const int len = buffer.length(); // length less than 8, this is guaranteed
        if (len == 0)
        { // getting number of bits to fill last byte
            file.put(0); // no need to add more bits
        } else
        {
            for (int i = 0; i < 8 - len; ++i)
            { // adding senseless bits
                buffer += "0";
            }
            fileContent.push_back(ToByte(buffer));
            file.put(8 - len);
        }


        for (int i = 0; i < fileContent.size(); ++i)
        {
            file.put(fileContent[i]);
        }
//        file.write(fileContent.data(), fileContent.size()); // ready to write

        fileContent.clear();
        file.close();
        in.close();
    }


    class CompareInts
    {
    public:
        int operator()(const pair<int, std::vector<uchar>>& p1, const pair<int, std::vector<uchar>>& p2)
        {
            return p1.first >= p2.first;
        }
    };


    struct CompareStringByLen
    {
        bool operator()(const std::string& a, const std::string& b) const
        {
            if (a.length() == b.length()) return a > b;
            return a.length() < b.length();
        }
    };

    string ToString(uchar byte) {
        string res = "";
        for(int i = 128; i > 0; i/=2)
        {
            uchar b = byte / 128;
            res += std::to_string(b);
            byte = (byte << 1) % 256;
        }
        return res;
    }

/**
* Decodes archived file
*
* @param
* string archiveName - archived file name
*
* @param
* string fileName - result encoded file name
*
* @throws logic_error, if archive format is incorrect
*
*/
    void DecodeFile(string archiveName, string fileName)
    {
        ifstream in(archiveName, std::ios::in);
//        in.imbue(locale(""));
        map<string, uchar, CompareStringByLen> charMap;
        if (in.eof())
        {
            throw logic_error("Decoding file is empty");
        }


//        int len = in.get();
        uchar len = in.get();
//        len = len == '\0' ? 25

         int ml = len == '\0' ? 256 : len;
        uchar kk = uchar(ml);

        uchar value;
        char value_buf;

        uchar counter = 0;

        int ctr = counter;
        std::cout<<!in.eof() << std::endl;
        std::cout<<(counter < len) << std::endl;

        while (in.get(value_buf) && ctr <  ml)
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
                    {
                        charMap[key] = value;
                    } else
                    {
                        throw logic_error("Decoding file format is incorrect");
                    }
                    ctr += 1;
                    break;
                }
                key += ch;
            }
        }

//        in.clear();
        if (in.eof())
        {
            throw logic_error("Decoding file is invalid");
        }
        uchar numOfCharsAtEnd = value_buf; // потому что в предыдущем while он уже прочитан

        int numberend = numOfCharsAtEnd;

        string fileContent = "";
        char buf;
        while (in.get(buf))
        {
            uchar ch = buf;
            if (ch != -1)
            {
                fileContent += ToString(ch);
            }
        }



        fileContent.erase(fileContent.length() - numOfCharsAtEnd, fileContent.length());

//        std::locale::global(std::locale(""));
        ofstream file(fileName, ios::out);

        unsigned long pointer = 0;
        while (pointer < fileContent.length())
        {
            bool found = false;
            for (auto symb : charMap)
            {
                if (!fileContent.compare(pointer, symb.first.size(), symb.first))
                {
                    pointer += symb.first.size();
                    file.put(symb.second);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                throw logic_error("Invalid decoding archive");
            }
        }
        file.close();
    }

}
