#include <iostream>
#include <map>
#include <string>
#include <zconf.h>
#include <stdexcept>
#include <fstream>
#include <locale>
#include <codecvt>
#include "HuffmanEncoderDecoder.h"


const string HUFF_ENCODING_EXTENCION = ".haff";
const string HUFF_DECODING_EXTENCION = ".unhaff";


/**
* Gets frequency of chars in file
*
* @throws runtime_error if file does not exist
*/
std::map<uchar, int> get_frequency(std::string path)
{

    if (access(path.c_str(), F_OK) == -1)
    {
        throw std::runtime_error("File does't exist (from get_frequency())");
    }


    std::ifstream f(path, std::ios::in | std::ios::binary);
    std::map<uchar, int> result;
    char ch_buf;
    while (f.get(ch_buf))
    {
        uchar ch = ch_buf;

        // increment frequency for current char
        result[ch] += 1;
    }
    f.close();
    return result;
}


/**
* Uses Huffman algorithm to archive file
*
* @param
* path - encoding file path
*
*/
std::map<uchar, string> create_huffman_codes(std::string path)
{
    auto freqs = get_frequency(path);

    // creates huffman codes
    Huffman* huff = new Huffman();
    for (auto elem : freqs)
        huff->addChance(elem.first, elem.second);
    huff->build();

    return huff->codes;

}


int main()
{
    std::string source_path = "samples/15";
    std::map<uchar, string> codes = create_huffman_codes(source_path);
    huffEncDec::encode_file(source_path, source_path + HUFF_ENCODING_EXTENCION, codes);
    huffEncDec::decode_file(source_path + HUFF_ENCODING_EXTENCION, source_path + HUFF_DECODING_EXTENCION);

    return 0;
}