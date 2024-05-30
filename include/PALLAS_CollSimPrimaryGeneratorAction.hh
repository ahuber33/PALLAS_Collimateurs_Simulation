/// PALLAS_CollSimPrimaryGeneratorAction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimPrimaryGeneratorAction_h
#define PALLAS_CollSimPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "PALLAS_CollSimRunAction.hh"
#include "G4GenericMessenger.hh"
#include <G4RunManager.hh>
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>

class G4ParticleGun;
class G4Event;

class PALLAS_CollSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PALLAS_CollSimPrimaryGeneratorAction(const char*);
  ~PALLAS_CollSimPrimaryGeneratorAction();

public:
  void GeneratePrimaries(G4Event* anEvent);

private:
  G4GenericMessenger* pMessenger;

  G4bool StatusGunParticle;
  G4String PhaseSpaceFilename;
  G4String MacroChargeFilename;
  G4String ParticleName;
  G4double EnergyReference;

  size_t charToSizeT(const char* str);
  void GunParticleInitialization();
  void GPSInitialization();
  void AlwaysInSetVerification();
  void SetParticleName();
  void ShowProgress(double progress, std::chrono::high_resolution_clock::time_point startTime);
  void ReadParticleFile(const std::string& filename);
  void ReadNumberFile(const std::string& filename);
  int GetEventNumberOfParticles (int eventID);

  struct ParticleData {
    G4double x;
    G4double xp;
    G4double z;
    G4double zp;
    G4double s;
    G4double delta;
  };

  G4String s_NEventsGenerated;
  std::vector<ParticleData> particleDataList;
  std::vector<int> EventNumberOfParticles;
  G4ParticleGun* particleGun;
  G4GeneralParticleSource *particleSource;
  G4double IncidentE;
  G4ParticleDefinition* particleDefinition;
  size_t NEventsGenerated;
  size_t currentEvent;
  size_t currentParticleNumber;
  size_t TotalNParticles;
  G4int eventID;
  G4int nEvent;
  std::chrono::duration<double> elapsedTime;
  bool isStartTimeInitialized = false;
  std::chrono::high_resolution_clock::time_point startTime;
  double progressRemaining;
  double averageTimePerProgress;
  double estimatedRemainingTime;
};

#endif



