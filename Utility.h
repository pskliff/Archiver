
#ifndef KDZ_UTILITY_H
#define KDZ_UTILITY_H

#include "EncoderDecoder.h"
#include <cmath>
#include <chrono>

typedef long long long64;

const int FREQ_COLUMNS_NUM = 3;
const int ENTROPY_COLUMNS_NUM = 2;
const int TIME_COLUMNS_NUM = 4;
const int COMPCOEF_COLUMNS_NUM = 3;
const string HUFF_ENCODING_EXTENCION = ".haff";
const string HUFF_DECODING_EXTENCION = ".unhaff";
const string FANO_ENCODING_EXTENCION = ".shan";
const string FANO_DECODING_EXTENCION = ".unshan";
const string LZ775_ENCODING_EXTENCION = ".lz775";
const string LZ775_DECODING_EXTENCION = ".unlz775";
const string LZ7710_ENCODING_EXTENCION = ".lz7710";
const string LZ7710_DECODING_EXTENCION = ".unlz7710";
const string LZ7720_ENCODING_EXTENCION = ".lz7720";
const string LZ7720_DECODING_EXTENCION = ".unlz7720";
const int HIST_BUF_MAX_4 = 1024 * 4;
const int PREV_BUF_MAX_1 = 1024;
const int HIST_BUF_MAX_8 = 1024 * 8;
const int PREV_BUF_MAX_2 = 1024 * 2;
const int HIST_BUF_MAX_16 = 1024 * 16;
const int PREV_BUF_MAX_4 = 1024 * 4;
const string HUFF_TYPE ="huff";
const string FANO_TYPE ="fano";
const string LZ77_TYPE ="lz77";


class Utility
{
public:



    /**
     * Provides interface to write data to csv
     */
    class CSVWriter
    {
    private:
        std::string output_path;
        std::string sep = ",";
        std::ofstream fout;
        const int n;

    public:
        CSVWriter(int len, std::string path) : n(len), fout(path, ios::out), output_path(path)
        {};


        /**
         * writes a new row to csv file
         * \param arr
         */
        void write_row(std::string arr[])
        {
            for (int i = 0; i < n - 1; ++i)
                fout << arr[i] << sep;
            fout << arr[n - 1] << std::endl;
        }


        /**
         * closes output stream
         */
        void close()
        {
            fout.close();
        }
    };


    /**
     * founds logarithm with custom base
     */
    double log_m(double base, double x)
    {
        return (log(x) / log(base));
    }



/**
* Gets frequency of chars in file
*
* @throws runtime_error if file does not exist
*/
    std::map<uchar, int> get_frequency(std::string path)
    {
        std::cout << "************ Entered  get_frequency   ****************\n";
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

        std::cout << "************ Exit  get_frequency   ****************\n";
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

        std::cout << "************ Entered  create_huffman_codes   ****************\n";
        auto freqs = get_frequency(path);

        // creates huffman codes
        Huffman* huff = new Huffman();
        for (auto elem : freqs)
            huff->addChance(elem.first, elem.second);
        huff->build();


        std::cout << "************ Exit  create_haffman_codes   ****************\n";
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
        std::cout << "************ Entered  create_fano_codes   ****************\n";
        auto freqs = get_frequency(path);

        // create and sort a vector of frequencies
        std::vector<std::pair<uchar, int>> freq_vec;
        for (auto iterator = freqs.begin(); iterator != freqs.end(); ++iterator)
            freq_vec.emplace_back(iterator->first, iterator->second);
        auto cmp = [](std::pair<uchar, int> const& a, std::pair<uchar, int> const& b)
        {
            return a.second != b.second ? a.second > b.second : a.first > b.first;
        };
        std::sort(freq_vec.begin(), freq_vec.end(), cmp);

        ShannonFano* fano = new ShannonFano(freq_vec.size());
        for (auto elem : freq_vec)
            fano->addChance(elem.first, elem.second);
        fano->build();


        std::cout << "************ Exit  create_fano_codes   ****************\n";
        return fano->codes;
    }


/**
 * Uses lz77 algorithm to archive file to array of nodes
 */
    std::vector<Node> create_lz77_codes(std::string path, int histBufMax, int prevBufMax)
    {
        std::cout << "************ Entered  create_lz77_codes   **************** " << histBufMax << " " << prevBufMax
                  << "\n";
        if (access(path.c_str(), F_OK) == -1)
            throw std::runtime_error("File does't exist (create lz77)");


        std::ifstream f(path, std::ios::in | std::ios::binary);
        std::vector<uchar> bytes;
        std::vector<Node> result;
        char ch_buf;
        while (f.get(ch_buf))
        {
            uchar ch = ch_buf;

            // increment frequency for current char
            bytes.emplace_back(ch);
        }
        f.close();

        encode_LZ77(bytes, result, histBufMax, prevBufMax);

        std::cout << "************ Exit  create_lz77_codes   ****************\n";
        return result;
    }


    /**
     * represents compress_coef.csv columns
     */
    class comp_coef_row
    {
    public:
        double coef;
        std::string type;
        std::string name;


        comp_coef_row(double c, std::string t, std::string n) : coef(c), type(t), name(n)
        {};


        std::string st_coef()
        {
            return to_string(coef);
        }
    };

    /**
     * represents comress_uncompress_time.csv columns
     */
    class alg_time_row
    {
    public:
        double comp;
        double decomp;
        std::string type;
        std::string name;


        alg_time_row(double c, double u, std::string t, std::string n) : comp(c), decomp(u), type(t), name(n)
        {};


        std::string st_comp()
        {
            return to_string(comp);
        }

        std::string st_decomp()
        {
            return to_string(decomp);
        }
    };

};



#endif //KDZ_UTILITY_H
