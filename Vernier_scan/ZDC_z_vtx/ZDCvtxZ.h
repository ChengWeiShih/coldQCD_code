#ifndef ZDCvtxZ_H
#define ZDCvtxZ_H

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
#include <TH2F.h>
#include <TGaxis.h>

#include "sPhenixStyle.h"

#include <stdlib.h>
using namespace std;

class ZDCvtxZ
{
    public:
        ZDCvtxZ(
            long long nRunEvent_in, 
            string input_full_directory_in = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform/ntuple_run51195_merged.root", 
            string output_directory_in = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform/quick_check",
            bool NS_offset_in = true
        );

        void RunEvent();
        void DrawPlots();
        void CloseRootFileOut();

    protected:

        long long nRunEvent;
        string input_full_directory;
        string output_directory;
        bool NS_offset;

        struct calo_struct{
            int index;
            string name;
            vector<double> *waveform;
            TH1F * hist;
            TF1 * fit;
            TF1 * fit2;
            TH1F * h1_waveform_peak;
            TH1F * h1_waveform_StdDev; // note : this is in height
            TH1F * h1_waveform_fit_width; // note : this is in width
            int status;
            int Ngroup;
            double peak_pos;
            double peak_height;
        };

        map<string, calo_struct> zdc_map;
        int ZDCNS_trigger_ID = 3;
        int MBDNS_trigger_ID = 10;

        
        TFile * file_in;
        TTree * tree;
        TFile * file_out;
        TTree * tree_out;


        TCanvas * c1;
        TCanvas * c2;
        
        TH1F * ZDC_vtxZ_charge;
        TH1F * ZDC_vtxZ_fit;
        TH1F * ZDC_vtxZ_num;
        TH1F * ZDC_vtxZ;
        
        TH2F * vtx_correlation;
        TH1F * vtx_diff;

        // note : for charge_discharge function
        TH2F * vtx_correlation_charge;
        TH1F * vtx_diff_charge;
        
        TH1F * MBD_vtxZ_dist_inclusive;
        TH1F * MBD_vtxZ_dist_MBDNSTrig;
        TH1F * MBD_vtxZ_dist_ZDCNSTrig;

        TLegend * legend;
        TLatex * draw_text;
        TLine * coord_line;

        long long evtID;
        long long evtBCO_gl1;
        int bunchnumber;
        vector<int> *live_trigger_vec = 0;
        double mbd_z_vtx;
        double z_ZDC_fit;
        double z_ZDC_charge;

        double ZDC_vtxZ_num_out; 
        double ZDC_vtxZ_fit_out;
        double ZDC_vtxZ_out;
        double mbd_vtxZ_out;
        int Ngood_waveform_S_out;
        int Ngood_waveform_N_out;

        map<int,int> live_trigger_map_evt;
        double baseline;

        void ClearEvent();

        static double land_offset(double *x, double *par)
        {
            // note : par[0] : size
            // note : par[1] : mpv
            // note : par[2] : width
            // note : par[3] : offset
            return (par[0]/0.178854) * TMath::Landau(x[0], par[1], par[2]) + par[3];
        }

        static double charge_discharge_func(double *x, double *par)
        {
            // note : par[0]: offset
            // note : par[1]: signal fired time
            // note : par[2]: charge tau
            // note : par[3]: discharge tau
            // note : par[4]: amplitude

            if (x[0] < par[1]){
                return par[0];
            }
            else {
                return par[0] + ( 1-TMath::Exp( -(x[0]-par[1])/par[2] ) ) * TMath::Exp( -(x[0]-par[1])/par[3] ) * par[4]/(0.3654103) * (par[2]/par[3]);
            }
        }


        double vector_stddev (vector <double> input_vector);
        double hist_stddev (TH1F * hist_in);
        double hist_bkgrm(TH1F * hist_in);
        vector<double> find_Ngroup(TH1F * hist_in, double baseline);
        pair<double, double> GetFitRangeHist(TH1F * hist_in);
        pair<double, double> GetNumZDCtime(TH1F * hist_in);
        bool ZDC_good(int peak1, int peak2, int peak3);


};

#endif