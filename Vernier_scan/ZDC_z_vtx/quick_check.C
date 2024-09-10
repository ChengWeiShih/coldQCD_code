#include "sPhenixStyle.C"

struct calo_struct{
    int index;
    string name;
    vector<double> *waveform;
    TH1F * hist;
    TF1 * fit;
    TH1F * h1_waveform_peak;
    TH1F * h1_waveform_StdDev;
    int status;
    int Ngroup;
    double peak_pos;
    double peak_height;
};

double land_offset(double *x, double *par)
{
    // note : par[0] : size
    // note : par[1] : mpv
    // note : par[2] : width
    // note : par[3] : offset
    return (par[0]/0.178854) * TMath::Landau(x[0], par[1], par[2]) + par[3];
}

double vector_stddev (vector <double> input_vector){
	
	double sum_subt = 0;
	double average  = accumulate( input_vector.begin(), input_vector.end(), 0.0 ) / double(input_vector.size());
	
	// cout<<"average is : "<<average<<endl;

	for (int i=0; i<input_vector.size(); i++){ sum_subt += pow((input_vector[i] - average),2); }

	//cout<<"sum_subt : "<<sum_subt<<endl;
	// cout<<"print from the function, average : "<<average<<" std : "<<stddev<<endl;

	return sqrt( sum_subt / double(input_vector.size()-1) );
}

double hist_stddev (TH1F * hist_in)
{
    vector<double> hist_content; hist_content.clear();
    for (int i = 0; i < hist_in -> GetNbinsX(); i++){
        hist_content.push_back(hist_in -> GetBinContent(i+1));
    }

    return vector_stddev(hist_content);
}

vector<double> find_Ngroup(TH1F * hist_in)
{
    double Highest_bin_Content  = hist_in -> GetBinContent(hist_in -> GetMaximumBin());
    double Highest_bin_Center   = hist_in -> GetBinCenter(hist_in -> GetMaximumBin());

    int group_Nbin = 0;
    int peak_group_ID;
    double group_entry = 0;
    double peak_group_ratio;
    vector<int> group_Nbin_vec; group_Nbin_vec.clear();
    vector<double> group_entry_vec; group_entry_vec.clear();
    vector<double> group_widthL_vec; group_widthL_vec.clear();
    vector<double> group_widthR_vec; group_widthR_vec.clear();

    for (int i = 0; i < hist_in -> GetNbinsX(); i++){
        // todo : the background rejection is here : Highest_bin_Content/2. for the time being
        double bin_content = ( hist_in -> GetBinContent(i+1) <= Highest_bin_Content/2.) ? 0. : ( hist_in -> GetBinContent(i+1) - Highest_bin_Content/2. );

        if (bin_content != 0){
            
            if (group_Nbin == 0) {
                group_widthL_vec.push_back(hist_in -> GetBinCenter(i+1) - (hist_in -> GetBinWidth(i+1)/2.));
            }

            group_Nbin += 1; 
            group_entry += bin_content;
        }
        else if (bin_content == 0 && group_Nbin != 0){
            group_widthR_vec.push_back(hist_in -> GetBinCenter(i+1) - (hist_in -> GetBinWidth(i+1)/2.));
            group_Nbin_vec.push_back(group_Nbin);
            group_entry_vec.push_back(group_entry);
            group_Nbin = 0;
            group_entry = 0;
        }
    }
    if (group_Nbin != 0) {
        group_Nbin_vec.push_back(group_Nbin);
        group_entry_vec.push_back(group_entry);
        group_widthR_vec.push_back(hist_in -> GetXaxis()->GetXmax());
    } // note : the last group at the edge

    // note : find the peak group
    for (int i = 0; i < group_Nbin_vec.size(); i++){
        if (group_widthL_vec[i] < Highest_bin_Center && Highest_bin_Center < group_widthR_vec[i]){
            peak_group_ID = i;
            break;
        }
    }
    
    
    peak_group_ratio = group_entry_vec[peak_group_ID] / (accumulate( group_entry_vec.begin(), group_entry_vec.end(), 0.0 ));

    // for (int i = 0; i < group_Nbin_vec.size(); i++)
    // {
    //     cout<<" "<<endl;
    //     cout<<"group size : "<<group_Nbin_vec[i]<<endl;
    //     cout<<"group entry : "<<group_entry_vec[i]<<endl;
    //     cout<<group_widthL_vec[i]<<" "<<group_widthR_vec[i]<<endl;
    // }

    // cout<<" "<<endl;
    // cout<<"N group : "<<group_Nbin_vec.size()<<endl;
    // cout<<"Peak group ID : "<<peak_group_ID<<endl;
    // cout<<"peak group width : "<<group_widthL_vec[peak_group_ID]<<" "<<group_widthR_vec[peak_group_ID]<<endl;
    // cout<<"ratio : "<<peak_group_ratio<<endl;
    
    // note : {N_group, ratio (if two), peak widthL, peak widthR}
    return {double(group_Nbin_vec.size()), peak_group_ratio, group_widthL_vec[peak_group_ID], group_widthR_vec[peak_group_ID]};
}

