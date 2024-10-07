#ifndef GL1_SCALER_ANA_H
#define GL1_SCALER_ANA_H

#include <iostream>
#include <iomanip>
#include <numeric>
#include <fstream>
#include <sstream>


#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TAxis.h>
#include <TString.h>
#include <TF1.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLine.h>
#include <TH1F.h>
#include <TGaxis.h>

#include "sPhenixStyle.h"

#include <stdlib.h>
using namespace std;

class gl1_scaler_ana
{
    public:
        gl1_scaler_ana(
            string input_directory_in, 
            string input_filename_in, 
            string output_directory_in, 
            vector<pair<int,int>> range_t_V_in,
            vector<pair<int,int>> range_t_H_in,
            bool NCollision_corr_in, 
            bool beam_intensity_corr_in,
            bool accidental_correction_in,
            bool use_set_pos_in = false,
            bool MBD_zvtx_effi_in = false,
            bool Angelika_rate_tag_in = false
        );
        void SetAngelika_rate(bool Angelika_rate_tag_in) {Angelika_rate_tag = Angelika_rate_tag_in;}
        void PrepareRate(string input_file_directory = "null");
        void OutputRawRate(string output_file_directory);
        void SetDetectorName(string detector_name);
        void SetDemoFactor(pair<double,double> demo_factor_in) {demo_factor = demo_factor_in;}
        void ImportCADReadings(string cad_reading_directory, bool SD_column = false);
        void ImportAngelikaRates(string Angelika_file_directory, string scan_direction_string, string detector_string = "ZDC");
        void MakeCorrelationAngelika();
        std::pair<TGraphErrors *, TGraphErrors *> CombineMacro(string detector_name, bool only_raw_rate_tag_in = false, string suffix_string = "");
        void SetNcollision_corr(bool NCollision_corr_in) {NCollision_corr = NCollision_corr_in;}
        void SetBeamIntensity_corr(bool beam_intensity_corr_in) {beam_intensity_corr = beam_intensity_corr_in;}
        void SetMBDvtxZEffiFunc(TF1* MBD_vtxZ_effi_func_in);
        void PrepareData();
        void CombineData(); // note : combine BPM and detector rate
        virtual void DrawPlots();
        void SaveHistROOT();
        virtual void ClearUp();
        static void PrintInfo(string scan_direction_string, vector<pair<string, TGraphErrors *>> vector_in, int column_size = 10);
        void CalculateMachineLumi();
        void CalculateDetectorCrossSection();
        pair<double, double> GetOverlapWidths();
        double GetDetectorCrossSection();
        void GetInformation();
        void SetSphLabelingPreliminary(bool preliminary_tag) {if (preliminary_tag) {sPH_labeling = "Preliminary";}}
        void SetShowEBarTag(bool show_eBar_tag_in) {show_eBar_tag = show_eBar_tag_in;}

    protected:
        // static double FindHighestEntry_grE(TGraphErrors * gr);
        // static double FindHighestEntry_gr(TGraph * gr);
        template <typename T> static pair<double, double> FindHighestEntry_gr(T * gr);
        static double tg_average(TGraph * input_tgraph);
        static double tg_stddev(TGraph * input_tgraph);

        void PrepareData_subfunc(
            string direction_string,
            vector<pair<int,int>> &range_t_All, 
            map<int, pair<long long, long long>> &step_selected_T_range_All,
            map<int, pair<long long, long long>> &step_counting_range_All,
            vector<TGraphErrors *> &gr_raw_detectorNS_rate_vec_All,
            vector<TGraph *> &gr_raw_detectorNS_rate_vec_All_wide,
            vector<TH1F *> &h1D_raw_detectorNS_rate_vec_All,
            vector<TF1 *> &fit_vec_All,
            vector<double> &DetectorNS_rate_avg_vec_All,
            vector<double> &DetectorNS_rate_avg_vec_All_error,
            vector<pair<double,double>> &outlier_rejection_factor_vec_All,
            vector<double> &multi_collision_correction_All,
            vector<TF1 *> &fit_gaus_vec_All,
            vector<double> &accidental_correction_All,
            vector<TH1F *> &h1D_detectorNS_vertexZ_vecAll,
            vector<TH1F *> &h1D_detectorNS_vertexZ_postCorr_vecAll,
            vector<double> &detectorNS_zvtx_effi_correction_All
        );

        void PrepareMBDvtxZEffi(
            string direction_string,
            vector<TH1F *> &h1D_detectorNS_vertexZ_vecAll,
            vector<TH1F *> &h1D_detectorNS_vertexZ_postCorr_vecAll,
            vector<double> &detectorNS_zvtx_effi_correction_All
        );

        // note : variables for the constructor
        string input_directory;
        string input_filename;
        string output_directory;
        string final_output_directory;
        bool NCollision_corr;
        bool beam_intensity_corr;
        bool accidental_correction;
        bool use_set_pos;
        bool MBD_zvtx_effi;
        string detector_selection;

        string RawRate_Xaxis_title;

        bool show_eBar_tag;

