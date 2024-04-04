/// PALLAS_CollSimSteppingAction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimSteppingAction_h
#define PALLAS_CollSimSteppingAction_h

#include "G4UserSteppingAction.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "PALLAS_CollSimRunAction.hh"
#include "G4RunManager.hh"
#include "PALLAS_CollSimGeometry.hh"



class PALLAS_CollSimSteppingAction : public G4UserSteppingAction
{
public:
  PALLAS_CollSimSteppingAction();
  ~PALLAS_CollSimSteppingAction();
public:
  void UserSteppingAction(const G4Step*);


private:
  static const G4String path;


};
#endif
