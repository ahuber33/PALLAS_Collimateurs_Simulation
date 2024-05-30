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
#include "PALLAS_CollSimGeometryConstruction.hh"
#include "G4VProcess.hh"

class PALLAS_CollSimSteppingAction : public G4UserSteppingAction
{
public:
  PALLAS_CollSimSteppingAction();
  ~PALLAS_CollSimSteppingAction();

public:
  bool SetFlagGammaEnergyDeposition();
  void GetInputInformations(PALLAS_CollSimEventAction *evtac);
  void UpdateCollimatorInformations(PALLAS_CollSimEventAction *evtac);
  void UpdateFrontCollimatorInformations(PALLAS_CollSimEventAction *evtac);
  void UpdateBackCollimatorInformations(PALLAS_CollSimEventAction *evtac);
  void UpdateBremInformations(PALLAS_CollSimEventAction *evtac);
  void UserSteppingAction(const G4Step *);
  // int Flag_gamma;

private:
  G4GenericMessenger* sMessenger;
  G4bool TrackingStatus;
  static const G4String path;
  G4Track *theTrack;
  G4String particleName;
  G4String creatorProcessName;
  G4int particleID;
  G4String endproc;
  G4int parentID;
  G4int stepNo;
  G4String volumeNamePreStep;
  G4String volumeNamePostStep;
  G4float energy;
  G4float energyDeposited;
  G4double x;
  G4double y;
  G4double z;
  G4double px;
  G4double py;
  G4double pz;
  G4bool flag_Gamma = false;
};
#endif
