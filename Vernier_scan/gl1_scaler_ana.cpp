#include "gl1_scaler_ana.h"

gl1_scaler_ana::gl1_scaler_ana(
    string input_directory_in, 
    string input_filename_in, 
    string output_directory_in,
    vector<pair<int,int>> range_t_V_in,
    vector<pair<int,int>> range_t_H_in, 
    bool NCollision_corr_in, 
    bool beam_intensity_corr_in,
    bool accidental_correction_in,
    bool use_set_pos_in,
    bool MBD_zvtx_effi_in
)
: 
input_directory(input_directory_in), 
input_filename(input_filename_in), 
output_directory(output_directory_in), 
range_t_V(range_t_V_in),
range_t_H(range_t_H_in),
demo_factor({40., 80.}), 
NCollision_corr(NCollision_corr_in),
beam_intensity_corr(beam_intensity_corr_in),
accidental_correction(accidental_correction_in),
detector_selection("MBDNS"),
global_ana_counting(0),
BPM_StdDev(false),
use_set_pos(use_set_pos_in),
MBD_zvtx_effi(MBD_zvtx_effi_in)
{
    cout<<"============================================================================================================"<<endl;
    cout<<"============================================================================================================"<<endl;
    cout<<"_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~_~"<<endl;
    cout<<"============================================================================================================"<<endl;

    live_trigger_vec = 0;

    file_in = TFile::Open((input_directory + "/" + input_filename).c_str());
    tree = (TTree*)file_in->Get("tree");
    tree->SetBranchStatus("*", 0);
    tree->SetBranchStatus("evtID", 1);
    tree->SetBranchStatus("evtBCO_gl1", 1);
    tree->SetBranchStatus("bunchnumber", 1);
    tree->SetBranchStatus("LiveTrigger_Vec", 1);
    tree->SetBranchStatus("GTMBusyVector_Decimal", 1);
    tree->SetBranchStatus("GL1Scalers_clock_raw", 1);
    tree->SetBranchStatus("GL1Scalers_clock_live", 1);
    tree->SetBranchStatus("GL1Scalers_clock_scaled", 1);
    tree->SetBranchStatus("GL1Scalers_ZDCS_raw", 1);
    tree->SetBranchStatus("GL1Scalers_ZDCS_live", 1);
    tree->SetBranchStatus("GL1Scalers_ZDCS_scaled", 1);
    tree->SetBranchStatus("GL1Scalers_ZDCN_raw", 1);
    tree->SetBranchStatus("GL1Scalers_ZDCN_live", 1);
    tree->SetBranchStatus("GL1Scalers_ZDCN_scaled", 1);
    tree->SetBranchStatus("GL1Scalers_ZDCNS_raw", 1);
    tree->SetBranchStatus("GL1Scalers_ZDCNS_live", 1);
    tree->SetBranchStatus("GL1Scalers_ZDCNS_scaled", 1);
    tree->SetBranchStatus("GL1Scalers_MBDS_raw", 1);
    tree->SetBranchStatus("GL1Scalers_MBDS_live", 1);
    tree->SetBranchStatus("GL1Scalers_MBDS_scaled", 1);
    tree->SetBranchStatus("GL1Scalers_MBDN_raw", 1);
    tree->SetBranchStatus("GL1Scalers_MBDN_live", 1);
    tree->SetBranchStatus("GL1Scalers_MBDN_scaled", 1);
    tree->SetBranchStatus("GL1Scalers_MBDNS_raw", 1);
    tree->SetBranchStatus("GL1Scalers_MBDNS_live", 1);
    tree->SetBranchStatus("GL1Scalers_MBDNS_scaled", 1);
    tree->SetBranchStatus("LiveTrigger_Decimal", 1);
    tree->SetBranchStatus("mbd_z_vtx",  1);
    tree->SetBranchStatus("zdc_z_vtx", 1);

    tree->SetBranchAddress("evtID", &evtID);
    tree->SetBranchAddress("evtBCO_gl1", &evtBCO_gl1);
    tree->SetBranchAddress("bunchnumber", &bunchnumber);
    tree->SetBranchAddress("GTMBusyVector_Decimal", &GTMBusyVector_Decimal);
    tree->SetBranchAddress("LiveTrigger_Vec", &live_trigger_vec);
    tree->SetBranchAddress("LiveTrigger_Decimal", &LiveTrigger_Decimal);

    tree->SetBranchAddress("mbd_z_vtx", &mbd_z_vtx);
    tree->SetBranchAddress("zdc_z_vtx", &zdc_z_vtx);

    tree->SetBranchAddress("GL1Scalers_clock_raw", &GL1Scalers_clock_raw);
    tree->SetBranchAddress("GL1Scalers_clock_live", &GL1Scalers_clock_live);
    tree->SetBranchAddress("GL1Scalers_clock_scaled", &GL1Scalers_clock_scaled);

    tree->SetBranchAddress("GL1Scalers_ZDCS_raw", &GL1Scalers_ZDCS_raw);
    tree->SetBranchAddress("GL1Scalers_ZDCS_live", &GL1Scalers_ZDCS_live);
    tree->SetBranchAddress("GL1Scalers_ZDCS_scaled", &GL1Scalers_ZDCS_scaled);
    tree->SetBranchAddress("GL1Scalers_ZDCN_raw", &GL1Scalers_ZDCN_raw);
    tree->SetBranchAddress("GL1Scalers_ZDCN_live", &GL1Scalers_ZDCN_live);
    tree->SetBranchAddress("GL1Scalers_ZDCN_scaled", &GL1Scalers_ZDCN_scaled);
    tree->SetBranchAddress("GL1Scalers_ZDCNS_raw", &GL1Scalers_ZDCNS_raw);
    tree->SetBranchAddress("GL1Scalers_ZDCNS_live", &GL1Scalers_ZDCNS_live);
    tree->SetBranchAddress("GL1Scalers_ZDCNS_scaled", &GL1Scalers_ZDCNS_scaled);

    tree->SetBranchAddress("GL1Scalers_MBDS_raw", &GL1Scalers_MBDS_raw);
    tree->SetBranchAddress("GL1Scalers_MBDS_live", &GL1Scalers_MBDS_live);
    tree->SetBranchAddress("GL1Scalers_MBDS_scaled", &GL1Scalers_MBDS_scaled);
    tree->SetBranchAddress("GL1Scalers_MBDN_raw", &GL1Scalers_MBDN_raw);
    tree->SetBranchAddress("GL1Scalers_MBDN_live", &GL1Scalers_MBDN_live);
    tree->SetBranchAddress("GL1Scalers_MBDN_scaled", &GL1Scalers_MBDN_scaled);
    tree->SetBranchAddress("GL1Scalers_MBDNS_raw", &GL1Scalers_MBDNS_raw);
    tree->SetBranchAddress("GL1Scalers_MBDNS_live", &GL1Scalers_MBDNS_live);
    tree->SetBranchAddress("GL1Scalers_MBDNS_scaled", &GL1Scalers_MBDNS_scaled);

    pre_good_evt = false;
    this_good_evt = false;

    n_entries = tree->GetEntries();

    time_ZDCS_raw_counting.clear();
    time_ZDCN_raw_counting.clear();
    time_ZDCNS_raw_counting.clear();
    time_MBDS_raw_counting.clear();
    time_MBDN_raw_counting.clear();
    time_MBDNS_raw_counting.clear();
    time_MBDNS_30cm_raw_counting_pair.clear();
    time_LiveTrigger_Decimal.clear();
    time_MBDNS_zvtx.clear();
    time_ZDCNS_zvtx.clear();
    time_detectorNS_raw_counting.clear();
    time_GL1Scalers_range.clear();
    step_selected_T_range_V.clear();
    step_counting_range_V.clear();
    step_selected_T_range_H.clear();
    step_counting_range_H.clear();
    


    SetsPhenixStyle();
    c1 = new TCanvas("c1", "c1", 950, 800);
    c1 -> cd();

    gr_raw_ZDCS_rate = new TGraph(); 
    gl1_scaler_ana::quick_tgraph_set(gr_raw_ZDCS_rate, "Time [s]", "Raw ZDCS trigger rate [Hz]");

    gr_raw_ZDCN_rate = new TGraph(); 
    gl1_scaler_ana::quick_tgraph_set(gr_raw_ZDCN_rate, "Time [s]", "Raw ZDCN trigger rate [Hz]");

    gr_raw_ZDCNS_rate = new TGraph(); 
    gl1_scaler_ana::quick_tgraph_set(gr_raw_ZDCNS_rate, "Time [s]", "Raw ZDCNS trigger rate [Hz]");

    gr_raw_MBDS_rate = new TGraph(); 
    gl1_scaler_ana::quick_tgraph_set(gr_raw_MBDS_rate, "Time [s]", "Raw MBDS trigger rate [Hz]");

    gr_raw_MBDN_rate = new TGraph(); 
    gl1_scaler_ana::quick_tgraph_set(gr_raw_MBDN_rate, "Time [s]", "Raw MBDN trigger rate [Hz]");

    gr_raw_MBDNS_rate = new TGraph(); 
    gl1_scaler_ana::quick_tgraph_set(gr_raw_MBDNS_rate, "Time [s]", "Raw MBDNS trigger rate [Hz]");

    gr_raw_MBDNS_30cm_ratio = new TGraph();
    gl1_scaler_ana::quick_tgraph_set(gr_raw_MBDNS_30cm_ratio, "Time [s]", "MBDNS_30cm / MBDNS_inclusive", 1, 20, 0.4);


    gr_raw_detectorNS_rate_vecV.clear();
    gr_raw_detectorNS_rate_vecV_wide.clear();
    h1D_raw_detectorNS_rate_vecV.clear();
    gr_raw_detectorNS_rate_vecH.clear();
    gr_raw_detectorNS_rate_vecH_wide.clear();
    h1D_raw_detectorNS_rate_vecH.clear();
    fit_vecV.clear();
    fit_vecH.clear();

    h1D_detectorNS_vertexZ_vecV.clear();
    h1D_detectorNS_vertexZ_vecH.clear();

    h1D_detectorNS_vertexZ_postCorr_vecV.clear();
    h1D_detectorNS_vertexZ_postCorr_vecH.clear();


    for (int i = 0; i < range_t_V.size(); i++)
    {
        gr_raw_detectorNS_rate_vecV.push_back(new TGraphErrors());
        gr_raw_detectorNS_rate_vecV.back() -> GetXaxis() -> SetTitle("Time [s]");
        gr_raw_detectorNS_rate_vecV.back() -> SetLineColor(2);
        gr_raw_detectorNS_rate_vecV.back() -> GetYaxis() -> SetTitle("Raw MBDNS coin. trigger rate [Hz]");
        gr_raw_detectorNS_rate_vecV.back() -> GetXaxis() -> SetNdivisions(505);
        gr_raw_detectorNS_rate_vecV.back() -> SetMarkerStyle(20);
        gr_raw_detectorNS_rate_vecV.back() -> SetMarkerSize(0.7);
        gr_raw_detectorNS_rate_vecV.back() -> SetMarkerColor(2);

        gr_raw_detectorNS_rate_vecV_wide.push_back(new TGraph());
        gr_raw_detectorNS_rate_vecV_wide.back() -> GetXaxis() -> SetTitle("Time [s]");
        gr_raw_detectorNS_rate_vecV_wide.back() -> GetYaxis() -> SetTitle("Raw MBDNS coin. trigger rate [Hz]");
        gr_raw_detectorNS_rate_vecV_wide.back() -> GetXaxis() -> SetNdivisions(505);
        gr_raw_detectorNS_rate_vecV_wide.back() -> SetMarkerStyle(20);
        gr_raw_detectorNS_rate_vecV_wide.back() -> SetMarkerSize(0.7);
        gr_raw_detectorNS_rate_vecV_wide.back() -> SetMarkerColor(1);

        fit_vecV.push_back(new TF1(Form("fit_vecV_%d", i), "pol0", range_t_V[i].first, range_t_V[i].second));
        fit_vecV.back() -> SetLineColor(4);
        fit_vecV.back() -> SetLineWidth(3); 
        fit_vecV.back() -> SetLineStyle(7);

        h1D_detectorNS_vertexZ_vecV.push_back(new TH1F("", ";Reco. Z vertex [cm]; Entries", 100, -300, 300));
        h1D_detectorNS_vertexZ_vecV.back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vertexZ_postCorr_vecV.push_back(new TH1F(
            "",
            Form("%s;%s;%s", h1D_detectorNS_vertexZ_vecV.back()->GetTitle(), h1D_detectorNS_vertexZ_vecV.back()->GetXaxis()->GetTitle(), h1D_detectorNS_vertexZ_vecV.back()->GetYaxis()->GetTitle()),
            h1D_detectorNS_vertexZ_vecV.back()->GetNbinsX(),
            h1D_detectorNS_vertexZ_vecV.back()->GetXaxis()->GetXmin(),
            h1D_detectorNS_vertexZ_vecV.back()->GetXaxis()->GetXmax()
        ));
        h1D_detectorNS_vertexZ_postCorr_vecV.back() -> GetXaxis() -> SetNdivisions(505);
    }

    for (int i = 0; i < range_t_H.size(); i++)
    {
        gr_raw_detectorNS_rate_vecH.push_back(new TGraphErrors());
        gr_raw_detectorNS_rate_vecH.back() -> GetXaxis() -> SetTitle("Time [s]");
        gr_raw_detectorNS_rate_vecH.back() -> SetLineColor(2);
        gr_raw_detectorNS_rate_vecH.back() -> GetYaxis() -> SetTitle("Raw MBDNS coin. trigger rate [Hz]");
        gr_raw_detectorNS_rate_vecH.back() -> GetXaxis() -> SetNdivisions(505);
        gr_raw_detectorNS_rate_vecH.back() -> SetMarkerStyle(20);
        gr_raw_detectorNS_rate_vecH.back() -> SetMarkerSize(0.7);
        gr_raw_detectorNS_rate_vecH.back() -> SetMarkerColor(2);

        gr_raw_detectorNS_rate_vecH_wide.push_back(new TGraph());
        gr_raw_detectorNS_rate_vecH_wide.back() -> GetXaxis() -> SetTitle("Time [s]");
        gr_raw_detectorNS_rate_vecH_wide.back() -> GetYaxis() -> SetTitle("Raw MBDNS coin. trigger rate [Hz]");
        gr_raw_detectorNS_rate_vecH_wide.back() -> GetXaxis() -> SetNdivisions(505);
        gr_raw_detectorNS_rate_vecH_wide.back() -> SetMarkerStyle(20);
        gr_raw_detectorNS_rate_vecH_wide.back() -> SetMarkerSize(0.7);
        gr_raw_detectorNS_rate_vecH_wide.back() -> SetMarkerColor(1);

        fit_vecH.push_back(new TF1(Form("fit_vecH_%d", i), "pol0", range_t_H[i].first, range_t_H[i].second));
        fit_vecH.back() -> SetLineColor(4);
        fit_vecH.back() -> SetLineWidth(3);
        fit_vecH.back() -> SetLineStyle(7);

        h1D_detectorNS_vertexZ_vecH.push_back(new TH1F("", ";Reco. Z vertex [cm]; Entries", 100, -300, 300));
        h1D_detectorNS_vertexZ_vecH.back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vertexZ_postCorr_vecH.push_back(new TH1F(
            "",
            Form("%s;%s;%s", h1D_detectorNS_vertexZ_vecH.back()->GetTitle(), h1D_detectorNS_vertexZ_vecH.back()->GetXaxis()->GetTitle(), h1D_detectorNS_vertexZ_vecH.back()->GetYaxis()->GetTitle()),
            h1D_detectorNS_vertexZ_vecH.back()->GetNbinsX(),
            h1D_detectorNS_vertexZ_vecH.back()->GetXaxis()->GetXmin(),
            h1D_detectorNS_vertexZ_vecH.back()->GetXaxis()->GetXmax()
        ));
        h1D_detectorNS_vertexZ_postCorr_vecH.back() -> GetXaxis() -> SetNdivisions(505);
    }

    draw_text = new TLatex();
    draw_text -> SetNDC();
    draw_text -> SetTextSize(0.03);

    coord_line = new TLine();
    coord_line -> SetLineWidth(1);
    coord_line -> SetLineColor(16);
    coord_line -> SetLineStyle(2);

    legend = new TLegend(0.45,0.8,0.70,0.9);
    // legend -> SetMargin(0);
    legend->SetTextSize(0.03);

    DetectorNS_rate_avg_vecV.clear();
    DetectorNS_rate_avg_vecV_error.clear();
    DetectorNS_rate_avg_vecH.clear();
    DetectorNS_rate_avg_vecH_error.clear();

    gr_BPM_raw_detectorNS_rate_V = new TGraphErrors();
    gr_BPM_raw_detectorNS_rate_V -> SetMarkerStyle(20);
    gr_BPM_raw_detectorNS_rate_V -> SetMarkerSize(0.8);
    gr_BPM_raw_detectorNS_rate_V -> SetMarkerColor(1);
    gr_BPM_raw_detectorNS_rate_V -> GetXaxis() -> SetTitle("BPM position (Y scan) [mm]");
    gr_BPM_raw_detectorNS_rate_V -> GetYaxis() -> SetTitle("Raw MBDNS coin. trigger rate [Hz]");
    gr_BPM_raw_detectorNS_rate_V -> GetXaxis() -> SetNdivisions(505);

    gr_BPM_raw_detectorNS_rate_H = new TGraphErrors();
    gr_BPM_raw_detectorNS_rate_H -> SetMarkerStyle(20);
    gr_BPM_raw_detectorNS_rate_H -> SetMarkerSize(0.8);
    gr_BPM_raw_detectorNS_rate_H -> SetMarkerColor(1);
    gr_BPM_raw_detectorNS_rate_H -> GetXaxis() -> SetTitle("BPM position (X scan) [mm]");
    gr_BPM_raw_detectorNS_rate_H -> GetYaxis() -> SetTitle("Raw MBDNS coin. trigger rate [Hz]");
    gr_BPM_raw_detectorNS_rate_H -> GetXaxis() -> SetNdivisions(505);

    gr_BPM_raw_detectorNS_rate_V_demo = new TGraphErrors();
    gr_BPM_raw_detectorNS_rate_V_demo -> SetMarkerStyle(20);
    gr_BPM_raw_detectorNS_rate_V_demo -> SetMarkerSize(0.8);
    gr_BPM_raw_detectorNS_rate_V_demo -> SetMarkerColor(1);

    gr_BPM_raw_detectorNS_rate_H_demo = new TGraphErrors();
    gr_BPM_raw_detectorNS_rate_H_demo -> SetMarkerStyle(20);
    gr_BPM_raw_detectorNS_rate_H_demo -> SetMarkerSize(0.8);
    gr_BPM_raw_detectorNS_rate_H_demo -> SetMarkerColor(1);

    // fit_Gaus_V = new TF1("fit_Gaus_V", "gaus", -5, 5);
    fit_Gaus_V = new TF1("fit_Gaus_V", gaus_func, -5, 5, 4);
    fit_Gaus_V -> SetParameters(9.32502e+05, -8.97690e-01, 2.99013e-01, 0);
    fit_Gaus_V -> SetLineColor(2);
    fit_Gaus_V -> SetLineWidth(2);
    // fit_Gaus_V -> SetLineStyle(7);
    // fit_Gaus_H = new TF1("fit_Gaus_H", "gaus", -5, 5);
    fit_Gaus_H = new TF1("fit_Gaus_H", gaus_func, -5, 5, 4);
    fit_Gaus_H -> SetParameters(9.56217e+05, 1.44200e+00, 3.01624e-01, 0);
    fit_Gaus_H -> SetLineColor(2);
    fit_Gaus_H -> SetLineWidth(2);
    // fit_Gaus_H -> SetLineStyle(7);

    multi_collision_correction_V.clear();
    multi_collision_correction_H.clear();
    outlier_rejection_factor_vecV.clear();
    outlier_rejection_factor_vecH.clear();
    accidental_correction_V.clear();
    accidental_correction_H.clear();
    detectorNS_zvtx_effi_correction_H.clear();
    detectorNS_zvtx_effi_correction_V.clear();
}

