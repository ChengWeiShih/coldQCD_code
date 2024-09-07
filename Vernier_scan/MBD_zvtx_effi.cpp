#include "MBD_zvtx_effi.h"

MBD_zvtx_effi::MBD_zvtx_effi(
    string input_directory_in, 
    string input_filename_in, 
    string input_rate_file_in,
    string output_directory_in,
    vector<pair<int,int>> range_t_V_in,
    vector<pair<int,int>> range_t_H_in
)
: gl1_scaler_ana(
    input_directory_in, 
    input_filename_in, 
    output_directory_in,
    range_t_V_in,
    range_t_H_in, 
    false,
    false,
    false,
    false
),
input_rate_file(input_rate_file_in),
chosen_scan_steps({0, 6})
{
    gl1_scaler_ana::PrepareRate((input_directory_in + "/" + input_rate_file).c_str());
    final_output_directory = output_directory;

    TGaxis::SetMaxDigits(4);

    fit_gaus_mapH.clear();
    fit_gaus_mapV.clear();

    Rchi2_TrialStepV.clear();
    WidthFitError_TrialStepV.clear();
    MeanFitError_TrialStepV.clear();

    Rchi2_TrialStepH.clear();
    WidthFitError_TrialStepH.clear();
    MeanFitError_TrialStepH.clear();

    for (int step : chosen_scan_steps) // note : for each direction of the scan
    {
        fit_gaus_mapH[step] = new TF1(Form("fit_gaus_mapH_%d", step), gaus_func /*gaus_func_nohead*/, -300, 300, 4);
        fit_gaus_mapV[step] = new TF1(Form("fit_gaus_mapV_%d", step), gaus_func /*gaus_func_nohead*/, -300, 300, 4);
        fit_gaus_mapH[step] -> SetLineColor(2);
        fit_gaus_mapV[step] -> SetLineColor(2);

        Rchi2_TrialStepV[step] = new TGraph(); // note : for multiple scan steps in the scan
        WidthFitError_TrialStepV[step] = new TGraph();
        MeanFitError_TrialStepV[step] = new TGraph();

        Rchi2_TrialStepV[step] -> SetMarkerStyle(20);
        WidthFitError_TrialStepV[step] -> SetMarkerStyle(20);
        MeanFitError_TrialStepV[step] -> SetMarkerStyle(20);

        Rchi2_TrialStepV[step] -> SetMarkerColor(TColor::GetColor(color_code[step].c_str()));
        WidthFitError_TrialStepV[step] -> SetMarkerColor(TColor::GetColor(color_code[step].c_str()));
        MeanFitError_TrialStepV[step] -> SetMarkerColor(TColor::GetColor(color_code[step].c_str()));

        Rchi2_TrialStepV[step] -> SetTitle(";MBD effi. factor;Reduced #chi^{2}");
        WidthFitError_TrialStepV[step] -> SetTitle(";MBD effi. factor;Gaus Width Fit Error");
        MeanFitError_TrialStepV[step] -> SetTitle(";MBD effi. factor;Gaus Mean Fit Error");



        Rchi2_TrialStepH[step] = new TGraph(); // note : for multiple scan steps in the scan
        WidthFitError_TrialStepH[step] = new TGraph();
        MeanFitError_TrialStepH[step] = new TGraph();

        Rchi2_TrialStepH[step] -> SetMarkerStyle(20);
        WidthFitError_TrialStepH[step] -> SetMarkerStyle(20);
        MeanFitError_TrialStepH[step] -> SetMarkerStyle(20);

        Rchi2_TrialStepH[step] -> SetMarkerColor(TColor::GetColor(color_code[step + 2].c_str()));
        WidthFitError_TrialStepH[step] -> SetMarkerColor(TColor::GetColor(color_code[step + 2].c_str()));
        MeanFitError_TrialStepH[step] -> SetMarkerColor(TColor::GetColor(color_code[step + 2].c_str()));

        Rchi2_TrialStepH[step] -> SetTitle(";MBD effi. factor;Reduced #chi^{2}");
        WidthFitError_TrialStepH[step] -> SetTitle(";MBD effi. factor;Gaus Width Fit Error");
        MeanFitError_TrialStepH[step] -> SetTitle(";MBD effi. factor;Gaus Mean Fit Error");
    }

    c2 = new TCanvas("c2", "c2", 950 * chosen_scan_steps.size(), 1600);
    c2 -> Divide(chosen_scan_steps.size(), 2);
}

