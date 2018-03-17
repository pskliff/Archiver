
// КДЗ по дисциплине Алгоритмы и структуры данных 2017-2018 уч.год
// Шакин Кирилл Петрович, группа БПИ-163, дата (19.03.2018)
// CLion c++11, состав проекта (файлы *.cpp и *.h)
// Что сделано, а что нет

#include <iostream>
#include <map>
#include <string>
#include <zconf.h>
#include <stdexcept>
#include <fstream>
#include <locale>
#include <codecvt>
//#include "EncoderDecoder.h"
#include "experiments.h"






int main()
{
    std::string source_path = "samples/14";


//    Utility ut;
//    std::map<uchar, string> huff_codes = ut.create_huffman_codes(source_path);
//    EncDec::encode_file(source_path, source_path + HUFF_ENCODING_EXTENCION, huff_codes);
//    EncDec::decode_file(source_path + HUFF_ENCODING_EXTENCION, source_path + HUFF_DECODING_EXTENCION);


//    std::map<uchar, string> fano_codes = create_fano_codes(source_path);

//    EncDec::encode_file(source_path, source_path + FANO_ENCODING_EXTENCION, fano_codes);
//    EncDec::decode_file(source_path + FANO_ENCODING_EXTENCION, source_path + FANO_DECODING_EXTENCION);


//    std::vector<Node> lz77_codes = create_lz77_codes(source_path, HIST_BUF_MAX_16, PREV_BUF_MAX_4);
//    EncDec::write_lz77(lz77_codes, source_path + LZ7720_ENCODING_EXTENCION);
//    EncDec::decode_lz77(source_path + LZ7720_ENCODING_EXTENCION, source_path + LZ7720_DECODING_EXTENCION);


    experiments exp;
//    exp.write_frequency_to_csv("freqs.csv");
    exp.write_huff_to_csv("compress.csv", "coef.csv");


    return 0;
}