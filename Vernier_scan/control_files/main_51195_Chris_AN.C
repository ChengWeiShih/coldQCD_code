#include "../gl1_scaler_ana.h"

R__LOAD_LIBRARY(../libgl1_scaler_ana.so)

int main_51195_Chris_AN()
{
    // string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris/ntuple_file";
    // string input_filename = "ntuple_run51195_merged.root";
    // string output_data_filename = "ntuple_run51195_merged_raw_rate.root";
    // string output_directory = input_directory + "/quick_check";

    string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform_ZDCvtxZ";
    string input_filename = "ntuple_run51195_merged.root";
    string output_data_filename = "ntuple_run51195_merged_raw_rate.root";
    string output_directory = input_directory + "/PlotsForAN";
    string CAD_reading_filename = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/CAD_reading/plots/CAD_reading_51195.csv";
    string Angelika_file_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/Angelika_rate";

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
    MBDNS_ana_ffft->SetShowEBarTag(false);
    MBDNS_ana_ffft->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDNS_ana_ffft->ImportCADReadings(CAD_reading_filename.c_str(), true);
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
    MBDNS_ana_ftft->ImportCADReadings(CAD_reading_filename.c_str(), true);
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
    MBDNS_ana_ttft->ImportCADReadings(CAD_reading_filename.c_str(), true);
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
    ZDCNS_ana_ffft->ImportCADReadings(CAD_reading_filename.c_str(), true);
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
    ZDCNS_ana_ftft->ImportCADReadings(CAD_reading_filename.c_str(), true);
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
    ZDCNS_ana_fttt->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_fttt = ZDCNS_ana_fttt->CombineMacro("ZDCNS");
    ZDCNS_ana_fttt->SaveHistROOT();    


    // note : Angelika numbers 
    gl1_scaler_ana * ZDCNS_ana_ffttft = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, false, true, true, false, true
    );
    ZDCNS_ana_ffttft->SetShowEBarTag(false);
    ZDCNS_ana_ffttft->SetDemoFactor({30., 15.});
    ZDCNS_ana_ffttft->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCNS_ana_ffttft->ImportCADReadings(CAD_reading_filename.c_str(), true);
    ZDCNS_ana_ffttft->ImportAngelikaRates(Form("%s/sPhenix-ZDC-x.34949.csv",Angelika_file_directory.c_str()), "H", "ZDCNS");
    ZDCNS_ana_ffttft->ImportAngelikaRates(Form("%s/sPhenix-ZDC-y.34949.csv",Angelika_file_directory.c_str()), "V", "ZDCNS");
    pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_ffttft = ZDCNS_ana_ffttft->CombineMacro("ZDCNS");
    ZDCNS_ana_ffttft->MakeCorrelationAngelika();
    ZDCNS_ana_ffttft->SaveHistROOT();



    MBDNS_ana_ffft -> GetInformation();
    MBDNS_ana_ftft -> GetInformation();
    MBDNS_ana_ttft -> GetInformation();
    
    ZDCNS_ana_ffft -> GetInformation();
    ZDCNS_ana_ftft -> GetInformation();
    ZDCNS_ana_fttt -> GetInformation();
    ZDCNS_ana_ffttft -> GetInformation();



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
            {"ZDCNS_ana_fttt", ZDCNS_tgrE_fttt.first},
            {"ZDCNS_ana_ffttft", ZDCNS_tgrE_ffttft.first}
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
            {"ZDCNS_ana_fttt", ZDCNS_tgrE_fttt.second},
            {"ZDCNS_ana_ffttft", ZDCNS_tgrE_ffttft.second}
        },
        15
    );


    return 0;
}