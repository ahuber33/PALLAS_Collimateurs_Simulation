/// PALLAS_CollSimEventAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimSteppingAction.hh"
#include "PALLAS_CollSimEventAction.hh"
#include "PALLAS_CollSimRunAction.hh"

using namespace CLHEP;

PALLAS_CollSimEventAction::PALLAS_CollSimEventAction(const char *suff) : suffixe(suff) {}

PALLAS_CollSimEventAction::~PALLAS_CollSimEventAction() {}

// Initialize all counters and set up the event branches for
// filling histograms with ROOT
void PALLAS_CollSimEventAction::BeginOfEventAction(const G4Event *evt)
{
  // Input
  StatsInput.x = 0;
  StatsInput.xoffset = 0;
  StatsInput.xp = 0;
  StatsInput.y = 0;
  StatsInput.yoffset = 0;
  StatsInput.yp = 0;
  StatsInput.s = 0;
  StatsInput.soffset = 0;
  StatsInput.delta = 0;
  StatsInput.energy = 0;
  StatsInput.Nevent = 0;

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

  //BS YAG
  StatsBSYAG.x_exit.clear();
  StatsBSYAG.y_exit.clear();
  StatsBSYAG.z_exit.clear();
  StatsBSYAG.parentID.clear();
  StatsBSYAG.energy.clear();

  //BSPEC YAG
  StatsBSPECYAG.x_exit.clear();
  StatsBSPECYAG.y_exit.clear();
  StatsBSPECYAG.z_exit.clear();
  StatsBSPECYAG.parentID.clear();
  StatsBSPECYAG.energy.clear();
}

void PALLAS_CollSimEventAction::EndOfEventAction(const G4Event *evt)
{

  PALLAS_CollSimRunAction *runac = (PALLAS_CollSimRunAction *)(G4RunManager::GetRunManager()->GetUserRunAction());

  if (StatsInput.energy>0)
  {
    runac->UpdateStatisticsInput(StatsInput);
    runac->UpdateStatisticsCollimator(StatsCollimator);
    // runac->UpdateStatisticsFrontCollimator(StatsFrontCollimator);
    // runac->UpdateStatisticsBackCollimator(StatsBackCollimator);
  }

  if (StatsBSYAG.energy.size()>0) runac->UpdateStatisticsBSYAG(StatsBSYAG);
  if (StatsBSPECYAG.energy.size()>0) runac->UpdateStatisticsBSPECYAG(StatsBSPECYAG);

  //G4cout << "Etot verif = " << StatsCollimator.E_dep_verif << G4endl;
}
