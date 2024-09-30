/// PALLAS_CollSimEventAction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS


#ifndef PALLAS_CollSimEventAction_h
#define PALLAS_CollSimEventAction_h 1
#include "G4UserEventAction.hh"
#include "TTree.h"
#include "TBranch.h"


class G4Event;

// Structure générique pour gérer les sorties de coordonnées et d'énergies
template<typename T>
struct CoordExitData {
    std::vector<T> x_exit;
    std::vector<T> y_exit;
    std::vector<T> z_exit;

    void AddXExit(T d) { x_exit.push_back(d); }
    size_t GetXExitSize() const { return x_exit.size(); }
    T GetXExit(size_t a) const { return x_exit.at(a); }

    void AddYExit(T d) { y_exit.push_back(d); }
    size_t GetYExitSize() const { return y_exit.size(); }
    T GetYExit(size_t a) const { return y_exit.at(a); }

    void AddZExit(T d) { z_exit.push_back(d); }
    size_t GetZExitSize() const { return z_exit.size(); }
    T GetZExit(size_t a) const { return z_exit.at(a); }
};


// Structure pour gérer les collimateurs (avant et arrière) => EXEMPLE A SUIVRE POUR PLUS TARD
// struct RunTallyCollimator : public CoordExitData<float> {
//     std::vector<int> particleID;
//     std::vector<int> parentID;
//     std::vector<float> E_exit;
//     std::vector<float> px_exit, py_exit, pz_exit;
//     float E_start = 0.0f;
//     float E_dep = 0.0f;
//     float E_dep_e = 0.0f;
//     float E_dep_g = 0.0f;
//     std::vector<float> Energy_Brem_created;

//     void AddParticleID(int id) { particleID.push_back(id); }
//     size_t GetParticleIDSize() const { return particleID.size(); }
//     int GetParticleID(size_t a) const { return particleID.at(a); }
// };


struct RunTallyInput {
  float x = 0.0;
  float xoffset = 0.0;
  float xp = 0.0;
  float y = 0.0;
  float yoffset = 0.0;
  float yp = 0.0;
  float s = 0.0;
  float soffset = 0.0;
  float p = 0.0;
  float delta = 0.0;
  float energy = 0.0;
  int Nevent = 0.0;

};

struct RunTallyCollimator {
  float E_start = 0.0;
  float E_dep = 0.0;
  float E_dep_e = 0.0;
  float E_dep_g = 0.0;
  std::vector<float> Energy_Brem_created;
};


struct RunTallyFrontCollimator {
  std::vector<int> particleID;
  std::vector<int> parentID;
  std::vector<float> E_exit;
  std::vector<float> x_exit;
  std::vector<float> y_exit;
  std::vector<float> z_exit;
  std::vector<float> px_exit;
  std::vector<float> py_exit;
  std::vector<float> pz_exit;


};


struct RunTallyBackCollimator {
  std::vector<int> particleID;
  std::vector<int> parentID;
  std::vector<float> E_exit;
  std::vector<float> x_exit;
  std::vector<float> y_exit;
  std::vector<float> z_exit;
  std::vector<float> px_exit;
  std::vector<float> py_exit;
  std::vector<float> pz_exit;


};


struct RunTallyBSYAG {
  std::vector<float> x_exit;
  std::vector<float> y_exit;
  std::vector<float> z_exit;
  std::vector<float> parentID;
  std::vector<float> energy;
  float deposited_energy = 0.0;
  std::vector<float> total_deposited_energy;
  G4bool flag_BSYAG = false;


};



struct RunTallyBSPECYAG {
  std::vector<float> x_exit;
  std::vector<float> y_exit;
  std::vector<float> z_exit;
  std::vector<float> parentID;
  std::vector<float> energy;
  float deposited_energy = 0.0;
  std::vector<float> total_deposited_energy;
  G4bool flag_BSPECYAG = false;


};


