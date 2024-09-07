#include "sPhenixStyle.C"

struct calo_struct{
    int index;
    string name;
    vector<double> *waveform;
    TH1F * hist;
};

int quick_check(long long eventID, string input_full_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform/ntuple_run51195_00006.root", string output_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform/quick_check")
{
    map<string, calo_struct> zdc_map;
    zdc_map["zdcS1"] = {.index = 0, .name = "zdcS1_waveform", .waveform = 0, .hist = new TH1F("zdcS1", "zdcS1", 16, 0, 16)};
    zdc_map["zdcS2"] = {.index = 1, .name = "zdcS2_waveform", .waveform = 0, .hist = new TH1F("zdcS2", "zdcS2", 16, 0, 16)};
    zdc_map["zdcS3"] = {.index = 2, .name = "zdcS3_waveform", .waveform = 0, .hist = new TH1F("zdcS3", "zdcS3", 16, 0, 16)};
    zdc_map["zdcN1"] = {.index = 3, .name = "zdcN1_waveform", .waveform = 0, .hist = new TH1F("zdcN1", "zdcN1", 16, 0, 16)};
    zdc_map["zdcN2"] = {.index = 4, .name = "zdcN2_waveform", .waveform = 0, .hist = new TH1F("zdcN2", "zdcN2", 16, 0, 16)};
    zdc_map["zdcN3"] = {.index = 5, .name = "zdcN3_waveform", .waveform = 0, .hist = new TH1F("zdcN3", "zdcN3", 16, 0, 16)};
    
    int ZDCNS_trigger_ID = 3;


    TFile * file_in = TFile::Open(input_full_directory.c_str());
    TTree * tree = (TTree*)file_in->Get("tree");
    tree->SetBranchStatus("*", 0);
    tree->SetBranchStatus("evtID", 1);
    tree->SetBranchStatus("evtBCO_gl1", 1);
    tree->SetBranchStatus("bunchnumber", 1);
    tree->SetBranchStatus("LiveTrigger_Vec", 1);
    tree->SetBranchStatus("mbd_z_vtx", 1);
    for (auto &zdc : zdc_map) {
        tree->SetBranchStatus(zdc.second.name.c_str(), 1);
    }

    long long evtID;
    long long evtBCO_gl1;
    int bunchnumber;
    vector<int> *live_trigger_vec = 0;
    double mbd_z_vtx;

    tree->SetBranchAddress("evtID", &evtID);
    tree->SetBranchAddress("evtBCO_gl1", &evtBCO_gl1);
    tree->SetBranchAddress("bunchnumber", &bunchnumber);
    tree->SetBranchAddress("LiveTrigger_Vec", &live_trigger_vec);
    tree->SetBranchAddress("mbd_z_vtx", &mbd_z_vtx);
    for (auto &zdc : zdc_map) {
        tree->SetBranchAddress(zdc.second.name.c_str(), &zdc.second.waveform);
    }


    SetsPhenixStyle();
    TCanvas * c1 = new TCanvas("c1", "c1", 1200, 800);
    c1 -> Divide(3,2);

    c1 -> Print(Form("%s/ZDCNS_waveform.pdf(", output_directory.c_str()));

    map<int,int> live_trigger_map_evt;
    for (int i = 0; i < 10000; i++)
    {
        tree -> GetEntry(i);

        live_trigger_map_evt.clear();
        for (int i = 0; i < live_trigger_vec->size(); i++) {
            live_trigger_map_evt[live_trigger_vec->at(i)] = 1;
        }

        if (live_trigger_map_evt.find(ZDCNS_trigger_ID) != live_trigger_map_evt.end()) {
            cout<<"eID : "<<i<<" has ZDCNS trigger"<<endl;
            for (auto &zdc : zdc_map) {
                for (int i = 0; i < zdc.second.waveform->size(); i++) {
                    zdc.second.hist->SetBinContent(i+1, zdc.second.waveform->at(i));
                }
                c1 -> cd(zdc.second.index+1);
                zdc.second.hist->Draw();
            }

            c1 -> Print(Form("%s/ZDCNS_waveform.pdf", output_directory.c_str()));
        } 

        for (auto &zdc : zdc_map) {
            zdc.second.hist->Reset("ICESM");
        }

    }
    
    c1 -> Print(Form("%s/ZDCNS_waveform.pdf)", output_directory.c_str()));







    // tree -> GetEntry(eventID);
    
    // map<int,int> live_trigger_map; live_trigger_map.clear();
    // for (int i = 0; i < live_trigger_vec->size(); i++) {
    //     live_trigger_map[live_trigger_vec->at(i)] = 1;
    // }

    // int have_ZDCNS_trigger;
    // if (live_trigger_map.find(ZDCNS_trigger_ID) == live_trigger_map.end()) {
    //     have_ZDCNS_trigger = 0;
    // } else {
    //     have_ZDCNS_trigger = 1;
    // }

    // cout<<"eventID = "<<eventID<<endl;
    // cout<<"evtID = "<<evtID<<endl;
    // cout<<"ZDCNS coin. : "<<have_ZDCNS_trigger<<endl;

    // TCanvas * c1 = new TCanvas("c1", "c1", 1200, 800);
    // c1 -> Divide(3,2);

    // for (auto &zdc : zdc_map) {
    //     for (int i = 0; i < zdc.second.waveform->size(); i++) {
    //         zdc.second.hist->SetBinContent(i+1, zdc.second.waveform->at(i));
    //     }
    //     c1 -> cd(zdc.second.index+1);
    //     zdc.second.hist->Draw();
    // }

    return 0;

}