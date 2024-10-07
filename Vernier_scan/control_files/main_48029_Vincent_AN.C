#include "../gl1_scaler_ana.h"

R__LOAD_LIBRARY(../libgl1_scaler_ana.so)

int main_48029_Vincent_AN()
{
    string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/test_run_48029/from_Vincent/ntuple_file_waveform_ZDCvtxZ";
    string input_filename = "ntuple_run48029_merged.root";
    string output_data_filename = "ntuple_run48029_merged_raw_rate.root";
    string output_directory = input_directory + "/PlotsForAN";
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

    

    gl1_scaler_ana * MBDNS_ana_ffft = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, false, false, true
    );
    MBDNS_ana_ffft->SetShowEBarTag(false);
    MBDNS_ana_ffft->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDNS_ana_ffft->ImportCADReadings(CAD_reading_filename.c_str());
    pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE_ffft = MBDNS_ana_ffft->CombineMacro("MBDNS");

    gl1_scaler_ana * MBDNS_ana_ftft = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, true, false, true
    );
    MBDNS_ana_ftft->SetShowEBarTag(false);
    MBDNS_ana_ftft->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDNS_ana_ftft->ImportCADReadings(CAD_reading_filename.c_str());
    pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE_ftft = MBDNS_ana_ftft->CombineMacro("MBDNS");

    gl1_scaler_ana * MBDNS_ana_ttft = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        true, true, false, true
    );
    MBDNS_ana_ttft->SetShowEBarTag(false);
    MBDNS_ana_ttft->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDNS_ana_ttft->ImportCADReadings(CAD_reading_filename.c_str());
    pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE_ttft = MBDNS_ana_ttft->CombineMacro("MBDNS");
    MBDNS_ana_ttft->SaveHistROOT();

    






    gl1_scaler_ana * ZDCNS_ana_ffft = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, false, false, true
    );
    ZDCNS_ana_ffft->SetShowEBarTag(false);
    ZDCNS_ana_ffft->SetDemoFactor({30., 15.});
    ZDCNS_ana_ffft->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCNS_ana_ffft->ImportCADReadings(CAD_reading_filename.c_str());
    pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_ffft = ZDCNS_ana_ffft->CombineMacro("ZDCNS");

    gl1_scaler_ana * ZDCNS_ana_ftft = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, true, false, true
    );
    ZDCNS_ana_ftft->SetShowEBarTag(false);
    ZDCNS_ana_ftft->SetDemoFactor({30., 15.});
    ZDCNS_ana_ftft->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCNS_ana_ftft->ImportCADReadings(CAD_reading_filename.c_str());
    pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_ftft = ZDCNS_ana_ftft->CombineMacro("ZDCNS");

    gl1_scaler_ana * ZDCNS_ana_fttt = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, true, true, true
    );
    ZDCNS_ana_fttt->SetShowEBarTag(false);
    ZDCNS_ana_fttt->SetDemoFactor({30., 15.});
    ZDCNS_ana_fttt->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCNS_ana_fttt->ImportCADReadings(CAD_reading_filename.c_str());
    pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_fttt = ZDCNS_ana_fttt->CombineMacro("ZDCNS");
    ZDCNS_ana_fttt->SaveHistROOT();    


    MBDNS_ana_ffft -> GetInformation();
    MBDNS_ana_ftft -> GetInformation();
    MBDNS_ana_ttft -> GetInformation();
    
    ZDCNS_ana_ffft -> GetInformation();
    ZDCNS_ana_ftft -> GetInformation();
    ZDCNS_ana_fttt -> GetInformation();



    // note : vertical -> first
    // note : horizontal -> second

    gl1_scaler_ana::PrintInfo(
        "Vertical", 
        {
            // {"MBDS", MBDS_tgrE.first},
            // {"MBDN", MBDN_tgrE.first},
            {"MBDNS_ana_ffft", MBDNS_tgrE_ffft.first},
            {"MBDNS_ana_ftft", MBDNS_tgrE_ftft.first},
            {"MBDNS_ana_ttft", MBDNS_tgrE_ttft.first}
        },
        15
    );

    gl1_scaler_ana::PrintInfo(
        "Horizontal", 
        {
            // {"MBDS", MBDS_tgrE.second},
            // {"MBDN", MBDN_tgrE.second},
            {"MBDNS_ana_ffft", MBDNS_tgrE_ffft.second},
            {"MBDNS_ana_ftft", MBDNS_tgrE_ftft.second},
            {"MBDNS_ana_ttft", MBDNS_tgrE_ttft.second},
        },
        15
    );

    gl1_scaler_ana::PrintInfo(
        "Vertical", 
        {
            // {"ZDCS", ZDCS_tgrE.first},
            // {"ZDCN", ZDCN_tgrE.first},
            {"ZDCNS_ana_ffft", ZDCNS_tgrE_ffft.first},
            {"ZDCNS_ana_ftft", ZDCNS_tgrE_ftft.first},
            {"ZDCNS_ana_fttt", ZDCNS_tgrE_fttt.first}
        },
        15
    );

    gl1_scaler_ana::PrintInfo(
        "Horizontal", 
        {
            // {"ZDCS", ZDCS_tgrE.second},
            // {"ZDCN", ZDCN_tgrE.second},
            {"ZDCNS_ana_ffft", ZDCNS_tgrE_ffft.second},
            {"ZDCNS_ana_ftft", ZDCNS_tgrE_ftft.second},
            {"ZDCNS_ana_fttt", ZDCNS_tgrE_fttt.second}
        },
        15
    );
    
    return 0;
}