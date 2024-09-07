#include "gl1_scaler_ana.h"

R__LOAD_LIBRARY(libgl1_scaler_ana.so)

int main_48029_Vincent()
{
    string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/test_run_48029/from_Vincent/ntuple_file_waveform";
    string input_filename = "ntuple_run48029_merged.root";
    string output_data_filename = "ntuple_run48029_merged_raw_rate.root";
    string output_directory = input_directory + "/quick_check";
    string CAD_reading_filename = "/sphenix/tg/tg01/coldqcd/cwshih/test/test_run_48029/CAD_reading/plots/CAD_reading_48029.csv";


    vector<pair<int,int>> range_t_V = {
        {55, 352},
        {421, 451},
        {465, 499},
        {514, 547},
        {561, 612},
        {627, 734},

        {748, 783},
        {792, 826},
        {839, 876},
        {891, 923},
        {935, 988},
        {999, 1110}
    };

    vector<pair<int,int>> range_t_H = {
        {1126, 1193},
        {1207, 1240},
        {1255, 1290},
        {1302, 1337},
        {1347, 1402},
        {1410, 1523},

        {1540, 1571},
        {1585, 1616},
        {1632, 1664},
        {1678, 1716},
        {1732, 1778},
        {1794, 1902}
    };

    
    // gl1_scaler_ana * MBDS_ana = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     false, true, false
    // );
    // // MBDS_ana->PrepareRate();
    // // MBDS_ana->OutputRawRate((input_directory + "/" + output_data_filename).c_str());
    // MBDS_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // MBDS_ana->ImportCADReadings(CAD_reading_filename.c_str());
    // pair<TGraphErrors *, TGraphErrors *> MBDS_tgrE = MBDS_ana->CombineMacro("MBDS");



    // gl1_scaler_ana * MBDN_ana = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     false, true, false
    // );
    // MBDN_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // MBDN_ana->ImportCADReadings(CAD_reading_filename.c_str());
    // pair<TGraphErrors *, TGraphErrors *> MBDN_tgrE = MBDN_ana->CombineMacro("MBDN");



    gl1_scaler_ana * MBDNS_ana_fff = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, false, false
    );
    // MBDNS_ana_fff->PrepareRate();
    // MBDNS_ana_fff->OutputRawRate((input_directory + "/" + output_data_filename).c_str());
    MBDNS_ana_fff->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDNS_ana_fff->ImportCADReadings(CAD_reading_filename.c_str());
    pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE_fff = MBDNS_ana_fff->CombineMacro("MBDNS");
    MBDNS_ana_fff -> SaveHistROOT();

    // gl1_scaler_ana * MBDNS_ana_ftf = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     false, true, false
    // );
    // MBDNS_ana_ftf->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // MBDNS_ana_ftf->ImportCADReadings(CAD_reading_filename.c_str());
    // pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE_ftf = MBDNS_ana_ftf->CombineMacro("MBDNS");

    // gl1_scaler_ana * MBDNS_ana_ttf = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     true, true, false
    // );
    // MBDNS_ana_ttf->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // MBDNS_ana_ttf->ImportCADReadings(CAD_reading_filename.c_str());
    // pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE_ttf = MBDNS_ana_ttf->CombineMacro("MBDNS");

    // gl1_scaler_ana * MBDNS_ana_ttft = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     true, true, false, true
    // );
    // MBDNS_ana_ttft->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // MBDNS_ana_ttft->ImportCADReadings(CAD_reading_filename.c_str());
    // pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE_ttft = MBDNS_ana_ttft->CombineMacro("MBDNS");





    // gl1_scaler_ana * ZDCS_ana = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     false, true, false
    // );
    // ZDCS_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // ZDCS_ana->ImportCADReadings(CAD_reading_filename.c_str());
    // pair<TGraphErrors *, TGraphErrors *> ZDCS_tgrE = ZDCS_ana->CombineMacro("ZDCS");




    // gl1_scaler_ana * ZDCN_ana = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     false, true, false
    // );
    // ZDCN_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // ZDCN_ana->ImportCADReadings(CAD_reading_filename.c_str());
    // pair<TGraphErrors *, TGraphErrors *> ZDCN_tgrE = ZDCN_ana->CombineMacro("ZDCN");




    // gl1_scaler_ana * ZDCNS_ana_fff = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     false, false, false
    // );
    // ZDCNS_ana_fff->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // ZDCNS_ana_fff->ImportCADReadings(CAD_reading_filename.c_str());
    // pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_fff = ZDCNS_ana_fff->CombineMacro("ZDCNS");

    // gl1_scaler_ana * ZDCNS_ana_ftf = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     false, true, false
    // );
    // ZDCNS_ana_ftf->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // ZDCNS_ana_ftf->ImportCADReadings(CAD_reading_filename.c_str());
    // pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_ftf = ZDCNS_ana_ftf->CombineMacro("ZDCNS");

    // gl1_scaler_ana * ZDCNS_ana_ftt = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     false, true, true
    // );
    // ZDCNS_ana_ftt->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // ZDCNS_ana_ftt->ImportCADReadings(CAD_reading_filename.c_str());
    // pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_ftt = ZDCNS_ana_ftt->CombineMacro("ZDCNS");

    // gl1_scaler_ana * ZDCNS_ana_fttt = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     false, true, true, true
    // );
    // ZDCNS_ana_fttt->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // ZDCNS_ana_fttt->ImportCADReadings(CAD_reading_filename.c_str());
    // pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_fttt = ZDCNS_ana_fttt->CombineMacro("ZDCNS");


    // MBDS_ana -> GetInformation();
    // MBDN_ana -> GetInformation();
    // MBDNS_ana_fff -> GetInformation();
    // MBDNS_ana_ftf -> GetInformation();
    // MBDNS_ana_ttf -> GetInformation();
    // MBDNS_ana_ttft -> GetInformation();
    // ZDCS_ana -> GetInformation();
    // ZDCN_ana -> GetInformation();
    // ZDCNS_ana_fff -> GetInformation();
    // ZDCNS_ana_ftf -> GetInformation();
    // ZDCNS_ana_ftt -> GetInformation();
    // ZDCNS_ana_fttt -> GetInformation();

    // gl1_scaler_ana::PrintInfo(
    //     "Vertical", 
    //     {
    //         {"ZDCS", ZDCS_tgrE.first},
    //         {"ZDCN", ZDCN_tgrE.first},
    //         {"ZDCNS_ffff", ZDCNS_tgrE_fff.first},
    //         {"ZDCNS_ftff", ZDCNS_tgrE_ftf.first},
    //         {"ZDCNS_fttf", ZDCNS_tgrE_ftt.first},
    //         {"ZDCNS_fttt", ZDCNS_tgrE_fttt.first}
    //     },
    //     15
    // );

    // gl1_scaler_ana::PrintInfo(
    //     "Horizontal", 
    //     {
    //         {"ZDCS", ZDCS_tgrE.second},
    //         {"ZDCN", ZDCN_tgrE.second},
    //         {"ZDCNS_ffff", ZDCNS_tgrE_fff.second},
    //         {"ZDCNS_ftff", ZDCNS_tgrE_ftf.second},
    //         {"ZDCNS_fttf", ZDCNS_tgrE_ftt.second},
    //         {"ZDCNS_fttt", ZDCNS_tgrE_fttt.second}
    //     },
    //     15
    // );
    
    return 0;
}