class PALLAS_CollSimEventAction : public G4UserEventAction
{
public:
  PALLAS_CollSimEventAction(const char*);
  ~PALLAS_CollSimEventAction();

public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);  

  //Functions for Input Tree
  void SetXStart(G4float d){StatsInput.x=d;}
  void SetXOffsetStart(G4float d){StatsInput.xoffset=d;}
  void SetXpStart(G4float d){StatsInput.xp=d;}
  void SetYStart(G4float d){StatsInput.y=d;}
  void SetYOffsetStart(G4float d){StatsInput.yoffset=d;}
  void SetYpStart(G4float d){StatsInput.yp=d;}
  void SetSStart(G4float d){StatsInput.s=d;}
  void SetSOffsetStart(G4float d){StatsInput.soffset=d;}
  void SetPStart(G4float d){StatsInput.p=d;}
  void SetDeltaStart(G4float d){StatsInput.delta=d;}
  void SetEnergyStart(G4float d){StatsInput.energy=d;}
  void AddNeventStart(){StatsInput.Nevent++;}
  int GetNeventStart(){return StatsInput.Nevent;}

  //Functions for Collimator Tree
  void SetEstartCollimator(G4float d){StatsCollimator.E_start=d;}
  float GetEstartCollimator(){return StatsCollimator.E_start;}
  void AddEdepCollimator(G4float d){StatsCollimator.E_dep+=d;}
  float GetEdepCollimator(){return StatsCollimator.E_dep;}
  void AddEdepElectronCollimator(G4float d){StatsCollimator.E_dep_e+=d;}
  float GetEdepElectronCollimator(){return StatsCollimator.E_dep_e;}
  void AddEdepGammaCollimator(G4float d){StatsCollimator.E_dep_g+=d;}
  float GetEdepGammaCollimator(){return StatsCollimator.E_dep_g;}
  void AddEBremCreated(G4float d){StatsCollimator.Energy_Brem_created.push_back(d);}
  int GetEBremCreatedSize(){return StatsCollimator.Energy_Brem_created.size();}
  float GetEBremCreatedEnergy(G4int a){return StatsCollimator.Energy_Brem_created.at(a);}

  void AddParticleIDFront(G4float d){StatsFrontCollimator.particleID.push_back(d);}
  int GetParticleIDFrontSize(){return StatsFrontCollimator.particleID.size();}
  float GetParticleIDFront(G4float a){return StatsFrontCollimator.particleID.at(a);}
  void AddParentIDFront(G4float d){StatsFrontCollimator.parentID.push_back(d);}
  int GetParentIDFrontSize(){return StatsFrontCollimator.parentID.size();}
  float GetParentIDFront(G4float a){return StatsFrontCollimator.parentID.at(a);}
  void AddEnergyExitFront(G4float d){StatsFrontCollimator.E_exit.push_back(d);}
  int GetEnergyExitFrontSize(){return StatsFrontCollimator.E_exit.size();}
  float GetEnergyExitFront(G4float a){return StatsFrontCollimator.E_exit.at(a);}
  void AddXExitFront(G4float d){StatsFrontCollimator.x_exit.push_back(d);}
  int GetXExitFrontSize(){return StatsFrontCollimator.x_exit.size();}
  float GetXExitFront(G4float a){return StatsFrontCollimator.x_exit.at(a);}
  void AddYExitFront(G4float d){StatsFrontCollimator.y_exit.push_back(d);}
  int GetYExitFrontSize(){return StatsFrontCollimator.y_exit.size();}
  float GetYExitFront(G4float a){return StatsFrontCollimator.y_exit.at(a);}
  void AddZExitFront(G4float d){StatsFrontCollimator.z_exit.push_back(d);}
  int GetZExitFrontSize(){return StatsFrontCollimator.z_exit.size();}
  float GetZExitFront(G4float a){return StatsFrontCollimator.z_exit.at(a);}
  void AddPxExitFront(G4float d){StatsFrontCollimator.px_exit.push_back(d);}
  int GetPxExitFrontSize(){return StatsFrontCollimator.px_exit.size();}
  float GetPxExitFront(G4float a){return StatsFrontCollimator.px_exit.at(a);}
  void AddPyExitFront(G4float d){StatsFrontCollimator.py_exit.push_back(d);}
  int GetPyExitFrontSize(){return StatsFrontCollimator.py_exit.size();}
  float GetPyExitFront(G4float a){return StatsFrontCollimator.py_exit.at(a);}
  void AddPzExitFront(G4float d){StatsFrontCollimator.pz_exit.push_back(d);}
  int GetPzExitFrontSize(){return StatsFrontCollimator.pz_exit.size();}
  float GetPzExitFront(G4float a){return StatsFrontCollimator.pz_exit.at(a);}

  void AddParticleIDBack(G4float d){StatsBackCollimator.particleID.push_back(d);}
  int GetParticleIDBackSize(){return StatsBackCollimator.particleID.size();}
  float GetParticleIDBack(G4float a){return StatsBackCollimator.particleID.at(a);}
  void AddParentIDBack(G4float d){StatsBackCollimator.parentID.push_back(d);}
  int GetParentIDBackSize(){return StatsBackCollimator.parentID.size();}
  float GetParentIDBack(G4float a){return StatsBackCollimator.parentID.at(a);}
  void AddEnergyExitBack(G4float d){StatsBackCollimator.E_exit.push_back(d);}
  int GetEnergyExitBackSize(){return StatsBackCollimator.E_exit.size();}
  float GetEnergyExitBack(G4float a){return StatsBackCollimator.E_exit.at(a);}
  void AddXExitBack(G4float d){StatsBackCollimator.x_exit.push_back(d);}
  int GetXExitBackSize(){return StatsBackCollimator.x_exit.size();}
  float GetXExitBack(G4float a){return StatsBackCollimator.x_exit.at(a);}
  void AddYExitBack(G4float d){StatsBackCollimator.y_exit.push_back(d);}
  int GetYExitBackSize(){return StatsBackCollimator.y_exit.size();}
  float GetYExitBack(G4float a){return StatsBackCollimator.y_exit.at(a);}
  void AddZExitBack(G4float d){StatsBackCollimator.z_exit.push_back(d);}
  int GetZExitBackSize(){return StatsBackCollimator.z_exit.size();}
  float GetZExitBack(G4float a){return StatsBackCollimator.z_exit.at(a);}
  void AddPxExitBack(G4float d){StatsBackCollimator.px_exit.push_back(d);}
  int GetPxExitBackSize(){return StatsBackCollimator.px_exit.size();}
  float GetPxExitBack(G4float a){return StatsBackCollimator.px_exit.at(a);}
  void AddPyExitBack(G4float d){StatsBackCollimator.py_exit.push_back(d);}
  int GetPyExitBackSize(){return StatsBackCollimator.py_exit.size();}
  float GetPyExitBack(G4float a){return StatsBackCollimator.py_exit.at(a);}
  void AddPzExitBack(G4float d){StatsBackCollimator.pz_exit.push_back(d);}
  int GetPzExitBackSize(){return StatsBackCollimator.pz_exit.size();}
  float GetPzExitBack(G4float a){return StatsBackCollimator.pz_exit.at(a);}


  void AddXExitBSYAG(G4float d){StatsBSYAG.x_exit.push_back(d);}
  int GetXExitBSYAGSize(){return StatsBSYAG.x_exit.size();}
  float GetXExitBSYAG(G4float a){return StatsBSYAG.x_exit.at(a);}
  void AddYExitBSYAG(G4float d){StatsBSYAG.y_exit.push_back(d);}
  int GetYExitBSYAGSize(){return StatsBSYAG.y_exit.size();}
  float GetYExitBSYAG(G4float a){return StatsBSYAG.y_exit.at(a);}
  void AddZExitBSYAG(G4float d){StatsBSYAG.z_exit.push_back(d);}
  int GetZExitBSYAGSize(){return StatsBSYAG.z_exit.size();}
  float GetZExitBSYAG(G4float a){return StatsBSYAG.z_exit.at(a);}
  void ActiveFlagBSYAG(){StatsBSYAG.flag_BSYAG =true;}
  void ResetFlagBSYAG(){StatsBSYAG.flag_BSYAG =false;}
  G4bool ReturnFlagBSYAG(){return StatsBSYAG.flag_BSYAG;}
  void AddEnergyBSYAG(G4float d){StatsBSYAG.energy.push_back(d);}
  int GetEnergyBSYAGSize(){return StatsBSYAG.energy.size();}
  float GetEnergyBSYAG(G4float a){return StatsBSYAG.energy.at(a);}
  void AddDepositedEnergyBSYAG(G4float d){StatsBSYAG.deposited_energy+=d;}
  void ResetDepositedEnergyBSYAG(){StatsBSYAG.deposited_energy=0;}
  float GetDepositedEnergyBSYAG(){return StatsBSYAG.deposited_energy;}
  void AddTotalDepositedEnergyBSYAG(G4float d){StatsBSYAG.total_deposited_energy.push_back(d);}
  int GetTotalDepositedEnergyBSYAGSize(){return StatsBSYAG.total_deposited_energy.size();}
  float GetTotalDepositedEnergyBSYAG(G4float a){return StatsBSYAG.total_deposited_energy.at(a);}
  void AddParentIDBSYAG(G4float d){StatsBSYAG.parentID.push_back(d);}
  int GetParentIDBSYAGSize(){return StatsBSYAG.parentID.size();}
  float GetParentIDBSYAG(G4float a){return StatsBSYAG.parentID.at(a);}


  void AddXExitBSPECYAG(G4float d){StatsBSPECYAG.x_exit.push_back(d);}
  int GetXExitBSPECYAGSize(){return StatsBSPECYAG.x_exit.size();}
  float GetXExitBSPECYAG(G4float a){return StatsBSPECYAG.x_exit.at(a);}
  void AddYExitBSPECYAG(G4float d){StatsBSPECYAG.y_exit.push_back(d);}
  int GetYExitBSPECYAGSize(){return StatsBSPECYAG.y_exit.size();}
  float GetYExitBSPECYAG(G4float a){return StatsBSPECYAG.y_exit.at(a);}
  void AddZExitBSPECYAG(G4float d){StatsBSPECYAG.z_exit.push_back(d);}
  int GetZExitBSPECYAGSize(){return StatsBSPECYAG.z_exit.size();}
  float GetZExitBSPECYAG(G4float a){return StatsBSPECYAG.z_exit.at(a);}
  void ActiveFlagBSPECYAG(){StatsBSPECYAG.flag_BSPECYAG =true;}
  void ResetFlagBSPECYAG(){StatsBSPECYAG.flag_BSPECYAG =false;}
  G4bool ReturnFlagBSPECYAG(){return StatsBSPECYAG.flag_BSPECYAG;}
  void AddEnergyBSPECYAG(G4float d){StatsBSPECYAG.energy.push_back(d);}
  int GetEnergyBSPECYAGSize(){return StatsBSPECYAG.energy.size();}
  float GetEnergyBSPECYAG(G4float a){return StatsBSPECYAG.energy.at(a);}
  void AddDepositedEnergyBSPECYAG(G4float d){StatsBSPECYAG.deposited_energy+=d;}
  void ResetDepositedEnergyBSPECYAG(){StatsBSPECYAG.deposited_energy=0;}
  float GetDepositedEnergyBSPECYAG(){return StatsBSPECYAG.deposited_energy;}
  void AddTotalDepositedEnergyBSPECYAG(G4float d){StatsBSPECYAG.total_deposited_energy.push_back(d);}
  int GetTotalDepositedEnergyBSPECYAGSize(){return StatsBSPECYAG.total_deposited_energy.size();}
  float GetTotalDepositedEnergyBSPECYAG(G4float a){return StatsBSPECYAG.total_deposited_energy.at(a);}
  void AddParentIDBSPECYAG(G4float d){StatsBSPECYAG.parentID.push_back(d);}
  int GetParentIDBSPECYAGSize(){return StatsBSPECYAG.parentID.size();}
  float GetParentIDBSPECYAG(G4float a){return StatsBSPECYAG.parentID.at(a);}


  // EXEMPLE A SUIVRE POUR PLUS TARD AFIN D OPTIMISER LE CODE !!!!
  // void AddFrontCollimatorData(int particleID, float E_exit, float x, float y, float z) {
  //       StatsFrontCollimator.AddParticleID(particleID);
  //       StatsFrontCollimator.E_exit.push_back(E_exit);
  //       StatsFrontCollimator.AddXExit(x);
  //       StatsFrontCollimator.AddYExit(y);
  //       StatsFrontCollimator.AddZExit(z);
  //   }

private:

  TTree *EventTree;
  TBranch *EventBranch;
  RunTallyInput StatsInput;
  RunTallyCollimator StatsCollimator;
  RunTallyFrontCollimator StatsFrontCollimator;
  RunTallyBackCollimator StatsBackCollimator;
  RunTallyBSYAG StatsBSYAG;
  RunTallyBSPECYAG StatsBSPECYAG;

  
  G4String suffixe;
  
};


#endif
