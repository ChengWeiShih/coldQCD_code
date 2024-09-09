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
chosen_scan_steps({0, 6}),
general_hist_range({-300, 300}),
general_Nbins(100)
{
    gl1_scaler_ana::PrepareRate((input_directory_in + "/" + input_rate_file).c_str());
    final_output_directory = output_directory + "/vtx_effi";
    std::system(Form("if [ ! -d \"%s\" ]; then mkdir %s; fi;", final_output_directory.c_str(), final_output_directory.c_str()));
    
    TGaxis::SetMaxDigits(4);

    c2 = new TCanvas("c2", "c2", 950 * chosen_scan_steps.size(), 1600);
    c2 -> Divide(chosen_scan_steps.size(), 2);

    h1D_detectorNS_vtxZ_mapV.clear();
    h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_inclusive_vec"] = vector<TH1F *>();
    h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_narrow_vec"] = vector<TH1F *>(); // note : MBDNS vtxZ < \pm 30 cm
    h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_ZDCNSTrig_vec"] = vector<TH1F *>();
    h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_MBDNSTrig_vec"] = vector<TH1F *>();
    h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_inclusive_vec"] = vector<TH1F *>();
    h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"] = vector<TH1F *>();
    h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx_vec"] = vector<TH1F *>(); // note : in addition to the ZDCNS trigger, the MBD vertex has to be available. 
    h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig_vec"] = vector<TH1F *>(); // note : in addition to the ZDCNS trigger, the MBSNS trigger is required as well
    h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_MBDNSTrig_vec"] = vector<TH1F *>();
    h1D_detectorNS_vtxZ_mapV["h1D_MBD_effi_vec"] = vector<TH1F *>(); // note : the distribution regarding the MBD efficiency (ZDCNS+MBDvtxZ / ZDCNS)
    h1D_detectorNS_vtxZ_mapV["h1D_MBD_effi_vec2"] = vector<TH1F *>(); // note : the distribution regarding the MBD efficiency (ZDCNS+MBDNS / ZDCNS)

    h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_inclusive_vec"].clear();
    h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_ZDCNSTrig_vec"].clear();
    h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_MBDNSTrig_vec"].clear();
    h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_inclusive_vec"].clear();
    h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"].clear();
    h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx_vec"].clear();
    h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig_vec"].clear();
    h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_MBDNSTrig_vec"].clear();
    h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_narrow_vec"].clear();
    h1D_detectorNS_vtxZ_mapV["h1D_MBD_effi_vec"].clear();
    h1D_detectorNS_vtxZ_mapV["h1D_MBD_effi_vec2"].clear();



    h1D_detectorNS_vtxZ_mapH.clear();
    h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_inclusive_vec"] = vector<TH1F *>();
    h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_narrow_vec"] = vector<TH1F *>(); // note : MBDNS vtxZ < \pm 30 cm
    h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_ZDCNSTrig_vec"] = vector<TH1F *>();
    h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_MBDNSTrig_vec"] = vector<TH1F *>();
    h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_inclusive_vec"] = vector<TH1F *>();
    h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"] = vector<TH1F *>();
    h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx_vec"] = vector<TH1F *>(); // note : in addition to the ZDCNS trigger, the MBD vertex has to be available.
    h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig_vec"] = vector<TH1F *>(); // note : in addition to the ZDCNS trigger, the MBSNS trigger is required as well
    h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_MBDNSTrig_vec"] = vector<TH1F *>();
    h1D_detectorNS_vtxZ_mapH["h1D_MBD_effi_vec"] = vector<TH1F *>(); // note : the distribution regarding the MBD efficiency (ZDCNS+MBDvtxZ / ZDCNS)
    h1D_detectorNS_vtxZ_mapH["h1D_MBD_effi_vec2"] = vector<TH1F *>(); // note : the distribution regarding the MBD efficiency (ZDCNS+MBDNS / ZDCNS)
    
    h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_narrow_vec"].clear();
    h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_inclusive_vec"].clear();
    h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_ZDCNSTrig_vec"].clear();
    h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_MBDNSTrig_vec"].clear();
    h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_inclusive_vec"].clear();
    h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"].clear();
    h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx_vec"].clear();
    h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig_vec"].clear();
    h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_MBDNSTrig_vec"].clear();
    h1D_detectorNS_vtxZ_mapH["h1D_MBD_effi_vec"].clear();
    h1D_detectorNS_vtxZ_mapH["h1D_MBD_effi_vec2"].clear();

    c3 = new TCanvas("c3", "c3", 1500, 800);
    c3 -> Divide(4, 3);

    string effi_Y_title = "(ZDCNS+MBDvtx)/(ZDCNS)";
    string effi2_Y_title = "(ZDCNS+MBDNS)/(ZDCNS)";

    for (int i = 0; i < range_t_V.size(); i++)
    {
        h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_inclusive_vec"].push_back(new TH1F("",";MBD_vtxZ [cm] (V, inclusive);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_inclusive_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_ZDCNSTrig_vec"].push_back(new TH1F("",";MBD_vtxZ [cm] (V, ZDCNSTrig);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_ZDCNSTrig_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_MBDNSTrig_vec"].push_back(new TH1F("",";MBD_vtxZ [cm] (V, MBDNSTrig);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_MBDNSTrig_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_narrow_vec"].push_back(new TH1F("",";MBD_vtxZ [cm] (V, MBDNSTrig30);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapV["h1D_MBDNSVtxZ_narrow_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_inclusive_vec"].push_back(new TH1F("",";ZDC_vtxZ [cm] (V, inclusive);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_inclusive_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"].push_back(new TH1F("",";ZDC_vtxZ [cm] (V, ZDCNSTrig);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx_vec"].push_back(new TH1F("",";ZDC_vtxZ [cm] (V, ZDCNSTrig_MBDvtx);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig_vec"].push_back(new TH1F("",";ZDC_vtxZ [cm] (V, ZDCNSTrig_MBDNSTrig);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_MBDNSTrig_vec"].push_back(new TH1F("",";ZDC_vtxZ [cm] (V, MBDNSTrig);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_MBDNSTrig_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapV["h1D_MBD_effi_vec"].push_back(new TH1F("",Form(";ZDC_vtxZ [cm] (V_%d);%s", i, effi_Y_title.c_str()),general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapV["h1D_MBD_effi_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapV["h1D_MBD_effi_vec2"].push_back(new TH1F("",Form(";ZDC_vtxZ [cm] (V_%d);%s", i, effi2_Y_title.c_str()),general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapV["h1D_MBD_effi_vec2"].back() -> GetXaxis() -> SetNdivisions(505);
    }

    for (int i = 0; i < range_t_H.size(); i++)
    {
        h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_inclusive_vec"].push_back(new TH1F("",";MBD_vtxZ [cm] (H, inclusive);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_inclusive_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_ZDCNSTrig_vec"].push_back(new TH1F("",";MBD_vtxZ [cm] (H, ZDCNSTrig);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_ZDCNSTrig_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_MBDNSTrig_vec"].push_back(new TH1F("",";MBD_vtxZ [cm] (H, MBDNSTrig);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_MBDNSTrig_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_narrow_vec"].push_back(new TH1F("",";MBD_vtxZ [cm] (H, MBDNSTrig30);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapH["h1D_MBDNSVtxZ_narrow_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_inclusive_vec"].push_back(new TH1F("",";ZDC_vtxZ [cm] (H, inclusive);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_inclusive_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"].push_back(new TH1F("",";ZDC_vtxZ [cm] (H, ZDCNSTrig);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx_vec"].push_back(new TH1F("",";ZDC_vtxZ [cm] (H, ZDCNSTrig_MBDvtx);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig_vec"].push_back(new TH1F("",";ZDC_vtxZ [cm] (H, ZDCNSTrig_MBDNSTrig);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_MBDNSTrig_vec"].push_back(new TH1F("",";ZDC_vtxZ [cm] (H, MBDNSTrig);Entries",general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_MBDNSTrig_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapH["h1D_MBD_effi_vec"].push_back(new TH1F("",Form(";ZDC_vtxZ [cm] (H_%d);%s", i, effi_Y_title.c_str()),general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapH["h1D_MBD_effi_vec"].back() -> GetXaxis() -> SetNdivisions(505);

        h1D_detectorNS_vtxZ_mapH["h1D_MBD_effi_vec2"].push_back(new TH1F("",Form(";ZDC_vtxZ [cm] (H_%d);%s", i, effi2_Y_title.c_str()),general_Nbins, general_hist_range.first, general_hist_range.second));
        h1D_detectorNS_vtxZ_mapH["h1D_MBD_effi_vec2"].back() -> GetXaxis() -> SetNdivisions(505);
    }


    effi_fitting_vecV.clear();
    effi_fitting_vecH.clear();

    effi_fitting_vecV["h1D_MBD_effi_vec"] = vector<TF1 *>(); effi_fitting_vecV["h1D_MBD_effi_vec"].clear();
    effi_fitting_vecV["h1D_MBD_effi_vec2"] = vector<TF1 *>(); effi_fitting_vecV["h1D_MBD_effi_vec2"].clear();

    effi_fitting_vecH["h1D_MBD_effi_vec"] = vector<TF1 *>(); effi_fitting_vecH["h1D_MBD_effi_vec"].clear();
    effi_fitting_vecH["h1D_MBD_effi_vec2"] = vector<TF1 *>(); effi_fitting_vecH["h1D_MBD_effi_vec2"].clear();

    for (int i = 0; i < range_t_V.size(); i++)
    {
        effi_fitting_vecV["h1D_MBD_effi_vec"].push_back(new TF1(Form("MBDEffi_%d",i),gaus_func,general_hist_range.first, general_hist_range.second,4));
        effi_fitting_vecV["h1D_MBD_effi_vec"].back() -> SetLineColor(kRed);
        effi_fitting_vecV["h1D_MBD_effi_vec2"].push_back(new TF1(Form("MBDEffi2_%d",i),gaus_func,general_hist_range.first, general_hist_range.second,4));
        effi_fitting_vecV["h1D_MBD_effi_vec2"].back() -> SetLineColor(kRed);
    }

    for (int i = 0; i < range_t_H.size(); i++)
    {
        effi_fitting_vecH["h1D_MBD_effi_vec"].push_back(new TF1(Form("MBDEffi_%d",i),gaus_func,general_hist_range.first, general_hist_range.second,4));
        effi_fitting_vecH["h1D_MBD_effi_vec"].back() -> SetLineColor(kRed);
        effi_fitting_vecH["h1D_MBD_effi_vec2"].push_back(new TF1(Form("MBDEffi2_%d",i),gaus_func,general_hist_range.first, general_hist_range.second,4));
        effi_fitting_vecH["h1D_MBD_effi_vec2"].back() -> SetLineColor(kRed);
    }

    // note : for the combined case, we stack up the four histograms
    combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx = new TH1F("",Form(";ZDC_vtxZ [cm] (Inc., ZDCNSTrig_MBDvtx);Entries"),general_Nbins, general_hist_range.first, general_hist_range.second);
    combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx -> GetXaxis() -> SetNdivisions(505);

    combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig = new TH1F("",Form(";ZDC_vtxZ [cm] (Inc., ZDCNSTrig_MBDNSTrig);Entries"),general_Nbins, general_hist_range.first, general_hist_range.second);
    combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig -> GetXaxis() -> SetNdivisions(505);

    combined_h1D_ZDCNSVtxZ_ZDCNSTrig = new TH1F("",Form(";ZDC_vtxZ [cm] (Inc., ZDCNSTrig);Entries"),general_Nbins, general_hist_range.first, general_hist_range.second);
    combined_h1D_ZDCNSVtxZ_ZDCNSTrig -> GetXaxis() -> SetNdivisions(505);

    combined_h1D_effi = new TH1F("",Form(";Inc., ZDC_vtxZ [cm];%s", effi_Y_title.c_str()),general_Nbins, general_hist_range.first, general_hist_range.second);
    combined_h1D_effi -> GetXaxis() -> SetNdivisions(505);

    combined_h1D_effi2 = new TH1F("", Form(";Inc., ZDC_vtxZ [cm];%s", effi2_Y_title.c_str()),general_Nbins, general_hist_range.first, general_hist_range.second);
    combined_h1D_effi2 -> GetXaxis() -> SetNdivisions(505);

    combined_effi_fitting = new TF1("MBDEffi_Combined",gaus_func,general_hist_range.first, general_hist_range.second,4);
    combined_effi_fitting -> SetLineColor(kRed);

    combined_effi_fitting2 = new TF1("MBDEffi2_Combined",gaus_func,general_hist_range.first, general_hist_range.second,4);
    combined_effi_fitting2 -> SetLineColor(kRed);
}

void MBD_zvtx_effi::RunMacro()
{
    Make_detector_vtxZ(
        "Vertical",
        range_t_V,
        h1D_detectorNS_vtxZ_mapV
    );

    DeriveMBDEffi(
        h1D_detectorNS_vtxZ_mapV,
        effi_fitting_vecV
    );



    Make_detector_vtxZ(
        "Horizontal",
        range_t_H,
        h1D_detectorNS_vtxZ_mapH
    );

    DeriveMBDEffi(
        h1D_detectorNS_vtxZ_mapH,
        effi_fitting_vecH
    );


    PrepareCombine();

}


void MBD_zvtx_effi::Make_detector_vtxZ(
    string direction_string,
    vector<pair<int,int>> &range_t_All, 
    map<string, vector<TH1F *>> &h1D_detectorNS_vtxZ_mapAll
)
{

    cout<<"In MBD_zvtx_effi::Make_detector_vtxZ, start to prepare the histograms"<<endl;
    
    // note : the time range of each scan step, for the case of vertical scan
    for (int i = 0; i < range_t_All.size(); i++)
    {
        for (auto pair : time_MBDNS_zvtx) // note : pair.first = the second, pair.second = the vector of the vtx_z
        { 
            int this_second = pair.first;

            if (this_second >= range_t_All[i].first && this_second <= range_t_All[i].second)
            {
                for (int vtx_i = 0; vtx_i < pair.second.size(); vtx_i++) // note : the elements in the vector of the particular second
                {
                    vector<int> LiveTrigger_vec = prepare_trigger_vec(time_LiveTrigger_Decimal[this_second][vtx_i]);
                    LiveTrigger_map.clear(); // note : the live trigger map
                    for (int i = 0; i < LiveTrigger_vec.size(); i++) { LiveTrigger_map[LiveTrigger_vec.at(i)] = LiveTrigger_vec.at(i); }
                    
                    double this_mbd_vtx_z = pair.second[vtx_i];
                    double this_zdc_vtx_z = time_ZDCNS_zvtx[this_second][vtx_i];
                    // double this_zdc_vtx_z = this_mbd_vtx_z; // note just for test

                    if (this_mbd_vtx_z != -999)
                    {
                        h1D_detectorNS_vtxZ_mapAll["h1D_MBDNSVtxZ_inclusive_vec"][i] -> Fill(this_mbd_vtx_z);

                        if (LiveTrigger_map.find(ZDCNS_inclusive_ID) != LiveTrigger_map.end()) {
                            h1D_detectorNS_vtxZ_mapAll["h1D_MBDNSVtxZ_ZDCNSTrig_vec"][i] -> Fill(this_mbd_vtx_z);
                        }
                        
                        if (LiveTrigger_map.find(MBDNS_inclusive_ID) != LiveTrigger_map.end()) {
                            h1D_detectorNS_vtxZ_mapAll["h1D_MBDNSVtxZ_MBDNSTrig_vec"][i] -> Fill(this_mbd_vtx_z);
                        }

                        if (LiveTrigger_map.find(MBDNS_30cm_ID) != LiveTrigger_map.end()) {
                            h1D_detectorNS_vtxZ_mapAll["h1D_MBDNSVtxZ_narrow_vec"][i] -> Fill(this_mbd_vtx_z);
                        }
                    }

                    if (this_zdc_vtx_z != -999)
                    {
                        h1D_detectorNS_vtxZ_mapAll["h1D_ZDCNSVtxZ_inclusive_vec"][i] -> Fill(this_zdc_vtx_z);

                        if (LiveTrigger_map.find(ZDCNS_inclusive_ID) != LiveTrigger_map.end()) {
                            h1D_detectorNS_vtxZ_mapAll["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"][i] -> Fill(this_zdc_vtx_z);
                        }
                        
                        if (LiveTrigger_map.find(MBDNS_inclusive_ID) != LiveTrigger_map.end()) {
                            h1D_detectorNS_vtxZ_mapAll["h1D_ZDCNSVtxZ_MBDNSTrig_vec"][i] -> Fill(this_zdc_vtx_z);
                        } 

                        if (LiveTrigger_map.find(ZDCNS_inclusive_ID) != LiveTrigger_map.end() && this_mbd_vtx_z != -999) {
                            h1D_detectorNS_vtxZ_mapAll["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx_vec"][i] -> Fill(this_zdc_vtx_z);
                        }

                        if (LiveTrigger_map.find(ZDCNS_inclusive_ID) != LiveTrigger_map.end() && LiveTrigger_map.find(MBDNS_inclusive_ID) != LiveTrigger_map.end()) {
                            h1D_detectorNS_vtxZ_mapAll["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig_vec"][i] -> Fill(this_zdc_vtx_z);
                        }
                        
                    }
                } // note : end of the elements of the vectors in each scan step 
            } // note : end of each step
        }
    }
    
    cout<<"In MBD_zvtx_effi::Make_detector_vtxZ, done with the histogram preparation"<<endl;
}

void MBD_zvtx_effi::PrepareCombine()
{
    cout<<"In MBD_zvtx_effi::PrepareCombine, start to combine the histograms"<<endl;
    

    combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx -> Sumw2(true);
    combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig -> Sumw2(true);
    combined_h1D_ZDCNSVtxZ_ZDCNSTrig -> Sumw2(true);

    for (int step : chosen_scan_steps)
    {
        combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx    -> Add(h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx_vec"][step]);
        combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig -> Add(h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig_vec"][step]);
        combined_h1D_ZDCNSVtxZ_ZDCNSTrig           -> Add(h1D_detectorNS_vtxZ_mapV["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"][step]);

        combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx    -> Add(h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx_vec"][step]);
        combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig -> Add(h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig_vec"][step]);
        combined_h1D_ZDCNSVtxZ_ZDCNSTrig           -> Add(h1D_detectorNS_vtxZ_mapH["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"][step]);
    }

    combined_h1D_effi -> Sumw2(true);
    combined_h1D_effi -> Divide(combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx, combined_h1D_ZDCNSVtxZ_ZDCNSTrig);

    combined_effi_fitting -> SetParameters(
        combined_h1D_effi -> GetBinContent(combined_h1D_effi -> GetMaximumBin()),    
        combined_h1D_effi -> GetMean(),
        combined_h1D_effi -> GetStdDev(),
        0
    );
    combined_h1D_effi -> Fit(combined_effi_fitting, "NQ");



    combined_h1D_effi2 -> Sumw2(true);
    combined_h1D_effi2 -> Divide(combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig, combined_h1D_ZDCNSVtxZ_ZDCNSTrig);

    combined_effi_fitting2 -> SetParameters(
        combined_h1D_effi2 -> GetBinContent(combined_h1D_effi2 -> GetMaximumBin()),    
        combined_h1D_effi2 -> GetMean(),
        combined_h1D_effi2 -> GetStdDev(),
        0
    );
    combined_h1D_effi2 -> Fit(combined_effi_fitting2, "NQ");

    return;
}

void MBD_zvtx_effi::DeriveMBDEffi(
    map<string, vector<TH1F *>> &h1D_detectorNS_vtxZ_mapAll,
    map<string, vector<TF1 *>> &effi_fitting_vecAll
)
{
    cout<<"In MBD_zvtx_effi::DeriveMBDEffi, start to derive the MBD efficiency"<<endl;

    for (int i = 0; i < h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec"].size(); i++)
    {
        h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec"][i] -> Sumw2(true); // note : this is to ensure the error bars are calculated correctly
        h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec"][i] -> Divide(h1D_detectorNS_vtxZ_mapAll["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx_vec"][i], h1D_detectorNS_vtxZ_mapAll["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"][i]);

        h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec2"][i] -> Sumw2(true); // note : this is to ensure the error bars are calculated correctly
        h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec2"][i] -> Divide(h1D_detectorNS_vtxZ_mapAll["h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig_vec"][i], h1D_detectorNS_vtxZ_mapAll["h1D_ZDCNSVtxZ_ZDCNSTrig_vec"][i]);
    }

    for (int step : chosen_scan_steps)
    {
        effi_fitting_vecAll["h1D_MBD_effi_vec"][step] -> SetParameters(
            h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec"][step] -> GetBinContent(h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec"][step] -> GetMaximumBin()),    
            h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec"][step] -> GetMean(),
            h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec"][step] -> GetStdDev(),
            0
        );
        h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec"][step] -> Fit(effi_fitting_vecAll["h1D_MBD_effi_vec"][step], "NQ");

        

        effi_fitting_vecAll["h1D_MBD_effi_vec2"][step] -> SetParameters(
            h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec2"][step] -> GetBinContent(h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec2"][step] -> GetMaximumBin()),    
            h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec2"][step] -> GetMean(),
            h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec2"][step] -> GetStdDev(),
            0
        );
        h1D_detectorNS_vtxZ_mapAll["h1D_MBD_effi_vec2"][step] -> Fit(effi_fitting_vecAll["h1D_MBD_effi_vec2"][step], "NQ");   
    }

    cout<<"In MBD_zvtx_effi::DeriveMBDEffi, done with the MBD efficiency derivation"<<endl;

    // note : print the plots 
}

vector<TF1 *> MBD_zvtx_effi::GetEffiFitFuncSeparate()
{
    vector<TF1 *> output_vec; output_vec.clear();
    
    for (int step : chosen_scan_steps) {
        output_vec.push_back(effi_fitting_vecV["h1D_MBD_effi_vec"][step]);
    }
    for (int step : chosen_scan_steps) {
        output_vec.push_back(effi_fitting_vecV["h1D_MBD_effi_vec2"][step]);
    }
    
    for (int step : chosen_scan_steps) {
        output_vec.push_back(effi_fitting_vecH["h1D_MBD_effi_vec"][step]);
    }
    for (int step : chosen_scan_steps) {
        output_vec.push_back(effi_fitting_vecH["h1D_MBD_effi_vec2"][step]);
    }

    return output_vec;
}

vector<TF1 *>MBD_zvtx_effi::GetEffiFitFuncComb()
{
    vector<TF1 *> output_vec = {
        combined_effi_fitting,
        combined_effi_fitting2
    };

    return output_vec;

}

void MBD_zvtx_effi::ClearUp()
{
    return;
}

void MBD_zvtx_effi::DrawPlots()
{
    // note : ============================ ============================ ============================ ============================ ============================

    c1 -> cd(); 
    for (pair<string, vector<TH1F *>> str_H1Vec : h1D_detectorNS_vtxZ_mapV) // note : different kind of plots 
    {
        c1 -> Print(Form("%s/Single_%sV.pdf(", final_output_directory.c_str(), str_H1Vec.first.c_str()));

        for (TH1F * h1D : str_H1Vec.second) // note : in each step
        {
            h1D -> SetMinimum(0);
            h1D -> Draw("hist");
            c1 -> Print(Form("%s/Single_%sV.pdf", final_output_directory.c_str(), str_H1Vec.first.c_str()));
            c1 -> Clear();
        }

        c1 -> Print(Form("%s/Single_%sV.pdf)", final_output_directory.c_str(), str_H1Vec.first.c_str()));
    }

    c1 -> cd(); 
    for (pair<string, vector<TH1F *>> str_H1Vec : h1D_detectorNS_vtxZ_mapH) // note : different kind of plots 
    {
        c1 -> Print(Form("%s/Single_%sH.pdf(", final_output_directory.c_str(), str_H1Vec.first.c_str()));

        for (TH1F * h1D : str_H1Vec.second) // note : in each step
        {
            h1D -> SetMinimum(0);
            h1D -> Draw("hist");
            c1 -> Print(Form("%s/Single_%sH.pdf", final_output_directory.c_str(), str_H1Vec.first.c_str()));
            c1 -> Clear();
        }

        c1 -> Print(Form("%s/Single_%sH.pdf)", final_output_directory.c_str(), str_H1Vec.first.c_str()));
    }



    c3 -> cd();
    int i = 0; 
    for (pair<string, vector<TH1F *>> str_H1Vec : h1D_detectorNS_vtxZ_mapV) // note : different kind of plots 
    { 
        for (TH1F * h1D : str_H1Vec.second) // note : in each step
        {
            c3 -> cd(i+1);
            h1D -> Draw("hist");

            i++;
        }

        c3 -> Print(Form("%s/Grid_%sV.pdf", final_output_directory.c_str(), str_H1Vec.first.c_str()));
        c3 -> Clear();
        c3 -> Divide(4, 3);
        
        i = 0;
    }

    i = 0;
    c3 -> cd();
    for (pair<string, vector<TH1F *>> str_H1Vec : h1D_detectorNS_vtxZ_mapH) // note : different kind of plots 
    {
        c3 -> cd(i+1);
        for (TH1F * h1D : str_H1Vec.second) // note : in each step
        {
            c3 -> cd(i+1);
            h1D -> Draw("hist");

            i++;
        }

        c3 -> Print(Form("%s/Grid_%sH.pdf", final_output_directory.c_str(), str_H1Vec.first.c_str()));
        c3 -> Clear();
        c3 -> Divide(4, 3);

        i = 0;
    }

    double max_Y_ratio = 1.5;

    c2 -> cd();
    string plot_type = "h1D_MBD_effi_vec";
    for (int i = 0; i < chosen_scan_steps.size(); i++)
    {
        c2 -> cd(i+1);
        h1D_detectorNS_vtxZ_mapV[plot_type.c_str()][chosen_scan_steps[i]] -> SetMaximum( max_Y_ratio * h1D_detectorNS_vtxZ_mapV[plot_type.c_str()][chosen_scan_steps[i]] -> GetBinContent(h1D_detectorNS_vtxZ_mapV[plot_type.c_str()][chosen_scan_steps[i]] -> GetMaximumBin()));
        h1D_detectorNS_vtxZ_mapV[plot_type.c_str()][chosen_scan_steps[i]] -> Draw("hist");
        effi_fitting_vecV[plot_type.c_str()][chosen_scan_steps[i]] -> Draw("lsame");
        draw_text -> DrawLatex(0.21, 0.87, Form("Fit Width: %.3f cm", effi_fitting_vecV[plot_type.c_str()][chosen_scan_steps[i]] -> GetParameter(2)));
        draw_text -> DrawLatex(0.21, 0.84, Form("Fit Mean: %.3f cm", effi_fitting_vecV[plot_type.c_str()][chosen_scan_steps[i]] -> GetParameter(1)));

        c2 -> cd(i+1+chosen_scan_steps.size());
        h1D_detectorNS_vtxZ_mapH[plot_type.c_str()][chosen_scan_steps[i]] -> SetMaximum( max_Y_ratio * h1D_detectorNS_vtxZ_mapH[plot_type.c_str()][chosen_scan_steps[i]] -> GetBinContent(h1D_detectorNS_vtxZ_mapH[plot_type.c_str()][chosen_scan_steps[i]] -> GetMaximumBin()));
        h1D_detectorNS_vtxZ_mapH[plot_type.c_str()][chosen_scan_steps[i]] -> Draw("hist");
        effi_fitting_vecH[plot_type.c_str()][chosen_scan_steps[i]] -> Draw("lsame");
        draw_text -> DrawLatex(0.21, 0.87, Form("Fit Width: %.3f cm", effi_fitting_vecH[plot_type.c_str()][chosen_scan_steps[i]] -> GetParameter(2)));
        draw_text -> DrawLatex(0.21, 0.84, Form("Fit Mean: %.3f cm", effi_fitting_vecH[plot_type.c_str()][chosen_scan_steps[i]] -> GetParameter(1)));
    }
    c2 -> Print(Form("%s/VH_%s.pdf", final_output_directory.c_str(), plot_type.c_str()));
    c2 -> Clear();
    c2 -> Divide(chosen_scan_steps.size(), 2);

    c2 -> cd();
    plot_type = "h1D_MBD_effi_vec2";
    for (int i = 0; i < chosen_scan_steps.size(); i++)
    {
        c2 -> cd(i+1);
        h1D_detectorNS_vtxZ_mapV[plot_type.c_str()][chosen_scan_steps[i]] -> SetMaximum( max_Y_ratio * h1D_detectorNS_vtxZ_mapV[plot_type.c_str()][chosen_scan_steps[i]] -> GetBinContent(h1D_detectorNS_vtxZ_mapV[plot_type.c_str()][chosen_scan_steps[i]] -> GetMaximumBin()));
        h1D_detectorNS_vtxZ_mapV[plot_type.c_str()][chosen_scan_steps[i]] -> Draw("hist");
        effi_fitting_vecV[plot_type.c_str()][chosen_scan_steps[i]] -> Draw("lsame");
        draw_text -> DrawLatex(0.21, 0.87, Form("Fit Width: %.3f cm", effi_fitting_vecV[plot_type.c_str()][chosen_scan_steps[i]] -> GetParameter(2)));
        draw_text -> DrawLatex(0.21, 0.84, Form("Fit Mean: %.3f cm", effi_fitting_vecV[plot_type.c_str()][chosen_scan_steps[i]] -> GetParameter(1)));

        c2 -> cd(i+1+chosen_scan_steps.size());
        h1D_detectorNS_vtxZ_mapH[plot_type.c_str()][chosen_scan_steps[i]] -> SetMaximum( max_Y_ratio * h1D_detectorNS_vtxZ_mapH[plot_type.c_str()][chosen_scan_steps[i]] -> GetBinContent(h1D_detectorNS_vtxZ_mapH[plot_type.c_str()][chosen_scan_steps[i]] -> GetMaximumBin()));
        h1D_detectorNS_vtxZ_mapH[plot_type.c_str()][chosen_scan_steps[i]] -> Draw("hist");
        effi_fitting_vecH[plot_type.c_str()][chosen_scan_steps[i]] -> Draw("lsame");
        draw_text -> DrawLatex(0.21, 0.87, Form("Fit Width: %.3f cm", effi_fitting_vecH[plot_type.c_str()][chosen_scan_steps[i]] -> GetParameter(2)));
        draw_text -> DrawLatex(0.21, 0.84, Form("Fit Mean: %.3f cm", effi_fitting_vecH[plot_type.c_str()][chosen_scan_steps[i]] -> GetParameter(1)));
    }
    c2 -> Print(Form("%s/VH_%s.pdf", final_output_directory.c_str(), plot_type.c_str()));
    c2 -> Clear();

    // note : for the combined case
    c1 -> cd();
    combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx -> SetMinimum(0);
    combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDvtx -> Draw("hist");
    c1 -> Print(Form("%s/Combined_ZDCNSVtxZ_ZDCNSTrig_MBDvtx.pdf", final_output_directory.c_str()));
    c1 -> Clear();

    c1 -> cd();
    combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig -> SetMinimum(0);
    combined_h1D_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig -> Draw("hist");
    c1 -> Print(Form("%s/Combined_ZDCNSVtxZ_ZDCNSTrig_MBDNSTrig.pdf", final_output_directory.c_str()));
    c1 -> Clear();

    c1 -> cd();
    combined_h1D_ZDCNSVtxZ_ZDCNSTrig -> SetMinimum(0);
    combined_h1D_ZDCNSVtxZ_ZDCNSTrig -> Draw("hist");
    c1 -> Print(Form("%s/Combined_ZDCNSVtxZ_ZDCNSTrig.pdf", final_output_directory.c_str()));
    c1 -> Clear();

    c1 ->cd();
    combined_h1D_effi -> SetMaximum( max_Y_ratio * combined_h1D_effi -> GetBinContent(combined_h1D_effi -> GetMaximumBin()));
    combined_h1D_effi -> Draw("hist");
    combined_effi_fitting -> Draw("lsame");
    draw_text -> DrawLatex(0.21, 0.87, Form("Fit Width: %.3f cm", combined_effi_fitting -> GetParameter(2)));
    draw_text -> DrawLatex(0.21, 0.84, Form("Fit Mean: %.3f cm", combined_effi_fitting -> GetParameter(1)));
    c1 -> Print(Form("%s/Combined_effi.pdf", final_output_directory.c_str()));
    c1 -> Clear();

    c1 ->cd();
    combined_h1D_effi2 -> SetMaximum( max_Y_ratio * combined_h1D_effi2 -> GetBinContent(combined_h1D_effi2 -> GetMaximumBin()));
    combined_h1D_effi2 -> Draw("hist");
    combined_effi_fitting2 -> Draw("lsame");
    draw_text -> DrawLatex(0.21, 0.87, Form("Fit Width: %.3f cm", combined_effi_fitting2 -> GetParameter(2)));
    draw_text -> DrawLatex(0.21, 0.84, Form("Fit Mean: %.3f cm", combined_effi_fitting2 -> GetParameter(1)));
    c1 -> Print(Form("%s/Combined_effi2.pdf", final_output_directory.c_str()));
    c1 -> Clear();
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

std::vector<int> MBD_zvtx_effi::prepare_trigger_vec(long long trigger_input)
{
    std::bitset<64> trigger_input_bitset(trigger_input);
    std::vector<int> output_vec; output_vec.clear();

    for (unsigned int i=0; i<64; i++)
	{
	    if (trigger_input_bitset.test(i))
	    {
            output_vec.push_back(i);
	    }
	}

    return output_vec;

}