#include "../gl1_scaler_ana.h"

R__LOAD_LIBRARY(../libgl1_scaler_ana.so)

int main_51195_Chris_GL1_GL1P_rate()
{
    // string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris/ntuple_file";
    // string input_filename = "ntuple_run51195_merged.root";
    // string output_data_filename = "ntuple_run51195_merged_raw_rate.root";
    // string output_directory = input_directory + "/quick_check";

    string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform_GL1P";
    string input_filename = "ntuple_run51195_merged.root";
    string output_data_filename = "ntuple_run51195_merged_rawlive_rate.root";
    string output_directory = input_directory;

    vector<pair<int,int>> range_t_V = {
        {786, 858},
        {871, 902},
        {918, 953},
        {969, 1007},
        {1020, 1070},
        {1082, 1197},

        {1210, 1239},
        {1257, 1290},
        {1305, 1339},
        {1354, 1391},
        {1404, 1456},
        {1469, 1582}
    };

    vector<pair<int,int>> range_t_H = {
        {6, 50},
        {64,96},
        {113,146},
        {162,195},
        {213,262},
        {277,389},

        {404,433},
        {449,479},
        {495, 531},
        {548, 581},
        {597, 646},
        {659, 774}
    };


    gl1_scaler_ana * MBDNS_ana_ffft = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, false, false, true
    );
    MBDNS_ana_ffft->PrepareRate();
    MBDNS_ana_ffft->OutputRawRate(Form("%s/%s", output_directory.c_str(), output_data_filename.c_str()));

    return 0;
}