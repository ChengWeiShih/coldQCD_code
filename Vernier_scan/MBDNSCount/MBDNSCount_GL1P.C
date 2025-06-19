struct GL1P_Bunch_info {

    long long MBDS_raw_counting_front;
    long long MBDS_raw_counting_back;
    long long MBDN_raw_counting_front;
    long long MBDN_raw_counting_back;
    long long MBDNS_raw_counting_front;
    long long MBDNS_raw_counting_back;
    long long clock_raw_first;
    long long clock_raw_second;

    long long MBDS_live_counting_front;
    long long MBDS_live_counting_back;
    long long MBDN_live_counting_front;
    long long MBDN_live_counting_back;
    long long MBDNS_live_counting_front;
    long long MBDNS_live_counting_back;
    long long clock_live_first;
    long long clock_live_second;

};

struct Step_info {
    double step;
    double step_pos;
    double Rmeasure;
    double RTrue;
    double ks;
    double kn;
};

struct EachBunchOutput {
    TCanvas * rawH_c;
    TGraph  * rawH_graph;
    TF1     * rawH_f1_gaus;

    TCanvas * rawV_c;
    TGraph  * rawV_graph;
    TF1     * rawV_f1_gaus;

    TCanvas * liveH_c;
    TGraph  * liveH_graph;
    TF1     * liveH_f1_gaus;

    TCanvas * liveV_c;
    TGraph  * liveV_graph;
    TF1     * liveV_f1_gaus;

    std::vector<std::vector<Step_info>> step_info_vec;
};

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

double TGraphGetMax(TGraph * graph_in){

    double max_y = -1e9;
    for (int i = 0; i < graph_in->GetN(); i++) {
        double x, y;
        graph_in->GetPoint(i, x, y);
        if (y > max_y) {
            max_y = y;
        }
    }
    return max_y;
};

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

TFile * file_out;
TCanvas * c1;

// note : for bunches/all  

// note : two methods, two directions