// TCanvas * gl1_scaler_ana::c1 = new TCanvas("c1", "c1", 950, 800);

void gl1_scaler_ana::SetDetectorName(string detector_name) {
    if (
        gl1_scaler_ana::keep_alpha(detector_name) != detector_pool[0] && 
        gl1_scaler_ana::keep_alpha(detector_name) != detector_pool[1] &&
        gl1_scaler_ana::keep_alpha(detector_name) != detector_pool[2] &&
        gl1_scaler_ana::keep_alpha(detector_name) != detector_pool[3] &&
        gl1_scaler_ana::keep_alpha(detector_name) != detector_pool[4] &&
        gl1_scaler_ana::keep_alpha(detector_name) != detector_pool[5]
    ) 
    {
        cout<<"Detector name is not correct, please choose from MBD or ZDC"<<endl;
        exit(1);
    }
    else 
    {
        detector_selection = detector_name;
    }
}

void gl1_scaler_ana::SetMBDvtxZEffiFunc(TF1* MBD_vtxZ_effi_func_in) 
{
    MBD_vtxZ_effi_func = MBD_vtxZ_effi_func_in;

    if (MBD_vtxZ_effi_func != nullptr)
    {
        cout<<"equation received, N parameters : "<<MBD_vtxZ_effi_func->GetNpar()<<endl;
        for (int i = 0; i < MBD_vtxZ_effi_func->GetNpar(); i++)
        {
            cout<<"parameter "<<i<<" : "<<MBD_vtxZ_effi_func->GetParameter(i)<<endl;
        }
    }

}

