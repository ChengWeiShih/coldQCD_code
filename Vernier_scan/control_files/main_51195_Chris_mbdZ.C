// #include "MBD_zvtx_effi.h"

// R__LOAD_LIBRARY(libmbd_zvtx_effi.so)

// int main_51195_Chris_mbdZ()
// {
//     string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform";
//     string input_filename = "ntuple_run51195_merged.root";
//     string intput_rate_filename = "ntuple_run51195_merged_raw_rate.root";
//     string output_directory = input_directory + "/quick_check_mbd_effi";
//     // string CAD_reading_filename = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/CAD_reading/plots/CAD_reading_51195.csv";

//     vector<pair<int,int>> range_t_V = {
//         {786, 858},
//         {871, 902},
//         {918, 953},
//         {969, 1007},
//         {1020, 1070},
//         {1082, 1197},

//         {1210, 1239},
//         {1257, 1290},
//         {1305, 1339},
//         {1354, 1391},
//         {1404, 1456},
//         {1469, 1582}
//     };

//     vector<pair<int,int>> range_t_H = {
//         {6, 50},
//         {64,96},
//         {113,146},
//         {162,195},
//         {213,262},
//         {277,389},

//         {404,433},
//         {449,479},
//         {495, 531},
//         {548, 581},
//         {597, 646},
//         {659, 774}
//     };

//     MBD_zvtx_effi * MBDZ_ana = new MBD_zvtx_effi(
//         input_directory, 
//         input_filename, 
//         intput_rate_filename,
//         output_directory,
//         range_t_V,
//         range_t_H
//     );

//     // MBDZ_ana -> MBD_vtxZ_macro();
//     // MBDZ_ana -> DrawPlots();

//     MBDZ_ana -> MBD_vtxZ_macro(100, false);
//     MBDZ_ana -> DrawPlots(2);

//     return 0;
// }


// #include "../gl1_scaler_ana.h"
// R__LOAD_LIBRARY(../libgl1_scaler_ana.so)

#include "../MBD_zvtx_effi.h"
R__LOAD_LIBRARY(../libmbd_zvtx_effi.so)

int main_51195_Chris_mbdZ()
{
    // string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris/ntuple_file";
    // string input_filename = "ntuple_run51195_merged.root";
    // string output_data_filename = "ntuple_run51195_merged_raw_rate.root";
    // string output_directory = input_directory + "/quick_check";

    string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform_ZDCvtxZ";
    string input_filename = "ntuple_run51195_merged.root";
    string output_data_filename = "ntuple_run51195_merged_raw_rate.root";
    string output_directory = input_directory + "/quick_check";
    string CAD_reading_filename = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/CAD_reading/plots/CAD_reading_51195.csv";

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

    // gl1_scaler_ana * MBDS_ana = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     false, true, false
    // );
    // cout<<1111<<endl;
    // MBDS_ana->PrepareRate();
    // cout<<2222<<endl;
    // MBDS_ana->OutputRawRate((input_directory + "/" + output_data_filename).c_str());
    // MBDS_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // MBDS_ana->ImportCADReadings(CAD_reading_filename.c_str(), true);
    // pair<TGraphErrors *, TGraphErrors *> MBDNS = MBDS_ana->CombineMacro("MBDNS", true);

    MBD_zvtx_effi * MBDZ_ana = new MBD_zvtx_effi(
        input_directory, 
        input_filename, 
        output_data_filename,
        output_directory,
        range_t_V,
        range_t_H
    );

    MBDZ_ana -> RunMacro();
    MBDZ_ana -> DrawPlots();

    vector<TF1 *> effi_fit_functions = MBDZ_ana -> GetEffiFitFuncSeparate();
    for (TF1 * func : effi_fit_functions) {
        cout<<func->GetName()<<" width : "<<func->GetParameter(2)<<endl;
    }

    vector<TF1 *> effi_fit_functions_comb = MBDZ_ana -> GetEffiFitFuncComb();
    for (TF1 * func : effi_fit_functions_comb) {
        cout<<func->GetName()<<" width : "<<func->GetParameter(2)<<endl;
    }



    return 0;
}