// note : In each scan, Fit, TGraph 
// note : In each step, Ks, Kn, Rmeasure, RTrue, step, step_pos
std::vector<std::vector<Step_info>> GetAllWidth(
    int bunch_number,
    std::map<std::string, std::pair<long long, long long>> MBDS_raw_counting_pair,
    std::map<std::string, std::pair<long long, long long>> MBDN_raw_counting_pair,
    std::map<std::string, std::pair<long long, long long>> MBDNS_raw_counting_pair,
    std::map<std::string, std::pair<long long, long long>> clock_raw_pair,

    std::map<std::string, std::pair<long long, long long>> MBDS_live_counting_pair,
    std::map<std::string, std::pair<long long, long long>> MBDN_live_counting_pair,
    std::map<std::string, std::pair<long long, long long>> MBDNS_live_counting_pair,
    std::map<std::string, std::pair<long long, long long>> clock_live_pair
){

    std::vector<TGraph*> graph_vec; graph_vec.clear();

    graph_vec.push_back(new TGraph()); // note: graph_raw_H;
    graph_vec.push_back(new TGraph()); // note: graph_raw_V;
    graph_vec.push_back(new TGraph()); // note: graph_live_H;
    graph_vec.push_back(new TGraph()); // note: graph_live_V;


    std::string bunch_string = (bunch_number == -999) ? "All" : std::to_string(bunch_number);
    std::string bunch_string2 = (bunch_number == -999) ? "" : "_" + std::to_string(bunch_number);

    std::vector<TF1*> fit_vec; fit_vec.clear();

    fit_vec.push_back(new TF1(Form("fit_raw_H_%s", bunch_string.c_str()), gaus_func,-2, 2, 4)); // note : fit_raw_H;
    fit_vec.push_back(new TF1(Form("fit_raw_V_%s", bunch_string.c_str()), gaus_func,-2, 2, 4)); // note : fit_raw_V;
    fit_vec.push_back(new TF1(Form("fit_live_H_%s", bunch_string.c_str()), gaus_func,-2, 2, 4)); // note : fit_live_H;
    fit_vec.push_back(new TF1(Form("fit_live_V_%s", bunch_string.c_str()), gaus_func,-2, 2, 4)); // note : fit_live_V;

    for (int i = 0; i < 4; i++) {
        fit_vec[i] -> SetNpx(1000);
        fit_vec[i] -> SetParameters(9.32502e-02, 0, 2.99013e-01, 0);
    }

    TF1 * Sasha_func = new TF1(Form("Sasha_func_%s", bunch_string.c_str()), Sasha_stable_approach, 0, 10, 2);

    std::vector<std::vector<Step_info>> all_step_vec = std::vector<std::vector<Step_info>>(4); // note : for each of the raw_H, raw_V, live_H, live_V 

    

    // note : raw_H   0
    // note : raw_V   1
    // note : live_H  2
    // note : live_V  3
    for (int i = 0; i < 4; i++) {
        
        vector<pair<int,int>> *range_t_vec = (i % 2 == 0) ? &range_t_H : &range_t_V;
        vector<double> *BPM_set_pos = (i % 2 == 0) ? &BPM_set_pos_H : &BPM_set_pos_V;
        vector<double> *BeamIntensity_corr = (i % 2 == 0) ? &BeamIntensity_corr_H : &BeamIntensity_corr_V;

        string raw_or_live = (i < 2) ? "raw" : "live";
        string H_or_V = (i % 2 == 0) ? "H" : "V";

        graph_vec[i] -> SetMarkerStyle(20);
        graph_vec[i] -> SetMarkerColor(1);
        graph_vec[i] -> SetMarkerSize(0.6);

        graph_vec[i] -> GetXaxis()->SetTitle("Set scan step [mm]");
        graph_vec[i] -> GetYaxis()->SetTitle(Form("Rate [per bunch corssing] (Bunch %s)", bunch_string.c_str()));

        for (int step_i = 0; step_i < range_t_vec->size(); step_i++) {
            string step_text_first = Form("%d%s",range_t_vec->at(step_i).first, bunch_string2.c_str());
            string step_text_second = Form("%d%s",range_t_vec->at(step_i).second, bunch_string2.c_str());

            double BcoCount_raw = clock_raw_pair[ Form("%s",step_text_second.c_str()) ].second - clock_raw_pair[ Form("%s",step_text_first.c_str()) ].first;
            double bunch_correction = (bunch_number == -999) ? (n_bunches / n_bunches_space) : 1.;
            double NTotalBunches     = (clock_raw_pair[ Form("%s",step_text_second.c_str()) ].second - clock_raw_pair[ Form("%s",step_text_first.c_str()) ].first) * bunch_correction;

            double MBDCount_NS_raw = MBDNS_raw_counting_pair[ Form("%s",step_text_second.c_str()) ].second - MBDNS_raw_counting_pair[ Form("%s",step_text_first.c_str()) ].first;
            double MBDCount_N_raw  = MBDN_raw_counting_pair[ Form("%s",step_text_second.c_str()) ].second  - MBDN_raw_counting_pair[ Form("%s",step_text_first.c_str()) ].first;
            double MBDCount_S_raw  = MBDS_raw_counting_pair[ Form("%s",step_text_second.c_str()) ].second  - MBDS_raw_counting_pair[ Form("%s",step_text_first.c_str()) ].first;

            
            double BcoCount_live = clock_live_pair[ Form("%s",step_text_second.c_str()) ].second - clock_live_pair[ Form("%s",step_text_first.c_str()) ].first;

            double MBDCount_NS_live = MBDNS_live_counting_pair[ Form("%s",step_text_second.c_str()) ].second - MBDNS_live_counting_pair[ Form("%s",step_text_first.c_str()) ].first;
            double MBDCount_N_live  = MBDN_live_counting_pair[ Form("%s",step_text_second.c_str()) ].second  - MBDN_live_counting_pair[ Form("%s",step_text_first.c_str()) ].first;
            double MBDCount_S_live  = MBDS_live_counting_pair[ Form("%s",step_text_second.c_str()) ].second  - MBDS_live_counting_pair[ Form("%s",step_text_first.c_str()) ].first;

            std::cout<<Form("Bunch number = %d, i=%d, raw_%s", bunch_number, i, H_or_V.c_str())<<", "<<
                Form("step = %d, step_pos = %.3f, BcoCount = %.3f, MBDCount_NS = %.3f, MBDCount_N = %.3f, MBDCount_S = %.3f, BeamIntensity_corr = %.3f",
                    step_i, BPM_set_pos->at(step_i), BcoCount_raw, MBDCount_NS_raw, MBDCount_N_raw, MBDCount_S_raw, BeamIntensity_corr->at(step_i))<<std::endl;

            std::cout<<Form("i=%d, live_%s", i, H_or_V.c_str())<<", "<<
                Form("step = %d, step_pos = %.3f, BcoCount = %.3f, MBDCount_NS = %.3f, MBDCount_N = %.3f, MBDCount_S = %.3f",
                    step_i, BPM_set_pos->at(step_i), BcoCount_live, MBDCount_NS_live, MBDCount_N_live, MBDCount_S_live)<<std::endl;
            

            MBDCount_NS_raw = MBDCount_NS_raw * BeamIntensity_corr->at(step_i); 
            MBDCount_N_raw = MBDCount_N_raw * BeamIntensity_corr->at(step_i); 
            MBDCount_S_raw = MBDCount_S_raw * BeamIntensity_corr->at(step_i); 

            MBDCount_NS_live = MBDCount_NS_live * BeamIntensity_corr->at(step_i);
            MBDCount_N_live = MBDCount_N_live * BeamIntensity_corr->at(step_i);
            MBDCount_S_live = MBDCount_S_live * BeamIntensity_corr->at(step_i);

            double this_MBDCount_NS = (i < 2) ? MBDCount_NS_raw : MBDCount_NS_live / (BcoCount_live / BcoCount_raw);
            double this_MBDCount_N = (i < 2) ? MBDCount_N_raw : MBDCount_N_live / (BcoCount_live / BcoCount_raw);
            double this_MBDCount_S = (i < 2) ? MBDCount_S_raw : MBDCount_S_live / (BcoCount_live / BcoCount_raw);

            double Rmeas = this_MBDCount_NS / NTotalBunches;
            double ks = (this_MBDCount_S - this_MBDCount_NS) / this_MBDCount_NS; 
            double kn = (this_MBDCount_N - this_MBDCount_NS) / this_MBDCount_NS;

            Sasha_func -> SetParameters(ks, kn);
            double RTrue = Sasha_func -> GetX(Rmeas);

            std::cout<<Form("this_MBDCount_NS = %.3f, NTotalBunches = %.3f",this_MBDCount_NS, NTotalBunches)<<std::endl;

            std::cout<<Form("%s_%s", raw_or_live.c_str(), H_or_V.c_str())<<", post_BeamIntensityCorr, "<<
                Form("step = %d, step_pos = %.3f, MBDCount_NS = %.3f, MBDCount_N = %.3f, MBDCount_S = %.3f, ks = %.3f, kn = %.3f, Rmeas = %.3f, RTrue = %.3f",
                    step_i, BPM_set_pos->at(step_i), this_MBDCount_NS, this_MBDCount_N, this_MBDCount_S, ks, kn, Rmeas, RTrue)<<std::endl;
            

            std::cout<<std::endl;
            
            // todo: combine two points at the maximal-overlap step
            if (step_i == 6) {
                double First_max_Y = graph_vec[i] -> GetPointY(0);
                graph_vec[i] -> SetPoint( 0, BPM_set_pos->at(step_i), (RTrue + First_max_Y) / 2.);
            }
            else {
                graph_vec[i] -> SetPoint( graph_vec[i] -> GetN(), BPM_set_pos->at(step_i), RTrue);
            } 


            Step_info this_step;
            this_step.step = step_i;
            this_step.step_pos = BPM_set_pos->at(step_i);
            this_step.ks = ks;
            this_step.kn = kn;
            this_step.Rmeasure = Rmeas;
            this_step.RTrue = RTrue;

            all_step_vec[i].push_back(this_step);

        }

        graph_vec[i] -> GetXaxis() -> SetLimits(-1.3, 1.3);
        graph_vec[i] -> GetYaxis() -> SetRangeUser(0, TGraphGetMax(graph_vec[i]) * 1.5);
        graph_vec[i] -> Fit(fit_vec[i], "N");

        c1 -> cd();
        graph_vec[i] -> Draw("ap");
        fit_vec[i] -> SetLineColor(2);
        fit_vec[i] -> Draw("same");
        
        file_out -> cd();
        c1 ->           Write(Form("Bunch%s_%s%s_c1", bunch_string.c_str(), raw_or_live.c_str(), H_or_V.c_str()));
        graph_vec[i] -> Write(Form("Bunch%s_%s%s_graph", bunch_string.c_str(), raw_or_live.c_str(), H_or_V.c_str()));
        fit_vec[i] ->   Write(Form("Bunch%s_%s%s_fit", bunch_string.c_str(), raw_or_live.c_str(), H_or_V.c_str()));
        c1 -> Clear();
    }

    return all_step_vec;
}