void gl1_scaler_ana::PrepareRate(string input_file_directory)
{
    if (input_file_directory == "null")
    {
        for (int i = 0; i < n_entries; i++)
        {
            tree->GetEntry(i);

            if (i % 10000 == 0) cout << "Processing entry " << i << " / " << n_entries << endl;

            if (GTMBusyVector_Decimal != 0 || GTMBusyVector_Decimal == -999) {pre_good_evt = false; continue;}
            if (bunchnumber == -999) {pre_good_evt = false; continue;}
            if (evtBCO_gl1 == -999) {pre_good_evt = false; continue;}
            if (GL1Scalers_clock_raw == -999) {pre_good_evt = false; continue;}
            if (GL1Scalers_ZDCS_raw == -999) {pre_good_evt = false; continue;}
            if (GL1Scalers_ZDCN_raw == -999) {pre_good_evt = false; continue;}
            if (GL1Scalers_ZDCNS_raw == -999) {pre_good_evt = false; continue;}
            if (GL1Scalers_MBDS_raw == -999) {pre_good_evt = false; continue;}
            if (GL1Scalers_MBDN_raw == -999) {pre_good_evt = false; continue;}
            if (GL1Scalers_MBDNS_raw == -999) {pre_good_evt = false; continue;}

            this_good_evt = true;

            live_trigger_map.clear(); // note : the live trigger map
            for (int i = 0; i < live_trigger_vec->size(); i++) { live_trigger_map[live_trigger_vec->at(i)] = live_trigger_vec->at(i); }

            // note : in every second
            if (time_MBDNS_30cm_raw_counting_pair.find( int((GL1Scalers_clock_raw) * bco_span) ) == time_MBDNS_30cm_raw_counting_pair.end())
            {
                time_MBDNS_30cm_raw_counting_pair[ int((GL1Scalers_clock_raw) * bco_span) ] = {0,0};
                if (live_trigger_map.find(MBDNS_30cm_ID) != live_trigger_map.end()) {
                    time_MBDNS_30cm_raw_counting_pair[ int((GL1Scalers_clock_raw) * bco_span) ].first += 1;
                }
                if (live_trigger_map.find(MBDNS_inclusive_ID) != live_trigger_map.end()) {
                    time_MBDNS_30cm_raw_counting_pair[ int((GL1Scalers_clock_raw) * bco_span) ].second += 1;
                }
            }
            else
            {
                if (live_trigger_map.find(MBDNS_30cm_ID) != live_trigger_map.end()) {
                    time_MBDNS_30cm_raw_counting_pair[ int((GL1Scalers_clock_raw) * bco_span) ].first += 1;
                }
                if (live_trigger_map.find(MBDNS_inclusive_ID) != live_trigger_map.end()) {
                    time_MBDNS_30cm_raw_counting_pair[ int((GL1Scalers_clock_raw) * bco_span) ].second += 1;
                }
            }

            // note : in every second
            if (time_MBDNS_zvtx.find( int((GL1Scalers_clock_raw) * bco_span) ) == time_MBDNS_zvtx.end())
            {
                time_MBDNS_zvtx[ int((GL1Scalers_clock_raw) * bco_span) ] = vector<double>();
                time_MBDNS_zvtx[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(mbd_z_vtx);
            }
            else 
            {
                time_MBDNS_zvtx[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(mbd_z_vtx);
            }

            // note : in every second
            if (time_ZDCNS_zvtx.find( int((GL1Scalers_clock_raw) * bco_span) ) == time_ZDCNS_zvtx.end())
            {
                time_ZDCNS_zvtx[ int((GL1Scalers_clock_raw) * bco_span) ] = vector<double>();
                time_ZDCNS_zvtx[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(zdc_z_vtx);
            }
            else 
            {
                time_ZDCNS_zvtx[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(zdc_z_vtx);
            }

            // note : in every second
            if (time_LiveTrigger_Decimal.find( int((GL1Scalers_clock_raw) * bco_span) ) == time_LiveTrigger_Decimal.end())
            {
                time_LiveTrigger_Decimal[ int((GL1Scalers_clock_raw) * bco_span) ] = vector<long long>();
                time_LiveTrigger_Decimal[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(LiveTrigger_Decimal);
            }
            else 
            {
                time_LiveTrigger_Decimal[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(LiveTrigger_Decimal);
            }

            // note : in every second
            if (time_ZDCS_raw_counting.find( int((GL1Scalers_clock_raw) * bco_span) ) == time_ZDCS_raw_counting.end())
            {
                time_ZDCS_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ] = vector<long long>();
                time_ZDCS_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(GL1Scalers_ZDCS_raw);
            }
            else 
            {
                time_ZDCS_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(GL1Scalers_ZDCS_raw);
            }

            // note : in every second
            if (time_ZDCN_raw_counting.find( int((GL1Scalers_clock_raw) * bco_span) ) == time_ZDCN_raw_counting.end())
            {
                time_ZDCN_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ] = vector<long long>();
                time_ZDCN_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(GL1Scalers_ZDCN_raw);
            }
            else 
            {
                time_ZDCN_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(GL1Scalers_ZDCN_raw);
            }

            // note : in every second
            if (time_ZDCNS_raw_counting.find( int((GL1Scalers_clock_raw) * bco_span) ) == time_ZDCNS_raw_counting.end())
            {
                time_ZDCNS_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ] = vector<long long>();
                time_ZDCNS_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(GL1Scalers_ZDCNS_raw);
            }
            else 
            {
                time_ZDCNS_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(GL1Scalers_ZDCNS_raw);
            }

            // note : in every second
            if (time_MBDS_raw_counting.find( int((GL1Scalers_clock_raw) * bco_span) ) == time_MBDS_raw_counting.end())
            {
                time_MBDS_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ] = vector<long long>();
                time_MBDS_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(GL1Scalers_MBDS_raw);
            }
            else 
            {
                time_MBDS_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(GL1Scalers_MBDS_raw);
            }

            // note : in every second
            if (time_MBDN_raw_counting.find( int((GL1Scalers_clock_raw) * bco_span) ) == time_MBDN_raw_counting.end())
            {
                time_MBDN_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ] = vector<long long>();
                time_MBDN_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(GL1Scalers_MBDN_raw);
            }
            else 
            {
                time_MBDN_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(GL1Scalers_MBDN_raw);
            }

            // note : in every second
            if (time_MBDNS_raw_counting.find( int((GL1Scalers_clock_raw) * bco_span) ) == time_MBDNS_raw_counting.end())
            {
                time_MBDNS_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ] = vector<long long>();
                time_MBDNS_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(GL1Scalers_MBDNS_raw);
            }
            else 
            {
                time_MBDNS_raw_counting[ int((GL1Scalers_clock_raw) * bco_span) ].push_back(GL1Scalers_MBDNS_raw);
            }

            // note : in every second
            if (time_GL1Scalers_range.find( int((GL1Scalers_clock_raw) * bco_span) ) == time_GL1Scalers_range.end())
            {
                time_GL1Scalers_range[ int((GL1Scalers_clock_raw) * bco_span) ] = {GL1Scalers_clock_raw, -999};
            }
            else 
            {
                time_GL1Scalers_range[ int((GL1Scalers_clock_raw) * bco_span) ].second = GL1Scalers_clock_raw;
            }

            pre_good_evt = this_good_evt;
        }
    }
    else 
    {        
        TFile * rate_file_in = TFile::Open(input_file_directory.c_str());
        TTree * rate_tree_in = (TTree*)rate_file_in->Get("tree");

        int timestamp;

        long long ZDCS_raw_counting_front_in;
        long long ZDCS_raw_counting_back_in;
        long long ZDCN_raw_counting_front_in;
        long long ZDCN_raw_counting_back_in;
        long long ZDCNS_raw_counting_front_in;
        long long ZDCNS_raw_counting_back_in;
        long long MBDS_raw_counting_front_in;
        long long MBDS_raw_counting_back_in;
        long long MBDN_raw_counting_front_in;
        long long MBDN_raw_counting_back_in;
        long long MBDNS_raw_counting_front_in;
        long long MBDNS_raw_counting_back_in;
        long long MBDNS_30cm_raw_counting_first_in;
        long long MBDNS_30cm_raw_counting_second_in;

        long long GL1Scalers_range_first_in;
        long long GL1Scalers_range_second_in;

        vector<double> *MBD_zvtx_in = 0;
        vector<double> *ZDC_zvtx_in = 0;
        vector<long long> *LiveTrigger_Decimal_in = 0;

        rate_tree_in -> SetBranchAddress("timestamp", &timestamp);

        rate_tree_in -> SetBranchAddress("ZDCS_raw_counting_front", &ZDCS_raw_counting_front_in);
        rate_tree_in -> SetBranchAddress("ZDCS_raw_counting_back", &ZDCS_raw_counting_back_in);

        rate_tree_in -> SetBranchAddress("ZDCN_raw_counting_front", &ZDCN_raw_counting_front_in);
        rate_tree_in -> SetBranchAddress("ZDCN_raw_counting_back", &ZDCN_raw_counting_back_in);

        rate_tree_in -> SetBranchAddress("ZDCNS_raw_counting_front", &ZDCNS_raw_counting_front_in);
        rate_tree_in -> SetBranchAddress("ZDCNS_raw_counting_back", &ZDCNS_raw_counting_back_in);

        rate_tree_in -> SetBranchAddress("MBDS_raw_counting_front", &MBDS_raw_counting_front_in);
        rate_tree_in -> SetBranchAddress("MBDS_raw_counting_back", &MBDS_raw_counting_back_in);

        rate_tree_in -> SetBranchAddress("MBDN_raw_counting_front", &MBDN_raw_counting_front_in);
        rate_tree_in -> SetBranchAddress("MBDN_raw_counting_back", &MBDN_raw_counting_back_in);

        rate_tree_in -> SetBranchAddress("MBDNS_raw_counting_front", &MBDNS_raw_counting_front_in);
        rate_tree_in -> SetBranchAddress("MBDNS_raw_counting_back", &MBDNS_raw_counting_back_in);

        rate_tree_in -> SetBranchAddress("MBDNS_30cm_raw_counting_first", &MBDNS_30cm_raw_counting_first_in);
        rate_tree_in -> SetBranchAddress("MBDNS_30cm_raw_counting_second", &MBDNS_30cm_raw_counting_second_in);

        rate_tree_in -> SetBranchAddress("GL1Scalers_range_first", &GL1Scalers_range_first_in);
        rate_tree_in -> SetBranchAddress("GL1Scalers_range_second", &GL1Scalers_range_second_in);

        rate_tree_in -> SetBranchAddress("MBDNS_zvtx", &MBD_zvtx_in);
        rate_tree_in -> SetBranchAddress("ZDCNS_zvtx", &ZDC_zvtx_in);

        rate_tree_in -> SetBranchAddress("LiveTrigger_Decimal", &LiveTrigger_Decimal_in);

        for (int i = 0; i < rate_tree_in->GetEntries(); i++)
        {
            rate_tree_in -> GetEntry(i);

            time_ZDCS_raw_counting[ timestamp ] = {ZDCS_raw_counting_front_in, ZDCS_raw_counting_back_in};
            time_ZDCN_raw_counting[ timestamp ] = {ZDCN_raw_counting_front_in, ZDCN_raw_counting_back_in};
            time_ZDCNS_raw_counting[ timestamp ] = {ZDCNS_raw_counting_front_in, ZDCNS_raw_counting_back_in};
            time_MBDS_raw_counting[ timestamp ] = {MBDS_raw_counting_front_in, MBDS_raw_counting_back_in};
            time_MBDN_raw_counting[ timestamp ] = {MBDN_raw_counting_front_in, MBDN_raw_counting_back_in};
            time_MBDNS_raw_counting[ timestamp ] = {MBDNS_raw_counting_front_in, MBDNS_raw_counting_back_in};
            time_MBDNS_30cm_raw_counting_pair[ timestamp ] = {MBDNS_30cm_raw_counting_first_in, MBDNS_30cm_raw_counting_second_in};
            time_GL1Scalers_range[ timestamp ] = {GL1Scalers_range_first_in, GL1Scalers_range_second_in};

            // note : for the vector
            if (time_MBDNS_zvtx.find(timestamp) == time_MBDNS_zvtx.end())
            {
                time_MBDNS_zvtx[timestamp] = vector<double>();
                for (int j = 0; j < MBD_zvtx_in->size(); j++)
                {
                    time_MBDNS_zvtx[timestamp].push_back(MBD_zvtx_in->at(j));
                }
            }

            // note : for the vector
            if (time_ZDCNS_zvtx.find(timestamp) == time_ZDCNS_zvtx.end())
            {
                time_ZDCNS_zvtx[timestamp] = vector<double>();
                for (int j = 0; j < ZDC_zvtx_in->size(); j++)
                {
                    time_ZDCNS_zvtx[timestamp].push_back(ZDC_zvtx_in->at(j));
                }
            }

            // note : for the vector
            if (time_LiveTrigger_Decimal.find(timestamp) == time_LiveTrigger_Decimal.end())
            {
                time_LiveTrigger_Decimal[timestamp] = vector<long long>();
                for (int j = 0; j < LiveTrigger_Decimal_in->size(); j++)
                {
                    time_LiveTrigger_Decimal[timestamp].push_back(LiveTrigger_Decimal_in->at(j));
                }
            }
        }        

        rate_file_in -> Close();

        // cout<<"quick check : "<<time_ZDCS_raw_counting.size()<<endl;
        // for (auto pair : time_ZDCS_raw_counting)
        // {
        //     cout<<pair.first<<" "<<pair.second.front()<<" "<<pair.second.back()<<endl;
        // }

    }
}

void gl1_scaler_ana::OutputRawRate(string output_file_directory)
{
    TFile * file_out = new TFile(output_file_directory.c_str(), "recreate");
    TTree * tree_out = new TTree("tree", "tree");

    int timestamp;

    long long ZDCS_raw_counting_front;
    long long ZDCS_raw_counting_back;

    long long ZDCN_raw_counting_front;
    long long ZDCN_raw_counting_back;

    long long ZDCNS_raw_counting_front;
    long long ZDCNS_raw_counting_back;

    long long MBDS_raw_counting_front;
    long long MBDS_raw_counting_back;

    long long MBDN_raw_counting_front;
    long long MBDN_raw_counting_back;

    long long MBDNS_raw_counting_front;
    long long MBDNS_raw_counting_back;

    long long MBDNS_30cm_raw_counting_first;
    long long MBDNS_30cm_raw_counting_second;

    long long GL1Scalers_range_first;
    long long GL1Scalers_range_second;

    vector<double> MBDNS_zvtx_out_vec;
    vector<double> ZDCNS_zvtx_out_vec;
    vector<long long> LiveTrigger_Decimal_out_vec;

    tree_out -> Branch("timestamp", &timestamp, "timestamp/I");

    tree_out -> Branch("ZDCS_raw_counting_front", &ZDCS_raw_counting_front, "ZDCS_raw_counting_front/L");
    tree_out -> Branch("ZDCS_raw_counting_back", &ZDCS_raw_counting_back, "ZDCS_raw_counting_back/L");

    tree_out -> Branch("ZDCN_raw_counting_front", &ZDCN_raw_counting_front, "ZDCN_raw_counting_front/L");
    tree_out -> Branch("ZDCN_raw_counting_back", &ZDCN_raw_counting_back, "ZDCN_raw_counting_back/L");

    tree_out -> Branch("ZDCNS_raw_counting_front", &ZDCNS_raw_counting_front, "ZDCNS_raw_counting_front/L");
    tree_out -> Branch("ZDCNS_raw_counting_back", &ZDCNS_raw_counting_back, "ZDCNS_raw_counting_back/L");

    tree_out -> Branch("MBDS_raw_counting_front", &MBDS_raw_counting_front, "MBDS_raw_counting_front/L");
    tree_out -> Branch("MBDS_raw_counting_back", &MBDS_raw_counting_back, "MBDS_raw_counting_back/L");

    tree_out -> Branch("MBDN_raw_counting_front", &MBDN_raw_counting_front, "MBDN_raw_counting_front/L");
    tree_out -> Branch("MBDN_raw_counting_back", &MBDN_raw_counting_back, "MBDN_raw_counting_back/L");

    tree_out -> Branch("MBDNS_raw_counting_front", &MBDNS_raw_counting_front, "MBDNS_raw_counting_front/L");
    tree_out -> Branch("MBDNS_raw_counting_back", &MBDNS_raw_counting_back, "MBDNS_raw_counting_back/L");

    tree_out -> Branch("MBDNS_30cm_raw_counting_first", &MBDNS_30cm_raw_counting_first, "MBDNS_30cm_raw_counting_first/L");
    tree_out -> Branch("MBDNS_30cm_raw_counting_second", &MBDNS_30cm_raw_counting_second, "MBDNS_30cm_raw_counting_second/L");

    tree_out -> Branch("GL1Scalers_range_first", &GL1Scalers_range_first, "GL1Scalers_range_first/L");
    tree_out -> Branch("GL1Scalers_range_second", &GL1Scalers_range_second, "GL1Scalers_range_second/L");

    tree_out -> Branch("MBDNS_zvtx", &MBDNS_zvtx_out_vec);
    tree_out -> Branch("ZDCNS_zvtx", &ZDCNS_zvtx_out_vec);

    tree_out -> Branch("LiveTrigger_Decimal", &LiveTrigger_Decimal_out_vec);

    cout<<"In gl1_scaler_ana::OutputRawRate, "<<00000<<endl;

    for ( auto pair : time_ZDCS_raw_counting)
    {
        timestamp = pair.first;
        ZDCS_raw_counting_front = pair.second.front();
        ZDCS_raw_counting_back = pair.second.back();

        ZDCN_raw_counting_front = time_ZDCN_raw_counting[pair.first].front();
        ZDCN_raw_counting_back = time_ZDCN_raw_counting[pair.first].back();

        ZDCNS_raw_counting_front = time_ZDCNS_raw_counting[pair.first].front();
        ZDCNS_raw_counting_back = time_ZDCNS_raw_counting[pair.first].back();

        MBDS_raw_counting_front = time_MBDS_raw_counting[pair.first].front();
        MBDS_raw_counting_back = time_MBDS_raw_counting[pair.first].back();

        MBDN_raw_counting_front = time_MBDN_raw_counting[pair.first].front();
        MBDN_raw_counting_back = time_MBDN_raw_counting[pair.first].back();

        MBDNS_raw_counting_front = time_MBDNS_raw_counting[pair.first].front();
        MBDNS_raw_counting_back = time_MBDNS_raw_counting[pair.first].back();

        MBDNS_30cm_raw_counting_first = time_MBDNS_30cm_raw_counting_pair[pair.first].first;
        MBDNS_30cm_raw_counting_second = time_MBDNS_30cm_raw_counting_pair[pair.first].second;

        GL1Scalers_range_first = time_GL1Scalers_range[pair.first].first;
        GL1Scalers_range_second = time_GL1Scalers_range[pair.first].second;

        cout<<"In gl1_scaler_ana::OutputRawRate, "<<11111<<endl;

        MBDNS_zvtx_out_vec = time_MBDNS_zvtx[pair.first];

        cout<<"In gl1_scaler_ana::OutputRawRate, "<<22222<<endl;

        ZDCNS_zvtx_out_vec = time_ZDCNS_zvtx[pair.first];
        
        cout<<"In gl1_scaler_ana::OutputRawRate, "<<33333<<endl;
        
        LiveTrigger_Decimal_out_vec = time_LiveTrigger_Decimal[pair.first];

        cout<<"In gl1_scaler_ana::OutputRawRate, "<<4444<<endl;

        tree_out -> Fill();
    }

    file_out -> Write();
    file_out -> Close();

}

