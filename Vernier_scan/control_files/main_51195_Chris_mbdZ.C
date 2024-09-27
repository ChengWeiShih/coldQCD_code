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


#include "../gl1_scaler_ana.h"
R__LOAD_LIBRARY(../libgl1_scaler_ana.so)

// #include "../MBD_zvtx_effi.h"
// R__LOAD_LIBRARY(../libmbd_zvtx_effi.so)

double gaus_func(double *x, double *par)
{
    // note : par[0] : size
    // note : par[1] : mean
    // note : par[2] : width
    // note : par[3] : offset
    return par[0] * TMath::Gaus(x[0],par[1],par[2]) + par[3];
}

int main_51195_Chris_mbdZ(int process_id)
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

    std::string numStr = std::to_string(process_id);
    numStr = std::string(5 - numStr.length(), '0') + numStr;

    TFile * file_out = new TFile(Form("%s/MBDEffiScan_%s.root",output_directory.c_str(),numStr.c_str()), "recreate");
    TTree * tree_out = new TTree("tree", "tree");
    double effi_width_out;
    double overlap_widthH_out;
    double overlap_widthV_out;
    double MBDcrosssection_out;
    tree_out -> Branch("effi_width", &effi_width_out);
    tree_out -> Branch("overlap_widthH", &overlap_widthH_out);
    tree_out -> Branch("overlap_widthV", &overlap_widthV_out);
    tree_out -> Branch("MBDcrosssection", &MBDcrosssection_out);


    vector<double> effi_width_vec; effi_width_vec.clear();
    vector<double> overlap_width_vecH; overlap_width_vecH.clear();
    vector<double> overlap_width_vecV; overlap_width_vecV.clear();
    vector<double> crosssection_vec; crosssection_vec.clear();

    TF1 * dummy_effi_func = new TF1("dummy_effi_func", gaus_func, -400, 400, 4);

    map<int, gl1_scaler_ana*> MBDNS_ana_map;

    for (int i = 0; i < 1; i++)
    {
        double effi_width = 170 + process_id * 30;

        dummy_effi_func -> SetParameters(1, 0, effi_width, 0);

        MBDNS_ana_map[i] = new gl1_scaler_ana(
            input_directory, 
            input_filename, 
            output_directory,
            range_t_V,
            range_t_H,
            true, true, false, true, true
        );

        MBDNS_ana_map[i] -> PrepareRate((input_directory + "/" + output_data_filename).c_str());
        MBDNS_ana_map[i] -> ImportCADReadings(CAD_reading_filename.c_str(), true);
        MBDNS_ana_map[i] -> SetMBDvtxZEffiFunc(dummy_effi_func);
        MBDNS_ana_map[i] -> CombineMacro("MBDNS", false, Form("%.0f", effi_width));

        double MBDNScrosssection = MBDNS_ana_map[i] -> GetDetectorCrossSection();
        pair<double, double> overlap_width_pair = MBDNS_ana_map[i] -> GetOverlapWidths();

        effi_width_vec.push_back(effi_width);
        overlap_width_vecV.push_back(overlap_width_pair.first);
        overlap_width_vecH.push_back(overlap_width_pair.second);
        crosssection_vec.push_back(MBDNScrosssection);


        effi_width_out = effi_width; 
        overlap_widthH_out = overlap_width_pair.second;
        overlap_widthV_out = overlap_width_pair.first;
        MBDcrosssection_out = MBDNScrosssection;

        tree_out -> Fill();
    }


    // SetsPhenixStyle();
    // TCanvas * c3 = new TCanvas("c3", "c3", 950, 800);
    // c3 -> cd();

    // TGraph * effi_width_graph = new TGraph(effi_width_vec.size(), &effi_width_vec[0], &crosssection_vec[0]);
    // effi_width_graph -> GetXaxis() -> SetNdivisions(505);
    // effi_width_graph -> SetTitle(";MBD Z vertex efficiency width [cm];MBD cross section [mb]");
    // effi_width_graph -> SetMarkerStyle(20);
    // effi_width_graph -> SetMarkerSize(0.7);
    // effi_width_graph -> Draw("AP");
    // c3 -> Print(Form("%s/effi_width_graph.pdf", output_directory.c_str()));
    // c3 -> Clear();

    // TGraph * width_H_graph = new TGraph(effi_width_vec.size(), &effi_width_vec[0], &overlap_width_vecH[0]);
    // width_H_graph -> GetXaxis() -> SetNdivisions(505);
    // width_H_graph -> SetTitle(";MBD Z vertex efficiency width [cm];Width of overlap region (H) [mm]");
    // width_H_graph -> SetMarkerStyle(20);
    // width_H_graph -> SetMarkerSize(0.7);
    // width_H_graph -> Draw("AP");
    // c3 -> Print(Form("%s/width_H_graph.pdf", output_directory.c_str()));
    // c3 -> Clear();

    // TGraph * width_V_graph = new TGraph(effi_width_vec.size(), &effi_width_vec[0], &overlap_width_vecV[0]);
    // width_V_graph -> GetXaxis() -> SetNdivisions(505);
    // width_V_graph -> SetTitle(";MBD Z vertex efficiency width [cm];Width of overlap region (V) [mm]");
    // width_V_graph -> SetMarkerStyle(20);
    // width_V_graph -> SetMarkerSize(0.7);
    // width_V_graph -> Draw("AP");
    // c3 -> Print(Form("%s/width_V_graph.pdf", output_directory.c_str()));
    // c3 -> Clear();




    file_out -> Write();
    file_out -> Close();

    

    // gl1_scaler_ana * MBDNS_ana_ttftt = new gl1_scaler_ana(
    //     input_directory, 
    //     input_filename, 
    //     output_directory,
    //     range_t_V,
    //     range_t_H,
    //     true, true, false, true, true
    // );
    // MBDNS_ana_ttftt->PrepareRate();
    // MBDNS_ana_ttftt->OutputRawRate((input_directory + "/" + output_data_filename).c_str());
    // MBDNS_ana_ttftt->PrepareRate((input_directory + "/" + output_data_filename).c_str());
    // MBDNS_ana_ttftt->ImportCADReadings(CAD_reading_filename.c_str(), true);
    // MBDNS_ana_ttftt->SetMBDvtxZEffiFunc(dummy_effi_func);
    // pair<TGraphErrors *, TGraphErrors *> MBDNS = MBDNS_ana_ttftt->CombineMacro("MBDNS", false);

    // MBD_zvtx_effi * MBDZ_ana = new MBD_zvtx_effi(
    //     input_directory, 
    //     input_filename, 
    //     output_data_filename,
    //     output_directory,
    //     range_t_V,
    //     range_t_H
    // );

    // MBDZ_ana -> RunMacro();
    // MBDZ_ana -> DrawPlots();

    // vector<TF1 *> effi_fit_functions = MBDZ_ana -> GetEffiFitFuncSeparate();
    // for (TF1 * func : effi_fit_functions) {
    //     cout<<func->GetName()<<" width : "<<func->GetParameter(2)<<endl;
    // }

    // vector<TF1 *> effi_fit_functions_comb = MBDZ_ana -> GetEffiFitFuncComb();
    // for (TF1 * func : effi_fit_functions_comb) {
    //     cout<<func->GetName()<<" width : "<<func->GetParameter(2)<<endl;
    // }





    return 0;
}