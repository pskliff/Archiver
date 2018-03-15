#include <iostream>
#include <map>
#include <string>
#include <zconf.h>
#include <stdexcept>
#include <fstream>
#include <locale>
#include <codecvt>
#include "HuffmanEncoderDecoder.h"




/**
* Gets map with file chars frequency
*
* @param
* string fileName
*
* @return map with key - wchar_t, and value - fileContent.Count(key)
*
* @throws runtime_error if file does not exist
*/
std::map<uchar, int> GetFrequencyMap(std::string fileName)
{

    // check file existence
    if (access(fileName.c_str(), F_OK) == -1)
    {
        throw std::runtime_error("File does not exist (from GetFrequencyMap())");
    }
    std::ifstream f(fileName, std::ios::in | std::ios::binary);
//    f.imbue(std::locale(""));
    std::map<uchar, int> result;
    char ch_buf;
    while (f.get(ch_buf))
    {
        uchar ch = ch_buf;
//        std::cout<< ch << std::endl;
        if (ch != -1)
            // increase number of occurrences for current char
            result[ch] += 1;
    }
    f.close();
    return result;
}





/**
* Prepares and archive text file via Haffman algorithm
*
*
* @param
* string fileName - decoding file
*
* @throws invalid_argument, if file name is invalid (do not have suffix ".txt" or if its len <= 4)
* @throws throw logic_error if file is empty
*
*/
void ArchiveHuffman(std::string fileName)
{
//    if (fileName.length() <= 4 || fileName.substr(fileName.length()-4, 4) != ".txt") throw invalid_argument("Invalid filename");
    auto frequencyMap = GetFrequencyMap(fileName);
//    map <wchar_t, string> codes;
//    std::priority_queue<pair<int, std::vector<wchar_t>>, vector<pair<int, std::vector<wchar_t>>>, CompareInts> queue;

    Huffman* huff = new Huffman();
    for (auto elem : frequencyMap)
        huff->addChance(elem.first, elem.second);

    //TODO move to class and test how it works for now
//    if (huff->queue.size() == 1) { // if there is only one char type
//        huff->codes[huff->queue.top()->ch] = "0";
//    } else if (huff->queue.size() == 0) { // empty file error
//        throw logic_error("Archiving file is empty");
//    }

    huff->build();

//    fileName.erase(fileName.length()-4, 4);
    huffEncDec::EncodeMapToFile(fileName, fileName+"haff.txt",  huff->codes); // encoding map and file content
}

//TODO:ios::binary and remove locale

string ToString(uchar byte) {
    string res = "";
    for(int i = 128; i > 0; i/=2)
    {
        res += std::to_string(byte / i);
        byte = byte << 1;
    }
    return res;
}

int main()
{
//    std::map<char, int> freqs = GetFrequencyMap("samples/01");
//    for (auto f : freqs)
//    {
//        std::cout << f.first << " " << f.second << std::endl;
//    }

    int k = 255;
    uchar ch = k;
//    k = 254;
    ch = (uchar)k;
    int m = ch;

    string res = ToString((uchar)'t');
    ArchiveHuffman("samples/15");
    huffEncDec::DecodeFile("samples/15haff.txt", "samples/15res");

    return 0;
}