
#ifndef KDZ_UTILITY_H
#define KDZ_UTILITY_H

#include "EncoderDecoder.h"
#include <cmath>


class Utility
{
    const int FREQ_COLUMNS_NUM = 3;
    const int ENTROPY_COLUMNS_NUM = 2;


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
        CSVWriter writer(FREQ_COLUMNS_NUM, output_path);
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
                entropy -= freq * log_m(result.size(), freq);
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
        CSVWriter ent_writer(ENTROPY_COLUMNS_NUM, "entropy.csv");
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
};


#endif //KDZ_UTILITY_H
