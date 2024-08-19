//____________________________________________________________________________..
//
// This is a template for a Fun4All SubsysReco module with all methods from the
// $OFFLINE_MAIN/include/fun4all/SubsysReco.h baseclass
// You do not have to implement all of them, you can just remove unused methods
// here and in ForwardCaloNtuplizer.h.
//
// ForwardCaloNtuplizer(const std::string &name = "ForwardCaloNtuplizer")
// everything is keyed to ForwardCaloNtuplizer, duplicate names do work but it makes
// e.g. finding culprits in logs difficult or getting a pointer to the module
// from the command line
//
// ForwardCaloNtuplizer::~ForwardCaloNtuplizer()
// this is called when the Fun4AllServer is deleted at the end of running. Be
// mindful what you delete - you do loose ownership of object you put on the node tree
//
// int ForwardCaloNtuplizer::Init(PHCompositeNode *topNode)
// This method is called when the module is registered with the Fun4AllServer. You
// can create historgrams here or put objects on the node tree but be aware that
// modules which haven't been registered yet did not put antyhing on the node tree
//
// int ForwardCaloNtuplizer::InitRun(PHCompositeNode *topNode)
// This method is called when the first event is read (or generated). At
// this point the run number is known (which is mainly interesting for raw data
// processing). Also all objects are on the node tree in case your module's action
// depends on what else is around. Last chance to put nodes under the DST Node
// We mix events during readback if branches are added after the first event
//
// int ForwardCaloNtuplizer::process_event(PHCompositeNode *topNode)
// called for every event. Return codes trigger actions, you find them in
// $OFFLINE_MAIN/include/fun4all/Fun4AllReturnCodes.h
//   everything is good:
//     return Fun4AllReturnCodes::EVENT_OK
//   abort event reconstruction, clear everything and process next event:
//     return Fun4AllReturnCodes::ABORT_EVENT;
//   proceed but do not save this event in output (needs output manager setting):
//     return Fun4AllReturnCodes::DISCARD_EVENT;
//   abort processing:
//     return Fun4AllReturnCodes::ABORT_RUN
// all other integers will lead to an error and abort of processing
//
// int ForwardCaloNtuplizer::ResetEvent(PHCompositeNode *topNode)
// If you have internal data structures (arrays, stl containers) which needs clearing
// after each event, this is the place to do that. The nodes under the DST node are cleared
// by the framework
//
// int ForwardCaloNtuplizer::EndRun(const int runnumber)
// This method is called at the end of a run when an event from a new run is
// encountered. Useful when analyzing multiple runs (raw data). Also called at
// the end of processing (before the End() method)
//
// int ForwardCaloNtuplizer::End(PHCompositeNode *topNode)
// This is called at the end of processing. It needs to be called by the macro
// by Fun4AllServer::End(), so do not forget this in your macro
//
// int ForwardCaloNtuplizer::Reset(PHCompositeNode *topNode)
// not really used - it is called before the dtor is called
//
// void ForwardCaloNtuplizer::Print(const std::string &what) const
// Called from the command line - useful to print information when you need it
//
//____________________________________________________________________________..

#include "ForwardCaloNtuplizer.h"

#include <calobase/TowerInfoDefs.h>
#include <caloreco/CaloWaveformFitting.h>

#include <calobase/TowerInfoDefs.h>
#include <caloreco/CaloWaveformFitting.h>
#include <ffarawobjects/CaloPacketContainerv1.h>
#include <ffarawobjects/CaloPacketv1.h>
#include <ffarawobjects/Gl1Packetv2.h>
#include <fun4all/Fun4AllReturnCodes.h>

#include <Event/packet.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>   // for PHIODataNode
#include <phool/PHNodeIterator.h> // for PHNodeIterator
#include <phool/PHObject.h>       // for PHObject
#include <phool/getClass.h>
#include <phool/phool.h>
#include <phool/recoConsts.h>

