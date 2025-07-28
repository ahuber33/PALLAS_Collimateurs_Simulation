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
#include "G4AnalysisManager.hh"
#include <mutex>
#include <G4AutoLock.hh>
#include "G4RunManager.hh"


//class G4Run;

struct RunTallyHorizontalCollGlobal {
  int N_photonNuclear;
  int N_Radioactivation;
  int N_Brem;
  int N_hadElastic;
  int N_neutronInelastic;
  int N_nCapture;
  int N_nFission;
  int N_conv;
  int N_phot;
  int N_compt;
  int N_annihil;
  int N_other;
  float Edep;
};


struct RunTallyVerticalCollGlobal {
  int N_photonNuclear;
  int N_Radioactivation;
  int N_Brem;
  int N_hadElastic;
  int N_neutronInelastic;
  int N_nCapture;
  int N_nFission;
  int N_conv;
  int N_phot;
  int N_compt;
  int N_annihil;
  int N_other;
  float Edep;

};


class PALLAS_CollSimRunAction : public G4UserRunAction
{

public:
  PALLAS_CollSimRunAction(const char*, G4bool);
  ~PALLAS_CollSimRunAction();

public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);

  template<typename T>
  void UpdateStatistics(T& stats, const T& newStats, TTree* tree);
  //adds the photon fates from an event to the run tree
  void UpdateStatisticsInput(RunTallyInput);
  void UpdateStatisticsHorizontalCollGlobal(RunTallyHorizontalCollGlobal);
  void UpdateStatisticsVerticalCollGlobal(RunTallyVerticalCollGlobal);
  void UpdateStatisticsHorizontalColl(RunTallyHorizontalColl);
  void UpdateStatisticsVerticalColl(RunTallyVerticalColl);
  void UpdateStatisticsBSYAG(RunTallyBSYAG);
  void UpdateStatisticsBSPECYAG(RunTallyBSPECYAG);

  //Functions for Horizontal Collimator Tree
  void AddHorizontalCollPhotonNuclearInt(){StatsHorizontalCollGlobal.N_photonNuclear++;}
  void AddHorizontalCollRadioactivationInt(){StatsHorizontalCollGlobal.N_Radioactivation++;}
  void AddHorizontalCollBremInt(){StatsHorizontalCollGlobal.N_Brem++;}
  void AddHorizontalCollhadElasticInt(){StatsHorizontalCollGlobal.N_hadElastic++;}
  void AddHorizontalCollneutronInelasticInt(){StatsHorizontalCollGlobal.N_neutronInelastic++;}
  void AddHorizontalCollnCaptureInt(){StatsHorizontalCollGlobal.N_nCapture++;}
  void AddHorizontalCollnFissionInt(){StatsHorizontalCollGlobal.N_nFission++;}
  void AddHorizontalCollConvInt(){StatsHorizontalCollGlobal.N_conv++;}
  void AddHorizontalCollPhotInt(){StatsHorizontalCollGlobal.N_phot++;}
  void AddHorizontalCollComptInt(){StatsHorizontalCollGlobal.N_compt++;}
  void AddHorizontalCollAnnihilInt(){StatsHorizontalCollGlobal.N_annihil++;}
  void AddHorizontalCollOtherInt(){StatsHorizontalCollGlobal.N_other++;}


  //Functions for Vertical Collimator Tree
  void AddVerticalCollPhotonNuclearInt(){StatsVerticalCollGlobal.N_photonNuclear++;}
  void AddVerticalCollRadioactivationInt(){StatsVerticalCollGlobal.N_Radioactivation++;}
  void AddVerticalCollBremInt(){StatsVerticalCollGlobal.N_Brem++;}
  void AddVerticalCollhadElasticInt(){StatsVerticalCollGlobal.N_hadElastic++;}
  void AddVerticalCollneutronInelasticInt(){StatsVerticalCollGlobal.N_neutronInelastic++;}
  void AddVerticalCollnCaptureInt(){StatsVerticalCollGlobal.N_nCapture++;}
  void AddVerticalCollnFissionInt(){StatsVerticalCollGlobal.N_nFission++;}
  void AddVerticalCollConvInt(){StatsVerticalCollGlobal.N_conv++;}
  void AddVerticalCollPhotInt(){StatsVerticalCollGlobal.N_phot++;}
  void AddVerticalCollComptInt(){StatsVerticalCollGlobal.N_compt++;}
  void AddVerticalCollAnnihilInt(){StatsVerticalCollGlobal.N_annihil++;}
  void AddVerticalCollOtherInt(){StatsVerticalCollGlobal.N_other++;}

  

private:
  G4String suffixe;
  G4String fileName;
  RunTallyInput StatsInput;
  RunTallyHorizontalCollGlobal StatsHorizontalCollGlobal;
  RunTallyVerticalCollGlobal StatsVerticalCollGlobal;
  RunTallyHorizontalColl StatsHorizontalColl;
  RunTallyVerticalColl StatsVerticalColl;
  RunTallyBSYAG StatsBSYAG;
  RunTallyBSPECYAG StatsBSPECYAG;
  G4bool flag_MT;
  TFile *f=nullptr;
  TTree *Tree_Input=nullptr;
  TTree *Tree_HorizontalCollGlobal=nullptr;
  TTree *Tree_VerticalCollGlobal=nullptr;
  TTree *Tree_HorizontalColl=nullptr;
  TTree *Tree_VerticalColl=nullptr;
  TTree *Tree_BSYAG=nullptr;
  TTree *Tree_BSPECYAG=nullptr;
  TBranch *RunBranch=nullptr;
  time_t start;
  static std::atomic<int> activeThreads;
  static G4Mutex fileMutex;
  

};

#endif
