#include "../gl1_scaler_ana.h"

R__LOAD_LIBRARY(../libgl1_scaler_ana.so)

int main_51195_Chris_Angelika()
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

    gl1_scaler_ana * ZDCS_ana = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, false, false, true
    );
    ZDCS_ana->SetAngelika_rate(true);
    ZDCS_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCS_ana->ImportCADReadings(CAD_reading_filename.c_str(), true);

    ZDCS_ana -> ImportAngelikaRates(Form("%s/sPhenix-ZDC-x.34949.csv",Angelika_file_directory.c_str()), "H", "ZDCS");
    ZDCS_ana -> ImportAngelikaRates(Form("%s/sPhenix-ZDC-y.34949.csv",Angelika_file_directory.c_str()), "V", "ZDCS");

    pair<TGraphErrors *, TGraphErrors *> ZDCS_tgrE = ZDCS_ana->CombineMacro("ZDCS");
    ZDCS_ana->MakeCorrelationAngelika();    





    gl1_scaler_ana * ZDCN_ana = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, false, false, true
    );
    ZDCN_ana->SetAngelika_rate(true);
    ZDCN_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCN_ana->ImportCADReadings(CAD_reading_filename.c_str(), true);

    ZDCN_ana -> ImportAngelikaRates(Form("%s/sPhenix-ZDC-x.34949.csv",Angelika_file_directory.c_str()), "H", "ZDCN");
    ZDCN_ana -> ImportAngelikaRates(Form("%s/sPhenix-ZDC-y.34949.csv",Angelika_file_directory.c_str()), "V", "ZDCN");

    pair<TGraphErrors *, TGraphErrors *> ZDCN_tgrE = ZDCN_ana->CombineMacro("ZDCN");
    ZDCN_ana->MakeCorrelationAngelika();   





    gl1_scaler_ana * ZDCNS_ana_ffft = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, false, false, true
    );
    ZDCNS_ana_ffft->SetAngelika_rate(true);
    ZDCNS_ana_ffft->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCNS_ana_ffft->ImportCADReadings(CAD_reading_filename.c_str(), true);

    ZDCNS_ana_ffft -> ImportAngelikaRates(Form("%s/sPhenix-ZDC-x.34949.csv",Angelika_file_directory.c_str()), "H", "ZDCNS");
    ZDCNS_ana_ffft -> ImportAngelikaRates(Form("%s/sPhenix-ZDC-y.34949.csv",Angelika_file_directory.c_str()), "V", "ZDCNS");

    pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_ffft = ZDCNS_ana_ffft->CombineMacro("ZDCNS");
    ZDCNS_ana_ffft->MakeCorrelationAngelika();   






    gl1_scaler_ana * ZDCNS_ana_fftt = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, false, true, true
    );
    ZDCNS_ana_fftt->SetAngelika_rate(true);
    ZDCNS_ana_fftt->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCNS_ana_fftt->ImportCADReadings(CAD_reading_filename.c_str(), true);

    ZDCNS_ana_fftt -> ImportAngelikaRates(Form("%s/sPhenix-ZDC-x.34949.csv",Angelika_file_directory.c_str()), "H", "ZDCNS");
    ZDCNS_ana_fftt -> ImportAngelikaRates(Form("%s/sPhenix-ZDC-y.34949.csv",Angelika_file_directory.c_str()), "V", "ZDCNS");

    pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_fftt = ZDCNS_ana_fftt->CombineMacro("ZDCNS");
    ZDCNS_ana_fftt->MakeCorrelationAngelika();   
    
    


    return 0;
}