#include "MBD_zvtx_effi.h"

R__LOAD_LIBRARY(libmbd_zvtx_effi_test.so)

int main_combine_Chris_Vincent()
{
    string input_directory_51195 = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform";
    string input_filename_51195 = "ntuple_run51195_merged.root";
    string input_rate_filename_51195 = "ntuple_run51195_merged_raw_rate.root";
    string output_directory_51195 = input_directory_51195 + "/quick_check_mbd_effi";
    // string CAD_reading_filename = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/CAD_reading/plots/CAD_reading_51195.csv";

    vector<pair<int,int>> range_t_V_51195 = {
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

    vector<pair<int,int>> range_t_H_51195 = {
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

    MBD_zvtx_effi * MBDZ_ana_51195 = new MBD_zvtx_effi(
        input_directory_51195, 
        input_filename_51195, 
        input_rate_filename_51195,
        output_directory_51195,
        range_t_V_51195,
        range_t_H_51195
    );

    // MBDZ_ana_51195 -> MBD_vtxZ_macro();
    // MBDZ_ana_51195 -> DrawPlots();

    std::string color_code_51195[4] = {
        "#CD5C5C",  // Indian Red (lightest)
        "#DC143C",  // Crimson
        "#B22222",  // Firebrick
        "#8B0000"   // Dark Red (darkest)
    };

    MBDZ_ana_51195 -> MBD_vtxZ_macro(100, false);
    MBDZ_ana_51195 -> DrawPlots(2);
    vector<TGraph *> Rchi2_TGraph_51195 = MBDZ_ana_51195 -> GetRchi2_TGraph();
    vector<double> effi_factor_51195; effi_factor_51195.clear();
    vector<TF1 *> effi_func_51195; effi_func_51195.clear();
    for (TGraph * gr : Rchi2_TGraph_51195)
    {
        pair<double, double> lowest_entry =  MBD_zvtx_effi::FindLowestEntry_gr(gr);
        effi_factor_51195.push_back(lowest_entry.first);
        effi_func_51195.push_back(new TF1("",MBD_zvtx_effi::pure_gaus_func, -300, 300, 2));
        effi_func_51195.back() -> SetParameters(0, lowest_entry.first);
        effi_func_51195.back() -> SetLineColor(TColor::GetColor(color_code_51195[effi_func_51195.size()-1].c_str()));
    }





    // string input_directory_48029 = "/sphenix/tg/tg01/coldqcd/cwshih/test/test_run_48029/from_Vincent/ntuple_file_waveform";
    // string input_filename_48029 = "ntuple_run48029_merged.root";
    // string input_rate_filename_48029 = "ntuple_run48029_merged_raw_rate.root";
    // string output_directory_48029 = input_directory_48029 + "/quick_check";


    // vector<pair<int,int>> range_t_V_48029 = {
    //     {55, 352},
    //     {421, 451},
    //     {465, 499},
    //     {514, 547},
    //     {561, 612},
    //     {627, 734},

    //     {748, 783},
    //     {792, 826},
    //     {839, 876},
    //     {891, 923},
    //     {935, 988},
    //     {999, 1110}
    // };

    // vector<pair<int,int>> range_t_H_48029 = {
    //     {1126, 1193},
    //     {1207, 1240},
    //     {1255, 1290},
    //     {1302, 1337},
    //     {1347, 1402},
    //     {1410, 1523},

    //     {1540, 1571},
    //     {1585, 1616},
    //     {1632, 1664},
    //     {1678, 1716},
    //     {1732, 1778},
    //     {1794, 1902}
    // };

    // std::string color_code_48029[4] = {
    //     "#87CEEB",  // Sky Blue (lightest)
    //     "#1E90FF",  // Dodger Blue
    //     "#4169E1",  // Royal Blue
    //     "#000080"   // Navy (darkest)
    // };

    // MBD_zvtx_effi * MBDZ_ana_48029 = new MBD_zvtx_effi(
    //     input_directory_48029, 
    //     input_filename_48029, 
    //     input_rate_filename_48029,
    //     output_directory_48029,
    //     range_t_V_48029,
    //     range_t_H_48029
    // );

    // // MBDZ_ana_48029 -> MBD_vtxZ_macro();
    // // MBDZ_ana_48029 -> DrawPlots();

    // MBDZ_ana_48029 -> MBD_vtxZ_macro(100, false);
    // MBDZ_ana_48029 -> DrawPlots(2);
    // vector<TGraph *> Rchi2_TGraph_48029 = MBDZ_ana_48029 -> GetRchi2_TGraph();
    // vector<double> effi_factor_48029; effi_factor_48029.clear();
    // vector<TF1 *> effi_func_48029; effi_func_48029.clear();
    // for (TGraph * gr : Rchi2_TGraph_48029)
    // {
    //     pair<double, double> lowest_entry =  MBD_zvtx_effi::FindLowestEntry_gr(gr);
    //     effi_factor_48029.push_back(lowest_entry.first);
    //     effi_func_48029.push_back(new TF1("",MBD_zvtx_effi::pure_gaus_func, -300, 300, 2));
    //     effi_func_48029.back() -> SetParameters(0, lowest_entry.first);
    //     effi_func_48029.back() -> SetLineColor(TColor::GetColor(color_code_48029[effi_func_48029.size()-1].c_str()));
    // }

    // SetsPhenixStyle();
    // TCanvas * c1 = new TCanvas("c1", "c1", 950, 800);

    // c1 -> cd();
    // for (int i = 0; i < effi_func_51195.size(); i++)
    // {
    //     if (i == 0) {
    //         effi_func_51195[i] -> GetYaxis() -> SetRangeUser(0.5, 1.1);
    //         effi_func_51195[i] -> Draw();
    //     }
    //     else {effi_func_51195[i] -> Draw("same");}
    // }
    // for (int i = 0; i < effi_func_48029.size(); i++)
    // {
    //     effi_func_48029[i] -> Draw("same");
    // }

    // c1 -> Print(Form("%s/effi_func_combine.pdf", output_directory_51195.c_str()));







    return 0;
}

// #include "MBD_zvtx_effi.h"

// int main()
// {
//     TGraph * aaa = new TGraph();
//     aaa->SetPoint(0,0,3);

//     pair<double, double> lowest_entry =  MBD_zvtx_effi::FindLowestEntry_gr(aaa);

//     return 0;
// }