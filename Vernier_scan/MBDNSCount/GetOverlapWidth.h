#ifndef GETOVERLAPWIDTH_H
#define GETOVERLAPWIDTH_H

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

// #include "sPhenixStyle.h"

#include <stdlib.h>
using namespace std;

class GetOverlapWidth {
    public:
        GetOverlapWidth();
        void ImportCADReadings(string cad_reading_directory, bool SD_column);

        // note : for bunches/all        
        void GetAllWidth(
            int bunch_number,
            std::pair<double, double> MBDS_raw_counting_pair,
            std::pair<double, double> MBDN_raw_counting_pair,
            std::pair<double, double> MBDNS_raw_counting_pair,
            std::pair<double, double> clock_raw_pair,

            std::pair<double, double> MBDS_live_counting_pair,
            std::pair<double, double> MBDN_live_counting_pair,
            std::pair<double, double> MBDNS_live_counting_pair,
            std::pair<double, double> clock_live_pair
        );

        // note : two methods, two directions, bunches/all -> each: one TGraph, one TF1

    protected:

        double Sasha_stable_approach(double *x, double *par){
            double ks = par[0];
            double kn = par[1];

            return 1 - TMath::Exp(-1 * x[0] * (1+ks)) - TMath::Exp(-1 * x[0] * (1+kn)) + TMath::Exp(-1 * x[0] * (1+ks+kn));

            // note : p[0] + p[1]*(x-p[3])+p[2] * (x-p[3])^2
        };

        double gaus_func(double *x, double *par){
            // note : par[0] : size
            // note : par[1] : mean
            // note : par[2] : width
            // note : par[3] : offset
            return par[0] * TMath::Gaus(x[0],par[1],par[2]) + par[3];
        };


        // note : Y
        std::vector<double> Average_BPM_pos_V;
        std::vector<double> StdDev_BPM_pos_V;
        std::vector<double> BeamIntensity_corr_V;

        // note : X
        std::vector<double> Average_BPM_pos_H;
        std::vector<double> StdDev_BPM_pos_H;
        std::vector<double> BeamIntensity_corr_H;

        std::vector<double> BPM_set_pos_V = { // note : according to the second vernier scan, the run 51195
            0.0, 0.1, 0.25, 0.4, 0.6, 0.9,
            0.0, -0.1, -0.25, -0.4, -0.6, -0.9
        };
        std::vector<double> BPM_set_pos_H = { // note : according to the second vernier scan, the run 51195
            0.0, 0.1, 0.25, 0.4, 0.6, 0.9,
            0.0, -0.1, -0.25, -0.4, -0.6, -0.9
        };

        std::vector<std::pair<int,int>> range_t_V = {
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

        std::vector<std::pair<int,int>> range_t_H = {
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

        const double n_bunches = 111;
        const double n_bunches_space = 120;


};

#endif