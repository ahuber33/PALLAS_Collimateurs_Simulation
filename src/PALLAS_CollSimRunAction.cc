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

  Tree_Collimator = new TTree("Collimator","Collimator Information");  //Tree to access Collimator information
  Tree_FrontCollimator = new TTree("FrontCollimator","FrontCollimator Information");  //Tree to access Front Collimator infos
  Tree_BackCollimator = new TTree("BackCollimator","BackCollimator Information");  //Tree to access Back Collimator infos

  //*****************************INFORMATION FROM THE COLLIMATOR**************************************
  RunBranch = Tree_Collimator->Branch("E_start", &StatsCollimator.E_start, "E_start/F" );
  RunBranch = Tree_Collimator->Branch("E_dep", &StatsCollimator.E_dep, "E_dep/F" );
  RunBranch = Tree_Collimator->Branch("E_dep_e", &StatsCollimator.E_dep_e, "E_dep_e/F" );
  RunBranch = Tree_Collimator->Branch("E_dep_g", &StatsCollimator.E_dep_g, "E_dep_g/F" );
  RunBranch = Tree_Collimator->Branch("E_gamma_Brem", "vector<float>" , &StatsCollimator.E_gamma_Brem);

  //*****************************INFORMATION FROM THE FRONT SURFACE**************************************
  RunBranch = Tree_FrontCollimator->Branch("ParticuleID", "vector<int>" , &StatsFrontCollimator.ParticuleID);
  RunBranch = Tree_FrontCollimator->Branch("E_exit", "vector<float>" , &StatsFrontCollimator.E_exit);
  RunBranch = Tree_FrontCollimator->Branch("x_exit", "vector<float>" , &StatsFrontCollimator.x_exit);
  RunBranch = Tree_FrontCollimator->Branch("y_exit", "vector<float>" , &StatsFrontCollimator.y_exit);
  RunBranch = Tree_FrontCollimator->Branch("z_exit", "vector<float>" , &StatsFrontCollimator.z_exit);
  RunBranch = Tree_FrontCollimator->Branch("px_exit", "vector<float>" , &StatsFrontCollimator.px_exit);
  RunBranch = Tree_FrontCollimator->Branch("py_exit", "vector<float>" , &StatsFrontCollimator.py_exit);
  RunBranch = Tree_FrontCollimator->Branch("pz_exit", "vector<float>" , &StatsFrontCollimator.pz_exit);

  //*****************************INFORMATION FROM THE BACK SURFACE**************************************
  RunBranch = Tree_BackCollimator->Branch("ParticuleID", "vector<int>" , &StatsBackCollimator.ParticuleID);
  RunBranch = Tree_BackCollimator->Branch("E_exit", "vector<float>" , &StatsBackCollimator.E_exit);
  RunBranch = Tree_BackCollimator->Branch("x_exit", "vector<float>" , &StatsBackCollimator.x_exit);
  RunBranch = Tree_BackCollimator->Branch("y_exit", "vector<float>" , &StatsBackCollimator.y_exit);
  RunBranch = Tree_BackCollimator->Branch("z_exit", "vector<float>" , &StatsBackCollimator.z_exit);
  RunBranch = Tree_BackCollimator->Branch("px_exit", "vector<float>" , &StatsBackCollimator.px_exit);
  RunBranch = Tree_BackCollimator->Branch("py_exit", "vector<float>" , &StatsBackCollimator.py_exit);
  RunBranch = Tree_BackCollimator->Branch("pz_exit", "vector<float>" , &StatsBackCollimator.pz_exit);


  //set the random seed to the CPU clock
  //G4Random::setTheEngine(new CLHEP::HepJamesRandom);
  G4long seed = time(NULL);
  //G4Random::setTheSeed(seed);
  G4Random::setTheSeed(1712317304);
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
  Tree_Collimator->Write();
  Tree_FrontCollimator->Write();
  Tree_BackCollimator->Write();
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

void PALLAS_CollSimRunAction::UpdateStatisticsCollimator(RunTallyCollimator aRunTallyCollimator){
  StatsCollimator = aRunTallyCollimator;
  Tree_Collimator->Fill();
}

void PALLAS_CollSimRunAction::UpdateStatisticsFrontCollimator(RunTallyFrontCollimator aRunTallyFrontCollimator){
  StatsFrontCollimator = aRunTallyFrontCollimator;
  Tree_FrontCollimator->Fill();
}

void PALLAS_CollSimRunAction::UpdateStatisticsBackCollimator(RunTallyBackCollimator aRunTallyBackCollimator){
  StatsBackCollimator = aRunTallyBackCollimator;
  Tree_BackCollimator->Fill();
}
