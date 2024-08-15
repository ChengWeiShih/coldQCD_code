#include <iostream>

#include <TH1.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TF1.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>
#include <TString.h>
#include <TLatex.h>

using namespace std;

// double func(Double_t * x, Double_t * par)
// {
//     return 
// }

int main()
{
    TCanvas * c1 = new TCanvas("c1", "c1", 950, 800);
    TGraph * gr = new TGraph();
    TGraph * gr_TF1 = new TGraph();

    TH1F * h1 = new TH1F("h1", "h1", 25, 0, 25);

    TF1 * correlation_fit = new TF1("correlation_fit","pol4",0,30);

    TLatex * draw_text = new TLatex();
    draw_text -> SetNDC();
    draw_text -> SetTextSize(0.03);

    c1 -> Print("poisson_test.pdf(");

    for (int i = 0; i < 100; i++)
    {
        double poisson_mean = 0.01 + 0.01 * i;
        TF1 * f1 = new TF1("f1", Form("TMath::Poisson(x, %.4f)",poisson_mean), -10, 100);
        h1 -> FillRandom("f1", 5000000);
        h1 -> SetTitle(Form("TMath::Poisson(x, %.4f);NCollision per crossing;P(x=k)",poisson_mean));
        h1 -> SetStats(0);

        // gr -> SetPoint(gr->GetN(), poisson_mean, h1 -> Integral(2,-1) / h1 -> GetBinContent(1));
        // cout<<i<<" mean : "<<poisson_mean<<" "<<h1 -> Integral(2,-1) <<" "<< h1 -> GetBinContent(1)<<endl;

        gr_TF1 -> SetPoint(gr_TF1->GetN(), poisson_mean, (1 - f1 -> Eval(0)) / f1 -> Eval(0));

        // double integration_check = 0;
        // for (int j = 0; j < 40; j++)
        // {
        //     cout<<integration_check<<" ";
        //     integration_check += f1 -> Eval(j);
        // }

        // cout<<poisson_mean<<" Integration check : "<<integration_check<<endl;

        h1 -> Scale(1.0 / h1 -> Integral(-1,-1));
        h1 -> Draw();

        f1 -> Draw("same");

        c1 -> Print("poisson_test.pdf");
        h1 -> Reset("ICESM");
        c1 -> Clear();
    }

    c1 -> Print("poisson_test.pdf)");
    c1 -> Clear();

    c1 -> cd();
    // gr -> SetMarkerStyle(20);   
    // gr -> SetMarkerSize(0.5);
    // gr -> Draw("ap");

    gr_TF1 -> SetMarkerStyle(20);
    gr_TF1 -> SetMarkerSize(0.5);
    // gr_TF1 -> SetMarkerColor(kRed);
    gr_TF1 -> GetXaxis() -> SetTitle("Poisson mean (#lambda)");
    gr_TF1 -> GetYaxis() -> SetTitle("(1 - P(0)) / P(0)");
    gr_TF1 -> Draw("ap");
    gr_TF1 -> Fit(correlation_fit, "N");
    
    draw_text -> DrawLatex(0.15, 0.85, Form("Fit : %.3f + %.3f * x + %.3f * x^{2} + %.3f * x^{3} + %.3f * x^{4}",
            correlation_fit -> GetParameter(0),
            correlation_fit -> GetParameter(1),
            correlation_fit -> GetParameter(2),
            correlation_fit -> GetParameter(3),
            correlation_fit -> GetParameter(4)
        )
    );

    correlation_fit -> Draw("lsame");
    gr_TF1 -> Draw("psame");

    c1 -> Print("poisson_ratio.pdf");

}