#include "../ZDCvtxZ.h"

R__LOAD_LIBRARY(../libzdcvtxz.so)

int test_run()
{
    long long nRunEvent_in = -1;
    string input_full_directory_in = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform/ntuple_run51195_merged.root";
    string output_directory_in = "/sphenix/tg/tg01/coldqcd/cwshih/test/run_51195/from_Chris_mbdZ/ntuple_file_waveform/quick_check";
    bool NS_offset_in = true;

    ZDCvtxZ * ZDCvtx = new ZDCvtxZ(
        nRunEvent_in,
        input_full_directory_in,
        output_directory_in,
        NS_offset_in
    );

    ZDCvtx -> RunEvent();
    ZDCvtx -> DrawPlots();
    ZDCvtx -> CloseRootFileOut();

    return 0;
}