std::pair<TGraphErrors *, TGraphErrors *> gl1_scaler_ana::CombineMacro(string detector_name, bool only_raw_rate_tag_in)
{
    cout<<endl;
    cout<<endl;
    cout<<"====================================== ====================================== ====================================== ======================================"<<endl;
    cout<<"here we are runing the gl1_scaler_ana::CombineMacro for the detector : "<<detector_name<<endl;
    SetDetectorName(detector_name);

    final_output_directory = output_directory + "/" + detector_selection;
    if (NCollision_corr) final_output_directory += "_NCorr";
    if (beam_intensity_corr) final_output_directory += "_ICorr";
    if (accidental_correction) final_output_directory += "_AcciCorr";
    if (use_set_pos) final_output_directory += "_SetPos";
    if (MBD_zvtx_effi) final_output_directory += "_ZvtxEffi";
    system(Form("mkdir -p %s", final_output_directory.c_str()));
    
    only_raw_rate_tag = only_raw_rate_tag_in;

    for (int i = 0; i < gr_raw_detectorNS_rate_vecV.size(); i++)
    {
        gr_raw_detectorNS_rate_vecV[i] -> GetYaxis() -> SetTitle(Form("Raw %s trigger rate [Hz]", detector_selection.c_str()));
        gr_raw_detectorNS_rate_vecV_wide[i] -> GetYaxis() -> SetTitle(Form("Raw %s trigger rate [Hz]", detector_selection.c_str()));

        gr_raw_detectorNS_rate_vecH[i] -> GetYaxis() -> SetTitle(Form("Raw %s trigger rate [Hz]", detector_selection.c_str()));
        gr_raw_detectorNS_rate_vecH_wide[i] -> GetYaxis() -> SetTitle(Form("Raw %s trigger rate [Hz]", detector_selection.c_str()));
    }

    if (NCollision_corr || beam_intensity_corr || accidental_correction){
        gr_BPM_raw_detectorNS_rate_V -> GetYaxis() -> SetTitle(Form("Corrected %s trigger rate [Hz]", detector_selection.c_str()));
        gr_BPM_raw_detectorNS_rate_H -> GetYaxis() -> SetTitle(Form("Corrected %s trigger rate [Hz]", detector_selection.c_str()));
    }
    else {
        gr_BPM_raw_detectorNS_rate_V -> GetYaxis() -> SetTitle(Form("Raw %s trigger rate [Hz]", detector_selection.c_str()));
        gr_BPM_raw_detectorNS_rate_H -> GetYaxis() -> SetTitle(Form("Raw %s trigger rate [Hz]", detector_selection.c_str()));
    }

    PrepareData();
    if (only_raw_rate_tag == false) {CombineData();} // note : run full analysis
    DrawPlots();
    if (only_raw_rate_tag == false) {GetMachineLumi();} // note : run full analysis
    if (only_raw_rate_tag == false) {GetDetectorCrossSection();} // note : run full analysis
    // ClearUp();

    global_ana_counting++;

    return {gr_BPM_raw_detectorNS_rate_V, gr_BPM_raw_detectorNS_rate_H};
}

void gl1_scaler_ana::PrepareData()
{
    // note : for ZDCS
    for (auto vec : time_ZDCS_raw_counting) { 
        gr_raw_ZDCS_rate -> SetPoint(gr_raw_ZDCS_rate->GetN(), vec.first + 0.5, vec.second.back() - vec.second.front() + 1); 
        // cout<<"Time : "<<vec.first<<" s, ZDCS rate : "<<vec.second.back() - vec.second.front()<<" Hz"<<endl;
    }

    // note : for ZDCN
    for (auto vec : time_ZDCN_raw_counting) { 
        gr_raw_ZDCN_rate -> SetPoint(gr_raw_ZDCN_rate->GetN(), vec.first + 0.5, vec.second.back() - vec.second.front() + 1); 
        // cout<<"Time : "<<vec.first<<" s, ZDCN rate : "<<vec.second.back() - vec.second.front()<<" Hz"<<endl;
    }

    // note : for ZDCNS
    for (auto vec : time_ZDCNS_raw_counting) { 
        gr_raw_ZDCNS_rate -> SetPoint(gr_raw_ZDCNS_rate->GetN(), vec.first + 0.5, vec.second.back() - vec.second.front() + 1); 
        // cout<<"Time : "<<vec.first<<" s, ZDCNS rate : "<<vec.second.back() - vec.second.front()<<" Hz"<<endl;
    }

    // note : for MBDS
    for (auto vec : time_MBDS_raw_counting) { 
        gr_raw_MBDS_rate -> SetPoint(gr_raw_MBDS_rate->GetN(), vec.first + 0.5, vec.second.back() - vec.second.front() + 1); 
        // cout<<"Time : "<<vec.first<<" s, MBDS rate : "<<vec.second.back() - vec.second.front()<<" Hz"<<endl;
    }

    // note : for MBDN
    for (auto vec : time_MBDN_raw_counting) { 
        gr_raw_MBDN_rate -> SetPoint(gr_raw_MBDN_rate->GetN(), vec.first + 0.5, vec.second.back() - vec.second.front() + 1); 
        // cout<<"Time : "<<vec.first<<" s, MBDN rate : "<<vec.second.back() - vec.second.front()<<" Hz"<<endl;
    }

    // note : for MBDNS
    for (auto vec : time_MBDNS_raw_counting) { 
        gr_raw_MBDNS_rate -> SetPoint(gr_raw_MBDNS_rate->GetN(), vec.first + 0.5, vec.second.back() - vec.second.front() + 1); 
        // cout<<"Time : "<<vec.first<<" s, MBDNS rate : "<<vec.second.back() - vec.second.front()<<" Hz"<<endl;

        if (only_raw_rate_tag == true) {cout<<"("<<vec.first<<", "<<vec.second.back() - vec.second.front()<<")"<<endl;} // note : only the raw rate part
    }

    // note : for MBDNS_30cm ratio
    for (auto vec : time_MBDNS_30cm_raw_counting_pair) { 
        gr_raw_MBDNS_30cm_ratio -> SetPoint(gr_raw_MBDNS_30cm_ratio->GetN(), vec.first + 0.5, double(vec.second.first) / double(vec.second.second)); 
        // cout<<"test test : "<<gr_raw_MBDNS_30cm_ratio->GetN()<<" "<<vec.first + 0.5<<" "<<double(vec.second.first) / double(vec.second.second)<<endl;
    }
    
    if (only_raw_rate_tag == true) {return;} // note : only the raw rate part

    // note : the detector selection, the new feature 
    if (gl1_scaler_ana::keep_alpha(detector_selection) == detector_pool[0]) {// note : ZDCS
        time_detectorNS_raw_counting = time_ZDCS_raw_counting;
    } 
    else if (gl1_scaler_ana::keep_alpha(detector_selection) == detector_pool[1]) { // note : ZDCN
        time_detectorNS_raw_counting = time_ZDCN_raw_counting;
    }
    else if (gl1_scaler_ana::keep_alpha(detector_selection) == detector_pool[2]) { // note : ZDCNS
        time_detectorNS_raw_counting = time_ZDCNS_raw_counting;
    }
    else if (gl1_scaler_ana::keep_alpha(detector_selection) == detector_pool[3]) { // note : MBDS
        time_detectorNS_raw_counting = time_MBDS_raw_counting;
    }
    else if (gl1_scaler_ana::keep_alpha(detector_selection) == detector_pool[4]) { // note : MBDN
        time_detectorNS_raw_counting = time_MBDN_raw_counting;
    }
    else if (gl1_scaler_ana::keep_alpha(detector_selection) == detector_pool[5]) { // note : MBDNS
        time_detectorNS_raw_counting = time_MBDNS_raw_counting;
    }
    else 
    {
        cout<<"Detector name is not correct, please choose from MBD or ZDC"<<endl;
        exit(1);
    }

    // note : for vertical
    PrepareData_subfunc(
        "Vertical",
        range_t_V, 
        step_selected_T_range_V,
        step_counting_range_V,
        gr_raw_detectorNS_rate_vecV,
        gr_raw_detectorNS_rate_vecV_wide,
        h1D_raw_detectorNS_rate_vecV,
        fit_vecV,
        DetectorNS_rate_avg_vecV,
        DetectorNS_rate_avg_vecV_error,
        outlier_rejection_factor_vecV,
        multi_collision_correction_V,
        fit_gaus_vecV,
        accidental_correction_V,
        h1D_detectorNS_vertexZ_vecV,
        h1D_detectorNS_vertexZ_postCorr_vecV,
        detectorNS_zvtx_effi_correction_V
    );

    // note : for horizontal
    PrepareData_subfunc(
        "Horizontal",
        range_t_H, 
        step_selected_T_range_H,
        step_counting_range_H,
        gr_raw_detectorNS_rate_vecH,
        gr_raw_detectorNS_rate_vecH_wide,
        h1D_raw_detectorNS_rate_vecH,
        fit_vecH,
        DetectorNS_rate_avg_vecH,
        DetectorNS_rate_avg_vecH_error,
        outlier_rejection_factor_vecH,
        multi_collision_correction_H,
        fit_gaus_vecH,
        accidental_correction_H,
        h1D_detectorNS_vertexZ_vecH,
        h1D_detectorNS_vertexZ_postCorr_vecH,
        detectorNS_zvtx_effi_correction_H
    );
}