#include <cmath>
#include <fstream>
#include <phool/PHCompositeNode.h>

R__LOAD_LIBRARY (libuspin.so)

//____________________________________________________________________________..
ForwardCaloNtuplizer::ForwardCaloNtuplizer(const std::string &name, const std::string output_directory_in, const std::string file_out_name_in, const bool &with_waveform_in, const bool &get_mbd_z_in)
:SubsysReco(name), 
output_directory(output_directory_in),
file_out_name(file_out_name_in),
with_waveform(with_waveform_in),
get_mbd_z(get_mbd_z_in),
evtID(0)
{ 
    std::cout << "ForwardCaloNtuplizer::ForwardCaloNtuplizer(const std::string &name) Calling ctor" << std::endl; 
    std::cout << std::endl;

    clear_tree_vec();

    Init_FC_unit_map();
    
    if (file_out_name.find(".root") == std::string::npos)
    {
        std::cout << "file_out_name does not contain .root, the quit the run" << std::endl;
        exit(1);
    }

    plot_filename = file_out_name.substr(0, file_out_name.find(".root")) + ".pdf";

    FC_unit_waveform2D_map.clear();

    trigger_type.clear();

    GL1PScaler_raw_vec = std::vector<std::vector<long long>>(9, std::vector<long long>(120, 0));
    GL1PScaler_live_vec = std::vector<std::vector<long long>>(9, std::vector<long long>(120, 0));
    GL1PScaler_scaled_vec = std::vector<std::vector<long long>>(9, std::vector<long long>(120, 0));
}

//____________________________________________________________________________..
ForwardCaloNtuplizer::~ForwardCaloNtuplizer() 
{ 
    std::cout << "ForwardCaloNtuplizer::~ForwardCaloNtuplizer() Calling dtor" << std::endl; 
    std::cout << std::endl;
}

