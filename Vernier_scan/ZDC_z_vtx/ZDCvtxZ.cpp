#include "ZDCvtxZ.h"

ZDCvtxZ::ZDCvtxZ(
    long long nRunEvent_in, 
    string input_full_directory_in,
    string output_directory_in,
    bool NS_offset_in
):
nRunEvent(nRunEvent_in),
input_full_directory(input_full_directory_in),
output_directory(output_directory_in),
NS_offset(NS_offset_in)
{
    zdc_map.clear();
    zdc_map["zdcS1"] = {
        .index = 0, 
        .name = "zdcS1_waveform", 
        .waveform = 0, 
        .hist = new TH1F("zdcS1", "zdcS1", 16, 0, 16), 
        .fit = new TF1("",land_offset, 0, 16, 4),
        .fit2 = new TF1("",charge_discharge_func,0,16,5),
        .h1_waveform_peak = new TH1F("",";zdcS1_PeakSample;Entries",16,0,16),
        .h1_waveform_StdDev = new TH1F("",";zdcS1_SetDeV;Entries",100,0,150),
        .h1_waveform_fit_width = new TH1F("",";zdcS1_FitWidth;Entries",100,0,2),
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
        .fit2 = new TF1("",charge_discharge_func,0,16,5),
        .h1_waveform_peak = new TH1F("",";zdcS2_PeakSample;Entries",16,0,16),
        .h1_waveform_StdDev = new TH1F("",";zdcS2_SetDeV;Entries",100,0,150),
        .h1_waveform_fit_width = new TH1F("",";zdcS2_FitWidth;Entries",100,0,2),
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
        .fit2 = new TF1("",charge_discharge_func,0,16,5),
        .h1_waveform_peak = new TH1F("",";zdcS3_PeakSample;Entries",16,0,16),
        .h1_waveform_StdDev = new TH1F("",";zdcS3_SetDeV;Entries",100,0,150),
        .h1_waveform_fit_width = new TH1F("",";zdcS3_FitWidth;Entries",100,0,2),
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
        .fit2 = new TF1("",charge_discharge_func,0,16,5),
        .h1_waveform_peak = new TH1F("",";zdcN1_PeakSample;Entries",16,0,16),
        .h1_waveform_StdDev = new TH1F("",";zdcN1_SetDeV;Entries",100,0,150),
        .h1_waveform_fit_width = new TH1F("",";zdcN1_FitWidth;Entries",100,0,2),
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
        .fit2 = new TF1("",charge_discharge_func,0,16,5),
        .h1_waveform_peak = new TH1F("",";zdcN2_PeakSample;Entries",16,0,16),
        .h1_waveform_StdDev = new TH1F("",";zdcN2_SetDeV;Entries",100,0,150),
        .h1_waveform_fit_width = new TH1F("",";zdcN2_FitWidth;Entries",100,0,2),
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
        .fit2 = new TF1("",charge_discharge_func,0,16,5),
        .h1_waveform_peak = new TH1F("",";zdcN3_PeakSample;Entries",16,0,16),
        .h1_waveform_StdDev = new TH1F("",";zdcN3_SetDeV;Entries",100,0,150),
        .h1_waveform_fit_width = new TH1F("",";zdcN3_FitWidth;Entries",100,0,2),
        .status = 0,
        .Ngroup = -999,
        .peak_pos = -999,
        .peak_height = -999
    };

    file_in = TFile::Open(input_full_directory.c_str());
    tree = (TTree*)file_in->Get("tree");
    tree->SetBranchStatus("*", 0);
    tree->SetBranchStatus("evtID", 1);
    tree->SetBranchStatus("evtBCO_gl1", 1);
    tree->SetBranchStatus("bunchnumber", 1);
    tree->SetBranchStatus("LiveTrigger_Vec", 1);
    tree->SetBranchStatus("mbd_z_vtx", 1);
    for (auto &zdc : zdc_map) {
        tree->SetBranchStatus(zdc.second.name.c_str(), 1);
    }

    tree->SetBranchAddress("evtID", &evtID);
    tree->SetBranchAddress("evtBCO_gl1", &evtBCO_gl1);
    tree->SetBranchAddress("bunchnumber", &bunchnumber);
    tree->SetBranchAddress("LiveTrigger_Vec", &live_trigger_vec);
    tree->SetBranchAddress("mbd_z_vtx", &mbd_z_vtx);
    for (auto &zdc : zdc_map) {
        tree->SetBranchAddress(zdc.second.name.c_str(), &zdc.second.waveform);
    }

    SetsPhenixStyle();
    c1 = new TCanvas("c1", "c1", 1200, 800);
    c1 -> Divide(3,2);
    c1 -> Print(Form("%s/ZDCNS_waveform.pdf(", output_directory.c_str()));
    TGaxis::SetMaxDigits(4);

    c2 = new TCanvas("","",950, 800);

    c1 -> cd();

    ZDC_vtxZ_fit = new TH1F("ZDC_vtxZ_fit", "ZDC_vtxZ_fit;ZDC_vtxZ_fit [cm];Entries", 100, -500, 500);
    ZDC_vtxZ_fit -> GetXaxis() -> SetNdivisions(505);
    ZDC_vtxZ_num = new TH1F("ZDC_vtxZ_num", "ZDC_vtxZ_num;ZDC_vtxZ_num [cm];Entries", 100, -500, 500);
    ZDC_vtxZ_num -> GetXaxis() -> SetNdivisions(505);
    ZDC_vtxZ_charge = new TH1F("ZDC_vtxZ_charge", "ZDC_vtxZ_charge;ZDC_vtxZ_charge [cm];Entries", 100, -500, 500);
    ZDC_vtxZ_charge -> GetXaxis() -> SetNdivisions(505);
    ZDC_vtxZ = new TH1F("ZDC_vtxZ", "ZDC_vtxZ;ZDC_vtxZ [cm];Entries", 100, -500, 500);
    ZDC_vtxZ -> GetXaxis() -> SetNdivisions(505);

    vtx_correlation = new TH2F("",";MBD_vtxZ [cm];ZDC_vtxZ [cm]",150,-300,300,150,-300,300);
    vtx_correlation -> GetXaxis() -> SetNdivisions(505);
    vtx_diff = new TH1F("",";MBD_vtxZ - ZDC_vtxZ [cm];Entries",100,-400,400);
    vtx_diff -> GetXaxis() -> SetNdivisions(505);

    vtx_correlation_charge = new TH2F("",";MBD_vtxZ [cm];ZDC_vtxZ [cm]",150,-300,300,150,-300,300);
    vtx_correlation_charge -> GetXaxis() -> SetNdivisions(505);
    vtx_diff_charge = new TH1F("",";MBD_vtxZ - ZDC_vtxZ [cm];Entries",100,-400,400);
    vtx_diff_charge -> GetXaxis() -> SetNdivisions(505);

    MBD_vtxZ_dist_inclusive = new TH1F("MBD_vtxZ_dist_inclusive", "MBD_vtxZ_dist_inclusive;MBD_vtxZ [cm];Entries", 100, -300, 300);
    MBD_vtxZ_dist_inclusive -> SetLineColor(kRed);
    MBD_vtxZ_dist_inclusive -> GetXaxis() -> SetNdivisions(505);    

    MBD_vtxZ_dist_MBDNSTrig = new TH1F("MBD_vtxZ_dist_MBDNSTrig", "MBD_vtxZ_dist_MBDNSTrig;MBD_vtxZ [cm];Entries", 100, -300, 300);
    MBD_vtxZ_dist_MBDNSTrig -> SetLineColor(kGreen);
    MBD_vtxZ_dist_MBDNSTrig -> SetMarkerSize(0.2);
    MBD_vtxZ_dist_MBDNSTrig -> SetMarkerColor(kGreen);
    MBD_vtxZ_dist_MBDNSTrig -> GetXaxis() -> SetNdivisions(505);

    MBD_vtxZ_dist_ZDCNSTrig = new TH1F("MBD_vtxZ_dist_ZDCNSTrig", "MBD_vtxZ_dist_ZDCNSTrig;MBD_vtxZ [cm];Entries", 100, -300, 300);
    MBD_vtxZ_dist_ZDCNSTrig -> SetLineColor(kBlue);
    MBD_vtxZ_dist_ZDCNSTrig -> GetXaxis() -> SetNdivisions(505);


    file_out = new TFile(Form("%s/ZDC_vtxZ.root", output_directory.c_str()), "recreate");
    tree_out = new TTree("tree", "tree");
    tree_out -> Branch("ZDC_vtxZ_num", &ZDC_vtxZ_num_out);
    tree_out -> Branch("ZDC_vtxZ_fit", &ZDC_vtxZ_fit_out);
    tree_out -> Branch("ZDC_vtxZ", &ZDC_vtxZ_out);
    tree_out -> Branch("mbd_vtxZ", &mbd_vtxZ_out);
    tree_out -> Branch("Ngood_waveform_S", &Ngood_waveform_S_out);
    tree_out -> Branch("Ngood_waveform_N", &Ngood_waveform_N_out);

    draw_text = new TLatex();
    draw_text -> SetNDC();
    draw_text -> SetTextSize(0.03);

    coord_line = new TLine();
    coord_line -> SetLineWidth(1);
    coord_line -> SetLineColor(16);
    coord_line -> SetLineStyle(2);

    legend = new TLegend(0.25,0.8,0.50,0.9);
    // legend -> SetMargin(0);
    legend->SetTextSize(0.03);
}

