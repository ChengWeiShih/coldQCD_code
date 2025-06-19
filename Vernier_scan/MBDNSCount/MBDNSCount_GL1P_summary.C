double  vector_average (vector <double> input_vector) {
	return accumulate( input_vector.begin(), input_vector.end(), 0.0 ) / double(input_vector.size());
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

double gaus_func(double *x, double *par)
{
    // note : par[0] : size
    // note : par[1] : mean
    // note : par[2] : width
    // note : par[3] : offset
    return par[0] * TMath::Gaus(x[0],par[1],par[2]) + par[3];
}

int MBDNSCount_GL1P_summary(){

    string input_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform_GL1P/Sasha_approach_GL1P";
    string input_filename = "MBDNSCount_GL1P_output.root";
    string output_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform_GL1P/Sasha_approach_GL1P";

    TCanvas * c1 = new TCanvas("c1", "c1", 800, 600);

    TFile * file_in = TFile::Open(Form("%s/%s", input_directory.c_str(), input_filename.c_str()));
    TTree * tree_in = (TTree*)file_in->Get("tree");

    int bunch_number;
    int is_raw;
    int is_H;
    double step;
    double step_pos;
    double Rmeasure;
    double RTrue;
    double Ks;
    double Kn;

    tree_in -> SetBranchAddress("bunch_number", &bunch_number);
    tree_in -> SetBranchAddress("is_raw", &is_raw);
    tree_in -> SetBranchAddress("is_H", &is_H);
    tree_in -> SetBranchAddress("step", &step);
    tree_in -> SetBranchAddress("step_pos", &step_pos);
    tree_in -> SetBranchAddress("Rmeasure", &Rmeasure);
    tree_in -> SetBranchAddress("RTrue", &RTrue);
    tree_in -> SetBranchAddress("Ks", &Ks);
    tree_in -> SetBranchAddress("Kn", &Kn);

    TGraph * gr_RawMethod_EachBunch_Rmeasure_Ks = new TGraph(); // note : inclusive to steps and V_H_direction
    TGraph * gr_RawMethod_EachBunch_Rmeasure_Kn = new TGraph(); // note : inclusive to steps and V_H_direction

    TGraph * gr_RawMethod_EachBunch_RTrue_Ks = new TGraph(); // note : inclusive to steps and V_H_direction
    TGraph * gr_RawMethod_EachBunch_RTrue_Kn = new TGraph(); // note : inclusive to steps and V_H_direction

    TGraph * gr_RawMethod_BunchNumber_MaxRmeasure_H = new TGraph();
    TGraph * gr_RawMethod_BunchNumber_MaxRTrue_H = new TGraph();

    TGraph * gr_RawMethod_BunchNumber_MaxRmeasure_V = new TGraph();
    TGraph * gr_RawMethod_BunchNumber_MaxRTrue_V = new TGraph();

    TGraph * gr_RawMethod_BunchNumber_WidthH = new TGraph();
    TGraph * gr_RawMethod_BunchNumber_WidthV = new TGraph();

    TH1D * h1D_RawMethod_WidthH = new TH1D("h1D_RawMethod_WidthH", "h1D_RawMethod_WidthH;Width [mm] (Horizontal);Entries", 50, 0.15, 0.35);
    TH1D * h1D_RawMethod_WidthV = new TH1D("h1D_RawMethod_WidthV", "h1D_RawMethod_WidthV;Width [mm] (Vertical);Entries", 50, 0.15, 0.35);


    TGraph * gr_LiveMethod_EachBunch_Rmeasure_Ks = new TGraph(); // note : inclusive to steps and V_H_direction
    TGraph * gr_LiveMethod_EachBunch_Rmeasure_Kn = new TGraph(); // note : inclusive to steps and V_H_direction

    TGraph * gr_LiveMethod_EachBunch_RTrue_Ks = new TGraph(); // note : inclusive to steps and V_H_direction
    TGraph * gr_LiveMethod_EachBunch_RTrue_Kn = new TGraph(); // note : inclusive to steps and V_H_direction

    TGraph * gr_LiveMethod_BunchNumber_MaxRmeasure_H = new TGraph();
    TGraph * gr_LiveMethod_BunchNumber_MaxRTrue_H = new TGraph();

    TGraph * gr_LiveMethod_BunchNumber_MaxRmeasure_V = new TGraph();
    TGraph * gr_LiveMethod_BunchNumber_MaxRTrue_V = new TGraph();

    TGraph * gr_LiveMethod_BunchNumber_WidthH = new TGraph();
    TGraph * gr_LiveMethod_BunchNumber_WidthV = new TGraph();

    TH1D * h1D_LiveMethod_WidthH = new TH1D("h1D_LiveMethod_WidthH", "h1D_LiveMethod_WidthH;Width [mm] (Horizontal);Entries", 50, 0.15, 0.35);
    TH1D * h1D_LiveMethod_WidthV = new TH1D("h1D_LiveMethod_WidthV", "h1D_LiveMethod_WidthV;Width [mm] (Vertical);Entries", 50, 0.15, 0.35);

    gr_RawMethod_EachBunch_Rmeasure_Ks -> SetMarkerStyle(20);
    gr_RawMethod_EachBunch_Rmeasure_Ks -> SetMarkerColor(1);
    gr_RawMethod_EachBunch_Rmeasure_Ks -> SetMarkerSize(0.6);
    gr_RawMethod_EachBunch_Rmeasure_Ks -> SetLineWidth(0);
    gr_RawMethod_EachBunch_Rmeasure_Ks -> SetTitle(";Rmeasure [per bunch crossing];Ks");

    gr_RawMethod_EachBunch_Rmeasure_Kn -> SetMarkerStyle(20);
    gr_RawMethod_EachBunch_Rmeasure_Kn -> SetMarkerColor(1);
    gr_RawMethod_EachBunch_Rmeasure_Kn -> SetMarkerSize(0.6);
    gr_RawMethod_EachBunch_Rmeasure_Kn -> SetLineWidth(0);
    gr_RawMethod_EachBunch_Rmeasure_Kn -> SetTitle(";Rmeasure [per bunch crossing];Kn");

    gr_RawMethod_EachBunch_RTrue_Ks -> SetMarkerStyle(20);
    gr_RawMethod_EachBunch_RTrue_Ks -> SetMarkerColor(1);
    gr_RawMethod_EachBunch_RTrue_Ks -> SetMarkerSize(0.6);
    gr_RawMethod_EachBunch_RTrue_Ks -> SetLineWidth(0);
    gr_RawMethod_EachBunch_RTrue_Ks -> SetTitle(";RTrue [per bunch crossing];Ks");

    gr_RawMethod_EachBunch_RTrue_Kn -> SetMarkerStyle(20);
    gr_RawMethod_EachBunch_RTrue_Kn -> SetMarkerColor(1);
    gr_RawMethod_EachBunch_RTrue_Kn -> SetMarkerSize(0.6);
    gr_RawMethod_EachBunch_RTrue_Kn -> SetLineWidth(0);
    gr_RawMethod_EachBunch_RTrue_Kn -> SetTitle(";RTrue [per bunch crossing];Kn");

    gr_RawMethod_BunchNumber_MaxRmeasure_H -> SetMarkerStyle(20);
    gr_RawMethod_BunchNumber_MaxRmeasure_H -> SetMarkerColor(1);
    gr_RawMethod_BunchNumber_MaxRmeasure_H -> SetMarkerSize(0.6);
    gr_RawMethod_BunchNumber_MaxRmeasure_H -> SetLineWidth(0);
    gr_RawMethod_BunchNumber_MaxRmeasure_H -> SetTitle(";Bunch number;(Overlap steps in H) Rmeasure [per bunch crossing]");

    gr_RawMethod_BunchNumber_MaxRTrue_H -> SetMarkerStyle(20);
    gr_RawMethod_BunchNumber_MaxRTrue_H -> SetMarkerColor(1);
    gr_RawMethod_BunchNumber_MaxRTrue_H -> SetMarkerSize(0.6);
    gr_RawMethod_BunchNumber_MaxRTrue_H -> SetLineWidth(0);
    gr_RawMethod_BunchNumber_MaxRTrue_H -> SetTitle(";Bunch number;(Overlap steps in H) RTrue [per bunch crossing]");

    gr_RawMethod_BunchNumber_MaxRmeasure_V -> SetMarkerStyle(20);
    gr_RawMethod_BunchNumber_MaxRmeasure_V -> SetMarkerColor(1);
    gr_RawMethod_BunchNumber_MaxRmeasure_V -> SetMarkerSize(0.6);
    gr_RawMethod_BunchNumber_MaxRmeasure_V -> SetLineWidth(0);
    gr_RawMethod_BunchNumber_MaxRmeasure_V -> SetTitle(";Bunch number;(Overlap steps in V) Rmeasure [per bunch crossing]");

    gr_RawMethod_BunchNumber_MaxRTrue_V -> SetMarkerStyle(20);
    gr_RawMethod_BunchNumber_MaxRTrue_V -> SetMarkerColor(1);
    gr_RawMethod_BunchNumber_MaxRTrue_V -> SetMarkerSize(0.6);
    gr_RawMethod_BunchNumber_MaxRTrue_V -> SetLineWidth(0);
    gr_RawMethod_BunchNumber_MaxRTrue_V -> SetTitle(";Bunch number;(Overlap steps in V) RTrue [per bunch crossing]");

    gr_RawMethod_BunchNumber_WidthV -> SetMarkerStyle(20);
    gr_RawMethod_BunchNumber_WidthV -> SetMarkerColor(1);
    gr_RawMethod_BunchNumber_WidthV -> SetMarkerSize(0.6);
    gr_RawMethod_BunchNumber_WidthV -> SetLineWidth(0);
    gr_RawMethod_BunchNumber_WidthV -> SetTitle(";Bunch number;(in V) Overlapped Width [mm]");

    gr_RawMethod_BunchNumber_WidthH -> SetMarkerStyle(20);
    gr_RawMethod_BunchNumber_WidthH -> SetMarkerColor(1);
    gr_RawMethod_BunchNumber_WidthH -> SetMarkerSize(0.6);
    gr_RawMethod_BunchNumber_WidthH -> SetLineWidth(0);
    gr_RawMethod_BunchNumber_WidthH -> SetTitle(";Bunch number;(in H) Overlapped Width [mm]");

    gr_LiveMethod_EachBunch_Rmeasure_Ks -> SetMarkerStyle(20);
    gr_LiveMethod_EachBunch_Rmeasure_Ks -> SetMarkerColor(1);
    gr_LiveMethod_EachBunch_Rmeasure_Ks -> SetMarkerSize(0.6);
    gr_LiveMethod_EachBunch_Rmeasure_Ks -> SetLineWidth(0);
    gr_LiveMethod_EachBunch_Rmeasure_Ks -> SetTitle(";Rmeasure [per bunch crossing];Ks");

    gr_LiveMethod_EachBunch_Rmeasure_Kn -> SetMarkerStyle(20);
    gr_LiveMethod_EachBunch_Rmeasure_Kn -> SetMarkerColor(1);
    gr_LiveMethod_EachBunch_Rmeasure_Kn -> SetMarkerSize(0.6);
    gr_LiveMethod_EachBunch_Rmeasure_Kn -> SetLineWidth(0);
    gr_LiveMethod_EachBunch_Rmeasure_Kn -> SetTitle(";Rmeasure [per bunch crossing];Kn");

    gr_LiveMethod_EachBunch_RTrue_Ks -> SetMarkerStyle(20);
    gr_LiveMethod_EachBunch_RTrue_Ks -> SetMarkerColor(1);
    gr_LiveMethod_EachBunch_RTrue_Ks -> SetMarkerSize(0.6);
    gr_LiveMethod_EachBunch_RTrue_Ks -> SetLineWidth(0);
    gr_LiveMethod_EachBunch_RTrue_Ks -> SetTitle(";RTrue [per bunch crossing];Ks");

    gr_LiveMethod_EachBunch_RTrue_Kn -> SetMarkerStyle(20);
    gr_LiveMethod_EachBunch_RTrue_Kn -> SetMarkerColor(1);
    gr_LiveMethod_EachBunch_RTrue_Kn -> SetMarkerSize(0.6);
    gr_LiveMethod_EachBunch_RTrue_Kn -> SetLineWidth(0);
    gr_LiveMethod_EachBunch_RTrue_Kn -> SetTitle(";RTrue [per bunch crossing];Kn");

    gr_LiveMethod_BunchNumber_MaxRmeasure_H -> SetMarkerStyle(20);
    gr_LiveMethod_BunchNumber_MaxRmeasure_H -> SetMarkerColor(1);
    gr_LiveMethod_BunchNumber_MaxRmeasure_H -> SetMarkerSize(0.6);
    gr_LiveMethod_BunchNumber_MaxRmeasure_H -> SetLineWidth(0);
    gr_LiveMethod_BunchNumber_MaxRmeasure_H -> SetTitle(";Bunch number;(Overlap steps in H) Rmeasure [per bunch crossing]");

    gr_LiveMethod_BunchNumber_MaxRTrue_H -> SetMarkerStyle(20);
    gr_LiveMethod_BunchNumber_MaxRTrue_H -> SetMarkerColor(1);
    gr_LiveMethod_BunchNumber_MaxRTrue_H -> SetMarkerSize(0.6);
    gr_LiveMethod_BunchNumber_MaxRTrue_H -> SetLineWidth(0);
    gr_LiveMethod_BunchNumber_MaxRTrue_H -> SetTitle(";Bunch number;(Overlap steps in H) RTrue [per bunch crossing]");

    gr_LiveMethod_BunchNumber_MaxRmeasure_V -> SetMarkerStyle(20);
    gr_LiveMethod_BunchNumber_MaxRmeasure_V -> SetMarkerColor(1);
    gr_LiveMethod_BunchNumber_MaxRmeasure_V -> SetMarkerSize(0.6);
    gr_LiveMethod_BunchNumber_MaxRmeasure_V -> SetLineWidth(0);
    gr_LiveMethod_BunchNumber_MaxRmeasure_V -> SetTitle(";Bunch number;(Overlap steps in V) Rmeasure [per bunch crossing]");

    gr_LiveMethod_BunchNumber_MaxRTrue_V -> SetMarkerStyle(20);
    gr_LiveMethod_BunchNumber_MaxRTrue_V -> SetMarkerColor(1);
    gr_LiveMethod_BunchNumber_MaxRTrue_V -> SetMarkerSize(0.6);
    gr_LiveMethod_BunchNumber_MaxRTrue_V -> SetLineWidth(0);
    gr_LiveMethod_BunchNumber_MaxRTrue_V -> SetTitle(";Bunch number;(Overlap steps in V) RTrue [per bunch crossing]");

    gr_LiveMethod_BunchNumber_WidthV -> SetMarkerStyle(20);
    gr_LiveMethod_BunchNumber_WidthV -> SetMarkerColor(1);
    gr_LiveMethod_BunchNumber_WidthV -> SetMarkerSize(0.6);
    gr_LiveMethod_BunchNumber_WidthV -> SetLineWidth(0);
    gr_LiveMethod_BunchNumber_WidthV -> SetTitle(";Bunch number;(in V) Overlapped Width [mm]");

    gr_LiveMethod_BunchNumber_WidthH -> SetMarkerStyle(20);
    gr_LiveMethod_BunchNumber_WidthH -> SetMarkerColor(1);
    gr_LiveMethod_BunchNumber_WidthH -> SetMarkerSize(0.6);
    gr_LiveMethod_BunchNumber_WidthH -> SetLineWidth(0);
    gr_LiveMethod_BunchNumber_WidthH -> SetTitle(";Bunch number;(in H) Overlapped Width [mm]");

    TH1D * h1D_fake = new TH1D("","",10, -2, 2);
    h1D_fake -> GetXaxis() -> SetTitle("Set scan step [mm]");
    h1D_fake -> GetYaxis() -> SetTitle("Rate [per bunch crossing]");
    h1D_fake -> SetMinimum(0);
    h1D_fake -> SetMaximum(0.15);

    std::map<string, TGraph*> gr_map; gr_map.clear();
    std::map<string, TF1*> f1_map; f1_map.clear();

    TF1 * GL1P_FinalFit_LiveH = new TF1("GL1P_FinalFit_LiveH",gaus_func,-2,2,4);
    GL1P_FinalFit_LiveH -> SetNpx(1000);
    GL1P_FinalFit_LiveH -> SetParameters(9.32502e-02, 0, 2.99013e-01, 0);
    GL1P_FinalFit_LiveH -> SetLineColor(38);
    GL1P_FinalFit_LiveH -> SetLineWidth(2);

    TF1 * GL1P_FinalFit_LiveV = new TF1("GL1P_FinalFit_LiveV",gaus_func,-2,2,4);
    GL1P_FinalFit_LiveV -> SetNpx(1000);
    GL1P_FinalFit_LiveV -> SetParameters(9.32502e-02, 0, 2.99013e-01, 0);
    GL1P_FinalFit_LiveV -> SetLineColor(38);
    GL1P_FinalFit_LiveV -> SetLineWidth(2);

    for (TObject* keyAsObj : *file_in->GetListOfKeys()) {
        auto key = dynamic_cast<TKey*>(keyAsObj);
        std::string hist_name  = key->GetName();
        std::string class_name = key->GetClassName();

        if (class_name == "TGraph"){
            TGraph * gr = (TGraph*)file_in->Get(hist_name.c_str());
            gr_map[hist_name] = gr;
        }
        else if (class_name == "TF1"){
            TF1 * f1 = (TF1*)file_in->Get(hist_name.c_str());

            f1_map[hist_name] = f1;
            f1_map[hist_name] -> GetXaxis() -> SetTitle("Set scan step [mm]");
            f1_map[hist_name] -> GetYaxis() -> SetTitle("Rate [per bunch crossing]");
        }
    }

    for (int i = 0; i < tree_in -> GetEntries(); i++) {
        tree_in -> GetEntry(i);

        if (bunch_number == -999) {continue;}

        if (is_raw) { // note : the raw approach
            gr_RawMethod_EachBunch_Rmeasure_Ks -> SetPoint(gr_RawMethod_EachBunch_Rmeasure_Ks -> GetN(), Rmeasure, Ks);
            gr_RawMethod_EachBunch_Rmeasure_Kn -> SetPoint(gr_RawMethod_EachBunch_Rmeasure_Kn -> GetN(), Rmeasure, Kn);

            gr_RawMethod_EachBunch_RTrue_Ks -> SetPoint(gr_RawMethod_EachBunch_RTrue_Ks -> GetN(), RTrue, Ks);
            gr_RawMethod_EachBunch_RTrue_Kn -> SetPoint(gr_RawMethod_EachBunch_RTrue_Kn -> GetN(), RTrue, Kn);
            

            if (is_H){
                if (step == 0 || step == 6) {
                    gr_RawMethod_BunchNumber_MaxRmeasure_H -> SetPoint(gr_RawMethod_BunchNumber_MaxRmeasure_H -> GetN(), bunch_number, Rmeasure);
                    gr_RawMethod_BunchNumber_MaxRTrue_H -> SetPoint(gr_RawMethod_BunchNumber_MaxRTrue_H -> GetN(), bunch_number, RTrue);
                }

            }

            if (!is_H){
                if (step == 0 || step == 6) {
                    gr_RawMethod_BunchNumber_MaxRmeasure_V -> SetPoint(gr_RawMethod_BunchNumber_MaxRmeasure_V -> GetN(), bunch_number, Rmeasure);
                    gr_RawMethod_BunchNumber_MaxRTrue_V -> SetPoint(gr_RawMethod_BunchNumber_MaxRTrue_V -> GetN(), bunch_number, RTrue);
                }
            }
            

        }

        if (!is_raw){ // note : the live rate approach 
            gr_LiveMethod_EachBunch_Rmeasure_Ks -> SetPoint(gr_LiveMethod_EachBunch_Rmeasure_Ks -> GetN(), Rmeasure, Ks);
            gr_LiveMethod_EachBunch_Rmeasure_Kn -> SetPoint(gr_LiveMethod_EachBunch_Rmeasure_Kn -> GetN(), Rmeasure, Kn);

            gr_LiveMethod_EachBunch_RTrue_Ks -> SetPoint(gr_LiveMethod_EachBunch_RTrue_Ks -> GetN(), RTrue, Ks);
            gr_LiveMethod_EachBunch_RTrue_Kn -> SetPoint(gr_LiveMethod_EachBunch_RTrue_Kn -> GetN(), RTrue, Kn);


            if (is_H){
                if (step == 0 || step == 6) {
                    gr_LiveMethod_BunchNumber_MaxRmeasure_H -> SetPoint(gr_LiveMethod_BunchNumber_MaxRmeasure_H -> GetN(), bunch_number, Rmeasure);
                    gr_LiveMethod_BunchNumber_MaxRTrue_H -> SetPoint(gr_LiveMethod_BunchNumber_MaxRTrue_H -> GetN(), bunch_number, RTrue);
                }
            }

            if (!is_H){
                if (step == 0 || step == 6) {
                    gr_LiveMethod_BunchNumber_MaxRmeasure_V -> SetPoint(gr_LiveMethod_BunchNumber_MaxRmeasure_V -> GetN(), bunch_number, Rmeasure);
                    gr_LiveMethod_BunchNumber_MaxRTrue_V -> SetPoint(gr_LiveMethod_BunchNumber_MaxRTrue_V -> GetN(), bunch_number, RTrue);
                }
            }
        }


    }

    // Division : --------------------------------------------------------------------------------------------
    // note: [step][bunch_number]
    std::vector<std::vector<double>> rawH_StepValue_vec = std::vector<std::vector<double>>(11);
    std::vector<std::vector<double>> rawV_StepValue_vec = std::vector<std::vector<double>>(11);
    std::vector<std::vector<double>> liveH_StepValue_vec = std::vector<std::vector<double>>(11);
    std::vector<std::vector<double>> liveV_StepValue_vec = std::vector<std::vector<double>>(11);

    for (int bunch_i = 0; bunch_i < 111; bunch_i++) {
        TGraph * gr_rawH = gr_map[Form("Bunch%d_rawH_graph", bunch_i)];
        TGraph * gr_rawV = gr_map[Form("Bunch%d_rawV_graph", bunch_i)];
        TGraph * gr_liveH = gr_map[Form("Bunch%d_liveH_graph", bunch_i)];
        TGraph * gr_liveV = gr_map[Form("Bunch%d_liveV_graph", bunch_i)];

        for (int point_i = 0; point_i < gr_rawH->GetN(); point_i++) {
            rawH_StepValue_vec[point_i].push_back(gr_rawH->GetPointY(point_i));
            rawV_StepValue_vec[point_i].push_back(gr_rawV->GetPointY(point_i));
            
            liveH_StepValue_vec[point_i].push_back(gr_liveH->GetPointY(point_i));
            liveV_StepValue_vec[point_i].push_back(gr_liveV->GetPointY(point_i));
        }
    }

    std::vector<TH1D*> rawH_h1D_StepRate_vec; rawH_h1D_StepRate_vec.clear();
    std::vector<TH1D*> rawV_h1D_StepRate_vec; rawV_h1D_StepRate_vec.clear();
    std::vector<TH1D*> liveH_h1D_StepRate_vec; liveH_h1D_StepRate_vec.clear();
    std::vector<TH1D*> liveV_h1D_StepRate_vec; liveV_h1D_StepRate_vec.clear();

    TGraphErrors * grE_rawH_GL1P_Avg = new TGraphErrors();
    grE_rawH_GL1P_Avg -> SetMarkerStyle(20);
    grE_rawH_GL1P_Avg -> SetMarkerColor(91);
    grE_rawH_GL1P_Avg -> SetMarkerSize(0.8);
    grE_rawH_GL1P_Avg -> SetLineColor(91);
    grE_rawH_GL1P_Avg -> SetLineWidth(2);
    grE_rawH_GL1P_Avg -> SetTitle(";(rawH) Set scan step [mm];Rate [per bunch crossing]");
    
    TGraphErrors * grE_rawV_GL1P_Avg = new TGraphErrors();
    grE_rawV_GL1P_Avg -> SetMarkerStyle(20);
    grE_rawV_GL1P_Avg -> SetMarkerColor(91);
    grE_rawV_GL1P_Avg -> SetMarkerSize(0.8);
    grE_rawV_GL1P_Avg -> SetLineColor(91);
    grE_rawV_GL1P_Avg -> SetLineWidth(2);
    grE_rawV_GL1P_Avg -> SetTitle(";(rawV) Set scan step [mm];Rate [per bunch crossing]");
    
    TGraphErrors * grE_liveH_GL1P_Avg = new TGraphErrors();
    grE_liveH_GL1P_Avg -> SetMarkerStyle(20);
    grE_liveH_GL1P_Avg -> SetMarkerColor(91);
    grE_liveH_GL1P_Avg -> SetMarkerSize(0.8);
    grE_liveH_GL1P_Avg -> SetLineColor(91);
    grE_liveH_GL1P_Avg -> SetLineWidth(2);
    grE_liveH_GL1P_Avg -> SetTitle(";(liveH) Set scan step [mm];Rate [per bunch crossing]");
    
    TGraphErrors * grE_liveV_GL1P_Avg = new TGraphErrors();
    grE_liveV_GL1P_Avg -> SetMarkerStyle(20);
    grE_liveV_GL1P_Avg -> SetMarkerColor(91);
    grE_liveV_GL1P_Avg -> SetMarkerSize(0.8);
    grE_liveV_GL1P_Avg -> SetLineColor(91);
    grE_liveV_GL1P_Avg -> SetLineWidth(2);
    grE_liveV_GL1P_Avg -> SetTitle(";(liveV) Set scan step [mm];Rate [per bunch crossing]");
    
    for (int step_i = 0; step_i < 11; step_i++) {

        grE_rawH_GL1P_Avg -> SetPoint(grE_rawH_GL1P_Avg->GetN(), gr_map[Form("Bunch0_rawH_graph")] -> GetPointX(step_i), vector_average(rawH_StepValue_vec[step_i]));
        grE_rawH_GL1P_Avg -> SetPointError(grE_rawH_GL1P_Avg->GetN() - 1, 0, vector_stddev(rawH_StepValue_vec[step_i]));

        grE_rawV_GL1P_Avg -> SetPoint(grE_rawV_GL1P_Avg->GetN(), gr_map[Form("Bunch0_rawH_graph")] -> GetPointX(step_i), vector_average(rawV_StepValue_vec[step_i]));
        grE_rawV_GL1P_Avg -> SetPointError(grE_rawV_GL1P_Avg->GetN() - 1, 0, vector_stddev(rawV_StepValue_vec[step_i]));

        grE_liveH_GL1P_Avg -> SetPoint(grE_liveH_GL1P_Avg->GetN(), gr_map[Form("Bunch0_rawH_graph")] -> GetPointX(step_i), vector_average(liveH_StepValue_vec[step_i]));
        grE_liveH_GL1P_Avg -> SetPointError(grE_liveH_GL1P_Avg->GetN() - 1, 0, vector_stddev(liveH_StepValue_vec[step_i]));

        grE_liveV_GL1P_Avg -> SetPoint(grE_liveV_GL1P_Avg->GetN(), gr_map[Form("Bunch0_rawH_graph")] -> GetPointX(step_i), vector_average(liveV_StepValue_vec[step_i]));
        grE_liveV_GL1P_Avg -> SetPointError(grE_liveV_GL1P_Avg->GetN() - 1, 0, vector_stddev(liveV_StepValue_vec[step_i]));

        
        // note : in each step, we have 111 bunches, which means 111 data 
        rawH_h1D_StepRate_vec.push_back( new TH1D( 
                Form("rawH_h1D_StepRate_Step%d",step_i), 
                Form("rawH_h1D_StepRate_Step%d;Rate [per bunch crossing];Entries",step_i), 
                100, 
                vector_average(rawH_StepValue_vec[step_i]) - 8 * vector_stddev(rawH_StepValue_vec[step_i]),
                vector_average(rawH_StepValue_vec[step_i]) + 8 * vector_stddev(rawH_StepValue_vec[step_i])
            ) 
        );

        rawV_h1D_StepRate_vec.push_back( new TH1D( 
                Form("rawV_h1D_StepRate_Step%d",step_i), 
                Form("rawV_h1D_StepRate_Step%d;Rate [per bunch crossing];Entries",step_i), 
                100, 
                vector_average(rawV_StepValue_vec[step_i]) - 8 * vector_stddev(rawV_StepValue_vec[step_i]),
                vector_average(rawV_StepValue_vec[step_i]) + 8 * vector_stddev(rawV_StepValue_vec[step_i])
            ) 
        );

        liveH_h1D_StepRate_vec.push_back( new TH1D( 
                Form("liveH_h1D_StepRate_Step%d",step_i), 
                Form("liveH_h1D_StepRate_Step%d;Rate [per bunch crossing];Entries",step_i), 
                100, 
                vector_average(liveH_StepValue_vec[step_i]) - 8 * vector_stddev(liveH_StepValue_vec[step_i]),
                vector_average(liveH_StepValue_vec[step_i]) + 8 * vector_stddev(liveH_StepValue_vec[step_i])
            ) 
        );

        liveV_h1D_StepRate_vec.push_back( new TH1D( 
                Form("liveV_h1D_StepRate_Step%d",step_i), 
                Form("liveV_h1D_StepRate_Step%d;Rate [per bunch crossing];Entries",step_i), 
                100, 
                vector_average(liveV_StepValue_vec[step_i]) - 8 * vector_stddev(liveV_StepValue_vec[step_i]),
                vector_average(liveV_StepValue_vec[step_i]) + 8 * vector_stddev(liveV_StepValue_vec[step_i])
            ) 
        );

        for (int bunch_i = 0; bunch_i < 111; bunch_i++) {
            rawH_h1D_StepRate_vec[step_i] -> Fill(rawH_StepValue_vec[step_i][bunch_i]);
            rawV_h1D_StepRate_vec[step_i] -> Fill(rawV_StepValue_vec[step_i][bunch_i]);
            liveH_h1D_StepRate_vec[step_i] -> Fill(liveH_StepValue_vec[step_i][bunch_i]);
            liveV_h1D_StepRate_vec[step_i] -> Fill(liveV_StepValue_vec[step_i][bunch_i]);
        }
    }

    grE_liveH_GL1P_Avg -> Fit("GL1P_FinalFit_LiveH", "N");
    grE_liveV_GL1P_Avg -> Fit("GL1P_FinalFit_LiveV", "N");    

    std::cout<<"fit_H, Height: "<<Form("%.5f", GL1P_FinalFit_LiveH -> GetParameter(0))<<", fit unc. : "<< Form("%.5f", GL1P_FinalFit_LiveH -> GetParError(0))<<", ratio (unc./par.):"<< Form("%.5f", GL1P_FinalFit_LiveH -> GetParError(0) / GL1P_FinalFit_LiveH -> GetParameter(0))<<std::endl;
    std::cout<<"fit_H, Mean:   "<<Form("%.5f", GL1P_FinalFit_LiveH -> GetParameter(1))<<", fit unc. : "<< Form("%.5f", GL1P_FinalFit_LiveH -> GetParError(1))<<", ratio (unc./par.):"<< Form("%.5f", GL1P_FinalFit_LiveH -> GetParError(1) / GL1P_FinalFit_LiveH -> GetParameter(1))<<std::endl;
    std::cout<<"fit_H, Width:  "<<Form("%.5f", GL1P_FinalFit_LiveH -> GetParameter(2))<<", fit unc. : "<< Form("%.5f", GL1P_FinalFit_LiveH -> GetParError(2))<<", ratio (unc./par.):"<< Form("%.5f", GL1P_FinalFit_LiveH -> GetParError(2) / GL1P_FinalFit_LiveH -> GetParameter(2))<<std::endl;
    std::cout<<"fit_H, Offset: "<<Form("%.5f", GL1P_FinalFit_LiveH -> GetParameter(3))<<", fit unc. : "<< Form("%.5f", GL1P_FinalFit_LiveH -> GetParError(3))<<", ratio (unc./par.):"<< Form("%.5f", GL1P_FinalFit_LiveH -> GetParError(3) / GL1P_FinalFit_LiveH -> GetParameter(3))<<std::endl;
    std::cout<<std::endl;
    std::cout<<"fit_V, Height: "<<Form("%.5f", GL1P_FinalFit_LiveV -> GetParameter(0))<<", fit unc. : "<< Form("%.5f", GL1P_FinalFit_LiveV -> GetParError(0))<<", ratio (unc./par.):"<< Form("%.5f", GL1P_FinalFit_LiveV -> GetParError(0) / GL1P_FinalFit_LiveV -> GetParameter(0))<<std::endl;
    std::cout<<"fit_V, Mean:   "<<Form("%.5f", GL1P_FinalFit_LiveV -> GetParameter(1))<<", fit unc. : "<< Form("%.5f", GL1P_FinalFit_LiveV -> GetParError(1))<<", ratio (unc./par.):"<< Form("%.5f", GL1P_FinalFit_LiveV -> GetParError(1) / GL1P_FinalFit_LiveV -> GetParameter(1))<<std::endl;
    std::cout<<"fit_V, Width:  "<<Form("%.5f", GL1P_FinalFit_LiveV -> GetParameter(2))<<", fit unc. : "<< Form("%.5f", GL1P_FinalFit_LiveV -> GetParError(2))<<", ratio (unc./par.):"<< Form("%.5f", GL1P_FinalFit_LiveV -> GetParError(2) / GL1P_FinalFit_LiveV -> GetParameter(2))<<std::endl;
    std::cout<<"fit_V, Offset: "<<Form("%.5f", GL1P_FinalFit_LiveV -> GetParameter(3))<<", fit unc. : "<< Form("%.5f", GL1P_FinalFit_LiveV -> GetParError(3))<<", ratio (unc./par.):"<< Form("%.5f", GL1P_FinalFit_LiveV -> GetParError(3) / GL1P_FinalFit_LiveV -> GetParameter(3))<<std::endl;


    // Division : --------------------------------------------------------------------------------------------
    TFile * file_out = new TFile(Form("%s/MBDNSCount_GL1P_summary.root", output_directory.c_str()), "RECREATE");
    file_out -> cd();

    c1 -> cd();
    grE_liveH_GL1P_Avg -> Draw("ap");
    GL1P_FinalFit_LiveH -> Draw("lsame");
    c1 -> Write("c1_grE_liveH_GL1P_Avg");

    grE_liveV_GL1P_Avg -> Draw("ap");
    GL1P_FinalFit_LiveV -> Draw("lsame");
    c1 -> Write("c1_grE_liveV_GL1P_Avg");



    grE_rawH_GL1P_Avg -> Write("grE_rawH_GL1P_Avg");
    grE_rawV_GL1P_Avg -> Write("grE_rawV_GL1P_Avg");
    grE_liveH_GL1P_Avg -> Write("grE_liveH_GL1P_Avg");
    grE_liveV_GL1P_Avg -> Write("grE_liveV_GL1P_Avg");

    for (int i = 0; i < rawH_h1D_StepRate_vec.size(); i++) {rawH_h1D_StepRate_vec[i] -> Write();}
    for (int i = 0; i < rawV_h1D_StepRate_vec.size(); i++) {rawV_h1D_StepRate_vec[i] -> Write();}
    for (int i = 0; i < liveH_h1D_StepRate_vec.size(); i++) {liveH_h1D_StepRate_vec[i] -> Write();}
    for (int i = 0; i < liveV_h1D_StepRate_vec.size(); i++) {liveV_h1D_StepRate_vec[i] -> Write();}


    f1_map["BunchAll_rawH_fit"] -> SetLineColor(2);
    f1_map["BunchAll_rawV_fit"] -> SetLineColor(2);
    f1_map["BunchAll_liveH_fit"] -> SetLineColor(2);
    f1_map["BunchAll_liveV_fit"] -> SetLineColor(2);

    // Division : --------------------------------------------------------------------------------------------
    int count = 0;

    c1 -> cd();
    h1D_fake -> Draw("hist");
    for (auto pair : f1_map) {
        string hist_name = pair.first;
        TF1 * f1 = pair.second;

        if (hist_name.find("BunchAll") != std::string::npos) {continue;}

        if (hist_name.find("rawH_fit") != std::string::npos) {
            std::string draw_text = (count == 0) ? "lsame" : "lsame";

            f1 -> SetLineColor(1);
            f1 -> Draw(draw_text.c_str());

            gr_RawMethod_BunchNumber_WidthH -> SetPoint(
                gr_RawMethod_BunchNumber_WidthH -> GetN(),
                stoi( hist_name.substr( 5, hist_name.find("_") - 5 ) ),
                f1 -> GetParameter(2) 
            );

            h1D_RawMethod_WidthH -> Fill( f1 -> GetParameter(2) );

            count += 1;
        }
    }
    f1_map["BunchAll_rawH_fit"] -> Draw("lsame");
    grE_rawH_GL1P_Avg -> Draw("p same");
    file_out -> cd();
    c1 -> Write("c1_rawH_allFit");
    c1 -> Clear();

    // Division : --------------------------------------------------------------------------------------------

    count = 0;
    c1 -> cd();
    h1D_fake -> Draw("hist");
    for (auto pair : f1_map) {
        string hist_name = pair.first;
        TF1 * f1 = pair.second;

        if (hist_name.find("BunchAll") != std::string::npos) {continue;}

        if (hist_name.find("rawV_fit") != std::string::npos) {
            std::string draw_text = (count == 0) ? "lsame" : "lsame";

            f1 -> SetLineColor(1);
            f1 -> Draw(draw_text.c_str());

            gr_RawMethod_BunchNumber_WidthV -> SetPoint(
                gr_RawMethod_BunchNumber_WidthV -> GetN(),
                stoi( hist_name.substr( 5, hist_name.find("_") - 5 ) ),
                f1 -> GetParameter(2) 
            );

            h1D_RawMethod_WidthV -> Fill( f1 -> GetParameter(2) );

            count += 1;
        }
    }
    f1_map["BunchAll_rawV_fit"] -> Draw("lsame");
    grE_rawV_GL1P_Avg -> Draw("p same");
    file_out -> cd();
    c1 -> Write("c1_rawV_allFit");
    c1 -> Clear();

    // Division : --------------------------------------------------------------------------------------------

    count = 0;
    c1 -> cd();
    h1D_fake -> Draw("hist");
    for (auto pair : f1_map) {
        string hist_name = pair.first;
        TF1 * f1 = pair.second;

        if (hist_name.find("BunchAll") != std::string::npos) {continue;}

        if (hist_name.find("liveH_fit") != std::string::npos) {
            std::string draw_text = (count == 0) ? "lsame" : "lsame";

            f1 -> SetLineColor(1);
            f1 -> Draw(draw_text.c_str());

            gr_LiveMethod_BunchNumber_WidthH -> SetPoint(
                gr_LiveMethod_BunchNumber_WidthH -> GetN(),
                stoi( hist_name.substr( 5, hist_name.find("_") - 5 ) ),
                f1 -> GetParameter(2) 
            );

            h1D_LiveMethod_WidthH -> Fill( f1 -> GetParameter(2) );

            count += 1;
        }
    }
    f1_map["BunchAll_liveH_fit"] -> Draw("lsame");
    grE_liveH_GL1P_Avg -> Draw("p same");
    file_out -> cd();
    c1 -> Write("c1_liveH_allFit");
    c1 -> Clear();


    // Division : --------------------------------------------------------------------------------------------

    count = 0;
    c1 -> cd();
    h1D_fake -> Draw("hist");
    for (auto pair : f1_map) {
        string hist_name = pair.first;
        TF1 * f1 = pair.second;

        if (hist_name.find("BunchAll") != std::string::npos) {continue;}

        if (hist_name.find("liveV_fit") != std::string::npos) {
            std::string draw_text = (count == 0) ? "lsame" : "lsame";

            f1 -> SetLineColor(1);
            f1 -> Draw(draw_text.c_str());

            gr_LiveMethod_BunchNumber_WidthV -> SetPoint(
                gr_LiveMethod_BunchNumber_WidthV -> GetN(),
                stoi( hist_name.substr( 5, hist_name.find("_") - 5 ) ),
                f1 -> GetParameter(2) 
            );

            h1D_LiveMethod_WidthV -> Fill( f1 -> GetParameter(2) );

            count += 1;
        }
    }
    f1_map["BunchAll_liveV_fit"] -> Draw("lsame");
    grE_liveV_GL1P_Avg -> Draw("p same");
    file_out -> cd();
    c1 -> Write("c1_liveV_allFit");
    c1 -> Clear();
    // Division : --------------------------------------------------------------------------------------------

    gStyle -> SetOptFit(1);

    h1D_RawMethod_WidthH -> Fit("gaus");
    h1D_RawMethod_WidthV -> Fit("gaus");
    h1D_LiveMethod_WidthH -> Fit("gaus");
    h1D_LiveMethod_WidthV -> Fit("gaus");

    c1 -> cd();
    h1D_RawMethod_WidthH -> Draw("ep");
    h1D_RawMethod_WidthH -> GetFunction("gaus") -> SetLineColor(2);
    h1D_RawMethod_WidthH -> GetFunction("gaus") -> SetNpx(1000);
    h1D_RawMethod_WidthH -> GetFunction("gaus") -> Draw("lsame");
    c1 -> Write("c1_h1D_RawMethod_WidthH");
    c1 -> Clear();

    c1 -> cd();
    h1D_RawMethod_WidthV -> Draw("ep");
    h1D_RawMethod_WidthV -> GetFunction("gaus") -> SetLineColor(2);
    h1D_RawMethod_WidthV -> GetFunction("gaus") -> SetNpx(1000);
    h1D_RawMethod_WidthV -> GetFunction("gaus") -> Draw("lsame");
    c1 -> Write("c1_h1D_RawMethod_WidthV");
    c1 -> Clear();

    c1 -> cd();
    h1D_LiveMethod_WidthH -> Draw("ep");
    h1D_LiveMethod_WidthH -> GetFunction("gaus") -> SetLineColor(2);
    h1D_LiveMethod_WidthH -> GetFunction("gaus") -> SetNpx(1000);
    h1D_LiveMethod_WidthH -> GetFunction("gaus") -> Draw("lsame");
    c1 -> Write("c1_h1D_LiveMethod_WidthH");
    c1 -> Clear();

    c1 -> cd();
    h1D_LiveMethod_WidthV -> Draw("ep");
    h1D_LiveMethod_WidthV -> GetFunction("gaus") -> SetLineColor(2);
    h1D_LiveMethod_WidthV -> GetFunction("gaus") -> SetNpx(1000);
    h1D_LiveMethod_WidthV -> GetFunction("gaus") -> Draw("lsame");
    c1 -> Write("c1_h1D_LiveMethod_WidthV");
    c1 -> Clear();


    file_out -> cd();

    gr_RawMethod_EachBunch_Rmeasure_Ks -> Write("gr_RawMethod_EachBunch_Rmeasure_Ks");
    gr_RawMethod_EachBunch_Rmeasure_Kn -> Write("gr_RawMethod_EachBunch_Rmeasure_Kn");
    
    gr_RawMethod_EachBunch_RTrue_Ks -> Write("gr_RawMethod_EachBunch_RTrue_Ks");
    gr_RawMethod_EachBunch_RTrue_Kn -> Write("gr_RawMethod_EachBunch_RTrue_Kn");
    
    gr_RawMethod_BunchNumber_MaxRmeasure_H -> Write("gr_RawMethod_BunchNumber_MaxRmeasure_H");
    gr_RawMethod_BunchNumber_MaxRTrue_H -> Write("gr_RawMethod_BunchNumber_MaxRTrue_H");
    
    gr_RawMethod_BunchNumber_MaxRmeasure_V -> Write("gr_RawMethod_BunchNumber_MaxRmeasure_V");
    gr_RawMethod_BunchNumber_MaxRTrue_V -> Write("gr_RawMethod_BunchNumber_MaxRTrue_V");
    
    gr_RawMethod_BunchNumber_WidthV -> Write("gr_RawMethod_BunchNumber_WidthV");
    gr_RawMethod_BunchNumber_WidthH -> Write("gr_RawMethod_BunchNumber_WidthH");

    gr_LiveMethod_EachBunch_Rmeasure_Ks -> Write("gr_LiveMethod_EachBunch_Rmeasure_Ks");
    gr_LiveMethod_EachBunch_Rmeasure_Kn -> Write("gr_LiveMethod_EachBunch_Rmeasure_Kn");

    gr_LiveMethod_EachBunch_RTrue_Ks -> Write("gr_LiveMethod_EachBunch_RTrue_Ks");
    gr_LiveMethod_EachBunch_RTrue_Kn -> Write("gr_LiveMethod_EachBunch_RTrue_Kn");
    
    gr_LiveMethod_BunchNumber_MaxRmeasure_H -> Write("gr_LiveMethod_BunchNumber_MaxRmeasure_H");
    gr_LiveMethod_BunchNumber_MaxRTrue_H -> Write("gr_LiveMethod_BunchNumber_MaxRTrue_H");
    
    gr_LiveMethod_BunchNumber_MaxRmeasure_V -> Write("gr_LiveMethod_BunchNumber_MaxRmeasure_V");
    gr_LiveMethod_BunchNumber_MaxRTrue_V -> Write("gr_LiveMethod_BunchNumber_MaxRTrue_V");
    
    gr_LiveMethod_BunchNumber_WidthV -> Write("gr_LiveMethod_BunchNumber_WidthV");
    gr_LiveMethod_BunchNumber_WidthH -> Write("gr_LiveMethod_BunchNumber_WidthH");

    h1D_RawMethod_WidthH -> Write();
    h1D_RawMethod_WidthV -> Write();
    h1D_LiveMethod_WidthH -> Write();
    h1D_LiveMethod_WidthV -> Write();

    file_out -> Close();

    return 888;
}