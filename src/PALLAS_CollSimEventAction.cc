/// PALLAS_CollSimEventAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimSteppingAction.hh"
#include "PALLAS_CollSimEventAction.hh"
#include "PALLAS_CollSimRunAction.hh"


using namespace CLHEP;

PALLAS_CollSimEventAction::PALLAS_CollSimEventAction(char* suff):suffixe(suff){}

PALLAS_CollSimEventAction::~PALLAS_CollSimEventAction(){}


// Initialize all counters and set up the event branches for
// filling histograms with ROOT
void PALLAS_CollSimEventAction::BeginOfEventAction(const G4Event* evt){

  //StatsOptical.DetectorPositionX.clear();
  StatsTP.ParticuleID=0;
  StatsTP.E_start=0;
  StatsTP.E_dep=0;
  StatsTP.Charge=0;
  StatsTP.PositionX =0;
  StatsTP.PositionY =0;
  StatsTP.PositionZ =-300;
  StatsTP.Time =0;
  StatsTP.TotalLength =0;
  StatsTP.InteractionDepth =0;

}


// Get the number of stored trajectories and calculate the statistics
void PALLAS_CollSimEventAction::EndOfEventAction(const G4Event* evt){
  //G4int event_id = evt->GetEventID();

  PALLAS_CollSimRunAction *runac = (PALLAS_CollSimRunAction*)(G4RunManager::GetRunManager()->GetUserRunAction());


runac->UpdateStatisticsTP(StatsTP);



}