void hist_bkgrm(TH1F * hist_in)
{
    double baseline_left = 0.;
    double baseline_right = 0.;

    for(int s = 0; s < 3; s++) { baseline_left += hist_in -> GetBinContent(s+1); }
    for (int s = hist_in -> GetNbinsX()-3; s < hist_in -> GetNbinsX(); s++) { baseline_right += hist_in -> GetBinContent(s+1); }

    baseline_left /= 3.;
    baseline_right /=3.;
    double baseline = (baseline_left < baseline_right) ? baseline_left : baseline_right;

    for(int s = 0; s < hist_in -> GetNbinsX(); s++) { 
        
        if (hist_in -> GetBinContent(s+1) < baseline) {hist_in -> SetBinContent(s+1, 0.); continue; }

        hist_in -> SetBinContent(s+1, hist_in -> GetBinContent(s+1) - baseline); 
    }
}

pair<double, double> GetFitRangeHist(TH1F * hist_in)
{
    int peak_bin = hist_in -> GetMaximumBin(); // note : 1 ~ 16
    if (peak_bin > 3 && peak_bin < 14) {return {3., 13.};}
    else if (peak_bin < 4) {return {0., 10.};} // note : peak at bin 2, 3
    else {return {10., 16.};} // note : peak at bin 14, 15
}

pair<double, double> GetNumZDCtime(TH1F * hist_in)
{
    int peak_bin = hist_in -> GetMaximumBin(); // note : 1 ~ 16
    double numerator = 0; 
    double denominator = 0;

    for (int i = 0; i < 3; i++)
    {
        numerator += hist_in -> GetBinContent(peak_bin + i) * hist_in -> GetBinLowEdge(peak_bin + i);
        denominator += hist_in -> GetBinContent(peak_bin + i);

        if (peak_bin + i == hist_in -> GetNbinsX()) {break;}
    }

    return {numerator/denominator, denominator};
}

