#include <iostream>

#include <TMath.h>
#include <cmath>

int main()
{
    double um2_to_mb = 1.0e-19;
    double fbeam = 78 * 1000; // note : 78 kHz

    double the_bunch_factor = 111; // note : 111 bunches

    double DCCT_B = 15055.9454 * pow(10, 9) / the_bunch_factor; // note : beam intensity of blue beam, take the average of the whole trend
    double DCCT_Y = 18334.9176 * pow(10, 9) / the_bunch_factor; // note : beam intensity of yellow beam, take the average of the whole trend
    
    
    
    double MBD_width_beam_overlap_X = 284.95; // note : unit : um
    double MBD_width_beam_overlap_Y = 276.44; // note : unit : um

    double MBD_machine_lumi = (fbeam / (2 * M_PI * MBD_width_beam_overlap_X * MBD_width_beam_overlap_Y)) * (DCCT_B * DCCT_Y) * um2_to_mb;

    double MBD_Rmax = 1043710.957 / the_bunch_factor; // note : unit : Hz, take a average of the four center overlap steps.
    double MBD_effi = 1.;//0.4234; // note : given a perfect gaussian with the width 60 cm, the protion of the events happened within \pm 30 cm
    double MBD_crossingsection = MBD_Rmax / (MBD_machine_lumi * MBD_effi);

    std::cout << "MBD_Machine luminosity : " << MBD_machine_lumi << std::endl;
    std::cout << "MBD_crossingsection : " << MBD_crossingsection << std::endl;




    double ZDC_width_beam_overlap_X = 256.55; // note : unit : um
    double ZDC_width_beam_overlap_Y = 249.37; // note : unit : um

    double ZDC_machine_lumi = (fbeam / (2 * M_PI * ZDC_width_beam_overlap_X * ZDC_width_beam_overlap_Y)) * (DCCT_B * DCCT_Y) * um2_to_mb;

    double ZDC_Rmax = 13592.27807 / the_bunch_factor; // note : unit : Hz, take a average of the four center overlap steps.
    double ZDC_effi = 1.;//0.4234; // note : given a perfect gaussian with the width 60 cm, the protion of the events happened within \pm 30 cm
    double ZDC_crossingsection = ZDC_Rmax / (ZDC_machine_lumi * ZDC_effi);
    std::cout<<std::endl;
    std::cout << "ZDC_Machine luminosity : " << ZDC_machine_lumi << std::endl;
    std::cout << "ZDC_crossingsection : " << ZDC_crossingsection << std::endl;




    // note : Test
    // double test_DCCT_BY = 10459 * pow(10,18);
    // double test_width_beam_overlap_X = 270.8; // note : unit : um
    // double test_width_beam_overlap_Y = 268.9; // note : unit : um
    // double test_machine_lumi = (fbeam / (2 * M_PI * test_width_beam_overlap_X * test_width_beam_overlap_Y)) * (test_DCCT_BY) * um2_to_mb;
    // double test_MBD_effi = 0.433;
    // double test_Rmax = 1337.4;
    // double test_MBD_crossingsection = test_Rmax / (test_machine_lumi * test_MBD_effi);
    // std::cout << "Test machine luminosity : " << test_machine_lumi << std::endl;
    // std::cout << "Test MBD_crossingsection : " << test_MBD_crossingsection << std::endl;

    return 0;
}