int MBDNSCount_GL1P(){

    string input_directory_in = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform_GL1P"; 
    string input_filename_in = "ntuple_run51195_merged_rawlive_rate.root"; 
    string output_directory_in = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform_GL1P/Sasha_approach_GL1P";
    string CAD_reading_filename = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/CAD_reading/plots/CAD_reading_51195.csv";

    ImportCADReadings(CAD_reading_filename, true);

    std::cout<<"BeamIntensity_corr_V.size() = "<<BeamIntensity_corr_V.size()<<std::endl;
    std::cout<<"BeamIntensity_corr_H.size() = "<<BeamIntensity_corr_H.size()<<std::endl;


    std::map<int, GL1P_Bunch_info> GL1P_info_map; GL1P_info_map.clear();

    TFile * file_in = TFile::Open(Form("%s/%s", input_directory_in.c_str(), input_filename_in.c_str()));
    TTree * tree_in = (TTree *)file_in->Get("tree");

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

    GL1P_Bunch_info empty_info;
    empty_info = {
        -999, -999, -999, -999, -999, -999, -999, -999,
        -999, -999, -999, -999, -999, -999, -999, -999
    };

    for (int i = 0; i < 120; i++) {

        GL1P_info_map.insert(
            std::make_pair(
                i,
                empty_info
            )
        );

        tree_in -> SetBranchAddress(Form("GL1P_MBDS_raw_counting_front_%d", i), &(GL1P_info_map[i].MBDS_raw_counting_front));
        tree_in -> SetBranchAddress(Form("GL1P_MBDS_raw_counting_back_%d", i), &(GL1P_info_map[i].MBDS_raw_counting_back));
        tree_in -> SetBranchAddress(Form("GL1P_MBDN_raw_counting_front_%d", i), &(GL1P_info_map[i].MBDN_raw_counting_front));
        tree_in -> SetBranchAddress(Form("GL1P_MBDN_raw_counting_back_%d", i), &(GL1P_info_map[i].MBDN_raw_counting_back));
        tree_in -> SetBranchAddress(Form("GL1P_MBDNS_raw_counting_front_%d", i), &(GL1P_info_map[i].MBDNS_raw_counting_front));
        tree_in -> SetBranchAddress(Form("GL1P_MBDNS_raw_counting_back_%d", i), &(GL1P_info_map[i].MBDNS_raw_counting_back));
        tree_in -> SetBranchAddress(Form("GL1P_clock_raw_first_%d", i), &(GL1P_info_map[i].clock_raw_first));
        tree_in -> SetBranchAddress(Form("GL1P_clock_raw_second_%d", i), &(GL1P_info_map[i].clock_raw_second));

        tree_in -> SetBranchAddress(Form("GL1P_MBDS_live_counting_front_%d", i), &(GL1P_info_map[i].MBDS_live_counting_front));
        tree_in -> SetBranchAddress(Form("GL1P_MBDS_live_counting_back_%d", i), &(GL1P_info_map[i].MBDS_live_counting_back));
        tree_in -> SetBranchAddress(Form("GL1P_MBDN_live_counting_front_%d", i), &(GL1P_info_map[i].MBDN_live_counting_front));
        tree_in -> SetBranchAddress(Form("GL1P_MBDN_live_counting_back_%d", i), &(GL1P_info_map[i].MBDN_live_counting_back));
        tree_in -> SetBranchAddress(Form("GL1P_MBDNS_live_counting_front_%d", i), &(GL1P_info_map[i].MBDNS_live_counting_front));
        tree_in -> SetBranchAddress(Form("GL1P_MBDNS_live_counting_back_%d", i), &(GL1P_info_map[i].MBDNS_live_counting_back));
        tree_in -> SetBranchAddress(Form("GL1P_clock_live_first_%d", i), &(GL1P_info_map[i].clock_live_first));
        tree_in -> SetBranchAddress(Form("GL1P_clock_live_second_%d", i), &(GL1P_info_map[i].clock_live_second));
    }


    map<std::string, pair<long long, long long>> time_MBDS_raw_counting;
    map<std::string, pair<long long, long long>> time_MBDN_raw_counting;
    map<std::string, pair<long long, long long>> time_MBDNS_raw_counting; // note : the MBDNS counting in every second, the second is given by the GL1 clock
    map<std::string, pair<long long, long long>> time_GL1Scalers_range; // note : the range of every second

    map<std::string, pair<long long, long long>> time_MBDS_live_counting;
    map<std::string, pair<long long, long long>> time_MBDN_live_counting;
    map<std::string, pair<long long, long long>> time_MBDNS_live_counting; // note : the MBDNS counting in every second, the second is given by the GL1 clock
    map<std::string, pair<long long, long long>> time_GL1Scalers_live_range; // note : the range of every second

    time_MBDS_raw_counting.clear();
    time_MBDN_raw_counting.clear();
    time_MBDNS_raw_counting.clear();
    time_GL1Scalers_range.clear();

    time_MBDS_live_counting.clear();
    time_MBDN_live_counting.clear();
    time_MBDNS_live_counting.clear();
    time_GL1Scalers_live_range.clear();


    std::map<std::string, pair<long long, long long>> GL1P_time_MBDS_raw_counting;
    std::map<std::string, pair<long long, long long>> GL1P_time_MBDN_raw_counting;
    std::map<std::string, pair<long long, long long>> GL1P_time_MBDNS_raw_counting; // note : the MBDNS counting in every second, the second is given by the GL1 clock
    std::map<std::string, pair<long long, long long>> GL1P_time_GL1Scalers_range; // note : the range of every second

    std::map<std::string, pair<long long, long long>> GL1P_time_MBDS_live_counting;
    std::map<std::string, pair<long long, long long>> GL1P_time_MBDN_live_counting;
    std::map<std::string, pair<long long, long long>> GL1P_time_MBDNS_live_counting; // note : the MBDNS counting in every second, the second is given by the GL1 clock
    std::map<std::string, pair<long long, long long>> GL1P_time_GL1Scalers_live_range; // note : the range of every second

    GL1P_time_MBDS_raw_counting.clear();
    GL1P_time_MBDN_raw_counting.clear();
    GL1P_time_MBDNS_raw_counting.clear();
    GL1P_time_GL1Scalers_range.clear();

    GL1P_time_MBDS_live_counting.clear();
    GL1P_time_MBDN_live_counting.clear();
    GL1P_time_MBDNS_live_counting.clear();
    GL1P_time_GL1Scalers_live_range.clear();


    for (int i = 0; i < tree_in->GetEntries(); i++){
        tree_in -> GetEntry(i);

        time_MBDS_raw_counting[ Form("%d", timestamp) ] = {MBDS_raw_counting_front_in, MBDS_raw_counting_back_in};
        time_MBDN_raw_counting[ Form("%d", timestamp) ] = {MBDN_raw_counting_front_in, MBDN_raw_counting_back_in};
        time_MBDNS_raw_counting[ Form("%d", timestamp) ] = {MBDNS_raw_counting_front_in, MBDNS_raw_counting_back_in};

        time_GL1Scalers_range[ Form("%d", timestamp) ] = {GL1Scalers_range_first_in, GL1Scalers_range_second_in};

        time_MBDS_live_counting[ Form("%d", timestamp) ] = {MBDS_live_counting_front_in, MBDS_live_counting_back_in};
        time_MBDN_live_counting[ Form("%d", timestamp) ] = {MBDN_live_counting_front_in, MBDN_live_counting_back_in};
        time_MBDNS_live_counting[ Form("%d", timestamp) ] = {MBDNS_live_counting_front_in, MBDNS_live_counting_back_in};

        time_GL1Scalers_live_range[ Form("%d", timestamp) ] = {GL1Scalers_live_range_first_in, GL1Scalers_live_range_second_in};


        for (int j = 0; j < 120; j++) {
            GL1P_Bunch_info info = GL1P_info_map[j];

            GL1P_time_MBDS_raw_counting[ Form("%d_%d", timestamp, j) ] = {info.MBDS_raw_counting_front, info.MBDS_raw_counting_back};
            GL1P_time_MBDN_raw_counting[ Form("%d_%d", timestamp, j) ] = {info.MBDN_raw_counting_front, info.MBDN_raw_counting_back};
            GL1P_time_MBDNS_raw_counting[ Form("%d_%d", timestamp, j) ] = {info.MBDNS_raw_counting_front, info.MBDNS_raw_counting_back};

            GL1P_time_GL1Scalers_range[ Form("%d_%d", timestamp, j) ] = {info.clock_raw_first, info.clock_raw_second};
            

            GL1P_time_MBDS_live_counting[ Form("%d_%d", timestamp, j) ] = {info.MBDS_live_counting_front, info.MBDS_live_counting_back};
            GL1P_time_MBDN_live_counting[ Form("%d_%d", timestamp, j) ] = {info.MBDN_live_counting_front, info.MBDN_live_counting_back};
            GL1P_time_MBDNS_live_counting[ Form("%d_%d", timestamp, j) ] = {info.MBDNS_live_counting_front, info.MBDNS_live_counting_back};

            GL1P_time_GL1Scalers_live_range[ Form("%d_%d", timestamp, j) ] = {info.clock_live_first, info.clock_live_second};
        }
    }

    file_out = new TFile(Form("%s/MBDNSCount_GL1P_output.root", output_directory_in.c_str()), "RECREATE");
    TTree * tree_out = new TTree("tree","tree");

    int out_bunch_number;
    int out_is_raw;
    int out_is_H;
    double out_step;
    double out_step_pos;
    double out_Rmeasure;
    double out_RTrue;
    double out_Ks;
    double out_Kn;

    tree_out -> Branch("bunch_number", &out_bunch_number);
    tree_out -> Branch("is_raw", &out_is_raw);
    tree_out -> Branch("is_H", &out_is_H);

    tree_out -> Branch("step", &out_step);
    tree_out -> Branch("step_pos", &out_step_pos);
    tree_out -> Branch("Rmeasure", &out_Rmeasure);
    tree_out -> Branch("RTrue", &out_RTrue);
    tree_out -> Branch("Ks", &out_Ks);
    tree_out -> Branch("Kn", &out_Kn);

    c1 = new TCanvas("c1", "c1", 800, 600);


    std::vector<std::vector<Step_info>> Inclusive_step_info_vec = GetAllWidth(
        -999,

        time_MBDS_raw_counting,
        time_MBDN_raw_counting,
        time_MBDNS_raw_counting,        
        time_GL1Scalers_range,

        time_MBDS_live_counting,
        time_MBDN_live_counting,
        time_MBDNS_live_counting,        
        time_GL1Scalers_live_range
    );

    for (int i = 0; i < Inclusive_step_info_vec.size(); i++){
        for (int step_i = 0; step_i < Inclusive_step_info_vec[i].size(); step_i++){

            Step_info this_step = Inclusive_step_info_vec[i][step_i];

            out_bunch_number = -999;
            out_is_raw = (i < 2) ? 1 : 0; // note : 1 for raw, 0 for live
            out_is_H = (i % 2 == 0) ? 1 : 0; // note : 1 for H, 0 for V

            out_step = this_step.step;
            out_step_pos = this_step.step_pos;
            out_Rmeasure = this_step.Rmeasure;
            out_RTrue = this_step.RTrue;
            out_Ks = this_step.ks;
            out_Kn = this_step.kn;

            tree_out -> Fill();
        }
    }

    for (int bunch_number = 0; bunch_number < n_bunches; bunch_number++) { // note : 0 to 110
        
        std::vector<std::vector<Step_info>> step_info_vec = GetAllWidth(
            bunch_number,

            GL1P_time_MBDS_raw_counting,
            GL1P_time_MBDN_raw_counting,
            GL1P_time_MBDNS_raw_counting,        
            GL1P_time_GL1Scalers_range,

            GL1P_time_MBDS_live_counting,
            GL1P_time_MBDN_live_counting,
            GL1P_time_MBDNS_live_counting,        
            GL1P_time_GL1Scalers_live_range
        );


        for (int i = 0; i < step_info_vec.size(); i++){
            for (int step_i = 0; step_i < step_info_vec[i].size(); step_i++){

                Step_info this_step = step_info_vec[i][step_i];

                out_bunch_number = bunch_number;
                out_is_raw = (i < 2) ? 1 : 0; // note : 1 for raw, 0 for live
                out_is_H = (i % 2 == 0) ? 1 : 0; // note : 1 for H, 0 for V

                out_step = this_step.step;
                out_step_pos = this_step.step_pos;
                out_Rmeasure = this_step.Rmeasure;
                out_RTrue = this_step.RTrue;
                out_Ks = this_step.ks;
                out_Kn = this_step.kn;

                tree_out -> Fill();
            }
        }

    }

    file_out -> cd();
    tree_out -> Write();
    file_out -> Close();

    return 888;
}

