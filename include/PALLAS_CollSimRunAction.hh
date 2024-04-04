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
  void UpdateStatisticsTP(RunTallyTP);



private:
  G4String suffixe;
  RunTallyTP StatsTP;
  TFile *f;
  TTree *Tree_TP;
  TBranch *RunBranch;
  time_t start;

};

#endif
