#ifndef MBDNSCOUNT_H
#define MBDNSCOUNT_H

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
#include <TH1D.h>

// #include "sPhenixStyle.h"

#include <stdlib.h>
using namespace std;

class MBDNSCount{
    public:
        MBDNSCount(
            string input_directory_in, 
            string input_filename_in, 
            string output_directory_in, 
            vector<pair<int,int>> range_t_V_in,
            vector<pair<int,int>> range_t_H_in
        );

        void ImportCADReadings(string cad_reading_directory, bool SD_column = false);

    protected:

        std::vector<TH1D*> h1D_Y_vec;
        std::vector<TH1D*> h1D_X_vec;


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
};

#endif