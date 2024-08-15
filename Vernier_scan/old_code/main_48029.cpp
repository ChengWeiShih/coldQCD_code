#include "gl1_scaler_ana.h"

int main()
{
    string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/test_run_48029/GL1_selfgen_DST/ntuple_file_2";
    string input_filename = "ntuple_run48029_merged.root";
    string output_data_filename = "ntuple_run48029_merged_raw_rate.root";
    string output_directory = input_directory + "/quick_check";


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

    gl1_scaler_ana * MBDNS_ana = new gl1_scaler_ana(
        input_directory, 
        input_filename, 
        output_directory,
        range_t_V,
        range_t_H
    );
    MBDNS_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE = MBDNS_ana->CombineMacro("MBDNS");


    // vector<double> Average_BPM_pos_V = { 
    //     -0.8955000040
    //     ,-0.7848846910
    //     ,-0.6212499735
    //     ,-0.4642499760
    //     ,-0.2646578810
    //     , 0.0339359045

    //     ,-0.8877499700
    //     ,-0.9983078240
    //     ,-1.1607857645
    //     ,-1.3163929580
    //     ,-1.5157895085
    //     ,-1.8121411805
    //     // ,-0.8864584265
    // };
    // vector<double> BeamIntensity_corr_V = { // note : the correction given by the DCCT readings, 
    //     1.0000000000
    //     ,1.0003342246
    //     ,1.0006587735
    //     ,1.0009807643
    //     ,1.0013684726
    //     ,1.0021379909

    //     ,1.0029446185
    //     ,1.0032875116
    //     ,1.0035655534
    //     ,1.0037793039
    //     ,1.0040018592
    //     ,1.0045751376
    // };


    // vector<double> Average_BPM_pos_H = { 
    //     1.4384230675
    //     ,1.5476666690
    //     ,1.7083572150
    //     ,1.8634286225
    //     ,2.0610527395
    //     ,2.3544354440

    //     ,1.4384999870
    //     ,1.3300416915
    //     ,1.1687143445
    //     ,1.0138571005
    //     ,0.8151841835
    //     ,0.5200257900
    //     // ,1.4327692390
    // };
    // vector<double> BeamIntensity_corr_H = { // note : the correction given by the DCCT readings 
    //     1.0055895998
    //     ,1.0059053234
    //     ,1.0062043754
    //     ,1.0064875776
    //     ,1.0067955832
    //     ,1.0073272894

    //     ,1.0078651170
    //     ,1.0081684585
    //     ,1.0084488888
    //     ,1.0087273819
    //     ,1.0090950065
    //     ,1.0097006287
    // };

    // gl1_scaler_ana ana(input_directory, input_filename, output_directory);
    // ana.PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // ana.PrepareRate();
    // ana.OutputRawRate((input_directory + "/" + output_data_filename).c_str());


    // gl1_scaler_ana * ZDCS_ana = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H
    // );
    // ZDCS_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // pair<TGraphErrors *, TGraphErrors *> ZDCS_tgrE = ZDCS_ana->CombineMacro("ZDCS");

    // gl1_scaler_ana * ZDCN_ana = new gl1_scaler_ana(input_directory, input_filename, output_directory);
    // ZDCN_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // pair<TGraphErrors *, TGraphErrors *> ZDCN_tgrE = ZDCN_ana->CombineMacro("ZDCN");

    // gl1_scaler_ana * ZDCNS_ana = new gl1_scaler_ana(input_directory, input_filename, output_directory);
    // ZDCNS_ana->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // pair<TGraphErrors *, TGraphErrors *> ZDCNS_tgrE = ZDCNS_ana->CombineMacro("ZDCNS");

    // gl1_scaler_ana::PrintInfo(
    //     "Vertical", 
    //     {
    //         {"ZDCS", ZDCS_tgrE.first},
    //         {"ZDCN", ZDCN_tgrE.first},
    //         {"ZDCNS", ZDCNS_tgrE.first}
    //     },
    //     15
    // );


    // pair<TGraphErrors *, TGraphErrors *> MBDS_tgrE = ana.CombineMacro("MBDS");
    // ana.ClearUp();
    // pair<TGraphErrors *, TGraphErrors *> MBDN_tgrE = ana.CombineMacro("MBDN");
    // ana.ClearUp();
    // pair<TGraphErrors *, TGraphErrors *> MBDNS_tgrE = ana.CombineMacro("MBDNS");
    // ana.ClearUp();
    

    return 0;
}