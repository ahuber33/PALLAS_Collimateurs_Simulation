/// PALLAS_CollSimTrackingAction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimTrackingAction_h
#define PALLAS_CollSimTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"

class PALLAS_CollSimTrackingAction : public G4UserTrackingAction
{
public:
  PALLAS_CollSimTrackingAction();
  ~PALLAS_CollSimTrackingAction();

  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);
};
#endif
