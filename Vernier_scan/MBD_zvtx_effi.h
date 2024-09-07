#ifndef MBD_ZVTX_EFFI_H
#define MBD_ZVTX_EFFI_H

#include "gl1_scaler_ana.h"
#include <TGaxis.h>
#include <TF1.h>

class MBD_zvtx_effi : public gl1_scaler_ana
{
    public:
        MBD_zvtx_effi(
            string input_file_directory, 
            string input_filename_in, 
            string input_rate_file_in,
            string output_directory_in,
            vector<pair<int,int>> range_t_V_in,
            vector<pair<int,int>> range_t_H_in
        );

        void MBD_vtxZ_macro(int total_trials = 100, bool run_simple = true);

        void MakeMBD_vertexZ_hist_func(
            vector<pair<int,int>> &range_t_All, 
            vector<TH1F *> &h1D_detectorNS_vertexZ_vecAll,
            map<int, TF1 *> &fit_gaus_map_all,
            TF1 * MBD_z_effi, // note : Gaussian distribution describing the vertex Z efficiency resolution
            bool apply_effi_corr = false
        );
        
        vector<TGraph *> GetRchi2_TGraph();

        void DrawPlots(int selection = 1);
        void ClearUp() override;

        static double negative_pol2(double *x, double *par)
        {
            return -1 * pow( par[0] * (x[0] + par[1]) ,2) + 1;
        }

        static double pure_gaus_func(double *x, double *par)
        {
            // note : par[0] : mean
            // note : par[1] : width
            return TMath::Gaus(x[0],par[0],par[1]);
        }

        template <typename T> static pair<double, double> FindLowestEntry_gr(T * gr);

    protected:
        string input_rate_file;
        vector<int> chosen_scan_steps;

        TCanvas * c2;
        void DrawTrialPlots(int index, int total_trials);
        
        map<int, TF1 *> fit_gaus_mapH;
        map<int, TF1 *> fit_gaus_mapV;

        map<int, TGraph *> Rchi2_TrialStepV;
        map<int, TGraph *> WidthFitError_TrialStepV;
        map<int, TGraph *> MeanFitError_TrialStepV;

        map<int, TGraph *> Rchi2_TrialStepH;
        map<int, TGraph *> WidthFitError_TrialStepH;
        map<int, TGraph *> MeanFitError_TrialStepH;

        vector<string> color_code = {"#167288", "#8cdaec", "#b45248", "#d48c84", "#a89a49", "#d6cfa2", "#3cb464", "#9bddb1", "#643c6a", "#836394"};
    
    private:
        static double gaus_func_nohead(double *x, double *par)
        {
            if (x[0] > -30 && x[0] < 30){
                TF1::RejectPoint();
                return 0;
            }

            // note : par[0] : size
            // note : par[1] : mean
            // note : par[2] : width
            // note : par[3] : offset
            return par[0] * TMath::Gaus(x[0],par[1],par[2]) + par[3];
        }

        

};

#endif