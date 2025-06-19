
#include "GetOverlapWidth.h"

GetOverlapWidth::GetOverlapWidth() {
    Average_BPM_pos_V.clear();
    StdDev_BPM_pos_V.clear();
    BeamIntensity_corr_V.clear();

    Average_BPM_pos_H.clear();
    StdDev_BPM_pos_H.clear();
    BeamIntensity_corr_H.clear();
}

void GetOverlapWidth::ImportCADReadings(string cad_reading_directory, bool SD_column)
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

// note : for bunches/all        
// note : two methods, two directions, bunches/all -> each: one TGraph, one TF1
// note : in each method 

void GetOverlapWidth::GetAllWidth(
    int bunch_number,
    std::pair<double, double> MBDS_raw_counting_pair,
    std::pair<double, double> MBDN_raw_counting_pair,
    std::pair<double, double> MBDNS_raw_counting_pair,
    std::pair<double, double> clock_raw_pair,

    std::pair<double, double> MBDS_live_counting_pair,
    std::pair<double, double> MBDN_live_counting_pair,
    std::pair<double, double> MBDNS_live_counting_pair,
    std::pair<double, double> clock_live_pair
) {

    TGraph * graph_raw_H = new TGraph();
    TGraph * graph_raw_V = new TGraph();
    TGraph * graph_live_H = new TGraph();
    TGraph * graph_live_V = new TGraph();


    std::string bunch_string = (bunch_number == -999) ? "All" : std::to_string(bunch_number);

    TF1 * fit_raw_H = new TF1(Form("fit_raw_H_%s", bunch_string.c_str()), gaus_func,-2, 2, 4);
    TF1 * fit_raw_V = new TF1(Form("fit_raw_V_%s", bunch_string.c_str()), gaus_func,-2, 2, 4);
    TF1 * fit_live_H = new TF1(Form("fit_live_H_%s", bunch_string.c_str()), gaus_func,-2, 2, 4);
    TF1 * fit_live_V = new TF1(Form("fit_live_V_%s", bunch_string.c_str()), gaus_func,-2, 2, 4);
    


}