void ForwardCaloNtuplizer::Init_FC_unit_map()
{
    FC_unit_map.clear();

    FC_unit_map[zdcS1_ch] = {.ch = zdcS1_ch, .name = "zdcS1", .waveform = {}, .adc = -999, .sig_T_window = ZDC_time_cut, .pos = -999};
    FC_unit_map[zdcS2_ch] = {.ch = zdcS2_ch, .name = "zdcS2", .waveform = {}, .adc = -999, .sig_T_window = ZDC_time_cut, .pos = -999};
    FC_unit_map[zdcS3_ch] = {.ch = zdcS3_ch, .name = "zdcS3", .waveform = {}, .adc = -999, .sig_T_window = ZDC_time_cut, .pos = -999};

    FC_unit_map[zdcN1_ch] = {.ch = zdcN1_ch, .name = "zdcN1", .waveform = {}, .adc = -999, .sig_T_window = ZDC_time_cut, .pos = -999};
    FC_unit_map[zdcN2_ch] = {.ch = zdcN2_ch, .name = "zdcN2", .waveform = {}, .adc = -999, .sig_T_window = ZDC_time_cut, .pos = -999};
    FC_unit_map[zdcN3_ch] = {.ch = zdcN3_ch, .name = "zdcN3", .waveform = {}, .adc = -999, .sig_T_window = ZDC_time_cut, .pos = -999};

    FC_unit_map[vetoNF_ch] = {.ch = vetoNF_ch, .name = "vetoNF", .waveform = {}, .adc = -999, .sig_T_window = veto_time_cut, .pos = -999};
    FC_unit_map[vetoNB_ch] = {.ch = vetoNB_ch, .name = "vetoNB", .waveform = {}, .adc = -999, .sig_T_window = veto_time_cut, .pos = -999};
    FC_unit_map[vetoSF_ch] = {.ch = vetoSF_ch, .name = "vetoSF", .waveform = {}, .adc = -999, .sig_T_window = veto_time_cut, .pos = -999};
    FC_unit_map[vetoSB_ch] = {.ch = vetoSB_ch, .name = "vetoSB", .waveform = {}, .adc = -999, .sig_T_window = veto_time_cut, .pos = -999};

    FC_unit_map[smdSH0_ch] = {.ch = smdSH0_ch, .name = "smdSH0", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSH1_ch] = {.ch = smdSH1_ch, .name = "smdSH1", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSH2_ch] = {.ch = smdSH2_ch, .name = "smdSH2", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSH3_ch] = {.ch = smdSH3_ch, .name = "smdSH3", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSH4_ch] = {.ch = smdSH4_ch, .name = "smdSH4", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSH5_ch] = {.ch = smdSH5_ch, .name = "smdSH5", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSH6_ch] = {.ch = smdSH6_ch, .name = "smdSH6", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};

    FC_unit_map[smdSV0_ch] = {.ch = smdSV0_ch, .name = "smdSV0", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSV1_ch] = {.ch = smdSV1_ch, .name = "smdSV1", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSV2_ch] = {.ch = smdSV2_ch, .name = "smdSV2", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSV3_ch] = {.ch = smdSV3_ch, .name = "smdSV3", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSV4_ch] = {.ch = smdSV4_ch, .name = "smdSV4", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSV5_ch] = {.ch = smdSV5_ch, .name = "smdSV5", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSV6_ch] = {.ch = smdSV6_ch, .name = "smdSV6", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};
    FC_unit_map[smdSV7_ch] = {.ch = smdSV7_ch, .name = "smdSV7", .waveform = {}, .adc = -999, .sig_T_window = smdS_time_cut, .pos = -999};

    FC_unit_map[smdNH0_ch] = {.ch = smdNH0_ch, .name = "smdNH0", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNH1_ch] = {.ch = smdNH1_ch, .name = "smdNH1", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNH2_ch] = {.ch = smdNH2_ch, .name = "smdNH2", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNH3_ch] = {.ch = smdNH3_ch, .name = "smdNH3", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNH4_ch] = {.ch = smdNH4_ch, .name = "smdNH4", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNH5_ch] = {.ch = smdNH5_ch, .name = "smdNH5", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNH6_ch] = {.ch = smdNH6_ch, .name = "smdNH6", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};

    FC_unit_map[smdNV0_ch] = {.ch = smdNV0_ch, .name = "smdNV0", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNV1_ch] = {.ch = smdNV1_ch, .name = "smdNV1", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNV2_ch] = {.ch = smdNV2_ch, .name = "smdNV2", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNV3_ch] = {.ch = smdNV3_ch, .name = "smdNV3", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNV4_ch] = {.ch = smdNV4_ch, .name = "smdNV4", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNV5_ch] = {.ch = smdNV5_ch, .name = "smdNV5", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNV6_ch] = {.ch = smdNV6_ch, .name = "smdNV6", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};
    FC_unit_map[smdNV7_ch] = {.ch = smdNV7_ch, .name = "smdNV7", .waveform = {}, .adc = -999, .sig_T_window = smdN_time_cut, .pos = -999};    
}