void gl1_scaler_ana::PrepareData_subfunc(
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
)
{
    // note : the time range of each scan step, for the case of vertical scan
    for (int i = 0; i < range_t_All.size(); i++)
    {
        step_selected_T_range_All[i] = {time_GL1Scalers_range[range_t_All[i].first].first, time_GL1Scalers_range[range_t_All[i].second].second};
        step_counting_range_All[i] = {time_detectorNS_raw_counting[range_t_All[i].first].front(), time_detectorNS_raw_counting[range_t_All[i].second].back()};
        
        vector<double> Y_point_vec; Y_point_vec.clear();
        vector<pair<double,double>> pair_vec; pair_vec.clear();
        vector<pair<double,double>> pairE_vec; pairE_vec.clear();

        for (auto vec : time_detectorNS_raw_counting) 
        { 
            if (vec.first >= range_t_All[i].first && vec.first <= range_t_All[i].second)
            {
                Y_point_vec.push_back(vec.second.back() - vec.second.front() + 1);
                pair_vec.push_back({vec.first + 0.5, vec.second.back() - vec.second.front() + 1});
                pairE_vec.push_back({0.5, sqrt(double(vec.second.back() - vec.second.front()))});
                // gr_raw_detectorNS_rate_vec_All[i] -> SetPoint(gr_raw_detectorNS_rate_vec_All[i]->GetN(), vec.first + 0.5, vec.second.back() - vec.second.front() + 1);
                // gr_raw_detectorNS_rate_vec_All[i] -> SetPointError(gr_raw_detectorNS_rate_vec_All[i]->GetN() - 1, 0.5, sqrt(double(vec.second.back() - vec.second.front())));

                if (detector_selection == "MBDNS")
                {
                    for (int vtx_i = 0; vtx_i < time_MBDNS_zvtx[vec.first].size(); vtx_i++){
                        if (time_MBDNS_zvtx[vec.first][vtx_i] == -999) {continue;}
                        h1D_detectorNS_vertexZ_vecAll[i] -> Fill(time_MBDNS_zvtx[vec.first][vtx_i]);
                    }

                    if (MBD_zvtx_effi)
                    {
                        if (MBD_vtxZ_effi_func == nullptr) {
                            cout<<"Error : MBD_vtxZ_effi_func is not defined"<<endl;
                            exit(1);
                        }

                        for (int vtx_i = 0; vtx_i < time_MBDNS_zvtx[vec.first].size(); vtx_i++){
                            if (time_MBDNS_zvtx[vec.first][vtx_i] == -999) {continue;}
                            h1D_detectorNS_vertexZ_postCorr_vecAll[i] -> Fill(time_MBDNS_zvtx[vec.first][vtx_i], 1. / MBD_vtxZ_effi_func ->Eval(time_MBDNS_zvtx[vec.first][vtx_i]));
                        }
                    }
                }
            }

            if (vec.first >= (range_t_All[i].first - width_range_time) && vec.first <= (range_t_All[i].second + width_range_time))
            {
                gr_raw_detectorNS_rate_vec_All_wide[i] -> SetPoint(gr_raw_detectorNS_rate_vec_All_wide[i]->GetN(), vec.first + 0.5, vec.second.back() - vec.second.front() + 1); 
            }           
        }

        int vec_size = Y_point_vec.size();
        int ind[Y_point_vec.size()];
        TMath::Sort(vec_size, &Y_point_vec[0], ind, false); // note : from small to large numbers 

        for (int pi = int(outlier_rejection_factor); pi < Y_point_vec.size(); pi++)
        {
            gr_raw_detectorNS_rate_vec_All[i] -> SetPoint(gr_raw_detectorNS_rate_vec_All[i]->GetN(), pair_vec[ind[pi]].first, pair_vec[ind[pi]].second);
            gr_raw_detectorNS_rate_vec_All[i] -> SetPointError(gr_raw_detectorNS_rate_vec_All[i]->GetN() - 1., pairE_vec[ind[pi]].first, pairE_vec[ind[pi]].second);
        }

        // double window_center = (gr_raw_detectorNS_rate_vec_All[i]->GetYaxis()->GetXmin() + gr_raw_detectorNS_rate_vec_All[i]->GetYaxis()->GetXmax()) / 2;
        // double window_width = ((gr_raw_detectorNS_rate_vec_All[i]->GetYaxis()->GetXmax() - gr_raw_detectorNS_rate_vec_All[i]->GetYaxis()->GetXmin()) / 2.);
        // h1D_raw_detectorNS_rate_vec_All.push_back(new TH1F("",Form("%s scan step %i;raw %s trigger rate [Hz] per point;Entry",direction_string.c_str(),i, detector_selection.c_str()),50, window_center - window_width * search_window, window_center + window_width * search_window));
        // h1D_raw_detectorNS_rate_vec_All[i] -> GetXaxis() -> SetNdivisions(505);
        // for (int pi = 0; pi < gr_raw_detectorNS_rate_vec_All[i] -> GetN(); pi++){h1D_raw_detectorNS_rate_vec_All[i] -> Fill(gr_raw_detectorNS_rate_vec_All[i] -> GetPointY(pi));}

        // fit_gaus_vec_All.push_back(new TF1("", "gaus", window_center - window_width, window_center + window_width));
        // fit_gaus_vec_All[i] -> SetParameters(
        //     h1D_raw_detectorNS_rate_vec_All[i]->GetBinContent(h1D_raw_detectorNS_rate_vec_All[i]->GetMaximumBin()), 
        //     h1D_raw_detectorNS_rate_vec_All[i]->GetBinCenter(h1D_raw_detectorNS_rate_vec_All[i]->GetMaximumBin()), 
        //     h1D_raw_detectorNS_rate_vec_All[i]->GetStdDev() * 1.3
        // );
        // fit_gaus_vec_All[i] -> SetParLimits(0, 0, h1D_raw_detectorNS_rate_vec_All[i]->GetBinContent(h1D_raw_detectorNS_rate_vec_All[i]->GetMaximumBin()) * 1.5);// note : par[0] : size
        // fit_gaus_vec_All[i] -> SetParLimits(1, 
        //     h1D_raw_detectorNS_rate_vec_All[i]->GetBinCenter(h1D_raw_detectorNS_rate_vec_All[i]->GetMaximumBin()) - h1D_raw_detectorNS_rate_vec_All[i]->GetStdDev() * 2.5, 
        //     h1D_raw_detectorNS_rate_vec_All[i]->GetBinCenter(h1D_raw_detectorNS_rate_vec_All[i]->GetMaximumBin()) + h1D_raw_detectorNS_rate_vec_All[i]->GetStdDev() * 2.5
        // ); // note : par[1] : mean
        // fit_gaus_vec_All[i] -> SetParLimits(2, window_center - window_width/2., window_center + window_width/2. );// note : par[2] : width
        // h1D_raw_detectorNS_rate_vec_All[i] -> Fit(fit_gaus_vec_All[i], "NQ");
        // double peak_location = h1D_raw_detectorNS_rate_vec_All[i] -> GetBinCenter(h1D_raw_detectorNS_rate_vec_All[i] -> GetMaximumBin());
        // double outlier_rejection_window = fabs(peak_location) * outlier_rejection_factor;
        
        outlier_rejection_factor_vec_All.push_back({pair_vec[ind[int(outlier_rejection_factor) - 1]].second, pair_vec[ind[int(outlier_rejection_factor) - 1]].second});


        // for (int pi = 0; pi < gr_raw_detectorNS_rate_vec_All[i] -> GetN(); pi++){
        //     // cout<<"pi : "<<pi<<" then, "<< fit_vec_All[i] -> GetParameter(0) * (1+outlier_rejection_factor) << " "
        //     // << fit_vec_All[i] -> GetParameter(0) * (1-outlier_rejection_factor) << " "
        //     // << gr_raw_detectorNS_rate_vec_All[i] -> GetPointY(pi) << endl;
        //     if (
        //         gr_raw_detectorNS_rate_vec_All[i] -> GetPointY(pi) < outlier_rejection_factor_vec_All[i].first ||
        //         gr_raw_detectorNS_rate_vec_All[i] -> GetPointY(pi) > outlier_rejection_factor_vec_All[i].second
        //     )
        //     {
        //         gr_raw_detectorNS_rate_vec_All[i] -> RemovePoint(pi);
        //         pi --;
        //     }
        //     // cout<<"test !!!!"<<pi<<endl;
        // }

        gr_raw_detectorNS_rate_vec_All[i] -> Fit(fit_vec_All[i], "NQ");
        DetectorNS_rate_avg_vec_All.push_back(fit_vec_All[i]->GetParameter(0));
        DetectorNS_rate_avg_vec_All_error.push_back(fit_vec_All[i]->GetParError(0));
    }

    // note : to compute the MBDNS vertex Z efficiency corrections
    if (detector_selection == "MBDNS" && MBD_zvtx_effi)
    {
        PrepareMBDvtxZEffi(
            direction_string,
            h1D_detectorNS_vertexZ_vecAll, 
            h1D_detectorNS_vertexZ_postCorr_vecAll, 
            detectorNS_zvtx_effi_correction_All
        );
    }

    // note : to compute the multi-collision correction factor
    for (int i = 0; i < step_selected_T_range_All.size(); i++)
    {        
        // cout<<"Vertical scan, Time range : "<<step_selected_T_range_All[i].second<<" - "<<step_selected_T_range_All[i].first<<" s = "<<step_selected_T_range_All[i].second - step_selected_T_range_All[i].first<<endl;
        double total_crossing = (step_selected_T_range_All[i].second - step_selected_T_range_All[i].first) * (n_bunches / n_bunches_space);
        double Ntrigger = (detector_selection == "MBDNS" && MBD_zvtx_effi) ? (step_counting_range_All[i].second - step_counting_range_All[i].first) * detectorNS_zvtx_effi_correction_All[i] : step_counting_range_All[i].second - step_counting_range_All[i].first;
        double the_ratio = Ntrigger / (total_crossing);
        double poisson_lambda = -1.0 * TMath::Log( 1 - the_ratio );
        double multi_collision_ratio = (1 - TMath::Poisson(0,poisson_lambda) - TMath::Poisson(1,poisson_lambda))/(1 - TMath::Poisson(0,poisson_lambda));
        double estimated_total_collision = TMath::Abs(poisson_lambda * total_crossing);
        double the_correction = estimated_total_collision / Ntrigger;
        // double collision_numerator = 0;
        // double collision_demoninator = 0;
        // for (int ni = 1; ni < 11; ni++)
        // {
        //     collision_numerator += TMath::Poisson(ni,poisson_lambda) * (ni);
        //     collision_demoninator += TMath::Poisson(ni,poisson_lambda);
        // }
        

        // cout<<"Vertical Ncrossing = "<<std::setprecision(10)<<total_crossing<<" Ntrigger : "<<Ntrigger<<" the ratio : "<<the_ratio<<" poi_lambda : "<<poisson_lambda<<"The rate array = {"
        // <<TMath::Poisson(0,poisson_lambda)<<", "
        // <<TMath::Poisson(1,poisson_lambda)<<", "
        // <<TMath::Poisson(2,poisson_lambda)<<", "
        // <<TMath::Poisson(3,poisson_lambda)<<", "
        // <<TMath::Poisson(4,poisson_lambda)<<"}, multi-collision rate : "<<multi_collision_ratio<<" estimated total collision : "<<estimated_total_collision<<" correction : "<<the_correction<<endl;
        cout<<direction_string<<" : "<<poisson_lambda<<" "
        << TMath::Poisson(0,poisson_lambda)<<" "
        << TMath::Poisson(1,poisson_lambda)<<" "
        << TMath::Poisson(2,poisson_lambda)<<" "
        << TMath::Poisson(3,poisson_lambda)<<" "
        << multi_collision_ratio<<" "
        << the_correction<<endl;

        multi_collision_correction_All.push_back(the_correction);
    }

    if (detector_selection == "MBDNS")
    {
        cout<<"=================================== =================================== =================================== ==================================="<<endl;

        for (int i = 0; i < range_t_All.size(); i++)
        {
            double sum_step_MBD30cm = 0;
            double sum_step_MBDinclusive = 0;

            for (auto pair : time_MBDNS_30cm_raw_counting_pair) 
            { 
                if (pair.first >= range_t_All[i].first && pair.first <= range_t_All[i].second)
                {
                    sum_step_MBD30cm += pair.second.first;
                    sum_step_MBDinclusive += pair.second.second;
                }    
            }

            cout<<direction_string<<" scan step "<<i<<" : MBDNS_30cm : "<<sum_step_MBD30cm<<", MBDNS_inclusive : "<<sum_step_MBDinclusive<<", ratio : "<<sum_step_MBD30cm / sum_step_MBDinclusive<<endl;
        }

        cout<<"=================================== =================================== =================================== ==================================="<<endl;
    }

    

    // note : for the accidental correction factor of ZDCNS
    // note : method quoted from Angelica's method
    if (detector_selection == "ZDCNS")
    {
        for (int i = 0; i < step_selected_T_range_All.size(); i++)
        {
            double duration_second = (step_selected_T_range_All[i].second - step_selected_T_range_All[i].first) * bco_span;
            double Ntrigger_ZDCNS = step_counting_range_All[i].second - step_counting_range_All[i].first;
            double Ntrigger_ZDCN = time_ZDCN_raw_counting[range_t_All[i].second].back() - time_ZDCN_raw_counting[range_t_All[i].first].front();
            double Ntrigger_ZDCS = time_ZDCS_raw_counting[range_t_All[i].second].back() - time_ZDCS_raw_counting[range_t_All[i].first].front();

            double ZDCNS_rate = fit_vec_All[i]->GetParameter(0); // note : the ZDCNS rate from the fitting
            double ZDCN_rate = Ntrigger_ZDCN / duration_second;
            double ZDCS_rate = Ntrigger_ZDCS / duration_second;
            double ZDCBY = (ZDCN_rate * ZDCS_rate) / (n_bunches * fbeam);
            double ZDC_StandardCorrected = ZDCNS_rate - ZDCBY;
            double ZDCNS_log_corr = (n_bunches * fbeam) * (-1.0*TMath::Log(1.0-ZDC_StandardCorrected/((fbeam*n_bunches+ZDCNS_rate)-ZDCN_rate-ZDCS_rate)));

            // cout<<direction_string<<" scan step "<<i<<" : ZDCNS rate : "<<ZDCNS_rate<<" Hz, ZDCN rate : "<<ZDCN_rate<<" Hz, ZDCS rate : "<<ZDCS_rate<<" Hz, ZDCBY : "<<ZDCBY<<" Hz, ZDC_StandardCorrected : "<<ZDC_StandardCorrected<<" Hz, ZDCNS_log_corr : "<<ZDCNS_log_corr<<" Hz"<<endl;

            accidental_correction_All.push_back(ZDCNS_log_corr / ZDCNS_rate);
            cout<<direction_string<<" scan step "<<i<<" acci_correction : "<<ZDCNS_log_corr / ZDCNS_rate<<endl;

        }
    }
}

