#ifndef FUN4ALL_FORWARDCALO_H
#define FUN4ALL_FORWARDCALO_H

#include <forwardcalontuplizer/ForwardCaloNtuplizer.h>

#include <G4_Input.C>

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

    // cout<<"test, the runnumber from Fun4AllInputManager : "<<in1->RunNumber()<<endl;

    // INPUTREADHITS::filename[0] = input_filename;

    // Fun4AllInputManager * in1 = new Fun4AllDstInputManager("in1");
    // in1 -> AddFile(fname);
    // se -> registerInputManager(in1);

    recoConsts *rc = recoConsts::instance();

    // const std::string& dbtag= "2024p007";

    rc->set_StringFlag("CDB_GLOBALTAG", dbtag);
    // rc->set_uint64Flag("TIMESTAMP", CDB::timestamp);
    CDBInterface::instance()->Verbosity(1);


    // MBD/BBC Reconstruction
    MbdReco *mbdreco = new MbdReco();
    se->registerSubsystem(mbdreco);

    bool get_mbd_z = true;  

    ForwardCaloNtuplizer * forwardcalontuplizer = new ForwardCaloNtuplizer(
        "ForwardCaloNtuplizer",
        output_directory,
        outname,
        get_mbd_z
    );

    se -> registerSubsystem(forwardcalontuplizer);
    
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
