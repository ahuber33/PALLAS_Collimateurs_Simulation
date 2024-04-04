/// PALLAS_CollSimRunAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimRunAction.hh"


PALLAS_CollSimRunAction::PALLAS_CollSimRunAction(char* suff):suffixe(suff){}
PALLAS_CollSimRunAction::~PALLAS_CollSimRunAction(){}


//-----------------------------------------------------
//  BeginOfRunAction:  used to calculate the start time and
//  to set up information in the run tree.
//-----------------------------------------------------
void PALLAS_CollSimRunAction::BeginOfRunAction(const G4Run* aRun){

  G4String fileName = suffixe+".root";

  start = time(NULL);     //start the timer clock to calculate run times

  Tree_TP = new TTree("TP","TP Information");  //Tree to access position of energy deposition

  //*****************************INFORMATION FROM THE TP**************************************
  //RunBranch = Tree_TP->Branch("ParticuleID", &StatsTP.ParticuleID, "ParticuleID/I" );
  RunBranch = Tree_TP->Branch("E_start", &StatsTP.E_start, "E_start/F" );
  RunBranch = Tree_TP->Branch("E_dep", &StatsTP.E_dep, "E_dep/F" );
  //RunBranch = Tree_TP->Branch("Charge", &StatsTP.Charge, "Charge/F" );
  RunBranch = Tree_TP->Branch("PositionX", &StatsTP.PositionX, "PositionX/F" );
  RunBranch = Tree_TP->Branch("PositionY", &StatsTP.PositionY, "PositionY/F" );
  RunBranch = Tree_TP->Branch("PositionZ", &StatsTP.PositionZ, "PositionZ/F" );
  //RunBranch = Tree_TP->Branch("Time", &StatsTP.Time, "Time/F" );
  //RunBranch = Tree_TP->Branch("TotalLength", &StatsTP.TotalLength, "TotalLength/F" );
  //RunBranch = Tree_TP->Branch("InteractionDepth", &StatsTP.InteractionDepth, "InteractionDepth/F" );



  //set the random seed to the CPU clock
  //G4Random::setTheEngine(new CLHEP::HepJamesRandom);
  G4long seed = time(NULL);
  G4Random::setTheSeed(seed);
  //G4Random::setTheSeed(1655805950);
  G4cout << "seed = " << seed << G4endl;

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  if (G4VVisManager::GetConcreteInstance()){
    G4UImanager* UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/vis/scene/notifyHandlers");
  }


}  //end BeginOfRunAction


//-----------------------------------------------------
//  EndOfRunAction:  used to calculate the end time and
//  to write information to the run tree.
//-----------------------------------------------------
void PALLAS_CollSimRunAction::EndOfRunAction(const G4Run*aRun){

  //update the temp root file
  G4String fileName = suffixe+".root";
  f = new TFile(fileName,"update");
  Tree_TP->Write();
  f->Close();

  if (G4VVisManager::GetConcreteInstance()){
    G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
  }

  //display run time and write to file Rntime.out
  time_t end = time(NULL);
  G4int elapsed = end-start;
  G4cout << "Run Completed in " << elapsed/3600
  << ":" << (elapsed%3600)/60 << ":"
  << ((elapsed%3600)%60) << G4endl;

  // Output the time in the file Runtime.out
  std::ofstream timeout;
  timeout.open("Runtime.out",std::ios::app);
  timeout << "Run " << aRun->GetRunID()
  << ": " <<elapsed/3600
  << ":" <<(elapsed%3600)/60
  << ":" <<((elapsed%3600)%60) << G4endl;
  timeout.close();

  G4cout<<"Leaving Run Action"<<G4endl;
}

//---------------------------------------------------------
//  For each event update the statistics in the Run tree
//---------------------------------------------------------

void PALLAS_CollSimRunAction::UpdateStatisticsTP(RunTallyTP aRunTallyTP){
  StatsTP = aRunTallyTP;
  Tree_TP->Fill();
}
