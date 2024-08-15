#include "gl1_scaler_ana.h"

R__LOAD_LIBRARY(libgl1_scaler_ana.so)

int main_51195_selfgen()
{
    string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/Selfgen_GL1/ntuple_file";
    string input_filename = "ntuple_run51195_merged.root";
    string output_data_filename = "ntuple_run51195_merged_raw_rate.root";
    string output_directory = input_directory + "/quick_check";


    vector<pair<int,int>> range_t_V = {
        {},
        {},
        {},
        {},
        {},
        {},

        {},
        {},
        {},
        {},
        {},
        {}
    };

    vector<pair<int,int>> range_t_H = {
        {},
        {},
        {},
        {},
        {},
        {},

        {},
        {},
        {},
        {},
        {},
        {}
    };

    gl1_scaler_ana * MBDNS_ana = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H
    );
    // MBDNS_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDNS_ana->PrepareRate();
    MBDNS_ana->OutputRawRate((input_directory + "/" + output_data_filename).c_str());
    // MBDNS_ana->ImportCADReadings("/sphenix/tg/tg01/coldqcd/cwshih/test/test_run_48029/CAD_reading/plots/CAD_reading_48029.csv");
    bool only_raw_rate_tag = true;
    pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE = MBDNS_ana->CombineMacro("MBDNS", only_raw_rate_tag);
    
    return 0;
}