void ZDCvtxZ::RunEvent()
{

    long long final_nEvt = (nRunEvent == -1 || nRunEvent > tree->GetEntries()) ? tree->GetEntries() : nRunEvent;

    for (int i = 0; i < final_nEvt; i++)
    {
        tree -> GetEntry(i);

        if (i % 1000 == 0) {cout<<"eventID : "<<i<<endl;}

        live_trigger_map_evt.clear();
        for (int i = 0; i < live_trigger_vec->size(); i++) {
            live_trigger_map_evt[live_trigger_vec->at(i)] = 1;
        }

        if(mbd_z_vtx != -999){
            MBD_vtxZ_dist_inclusive -> Fill(mbd_z_vtx);
        }

        if (live_trigger_map_evt.find(MBDNS_trigger_ID) != live_trigger_map_evt.end() && mbd_z_vtx != -999){
            MBD_vtxZ_dist_MBDNSTrig -> Fill(mbd_z_vtx);
        }

        if (live_trigger_map_evt.find(ZDCNS_trigger_ID) != live_trigger_map_evt.end() && mbd_z_vtx != -999) {
            // cout<<"eID : "<<i<<" has ZDCNS trigger"<<endl;

            MBD_vtxZ_dist_ZDCNSTrig -> Fill(mbd_z_vtx);

            for (auto &zdc : zdc_map) {
                for (int i = 0; i < zdc.second.waveform->size(); i++) {
                    zdc.second.hist->SetBinContent(i+1, zdc.second.waveform->at(i));
                }

                zdc.second.hist->SetTitle(Form(";%s;A. U.",zdc.second.name.c_str()));
                baseline = hist_bkgrm(zdc.second.hist);
                // zdc.second.hist->SetMinimum( zdc.second.hist->GetBinContent(zdc.second.hist->GetMinimumBin()) * 0.95 );
                zdc.second.hist->SetMaximum( zdc.second.hist->GetBinContent(zdc.second.hist->GetMaximumBin()) + (zdc.second.hist->GetBinContent(zdc.second.hist->GetMaximumBin()) - baseline) * 1 );
                vector<double> Ngroup_info = find_Ngroup(zdc.second.hist, baseline);

                zdc.second.Ngroup = Ngroup_info[0];
                zdc.second.status = (Ngroup_info[0] == 1) ? 1 : 0;
                zdc.second.peak_pos = zdc.second.hist -> GetMaximumBin();
                zdc.second.peak_height = zdc.second.hist->GetBinContent(zdc.second.hist->GetMaximumBin()) - baseline;

                if (Ngroup_info[0] == 1){
                    zdc.second.h1_waveform_peak->Fill(zdc.second.hist->GetMaximumBin()-1);
                }
                
                zdc.second.h1_waveform_StdDev->Fill(hist_stddev(zdc.second.hist));

                // note : landau fitting setting 
                zdc.second.fit->SetLineColor(kRed);
                zdc.second.fit->SetParameters(
                    zdc.second.hist->GetBinContent(zdc.second.hist->GetMaximumBin()) - baseline,
                    zdc.second.hist->GetMaximumBin(),
                    0.3,
                    baseline
                );
                zdc.second.fit->SetParLimits(0, 0, (zdc.second.hist->GetBinContent(zdc.second.hist->GetMaximumBin()) - baseline) * 1.5); // note : the size of the landau
                zdc.second.fit->SetParLimits(1, Ngroup_info[2], Ngroup_info[3]); // note : the mpv of the landau
                zdc.second.fit->SetParLimits(3,   - 20, baseline + 20); // note : the offset of the landau

                zdc.second.hist->Fit(zdc.second.fit, "NQ","", zdc.second.hist->GetMaximumBin()-3, zdc.second.hist->GetMaximumBin()+4);
                zdc.second.h1_waveform_fit_width->Fill(zdc.second.fit->GetParameter(2));

                // note : charge discharge fitting setting
                zdc.second.fit2->SetLineColor(kGreen);
                zdc.second.fit2->SetParameters(
                    baseline,
                    zdc.second.hist->GetBinLowEdge(zdc.second.hist->GetMaximumBin()),
                    42, 
                    0.888,
                    zdc.second.hist->GetBinContent(zdc.second.hist->GetMaximumBin()) - baseline
                );
                zdc.second.fit2->SetParLimits(0, baseline - 20, baseline + 20); // note : the offset of the charge discharge
                zdc.second.fit2->SetParLimits(1, zdc.second.hist->GetBinCenter(zdc.second.hist->GetMaximumBin())-1, zdc.second.hist->GetBinCenter(zdc.second.hist->GetMaximumBin())+1);
                zdc.second.hist->Fit(zdc.second.fit2, "NQ","", zdc.second.hist->GetMaximumBin()-3, zdc.second.hist->GetMaximumBin()+4);

                c1 -> cd(zdc.second.index+1);
                zdc.second.hist->Draw("hist");
                coord_line -> DrawLine(0, (zdc.second.hist->GetBinContent(zdc.second.hist->GetMaximumBin()) + baseline)/2., 16, (zdc.second.hist->GetBinContent(zdc.second.hist->GetMaximumBin()) + baseline)/2.);
                zdc.second.fit->Draw("l same");
                zdc.second.fit2->Draw("l same");

                draw_text -> DrawLatex(0.2, 0.87, Form("eID: %d", i));
                draw_text -> DrawLatex(0.2, 0.84, Form("StdDev: %.2f", vector_stddev(*zdc.second.waveform)));
                draw_text -> DrawLatex(0.2, 0.81, Form("Ngroup: %.0f", Ngroup_info[0]));
                draw_text -> DrawLatex(0.2, 0.78, Form("fit PeakPos: %.2f", zdc.second.fit->GetParameter(1)));
                draw_text -> DrawLatex(0.2, 0.75, Form("fit LandSize: %.2f", zdc.second.fit->GetParameter(0)));
                draw_text -> DrawLatex(0.2, 0.72, Form("fit LandWidth: %.2f", zdc.second.fit->GetParameter(2)));
                draw_text -> DrawLatex(0.2, 0.69, Form("mpv fit limits: %.2f ~ %.2f", Ngroup_info[2], Ngroup_info[3]));
            }

            if (
                (zdc_map["zdcS1"].status + zdc_map["zdcS2"].status + zdc_map["zdcS3"].status) > 1 &&
                (zdc_map["zdcN1"].status + zdc_map["zdcN2"].status + zdc_map["zdcN3"].status) > 1
                // zdc_map["zdcS1"].fit -> GetParameter(2) > 0.25 && 
                // zdc_map["zdcS2"].fit -> GetParameter(2) > 0.25 &&
                // zdc_map["zdcS3"].fit -> GetParameter(2) > 0.25 &&
                // zdc_map["zdcN1"].fit -> GetParameter(2) > 0.25 &&
                // zdc_map["zdcN2"].fit -> GetParameter(2) > 0.25 &&
                // zdc_map["zdcN3"].fit -> GetParameter(2) > 0.25
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

                double T_numerator_charge_S = 0;
                double T_denominator_charge_S = 0;
                double T_numerator_charge_N = 0;
                double T_denominator_charge_N = 0;

                int final_good_module_S = 0;
                int final_good_module_N = 0;

                for (auto &zdc : zdc_map) {
                    if (zdc.first.find("zdcS") != string::npos && zdc.second.status == 1 && zdc.second.fit -> GetParameter(2) > 0.25) {
                        T_numerator_S += zdc.second.peak_pos * zdc.second.peak_height;
                        T_denominator_S += zdc.second.peak_height;

                        T_numerator_fit_S += zdc.second.fit->GetParameter(1) * (zdc.second.fit->GetParameter(0) - zdc.second.fit->GetParameter(3));
                        T_denominator_fit_S += (zdc.second.fit->GetParameter(0) - zdc.second.fit->GetParameter(3));

                        T_numerator_charge_S += zdc.second.fit2->GetParameter(1) * (zdc.second.fit->GetParameter(0) - zdc.second.fit->GetParameter(3));
                        T_denominator_charge_S += (zdc.second.fit->GetParameter(0) - zdc.second.fit->GetParameter(3));

                        T_numerator_num_S += GetNumZDCtime(zdc.second.hist).first * GetNumZDCtime(zdc.second.hist).second;
                        T_denominator_num_S += GetNumZDCtime(zdc.second.hist).second;   

                        final_good_module_S++;

                    }
                    if (zdc.first.find("zdcN") != string::npos && zdc.second.status == 1 && zdc.second.fit -> GetParameter(2) > 0.25) {
                        T_numerator_N += zdc.second.peak_pos * zdc.second.peak_height;
                        T_denominator_N += zdc.second.peak_height;

                        T_numerator_fit_N += zdc.second.fit->GetParameter(1) * (zdc.second.fit->GetParameter(0) - zdc.second.fit->GetParameter(3));
                        T_denominator_fit_N += (zdc.second.fit->GetParameter(0) - zdc.second.fit->GetParameter(3));

                        T_numerator_charge_N += zdc.second.fit2->GetParameter(1) * (zdc.second.fit->GetParameter(0) - zdc.second.fit->GetParameter(3));
                        T_denominator_charge_N += (zdc.second.fit->GetParameter(0) - zdc.second.fit->GetParameter(3));

                        T_numerator_num_N += GetNumZDCtime(zdc.second.hist).first * GetNumZDCtime(zdc.second.hist).second;
                        T_denominator_num_N += GetNumZDCtime(zdc.second.hist).second;

                        final_good_module_N++;
                    }
                }

                double sample_offset = (NS_offset) ? 1 : 0; // todo : the -1 is for the shift for the run 51195
                double T_final_S = ((T_numerator_S / T_denominator_S) - sample_offset) - (T_numerator_N / T_denominator_N);
                double T_final_fit_S = ((T_numerator_fit_S / T_denominator_fit_S) - sample_offset) - (T_numerator_fit_N / T_denominator_fit_N);
                double T_final_num_S = ((T_numerator_num_S / T_denominator_num_S) - sample_offset) - (T_numerator_num_N / T_denominator_num_N);
                double T_final_charge_S = ((T_numerator_charge_S / T_denominator_charge_S) - sample_offset) - (T_numerator_charge_N / T_denominator_charge_N);

                // note:        cm/s           s                                                       
                float TSAMPLE = 1.0 / ( (9.4e+6) * 6 ); // note : the time span of each sample
                float z_ZDC = ((3e+10) * ( T_final_S ) * TSAMPLE) / 2.0;
                z_ZDC_fit = ((3e+10) * ( T_final_fit_S ) * TSAMPLE) / 2.0;
                z_ZDC_charge = ((3e+10) * ( T_final_charge_S ) * TSAMPLE) / 2.0;
                float z_ZDC_num = ((3e+10) * ( T_final_num_S ) * TSAMPLE) / 2.0;

                if (
                        mbd_z_vtx != -999 && 
                        ZDC_good(zdc_map["zdcS1"].peak_pos, zdc_map["zdcS2"].peak_pos, zdc_map["zdcS3"].peak_pos) &&
                        ZDC_good(zdc_map["zdcN1"].peak_pos, zdc_map["zdcN2"].peak_pos, zdc_map["zdcN3"].peak_pos) &&
                        fabs(zdc_map["zdcS1"].peak_pos - zdc_map["zdcN1"].peak_pos) < 2 && 
                        final_good_module_N > 1 && 
                        final_good_module_S > 1
                    )
                {
                    ZDC_vtxZ_charge -> Fill(z_ZDC_charge);
                    ZDC_vtxZ_num -> Fill(z_ZDC_num);
                    ZDC_vtxZ_fit -> Fill(z_ZDC_fit);
                    if (T_final_S != 0){
                        ZDC_vtxZ -> Fill(z_ZDC);
                    }

                    vtx_correlation -> Fill(mbd_z_vtx, z_ZDC_fit);
                    vtx_diff -> Fill(mbd_z_vtx - z_ZDC_fit);

                    vtx_correlation_charge -> Fill(mbd_z_vtx, z_ZDC_charge);
                    vtx_diff_charge -> Fill(mbd_z_vtx - z_ZDC_charge);

                    c1 -> cd(1);
                    draw_text -> DrawLatex(0.2, 0.66, Form("ZDCvtxZ: %.2f cm", z_ZDC_fit));
                    c1 -> cd(2);
                    draw_text -> DrawLatex(0.2, 0.66, Form("MBDvtxZ: %.2f cm", mbd_z_vtx));
                    
                    ZDC_vtxZ_num_out = z_ZDC_num;
                    ZDC_vtxZ_fit_out = z_ZDC_fit;
                    ZDC_vtxZ_out = z_ZDC;
                    mbd_vtxZ_out = mbd_z_vtx;
                    Ngood_waveform_S_out = (zdc_map["zdcS1"].status + zdc_map["zdcS2"].status + zdc_map["zdcS3"].status);
                    Ngood_waveform_N_out = (zdc_map["zdcN1"].status + zdc_map["zdcN2"].status + zdc_map["zdcN3"].status);
                    tree_out -> Fill();

                    if (fabs(z_ZDC_fit - mbd_z_vtx) > 200) {
                        c1 -> Print(Form("%s/ZDCNS_waveform.pdf", output_directory.c_str()));
                        c1 -> Clear();
                        c1 -> Divide(3,2);
                    }
                }
                
            }

            

        } 

        ClearEvent();

    }
}