void gl1_scaler_ana::PrepareMBDvtxZEffi(
    string direction_string,
    vector<TH1F *> &h1D_detectorNS_vertexZ_vecAll,
    vector<TH1F *> &h1D_detectorNS_vertexZ_postCorr_vecAll,
    vector<double> &detectorNS_zvtx_effi_correction_All
)
{   
    if (h1D_detectorNS_vertexZ_vecAll.size() != h1D_detectorNS_vertexZ_postCorr_vecAll.size()){
        cout<<"Error : h1D_detectorNS_vertexZ_vecAll.size() != h1D_detectorNS_vertexZ_postCorr_vecAll.size()"<<endl;
        exit(1);
    }

    cout<<"=================================== =================================== MBDNS zvtx effi corr =================================== ==================================="<<endl;    

    for (int i = 0; i < h1D_detectorNS_vertexZ_vecAll.size(); i++)
    {
        detectorNS_zvtx_effi_correction_All.push_back(h1D_detectorNS_vertexZ_postCorr_vecAll[i]->GetSumOfWeights() / h1D_detectorNS_vertexZ_vecAll[i]->GetSumOfWeights());
        cout<<direction_string<<" scan step "<<i<<" : MBDNS zvtx effi correction factor : "<<detectorNS_zvtx_effi_correction_All[i]<<endl;
    }   

    cout<<"=================================== =================================== MBDNS zvtx effi corr =================================== ==================================="<<endl;
}

void gl1_scaler_ana::CombineData()
{
    // note : combine BPM and MBD rate

    for (int i = 0; i < range_t_V.size(); i++)
    {
        double Y_point = DetectorNS_rate_avg_vecV[i];
        if (NCollision_corr) {
            cout<<"Vertical Scan : "<<Y_point<<" Hz, NCollision Correction factor : "<<multi_collision_correction_V[i]<<endl;
            Y_point = Y_point * multi_collision_correction_V[i];
        }
        if (beam_intensity_corr) {
            cout<<"Vertical Scan : "<<Y_point<<" Hz, BeamIntensity Correction factor : "<<BeamIntensity_corr_V[i]<<endl;
            cout<<endl;
            Y_point = Y_point * BeamIntensity_corr_V[i];
        }
        if (accidental_correction) {
            cout<<"Vertical Scan : "<<Y_point<<" Hz, Accidental Correction factor : "<<accidental_correction_V[i]<<endl;
            cout<<endl;
            Y_point = Y_point * accidental_correction_V[i];
        }

        gr_BPM_raw_detectorNS_rate_V -> SetPoint(i, Average_BPM_pos_V[i], Y_point);
        gr_BPM_raw_detectorNS_rate_V -> SetPointError(i, StdDev_BPM_pos_V[i], DetectorNS_rate_avg_vecV_error[i]);

        gr_BPM_raw_detectorNS_rate_V_demo -> SetPoint(i, Average_BPM_pos_V[i], Y_point);
        gr_BPM_raw_detectorNS_rate_V_demo -> SetPointError(i, StdDev_BPM_pos_V[i] * demo_factor.first, DetectorNS_rate_avg_vecV_error[i] * demo_factor.second);
    }

    for (int i = 0; i < range_t_H.size(); i++)
    {
        double Y_point = DetectorNS_rate_avg_vecH[i];
        if (NCollision_corr) {
            cout<<"Horizontal Scan : "<<Y_point<<" Hz, NCollision Correction factor : "<<multi_collision_correction_H[i]<<endl;
            Y_point = Y_point * multi_collision_correction_H[i];
        }
        if (beam_intensity_corr) {
            cout<<"Horizontal Scan : "<<Y_point<<" Hz, BeamIntensity Correction factor : "<<BeamIntensity_corr_H[i]<<endl;
            cout<<endl;
            Y_point = Y_point * BeamIntensity_corr_H[i];
        }
        if (accidental_correction) {
            cout<<"Horizontal Scan : "<<Y_point<<" Hz, Accidental Correction factor : "<<accidental_correction_H[i]<<endl;
            cout<<endl;
            Y_point = Y_point * accidental_correction_H[i];
        }

        gr_BPM_raw_detectorNS_rate_H -> SetPoint(i, Average_BPM_pos_H[i], Y_point);
        gr_BPM_raw_detectorNS_rate_H -> SetPointError(i, StdDev_BPM_pos_H[i], DetectorNS_rate_avg_vecH_error[i]);

        gr_BPM_raw_detectorNS_rate_H_demo -> SetPoint(i, Average_BPM_pos_H[i], Y_point);
        gr_BPM_raw_detectorNS_rate_H_demo -> SetPointError(i, StdDev_BPM_pos_H[i] * demo_factor.first, DetectorNS_rate_avg_vecH_error[i] * demo_factor.second);
    }

    cout<<"Vertical scan : "<<endl;
    for (int i = 0; i < range_t_V.size(); i++)
    {
        cout<<"BPM position : "<<Average_BPM_pos_V[i]<<" mm, "<<detector_selection<<" rate : "<<DetectorNS_rate_avg_vecV[i]<<" Hz, Error : "<<DetectorNS_rate_avg_vecV_error[i]<<" Hz"<<endl;
    }

    cout<<"Horizontal scan : "<<endl;
    for (int i = 0; i < range_t_H.size(); i++)
    {
        cout<<"BPM position : "<<Average_BPM_pos_H[i]<<" mm, "<<detector_selection<<" rate : "<<DetectorNS_rate_avg_vecH[i]<<" Hz, Error : "<<DetectorNS_rate_avg_vecH_error[i]<<" Hz"<<endl;
    }

    // note : par[0] : size
    // note : par[1] : mean
    // note : par[2] : width
    // note : par[3] : offset

    double max_location_V = gl1_scaler_ana::FindHighestEntry_gr(gr_BPM_raw_detectorNS_rate_V).first; 
    double max_V = gl1_scaler_ana::FindHighestEntry_gr(gr_BPM_raw_detectorNS_rate_V).second;
    double max_location_H = gl1_scaler_ana::FindHighestEntry_gr(gr_BPM_raw_detectorNS_rate_H).first; 
    double max_H = gl1_scaler_ana::FindHighestEntry_gr(gr_BPM_raw_detectorNS_rate_H).second;

    fit_Gaus_V -> SetParameters(max_V, max_location_V, nominal_width, 0);
    gr_BPM_raw_detectorNS_rate_V -> Fit(fit_Gaus_V, "N");
    fit_Gaus_H -> SetParameters(max_H, max_location_H, nominal_width, 0);
    gr_BPM_raw_detectorNS_rate_H -> Fit(fit_Gaus_H, "N");

    return;
}

template <typename T>
// typename std::enabel_if<std::is_base_of<TGraph, T>::value || std::is_base_of<TGraphErrors, T>::value, double>::type
pair<double, double> gl1_scaler_ana::FindHighestEntry_gr(T * gr)
{
    double max;
    double max_location;
    for (int i = 0; i < gr->GetN(); i++)
    {
        if (i == 0) {
            max = gr->GetPointY(i);
            max_location = gr->GetPointX(i);
        }
        else 
        {
            if (gr->GetPointY(i) > max) {
                max = gr->GetPointY(i);
                max_location = gr->GetPointX(i);
            }
        }
    }
    return {max_location, max};
}

// double gl1_scaler_ana::FindHighestEntry_gr(TObject * gr)
// {
//     if ()

//     if (gr -> ClassName() != std::string("TGraphErrors") && gr -> ClassName() != std::string("TGraph"))
//     {
//         cout<<"The input object is not TGraph or TGraphErrors, please check the input object"<<endl;
//         exit(1);
//     }


//     double max;
//     for (int i = 0; i < gr->GetN(); i++)
//     {
//         if (i == 0) {max = gr->GetPointY(i);}
//         else 
//         {
//             if (gr->GetPointY(i) > max) {max = gr->GetPointY(i);}
//         }
//     }
//     return max;
// }

// double gl1_scaler_ana::FindHighestEntry_gr(TGraph * gr)
// {
//     double max;
//     for (int i = 0; i < gr->GetN(); i++)
//     {
//         if (i == 0) {max = gr->GetPointY(i);}
//         else 
//         {
//             if (gr->GetPointY(i) > max) {max = gr->GetPointY(i);}
//         }
//     }
//     return max;
// }



