// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef FORWARDCALONTUPLIZER_H
#define FORWARDCALONTUPLIZER_H

#include <fun4all/SubsysReco.h>
#include <globalvertex/MbdVertex.h>
#include <globalvertex/MbdVertexMapv1.h>

#include <cmath>
#include <map>
#include <string>
#include <vector>
#include <numeric>

#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TString.h>    // note : for Form
#include <TCanvas.h>

class CaloWaveformFitting;
class TowerInfoContainer;
class PHCompositeNode;
class Gl1Packet;
class CaloPacketContainer;
class CaloPacket;

class MbdVertex;
class MbdVertexMap;

class TH1;
class TH2;
class TTree;
class TCanvas;
class TString;


class ForwardCaloNtuplizer : public SubsysReco
{
    public:
        ForwardCaloNtuplizer(
            const std::string &name = "ForwardCaloNtuplizer", 
            const std::string output_directory_in = "/sphenix/tg/tg01/coldqcd/cwshih/test/test_run_48082",
            const std::string file_out_name_in = "output.root",
            const bool &with_waveform_in = true,
            const bool &get_mbd_z_in = true
        );

        struct FC_unit {
            int ch;
            std::string name;
            std::vector<double> waveform;
            double adc;
            std::pair<int,int> sig_T_window;
            double pos;
        };

        struct GL1Scaler_unit {
            int index;
            std::string name;
            long long raw;
            long long live;
            long long scaled;
        };

        ~ForwardCaloNtuplizer() override;

        /** Called during initialization.
            Typically this is where you can book histograms, and e.g.
            register them to Fun4AllServer(so they can be output to file
            using Fun4AllServer::dumpHistos() method).
        */
        int Init(PHCompositeNode *topNode) override;

        /** Called for first event when run number is known.
            Typically this is where you may want to fetch data from
            database, because you know the run number. A place
            to book histograms which have to know the run number.
        */
        int InitRun(PHCompositeNode *topNode) override;

        /** Called for each event.
            This is where you do the real work.
        */
        int process_event(PHCompositeNode *topNode) override;

        /// Clean up internals after each event.
        int ResetEvent(PHCompositeNode *topNode) override;

        /// Called at the end of each run.
        int EndRun(const int runnumber) override;

        /// Called at the end of all processing.
        int End(PHCompositeNode *topNode) override;

        /// Reset
        int Reset(PHCompositeNode * /*topNode*/) override;

        void Print(const std::string &what = "ALL") const override;

        // note : self added functions
        void Init_FC_unit_map();

        void clear_tree_vec();

        void clear_FC_unit_map();

    protected: 
        TFile * file_out;
        TTree * tree_out;
        std::string output_directory;
        std::string file_out_name;
        bool with_waveform;
        bool get_mbd_z;

        CaloWaveformFitting *WaveformProcessingFast = nullptr;
        Gl1Packet *p_gl1;
        CaloPacketContainer *zdc_cont;

        MbdVertexMapv1 *m_mbdvtxmap = nullptr;
        MbdVertex *m_mbdvtx = nullptr;

        std::vector<float> anaWaveformFast(CaloPacket *p, const int channel);
        static std::vector<int> prepare_trigger_vec(long long trigger_input);
        void prepare_mbd_z(PHCompositeNode *topNode);

    private:

        long long evtID;
        long long evtBCO_gl1;
        long long evtBCO_zdc;
        int bunchnumber;

        long long GTMBusyVector_Decimal;
        long long trigger_input_decimal;
        long long live_trigger_decimal;
        long long scaled_trigger_decimal;

        double mbd_z_vtx;

        std::vector<int> trigger_input_vec;
        std::vector<int> live_trigger_vec;
        std::vector<int> scaled_trigger_vec;

        std::map<int, GL1Scaler_unit> GL1Scalers_map;
        // note : for the run 48029, the trigger_input_channel 5 is HCal_coincidence, but it's not used, so the reading is the same as the trigger_input_channel 0, the clock
        // note : for the GL1P, we have HCAL_coincidence to pretend to be the clock
        // long long GL1Scalers5_raw;
        // long long GL1Scalers5_live;
        // long long GL1Scalers5_scaled;

        // note : the ZDC_S trigger channel
        // long long GL1Scalers1_raw;
        // long long GL1Scalers1_live;
        // long long GL1Scalers1_scaled;

        // note : the ZDC_N trigger channel
        // long long GL1Scalers2_raw;
        // long long GL1Scalers2_live;
        // long long GL1Scalers2_scaled;