//____________________________________________________________________________..
int ForwardCaloNtuplizer::Init(PHCompositeNode *topNode)
{
    std::cout << "ForwardCaloNtuplizer::Init(PHCompositeNode *topNode) Initializing" << std::endl;
    std::cout << std::endl;

    for (auto pair : FC_unit_map)
    {
        FC_unit_waveform2D_map.insert({
            pair.first,
            new TH2F(Form("%s",pair.second.name.c_str()),Form("%s;sample;adc count",pair.second.name.c_str()), 16,-0.5, 15.5, 200, 0, 16000)
        });
        FC_unit_waveform2D_map[pair.first] -> SetStats(0);
    }

    // Create output file
    file_out = new TFile(Form("%s/%s",output_directory.c_str(), file_out_name.c_str()), "RECREATE");
    tree_out = new TTree("tree", "tree");

    trigger_input_vec.clear();
    live_trigger_vec.clear();
    scaled_trigger_vec.clear();

    // note : for ZDC
    GL1Scalers_map[1] = {.index = 1, .name = "ZDCS", .raw = -999, .live = -999, .scaled = -999};
    GL1Scalers_map[2] = {.index = 2, .name = "ZDCN", .raw = -999, .live = -999, .scaled = -999};
    GL1Scalers_map[3] = {.index = 3, .name = "ZDCNS", .raw = -999, .live = -999, .scaled = -999};

    // note : for the GL1 clock
    GL1Scalers_map[5] = {.index = 5, .name = "clock", .raw = -999, .live = -999, .scaled = -999};
    
    // note : for MBD
    GL1Scalers_map[8] = {.index = 8, .name = "MBDS", .raw = -999, .live = -999, .scaled = -999};
    GL1Scalers_map[9] = {.index = 9, .name = "MBDN", .raw = -999, .live = -999, .scaled = -999};
    GL1Scalers_map[10] = {.index = 10, .name = "MBDNS", .raw = -999, .live = -999, .scaled = -999};
    
    tree_out -> Branch("evtID", &evtID);
    tree_out -> Branch("evtBCO_gl1", &evtBCO_gl1);
    tree_out -> Branch("evtBCO_zdc", &evtBCO_zdc);
    tree_out -> Branch("bunchnumber", &bunchnumber);
    tree_out -> Branch("mbd_z_vtx", &mbd_z_vtx);
    

    tree_out -> Branch("GTMBusyVector_Decimal", &GTMBusyVector_Decimal);
    tree_out -> Branch("TriggerInput_Decimal", &trigger_input_decimal);
    tree_out -> Branch("LiveTrigger_Decimal", &live_trigger_decimal);
    tree_out -> Branch("ScaledTrigger_Decimal", &scaled_trigger_decimal);

    tree_out -> Branch("TriggerInput_Vec", &trigger_input_vec);
    tree_out -> Branch("LiveTrigger_Vec", &live_trigger_vec);
    tree_out -> Branch("ScaledTrigger_Vec", &scaled_trigger_vec);

    tree_out -> Branch("zdcS1_adc", &FC_unit_map[zdcS1_ch].adc);
    tree_out -> Branch("zdcS2_adc", &FC_unit_map[zdcS2_ch].adc);
    tree_out -> Branch("zdcS3_adc", &FC_unit_map[zdcS3_ch].adc);
    tree_out -> Branch("zdcN1_adc", &FC_unit_map[zdcN1_ch].adc);
    tree_out -> Branch("zdcN2_adc", &FC_unit_map[zdcN2_ch].adc);
    tree_out -> Branch("zdcN3_adc", &FC_unit_map[zdcN3_ch].adc);
    tree_out -> Branch("vetoNF_adc", &FC_unit_map[vetoNF_ch].adc);
    tree_out -> Branch("vetoNB_adc", &FC_unit_map[vetoNB_ch].adc);
    tree_out -> Branch("vetoSF_adc", &FC_unit_map[vetoSF_ch].adc);
    tree_out -> Branch("vetoSB_adc", &FC_unit_map[vetoSB_ch].adc);

    tree_out -> Branch("smdSH_adc", &smdSH_adc); // note : vector
    tree_out -> Branch("smdSH_pos", &smdSH_pos); // note : vector
    tree_out -> Branch("smdNH_adc", &smdNH_adc); // note : vector
    tree_out -> Branch("smdNH_pos", &smdNH_pos); // note : vector
    tree_out -> Branch("smdSV_adc", &smdSV_adc); // note : vector
    tree_out -> Branch("smdSV_pos", &smdSV_pos); // note : vector
    tree_out -> Branch("smdNV_adc", &smdNV_adc); // note : vector
    tree_out -> Branch("smdNV_pos", &smdNV_pos); // note : vector

    for (auto &GL1pair : GL1Scalers_map)
    {
        tree_out -> Branch(Form("GL1Scalers_%s_raw",GL1pair.second.name.c_str()), &GL1pair.second.raw);
        tree_out -> Branch(Form("GL1Scalers_%s_live",GL1pair.second.name.c_str()), &GL1pair.second.live);
        tree_out -> Branch(Form("GL1Scalers_%s_scaled",GL1pair.second.name.c_str()), &GL1pair.second.scaled);
    }

    if (with_waveform == true)
    {
        for (auto &unit : FC_unit_map)
        {
            tree_out -> Branch(Form("%s_waveform",unit.second.name.c_str()), &unit.second.waveform);
        }
    }

    bunchnumber_1D = new TH1F("bunchnumber_1D", "bunchnumber_1D;bunch number;Entry",150,0,150);

    c1 = new TCanvas("c1", "c1", 950, 800);
    c1 -> cd();
    c1 -> Print(Form("%s/%s(",output_directory.c_str(),plot_filename.c_str()));

    return Fun4AllReturnCodes::EVENT_OK;
}