void gl1_scaler_ana::DrawPlots()
{   
    c1 -> cd();

    gr_raw_ZDCS_rate -> Draw("AL");
    c1 -> Print(Form("%s/raw_ZDCS_rate.pdf", final_output_directory.c_str()));
    c1 -> SetLogy(1);
    c1 -> Print(Form("%s/raw_ZDCS_rate_log.pdf", final_output_directory.c_str()));
    c1 -> SetLogy(0);
    c1 -> Clear();

    gr_raw_ZDCN_rate -> Draw("AL");
    c1 -> Print(Form("%s/raw_ZDCN_rate.pdf", final_output_directory.c_str()));
    c1 -> SetLogy(1);
    c1 -> Print(Form("%s/raw_ZDCN_rate_log.pdf", final_output_directory.c_str()));
    c1 -> SetLogy(0);
    c1 -> Clear();

    gr_raw_ZDCNS_rate -> Draw("AL");
    c1 -> Print(Form("%s/raw_ZDCNS_rate.pdf", final_output_directory.c_str()));
    c1 -> SetLogy(1);
    c1 -> Print(Form("%s/raw_ZDCNS_rate_log.pdf", final_output_directory.c_str()));
    c1 -> SetLogy(0);
    c1 -> Clear();

    gr_raw_MBDS_rate -> Draw("AL");
    c1 -> Print(Form("%s/raw_MBDS_rate.pdf", final_output_directory.c_str()));
    c1 -> SetLogy(1);
    c1 -> Print(Form("%s/raw_MBDS_rate_log.pdf", final_output_directory.c_str()));
    c1 -> SetLogy(0);
    c1 -> Clear();
    
    gr_raw_MBDN_rate -> Draw("AL");
    c1 -> Print(Form("%s/raw_MBDN_rate.pdf", final_output_directory.c_str()));
    c1 -> SetLogy(1);
    c1 -> Print(Form("%s/raw_MBDN_rate_log.pdf", final_output_directory.c_str()));
    c1 -> SetLogy(0);
    c1 -> Clear();

    gr_raw_MBDNS_rate -> Draw("AL");
    c1 -> Print(Form("%s/raw_MBDNS_rate.pdf", final_output_directory.c_str()));
    c1 -> SetLogy(1);
    c1 -> Print(Form("%s/raw_MBDNS_rate_log.pdf", final_output_directory.c_str()));
    c1 -> SetLogy(0);
    c1 -> Clear();

    if (detector_selection == "MBDNS"){
        gr_raw_MBDNS_30cm_ratio -> Draw("AL");
        c1 -> Print(Form("%s/raw_MBDNS_30cm_ratio.pdf", final_output_directory.c_str()));
        c1 -> SetLogy(1);
        c1 -> Print(Form("%s/raw_MBDNS_30cm_ratio_log.pdf", final_output_directory.c_str()));
        c1 -> SetLogy(0);
        c1 -> Clear();
    }
    
    if (only_raw_rate_tag == true) {return;} // note : only the raw rate part

    // note : ============================ ============================ ============================ ============================ ============================

    c1 -> Print(Form("%s/raw_%s_rate_vecV.pdf(", final_output_directory.c_str(),detector_selection.c_str()));
    for (int i = 0; i < range_t_V.size(); i++)
    {
        c1 -> cd();
        double window_center = (gr_raw_detectorNS_rate_vecV[i]->GetYaxis()->GetXmin() + gr_raw_detectorNS_rate_vecV[i]->GetYaxis()->GetXmax()) / 2;
        double window_width = ((gr_raw_detectorNS_rate_vecV[i]->GetYaxis()->GetXmax() - gr_raw_detectorNS_rate_vecV[i]->GetYaxis()->GetXmin()) / 2.) * 5;
        gr_raw_detectorNS_rate_vecV_wide[i] -> GetYaxis() -> SetRangeUser(window_center - window_width, window_center + window_width);
        gr_raw_detectorNS_rate_vecV_wide[i] -> Draw("AL");
        gr_raw_detectorNS_rate_vecV[i] -> Draw("PSAME");
        fit_vecV[i] -> Draw("lsame");

        coord_line->DrawLine(
            gr_raw_detectorNS_rate_vecV_wide[i]->GetXaxis()->GetXmin(), outlier_rejection_factor_vecV[i].first, 
            gr_raw_detectorNS_rate_vecV_wide[i]->GetXaxis()->GetXmax(), outlier_rejection_factor_vecV[i].first
        );
        coord_line->DrawLine(
            gr_raw_detectorNS_rate_vecV_wide[i]->GetXaxis()->GetXmin(), outlier_rejection_factor_vecV[i].second, 
            gr_raw_detectorNS_rate_vecV_wide[i]->GetXaxis()->GetXmax(), outlier_rejection_factor_vecV[i].second
        );

        draw_text -> DrawLatex(0.22, 0.88, Form("Pol0 fit: %.3f #pm %.3f", fit_vecV[i]->GetParameter(0), fit_vecV[i]->GetParError(0)));

        c1 -> Print(Form("%s/raw_%s_rate_vecV.pdf", final_output_directory.c_str(),detector_selection.c_str()));
        // c1 -> SetLogy(1);
        // c1 -> Print(Form("%s/raw_%s_rate_vecV.pdf", final_output_directory.c_str(),detector_selection.c_str()));
        // c1 -> SetLogy(0);
        c1 -> Clear();
    }
    c1 -> Print(Form("%s/raw_%s_rate_vecV.pdf)", final_output_directory.c_str(),detector_selection.c_str()));
    c1 -> Clear();

    // note : ============================ ============================ ============================ ============================ ============================

    c1 -> Print(Form("%s/raw_%s_rate_vecH.pdf(", final_output_directory.c_str(),detector_selection.c_str()));
    for (int i = 0; i < range_t_V.size(); i++)
    {
        c1 -> cd();
        double window_center = (gr_raw_detectorNS_rate_vecH[i]->GetYaxis()->GetXmin() + gr_raw_detectorNS_rate_vecH[i]->GetYaxis()->GetXmax()) / 2;
        double window_width = ((gr_raw_detectorNS_rate_vecH[i]->GetYaxis()->GetXmax() - gr_raw_detectorNS_rate_vecH[i]->GetYaxis()->GetXmin()) / 2.) * 5;
        // cout<<"111"<<endl;
        gr_raw_detectorNS_rate_vecH_wide[i] -> GetYaxis() -> SetRangeUser(window_center - window_width, window_center + window_width);
        // cout<<"222"<<endl;
        gr_raw_detectorNS_rate_vecH_wide[i] -> Draw("AL");
        // cout<<"333"<<endl;
        gr_raw_detectorNS_rate_vecH[i] -> Draw("PSAME");
        // cout<<"444"<<endl;
        fit_vecH[i] -> Draw("lsame");
        // cout<<"555"<<endl;

        coord_line->DrawLine(
            gr_raw_detectorNS_rate_vecH_wide[i]->GetXaxis()->GetXmin(), outlier_rejection_factor_vecH[i].first, 
            gr_raw_detectorNS_rate_vecH_wide[i]->GetXaxis()->GetXmax(), outlier_rejection_factor_vecH[i].first
        );
        // cout<<"666"<<endl;
        coord_line->DrawLine(
            gr_raw_detectorNS_rate_vecH_wide[i]->GetXaxis()->GetXmin(), outlier_rejection_factor_vecH[i].second, 
            gr_raw_detectorNS_rate_vecH_wide[i]->GetXaxis()->GetXmax(), outlier_rejection_factor_vecH[i].second
        );
        // cout<<"777"<<endl;

        draw_text -> DrawLatex(0.22, 0.88, Form("Pol0 fit: %.3f #pm %.3f", fit_vecV[i]->GetParameter(0), fit_vecV[i]->GetParError(0)));

        // cout<<"888"<<endl;
        
        c1 -> Print(Form("%s/raw_%s_rate_vecH.pdf", final_output_directory.c_str(),detector_selection.c_str()));

        // c1 -> SetLogy(1);
        // c1 -> Print(Form("%s/raw_%s_rate_vecH.pdf", final_output_directory.c_str(),detector_selection.c_str()));
        // c1 -> SetLogy(0);
        c1 -> Clear();
    }
    c1 -> Print(Form("%s/raw_%s_rate_vecH.pdf)", final_output_directory.c_str(),detector_selection.c_str()));
    c1 -> Clear();

    // note : ============================ ============================ ============================ ============================ ============================

    c1 -> cd();
    c1 -> Print(Form("%s/h1D_raw_%s_rate_vecV.pdf(", final_output_directory.c_str(), detector_selection.c_str()));
    for (int i = 0; i < h1D_raw_detectorNS_rate_vecV.size(); i++)
    {
        h1D_raw_detectorNS_rate_vecV[i] -> Draw();
        // fit_gaus_vecV[i] -> SetLineColor(kRed);
        // fit_gaus_vecV[i] -> Draw("lsame");
        
        coord_line->DrawLine(
            outlier_rejection_factor_vecV[i].first, h1D_raw_detectorNS_rate_vecV[i]->GetMinimum(), 
            outlier_rejection_factor_vecV[i].first, h1D_raw_detectorNS_rate_vecV[i]->GetMaximum()
        );
        coord_line->DrawLine(
            outlier_rejection_factor_vecV[i].second, h1D_raw_detectorNS_rate_vecV[i]->GetMinimum(), 
            outlier_rejection_factor_vecV[i].second, h1D_raw_detectorNS_rate_vecV[i]->GetMaximum()
        );

        c1 -> Print(Form("%s/h1D_raw_%s_rate_vecV.pdf", final_output_directory.c_str(), detector_selection.c_str()));
        c1 -> Clear();
    }
    c1 -> Print(Form("%s/h1D_raw_%s_rate_vecV.pdf)", final_output_directory.c_str(), detector_selection.c_str()));
    c1 -> Clear();

    // note : ============================ ============================ ============================ ============================ ============================

    c1 -> cd();
    c1 -> Print(Form("%s/h1D_raw_%s_rate_vecH.pdf(", final_output_directory.c_str(), detector_selection.c_str()));
    for (int i = 0; i < h1D_raw_detectorNS_rate_vecH.size(); i++)
    {
        h1D_raw_detectorNS_rate_vecH[i] -> Draw();
        // fit_gaus_vecH[i] -> SetLineColor(kRed);
        // fit_gaus_vecH[i] -> Draw("lsame");
        
        coord_line->DrawLine(
            outlier_rejection_factor_vecH[i].first, h1D_raw_detectorNS_rate_vecH[i]->GetMinimum(), 
            outlier_rejection_factor_vecH[i].first, h1D_raw_detectorNS_rate_vecH[i]->GetMaximum()
        );
        coord_line->DrawLine(
            outlier_rejection_factor_vecH[i].second, h1D_raw_detectorNS_rate_vecH[i]->GetMinimum(), 
            outlier_rejection_factor_vecH[i].second, h1D_raw_detectorNS_rate_vecH[i]->GetMaximum()
        );

        c1 -> Print(Form("%s/h1D_raw_%s_rate_vecH.pdf", final_output_directory.c_str(), detector_selection.c_str()));
        c1 -> Clear();
    }
    c1 -> Print(Form("%s/h1D_raw_%s_rate_vecH.pdf)", final_output_directory.c_str(), detector_selection.c_str()));
    c1 -> Clear();

    // note : ============================ ============================ ============================ ============================ ============================
    if (detector_selection == "MBDNS")
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

    // note : ============================ ============================ ============================ ============================ ============================

    c1 -> cd();
    gr_BPM_raw_detectorNS_rate_V -> GetYaxis() -> SetRangeUser( 0, (gl1_scaler_ana::FindHighestEntry_gr(gr_BPM_raw_detectorNS_rate_V)).second * 1.7 );
    gr_BPM_raw_detectorNS_rate_V -> Draw("AP");
    gr_BPM_raw_detectorNS_rate_V_demo -> Draw("Psame");
    fit_Gaus_V -> Draw("lsame");
    draw_text -> DrawLatex(0.22, 0.88, Form("#splitline{Error bars are timed %.0f (X), %.0f (Y) for visibility,}{original error bars included in the fit}", demo_factor.first, demo_factor.second));
    draw_text -> DrawLatex(0.22, 0.80, Form("Fit height: %.3f #pm %.3f", fit_Gaus_V->GetParameter(0), fit_Gaus_V->GetParError(0)));
    draw_text -> DrawLatex(0.22, 0.76, Form("Fit mean: %.3f #pm %.6f mm", fit_Gaus_V->GetParameter(1), fit_Gaus_V->GetParError(1)));
    draw_text -> DrawLatex(0.22, 0.72, Form("Fit width: %.5f #pm %.6f mm", fit_Gaus_V->GetParameter(2), fit_Gaus_V->GetParError(2)));
    double text_shift = (fit_Gaus_V -> GetNpar() == 4) ? 0.04 : 0.0;
    if (fit_Gaus_V -> GetNpar() == 4) {draw_text -> DrawLatex(0.22, 0.68, Form("Fit offset: %.5f #pm %.5f", fit_Gaus_V->GetParameter(3), fit_Gaus_V->GetParError(3)));}
    
    draw_text -> DrawLatex(0.22, 0.68 - text_shift, Form("Reduced #chi^{2} : %.5f", fit_Gaus_V->GetChisquare()/double(fit_Gaus_V->GetNDF())));
    c1 -> Print(Form("%s/BPM_raw_%s_rate_V.pdf", final_output_directory.c_str(),detector_selection.c_str()));
    c1 -> Clear();

    c1 -> cd();
    gr_BPM_raw_detectorNS_rate_H -> GetYaxis() -> SetRangeUser( 0, (gl1_scaler_ana::FindHighestEntry_gr(gr_BPM_raw_detectorNS_rate_H)).second * 1.7 );
    gr_BPM_raw_detectorNS_rate_H -> Draw("AP");
    gr_BPM_raw_detectorNS_rate_H_demo -> Draw("Psame");
    fit_Gaus_H -> Draw("lsame");
    draw_text -> DrawLatex(0.22, 0.88, Form("#splitline{Error bars are timed %.0f (X), %.0f (Y) for visibility,}{original error bars included in the fit}", demo_factor.first, demo_factor.second));
    draw_text -> DrawLatex(0.22, 0.80, Form("Fit height: %.3f #pm %.3f", fit_Gaus_H->GetParameter(0), fit_Gaus_H->GetParError(0)));
    draw_text -> DrawLatex(0.22, 0.76, Form("Fit mean: %.3f #pm %.6f mm", fit_Gaus_H->GetParameter(1), fit_Gaus_H->GetParError(1)));
    draw_text -> DrawLatex(0.22, 0.72, Form("Fit width: %.5f #pm %.6f mm", fit_Gaus_H->GetParameter(2), fit_Gaus_H->GetParError(2)));
    if (fit_Gaus_H -> GetNpar() == 4) {draw_text -> DrawLatex(0.22, 0.68, Form("Fit offset: %.5f #pm %.5f", fit_Gaus_H->GetParameter(3), fit_Gaus_H->GetParError(3)));}
    draw_text -> DrawLatex(0.22, 0.68 - text_shift, Form("Reduced #chi^{2} : %.5f", fit_Gaus_H->GetChisquare()/double(fit_Gaus_H->GetNDF())));
    c1 -> Print(Form("%s/BPM_raw_%s_rate_H.pdf", final_output_directory.c_str(),detector_selection.c_str()));
    c1 -> Clear();

    cout<<"The final gaussian + offset height : "<<std::setprecision(10)<<((fit_Gaus_V->GetParameter(0) + fit_Gaus_V -> GetParameter(3)) + (fit_Gaus_H->GetParameter(0) + fit_Gaus_H -> GetParameter(3)))/2.<<endl;

}