void MBD_zvtx_effi::MBD_vtxZ_macro(int total_trials, bool run_simple)
{
    // TF1 * MBD_z_effi_func = new TF1("MBD_z_effi_func", pure_gaus_func /*negative_pol2*/, -300, 300, 2);
    TF1 * MBD_z_effi_func = new TF1("MBD_z_effi_func", "pol0", -300, 300);

    if (run_simple) 
    {
        MakeMBD_vertexZ_hist_func(
            range_t_V, 
            h1D_detectorNS_vertexZ_vecV,
            fit_gaus_mapV,
            MBD_z_effi_func,
            false
        );

        MakeMBD_vertexZ_hist_func(
            range_t_H, 
            h1D_detectorNS_vertexZ_vecH,
            fit_gaus_mapH,
            MBD_z_effi_func,
            false
        );

        return;
    }

    

    for (int i = 0; i < total_trials; i++)
    {
        // note : for the case of negative pol2
        // double effi_factor = double(i) * 0.00004;
        // MBD_z_effi_func -> SetParameters( effi_factor, 0.);

        // note : for the case of gaussian
        // double effi_factor = 0.5 * i + 50;
        // MBD_z_effi_func -> SetParameters( 0, effi_factor);

        double effi_factor = double(i);
        MBD_z_effi_func -> SetParameter(0,1.);

        MakeMBD_vertexZ_hist_func(
            range_t_V, 
            h1D_detectorNS_vertexZ_vecV,
            fit_gaus_mapV,
            MBD_z_effi_func,
            true
        );

        MakeMBD_vertexZ_hist_func(
            range_t_H, 
            h1D_detectorNS_vertexZ_vecH,
            fit_gaus_mapH,
            MBD_z_effi_func,
            true
        );

        DrawTrialPlots(i, total_trials);


        for (int step : chosen_scan_steps)
        {
            Rchi2_TrialStepV[step] -> SetPoint(Rchi2_TrialStepV[step] -> GetN(), effi_factor, fit_gaus_mapV[step] -> GetChisquare() / double(fit_gaus_mapV[step] -> GetNDF()));
            WidthFitError_TrialStepV[step] -> SetPoint(WidthFitError_TrialStepV[step] -> GetN(), effi_factor, fit_gaus_mapV[step] -> GetParError(2));
            MeanFitError_TrialStepV[step] -> SetPoint(MeanFitError_TrialStepV[step] -> GetN(), effi_factor, fit_gaus_mapV[step] -> GetParError(1));
        }

        for (int step : chosen_scan_steps)
        {
            Rchi2_TrialStepH[step] -> SetPoint(Rchi2_TrialStepH[step] -> GetN(), effi_factor, fit_gaus_mapH[step] -> GetChisquare() / double(fit_gaus_mapH[step] -> GetNDF()));
            WidthFitError_TrialStepH[step] -> SetPoint(WidthFitError_TrialStepH[step] -> GetN(), effi_factor, fit_gaus_mapH[step] -> GetParError(2));
            MeanFitError_TrialStepH[step] -> SetPoint(MeanFitError_TrialStepH[step] -> GetN(), effi_factor, fit_gaus_mapH[step] -> GetParError(1));
        }

        ClearUp();
    }

    

}

vector<TGraph *> MBD_zvtx_effi::GetRchi2_TGraph()
{
    vector<TGraph *> Rchi2_TGraph_vec;
    for (int step : chosen_scan_steps)
    {
        Rchi2_TGraph_vec.push_back(Rchi2_TrialStepV[step]);
        Rchi2_TGraph_vec.push_back(Rchi2_TrialStepH[step]);
    }

    return Rchi2_TGraph_vec;
}