        TCanvas * c1;
        TFile * file_in;
        TTree * tree;
        long long evtID;
        long long evtBCO_gl1;
        int bunchnumber;
        long long GTMBusyVector_Decimal;
        vector<int> *live_trigger_vec;
        long long LiveTrigger_Decimal;
        map<int, int> live_trigger_map;
        double mbd_z_vtx;
        double zdc_z_vtx;
        string sPH_labeling;
        TLatex * ltx;

        long long GL1Scalers_clock_raw; // note : 0
        long long GL1Scalers_clock_live;
        long long GL1Scalers_clock_scaled;

        long long GL1Scalers_ZDCS_raw; // note : 1
        long long GL1Scalers_ZDCS_live;
        long long GL1Scalers_ZDCS_scaled;
        long long GL1Scalers_ZDCN_raw; // note : 2
        long long GL1Scalers_ZDCN_live;
        long long GL1Scalers_ZDCN_scaled;
        long long GL1Scalers_ZDCNS_raw; // note : 3
        long long GL1Scalers_ZDCNS_live;
        long long GL1Scalers_ZDCNS_scaled;
        

        long long GL1Scalers_MBDS_raw; // note : 8
        long long GL1Scalers_MBDS_live;
        long long GL1Scalers_MBDS_scaled;
        long long GL1Scalers_MBDN_raw; // note : 9
        long long GL1Scalers_MBDN_live;
        long long GL1Scalers_MBDN_scaled;
        long long GL1Scalers_MBDNS_raw; // note : 10
        long long GL1Scalers_MBDNS_live;
        long long GL1Scalers_MBDNS_scaled;

        bool Angelika_rate_tag;
        vector<float> Angelika_rate_V;
        vector<float> Angelika_rate_H;
        vector<float> Angelika_time_step_vecH;
        vector<float> Angelika_ZDCNS_rate_vecH;
        vector<float> Angelika_ZDCN_rate_vecH;
        vector<float> Angelika_ZDCS_rate_vecH;
        vector<float> Angelika_time_step_vecV;
        vector<float> Angelika_ZDCNS_rate_vecV;
        vector<float> Angelika_ZDCN_rate_vecV;
        vector<float> Angelika_ZDCS_rate_vecV;
        TGraph * Angelika_vertical_correlation = nullptr;
        TGraph * Angelika_horizontal_correlation = nullptr;
        TF1 * unity_line;

        
        TF1 * MBD_vtxZ_effi_func = nullptr;

        bool pre_good_evt;
        bool this_good_evt;
        int n_entries;

        const double n_bunches = 111;
        const double n_bunches_space = 120;
        const double bco_span = 106 * pow(10, -9);
        const int width_range_time = 40; // note : unit, second, this is for the display
        const int MBDNS_inclusive_ID = 10; // note : this is the id of the trigger
        const int MBDNS_30cm_ID = 13;     // note : this is the id of the trigger
        const int ZDCNS_inclusive_ID = 3; // note : this is the id of the trigger

        int global_ana_counting = 0;


        // note : used in the PrepareRate
        map<int, vector<long long>> time_ZDCS_raw_counting;
        map<int, vector<long long>> time_ZDCN_raw_counting;
        map<int, vector<long long>> time_ZDCNS_raw_counting; // note : the ZDCNS counting in every second, the second is given by the GL1 clock
        map<int, vector<long long>> time_MBDS_raw_counting;
        map<int, vector<long long>> time_MBDN_raw_counting;
        map<int, vector<long long>> time_MBDNS_raw_counting; // note : the MBDNS counting in every second, the second is given by the GL1 clock
        map<int, pair<int, int>>    time_MBDNS_30cm_raw_counting_pair;
        map<int, vector<double>>    time_MBDNS_zvtx; // note : this one keeps the MBDNS zvtx readings in every second, every second has one vector
        map<int, vector<double>>    time_ZDCNS_zvtx; // note : this one keeps the ZDCNS zvtx readings in every second, every second has one vector
        map<int, vector<long long>>     time_LiveTrigger_Decimal; // note : this one keeps the LiveTrigger readings in every second, every second has one vector
        map<int, pair<long long, long long>> time_GL1Scalers_range; // note : the range of every second

        // note : the copied one, for switch
        map<int, vector<long long>> time_detectorNS_raw_counting; // note : the detectorNS to be filled by the above two

        
        // note : for the correction of multiple collision
        map<int, pair<long long, long long>> step_selected_T_range_V;
        map<int, pair<long long, long long>> step_counting_range_V;
        map<int, pair<long long, long long>> step_selected_T_range_H;
        map<int, pair<long long, long long>> step_counting_range_H;
        // note : maybe mainly for MBD
        vector<double> multi_collision_correction_V;
        vector<double> multi_collision_correction_H;
        
        // note : maybe mainly for the MBD vertex Z effi.
        vector<double> detectorNS_zvtx_effi_correction_V;
        vector<double> detectorNS_zvtx_effi_correction_H;

        // note : maybe mainly for ZDC
        vector<double> accidental_correction_V;
        vector<double> accidental_correction_H;

