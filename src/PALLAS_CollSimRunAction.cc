/// PALLAS_CollSimRunAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimRunAction.hh"
std::atomic<int> PALLAS_CollSimRunAction::activeThreads(0);
G4Mutex PALLAS_CollSimRunAction::fileMutex = G4MUTEX_INITIALIZER;

PALLAS_CollSimRunAction::PALLAS_CollSimRunAction(const char* suff, G4bool pMT)
                                                :suffixe(suff),
                                                flag_MT(pMT)
{}

PALLAS_CollSimRunAction::~PALLAS_CollSimRunAction()
{}

//-----------------------------------------------------
//  BeginOfRunAction:  used to calculate the start time and
//  to set up information in the run tree.
//-----------------------------------------------------
void PALLAS_CollSimRunAction::BeginOfRunAction(const G4Run* aRun){

  G4AutoLock lock(&fileMutex); // Verrouillage automatique du mutex

  StatsHorizontalCollGlobal = {};
  StatsVerticalCollGlobal = {};

  start = time(NULL);     //start the timer clock to calculate run times

  int a=activeThreads;

  if (flag_MT == true) 
  {
    G4String s = std::to_string(a);
    fileName = suffixe + "_" +s+".root";
  }

  else 
  {
    fileName = suffixe +".root";
  }

  G4cout << "Filename = " << fileName << G4endl;

  f = new TFile(fileName.c_str(),"RECREATE");

  Tree_Input = new TTree("Input","Input Information");  //Tree to access Input information
  Tree_HorizontalCollGlobal = new TTree("Horizontal_CollGlobal","Horizontal Collimator Global Information");  //Tree to access GLOBAL Horizontal Collimator information
  Tree_VerticalCollGlobal = new TTree("Vertical_CollGlobal","Vertical Collimator Global Information");  //Tree to access GLLOBAL Vertical Collimator information
  Tree_HorizontalColl = new TTree("Horizontal_Coll","Horizontal Collimator Information");  //Tree to access Horizontal Collimator information
  Tree_VerticalColl = new TTree("Vertical_Coll","Vertical Collimator Information");  //Tree to access Vertical Collimator information
  Tree_BSYAG = new TTree("BSYAG","BS YAG Information");  //Tree to access Back Collimator infos
  Tree_BSPECYAG = new TTree("BSPECYAG","BSPEC YAG Information");  //Tree to access Back Collimator infos

  //*****************************INFORMATIONS FROM THE INPUT*******************************************
  // RunBranch = Tree_Input->Branch("x", &StatsInput.x, "x/F" );
  // RunBranch = Tree_Input->Branch("xoffset", &StatsInput.xoffset, "xoffset/F" );
  // RunBranch = Tree_Input->Branch("xp", &StatsInput.xp, "xp/F" );
  // RunBranch = Tree_Input->Branch("y", &StatsInput.y, "y/F" );
  // RunBranch = Tree_Input->Branch("yoffset", &StatsInput.yoffset, "yoffset/F" );
  // RunBranch = Tree_Input->Branch("yp", &StatsInput.yp, "yp/F" );
  // RunBranch = Tree_Input->Branch("s", &StatsInput.s, "s/F" );
  // RunBranch = Tree_Input->Branch("soffset", &StatsInput.soffset, "soffset/F" );
  // RunBranch = Tree_Input->Branch("p", &StatsInput.p, "p/F" );
  // RunBranch = Tree_Input->Branch("delta", &StatsInput.delta, "delta/F" );
  // RunBranch = Tree_Input->Branch("energy", &StatsInput.energy, "energy/F" );
  RunBranch = Tree_Input->Branch("Nevent", &StatsInput.Nevent, "Nevent/I" );

  //*****************************GLOBAL INFORMATION FROM THE HORIZONTAL COLLIMATOR**************************************
  RunBranch = Tree_HorizontalCollGlobal->Branch("photonNuclear", &StatsHorizontalCollGlobal.N_photonNuclear, "photonNuclear/I" );
  RunBranch = Tree_HorizontalCollGlobal->Branch("Radioactivation", &StatsHorizontalCollGlobal.N_Radioactivation, "Radioactivation/I" );
  //RunBranch = Tree_HorizontalCollGlobal->Branch("Brem", &StatsHorizontalCollGlobal.N_Brem, "Brem/I" );
  RunBranch = Tree_HorizontalCollGlobal->Branch("hadElastic", &StatsHorizontalCollGlobal.N_hadElastic, "hadElastic/I" );
  RunBranch = Tree_HorizontalCollGlobal->Branch("neutronInelastic", &StatsHorizontalCollGlobal.N_neutronInelastic, "neutronInelastic/I" );
  RunBranch = Tree_HorizontalCollGlobal->Branch("nCapture", &StatsHorizontalCollGlobal.N_nCapture, "nCapture/I" );
  RunBranch = Tree_HorizontalCollGlobal->Branch("nFission", &StatsHorizontalCollGlobal.N_nFission, "nFission/I" );
  // RunBranch = Tree_HorizontalCollGlobal->Branch("conv", &StatsHorizontalCollGlobal.N_conv, "conv/I" );
  // RunBranch = Tree_HorizontalCollGlobal->Branch("phot", &StatsHorizontalCollGlobal.N_phot, "phot/I" );
  // RunBranch = Tree_HorizontalCollGlobal->Branch("compt", &StatsHorizontalCollGlobal.N_compt, "compt/I" );
  // RunBranch = Tree_HorizontalCollGlobal->Branch("annihil", &StatsHorizontalCollGlobal.N_annihil, "annihil/I" );
  // RunBranch = Tree_HorizontalCollGlobal->Branch("Other", &StatsHorizontalCollGlobal.N_other, "Other/I" );


  //*****************************GLOBAL INFORMATION FROM THE VERTICAL COLLIMATOR**************************************
  RunBranch = Tree_VerticalCollGlobal->Branch("photonNuclear", &StatsVerticalCollGlobal.N_photonNuclear, "photonNuclear/I" );
  RunBranch = Tree_VerticalCollGlobal->Branch("Radioactivation", &StatsVerticalCollGlobal.N_Radioactivation, "Radioactivation/I" );
  //RunBranch = Tree_VerticalCollGlobal->Branch("Brem", &StatsVerticalCollGlobal.N_Brem, "Brem/I" );
  RunBranch = Tree_VerticalCollGlobal->Branch("hadElastic", &StatsVerticalCollGlobal.N_hadElastic, "hadElastic/I" );
  RunBranch = Tree_VerticalCollGlobal->Branch("neutronInelastic", &StatsVerticalCollGlobal.N_neutronInelastic, "neutronInelastic/I" );
  RunBranch = Tree_VerticalCollGlobal->Branch("nCapture", &StatsVerticalCollGlobal.N_nCapture, "nCapture/I" );
  RunBranch = Tree_VerticalCollGlobal->Branch("nFission", &StatsVerticalCollGlobal.N_nFission, "nFission/I" );
  // RunBranch = Tree_VerticalCollGlobal->Branch("conv", &StatsVerticalCollGlobal.N_conv, "conv/I" );
  // RunBranch = Tree_VerticalCollGlobal->Branch("phot", &StatsVerticalCollGlobal.N_phot, "phot/I" );
  // RunBranch = Tree_VerticalCollGlobal->Branch("compt", &StatsVerticalCollGlobal.N_compt, "compt/I" );
  // RunBranch = Tree_VerticalCollGlobal->Branch("annihil", &StatsVerticalCollGlobal.N_annihil, "annihil/I" );
  // RunBranch = Tree_VerticalCollGlobal->Branch("Other", &StatsVerticalCollGlobal.N_other, "Other/I" );
  

//************************************INFORMATIONS FROM THE HORIZONTAL COLLIMATOR*****************************************
  //RunBranch = Tree_HorizontalColl->Branch("parentID", "vector<int>" , &StatsHorizontalColl.parentID);
  RunBranch = Tree_HorizontalColl->Branch("particleID", "vector<int>" , &StatsHorizontalColl.particleID);
  RunBranch = Tree_HorizontalColl->Branch("energy", "vector<float>" , &StatsHorizontalColl.energy);
  RunBranch = Tree_HorizontalColl->Branch("Edep", &StatsHorizontalColl.Edep, "Edep/F" );

  //************************************INFORMATIONS FROM THE VERTICAL COLLIMATOR*****************************************
  //RunBranch = Tree_VerticalColl->Branch("parentID", "vector<int>" , &StatsVerticalColl.parentID);
  RunBranch = Tree_VerticalColl->Branch("particleID", "vector<int>" , &StatsVerticalColl.particleID);
  RunBranch = Tree_VerticalColl->Branch("energy", "vector<float>" , &StatsVerticalColl.energy);
  RunBranch = Tree_VerticalColl->Branch("Edep", &StatsVerticalColl.Edep, "Edep/F" );

  //************************************INFORMATIONS FROM THE BS YAG*****************************************
  RunBranch = Tree_BSYAG->Branch("x_exit", "vector<float>" , &StatsBSYAG.x_exit);
  RunBranch = Tree_BSYAG->Branch("y_exit", "vector<float>" , &StatsBSYAG.y_exit);
  RunBranch = Tree_BSYAG->Branch("z_exit", "vector<float>" , &StatsBSYAG.z_exit);
  RunBranch = Tree_BSYAG->Branch("parentID", "vector<int>" , &StatsBSYAG.parentID);
  RunBranch = Tree_BSYAG->Branch("particleID", "vector<int>" , &StatsBSYAG.particleID);
  RunBranch = Tree_BSYAG->Branch("energy", "vector<float>" , &StatsBSYAG.energy);
  RunBranch = Tree_BSYAG->Branch("deposited_energy", "vector<float>" , &StatsBSYAG.total_deposited_energy);

//************************************INFORMATIONS FROM THE BSPEC YAG*****************************************
  RunBranch = Tree_BSPECYAG->Branch("x_exit", "vector<float>" , &StatsBSPECYAG.x_exit);
  RunBranch = Tree_BSPECYAG->Branch("y_exit", "vector<float>" , &StatsBSPECYAG.y_exit);
  RunBranch = Tree_BSPECYAG->Branch("z_exit", "vector<float>" , &StatsBSPECYAG.z_exit);
  RunBranch = Tree_BSPECYAG->Branch("parentID", "vector<int>" , &StatsBSPECYAG.parentID);
  RunBranch = Tree_BSPECYAG->Branch("particleID", "vector<int>" , &StatsBSPECYAG.particleID);
  RunBranch = Tree_BSPECYAG->Branch("energy", "vector<float>" , &StatsBSPECYAG.energy);
  RunBranch = Tree_BSPECYAG->Branch("deposited_energy", "vector<float>" , &StatsBSPECYAG.total_deposited_energy);



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
UpdateStatisticsHorizontalCollGlobal(StatsHorizontalCollGlobal);
UpdateStatisticsVerticalCollGlobal(StatsVerticalCollGlobal);
  G4AutoLock lock(&fileMutex); // Verrouillage automatique du mutex
  
  f->cd();
  Tree_Input->Write();
  Tree_HorizontalCollGlobal->Write();
  Tree_VerticalCollGlobal->Write();
  Tree_HorizontalColl->Write();
  Tree_VerticalColl->Write();
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

template <typename T>
void PALLAS_CollSimRunAction::UpdateStatistics(T& stats, const T& newStats, TTree* tree) {
    std::lock_guard<std::mutex> lock(fileMutex);  // Protéger avec un verrou
    stats = newStats;  // Assigner les nouvelles statistiques
    if (tree) {
        tree->Fill();  // Remplir l'arbre
    } else {
        G4cerr << "Error: Tree is nullptr" << G4endl;
    }
}

// Implémentations spécifiques utilisant le template
void PALLAS_CollSimRunAction::UpdateStatisticsInput(RunTallyInput aRunTallyInput) {
    UpdateStatistics(StatsInput, aRunTallyInput, Tree_Input);
}


void PALLAS_CollSimRunAction::UpdateStatisticsHorizontalCollGlobal(RunTallyHorizontalCollGlobal aRunTallyHorizontalCollGlobal) {
    UpdateStatistics(StatsHorizontalCollGlobal, aRunTallyHorizontalCollGlobal, Tree_HorizontalCollGlobal);
}


void PALLAS_CollSimRunAction::UpdateStatisticsVerticalCollGlobal(RunTallyVerticalCollGlobal aRunTallyVerticalCollGlobal) {
    UpdateStatistics(StatsVerticalCollGlobal, aRunTallyVerticalCollGlobal, Tree_VerticalCollGlobal);
}

void PALLAS_CollSimRunAction::UpdateStatisticsHorizontalColl(RunTallyHorizontalColl aRunTallyHorizontalColl) {
    UpdateStatistics(StatsHorizontalColl, aRunTallyHorizontalColl, Tree_HorizontalColl);
}

void PALLAS_CollSimRunAction::UpdateStatisticsVerticalColl(RunTallyVerticalColl aRunTallyVerticalColl) {
    UpdateStatistics(StatsVerticalColl, aRunTallyVerticalColl, Tree_VerticalColl);
}

void PALLAS_CollSimRunAction::UpdateStatisticsBSYAG(RunTallyBSYAG aRunTallyBSYAG) {
    UpdateStatistics(StatsBSYAG, aRunTallyBSYAG, Tree_BSYAG);
}

void PALLAS_CollSimRunAction::UpdateStatisticsBSPECYAG(RunTallyBSPECYAG aRunTallyBSPECYAG) {
    UpdateStatistics(StatsBSPECYAG, aRunTallyBSPECYAG, Tree_BSPECYAG);
}