void MBD_zvtx_effi::MakeMBD_vertexZ_hist_func(
    vector<pair<int,int>> &range_t_All, 
    vector<TH1F *> &h1D_detectorNS_vertexZ_vecAll,
    map<int, TF1 *> &fit_gaus_map_all,
    TF1 * MBD_z_effi, // note : Gaussian distribution describing the vertex Z efficiency resolution
    bool apply_effi_corr
)
{
    if (apply_effi_corr)
    {
        for (int step : chosen_scan_steps)
        {
            for (auto pair : time_MBDNS_zvtx) 
            { 
                if (pair.first >= range_t_All[step].first && pair.first <= range_t_All[step].second)
                {
                    if (detector_selection == "MBDNS")
                    {
                        for (double Dvtx_i : pair.second){
                            if (Dvtx_i == -999) {continue;}
                            h1D_detectorNS_vertexZ_vecAll[step] -> Fill(Dvtx_i, 1. / MBD_z_effi -> Eval(Dvtx_i));
                        }
                    }
                }
            }

            fit_gaus_map_all[step] -> SetParameters(
                h1D_detectorNS_vertexZ_vecAll[step] -> GetBinContent(h1D_detectorNS_vertexZ_vecAll[step] -> GetMaximumBin()), 
                0., 
                h1D_detectorNS_vertexZ_vecAll[step] -> GetStdDev(), 
                0
            );

            h1D_detectorNS_vertexZ_vecAll[step] -> Fit(fit_gaus_map_all[step], "NQ","",-130,130);
            // h1D_detectorNS_vertexZ_vecAll[step] -> Fit(fit_gaus_map_all[step], "NQ");
        }

    }
    else 
    {
        // note : the time range of each scan step, for the case of vertical scan
        for (int i = 0; i < range_t_All.size(); i++)
        {
            for (auto pair : time_MBDNS_zvtx) 
            { 
                if (pair.first >= range_t_All[i].first && pair.first <= range_t_All[i].second)
                {
                    if (detector_selection == "MBDNS")
                    {
                        for (int vtx_i = 0; vtx_i < pair.second.size(); vtx_i++)
                        {
                            if (pair.second[vtx_i] == -999) {continue;}
                            h1D_detectorNS_vertexZ_vecAll[i] -> Fill(pair.second[vtx_i]);
                        }
                    }
                }
            }
        }
    }

    
}

void MBD_zvtx_effi::DrawTrialPlots(int index, int total_trials)
{

    int i = 0;
    for (int step : chosen_scan_steps)
    {
        c2 -> cd(i + 1);
        h1D_detectorNS_vertexZ_vecV[step] -> GetXaxis() -> SetTitle(Form("MBDz_V, step_%d, id_%d", step, index));
        h1D_detectorNS_vertexZ_vecV[step] -> Draw("hist");
        fit_gaus_mapV[step] -> Draw("lsame");

        c2 -> cd(i + 1 + chosen_scan_steps.size());
        h1D_detectorNS_vertexZ_vecH[step] -> GetXaxis() -> SetTitle(Form("MBDz_H, step_%d, id_%d", step, index));
        h1D_detectorNS_vertexZ_vecH[step] -> Draw("hist");
        fit_gaus_mapH[step] -> Draw("lsame");

        i++;
    }

    if (index == 0) {
        c2 -> Print(Form("%s/h1D_MBDNS_vtxZ_trial.pdf(", final_output_directory.c_str()));
        c2 -> Clear();
        c2 -> Divide(chosen_scan_steps.size(), 2);
    }
    else if (index == total_trials - 1) {
        c2 -> Print(Form("%s/h1D_MBDNS_vtxZ_trial.pdf)", final_output_directory.c_str()));
        c2 -> Clear();
        c2 -> Divide(chosen_scan_steps.size(), 2);
    }
    else {
        c2 -> Print(Form("%s/h1D_MBDNS_vtxZ_trial.pdf", final_output_directory.c_str()));
        c2 -> Clear();
        c2 -> Divide(chosen_scan_steps.size(), 2);
    }
}

void MBD_zvtx_effi::ClearUp()
{
    for (TH1F * h1D : h1D_detectorNS_vertexZ_vecV)
    {
        h1D -> Reset("ICESM");
    }
    
    for (TH1F * h1D : h1D_detectorNS_vertexZ_vecH)
    {
        h1D -> Reset("ICESM");
    }
}

