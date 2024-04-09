/// PALLAS_CollSimEventAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimSteppingAction.hh"
#include "PALLAS_CollSimEventAction.hh"
#include "PALLAS_CollSimRunAction.hh"

using namespace CLHEP;

PALLAS_CollSimEventAction::PALLAS_CollSimEventAction(char *suff) : suffixe(suff) {}

PALLAS_CollSimEventAction::~PALLAS_CollSimEventAction() {}

// Initialize all counters and set up the event branches for
// filling histograms with ROOT
void PALLAS_CollSimEventAction::BeginOfEventAction(const G4Event *evt)
{

  // Collimator
  StatsCollimator.E_start = 0;
  StatsCollimator.E_dep = 0;
  StatsCollimator.E_dep_e = 0;
  StatsCollimator.E_dep_g = 0;
  StatsCollimator.Energy_Brem_created.clear();

  // FRONT Collimator
  StatsFrontCollimator.particleID.clear();
  StatsFrontCollimator.parentID.clear();
  StatsFrontCollimator.E_exit.clear();
  StatsFrontCollimator.x_exit.clear();
  StatsFrontCollimator.y_exit.clear();
  StatsFrontCollimator.z_exit.clear();
  StatsFrontCollimator.px_exit.clear();
  StatsFrontCollimator.py_exit.clear();
  StatsFrontCollimator.pz_exit.clear();

  // BACK Collimator
  StatsBackCollimator.particleID.clear();
  StatsBackCollimator.parentID.clear();
  StatsBackCollimator.E_exit.clear();
  StatsBackCollimator.x_exit.clear();
  StatsBackCollimator.y_exit.clear();
  StatsBackCollimator.z_exit.clear();
  StatsBackCollimator.px_exit.clear();
  StatsBackCollimator.py_exit.clear();
  StatsBackCollimator.pz_exit.clear();
}

void PALLAS_CollSimEventAction::EndOfEventAction(const G4Event *evt)
{

  PALLAS_CollSimRunAction *runac = (PALLAS_CollSimRunAction *)(G4RunManager::GetRunManager()->GetUserRunAction());

  runac->UpdateStatisticsCollimator(StatsCollimator);
  runac->UpdateStatisticsFrontCollimator(StatsFrontCollimator);
  runac->UpdateStatisticsBackCollimator(StatsBackCollimator);

  //G4cout << "Etot verif = " << StatsCollimator.E_dep_verif << G4endl;
}
