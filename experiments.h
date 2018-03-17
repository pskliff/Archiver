

#ifndef KDZ_EXPERIMENTS_H
#define KDZ_EXPERIMENTS_H

#include "Utility.h"


class experiments
{
    Utility ut;
public:
/**
* Writes frequencies and entropies for every file in csvs
*
* @throws runtime_error if file does not exist
*/
    void write_frequency_to_csv(std::string output_path)
    {
        std::cout << "************ Entered  write_frequency_csv   ****************\n";


        // create writer for frequencies
        std::string buf[] = {"name", "char", "freq"};
        Utility::CSVWriter writer(FREQ_COLUMNS_NUM, output_path);
        writer.write_row(buf);

        std::string file_name;
        std::vector<std::pair<string, double>> entropies;
        int sum = 0;

        // go though all test files
        for (int i = 1; i <= 36; ++i)
        {
            std::string file_ind = (i < 10 ? "0" + to_string(i) : to_string(i));
            file_name = "samples/" + file_ind;

            if (access(file_name.c_str(), F_OK) == -1)
                throw std::runtime_error("File does't exist (get_frequency())");
            std::ifstream in(file_name, std::ios::in | std::ios::binary);


            // find char frequencies for given file
            std::map<uchar, int> result;
            char ch_buf;
            sum = 0;
            while (in.get(ch_buf))
            {
                uchar ch = ch_buf;

                // increment frequency for current char
                result[ch] += 1;
                ++sum;
            }
            in.close();


            // count entropy and write freqs into file
            double entropy = 0;
            double freq = 0;
            for (auto c : result)
            {
                freq = c.second * 1.0 / sum;
                entropy -= freq * ut.log_m(result.size(), freq);
                buf[0] = file_ind;
                buf[1] = to_string((int) c.first);
                buf[2] = to_string(freq);
                writer.write_row(buf);
            }
            entropies.emplace_back(file_ind, entropy);
        }

        writer.close();



        // write entropies into file
        std::string ent_buf[] = {"name", "entropy"};
        Utility::CSVWriter ent_writer(ENTROPY_COLUMNS_NUM, "entropy.csv");
        ent_writer.write_row(ent_buf);

        for (auto p : entropies)
        {
            ent_buf[0] = p.first;
            ent_buf[1] = to_string(p.second);
            ent_writer.write_row(ent_buf);
        }

        ent_writer.close();

        std::cout << "************ Exit  write_frequency_csv   ****************\n";
    }




    /**
     * Counts how much time in ns huffman needs to compress and decompress
     * and also how much are the files compressed
     * and writes it in 2 csv files
     */
    void write_huff_to_csv(std::string compress_path, std::string coef_path)
    {
        std::cout << "\n************ Entered  write_huff_csv   ****************\n";
        std::string source_path;
        std::vector<Utility::comp_coef_row> compress_coefs;
        std::vector<Utility::alg_time_row> alg_times;
        double before_comp, after_comp;

        // huffman goes through 36 files
        for (int i = 14; i <= 15; ++i)
        {
            std::cout << "************ Entered  &&&&&&&&& FILE CYCLE &&&&&&&&&&   ****************\n";

            std::string file_ind = (i < 10 ? "0" + to_string(i) : to_string(i));
            source_path = "samples/" + file_ind;

            std::map<uchar, string> huff_codes = ut.create_huffman_codes(source_path);

            // do comp and decomp on every file for 20 times to get mean results
            long64 comp_time = 0, decomp_time = 0;
            for (int j = 0; j < 20; ++j)
            {
                std::cout << "************ Entered  TIMES cycle   ****************\n";
                auto start = chrono::high_resolution_clock::now();
                before_comp = EncDec::encode_file(source_path, source_path + HUFF_ENCODING_EXTENCION, huff_codes);
                auto end = chrono::high_resolution_clock::now();

                comp_time += chrono::duration_cast<chrono::nanoseconds>(end - start).count();

                start = chrono::high_resolution_clock::now();
                after_comp = EncDec::decode_file(source_path + HUFF_ENCODING_EXTENCION, source_path + HUFF_DECODING_EXTENCION);
                end = chrono::high_resolution_clock::now();

                decomp_time += chrono::duration_cast<chrono::nanoseconds>(end - start).count();
            }

            alg_times.emplace_back(Utility::alg_time_row(comp_time * 1.0 / 20, decomp_time * 1.0 / 20, HUFF_TYPE, file_ind));
            compress_coefs.emplace_back(Utility::comp_coef_row(after_comp / before_comp, HUFF_TYPE, file_ind));
        }


        // write times
        std::string time_buf[] = {"type", "name", "comp", "decomp"};
        Utility::CSVWriter time_writer(TIME_COLUMNS_NUM, compress_path);
        time_writer.write_row(time_buf);

        for (auto t : alg_times)
        {
            time_buf[0] = t.type;
            time_buf[1] = t.name;
            time_buf[2] = t.st_comp();
            time_buf[3] = t.st_decomp();
            time_writer.write_row(time_buf);
        }

        time_writer.close();


        // write coefs
        std::string comp_buf[] = {"type", "name", "k"};
        Utility::CSVWriter comp_writer(COMPCOEF_COLUMNS_NUM, coef_path);
        comp_writer.write_row(comp_buf);

        for(auto c : compress_coefs)
        {
            comp_buf[0] = c.type;
            comp_buf[1] = c.name;
            comp_buf[2] = c.st_coef();
            comp_writer.write_row(comp_buf);
        }
        comp_writer.close();

        std::cout << "************ Exit  write_huff_csv   ****************\n";
    }


};


#endif //KDZ_EXPERIMENTS_H
