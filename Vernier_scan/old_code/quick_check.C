int quick_check()
{
    string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/test_run_48029";
    string input_filename = "ntuple_run48029_merged.root";
    string output_directory = input_directory + "/quick_check";

    TFile * file_in = TFile::Open((input_directory + "/" + input_filename).c_str());
    TTree * tree = (TTree*)file_in->Get("tree");
    tree->SetBranchStatus("*", 0);
    tree->SetBranchStatus("evtID", 1);
    tree->SetBranchStatus("evtBCO_gl1", 1);
    tree->SetBranchStatus("evtBCO_zdc", 1);
    tree->SetBranchStatus("bunchnumber", 1);

    long long evtID;
    long long evtBCO_gl1;
    long long evtBCO_zdc;
    int bunchnumber;

    tree->SetBranchAddress("evtID", &evtID);
    tree->SetBranchAddress("evtBCO_gl1", &evtBCO_gl1);
    tree->SetBranchAddress("evtBCO_zdc", &evtBCO_zdc);
    tree->SetBranchAddress("bunchnumber", &bunchnumber);

    int n_entries = tree->GetEntries();

    int pre_evtBCO_gl1 = 0;
    int pre_evtBCO_zdc = 0;

    // TGraph * gr_diffBCO_gl1 = new TGraph();
    // gr_diffBCO_gl1 -> SetMarkerStyle(20);
    // TGraph * gr_diffBCO_zdc = new TGraph();
    // gr_diffBCO_zdc -> SetMarkerStyle(20);
    // TGraph * gr_diff_BCOGl1_BCOZDC = new TGraph();
    // gr_diff_BCOGl1_BCOZDC -> SetMarkerStyle(20);

    TGraph * gr_BCO_gl1 = new TGraph();
    gr_BCO_gl1 -> SetMarkerStyle(20);
    TGraph * gr_BCO_zdc = new TGraph();
    gr_BCO_zdc -> SetMarkerStyle(20);

    long long good_evt_count = 0;
    long long real_first_bcogl1 = 0;
    long long real_first_bcozdc = 0;
    bool pre_good_evt = false;
    bool this_good_evt = false;

    double bco_span = 106 * pow(10, -9);


    // note : 3600 corresponds to each second in 1 hour, since the Vernier scan is around 32 minutes, so 3600 is enough
    int gl1_evt_counting[3600];
    memset(gl1_evt_counting, 0, sizeof(gl1_evt_counting));
    int zdc_evt_counting[3600];
    memset(zdc_evt_counting, 0, sizeof(zdc_evt_counting));


    for (int evt_i = 0; evt_i < tree->GetEntries(); evt_i++)
    {
        tree->GetEntry(evt_i);

        this_good_evt = false;

        if (bunchnumber == -999) {pre_good_evt = false; continue;}
        if (evtBCO_gl1 == -999) {pre_good_evt = false; continue;}
        if (evtBCO_zdc == -999) {pre_good_evt = false; continue;}

        this_good_evt = true;

        if (good_evt_count == 0)
        {
            real_first_bcogl1 = evtBCO_gl1;
            real_first_bcozdc = evtBCO_zdc;

            gl1_evt_counting[0] += 1;
            zdc_evt_counting[0] += 1;
        }
        else 
        {
            if (evtBCO_gl1 < real_first_bcogl1) {
                cout<<"In event : "<<evt_i<<" evtBCO_gl1 = "<<evtBCO_gl1<<" real_first_bcogl1 = "<<real_first_bcogl1<<endl;
                cout<<"It shouldn't hapeen !!!, please check"<<endl;
                exit(0);
            }

            // if (evtBCO_zdc < real_first_bcozdc) {
            //     cout<<"In event : "<<evt_i<<" evtBCO_zdc = "<<evtBCO_zdc<<" real_first_bcozdc = "<<real_first_bcozdc<<endl;
            //     cout<<"It shouldn't hapeen !!!, please check"<<endl;
            //     exit(0);
            // }

            int diffBCO_gl1 = evtBCO_gl1 - pre_evtBCO_gl1;
            int diffBCO_zdc = evtBCO_zdc - pre_evtBCO_zdc;

            if (pre_good_evt == true && this_good_evt == true)
            {
                if (diffBCO_gl1 <= 0 || diffBCO_zdc <= 0)
                {
                    cout<<"aaaaaaaa - In event : "<<evt_i<<" this_evtBCO_gl1 = "<<evtBCO_gl1<<" pre_evtBCO_gl1 = "<<pre_evtBCO_gl1<<" this_evtBCO_zdc = "<<evtBCO_zdc<<" pre_evtBCO_zdc = "<<pre_evtBCO_zdc<<" diffBCO_gl1 = "<<diffBCO_gl1<<" diffBCO_zdc = "<<diffBCO_zdc<<endl;
                }
            }

            int gl1_evt_realtime_int = int((evtBCO_gl1 - real_first_bcogl1) * bco_span);
            int zdc_evt_realtime_int = int((evtBCO_zdc - real_first_bcozdc) * bco_span);

            gl1_evt_counting[gl1_evt_realtime_int] += 1;
            zdc_evt_counting[zdc_evt_realtime_int] += 1;

            if (evt_i % 1000 == 0)
            {
                cout<<"In event : "<<evt_i<<" evtBCO_gl1 = "<<evtBCO_gl1<<" evt_gl1_real_time : "<<gl1_evt_realtime_int<<" evtBCO_zdc = "<<evtBCO_zdc<<" evt_zdc_real_time : "<< zdc_evt_realtime_int <<endl;
            }

            if (evt_i % 100 == 0)
            {
                gr_BCO_gl1 -> SetPoint(good_evt_count, evt_i, evtBCO_gl1);
                gr_BCO_zdc -> SetPoint(good_evt_count, evt_i, evtBCO_zdc);
            }
        }

        pre_good_evt = this_good_evt;
        good_evt_count += 1;
        pre_evtBCO_gl1 = evtBCO_gl1;
        pre_evtBCO_zdc = evtBCO_zdc;     
    }

    TCanvas * c1 = new TCanvas("c1", "c1", 950, 800);
    c1 -> cd();

    std::vector<int> numbers(3600); // Create a vector with 1001 elements
    std::iota(numbers.begin(), numbers.end(), 0); 

    TGraph * gr_rate_gl1 = new TGraph(numbers.size(), &numbers[0], gl1_evt_counting);
    gr_rate_gl1 -> Draw("apl");
    c1 -> Print(Form("%s/evt_rate_gl1.pdf", output_directory.c_str()));
    c1 -> Clear();

    TGraph * gr_rate_zdc = new TGraph(numbers.size(), &numbers[0], zdc_evt_counting);
    gr_rate_zdc -> Draw("apl");
    c1 -> Print(Form("%s/evt_rate_zdc.pdf", output_directory.c_str()));
    c1 -> Clear();


    gr_BCO_gl1 -> Draw("ap");
    c1 -> Print(Form("%s/evtBCO_gl1.pdf", output_directory.c_str()));
    c1 -> Clear();
    
    gr_BCO_zdc -> Draw("ap");
    c1 -> Print(Form("%s/evtBCO_zdc.pdf", output_directory.c_str()));
    c1 -> Clear();

    return 0;

}