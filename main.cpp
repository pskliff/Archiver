
// КДЗ по дисциплине Алгоритмы и структуры данных 2017-2018 уч.год
// Шакин Кирилл Петрович, группа БПИ-163, дата (19.03.2018)
// CLion c++11, состав проекта: main.cpp, huffman.h, fano.h, lz77.h, 
// EncoderDecoder.h, Utility.h, experiments.h
// Сделано: Huffman, ShennonFano, LZ77(5,10,20) + замеры времени

#include <iostream>
#include <map>
#include <string>
//#include <zconf.h>
#include <stdexcept>
#include <fstream>
#include <locale>
#include <codecvt>
//#include "EncoderDecoder.h"
#include "experiments.h"



int main(int argc, char** argv)
{
//    std::string source_path = "samples/07";

    std::string source_path = "";
    std::string alg_type = "";
    Utility ut;
    if (argc == 1)
    {
        throw logic_error("no parameters given");
    } else if (argc == 3)
    {
        alg_type = argv[1];
        source_path = argv[2];
        if (alg_type == "huff")
        {
            std::map<uchar, string> huff_codes = ut.create_huffman_codes(source_path);
            EncDec::encode_file(source_path, source_path + HUFF_ENCODING_EXTENCION, huff_codes);
            EncDec::decode_file(source_path + HUFF_ENCODING_EXTENCION, source_path + HUFF_DECODING_EXTENCION);
            cout << " \n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ DONEDONEDONEDONEDONEDONE ################################\n";
        } else if (alg_type == "fano")
        {
            std::map<uchar, string> fano_codes = ut.create_fano_codes(source_path);

            EncDec::encode_file(source_path, source_path + FANO_ENCODING_EXTENCION, fano_codes);
            EncDec::decode_file(source_path + FANO_ENCODING_EXTENCION, source_path + FANO_DECODING_EXTENCION);
            cout << " \n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ DONEDONEDONEDONEDONEDONE ################################\n";
        } else if (alg_type == "lz775")
        {
            double before_comp;
            std::vector<Node> lz77_codes = ut.create_lz77_codes(source_path, HIST_BUF_MAX_4, PREV_BUF_MAX_1,
                                                                before_comp);
            EncDec::write_lz77(lz77_codes, source_path + LZ775_ENCODING_EXTENCION);
            EncDec::decode_lz77(source_path + LZ775_ENCODING_EXTENCION, source_path + LZ775_DECODING_EXTENCION);
            cout << " \n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ DONEDONEDONEDONEDONEDONE ################################\n";
        } else if (alg_type == "lz7710")
        {
            double before_comp;
            std::vector<Node> lz77_codes = ut.create_lz77_codes(source_path, HIST_BUF_MAX_8, PREV_BUF_MAX_2,
                                                                before_comp);
            EncDec::write_lz77(lz77_codes, source_path + LZ7710_ENCODING_EXTENCION);
            EncDec::decode_lz77(source_path + LZ7710_ENCODING_EXTENCION, source_path + LZ7710_DECODING_EXTENCION);
            cout << " \n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ DONEDONEDONEDONEDONEDONE ################################\n";
        } else if (alg_type == "lz7720")
        {
            double before_comp;
            std::vector<Node> lz77_codes = ut.create_lz77_codes(source_path, HIST_BUF_MAX_16, PREV_BUF_MAX_4,
                                                                before_comp);
            EncDec::write_lz77(lz77_codes, source_path + LZ7720_ENCODING_EXTENCION);
            EncDec::decode_lz77(source_path + LZ7720_ENCODING_EXTENCION, source_path + LZ7720_DECODING_EXTENCION);
            cout << " \n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ DONEDONEDONEDONEDONEDONE ################################\n";
        }
    }
    



//    experiments exp;
//   exp.write_frequency_to_csv("freqs.csv");
//   exp.write_lz77_to_csv("compress_lz7720.csv", "coef_lz7720.csv", HIST_BUF_MAX_16, PREV_BUF_MAX_4, LZ7720_ENCODING_EXTENCION,LZ7720_DECODING_EXTENCION, LZ7720_TYPE);
//   exp.write_lz77_to_csv("compress_lz7710.csv", "coef_lz7710.csv", HIST_BUF_MAX_8, PREV_BUF_MAX_2, LZ7710_ENCODING_EXTENCION,LZ7710_DECODING_EXTENCION, LZ7710_TYPE);
//   exp.write_lz77_to_csv("compress_lz775.csv", "coef_lz775.csv", HIST_BUF_MAX_4, PREV_BUF_MAX_1, LZ775_ENCODING_EXTENCION,LZ775_DECODING_EXTENCION, LZ775_TYPE);
//   exp.write_huff_to_csv("compress_huff.csv", "coef_huff.csv");
//   exp.write_fano_to_csv("compress_fano.csv", "coef_fano.csv");



return 0;
}