        // note : the ZDC_coincidence trigger channel
        // long long GL1Scalers3_raw;
        // long long GL1Scalers3_live;
        // long long GL1Scalers3_scaled;

        // note : the MBDS coincidence trigger channel
        // long long GL1Scalers8_raw;
        // long long GL1Scalers8_live;
        // long long GL1Scalers8_scaled;
        
        // note : the MBDN coincidence trigger channel
        // long long GL1Scalers9_raw;
        // long long GL1Scalers9_live;
        // long long GL1Scalers9_scaled;

        // note : the MBDNS coincidence trigger channel
        // long long GL1Scalers10_raw;
        // long long GL1Scalers10_live;
        // long long GL1Scalers10_scaled;


        // note : the gl1p scalers, 120 bunches, and 9 channels of scalers
        std::vector<std::vector<long long>> GL1PScaler_raw_vec;
        std::vector<std::vector<long long>> GL1PScaler_live_vec;
        std::vector<std::vector<long long>> GL1PScaler_scaled_vec;

        std::map<long long, int> trigger_type;

        std::map<int, FC_unit> FC_unit_map;
        std::map<int, TH2F*> FC_unit_waveform2D_map;
        TH1F * bunchnumber_1D;
        std::string plot_filename;
        TCanvas * c1;

        double zdcS1_adc;
        double zdcS2_adc;
        double zdcS3_adc;

        double zdcN1_adc;
        double zdcN2_adc;
        double zdcN3_adc;

        double vetoNF_adc;
        double vetoNB_adc;
        double vetoSF_adc;
        double vetoSB_adc;

        std::vector<double> smdSH_adc;
        std::vector<double> smdSH_pos;
        std::vector<double> smdNH_adc;
        std::vector<double> smdNH_pos;
        
        std::vector<double> smdSV_adc;
        std::vector<double> smdSV_pos;
        std::vector<double> smdNV_adc;
        std::vector<double> smdNV_pos;

        // note : the channel map
        int zdcS1_ch = 0;
        int zdcS2_ch = 2;
        int zdcS3_ch = 4;

        int zdcN1_ch = 8;
        int zdcN2_ch = 10;
        int zdcN3_ch = 12;

        int vetoSF_ch = 80;
        int vetoSB_ch = 81;
        int vetoNF_ch = 16;
        int vetoNB_ch = 17;

        int smdSV0_ch = 112;
        int smdSV1_ch = 113;
        int smdSV2_ch = 114;
        int smdSV3_ch = 115;
        int smdSV4_ch = 116;
        int smdSV5_ch = 117;
        int smdSV6_ch = 118;
        int smdSV7_ch = 119;

        int smdSH0_ch = 120;
        int smdSH1_ch = 121;
        int smdSH2_ch = 122;
        int smdSH3_ch = 123;
        int smdSH4_ch = 124;
        int smdSH5_ch = 125;
        int smdSH6_ch = 126;
        
        int smdNV0_ch = 48;
        int smdNV1_ch = 49;
        int smdNV2_ch = 50;
        int smdNV3_ch = 51;
        int smdNV4_ch = 52;
        int smdNV5_ch = 53;
        int smdNV6_ch = 54;
        int smdNV7_ch = 55;
        
        int smdNH0_ch = 56;
        int smdNH1_ch = 57;
        int smdNH2_ch = 58;
        int smdNH3_ch = 59;
        int smdNH4_ch = 60;
        int smdNH5_ch = 61;
        int smdNH6_ch = 62;

        // note : the timing cut, may have to be optimized
        std::pair<int,int> ZDC_time_cut =  {4, 10};
        std::pair<int,int> veto_time_cut = {6, 11};
        std::pair<int,int> smdN_time_cut = {9, 1000};
        std::pair<int,int> smdS_time_cut = {7, 12};

        // note : from gl1p_scalers to gl1_scalers
        std::map<int,int> gl1p_gl1_scaler_map = {
            {0, 10},
            {1, 14},
            {2, 12},
            {3, 8},
            {4, 9},
            {5, 3},
            {6, 1},
            {7, 2},
            {8, 5}
        };

        // note : from gl1_scalers to gl1p_scalers
        std::map<int,int> gl1_gl1p_scaler_map = {
            {10, 0},
            {14, 1},
            {12, 2},
            {8, 3},
            {9, 4},
            {3, 5},
            {1, 6},
            {2, 7},
            {5, 8}
        };

};

#endif // FORWARDCALONTUPLIZER_H