        // note : Y
        vector<double> Average_BPM_pos_V;
        vector<double> StdDev_BPM_pos_V;
        vector<double> BeamIntensity_corr_V;
        vector<pair<int,int>> range_t_V;
        vector<double> DetectorNS_rate_avg_vecV;
        vector<double> DetectorNS_rate_avg_vecV_error;

        // note : X
        vector<double> Average_BPM_pos_H;
        vector<double> StdDev_BPM_pos_H;
        vector<double> BeamIntensity_corr_H;
        vector<pair<int,int>> range_t_H;
        vector<double> DetectorNS_rate_avg_vecH;
        vector<double> DetectorNS_rate_avg_vecH_error;

        TGraph * gr_raw_ZDCS_rate;
        TGraph * gr_raw_ZDCN_rate;
        TGraph * gr_raw_ZDCNS_rate;
        TGraph * gr_raw_MBDS_rate;
        TGraph * gr_raw_MBDN_rate;
        TGraph * gr_raw_MBDNS_rate;
        TGraph * gr_raw_MBDNS_30cm_ratio;

        vector<TH1F *> h1D_raw_detectorNS_rate_vecV;
        vector<TH1F *> h1D_raw_detectorNS_rate_vecH;
        vector<TGraphErrors *> gr_raw_detectorNS_rate_vecV;
        vector<TGraphErrors *> gr_raw_detectorNS_rate_vecH;
        vector<TGraph *> gr_raw_detectorNS_rate_vecV_wide;
        vector<TGraph *> gr_raw_detectorNS_rate_vecH_wide;
        vector<TH1F *> h1D_detectorNS_vertexZ_vecV;
        vector<TH1F *> h1D_detectorNS_vertexZ_vecH;
        vector<TH1F *> h1D_detectorNS_vertexZ_postCorr_vecV;
        vector<TH1F *> h1D_detectorNS_vertexZ_postCorr_vecH;
        double search_window = 5; // note : this is for the weird second rejection
        double outlier_rejection_factor = 2; // note : this is for the weird second rejection
        vector<pair<double, double>> outlier_rejection_factor_vecV;
        vector<pair<double, double>> outlier_rejection_factor_vecH;
        vector<TF1 *> fit_vecV;
        vector<TF1 *> fit_vecH;
        vector<TF1 *> fit_gaus_vecV;
        vector<TF1 *> fit_gaus_vecH;
        TLatex * draw_text;
        TLegend * legend;
        TLine * coord_line;

        pair<double,double> demo_factor;
        TGraphErrors * gr_BPM_raw_detectorNS_rate_V;
        TGraphErrors * gr_BPM_raw_detectorNS_rate_V_demo;
        TGraphErrors * gr_BPM_raw_detectorNS_rate_H;
        TGraphErrors * gr_BPM_raw_detectorNS_rate_H_demo;
        TF1 * fit_Gaus_V;
        TF1 * fit_Gaus_H;

        bool only_raw_rate_tag;

        string detector_pool[6] = {"ZDCS", "ZDCN", "ZDCNS", "MBDS", "MBDN", "MBDNS"};
        vector<double> BPM_set_pos_V = { // note : according to the second vernier scan, the run 51195
            0.0, 0.1, 0.25, 0.4, 0.6, 0.9,
            0.0, -0.1, -0.25, -0.4, -0.6, -0.9
        };
        vector<double> BPM_set_pos_H = { // note : according to the second vernier scan, the run 51195
            0.0, 0.1, 0.25, 0.4, 0.6, 0.9,
            0.0, -0.1, -0.25, -0.4, -0.6, -0.9
        };
        double nominal_width = 0.26; // note : unit : mm

        bool BPM_StdDev;

        // note : for calculate the machine luminosity, and the detector cross section
        double um2_to_mb = 1.0e-19;
        double fbeam = 78.4 * 1000; // note : 78 kHz
        double Rmax_avg_single_bunch;
        double machine_lumi;
        double detector_cross_section;
        double DCCT_B; // note : the average beam intensity of the BLUE beam
        double DCCT_Y; // note : the average beam intensity of the YELLOW beam
        


    private:
        static double gaus_func(double *x, double *par)
        {
            // note : par[0] : size
            // note : par[1] : mean
            // note : par[2] : width
            // note : par[3] : offset
            return par[0] * TMath::Gaus(x[0],par[1],par[2]) + par[3];
        }

        static void quick_tgraph_set(TGraph * graph_in, string x_title, string y_title, int color = -999, int marker_style = -999, double marker_size = -999)
        {
            graph_in -> GetXaxis() -> SetTitle(x_title.c_str());
            graph_in -> GetYaxis() -> SetTitle(y_title.c_str());
            graph_in -> GetXaxis() -> SetNdivisions(505);

            if (marker_style != -999) {graph_in -> SetMarkerStyle(marker_style);}
            if (color != -999) {graph_in -> SetMarkerColor(color);}
            if (marker_size != -999) {graph_in -> SetMarkerSize(marker_size);}
        }

        static string keep_alpha(string input_string)
        {
            string output_string = "";
            for (int i = 0; i < input_string.size(); i++) {
                if (isalpha(input_string[i])){
                    output_string += input_string[i];
                }
                
            }
            return output_string;
        }

};

#endif