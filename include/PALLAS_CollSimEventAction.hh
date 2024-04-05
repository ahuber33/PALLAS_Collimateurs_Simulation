/// PALLAS_CollSimEventAction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS


#ifndef PALLAS_CollSimEventAction_h
#define PALLAS_CollSimEventAction_h 1
#include "G4UserEventAction.hh"
#include "TTree.h"
#include "TBranch.h"


class G4Event;

struct RunTallyCollimator {
  float E_start;
  float E_dep;
  float E_dep_e;
  float E_dep_g;
  std::vector<float> E_gamma_Brem;


  inline int operator ==(const RunTallyCollimator& right) const
  {return (this==&right);}
};


struct RunTallyFrontCollimator {
  std::vector<int> ParticuleID;
  std::vector<float> E_exit;
  std::vector<float> x_exit;
  std::vector<float> y_exit;
  std::vector<float> z_exit;
  std::vector<float> px_exit;
  std::vector<float> py_exit;
  std::vector<float> pz_exit;

  inline int operator ==(const RunTallyFrontCollimator& right) const
  {return (this==&right);}
};


struct RunTallyBackCollimator {
  std::vector<int> ParticuleID;
  std::vector<float> E_exit;
  std::vector<float> x_exit;
  std::vector<float> y_exit;
  std::vector<float> z_exit;
  std::vector<float> px_exit;
  std::vector<float> py_exit;
  std::vector<float> pz_exit;

  inline int operator ==(const RunTallyBackCollimator& right) const
  {return (this==&right);}
};



class PALLAS_CollSimEventAction : public G4UserEventAction
{
public:
  PALLAS_CollSimEventAction(char*);
  ~PALLAS_CollSimEventAction();

public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);  

  //Functions for TP Tree
  void SetEstartCollimator(G4float d){StatsCollimator.E_start=d;}
  float GetEstartCollimator(){return StatsCollimator.E_start;}
  void AddEdepCollimator(G4float d){StatsCollimator.E_dep+=d;}
  float GetEdepCollimator(){return StatsCollimator.E_dep;}
  void AddEdepElectronCollimator(G4float d){StatsCollimator.E_dep_e+=d;}
  float GetEdepElectronCollimator(){return StatsCollimator.E_dep_e;}
  void AddEdepGammaCollimator(G4float d){StatsCollimator.E_dep_g+=d;}
  float GetEdepGammaCollimator(){return StatsCollimator.E_dep_g;}


private:

  TTree *EventTree;
  TBranch *EventBranch;
  RunTallyCollimator StatsCollimator;
  RunTallyFrontCollimator StatsFrontCollimator;
  RunTallyBackCollimator StatsBackCollimator;
  G4String suffixe;
  
};


#endif
