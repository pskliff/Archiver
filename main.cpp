#include <iostream>
#include <map>
#include <string>
#include <zconf.h>
#include <stdexcept>
#include <fstream>
#include <locale>
#include <codecvt>
#include "EncoderDecoder.h"


const string HUFF_ENCODING_EXTENCION = ".haff";
const string HUFF_DECODING_EXTENCION = ".unhaff";
const string FANO_ENCODING_EXTENCION = ".shan";
const string FANO_DECODING_EXTENCION = ".unshan";


/**
* Gets frequency of chars in file
*
* @throws runtime_error if file does not exist
*/
std::map<uchar, int> get_frequency(std::string path)
{

    if (access(path.c_str(), F_OK) == -1)
        throw std::runtime_error("File does't exist (get_frequency())");


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


/**
* Uses ShannonFano algorithm to archive file
*
* @param
* path - encoding file path
*
*/
std::map<uchar, string> create_fano_codes(std::string path)
{
    auto freqs = get_frequency(path);
    std::vector<std::pair<uchar, int>> freq_vec;
    for (auto iterator = freqs.begin(); iterator != freqs.end(); ++iterator)
        freq_vec.emplace_back(iterator->first, iterator->second);

    auto cmp = [](std::pair<uchar, int> const & a, std::pair<uchar, int> const & b)
    {
        return a.second != b.second ?  a.second > b.second : a.first > b.first;
    };
    std::sort(freq_vec.begin(), freq_vec.end(), cmp);

    ShannonFano* fano = new ShannonFano(freq_vec.size());
    for (auto elem : freq_vec)
        fano->addChance(elem.first, elem.second);
    fano->build();

    return fano->codes;

}


int main()
{
    std::string source_path = "samples/14";
    std::map<uchar, string> huff_codes = create_huffman_codes(source_path);
    std::map<uchar, string> fano_codes = create_fano_codes(source_path);
    EncDec::encode_file(source_path, source_path + HUFF_ENCODING_EXTENCION, huff_codes);
    EncDec::decode_file(source_path + HUFF_ENCODING_EXTENCION, source_path + HUFF_DECODING_EXTENCION);
    EncDec::encode_file(source_path, source_path + FANO_ENCODING_EXTENCION, fano_codes);
    EncDec::decode_file(source_path + FANO_ENCODING_EXTENCION, source_path + FANO_DECODING_EXTENCION);

    return 0;
}