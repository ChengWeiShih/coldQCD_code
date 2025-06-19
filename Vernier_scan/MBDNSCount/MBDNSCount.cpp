// note : Y
vector<double> Average_BPM_pos_V;
vector<double> StdDev_BPM_pos_V;
vector<double> BeamIntensity_corr_V;

// note : X
vector<double> Average_BPM_pos_H;
vector<double> StdDev_BPM_pos_H;
vector<double> BeamIntensity_corr_H;

vector<double> BPM_set_pos_V = { // note : according to the second vernier scan, the run 51195
    0.0, 0.1, 0.25, 0.4, 0.6, 0.9,
    0.0, -0.1, -0.25, -0.4, -0.6, -0.9
};
vector<double> BPM_set_pos_H = { // note : according to the second vernier scan, the run 51195
    0.0, 0.1, 0.25, 0.4, 0.6, 0.9,
    0.0, -0.1, -0.25, -0.4, -0.6, -0.9
};

const double n_bunches = 111;
const double n_bunches_space = 120;

void ImportCADReadings(string cad_reading_directory, bool SD_column)
{
    double DCCT_B; // note : the average beam intensity of the BLUE beam
    double DCCT_Y; // note : the average beam intensity of the YELLOW beam
    bool BPM_StdDev = false;
    bool use_set_pos = true;

    Average_BPM_pos_V.clear();
    StdDev_BPM_pos_V.clear();
    BeamIntensity_corr_V.clear();
    Average_BPM_pos_H.clear();
    StdDev_BPM_pos_H.clear();
    BeamIntensity_corr_H.clear();

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

double Sasha_stable_approach(double *x, double *par)
{
    double ks = par[0];
    double kn = par[1];

    return 1 - TMath::Exp(-1 * x[0] * (1+ks)) - TMath::Exp(-1 * x[0] * (1+kn)) + TMath::Exp(-1 * x[0] * (1+ks+kn));

    // note : p[0] + p[1]*(x-p[3])+p[2] * (x-p[3])^2
}

double gaus_func(double *x, double *par)
{
    // note : par[0] : size
    // note : par[1] : mean
    // note : par[2] : width
    // note : par[3] : offset
    return par[0] * TMath::Gaus(x[0],par[1],par[2]) + par[3];
}

int MBDNSCount()
{
    string input_directory_in = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform_ZDCvtxZ"; 
    string input_filename_in = "ntuple_run51195_merged_raw_rate.root"; 
    string output_directory_in = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform_ZDCvtxZ/Sasha_approach";
    string CAD_reading_filename = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/CAD_reading/plots/CAD_reading_51195.csv";
    double MBDNS_abs_ecci = 0.57;

    ImportCADReadings(CAD_reading_filename, true);

    std::cout<<"BeamIntensity_corr_V.size() = "<<BeamIntensity_corr_V.size()<<std::endl;
    std::cout<<"BeamIntensity_corr_H.size() = "<<BeamIntensity_corr_H.size()<<std::endl;

    vector<pair<int,int>> range_t_V = {
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

    vector<pair<int,int>> range_t_H = {
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

    TFile * file_in = TFile::Open(Form("%s/%s", input_directory_in.c_str(), input_filename_in.c_str()));
    TTree * tree_in = (TTree *)file_in->Get("tree");

    tree_in -> SetBranchStatus("*", 0);
    tree_in -> SetBranchStatus("timestamp", 1);
    
    tree_in -> SetBranchStatus("MBDS_raw_counting_front", 1);
    tree_in -> SetBranchStatus("MBDS_raw_counting_back", 1);

    tree_in -> SetBranchStatus("MBDN_raw_counting_front", 1);
    tree_in -> SetBranchStatus("MBDN_raw_counting_back", 1);

    tree_in -> SetBranchStatus("MBDNS_raw_counting_front", 1);
    tree_in -> SetBranchStatus("MBDNS_raw_counting_back", 1);

    tree_in -> SetBranchStatus("MBDS_live_counting_front", 1);
    tree_in -> SetBranchStatus("MBDS_live_counting_back", 1);

    tree_in -> SetBranchStatus("MBDN_live_counting_front", 1);
    tree_in -> SetBranchStatus("MBDN_live_counting_back", 1);

    tree_in -> SetBranchStatus("MBDNS_live_counting_front", 1);
    tree_in -> SetBranchStatus("MBDNS_live_counting_back", 1);

    tree_in -> SetBranchStatus("GL1Scalers_range_first", 1);
    tree_in -> SetBranchStatus("GL1Scalers_range_second", 1);

    tree_in -> SetBranchStatus("GL1Scalers_live_range_first", 1);
    tree_in -> SetBranchStatus("GL1Scalers_live_range_second", 1);

    int timestamp;

    long long MBDS_raw_counting_front_in;
    long long MBDS_raw_counting_back_in;
    long long MBDN_raw_counting_front_in;
    long long MBDN_raw_counting_back_in;
    long long MBDNS_raw_counting_front_in;
    long long MBDNS_raw_counting_back_in;

    long long MBDS_live_counting_front_in;
    long long MBDS_live_counting_back_in;
    long long MBDN_live_counting_front_in;
    long long MBDN_live_counting_back_in;
    long long MBDNS_live_counting_front_in;
    long long MBDNS_live_counting_back_in;

    long long GL1Scalers_range_first_in;
    long long GL1Scalers_range_second_in;

    long long GL1Scalers_live_range_first_in;
    long long GL1Scalers_live_range_second_in;

    tree_in -> SetBranchAddress("timestamp", &timestamp);

    tree_in -> SetBranchAddress("MBDS_raw_counting_front", &MBDS_raw_counting_front_in);
    tree_in -> SetBranchAddress("MBDS_raw_counting_back", &MBDS_raw_counting_back_in);

    tree_in -> SetBranchAddress("MBDN_raw_counting_front", &MBDN_raw_counting_front_in);
    tree_in -> SetBranchAddress("MBDN_raw_counting_back", &MBDN_raw_counting_back_in);

    tree_in -> SetBranchAddress("MBDNS_raw_counting_front", &MBDNS_raw_counting_front_in);
    tree_in -> SetBranchAddress("MBDNS_raw_counting_back", &MBDNS_raw_counting_back_in);

    tree_in -> SetBranchAddress("GL1Scalers_range_first", &GL1Scalers_range_first_in);
    tree_in -> SetBranchAddress("GL1Scalers_range_second", &GL1Scalers_range_second_in);


    tree_in -> SetBranchAddress("MBDS_live_counting_front", &MBDS_live_counting_front_in);
    tree_in -> SetBranchAddress("MBDS_live_counting_back", &MBDS_live_counting_back_in);

    tree_in -> SetBranchAddress("MBDN_live_counting_front", &MBDN_live_counting_front_in);
    tree_in -> SetBranchAddress("MBDN_live_counting_back", &MBDN_live_counting_back_in);

    tree_in -> SetBranchAddress("MBDNS_live_counting_front", &MBDNS_live_counting_front_in);
    tree_in -> SetBranchAddress("MBDNS_live_counting_back", &MBDNS_live_counting_back_in);

    tree_in -> SetBranchAddress("GL1Scalers_live_range_first", &GL1Scalers_live_range_first_in);
    tree_in -> SetBranchAddress("GL1Scalers_live_range_second", &GL1Scalers_live_range_second_in);

    // tree_in -> SetBranchStatus("MBDNS_zvtx", 0);
    // tree_in -> SetBranchStatus("ZDCNS_zvtx", 0);
    // tree_in -> SetBranchStatus("LiveTrigger_Decimal", 0);

    map<int, pair<long long, long long>> time_MBDS_raw_counting;
    map<int, pair<long long, long long>> time_MBDN_raw_counting;
    map<int, pair<long long, long long>> time_MBDNS_raw_counting; // note : the MBDNS counting in every second, the second is given by the GL1 clock
    map<int, pair<long long, long long>> time_GL1Scalers_range; // note : the range of every second

    map<int, pair<long long, long long>> time_MBDS_live_counting;
    map<int, pair<long long, long long>> time_MBDN_live_counting;
    map<int, pair<long long, long long>> time_MBDNS_live_counting; // note : the MBDNS counting in every second, the second is given by the GL1 clock
    map<int, pair<long long, long long>> time_GL1Scalers_live_range; // note : the range of every second

    time_MBDS_raw_counting.clear();
    time_MBDN_raw_counting.clear();
    time_MBDNS_raw_counting.clear();
    time_GL1Scalers_range.clear();

    time_MBDS_live_counting.clear();
    time_MBDN_live_counting.clear();
    time_MBDNS_live_counting.clear();
    time_GL1Scalers_live_range.clear();


    for (int i = 0; i < tree_in->GetEntries(); i++){
        tree_in -> GetEntry(i);

        time_MBDS_raw_counting[ timestamp ] = {MBDS_raw_counting_front_in, MBDS_raw_counting_back_in};
        time_MBDN_raw_counting[ timestamp ] = {MBDN_raw_counting_front_in, MBDN_raw_counting_back_in};
        time_MBDNS_raw_counting[ timestamp ] = {MBDNS_raw_counting_front_in, MBDNS_raw_counting_back_in};

        time_GL1Scalers_range[ timestamp ] = {GL1Scalers_range_first_in, GL1Scalers_range_second_in};

        time_MBDS_live_counting[ timestamp ] = {MBDS_live_counting_front_in, MBDS_live_counting_back_in};
        time_MBDN_live_counting[ timestamp ] = {MBDN_live_counting_front_in, MBDN_live_counting_back_in};
        time_MBDNS_live_counting[ timestamp ] = {MBDNS_live_counting_front_in, MBDNS_live_counting_back_in};

        time_GL1Scalers_live_range[ timestamp ] = {GL1Scalers_live_range_first_in, GL1Scalers_live_range_second_in};
    }

    TFile * file_out = new TFile(Form("%s/MBDNSCount.root", output_directory_in.c_str()), "RECREATE");
    TTree * tree_out = new TTree("tree", "MBD counting");

    std::vector<double> MBDCount_NS_vec; MBDCount_NS_vec.clear();
    std::vector<double> MBDCount_N_vec; MBDCount_N_vec.clear();
    std::vector<double> MBDCount_S_vec; MBDCount_S_vec.clear();
    std::vector<double> BcoCount_vec; BcoCount_vec.clear();

    tree_out -> Branch("MBDCount_NS", &MBDCount_NS_vec);
    tree_out -> Branch("MBDCount_N", &MBDCount_N_vec);
    tree_out -> Branch("MBDCount_S", &MBDCount_S_vec);
    tree_out -> Branch("BcoCount", &BcoCount_vec);

    std::vector<TF1*> Sasha_stable_approach_V; Sasha_stable_approach_V.clear();
    std::vector<TF1*> Sasha_stable_approach_H; Sasha_stable_approach_H.clear();

    TCanvas * c1 = new TCanvas("","", 800, 600);
    c1 -> cd();

    TGraphErrors * MBDNS_Meas_V = new TGraphErrors(); // note : original measurement 
    MBDNS_Meas_V -> SetName("gr_MBDNS_Meas_V");
    MBDNS_Meas_V -> SetMarkerStyle(20);
    MBDNS_Meas_V -> SetMarkerColor(kRed);
    MBDNS_Meas_V -> SetMarkerSize(0.8);

    TGraph * RTrue_V = new TGraph(); // note : corrected by Sasha's equation
    RTrue_V -> SetName("gr_RTrue_V");
    RTrue_V -> SetMarkerStyle(20);
    RTrue_V -> SetMarkerColor(kBlue);
    RTrue_V -> SetMarkerSize(0.8);

    // note : Multiple collision correction
    TGraph * MBDNS_Meas_MMC_V = new TGraph(); // note : assuming a absolute trigger efficiency
    MBDNS_Meas_MMC_V -> SetName("gr_MBDNS_Meas_MMC_V");
    MBDNS_Meas_MMC_V -> SetMarkerStyle(20);
    MBDNS_Meas_MMC_V -> SetMarkerColor(kGreen);
    MBDNS_Meas_MMC_V -> SetMarkerSize(0.8);

    TGraphErrors * MBDNS_Meas_FromLive_V = new TGraphErrors(); // note : the MBDNS raw rate, derived by the live rate 
    MBDNS_Meas_FromLive_V -> SetName("gr_MBDNS_Meas_FromLive_V");
    MBDNS_Meas_FromLive_V -> SetMarkerStyle(20);
    MBDNS_Meas_FromLive_V -> SetMarkerColor(94);
    MBDNS_Meas_FromLive_V -> SetMarkerSize(0.8);



    TGraphErrors * MBDNS_Meas_H = new TGraphErrors(); 
    MBDNS_Meas_H -> SetName("gr_MBDNS_Meas_H");
    MBDNS_Meas_H -> SetMarkerStyle(20);
    MBDNS_Meas_H -> SetMarkerColor(kRed);
    MBDNS_Meas_H -> SetMarkerSize(0.8);

    TGraph * RTrue_H = new TGraph(); 
    RTrue_H -> SetName("gr_RTrue_H");
    RTrue_H -> SetMarkerStyle(20);
    RTrue_H -> SetMarkerColor(kBlue);
    RTrue_H -> SetMarkerSize(0.8);

    // note : Multiple collision correction
    TGraph * MBDNS_Meas_MMC_H = new TGraph(); 
    MBDNS_Meas_MMC_H -> SetName("gr_MBDNS_Meas_MMC_H");
    MBDNS_Meas_MMC_H -> SetMarkerStyle(20);
    MBDNS_Meas_MMC_H -> SetMarkerColor(kGreen);
    MBDNS_Meas_MMC_H -> SetMarkerSize(0.8);

    TGraphErrors * MBDNS_Meas_FromLive_H = new TGraphErrors(); // note : the MBDNS raw rate, derived by the live rate 
    MBDNS_Meas_FromLive_H -> SetName("gr_MBDNS_Meas_FromLive_H");
    MBDNS_Meas_FromLive_H -> SetMarkerStyle(20);
    MBDNS_Meas_FromLive_H -> SetMarkerColor(94);
    MBDNS_Meas_FromLive_H -> SetMarkerSize(0.8);

    

    TF1 * MBDNS_Meas_fit_V = new TF1("tf1_MBDNS_Meas_fit_V",gaus_func,-2,2,4);
    MBDNS_Meas_fit_V -> SetNpx(1000);
    MBDNS_Meas_fit_V -> SetParameters(9.32502e-02, 0, 2.99013e-01, 0);
    MBDNS_Meas_fit_V -> SetLineColor(46);
    MBDNS_Meas_fit_V -> SetLineWidth(2);

    TF1 * RTrue_fit_V = new TF1("tf1_RTrue_fit_V",gaus_func,-2,2,4);
    RTrue_fit_V -> SetNpx(1000);
    RTrue_fit_V -> SetParameters(9.32502e-02, 0, 2.99013e-01, 0);
    RTrue_fit_V -> SetLineColor(38);
    RTrue_fit_V -> SetLineWidth(2);

    TF1 * MBDNS_Meas_MMC_fit_V = new TF1("tf1_MBDNS_Meas_MMC_fit_V",gaus_func,-2,2,4);
    MBDNS_Meas_MMC_fit_V -> SetNpx(1000);
    MBDNS_Meas_MMC_fit_V -> SetParameters(9.32502e-02, 0, 2.99013e-01, 0);
    MBDNS_Meas_MMC_fit_V -> SetLineColor(31);
    MBDNS_Meas_MMC_fit_V -> SetLineWidth(2);


    TF1 * MBDNS_Meas_fit_H = new TF1("tf1_MBDNS_Meas_fit_H",gaus_func,-2,2,4);
    MBDNS_Meas_fit_H -> SetNpx(1000);
    MBDNS_Meas_fit_H -> SetParameters(9.32502e-02, 0, 2.99013e-01, 0);
    MBDNS_Meas_fit_H -> SetLineColor(46);
    MBDNS_Meas_fit_H -> SetLineWidth(2);

    TF1 * RTrue_fit_H = new TF1("tf1_RTrue_fit_H",gaus_func,-2,2,4);
    RTrue_fit_H -> SetNpx(1000);
    RTrue_fit_H -> SetParameters(9.32502e-02, 0, 2.99013e-01, 0);
    RTrue_fit_H -> SetLineColor(38);
    RTrue_fit_H -> SetLineWidth(2);

    TF1 * MBDNS_Meas_MMC_fit_H = new TF1("tf1_MBDNS_Meas_MMC_fit_H",gaus_func,-2,2,4);
    MBDNS_Meas_MMC_fit_H -> SetNpx(1000);
    MBDNS_Meas_MMC_fit_H -> SetParameters(9.32502e-02, 0, 2.99013e-01, 0);
    MBDNS_Meas_MMC_fit_H -> SetLineColor(31);
    MBDNS_Meas_MMC_fit_H -> SetLineWidth(2);


    TLegend * legend = new TLegend(0.21,0.6,0.6,0.9);
    // legend -> SetMargin(0);
    legend->SetTextSize(0.03);


    std::cout<<"=============================================================================================================="<<std::endl;

    for (int i = 0; i < range_t_V.size(); i++)
    {
        BcoCount_vec.push_back((time_GL1Scalers_range[range_t_V[i].second].second - time_GL1Scalers_range[range_t_V[i].first].first) * (n_bunches / n_bunches_space));

        MBDCount_NS_vec.push_back(time_MBDNS_raw_counting[range_t_V[i].second].second - time_MBDNS_raw_counting[range_t_V[i].first].first);
        MBDCount_N_vec.push_back(time_MBDN_raw_counting[range_t_V[i].second].second - time_MBDN_raw_counting[range_t_V[i].first].first);
        MBDCount_S_vec.push_back(time_MBDS_raw_counting[range_t_V[i].second].second - time_MBDS_raw_counting[range_t_V[i].first].first);

        std::cout<<"Vertical : "<<i<<Form(", BcoCount: %.3f, MBDCount_NS: %.3f, MBDCount_N: %.3f, MBDCount_S: %.3f, BeamIntensityCorr: %.3f", BcoCount_vec.back(), MBDCount_NS_vec.back(), MBDCount_N_vec.back(), MBDCount_S_vec.back(), BeamIntensity_corr_V[i])<<std::endl;

        MBDCount_NS_vec.back() = MBDCount_NS_vec.back() * BeamIntensity_corr_V[i];
        MBDCount_N_vec.back() = MBDCount_N_vec.back() * BeamIntensity_corr_V[i];
        MBDCount_S_vec.back() = MBDCount_S_vec.back() * BeamIntensity_corr_V[i];

        double Rmeas = MBDCount_NS_vec.back() / BcoCount_vec.back();
        double ks = (MBDCount_S_vec.back() - MBDCount_NS_vec.back()) / MBDCount_NS_vec.back(); 
        double kn = (MBDCount_N_vec.back() - MBDCount_NS_vec.back()) / MBDCount_NS_vec.back();

        Sasha_stable_approach_V.push_back(new TF1(Form("Sasha_stable_approach_V_%d", i), Sasha_stable_approach, Rmeas * 0.5, Rmeas * 2, 2));
        Sasha_stable_approach_V.back() -> SetParameters(ks, kn);
        double RTrue = Sasha_stable_approach_V.back() -> GetX(Rmeas);

        MBDNS_Meas_V -> SetPoint(MBDNS_Meas_V->GetN(), BPM_set_pos_V[i], Rmeas);
        MBDNS_Meas_V -> SetPointError(MBDNS_Meas_V->GetN()-1, 0, TMath::Sqrt(MBDCount_NS_vec.back())/BcoCount_vec.back());
        RTrue_V -> SetPoint(RTrue_V->GetN(), BPM_set_pos_V[i], RTrue);

        std::cout<<"Vertical : "<<i<<Form(", Rmeas: %.3f, ks: %.3f, kn: %.3f, RTrue: %.3f", Rmeas, ks, kn, RTrue)<<std::endl;

        // note : Multiple collision correction
        double total_crossing = BcoCount_vec.back();
        double Ntrigger = MBDCount_NS_vec.back() / MBDNS_abs_ecci;
        double the_ratio = Ntrigger / (total_crossing);
        double poisson_lambda = -1.0 * TMath::Log( 1 - the_ratio );
        double multi_collision_ratio = (1 - TMath::Poisson(0,poisson_lambda) - TMath::Poisson(1,poisson_lambda))/(1 - TMath::Poisson(0,poisson_lambda));
        double estimated_total_collision = TMath::Abs(poisson_lambda * total_crossing);
        std::cout<<"Vertical : "<<i<<Form(", total_crossing: %.3f, Ntrigger: %.3f, the_ratio: %.3f, poisson_lambda: %.3f, multi_collision_ratio: %.3f, estimated_total_collision: %.3f", total_crossing, Ntrigger, the_ratio, poisson_lambda, multi_collision_ratio, estimated_total_collision)<<std::endl;
        MBDNS_Meas_MMC_V -> SetPoint(MBDNS_Meas_MMC_V->GetN(), BPM_set_pos_V[i], TMath::Abs(poisson_lambda));

        std::cout<<std::endl;
    }    

    c1 -> cd();
    MBDNS_Meas_V -> GetYaxis() -> SetRangeUser(0, 0.25);
    MBDNS_Meas_V -> GetXaxis() -> SetTitle("Set Scan step [mm]");
    MBDNS_Meas_V -> GetYaxis() -> SetTitle("Rate [per bunch crossing]");
    MBDNS_Meas_V -> Draw("AP");
    MBDNS_Meas_V -> Fit("tf1_MBDNS_Meas_fit_V","N");
    RTrue_V -> Fit("tf1_RTrue_fit_V","N");
    MBDNS_Meas_MMC_V -> Fit("tf1_MBDNS_Meas_MMC_fit_V","N");

    std::cout<<"fit_V, Height: "<<Form("%.5f", RTrue_fit_V -> GetParameter(0))<<", fit unc. : "<< Form("%.5f", RTrue_fit_V -> GetParError(0))<<", ratio (unc./par.):"<< Form("%.5f", RTrue_fit_V -> GetParError(0) / RTrue_fit_V -> GetParameter(0))<<std::endl;
    std::cout<<"fit_V, Mean:   "<<Form("%.5f", RTrue_fit_V -> GetParameter(1))<<", fit unc. : "<< Form("%.5f", RTrue_fit_V -> GetParError(1))<<", ratio (unc./par.):"<< Form("%.5f", RTrue_fit_V -> GetParError(1) / RTrue_fit_V -> GetParameter(1))<<std::endl;
    std::cout<<"fit_V, Width:  "<<Form("%.5f", RTrue_fit_V -> GetParameter(2))<<", fit unc. : "<< Form("%.5f", RTrue_fit_V -> GetParError(2))<<", ratio (unc./par.):"<< Form("%.5f", RTrue_fit_V -> GetParError(2) / RTrue_fit_V -> GetParameter(2))<<std::endl;
    std::cout<<"fit_V, Offset: "<<Form("%.5f", RTrue_fit_V -> GetParameter(3))<<", fit unc. : "<< Form("%.5f", RTrue_fit_V -> GetParError(3))<<", ratio (unc./par.):"<< Form("%.5f", RTrue_fit_V -> GetParError(3) / RTrue_fit_V -> GetParameter(3))<<std::endl;

    legend -> AddEntry(MBDNS_Meas_V, "R_{meas}", "P");
    legend -> AddEntry(MBDNS_Meas_fit_V, Form("Gaus Fit, width: %.3f",MBDNS_Meas_fit_V->GetParameter(2)), "L");
    legend -> AddEntry(RTrue_V, "Rel. Collision rate", "P");
    legend -> AddEntry(RTrue_fit_V, Form("Gaus Fit, width: %.3f",RTrue_fit_V->GetParameter(2)), "L");
    legend -> AddEntry(MBDNS_Meas_MMC_V, "Abs. Collision rate", "P");
    legend -> AddEntry(MBDNS_Meas_MMC_fit_V, Form("Gaus Fit, width: %.3f",MBDNS_Meas_MMC_fit_V->GetParameter(2)), "L");

    MBDNS_Meas_fit_V -> Draw("lsame");
    RTrue_fit_V -> Draw("lsame");
    MBDNS_Meas_MMC_fit_V -> Draw("lsame");

    RTrue_V -> Draw("P same");
    MBDNS_Meas_V -> Draw("P same");
    MBDNS_Meas_MMC_V -> Draw("P same");

    legend -> Draw("same");

    c1 -> Write("c1_Vertical_scan");
    legend -> Clear();

    std::cout<<"=============================================================================================================="<<std::endl;

    tree_out -> Fill();
    MBDCount_NS_vec.clear();
    MBDCount_N_vec.clear();
    MBDCount_S_vec.clear();
    BcoCount_vec.clear();

    for (int i = 0; i < range_t_H.size(); i++)
    {
        BcoCount_vec.push_back((time_GL1Scalers_range[range_t_H[i].second].second - time_GL1Scalers_range[range_t_H[i].first].first) * (n_bunches / n_bunches_space));

        MBDCount_NS_vec.push_back(time_MBDNS_raw_counting[range_t_H[i].second].second - time_MBDNS_raw_counting[range_t_H[i].first].first);
        MBDCount_N_vec.push_back(time_MBDN_raw_counting[range_t_H[i].second].second - time_MBDN_raw_counting[range_t_H[i].first].first);
        MBDCount_S_vec.push_back(time_MBDS_raw_counting[range_t_H[i].second].second - time_MBDS_raw_counting[range_t_H[i].first].first);

        std::cout<<"Horizontal : "<<i<<Form(", BcoCount: %.3f, MBDCount_NS: %.3f, MBDCount_N: %.3f, MBDCount_S: %.3f, BeamIntensityCorr: %.3f", BcoCount_vec.back(), MBDCount_NS_vec.back(), MBDCount_N_vec.back(), MBDCount_S_vec.back(), BeamIntensity_corr_H[i])<<std::endl;

        MBDCount_NS_vec.back() = MBDCount_NS_vec.back() * BeamIntensity_corr_H[i];
        MBDCount_N_vec.back() = MBDCount_N_vec.back() * BeamIntensity_corr_H[i];
        MBDCount_S_vec.back() = MBDCount_S_vec.back() * BeamIntensity_corr_H[i];


        double Rmeas = MBDCount_NS_vec.back() / BcoCount_vec.back();
        double ks = (MBDCount_S_vec.back() - MBDCount_NS_vec.back()) / MBDCount_NS_vec.back();
        double kn = (MBDCount_N_vec.back() - MBDCount_NS_vec.back()) / MBDCount_NS_vec.back();

        Sasha_stable_approach_H.push_back(new TF1(Form("Sasha_stable_approach_H_%d", i), Sasha_stable_approach, Rmeas * 0.5, Rmeas * 2, 2));
        Sasha_stable_approach_H.back() -> SetParameters(ks, kn);
        double RTrue = Sasha_stable_approach_H.back() -> GetX(Rmeas);

        MBDNS_Meas_H -> SetPoint(MBDNS_Meas_H->GetN(), BPM_set_pos_H[i], Rmeas);
        MBDNS_Meas_H -> SetPointError(MBDNS_Meas_H->GetN()-1, 0, TMath::Sqrt(MBDCount_NS_vec.back())/BcoCount_vec.back());
        RTrue_H -> SetPoint(RTrue_H->GetN(), BPM_set_pos_H[i], RTrue);

        std::cout<<"Horizontal : "<<i<<Form(", Rmeas: %.3f, ks: %.3f, kn: %.3f, RTrue: %.3f", Rmeas, ks, kn, RTrue)<<std::endl;

        // note : Multiple collision correction
        double total_crossing = BcoCount_vec.back();
        double Ntrigger = MBDCount_NS_vec.back() / MBDNS_abs_ecci;
        double the_ratio = Ntrigger / (total_crossing);
        double poisson_lambda = -1.0 * TMath::Log( 1 - the_ratio );
        double multi_collision_ratio = (1 - TMath::Poisson(0,poisson_lambda) - TMath::Poisson(1,poisson_lambda))/(1 - TMath::Poisson(0,poisson_lambda));
        double estimated_total_collision = TMath::Abs(poisson_lambda * total_crossing);
        std::cout<<"Horizontal : "<<i<<Form(", total_crossing: %.3f, Ntrigger: %.3f, the_ratio: %.3f, poisson_lambda: %.3f, multi_collision_ratio: %.3f, estimated_total_collision: %.3f", total_crossing, Ntrigger, the_ratio, poisson_lambda, multi_collision_ratio, estimated_total_collision)<<std::endl;
        MBDNS_Meas_MMC_H -> SetPoint(MBDNS_Meas_MMC_H->GetN(), BPM_set_pos_H[i], TMath::Abs(poisson_lambda));

        std::cout<<std::endl;
    }    

    c1 -> cd();
    MBDNS_Meas_H -> GetYaxis() -> SetRangeUser(0, 0.25);
    MBDNS_Meas_H -> GetXaxis() -> SetTitle("Set Scan step [mm]");
    MBDNS_Meas_H -> GetYaxis() -> SetTitle("Rate [per bunch crossing]");
    MBDNS_Meas_H -> Draw("AP");
    MBDNS_Meas_H -> Fit("tf1_MBDNS_Meas_fit_H","N");
    RTrue_H -> Fit("tf1_RTrue_fit_H","N");
    MBDNS_Meas_MMC_H -> Fit("tf1_MBDNS_Meas_MMC_fit_H","N");

    std::cout<<"fit_H, Height: "<<Form("%.5f", RTrue_fit_H -> GetParameter(0))<<", fit unc. : "<< Form("%.5f", RTrue_fit_H -> GetParError(0))<< ", ratio (unc./par.):"<< Form("%.5f", RTrue_fit_H -> GetParError(0) / RTrue_fit_H -> GetParameter(0)) <<std::endl;
    std::cout<<"fit_H, Mean:   "<<Form("%.5f", RTrue_fit_H -> GetParameter(1))<<", fit unc. : "<< Form("%.5f", RTrue_fit_H -> GetParError(1))<< ", ratio (unc./par.):"<< Form("%.5f", RTrue_fit_H -> GetParError(1) / RTrue_fit_H -> GetParameter(1)) <<std::endl;
    std::cout<<"fit_H, Width:  "<<Form("%.5f", RTrue_fit_H -> GetParameter(2))<<", fit unc. : "<< Form("%.5f", RTrue_fit_H -> GetParError(2))<< ", ratio (unc./par.):"<< Form("%.5f", RTrue_fit_H -> GetParError(2) / RTrue_fit_H -> GetParameter(2)) <<std::endl;
    std::cout<<"fit_H, Offset: "<<Form("%.5f", RTrue_fit_H -> GetParameter(3))<<", fit unc. : "<< Form("%.5f", RTrue_fit_H -> GetParError(3))<< ", ratio (unc./par.):"<< Form("%.5f", RTrue_fit_H -> GetParError(3) / RTrue_fit_H -> GetParameter(3)) <<std::endl;

    legend -> AddEntry(MBDNS_Meas_H, "R_{meas}", "P");
    legend -> AddEntry(MBDNS_Meas_fit_H, Form("Gaus Fit, width: %.3f",MBDNS_Meas_fit_H->GetParameter(2)), "L");
    legend -> AddEntry(RTrue_H, "Rel. Collision rate", "P");
    legend -> AddEntry(RTrue_fit_H, Form("Gaus Fit, width: %.3f",RTrue_fit_H->GetParameter(2)), "L");
    legend -> AddEntry(MBDNS_Meas_MMC_H, "Abs. Collision rate", "P");
    legend -> AddEntry(MBDNS_Meas_MMC_fit_H, Form("Gaus Fit, width: %.3f",MBDNS_Meas_MMC_fit_H->GetParameter(2)), "L");

    MBDNS_Meas_fit_H -> Draw("lsame");
    RTrue_fit_H -> Draw("lsame");
    MBDNS_Meas_MMC_fit_H -> Draw("lsame");

    RTrue_H -> Draw("Psame");
    MBDNS_Meas_H -> Draw("Psame");
    MBDNS_Meas_MMC_H -> Draw("Psame");

    legend -> Draw("same");

    c1 -> Write("c1_Horizontal_scan");


    tree_out -> Fill();
    tree_out -> Write();

    for (int i = 0; i < Sasha_stable_approach_V.size(); i++)
    {
        Sasha_stable_approach_V[i] -> Write();
    }

    for (int i = 0; i < Sasha_stable_approach_H.size(); i++)
    {
        Sasha_stable_approach_H[i] -> Write();
    }

    MBDNS_Meas_V->Write();
    RTrue_V->Write();
    MBDNS_Meas_MMC_V->Write();
    
    MBDNS_Meas_H->Write();
    RTrue_H->Write();
    MBDNS_Meas_MMC_H->Write();

    MBDNS_Meas_fit_V->Write();
    RTrue_fit_V->Write();
    MBDNS_Meas_MMC_fit_V->Write();
    
    MBDNS_Meas_fit_H->Write();
    RTrue_fit_H->Write();
    MBDNS_Meas_MMC_fit_H->Write();

    std::cout<<"--------------- --------------- --------------- --------------- --------------- ---------------"<<std::endl;
    for (int i = 0; i < MBDNS_Meas_V->GetN(); i++)
    {
        std::cout<<"Vertical : "<<i<<Form(", BPM_set_pos_V: %.4f, (Rmeas: %.4f, error: %.4f), RTrue: %.4f, MBDNS_Meas_MMC: %.4f", MBDNS_Meas_V->GetPointX(i), MBDNS_Meas_V->GetPointY(i), MBDNS_Meas_V->GetErrorY(i), RTrue_V->GetPointY(i), MBDNS_Meas_MMC_V->GetPointY(i))<<std::endl;
    }

    std::cout<<"--------------- --------------- --------------- --------------- --------------- ---------------"<<std::endl;
    for (int i = 0; i < MBDNS_Meas_H->GetN(); i++)
    {
        std::cout<<"Horizontal : "<<i<<Form(", BPM_set_pos_H: %.4f, (Rmeas: %.4f, error: %.4f), RTrue: %.4f, MBDNS_Meas_MMC: %.4f", MBDNS_Meas_H->GetPointX(i), MBDNS_Meas_H->GetPointY(i), MBDNS_Meas_H->GetErrorY(i), RTrue_H->GetPointY(i), MBDNS_Meas_MMC_H->GetPointY(i))<<std::endl;
    }

    // std::cout<<"--------------- --------------- --------------- --------------- --------------- ---------------"<<std::endl;
    // for (int i = 0; i < MBDNS_Meas_V->GetN(); i++)
    // {
    //     std::cout<<""<<i<<Form(" & %.4f & %.4f & %.4f & %.4f \\\\", MBDNS_Meas_V->GetPointX(i), MBDNS_Meas_V->GetPointY(i), RTrue_V->GetPointY(i), MBDNS_Meas_MMC_V->GetPointY(i))<<std::endl;
    // }

    // std::cout<<"--------------- --------------- --------------- --------------- --------------- ---------------"<<std::endl;
    // for (int i = 0; i < MBDNS_Meas_H->GetN(); i++)
    // {
    //     std::cout<<""<<i<<Form(" & %.4f & %.4f & %.4f & %.4f \\\\", MBDNS_Meas_H->GetPointX(i), MBDNS_Meas_H->GetPointY(i), RTrue_H->GetPointY(i), MBDNS_Meas_MMC_H->GetPointY(i))<<std::endl;
    // }

    file_out -> Close();

    file_in -> Close();

    return 0;
}