void MBD_zvtx_effi::DrawPlots(int selection)
{
    // note : ============================ ============================ ============================ ============================ ============================
    if (detector_selection == "MBDNS" && selection == 1)
    {
        c1 -> cd();
        c1 -> Print(Form("%s/h1D_MBDNS_vtxZ_vecV.pdf(", final_output_directory.c_str()));
        for(int i = 0; i < h1D_detectorNS_vertexZ_vecV.size(); i++)
        {
            h1D_detectorNS_vertexZ_vecV[i] -> Draw("hist");
            c1 -> Print(Form("%s/h1D_MBDNS_vtxZ_vecV.pdf", final_output_directory.c_str()));
            c1 -> Clear();
        }
        c1 -> Print(Form("%s/h1D_MBDNS_vtxZ_vecV.pdf)", final_output_directory.c_str()));
        c1 -> Clear();

        c1 -> cd();
        c1 -> Print(Form("%s/h1D_MBDNS_vtxZ_vecH.pdf(", final_output_directory.c_str()));
        for(int i = 0; i < h1D_detectorNS_vertexZ_vecH.size(); i++)
        {
            h1D_detectorNS_vertexZ_vecH[i] -> Draw("hist");
            c1 -> Print(Form("%s/h1D_MBDNS_vtxZ_vecH.pdf", final_output_directory.c_str()));
            c1 -> Clear();
        }
        c1 -> Print(Form("%s/h1D_MBDNS_vtxZ_vecH.pdf)", final_output_directory.c_str()));
        c1 -> Clear();



        TCanvas * c2 = new TCanvas("c2","c2",1500,800);
        c2 -> Divide(4,3);
        for (int i = 0; i < h1D_detectorNS_vertexZ_vecV.size(); i++)
        {
            c2 -> cd(i+1);
            h1D_detectorNS_vertexZ_vecV[i] -> Draw("hist");
        }
        c2 -> Print(Form("%s/h1D_detectorNS_vertexZ_vecV_grid.pdf", final_output_directory.c_str()));
        c2 -> Clear();

        c2 -> Divide(4,3);
        for (int i = 0; i < h1D_detectorNS_vertexZ_vecH.size(); i++)
        {
            c2 -> cd(i+1);
            h1D_detectorNS_vertexZ_vecH[i] -> Draw("hist");
        }
        c2 -> Print(Form("%s/h1D_detectorNS_vertexZ_vecH_grid.pdf", final_output_directory.c_str()));
        c2 -> Clear();
    }

    if (selection == 2)
    {
        c1 -> cd();
        int index = 0;
        
        for (int step : chosen_scan_steps) {
            if (index == 0) {
                // Rchi2_TrialStepV[step] -> GetYaxis() -> SetRangeUser(0, 25);
                Rchi2_TrialStepV[step] -> Draw("ap");
                Rchi2_TrialStepH[step] -> Draw("psame");
            } 
            else {
                Rchi2_TrialStepV[step] -> Draw("psame");
                Rchi2_TrialStepH[step] -> Draw("psame");
            }

            index += 1;
        }

        c1 -> Print( Form("%s/Rchi2_TrialStep.pdf", final_output_directory.c_str()) );
        c1 -> Clear();
        index = 0;


        for (int step : chosen_scan_steps) {
            Rchi2_TrialStepV[step] -> Draw("ap");
            c1 -> Print( Form("%s/test_Rchi2_TrialStepV_%d.pdf", final_output_directory.c_str(), step) );
            c1 -> Clear();

        }

        for (int step : chosen_scan_steps) {
            Rchi2_TrialStepH[step] -> Draw("ap");
            c1 -> Print( Form("%s/test_Rchi2_TrialStepH_%d.pdf", final_output_directory.c_str(), step) );
            c1 -> Clear();

        }        

        for (int step : chosen_scan_steps) {
            if (index == 0) {
                WidthFitError_TrialStepV[step] -> Draw("ap");
                WidthFitError_TrialStepH[step] -> Draw("psame");
            } 
            else {
                WidthFitError_TrialStepV[step] -> Draw("psame");
                WidthFitError_TrialStepH[step] -> Draw("psame");
            }

            index += 1;
        }

        c1 -> Print(Form("%s/WidthFitError_TrialStep.pdf", final_output_directory.c_str()));
        c1 -> Clear();
        index = 0;



        for (int step : chosen_scan_steps) {
            if (index == 0) {
                MeanFitError_TrialStepV[step] -> Draw("ap");
                MeanFitError_TrialStepH[step] -> Draw("psame");
            } 
            else {
                MeanFitError_TrialStepV[step] -> Draw("psame");
                MeanFitError_TrialStepH[step] -> Draw("psame");
            }

            index += 1;
        }

        c1 -> Print(Form("%s/MeanFitError_TrialStep.pdf", final_output_directory.c_str()));
        c1 -> Clear();
        index = 0;
    }
}

template <typename T>
// typename std::enabel_if<std::is_base_of<TGraph, T>::value || std::is_base_of<TGraphErrors, T>::value, double>::type
pair<double, double> MBD_zvtx_effi::FindLowestEntry_gr(T * gr)
{
    double min;
    double min_location;
    for (int i = 0; i < gr->GetN(); i++)
    {
        if (i == 0) {
            min = gr->GetPointY(i);
            min_location = gr->GetPointX(i);
        }
        else 
        {
            if (gr->GetPointY(i) < min) {
                min = gr->GetPointY(i);
                min_location = gr->GetPointX(i);
            }
        }
    }
    return {min_location, min};
}

template std::pair<double, double> MBD_zvtx_effi::FindLowestEntry_gr<TGraph>(TGraph *gr);