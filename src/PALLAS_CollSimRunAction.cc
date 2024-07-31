/// PALLAS_CollSimRunAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimRunAction.hh"
std::atomic<int> PALLAS_CollSimRunAction::activeThreads(0);
G4Mutex PALLAS_CollSimRunAction::fileMutex = G4MUTEX_INITIALIZER;

PALLAS_CollSimRunAction::PALLAS_CollSimRunAction(const char* suff):suffixe(suff)
{}

PALLAS_CollSimRunAction::~PALLAS_CollSimRunAction()
{}

//-----------------------------------------------------
//  BeginOfRunAction:  used to calculate the start time and
//  to set up information in the run tree.
//-----------------------------------------------------
void PALLAS_CollSimRunAction::BeginOfRunAction(const G4Run* aRun){

  G4AutoLock lock(&fileMutex); // Verrouillage automatique du mutex

  start = time(NULL);     //start the timer clock to calculate run times

  int a=activeThreads;
  G4String s = std::to_string(a);
  
  G4String fileName = suffixe + "_" +s+".root";
  G4cout << "Filename = " << fileName << G4endl;
  f = new TFile(fileName.c_str(),"RECREATE");

  Tree_Input = new TTree("Input","Input Information");  //Tree to access Collimator information
  Tree_Collimator = new TTree("Collimator","Collimator Information");  //Tree to access Collimator information
  Tree_FrontCollimator = new TTree("FrontCollimator","FrontCollimator Information");  //Tree to access Front Collimator infos
  Tree_BackCollimator = new TTree("BackCollimator","BackCollimator Information");  //Tree to access Back Collimator infos
  Tree_BSYAG = new TTree("BSYAG","BS YAG Information");  //Tree to access Back Collimator infos
  Tree_BSPECYAG = new TTree("BSPECYAG","BSPEC YAG Information");  //Tree to access Back Collimator infos

  //*****************************INFORMATION FROM THE INPUT*******************************************
  RunBranch = Tree_Input->Branch("x", &StatsInput.x, "x/F" );
  RunBranch = Tree_Input->Branch("xoffset", &StatsInput.xoffset, "xoffset/F" );
  RunBranch = Tree_Input->Branch("xp", &StatsInput.xp, "xp/F" );
  RunBranch = Tree_Input->Branch("y", &StatsInput.y, "y/F" );
  RunBranch = Tree_Input->Branch("yoffset", &StatsInput.yoffset, "yoffset/F" );
  RunBranch = Tree_Input->Branch("yp", &StatsInput.yp, "yp/F" );
  RunBranch = Tree_Input->Branch("s", &StatsInput.s, "s/F" );
  RunBranch = Tree_Input->Branch("soffset", &StatsInput.soffset, "soffset/F" );
  RunBranch = Tree_Input->Branch("p", &StatsInput.p, "p/F" );
  RunBranch = Tree_Input->Branch("delta", &StatsInput.delta, "delta/F" );
  RunBranch = Tree_Input->Branch("energy", &StatsInput.energy, "energy/F" );
  RunBranch = Tree_Input->Branch("Nevent", &StatsInput.Nevent, "Nevent/I" );

  //*****************************INFORMATION FROM THE COLLIMATOR**************************************
  RunBranch = Tree_Collimator->Branch("E_start", &StatsCollimator.E_start, "E_start/F" );
  RunBranch = Tree_Collimator->Branch("E_dep", &StatsCollimator.E_dep, "E_dep/F" );
  RunBranch = Tree_Collimator->Branch("E_dep_e", &StatsCollimator.E_dep_e, "E_dep_e/F" );
  RunBranch = Tree_Collimator->Branch("E_dep_g", &StatsCollimator.E_dep_g, "E_dep_g/F" );
  RunBranch = Tree_Collimator->Branch("Energy_Brem_created", "vector<float>" , &StatsCollimator.Energy_Brem_created);

  //*****************************INFORMATION FROM THE FRONT SURFACE**************************************
  RunBranch = Tree_FrontCollimator->Branch("ParticleID", "vector<int>" , &StatsFrontCollimator.particleID);
  RunBranch = Tree_FrontCollimator->Branch("ParentID", "vector<int>" , &StatsFrontCollimator.parentID);
  RunBranch = Tree_FrontCollimator->Branch("E_exit", "vector<float>" , &StatsFrontCollimator.E_exit);
  RunBranch = Tree_FrontCollimator->Branch("x_exit", "vector<float>" , &StatsFrontCollimator.x_exit);
  RunBranch = Tree_FrontCollimator->Branch("y_exit", "vector<float>" , &StatsFrontCollimator.y_exit);
  RunBranch = Tree_FrontCollimator->Branch("z_exit", "vector<float>" , &StatsFrontCollimator.z_exit);
  RunBranch = Tree_FrontCollimator->Branch("px_exit", "vector<float>" , &StatsFrontCollimator.px_exit);
  RunBranch = Tree_FrontCollimator->Branch("py_exit", "vector<float>" , &StatsFrontCollimator.py_exit);
  RunBranch = Tree_FrontCollimator->Branch("pz_exit", "vector<float>" , &StatsFrontCollimator.pz_exit);

  //*****************************INFORMATION FROM THE BACK SURFACE**************************************
  RunBranch = Tree_BackCollimator->Branch("ParticleID", "vector<int>" , &StatsBackCollimator.particleID);
  RunBranch = Tree_BackCollimator->Branch("ParentID", "vector<int>" , &StatsBackCollimator.parentID);
  RunBranch = Tree_BackCollimator->Branch("E_exit", "vector<float>" , &StatsBackCollimator.E_exit);
  RunBranch = Tree_BackCollimator->Branch("x_exit", "vector<float>" , &StatsBackCollimator.x_exit);
  RunBranch = Tree_BackCollimator->Branch("y_exit", "vector<float>" , &StatsBackCollimator.y_exit);
  RunBranch = Tree_BackCollimator->Branch("z_exit", "vector<float>" , &StatsBackCollimator.z_exit);
  RunBranch = Tree_BackCollimator->Branch("px_exit", "vector<float>" , &StatsBackCollimator.px_exit);
  RunBranch = Tree_BackCollimator->Branch("py_exit", "vector<float>" , &StatsBackCollimator.py_exit);
  RunBranch = Tree_BackCollimator->Branch("pz_exit", "vector<float>" , &StatsBackCollimator.pz_exit);

  //************************************INFORMATION FROM THE BS YAG*****************************************
  RunBranch = Tree_BSYAG->Branch("x_exit", "vector<float>" , &StatsBSYAG.x_exit);
  RunBranch = Tree_BSYAG->Branch("y_exit", "vector<float>" , &StatsBSYAG.y_exit);
  RunBranch = Tree_BSYAG->Branch("z_exit", "vector<float>" , &StatsBSYAG.z_exit);
  RunBranch = Tree_BSYAG->Branch("parentID", "vector<float>" , &StatsBSYAG.parentID);
  RunBranch = Tree_BSYAG->Branch("energy", "vector<float>" , &StatsBSYAG.energy);

//************************************INFORMATION FROM THE BSPEC YAG*****************************************
  RunBranch = Tree_BSPECYAG->Branch("x_exit", "vector<float>" , &StatsBSPECYAG.x_exit);
  RunBranch = Tree_BSPECYAG->Branch("y_exit", "vector<float>" , &StatsBSPECYAG.y_exit);
  RunBranch = Tree_BSPECYAG->Branch("z_exit", "vector<float>" , &StatsBSPECYAG.z_exit);
  RunBranch = Tree_BSPECYAG->Branch("parentID", "vector<float>" , &StatsBSPECYAG.parentID);
  RunBranch = Tree_BSPECYAG->Branch("energy", "vector<float>" , &StatsBSPECYAG.energy);




  //set the random seed to the CPU clock
  //G4Random::setTheEngine(new CLHEP::HepJamesRandom);
  G4long seed = time(NULL) + a;
  G4Random::setTheSeed(seed);
  //G4Random::setTheSeed(1712670533);
  G4cout << "seed = " << seed << G4endl;

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  if (G4VVisManager::GetConcreteInstance()){
    G4UImanager* UI = G4UImanager::GetUIpointer();
    UI->ApplyCommand("/vis/scene/notifyHandlers");
  }

  activeThreads++;

}  //end BeginOfRunAction


