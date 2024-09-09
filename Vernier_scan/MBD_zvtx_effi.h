#ifndef MBD_ZVTX_EFFI_H
#define MBD_ZVTX_EFFI_H

#include "gl1_scaler_ana.h"
#include <TGaxis.h>
#include <TF1.h>
#include <bitset>

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

        void RunMacro();

        
        
        vector<TF1 *> GetEffiFitFuncSeparate();
        vector<TF1 *> GetEffiFitFuncComb();
        void DrawPlots() override;
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
        std::vector<int> prepare_trigger_vec(long long trigger_input);
        void Make_detector_vtxZ(
            string direction_string,
            vector<pair<int,int>> &range_t_All, 
            map<string, vector<TH1F *>> &h1D_detectorNS_vtxZ_mapAll
        );

        void DeriveMBDEffi(
            map<string, vector<TH1F *>> &h1D_detectorNS_vtxZ_mapAll,
            map<string, vector<TF1 *>> &effi_fitting_vecAll
        );

        void PrepareCombine();

        string input_rate_file;
        vector<int> chosen_scan_steps; 

        map<string, vector<TH1F *>> h1D_detectorNS_vtxZ_mapV;
        map<string, vector<TH1F *>> h1D_detectorNS_vtxZ_mapH;
        map<string, vector<TF1 *>> effi_fitting_vecV;
        map<string, vector<TF1 *>> effi_fitting_vecH;

        // note : for the combined case
        TH1F * combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx;
        TH1F * combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig;
        TH1F * combined_h1D_ZDCNSVtxZ_ZDCNSTrig;
        TH1F * combined_h1D_effi;
        TH1F * combined_h1D_effi2;
        TF1 *  combined_effi_fitting;
        TF1 *  combined_effi_fitting2;
        
        pair<double, double> general_hist_range;
        int general_Nbins;

        map<int, int> LiveTrigger_map;

        TCanvas * c2;
        TCanvas * c3;

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