void ZDCvtxZ::ClearEvent()
{
    for (auto &zdc : zdc_map) {
        zdc.second.hist->Reset("ICESM");
        zdc.second.Ngroup = -999;
        zdc.second.peak_pos = -999;
        zdc.second.peak_height = -999;
        zdc.second.status = 0;
    }
}

void ZDCvtxZ::DrawPlots()
{
    c1 -> Print(Form("%s/ZDCNS_waveform.pdf)", output_directory.c_str()));

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

    c1 -> cd();
    for (auto &zdc : zdc_map)
    {
        c1 -> cd(zdc.second.index+1);
        c1 -> SetLogy(1);
        zdc.second.h1_waveform_fit_width->Draw("hist");
    }
    c1 -> Print(Form("%s/ZDC_waveform_fit_width.pdf", output_directory.c_str()));


    c2 -> cd();
    ZDC_vtxZ_fit -> Draw("hist");
    c2 -> Print(Form("%s/ZDC_vtxZ_fit.pdf", output_directory.c_str()));
    c2 -> Clear();

    c2 -> cd();
    ZDC_vtxZ_charge -> Draw("hist");
    c2 -> Print(Form("%s/ZDC_vtxZ_charge.pdf", output_directory.c_str()));
    c2 -> Clear();

    c2 -> cd();
    vtx_correlation -> Draw("colz0");
    c2 -> Print(Form("%s/vtx_correlation_fit.pdf", output_directory.c_str()));
    c2 -> Clear();

    c2 -> cd();
    vtx_diff -> Draw("hist");
    c2 -> Print(Form("%s/vtx_diff_fit.pdf", output_directory.c_str()));
    c2 -> Clear();

    c2 -> cd();
    vtx_correlation_charge -> Draw("colz0");
    c2 -> Print(Form("%s/vtx_correlation_charge.pdf", output_directory.c_str()));
    c2 -> Clear();

    c2 -> cd();
    vtx_diff_charge -> Draw("hist");
    c2 -> Print(Form("%s/vtx_diff_charge.pdf", output_directory.c_str()));
    c2 -> Clear();

    c2 -> cd();
    legend -> AddEntry(MBD_vtxZ_dist_inclusive, "Inclusive", "f");
    legend -> AddEntry(MBD_vtxZ_dist_ZDCNSTrig, "ZDCNSTrig", "f");
    legend -> AddEntry(MBD_vtxZ_dist_MBDNSTrig, "MBDNSTrig", "l");
    MBD_vtxZ_dist_inclusive -> Scale(1./MBD_vtxZ_dist_inclusive->Integral(-1,-1));
    MBD_vtxZ_dist_MBDNSTrig -> Scale(1./MBD_vtxZ_dist_MBDNSTrig->Integral(-1,-1));
    MBD_vtxZ_dist_ZDCNSTrig -> Scale(1./MBD_vtxZ_dist_ZDCNSTrig->Integral(-1,-1));
    MBD_vtxZ_dist_inclusive -> SetMaximum(1.5 * max(MBD_vtxZ_dist_inclusive->GetBinContent(MBD_vtxZ_dist_inclusive->GetMaximumBin()), MBD_vtxZ_dist_ZDCNSTrig->GetBinContent(MBD_vtxZ_dist_ZDCNSTrig->GetMaximumBin())));
    // MBD_vtxZ_dist_inclusive -> SetMinimum(0);
    MBD_vtxZ_dist_inclusive -> GetYaxis() -> SetTitle("Entries [A.U.]");
    MBD_vtxZ_dist_inclusive -> Draw("hist");
    MBD_vtxZ_dist_ZDCNSTrig -> Draw("hist same");
    MBD_vtxZ_dist_MBDNSTrig -> Draw("ep same");
    legend -> Draw(" same ");
    c2 -> Print(Form("%s/MBD_vtxZ_dist_comp.pdf", output_directory.c_str()));
    c2 -> SetLogy(1);
    c2 -> Print(Form("%s/MBD_vtxZ_dist_comp_log.pdf", output_directory.c_str()));
    c2 -> Clear();
}