void gl1_scaler_ana::ClearUp()
{
    gr_raw_ZDCS_rate -> Set(0);
    gr_raw_ZDCN_rate -> Set(0);
    gr_raw_ZDCNS_rate -> Set(0);
    gr_raw_MBDS_rate -> Set(0);
    gr_raw_MBDN_rate -> Set(0);
    gr_raw_MBDNS_rate -> Set(0);
    gr_raw_MBDNS_30cm_ratio -> Set(0);

    
    step_selected_T_range_V.clear();
    step_counting_range_V.clear();
    multi_collision_correction_V.clear();
    accidental_correction_V.clear();
    gr_BPM_raw_detectorNS_rate_V -> Set(0);
    gr_BPM_raw_detectorNS_rate_V_demo -> Set(0);
    fit_Gaus_V -> SetParameters(9.32502e+05, -8.97690e-01, 2.99013e-01, 0);

    step_selected_T_range_H.clear();
    step_counting_range_H.clear();
    multi_collision_correction_H.clear();
    accidental_correction_H.clear();
    gr_BPM_raw_detectorNS_rate_H -> Set(0);
    gr_BPM_raw_detectorNS_rate_H_demo -> Set(0);
    fit_Gaus_H -> SetParameters(9.56217e+05, 1.44200e+00, 3.01624e-01, 0);

    DetectorNS_rate_avg_vecV.clear();
    DetectorNS_rate_avg_vecV_error.clear();
    DetectorNS_rate_avg_vecH.clear();
    DetectorNS_rate_avg_vecH_error.clear();

    for (int i = 0; i < range_t_V.size(); i++)
    {
        gr_raw_detectorNS_rate_vecV[i] -> Set(0);
        gr_raw_detectorNS_rate_vecV_wide[i] -> Set(0);
        fit_vecV[i] -> SetParameter(0, 0);
        // fit_gaus_vecV[i] -> SetParameters(0,0,0,0);

        
        gr_raw_detectorNS_rate_vecH[i] -> Set(0);
        gr_raw_detectorNS_rate_vecH_wide[i] -> Set(0);
        fit_vecH[i] -> SetParameter(0, 0);
        // fit_gaus_vecH[i] -> SetParameters(0,0,0,0);
    }

    h1D_raw_detectorNS_rate_vecV.clear();
    h1D_raw_detectorNS_rate_vecH.clear();
    outlier_rejection_factor_vecV.clear();
    outlier_rejection_factor_vecH.clear();
    fit_gaus_vecV.clear();
    fit_gaus_vecH.clear();
}

void gl1_scaler_ana::SaveHistROOT()
{
    TFile * file_in = new TFile(Form("%s/hist_out.root", final_output_directory.c_str()), "RECREATE");
    if (detector_selection == "MBDNS")
    {
        for (int i = 0; i < h1D_detectorNS_vertexZ_vecV.size(); i++)
        {
            h1D_detectorNS_vertexZ_vecV[i] -> Write(Form("MBDvtxZ_Vertical_scan_step_%d", i));
        }

        for (int i = 0; i < h1D_detectorNS_vertexZ_vecH.size(); i++)
        {
            h1D_detectorNS_vertexZ_vecH[i] -> Write(Form("MBDvtxZ_Horizontal_scan_step_%d", i));
        }
    }

    file_in -> Close();

}

void gl1_scaler_ana::PrintInfo(string scan_direction_string, vector<pair<string, TGraphErrors *>> vector_in, int column_size)
{

    cout<<"================================= ================================= ================================= ================================="<<endl;
    std::cout<<std::setw(column_size)<<Form("%s pos", scan_direction_string.c_str());
    for (const auto& pair : vector_in) {
        std::cout << std::setw(column_size) << pair.first;
    }
    std::cout << std::endl;

    // Print the elements of the vectors in a table format
    for (size_t i = 0; i < (vector_in[0].second) -> GetN(); ++i) {
        std::cout << std::setw(column_size) << (vector_in[0].second) -> GetX()[i];
        
        for (const auto& pair : vector_in) {
            std::cout << std::setw(column_size) << (pair.second) -> GetY()[i];
        }

        std::cout << std::endl;
    }
}

double  gl1_scaler_ana::tg_average (TGraph * input_tgraph) {
    vector<double> input_vector;
    for (int i = 0; i < input_tgraph->GetN(); i++){ input_vector.push_back(input_tgraph->GetPointY(i));}
	return accumulate( input_vector.begin(), input_vector.end(), 0.0 ) / double(input_vector.size());
}

double gl1_scaler_ana::tg_stddev (TGraph * input_tgraph) {
    vector<double> input_vector;
    for (int i = 0; i < input_tgraph->GetN(); i++){ input_vector.push_back(input_tgraph->GetPointY(i));}
	
	double sum_subt = 0;
	double average  = accumulate( input_vector.begin(), input_vector.end(), 0.0 ) / double(input_vector.size());
	
	// cout<<"average is : "<<average<<endl;

	for (int i=0; i<input_vector.size(); i++){ sum_subt += pow((input_vector[i] - average),2); }

	//cout<<"sum_subt : "<<sum_subt<<endl;
	// cout<<"print from the function, average : "<<average<<" std : "<<stddev<<endl;

	return sqrt( sum_subt / double(input_vector.size()-1) );
}	

void gl1_scaler_ana::ImportCADReadings(string cad_reading_directory, bool SD_column)
{
    std::ifstream file(cad_reading_directory.c_str());

    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        std::cout<< "Please check the file path : "<<cad_reading_directory<<std::endl;
        exit(1);
    }

    std::string line;

    // Skip the header line
    std::getline(file, line);

    // Vectors to store the data from each column
    std::vector<double> Average_H; Average_H.clear();
    std::vector<double> DCCT_correction_H; DCCT_correction_H.clear();
    std::vector<double> Average_V; Average_V.clear();
    std::vector<double> DCCT_correction_V; DCCT_correction_V.clear();
    std::vector<float> DCCT_B_avg; DCCT_B_avg.clear();
    std::vector<float> DCCT_B_std; DCCT_B_std.clear();
    std::vector<float> DCCT_Y_avg; DCCT_Y_avg.clear();
    std::vector<float> DCCT_Y_std; DCCT_Y_std.clear();

    std::vector<double> StdDev_H; StdDev_H.clear();
    std::vector<double> StdDev_V; StdDev_V.clear();

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;

        std::getline(ss, cell, '\t');
        Average_H.push_back(std::stof(cell));

        std::getline(ss, cell, '\t');
        DCCT_correction_H.push_back(std::stof(cell));

        std::getline(ss, cell, '\t');
        Average_V.push_back(std::stof(cell));

        std::getline(ss, cell, '\t');
        DCCT_correction_V.push_back(std::stof(cell));

        // note : the beam intensity for the machine luminosity calculation
        std::getline(ss, cell, '\t');
        DCCT_B_avg.push_back(std::stof(cell));

        std::getline(ss, cell, '\t');
        DCCT_B_std.push_back(std::stof(cell));

        std::getline(ss, cell, '\t');
        DCCT_Y_avg.push_back(std::stof(cell));

        std::getline(ss, cell, '\t');
        DCCT_Y_std.push_back(std::stof(cell));

        if (SD_column) {
            std::getline(ss, cell, '\t');
            StdDev_H.push_back(std::stof(cell));

            std::getline(ss, cell, '\t');
            StdDev_V.push_back(std::stof(cell));
        }
        else {
            StdDev_H.push_back(0);
            StdDev_V.push_back(0);
        }
    }

    file.close();


    std::cout << std::setw(15) << "Average_H"
              << std::setw(15) << "DCCT_corr_H"
              << std::setw(15) << "Average_V"
              << std::setw(15) << "DCCT_corr_V"
              << std::setw(15) << "DCCT_B_avg"
              << std::setw(15) << "DCCT_B_std"
              << std::setw(15) << "DCCT_Y_avg"
              << std::setw(15) << "DCCT_Y_std"            
              << std::setw(15) << "StdDev_H"
              << std::setw(15) << "StdDev_V"

              << std::endl;

    // Determine the maximum number of elements
    size_t maxSize = std::max({Average_H.size(), DCCT_correction_H.size(), Average_V.size(), DCCT_correction_V.size(), DCCT_B_avg.size(), DCCT_B_std.size(), DCCT_Y_avg.size(), DCCT_Y_std.size(), StdDev_H.size(), StdDev_V.size()});

    // Print the content of the vectors
    for (size_t i = 0; i < maxSize; ++i) {
        std::cout << std::setw(15) << (i < Average_H.size() ? std::to_string(Average_H[i]) : "")
                  << std::setw(15) << (i < DCCT_correction_H.size() ? std::to_string(DCCT_correction_H[i]) : "")
                  << std::setw(15) << (i < Average_V.size() ? std::to_string(Average_V[i]) : "")
                  << std::setw(15) << (i < DCCT_correction_V.size() ? std::to_string(DCCT_correction_V[i]) : "")
                  << std::setw(15) << (i < DCCT_B_avg.size() ? std::to_string(DCCT_B_avg[i]) : "")
                  << std::setw(15) << (i < DCCT_B_std.size() ? std::to_string(DCCT_B_std[i]) : "")
                  << std::setw(15) << (i < DCCT_Y_avg.size() ? std::to_string(DCCT_Y_avg[i]) : "")
                  << std::setw(15) << (i < DCCT_Y_std.size() ? std::to_string(DCCT_Y_std[i]) : "")
                  << std::setw(15) << (i < StdDev_H.size() ? std::to_string(StdDev_H[i]) : "")
                  << std::setw(15) << (i < StdDev_V.size() ? std::to_string(StdDev_V[i]) : "")
                  
                  << std::endl;
    }

    BPM_StdDev = SD_column;

    if (use_set_pos) {
        Average_BPM_pos_V = BPM_set_pos_V;
    }
    else {
        Average_BPM_pos_V = Average_V;
    }
    StdDev_BPM_pos_V = StdDev_V;
    BeamIntensity_corr_V = DCCT_correction_V;
    

    if (use_set_pos) {
        Average_BPM_pos_H = BPM_set_pos_H;
    }
    else {
        Average_BPM_pos_H = Average_H;
    }
    StdDev_BPM_pos_H = StdDev_H;
    BeamIntensity_corr_H = DCCT_correction_H;
    
    DCCT_B = DCCT_B_avg[0];
    DCCT_Y = DCCT_Y_avg[0];

}

void gl1_scaler_ana::GetMachineLumi()
{
    double DCCT_B_one_bunch = DCCT_B * pow(10, 9) / n_bunches; // note : beam intensity of blue beam, take the average of the whole trend
    double DCCT_Y_one_bunch = DCCT_Y * pow(10, 9) / n_bunches; // note : beam intensity of yellow beam, take the average of the whole trend

    double Gaus_V_width = fit_Gaus_V -> GetParameter(2); // todo : if we end up with having the double gaussian, modify here 
    double Gaus_H_width = fit_Gaus_H -> GetParameter(2); // todo : if we end up with having the double gaussian, modify here 

    double width_beam_overlap_X = Gaus_H_width * 1000.; // note : unit : um
    double width_beam_overlap_Y = Gaus_V_width * 1000.; // note : unit : um

    machine_lumi = (fbeam / (2 * M_PI * width_beam_overlap_X * width_beam_overlap_Y)) * (DCCT_B_one_bunch * DCCT_Y_one_bunch) * um2_to_mb;
    cout<<"================= ================= ================= ================= ================= ================= ================= ================="<<endl;
    cout<<"DCCT_B_one_bunch : "<<DCCT_B_one_bunch<<", DCCT_Y_one_bunch : "<<DCCT_Y_one_bunch<<endl;
    cout<<"Final directory : "<<final_output_directory<<endl;
    cout<<"width_beam_overlap_X : "<<width_beam_overlap_X<<" um, width_beam_overlap_Y : "<<width_beam_overlap_Y<<" um"<<endl;
    cout<<"Machine luminosity : "<<machine_lumi<<" mb^-1 s^-1"<<endl;
}

void gl1_scaler_ana::GetDetectorCrossSection()
{
    cout<<"The final gaussian + offset height : "<<endl;

    double V_gaus_total_height = fit_Gaus_V->GetParameter(0) + fit_Gaus_V -> GetParameter(3); // todo : if we end up with having the double gaussian, modify here
    double H_gaus_total_height = fit_Gaus_H->GetParameter(0) + fit_Gaus_H -> GetParameter(3); // todo : if we end up with having the double gaussian, modify here
    double Rmax_avg =  ((V_gaus_total_height + H_gaus_total_height) / 2.);
    Rmax_avg_single_bunch =   Rmax_avg/ n_bunches;
    double vertexZ_effi = 1.0; // todo : if we end up with changing the selecting region of vertex Z, we will have to look in to the detector efficiency

    detector_cross_section = Rmax_avg_single_bunch / (machine_lumi * vertexZ_effi);
    cout<<"Average Rmax : "<<Rmax_avg<<" Hz, Rmax_avg_single_bunch : "<<Rmax_avg_single_bunch<<" Hz " <<endl; 
    cout<< detector_selection << " cross section : "<<detector_cross_section<<" mb"<<endl;
    cout<<"\n\n\n\n\n"<<endl;

}

void gl1_scaler_ana::GetInformation()
{
    cout<<std::setw(15)<<"--->"<<detector_selection<<"_"<<NCollision_corr<<"_"<<beam_intensity_corr<<"_"<<accidental_correction<<"_"<<use_set_pos
    <<std::setw(7)<<", H_width: "<<fit_Gaus_H->GetParameter(2) <<" mm"
    <<std::setw(7)<<", V_width: "<<fit_Gaus_V->GetParameter(2) <<" mm"
    <<std::setw(7)<<", Machine Lumi : "<<machine_lumi<<" mb^-1 s^-1"
    <<std::setw(7)<<", cross section : "<<detector_cross_section<<" mb"
    <<std::setw(7)<<", avg. Rmax : "<<Rmax_avg_single_bunch<<" Hz"
    <<endl;


}