//-----------------------------------------------------
//  EndOfRunAction:  used to calculate the end time and
//  to write information to the run tree.
//-----------------------------------------------------
void PALLAS_CollSimRunAction::EndOfRunAction(const G4Run*aRun){
  G4AutoLock lock(&fileMutex); // Verrouillage automatique du mutex
  f->cd();
  //Tree_Input->Write();
  Tree_Collimator->Write();
  //Tree_FrontCollimator->Write();
  //Tree_BackCollimator->Write();
  Tree_BSYAG->Write();
  Tree_BSPECYAG->Write();
  f->Close();
  delete f;
  f=nullptr;

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

void PALLAS_CollSimRunAction::UpdateStatisticsInput(RunTallyInput aRunTallyInput){
  std::lock_guard<std::mutex> lock(fileMutex);
  StatsInput = aRunTallyInput;
  if (Tree_Input)Tree_Input->Fill();
  else{
    G4cerr << "Error : Tree_Input is nullptr" << G4endl;
  }
}

void PALLAS_CollSimRunAction::UpdateStatisticsCollimator(RunTallyCollimator aRunTallyCollimator){
  std::lock_guard<std::mutex> lock(fileMutex);
  StatsCollimator = aRunTallyCollimator;
  if (Tree_Collimator) Tree_Collimator->Fill();
  else{
    G4cerr << "Error : Tree_Collimator is nullptr" << G4endl;
  }
}

void PALLAS_CollSimRunAction::UpdateStatisticsFrontCollimator(RunTallyFrontCollimator aRunTallyFrontCollimator){
  std::lock_guard<std::mutex> lock(fileMutex);
  StatsFrontCollimator = aRunTallyFrontCollimator;
  if (Tree_FrontCollimator) Tree_FrontCollimator->Fill();
  else{
    G4cerr << "Error : Tree_FrontCollimator is nullptr" << G4endl;
  }
}

void PALLAS_CollSimRunAction::UpdateStatisticsBackCollimator(RunTallyBackCollimator aRunTallyBackCollimator){
  std::lock_guard<std::mutex> lock(fileMutex);
  StatsBackCollimator = aRunTallyBackCollimator;
  if (Tree_BackCollimator) Tree_BackCollimator->Fill();
  else{
    G4cerr << "Error : Tree_BackCollimator is nullptr" << G4endl;
  }
}

void PALLAS_CollSimRunAction::UpdateStatisticsBSYAG(RunTallyBSYAG aRunTallyBSYAG){
  std::lock_guard<std::mutex> lock(fileMutex);
  StatsBSYAG = aRunTallyBSYAG;
  if (Tree_BSYAG) Tree_BSYAG->Fill();
  else{
    G4cerr << "Error : Tree_BSYAG is nullptr" << G4endl;
  }
}

void PALLAS_CollSimRunAction::UpdateStatisticsBSPECYAG(RunTallyBSPECYAG aRunTallyBSPECYAG){
  std::lock_guard<std::mutex> lock(fileMutex);
  StatsBSPECYAG = aRunTallyBSPECYAG;
  if (Tree_BSPECYAG) Tree_BSPECYAG->Fill();
  else{
    G4cerr << "Error : Tree_BSPECYAG is nullptr" << G4endl;
  }
}