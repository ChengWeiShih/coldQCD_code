int Plot_RawCount_LiveCountLiveRatio()
{
    std::string input_file = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform_GL1P/ntuple_run51195_merged_rawlive_rate.root";
    TFile * file_in = TFile::Open(input_file.c_str());

    TTree * tree = (TTree*)file_in->Get("tree");
    int timestamp;
    long long MBDNS_raw_counting_front;
    long long MBDNS_raw_counting_back;
    long long MBDNS_live_counting_front;
    long long MBDNS_live_counting_back;
    long long GL1Scalers_range_first;
    long long GL1Scalers_range_second;
    long long GL1Scalers_live_range_first;
    long long GL1Scalers_live_range_second;

    tree -> SetBranchAddress("timestamp", &timestamp);
    tree -> SetBranchAddress("MBDNS_raw_counting_front", &MBDNS_raw_counting_front);
    tree -> SetBranchAddress("MBDNS_raw_counting_back", &MBDNS_raw_counting_back);
    tree -> SetBranchAddress("MBDNS_live_counting_front", &MBDNS_live_counting_front);
    tree -> SetBranchAddress("MBDNS_live_counting_back", &MBDNS_live_counting_back);
    tree -> SetBranchAddress("GL1Scalers_range_first", &GL1Scalers_range_first);
    tree -> SetBranchAddress("GL1Scalers_range_second", &GL1Scalers_range_second);
    tree -> SetBranchAddress("GL1Scalers_live_range_first", &GL1Scalers_live_range_first);
    tree -> SetBranchAddress("GL1Scalers_live_range_second", &GL1Scalers_live_range_second);

    TCanvas * c1 = new TCanvas("c1", "c1", 1800, 1200);
    TPad * pad1 = new TPad("pad1", "pad1", 0.0, 0.4, 1.0, 1.0);
    pad1 -> Draw();
    pad1 -> cd();
    
    pad1 -> SetBottomMargin(0.03);
    pad1 -> SetLeftMargin(0.18);

    TGraph * gr_original = new TGraph();
    TGraph * gr_LiveCountLiveRatio = new TGraph();
    TGraph * gr_ratio = new TGraph();

    for (int i = 0; i < tree -> GetEntries(); i++)
    {
        tree->GetEntry(i);

        gr_original -> SetPoint(i, timestamp, MBDNS_raw_counting_back - MBDNS_raw_counting_front);
        
        // std::cout<<MBDNS_live_counting_back<<", "<<MBDNS_live_counting_front<<", "<<GL1Scalers_live_range_second<<", "<<GL1Scalers_live_range_first<<", "<<GL1Scalers_range_second<<", "<<GL1Scalers_range_first<<std::endl;
        // std::cout<<timestamp<<", "<<double(MBDNS_live_counting_back - MBDNS_live_counting_front) / double( double(GL1Scalers_live_range_second - GL1Scalers_live_range_first) / double(GL1Scalers_range_second - GL1Scalers_range_first) )<<std::endl;

        gr_LiveCountLiveRatio -> SetPoint(
            i, 
            timestamp, 
            double(MBDNS_live_counting_back - MBDNS_live_counting_front) / ( double(GL1Scalers_live_range_second - GL1Scalers_live_range_first) / double(GL1Scalers_range_second - GL1Scalers_range_first) )
        );

        gr_ratio -> SetPoint(i, timestamp, gr_original->GetPointY(i) / gr_LiveCountLiveRatio->GetPointY(i));
    } 

    gr_original -> SetMarkerStyle(20);
    gr_original -> SetMarkerSize(0.6);
    gr_original -> SetMarkerColor(kBlack);

    gr_LiveCountLiveRatio -> SetMarkerStyle(20);
    gr_LiveCountLiveRatio -> SetMarkerSize(0.6);
    gr_LiveCountLiveRatio -> SetMarkerColor(kRed);

    gr_LiveCountLiveRatio->GetXaxis()->SetLabelOffset(999);
    gr_LiveCountLiveRatio->GetXaxis()->SetLabelSize(0);
    gr_LiveCountLiveRatio->GetXaxis()->SetTitleOffset(999);
    gr_LiveCountLiveRatio->GetYaxis()->SetLabelSize(0.045);
    gr_LiveCountLiveRatio->GetYaxis()->SetTitleOffset(0.8);
    gr_LiveCountLiveRatio->GetYaxis()->SetTitleSize(0.07);    
    gr_LiveCountLiveRatio->GetXaxis()->SetNdivisions();

    // gr_LiveCountLiveRatio -> GetXaxis() -> SetLegendOffset(999);
    // gr_LiveCountLiveRatio -> GetXaxis() -> SetTitleOffset(999);

    gr_ratio -> SetMarkerStyle(20);
    gr_ratio -> SetMarkerSize(0.6);
    gr_ratio -> SetMarkerColor(kBlack);

    TLegend * legend = new TLegend(0.5, 0.7, 0.9, 0.9);
    legend -> SetMargin(0.1);
    legend -> AddEntry(gr_original, "Raw MBDNS trigger count", "p");
    legend -> AddEntry(gr_LiveCountLiveRatio, "Live MBDNS trigger count / Clock live ratio", "p");

    pad1 -> cd();
    gr_LiveCountLiveRatio -> GetYaxis() -> SetTitle("Counts");
    gr_LiveCountLiveRatio -> GetYaxis() -> SetRangeUser(0, 1500 * 1000);
    gr_LiveCountLiveRatio -> Draw("AP");
    gr_original -> Draw("P SAME");

    legend -> Draw();

    c1 -> cd();
    TPad * pad2 = new TPad("pad2", "pad2", 0.0, 0.0, 1.0, 0.4);
    pad2 -> Draw();
    pad2 -> cd();

    pad2 -> SetTopMargin(0.02);
    pad2 -> SetBottomMargin(0.3);
    pad2 -> SetLeftMargin(0.18);

    gr_ratio -> GetYaxis() -> SetTitle("Ratio");
    gr_ratio -> GetXaxis() -> SetTitle("Timestamp [s]");

    gr_ratio->GetXaxis()->SetLabelSize(0.11);
    gr_ratio->GetXaxis()->SetTitleOffset(0.8);
    gr_ratio->GetXaxis()->SetTitleSize(0.14);
    gr_ratio->GetXaxis()->SetNdivisions();

    gr_ratio->GetYaxis()->SetLabelSize(0.11);
    gr_ratio->GetYaxis()->SetTitleOffset(0.4);
    gr_ratio->GetYaxis()->SetTitleSize(0.14);
    gr_ratio->GetYaxis()->SetNdivisions(505);
    
    gr_ratio -> GetYaxis() -> SetRangeUser(0.95, 1.05);
    gr_ratio -> Draw("AP");

    TLine * line = new TLine(gr_ratio->GetXaxis()->GetXmin(), 1, gr_ratio->GetXaxis()->GetXmax(), 1);
    line -> SetLineColor(kRed);
    line -> SetLineWidth(1);
    line -> SetLineStyle(9);
    line -> Draw("SAME");

    c1 -> Print("Plot_RawCount_LiveCountLiveRatio.pdf");

    return 0;
}