void ForwardCaloNtuplizer::clear_tree_vec()
{
    smdSH_adc.clear();
    smdSH_pos.clear();
    smdNH_adc.clear();
    smdNH_pos.clear();
    smdSV_adc.clear();
    smdSV_pos.clear();
    smdNV_adc.clear();
    smdNV_pos.clear();

    return;
}

void ForwardCaloNtuplizer::clear_FC_unit_map()
{
    for (auto &unit : FC_unit_map)
    {
        unit.second.adc = -999;
        unit.second.waveform.clear();
        unit.second.pos = -999;
    }

    return;
}

//____________________________________________________________________________..
int ForwardCaloNtuplizer::InitRun(PHCompositeNode *topNode)
{
    std::cout << "ForwardCaloNtuplizer::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
    std::cout << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int ForwardCaloNtuplizer::process_event(PHCompositeNode *topNode)
{
    if(evtID == 0) {
        std::cout << "ForwardCaloNtuplizer::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
        std::cout << std::endl;
    }
    
    if (evtID % 10000 == 0) {
        std::cout << "Event ID: " << evtID << std::endl;
        std::cout << std::endl;
    }

    p_gl1 = findNode::getClass<Gl1Packetv2>(topNode, /*"GL1RAWHIT"*/ /**/ "GL1Packet"); // note : for the selfgen DST, it may be the "GL1RAWHIT"
    // zdc_cont = findNode::getClass<CaloPacketContainerv1>(topNode, "ZDCPackets");

    bunchnumber = -999;
    evtBCO_gl1 = -999;
    evtBCO_zdc = -999;

    GTMBusyVector_Decimal = -999;
    trigger_input_decimal = -999;
    live_trigger_decimal = -999;
    scaled_trigger_decimal = -999;
    mbd_z_vtx = -999;

    for (auto &GL1pair : GL1Scalers_map)
    {
        GL1pair.second.raw = -999;
        GL1pair.second.live = -999;
        GL1pair.second.scaled = -999;
    }

    if (p_gl1)
    {
        bunchnumber = p_gl1->getBunchNumber();
        evtBCO_gl1 = p_gl1->getBCO();

        GTMBusyVector_Decimal = p_gl1->lValue(0, "GTMBusyVector");
        trigger_input_decimal = p_gl1->lValue(0, "TriggerInput");
        live_trigger_decimal = p_gl1->lValue(0,"LiveVector");
        scaled_trigger_decimal = p_gl1->lValue(0,"ScaledVector");

        trigger_input_vec = ForwardCaloNtuplizer::prepare_trigger_vec(trigger_input_decimal);
        live_trigger_vec = ForwardCaloNtuplizer::prepare_trigger_vec(live_trigger_decimal);
        scaled_trigger_vec = ForwardCaloNtuplizer::prepare_trigger_vec(scaled_trigger_decimal);

        if (get_mbd_z){
            prepare_mbd_z(topNode);
            if (evtID % 10000 == 0)
            {
                std::cout<<"reco. MBD vertex Z : "<<mbd_z_vtx<<" cm "<<std::endl;
            }
        }

        // for (int i = 0; i < 16; i++) { std::cout<<p_gl1->lValue(i, "GL1PRAW")<<" "; }
        // std::cout<<std::endl;

        // for (int i = 0; i < 16; i++) { std::cout<<p_gl1->lValue(i, "GL1PLIVE")<<" "; }
        // std::cout<<std::endl;

        // for (int i = 0; i < 16; i++) { std::cout<<p_gl1->lValue(i, "GL1PSCALED")<<" "; }
        // std::cout<<std::endl;

        for (auto &GL1pair : GL1Scalers_map)
        {
            if (p_gl1->lValue(GL1pair.second.index, 0) || p_gl1->lValue(GL1pair.second.index, 1) || p_gl1->lValue(GL1pair.second.index, 2))
            {
                GL1pair.second.raw = p_gl1->lValue(GL1pair.second.index, 0);
                GL1pair.second.live = p_gl1->lValue(GL1pair.second.index, 1);
                GL1pair.second.scaled = p_gl1->lValue(GL1pair.second.index, 2);
                // std::cout<<"detail test : "<<GL1pair.second.index<<" "<<GL1pair.second.raw<<" "<<GL1pair.second.live<<" "<<GL1pair.second.scaled<<" "<<p_gl1->lValue(GL1pair.second.index, 0)<<" "<<p_gl1->lValue(GL1pair.second.index, 1)<<" "<<p_gl1->lValue(GL1pair.second.index, 2)<<std::endl;
            }
        }

        // note : this is for the GL1P scalers
        for (int i = 0; i < int(GL1PScaler_raw_vec.size()); i++)
        {
            if (p_gl1->lValue(i, "GL1PRAW") || p_gl1->lValue(i, "GL1PLIVE") || p_gl1->lValue(i, "GL1PSCALED"))
            {
                GL1PScaler_raw_vec[i][bunchnumber] = p_gl1->lValue(i, "GL1PRAW");
                GL1PScaler_live_vec[i][bunchnumber] = p_gl1->lValue(i, "GL1PLIVE");
                GL1PScaler_scaled_vec[i][bunchnumber] = p_gl1->lValue(i, "GL1PSCALED");
            }
        }

        if (evtID % 1000 == 0)
        {
            std::cout<<"====================================================================================="<<std::endl;
            p_gl1->dump();
            std::cout<<"-----------------------------------------------------------------------------------------------"<<std::endl;
            for (int i = 0; i < int(GL1PScaler_raw_vec.size()); i++)
            {
                std::cout << "GL1P_scalers : "
                   << std::setw(18)<< std::setprecision(15) << std::accumulate( GL1PScaler_raw_vec[i].begin(), GL1PScaler_raw_vec[i].end(), 0.0 )      <<" "
                   << std::setw(18)<< std::setprecision(15) << std::accumulate( GL1PScaler_live_vec[i].begin(), GL1PScaler_live_vec[i].end(), 0.0 )    <<" "
                   << std::setw(18)<< std::setprecision(15) << std::accumulate( GL1PScaler_scaled_vec[i].begin(), GL1PScaler_scaled_vec[i].end(), 0.0 )<<" -----> "
                   << std::setw(18)<< std::setprecision(15) << p_gl1->lValue(gl1p_gl1_scaler_map[i], 0)<<
                   std::endl;
            }
        }

        // note : to check how many types of trigger inputs we have 
        if (trigger_type.find(p_gl1->lValue(0, "TriggerInput")) == trigger_type.end())
        {
            trigger_type.insert({p_gl1->lValue(0, "TriggerInput"), 1});
        }
        else
        {
            trigger_type[p_gl1->lValue(0, "TriggerInput")] += 1;
        }

        // if (evtID % 100 == 0)
        // {
        //     std::cout << "Trigger Type : " << p_gl1->lValue(0, "TriggerInput") << std::endl;
        // }

        // if (evtID % 1000 == 0)
        // {
        //     std::cout <<"-------------> "<< bunchnumber << std::endl;
        //     std::cout <<"-------------> "<< evtBCO_gl1 << std::endl;
        // }
        // if (zdc_cont->get_npackets() != 1)
        // {
        //     std::cout << "zdc_cont->get_npackets() != 1" << std::endl;

        //     tree_out->Fill();

        //     clear_tree_vec();
        //     clear_FC_unit_map();
        //     evtID += 1;
        //     return Fun4AllReturnCodes::EVENT_OK;
        // }

        // CaloPacket *p_zdc = zdc_cont->getPacket(0);

        // if (p_zdc)
        // {

        //     bunchnumber_1D -> Fill(bunchnumber);

        //     evtBCO_zdc = p_zdc->getBCO();

        //     // std::cout<<"N channel? "<<p_zdc->iValue(0, "CHANNELS")<<std::endl;

        //     for (int c = 0; c < p_zdc->iValue(0, "CHANNELS"); c++)
        //     {
        //         if (with_waveform == true && FC_unit_map.find(c) != FC_unit_map.end())
        //         {
        //             double baseline_left = 0.;
        //             double baseline_right = 0.;

        //             for(int s = 0; s < 3; s++) { baseline_left += p_zdc->iValue(s, c); }
        //             for (int s = p_zdc->iValue(0, "SAMPLES")-3; s < p_zdc->iValue(0, "SAMPLES"); s++) { baseline_right += p_zdc->iValue(s,c); }

        //             baseline_left /= 3.;
        //             baseline_right /=3.;
        //             // double baseline = (baseline_left < baseline_right) ? baseline_left : baseline_right;
        //             double baseline = 0;

        //             for (int s = 0; s < p_zdc->iValue(0, "SAMPLES"); s++)
        //             {
        //                 double single_sample_adc = p_zdc->iValue(s, c) - baseline;

        //                 FC_unit_map[c].waveform.push_back(single_sample_adc);
        //                 FC_unit_waveform2D_map[c] -> Fill(s, single_sample_adc);
        //                 // std::cout<< c <<" "<< p_zdc->iValue(s, c) << std::endl;
        //             }
                    
        //             // std::cout<<p_zdc->iValue(0, "SAMPLES")<<std::endl;
        //         }
        //         // std::cout<<std::endl;
                
        //         if (FC_unit_map.find(c) != FC_unit_map.end())
        //         {
        //             std::vector<float> resultFast = anaWaveformFast(p_zdc, c);  // note: fast waveform fitting
        //             float signalFast = resultFast.at(0);
        //             float timingFast = resultFast.at(1);
        //             // float pedFast = resultFast.at(2);

        //             if (FC_unit_map[c].sig_T_window.first < timingFast && timingFast < FC_unit_map[c].sig_T_window.second)
        //             {
        //                 FC_unit_map[c].adc = signalFast;
        //             }
        //         }
        //     }
        // }
        // else 
        // {
        //     std::cout << "p_zdc is not good" << std::endl;
        // }

    }// note : end if p_gl1 good
    else 
    {
        std::cout << "p_gl1 is not good" << std::endl;
    }

    tree_out->Fill();

    clear_tree_vec();
    clear_FC_unit_map();
    evtID += 1;
    return Fun4AllReturnCodes::EVENT_OK;
}

std::vector<int> ForwardCaloNtuplizer::prepare_trigger_vec(long long trigger_input)
{
    std::bitset<64> trigger_input_bitset(trigger_input);
    std::vector<int> output_vec; output_vec.clear();

    for (unsigned int i=0; i<64; i++)
	{
	    if (trigger_input_bitset.test(i))
	    {
            output_vec.push_back(i);
	    }
	}

    return output_vec;

}

std::vector<float> ForwardCaloNtuplizer::anaWaveformFast(CaloPacket *p, const int channel)
{
  std::vector<float> waveform;
  // Chris: preallocation = speed improvement
  waveform.reserve(p->iValue(0, "SAMPLES"));
  for (int s = 0; s < p->iValue(0, "SAMPLES"); s++)
  {
    waveform.push_back(p->iValue(s, channel));
  }
  std::vector<std::vector<float>> multiple_wfs;
  multiple_wfs.push_back(waveform);

  std::vector<std::vector<float>> fitresults_zdc;
  fitresults_zdc = WaveformProcessingFast->calo_processing_fast(multiple_wfs);

  std::vector<float> result;
  result = fitresults_zdc.at(0);
  return result;
}

void ForwardCaloNtuplizer::prepare_mbd_z(PHCompositeNode *topNode)
{
    if (Verbosity() >= VERBOSITY_MORE)
        std::cout << "Get centrality info." << std::endl;
    
    m_mbdvtxmap = findNode::getClass<MbdVertexMapv1>(topNode, "MbdVertexMap");
    if (!m_mbdvtxmap)
    {
        std::cout << "Error, can't find MbdVertexMap" << std::endl;
        exit(1);
    }

    std::cout << "MbdVertexMap size: " << m_mbdvtxmap->size() << std::endl;
    for (MbdVertexMap::ConstIter biter = m_mbdvtxmap->begin(); biter != m_mbdvtxmap->end(); ++biter)
    {
        m_mbdvtx = biter->second;
        mbd_z_vtx = m_mbdvtx->get_z();
    }

}

//____________________________________________________________________________..
int ForwardCaloNtuplizer::ResetEvent(PHCompositeNode *topNode)
{
    trigger_input_vec.clear();
    live_trigger_vec.clear();
    scaled_trigger_vec.clear();

    // std::cout<<"In ForwardCaloNtuplizer::ResetEvent(PHCompositeNode *topNode), running !!!"<<std::endl;

    if (Verbosity() > 1000) 
    {
        std::cout << "ForwardCaloNtuplizer::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
        std::cout << std::endl;
    }
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int ForwardCaloNtuplizer::EndRun(const int runnumber)
{
    std::cout << "ForwardCaloNtuplizer::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
    std::cout << std::endl;
    
    c1 -> cd();

    file_out->cd();

    for (auto &unit : FC_unit_waveform2D_map)
    {
        unit.second->Write(Form("%s",unit.second->GetName()));

        unit.second->Draw("colz0");
        c1 -> Print(Form("%s/%s",output_directory.c_str(), plot_filename.c_str()));
        c1 -> Clear();
    }

    bunchnumber_1D -> Write("bunchnumber_1D");

    c1 -> Print(Form("%s/%s)",output_directory.c_str(), plot_filename.c_str()));

    tree_out->Write();
    file_out->Close();
    
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int ForwardCaloNtuplizer::End(PHCompositeNode *topNode)
{
    std::cout << "ForwardCaloNtuplizer::End(PHCompositeNode *topNode) This is the End..." << std::endl;

    std::cout<<"------------------------------------------------------------------------------------------------------"<<std::endl;
    std::cout << "Trigger Type : " << std::endl;
    for (auto pair : trigger_type)
    {
        // std::cout << pair.first << " : " << pair.second << std::endl;
        std::cout << std::bitset<32>(pair.first); std::cout << " : " << pair.second << std::endl;
    }

    std::cout << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int ForwardCaloNtuplizer::Reset(PHCompositeNode *topNode)
{
    std::cout << "ForwardCaloNtuplizer::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
    std::cout << std::endl;
    return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void ForwardCaloNtuplizer::Print(const std::string &what) const
{
    std::cout << "ForwardCaloNtuplizer::Print(const std::string &what) const Printing info for " << what << std::endl;
    std::cout << std::endl;
}
