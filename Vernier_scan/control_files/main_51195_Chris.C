#include "../gl1_scaler_ana.h"

R__LOAD_LIBRARY(../libgl1_scaler_ana.so)

int main_51195_Chris()
{
    // string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris/ntuple_file";
    // string input_filename = "ntuple_run51195_merged.root";
    // string output_data_filename = "ntuple_run51195_merged_raw_rate.root";
    // string output_directory = input_directory + "/quick_check";

    string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform";
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

    gl1_scaler_ana * MBDS_ana = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, true, false
    );
    // MBDS_ana->PrepareRate();
    // MBDS_ana->OutputRawRate((input_directory + "/" + output_data_filename).c_str());
    MBDS_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDS_ana->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> MBDS_tgrE = MBDS_ana->CombineMacro("MBDS");



    gl1_scaler_ana * MBDN_ana = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, true, false
    );
    MBDN_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDN_ana->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> MBDN_tgrE = MBDN_ana->CombineMacro("MBDN");



    gl1_scaler_ana * MBDNS_ana_fff = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, false, false
    );
    MBDNS_ana_fff->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDNS_ana_fff->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE_fff = MBDNS_ana_fff->CombineMacro("MBDNS");

    gl1_scaler_ana * MBDNS_ana_ftf = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, true, false
    );
    MBDNS_ana_ftf->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDNS_ana_ftf->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE_ftf = MBDNS_ana_ftf->CombineMacro("MBDNS");

    gl1_scaler_ana * MBDNS_ana_ttf = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        true, true, false
    );
    MBDNS_ana_ttf->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDNS_ana_ttf->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE_ttf = MBDNS_ana_ttf->CombineMacro("MBDNS");
    MBDNS_ana_ttf->SaveHistROOT();

    
    gl1_scaler_ana * MBDNS_ana_ttft = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        true, true, false, true
    );
    MBDNS_ana_ttft->SetDemoFactor({30., 80.});
    MBDNS_ana_ttft->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDNS_ana_ttft->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE_ttft = MBDNS_ana_ttft->CombineMacro("MBDNS");
    // MBDNS_ana_ttft->SaveHistROOT();

    gl1_scaler_ana * MBDNS_ana_ftft = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, true, false, true
    );
    MBDNS_ana_ftft->SetDemoFactor({30., 80.});
    MBDNS_ana_ftft->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    MBDNS_ana_ftft->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE_ftft = MBDNS_ana_ftft->CombineMacro("MBDNS");




    gl1_scaler_ana * ZDCS_ana = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, true, false
    );
    ZDCS_ana->SetDemoFactor({30., 15.});
    ZDCS_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCS_ana->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> ZDCS_tgrE = ZDCS_ana->CombineMacro("ZDCS");




    gl1_scaler_ana * ZDCN_ana = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, true, false
    );
    ZDCN_ana->SetDemoFactor({30., 15.});
    ZDCN_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCN_ana->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> ZDCN_tgrE = ZDCN_ana->CombineMacro("ZDCN");




    gl1_scaler_ana * ZDCNS_ana_fff = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, false, false
    );
    ZDCNS_ana_fff->SetDemoFactor({30., 15.});
    ZDCNS_ana_fff->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCNS_ana_fff->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_fff = ZDCNS_ana_fff->CombineMacro("ZDCNS");

    gl1_scaler_ana * ZDCNS_ana_ftf = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, true, false
    );
    ZDCNS_ana_ftf->SetDemoFactor({30., 15.});
    ZDCNS_ana_ftf->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCNS_ana_ftf->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_ftf = ZDCNS_ana_ftf->CombineMacro("ZDCNS");

    gl1_scaler_ana * ZDCNS_ana_ftt = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, true, true
    );
    ZDCNS_ana_ftt->SetDemoFactor({30., 15.});
    ZDCNS_ana_ftt->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCNS_ana_ftt->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_ftt = ZDCNS_ana_ftt->CombineMacro("ZDCNS");

    gl1_scaler_ana * ZDCNS_ana_fttt = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H,
        false, true, true, true
    );
    ZDCNS_ana_fttt->SetDemoFactor({30., 15.});
    ZDCNS_ana_fttt->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    ZDCNS_ana_fttt->ImportCADReadings(CAD_reading_filename.c_str(), true);
    pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE_fttt = ZDCNS_ana_fttt->CombineMacro("ZDCNS");


    MBDS_ana -> GetInformation();
    MBDN_ana -> GetInformation();
    MBDNS_ana_fff -> GetInformation();
    MBDNS_ana_ftf -> GetInformation();
    MBDNS_ana_ttf -> GetInformation();
    MBDNS_ana_ttft -> GetInformation();
    MBDNS_ana_ftft -> GetInformation();
    ZDCS_ana -> GetInformation();
    ZDCN_ana -> GetInformation();
    ZDCNS_ana_fff -> GetInformation();
    ZDCNS_ana_ftf -> GetInformation();
    ZDCNS_ana_ftt -> GetInformation();
    ZDCNS_ana_fttt -> GetInformation();



    // note : vertical -> first
    // note : horizontal -> second

    gl1_scaler_ana::PrintInfo(
        "Vertical", 
        {
            {"MBDS", MBDS_tgrE.first},
            {"MBDN", MBDN_tgrE.first},
            {"MBDNS_ffff", MBDNS_tgrE_fff.first},
            {"MBDNS_ftff", MBDNS_tgrE_ftf.first},
            {"MBDNS_ttff", MBDNS_tgrE_ttf.first},
            {"MBDNS_ttft", MBDNS_tgrE_ttft.first},
            {"MBDNS_ftft", MBDNS_tgrE_ftft.first}
        },
        15
    );

    gl1_scaler_ana::PrintInfo(
        "Horizontal", 
        {
            {"MBDS", MBDS_tgrE.second},
            {"MBDN", MBDN_tgrE.second},
            {"MBDNS_ffff", MBDNS_tgrE_fff.second},
            {"MBDNS_ftff", MBDNS_tgrE_ftf.second},
            {"MBDNS_ttff", MBDNS_tgrE_ttf.second},
            {"MBDNS_ttft", MBDNS_tgrE_ttft.second},
            {"MBDNS_ftft", MBDNS_tgrE_ftft.second}
        },
        15
    );

    gl1_scaler_ana::PrintInfo(
        "Vertical", 
        {
            {"ZDCS", ZDCS_tgrE.first},
            {"ZDCN", ZDCN_tgrE.first},
            {"ZDCNS_ffff", ZDCNS_tgrE_fff.first},
            {"ZDCNS_ftff", ZDCNS_tgrE_ftf.first},
            {"ZDCNS_fttf", ZDCNS_tgrE_ftt.first},
            {"ZDCNS_fttt", ZDCNS_tgrE_fttt.first}
        },
        15
    );

    gl1_scaler_ana::PrintInfo(
        "Horizontal", 
        {
            {"ZDCS", ZDCS_tgrE.second},
            {"ZDCN", ZDCN_tgrE.second},
            {"ZDCNS_ffff", ZDCNS_tgrE_fff.second},
            {"ZDCNS_ftff", ZDCNS_tgrE_ftf.second},
            {"ZDCNS_fttf", ZDCNS_tgrE_ftt.second},
            {"ZDCNS_fttt", ZDCNS_tgrE_fttt.second}
        },
        15
    );


    return 0;
}