int quick_check(long long eventID, string input_full_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform/ntuple_run51195_merged.root", string output_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform/quick_check")
{
    map<string, calo_struct> zdc_map;
    zdc_map["zdcS1"] = {
        .index = 0, 
        .name = "zdcS1_waveform", 
        .waveform = 0, 
        .hist = new TH1F("zdcS1", "zdcS1", 16, 0, 16), 
        .fit = new TF1("",land_offset, 0, 16, 4),
        .h1_waveform_peak = new TH1F("",";zdcS1_PeakSample;Entries",16,0,16),
        .h1_waveform_StdDev = new TH1F("",";zdcS1_SetDeV;Entries",100,0,150),
        .status = 0,
        .Ngroup = -999,
        .peak_pos = -999,
        .peak_height = -999
    };
    zdc_map["zdcS2"] = {
        .index = 1, 
        .name = "zdcS2_waveform", 
        .waveform = 0, 
        .hist = new TH1F("zdcS2", "zdcS2", 16, 0, 16), 
        .fit = new TF1("",land_offset, 0, 16, 4),
        .h1_waveform_peak = new TH1F("",";zdcS2_PeakSample;Entries",16,0,16),
        .h1_waveform_StdDev = new TH1F("",";zdcS2_SetDeV;Entries",100,0,150),
        .status = 0,
        .Ngroup = -999,
        .peak_pos = -999,
        .peak_height = -999
    };
    zdc_map["zdcS3"] = {
        .index = 2, 
        .name = "zdcS3_waveform", 
        .waveform = 0, 
        .hist = new TH1F("zdcS3", "zdcS3", 16, 0, 16), 
        .fit = new TF1("",land_offset, 0, 16, 4),
        .h1_waveform_peak = new TH1F("",";zdcS3_PeakSample;Entries",16,0,16),
        .h1_waveform_StdDev = new TH1F("",";zdcS3_SetDeV;Entries",100,0,150),
        .status = 0,
        .Ngroup = -999,
        .peak_pos = -999,
        .peak_height = -999
    };
    zdc_map["zdcN1"] = {
        .index = 3, 
        .name = "zdcN1_waveform", 
        .waveform = 0, 
        .hist = new TH1F("zdcN1", "zdcN1", 16, 0, 16), 
        .fit = new TF1("",land_offset, 0, 16, 4),
        .h1_waveform_peak = new TH1F("",";zdcN1_PeakSample;Entries",16,0,16),
        .h1_waveform_StdDev = new TH1F("",";zdcN1_SetDeV;Entries",100,0,150),
        .status = 0,
        .Ngroup = -999,
        .peak_pos = -999,
        .peak_height = -999
    };
    zdc_map["zdcN2"] = {
        .index = 4, 
        .name = "zdcN2_waveform", 
        .waveform = 0, 
        .hist = new TH1F("zdcN2", "zdcN2", 16, 0, 16), 
        .fit = new TF1("",land_offset, 0, 16, 4),
        .h1_waveform_peak = new TH1F("",";zdcN2_PeakSample;Entries",16,0,16),
        .h1_waveform_StdDev = new TH1F("",";zdcN2_SetDeV;Entries",100,0,150),
        .status = 0,
        .Ngroup = -999,
        .peak_pos = -999,
        .peak_height = -999
    };
    zdc_map["zdcN3"] = {
        .index = 5, 
        .name = "zdcN3_waveform", 
        .waveform = 0, 
        .hist = new TH1F("zdcN3", "zdcN3", 16, 0, 16), 
        .fit = new TF1("",land_offset, 0, 16, 4),
        .h1_waveform_peak = new TH1F("",";zdcN3_PeakSample;Entries",16,0,16),
        .h1_waveform_StdDev = new TH1F("",";zdcN3_SetDeV;Entries",100,0,150),
        .status = 0,
        .Ngroup = -999,
        .peak_pos = -999,
        .peak_height = -999
    };
    
    int ZDCNS_trigger_ID = 3;


    TLatex * draw_text = new TLatex();
    draw_text -> SetNDC();
    draw_text -> SetTextSize(0.03);

    TLine * coord_line = new TLine();
    coord_line -> SetLineWidth(1);
    coord_line -> SetLineColor(16);
    coord_line -> SetLineStyle(2);

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

    TGaxis::SetMaxDigits(4);

    TH1F * ZDC_vtxZ_fit = new TH1F("ZDC_vtxZ_fit", "ZDC_vtxZ_fit;ZDC_vtxZ_fit [cm];Entries", 100, -500, 500);
    ZDC_vtxZ_fit -> GetXaxis() -> SetNdivisions(505);
    TH1F * ZDC_vtxZ_num = new TH1F("ZDC_vtxZ_num", "ZDC_vtxZ_num;ZDC_vtxZ_num [cm];Entries", 100, -500, 500);
    ZDC_vtxZ_num -> GetXaxis() -> SetNdivisions(505);
    TH1F * ZDC_vtxZ = new TH1F("ZDC_vtxZ", "ZDC_vtxZ;ZDC_vtxZ [cm];Entries", 100, -500, 500);
    ZDC_vtxZ -> GetXaxis() -> SetNdivisions(505);
    TH2F * vtx_correlation = new TH2F("",";MBD_vtxZ [cm];ZDC_vtxZ [cm]",150,-300,300,150,-300,300);
    vtx_correlation -> GetXaxis() -> SetNdivisions(505);

    TFile * file_out = new TFile(Form("%s/ZDC_vtxZ.root", output_directory.c_str()), "recreate");
    TTree * tree_out = new TTree("tree", "tree");
    double ZDC_vtxZ_num_out; 
    double ZDC_vtxZ_fit_out;
    double ZDC_vtxZ_out;
    double mbd_vtxZ_out;
    int Ngood_waveform_S_out;
    int Ngood_waveform_N_out;
    tree_out -> Branch("ZDC_vtxZ_num", &ZDC_vtxZ_num_out);
    tree_out -> Branch("ZDC_vtxZ_fit", &ZDC_vtxZ_fit_out);
    tree_out -> Branch("ZDC_vtxZ", &ZDC_vtxZ_out);
    tree_out -> Branch("mbd_vtxZ", &mbd_vtxZ_out);
    tree_out -> Branch("Ngood_waveform_S", &Ngood_waveform_S_out);
    tree_out -> Branch("Ngood_waveform_N", &Ngood_waveform_N_out);

    map<int,int> live_trigger_map_evt;
    for (int i = 0; i < tree->GetEntries(); i++)
    {
        tree -> GetEntry(i);

        if (i % 1000 == 0) {cout<<"eventID : "<<i<<endl;}

        live_trigger_map_evt.clear();
        for (int i = 0; i < live_trigger_vec->size(); i++) {
            live_trigger_map_evt[live_trigger_vec->at(i)] = 1;
        }

        if (live_trigger_map_evt.find(ZDCNS_trigger_ID) != live_trigger_map_evt.end()) {
            // cout<<"eID : "<<i<<" has ZDCNS trigger"<<endl;
            for (auto &zdc : zdc_map) {
                for (int i = 0; i < zdc.second.waveform->size(); i++) {
                    zdc.second.hist->SetBinContent(i+1, zdc.second.waveform->at(i));
                }

                zdc.second.hist->SetTitle(Form(";%s;A. U.",zdc.second.name.c_str()));
                hist_bkgrm(zdc.second.hist);
                zdc.second.hist->SetMinimum(0);
                vector<double> Ngroup_info = find_Ngroup(zdc.second.hist);

                zdc.second.Ngroup = Ngroup_info[0];
                zdc.second.status = (Ngroup_info[0] == 1) ? 1 : 0;
                zdc.second.peak_pos = zdc.second.hist -> GetMaximumBin();
                zdc.second.peak_height = zdc.second.hist->GetBinContent(zdc.second.hist->GetMaximumBin());

                if (Ngroup_info[0] == 1){
                    zdc.second.h1_waveform_peak->Fill(zdc.second.hist->GetMaximumBin()-1);
                }
                
                zdc.second.h1_waveform_StdDev->Fill(hist_stddev(zdc.second.hist));


                zdc.second.fit->SetLineColor(kRed);
                zdc.second.fit->SetParameters(
                    zdc.second.hist->GetBinContent(zdc.second.hist->GetMaximumBin()) - zdc.second.hist->GetBinContent(zdc.second.hist->GetMinimumBin()),
                    zdc.second.hist->GetMaximumBin(),
                    0.3,
                    zdc.second.hist->GetBinContent(zdc.second.hist->GetMinimumBin())
                );
                zdc.second.fit->SetParLimits(0, 0, 10000);

                zdc.second.hist->Fit(zdc.second.fit, "NQ","", GetFitRangeHist(zdc.second.hist).first, GetFitRangeHist(zdc.second.hist).second);

                c1 -> cd(zdc.second.index+1);
                zdc.second.hist->Draw("hist");
                coord_line -> DrawLine(0, zdc.second.hist->GetBinContent(zdc.second.hist->GetMaximumBin())/2., 16, zdc.second.hist->GetBinContent(zdc.second.hist->GetMaximumBin())/2.);
                zdc.second.fit->Draw("l same");

                draw_text -> DrawLatex(0.2, 0.87, Form("eID: %d", i));
                draw_text -> DrawLatex(0.2, 0.84, Form("StdDev: %.2f", vector_stddev(*zdc.second.waveform)));
                draw_text -> DrawLatex(0.2, 0.81, Form("Ngroup: %.0f", Ngroup_info[0]));
                draw_text -> DrawLatex(0.2, 0.78, Form("fit PeakPos: %.2f", zdc.second.fit->GetParameter(1)));
            }

            if (i % 1000 == 0) {c1 -> Print(Form("%s/ZDCNS_waveform.pdf", output_directory.c_str()));}



            if (
                (zdc_map["zdcS1"].status + zdc_map["zdcS2"].status + zdc_map["zdcS3"].status) > 1 &&
                (zdc_map["zdcN1"].status + zdc_map["zdcN2"].status + zdc_map["zdcN3"].status) > 1
            )
            {
                double T_numerator_S = 0;
                double T_denominator_S = 0;
                double T_numerator_N = 0;
                double T_denominator_N = 0;

                double T_numerator_fit_S = 0;
                double T_denominator_fit_S = 0;
                double T_numerator_fit_N = 0;
                double T_denominator_fit_N = 0;

                double T_numerator_num_S = 0;
                double T_denominator_num_S = 0;
                double T_numerator_num_N = 0;
                double T_denominator_num_N = 0;

                for (auto &zdc : zdc_map) {
                    if (zdc.first.find("zdcS") != string::npos && zdc.second.status == 1) {
                        T_numerator_S += zdc.second.peak_pos * zdc.second.peak_height;
                        T_denominator_S += zdc.second.peak_height;

                        T_numerator_fit_S += zdc.second.fit->GetParameter(1) * (zdc.second.fit->GetParameter(0) - zdc.second.fit->GetParameter(3));
                        T_denominator_fit_S += (zdc.second.fit->GetParameter(0) - zdc.second.fit->GetParameter(3));

                        T_numerator_num_S += GetNumZDCtime(zdc.second.hist).first * GetNumZDCtime(zdc.second.hist).second;
                        T_denominator_num_S += GetNumZDCtime(zdc.second.hist).second;   

                    }
                    if (zdc.first.find("zdcN") != string::npos && zdc.second.status == 1) {
                        T_numerator_N += zdc.second.peak_pos * zdc.second.peak_height;
                        T_denominator_N += zdc.second.peak_height;

                        T_numerator_fit_N += zdc.second.fit->GetParameter(1) * (zdc.second.fit->GetParameter(0) - zdc.second.fit->GetParameter(3));
                        T_denominator_fit_N += (zdc.second.fit->GetParameter(0) - zdc.second.fit->GetParameter(3));

                        T_numerator_num_N += GetNumZDCtime(zdc.second.hist).first * GetNumZDCtime(zdc.second.hist).second;
                        T_denominator_num_N += GetNumZDCtime(zdc.second.hist).second;
                    }
                }

                double sample_offset = 1; // todo : the -1 is for the shift for the run 51195
                double T_final_S = ((T_numerator_S / T_denominator_S) - sample_offset) - (T_numerator_N / T_denominator_N);
                double T_final_fit_S = ((T_numerator_fit_S / T_denominator_fit_S) - sample_offset) - (T_numerator_fit_N / T_denominator_fit_N);
                double T_final_num_S = ((T_numerator_num_S / T_denominator_num_S) - sample_offset) - (T_numerator_num_N / T_denominator_num_N);

                // note:        cm/s           s                                                       
                float TSAMPLE = 1.0 / ( (9.4e+6) * 6 ); // note : the time span of each sample
                float z_ZDC = ((3e+10) * ( T_final_S ) * TSAMPLE) / 2.0;
                float z_ZDC_fit = ((3e+10) * ( T_final_fit_S ) * TSAMPLE) / 2.0;
                float z_ZDC_num = ((3e+10) * ( T_final_num_S ) * TSAMPLE) / 2.0;

                ZDC_vtxZ_num -> Fill(z_ZDC_num);
                ZDC_vtxZ_fit -> Fill(z_ZDC_fit);
                if (T_final_S != 0){
                    ZDC_vtxZ -> Fill(z_ZDC);
                }

                if (mbd_z_vtx != -999){
                    vtx_correlation -> Fill(mbd_z_vtx, z_ZDC);
                    
                    ZDC_vtxZ_num_out = z_ZDC_num;
                    ZDC_vtxZ_fit_out = z_ZDC_fit;
                    ZDC_vtxZ_out = z_ZDC;
                    mbd_vtxZ_out = mbd_z_vtx;
                    Ngood_waveform_S_out = (zdc_map["zdcS1"].status + zdc_map["zdcS2"].status + zdc_map["zdcS3"].status);
                    Ngood_waveform_N_out = (zdc_map["zdcN1"].status + zdc_map["zdcN2"].status + zdc_map["zdcN3"].status);
                    tree_out -> Fill();
                }
                
            }

        } 

        for (auto &zdc : zdc_map) {
            zdc.second.hist->Reset("ICESM");
            zdc.second.Ngroup = -999;
            zdc.second.peak_pos = -999;
            zdc.second.peak_height = -999;
            zdc.second.status = 0;
        }

    }
    
    c1 -> Print(Form("%s/ZDCNS_waveform.pdf)", output_directory.c_str()));

    file_out -> Write();
    file_out -> Close();

    c1 -> cd();
    for (auto &zdc : zdc_map)
    {
        c1 -> cd(zdc.second.index+1);
        zdc.second.h1_waveform_peak->Draw("hist");
    }
    c1 -> Print(Form("%s/ZDC_waveform_peak.pdf", output_directory.c_str()));

    c1 -> cd();
    for (auto &zdc : zdc_map)
    {
        c1 -> cd(zdc.second.index+1);
        zdc.second.h1_waveform_StdDev->Draw("hist");
    }
    c1 -> Print(Form("%s/ZDC_waveform_StdDev.pdf", output_directory.c_str()));


    TCanvas * c2 = new TCanvas("","",950, 800);
    c2 -> cd();
    ZDC_vtxZ -> Draw("hist");
    c2 -> Print(Form("%s/ZDC_vtxZ.pdf", output_directory.c_str()));
    c2 -> Clear();

    c2 -> cd();
    vtx_correlation -> Draw("colz0");
    c2 -> Print(Form("%s/vtx_correlation.pdf", output_directory.c_str()));
    c2 -> Clear();

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