/// PALLAS_CollSimEventAction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS


#ifndef PALLAS_CollSimEventAction_h
#define PALLAS_CollSimEventAction_h 1
#include "G4UserEventAction.hh"
#include "TTree.h"
#include "TBranch.h"


class G4Event;

struct RunTallyTP {
  int ParticuleID;
  float E_start;
  float E_dep;
  float Charge;
  float PositionX;
  float PositionY;
  float PositionZ;
  float Time;
  float TotalLength;
  float InteractionDepth;


  inline int operator ==(const RunTallyTP& right) const
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
  void SetParticuleID(G4double a){StatsTP.ParticuleID =a;}
  float GetParticuleID(){return StatsTP.ParticuleID;}
  void SetEstartTP(G4double d){StatsTP.E_start+=d;}
  float GetEstartTP(){return StatsTP.E_start;}
  void AddEdepTP(G4double d){StatsTP.E_dep+=d;}
  float GetEdepTP(){return StatsTP.E_dep;}
  void SetCharge(G4double a){StatsTP.Charge =a;}
  float GetCharge(){return StatsTP.Charge;}
  void SetTPPositionX(G4double d){StatsTP.PositionX=d;}
  float GetTPPositionX(){return StatsTP.PositionX;}
  void SetTPPositionY(G4double d){StatsTP.PositionY=d;}
  float GetTPPositionY(){return StatsTP.PositionY;}
  void SetTPPositionZ(G4double d){StatsTP.PositionZ=d;}
  float GetTPPositionZ(){return StatsTP.PositionZ;}
  void SetTPTime(G4double d){StatsTP.Time=d;}
  float GetTPTime(){return StatsTP.Time;}
  void AddTrackLength(G4double d){StatsTP.TotalLength+=d;}
  float GetTotalTrackLength(){return StatsTP.TotalLength;}
  void SetInteractionDepthTP(G4double d){StatsTP.InteractionDepth+=d;}
  float GetInteractionDepthTP(){return StatsTP.InteractionDepth;}


private:

  TTree *EventTree;
  TBranch *EventBranch;
  RunTallyTP StatsTP;
  G4String suffixe;
  



};


#endif
