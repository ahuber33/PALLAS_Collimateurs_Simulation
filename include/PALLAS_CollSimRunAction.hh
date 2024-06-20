/// PALLAS_CollSimRunAction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimRunAction_h
#define PALLAS_CollSimRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "PALLAS_CollSimEventAction.hh"
#include "Randomize.hh"
#include "G4Run.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "TFile.h"


//class G4Run;

class PALLAS_CollSimRunAction : public G4UserRunAction
{

public:
  PALLAS_CollSimRunAction(char*);
  ~PALLAS_CollSimRunAction();

public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  //adds the photon fates from an event to the run tree
  void InitializeRootFile(G4String fileName);
  void UpdateStatisticsInput(RunTallyInput);
  void UpdateStatisticsCollimator(RunTallyCollimator);
  void UpdateStatisticsFrontCollimator(RunTallyFrontCollimator);
  void UpdateStatisticsBackCollimator(RunTallyBackCollimator);
  void UpdateStatisticsYAG(RunTallyYAG);
  

private:
  G4String suffixe;
  RunTallyInput StatsInput;
  RunTallyCollimator StatsCollimator;
  RunTallyFrontCollimator StatsFrontCollimator;
  RunTallyBackCollimator StatsBackCollimator;
  RunTallyYAG StatsYAG;
  TFile *f=nullptr;
  TTree *Tree_Input=nullptr;
  TTree *Tree_Collimator=nullptr;
  TTree *Tree_FrontCollimator=nullptr;
  TTree *Tree_BackCollimator=nullptr;
  TTree *Tree_YAG=nullptr;
  TBranch *RunBranch=nullptr;
  time_t start;
  

};

#endif