void ZDCvtxZ::CloseRootFileOut()
{

    ZDC_vtxZ_fit -> Write("ZDC_vtxZ_fit");
    vtx_correlation -> Write("vtx_correlation_fit");
    vtx_diff -> Write("vtx_diff_fit");

    MBD_vtxZ_dist_inclusive -> Write("MBD_vtxZ_dist_inclusive");
    MBD_vtxZ_dist_MBDNSTrig -> Write("MBD_vtxZ_dist_MBDNSTrig");
    MBD_vtxZ_dist_ZDCNSTrig -> Write("MBD_vtxZ_dist_ZDCNSTrig");

    file_out -> Write();
    file_out -> Close();
}


bool ZDCvtxZ::ZDC_good(int peak1, int peak2, int peak3)
{
    int diff_12 = abs(peak1 - peak2);
    int diff_13 = abs(peak1 - peak3);
    int diff_23 = abs(peak2 - peak3);
    double max_diff = max(diff_12, max(diff_13, diff_23));

    return (max_diff < 2) ? true : false;
}

double ZDCvtxZ::vector_stddev (vector <double> input_vector){
	
	double sum_subt = 0;
	double average  = accumulate( input_vector.begin(), input_vector.end(), 0.0 ) / double(input_vector.size());
	
	// cout<<"average is : "<<average<<endl;

	for (int i=0; i<input_vector.size(); i++){ sum_subt += pow((input_vector[i] - average),2); }

	//cout<<"sum_subt : "<<sum_subt<<endl;
	// cout<<"print from the function, average : "<<average<<" std : "<<stddev<<endl;

	return sqrt( sum_subt / double(input_vector.size()-1) );
}

