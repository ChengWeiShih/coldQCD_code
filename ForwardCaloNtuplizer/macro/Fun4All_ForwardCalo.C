#ifndef FUN4ALL_FORWARDCALO_H
#define FUN4ALL_FORWARDCALO_H

#include <forwardcalontuplizer/ForwardCaloNtuplizer.h>

#include <G4_Input.C>
#include <Calo_Calib.C>

#include <caloreco/CaloTowerBuilder.h>

#include <caloreco/CaloTowerCalib.h>
#include <caloreco/CaloTowerStatus.h>
#include <zdcinfo/ZdcReco.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/SubsysReco.h>


#include <phool/recoConsts.h>

#include <ffamodules/CDBInterface.h>
#include <mbd/MbdReco.h>

#include <iostream>
#include <string>
#include <cstdio> 

// cppcheck-suppress unknownMacro
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libForwardCaloNtuplizer.so)
R__LOAD_LIBRARY(libmbd.so)

R__LOAD_LIBRARY(libzdcinfo.so)
R__LOAD_LIBRARY(libcalo_reco.so)

void Fun4All_ForwardCalo(
    const int process = 0,
    const int nEvents = -1,
    const std::string &input_filename = "/sphenix/lustre01/sphnxpro/physics/slurp/calophysics/run_00048000_00048100/DST_TRIGGERED_EVENT_run2pp_new_2024p003-00048082-0000.root", 
    const std::string &output_directory = "/sphenix/tg/tg01/coldqcd/cwshih/test/test_run_48082",
    const std::string &outname = "test_out_48082.root",    
    const std::string &dbtag = "2024p007"
)

{
    // int skip = nEvents * process;
    int skip = 0; // note : if say, processing one dst file per job, then skip = 0

    Fun4AllServer * se = Fun4AllServer::instance();
    se -> Verbosity(0);

    Fun4AllInputManager *in1 = new Fun4AllDstInputManager("in1");
    in1 -> Verbosity(0);
    in1->AddFile(input_filename);
    se->registerInputManager(in1);

    cout<<11111<<endl;

    // cout<<"test, the runnumber from Fun4AllInputManager : "<<in1->RunNumber()<<endl;

    // INPUTREADHITS::filename[0] = input_filename;

    // Fun4AllInputManager * in1 = new Fun4AllDstInputManager("in1");
    // in1 -> AddFile(fname);
    // se -> registerInputManager(in1);

    recoConsts *rc = recoConsts::instance();

    cout<<22222<<endl;

    // const std::string& dbtag= "2024p007";

    rc->set_StringFlag("CDB_GLOBALTAG", dbtag);
    // rc->set_uint64Flag("TIMESTAMP", CDB::timestamp);
    CDBInterface::instance()->Verbosity(1);

    cout<<33333<<endl;

    // MBD/BBC Reconstruction
    MbdReco *mbdreco = new MbdReco();
    se->registerSubsystem(mbdreco);

    cout<<44444<<endl;

    // note : ------------------ ------------------ ------------------ for the ZDC ------------------ ------------------ ------------------ 
    CaloTowerDefs::BuilderType buildertype = CaloTowerDefs::kPRDFTowerv4;

    cout<<55555<<endl;

    CaloTowerBuilder *caZDC = new CaloTowerBuilder("ZDCBUILDER");
    caZDC->set_detector_type(CaloTowerDefs::ZDC);
    caZDC->set_builder_type(buildertype);
    caZDC->set_processing_type(CaloWaveformProcessing::FAST);
    caZDC->set_nsamples(16);
    caZDC->set_offlineflag();
    se->registerSubsystem(caZDC);

    cout<<66666<<endl;

    //ZDC Reconstruction--Calib Info
    ZdcReco *zdcreco = new ZdcReco();
    se->registerSubsystem(zdcreco);

    cout<<77777<<endl;

    /////////////////////////////////////////////////////
    // Set status of towers, Calibrate towers,  Cluster
    Process_Calo_Calib();

    cout<<88888<<endl;

    // note : ------------------ ------------------ ------------------ for the ZDC ------------------ ------------------ ------------------ 

    // //ZDC Reconstruction--Calib Info
    // ZdcReco *zdcreco = new ZdcReco();
    // se->registerSubsystem(zdcreco);

    // std::cout << "Calibrating ZDC" << std::endl;
    // CaloTowerCalib *calibZDC = new CaloTowerCalib("ZDC");
    // calibZDC->set_detector_type(CaloTowerDefs::ZDC);
    // se->registerSubsystem(calibZDC);

    bool get_mbd_z = true;  
    bool get_waveform = true;
    bool get_zdc_z = true;

    ForwardCaloNtuplizer * forwardcalontuplizer = new ForwardCaloNtuplizer(
        "ForwardCaloNtuplizer",
        output_directory,
        outname,
        get_waveform,
        get_mbd_z,
        get_zdc_z
    );

    cout<<99999<<endl;

    se -> registerSubsystem(forwardcalontuplizer);
    
    cout<<101010<<endl;

    //--------------
    // Set up Input Managers
    //--------------
    // InputManagers();

    se->fileopen("in1",input_filename);

    //-----------------
    // Event processing
    //-----------------
    se->skip(skip);
    se->run(nEvents);

    //-----
    // Exit
    //-----
    se->End();

    CDBInterface::instance()->Print();  // print used DB files
    se->PrintTimer();

    std::cout << "All done" << std::endl;
    delete se;

    gSystem->Exit(0);
    return 0;

}

#endif