double ZDCvtxZ::hist_stddev (TH1F * hist_in)
{
    vector<double> hist_content; hist_content.clear();
    for (int i = 0; i < hist_in -> GetNbinsX(); i++){
        hist_content.push_back(hist_in -> GetBinContent(i+1));
    }

    return vector_stddev(hist_content);
}

double ZDCvtxZ::hist_bkgrm(TH1F * hist_in)
{
    double baseline_left = 0.;
    double baseline_right = 0.;

    for(int s = 0; s < 3; s++) { baseline_left += hist_in -> GetBinContent(s+1); }
    for (int s = hist_in -> GetNbinsX()-3; s < hist_in -> GetNbinsX(); s++) { baseline_right += hist_in -> GetBinContent(s+1); }

    baseline_left /= 3.;
    baseline_right /=3.;
    double baseline = (baseline_left < baseline_right) ? baseline_left : baseline_right;

    // for(int s = 0; s < hist_in -> GetNbinsX(); s++) { 
        
    //     if (hist_in -> GetBinContent(s+1) < baseline) {hist_in -> SetBinContent(s+1, 0.); continue; }

    //     hist_in -> SetBinContent(s+1, hist_in -> GetBinContent(s+1) - baseline); 
    // }

    return baseline;
}

vector<double> ZDCvtxZ::find_Ngroup(TH1F * hist_in, double baseline)
{
    // double baseline = hist_bkgrm(hist_in);

    double Highest_bin_Content  = hist_in -> GetBinContent(hist_in -> GetMaximumBin());
    double Highest_bin_Center   = hist_in -> GetBinCenter(hist_in -> GetMaximumBin());

    double threshold_cut = (Highest_bin_Content + baseline) / 2.;

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
        double bin_content = ( hist_in -> GetBinContent(i+1) <= threshold_cut) ? 0. : ( hist_in -> GetBinContent(i+1) - threshold_cut );

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

pair<double, double> ZDCvtxZ::GetFitRangeHist(TH1F * hist_in)
{
    int peak_bin = hist_in -> GetMaximumBin(); // note : 1 ~ 16
    if (peak_bin > 3 && peak_bin < 14) {return {3., 13.};}
    else if (peak_bin < 4) {return {0., 10.};} // note : peak at bin 2, 3
    else {return {10., 16.};} // note : peak at bin 14, 15
}

pair<double, double> ZDCvtxZ::GetNumZDCtime(TH1F * hist_in)
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