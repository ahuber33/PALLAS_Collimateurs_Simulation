/// PALLAS_CollSimPrimaryGeneratorAction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimPrimaryGeneratorAction_h
#define PALLAS_CollSimPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "PALLAS_CollSimRunAction.hh"
#include "G4GenericMessenger.hh"
#include <G4RunManager.hh>
#include "G4MTRunManager.hh"
#include <G4EventManager.hh>
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "ParticleData.hh"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <thread>  // Pour std::this_thread::get_id()
#include <queue>
#include <atomic>

class G4ParticleGun;
class G4Event;

class PALLAS_CollSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PALLAS_CollSimPrimaryGeneratorAction(size_t, size_t, std::vector<std::queue<ParticleData>>, const std::vector<ParticleData>&);
  ~PALLAS_CollSimPrimaryGeneratorAction();

public:
  void GeneratePrimaries(G4Event* anEvent);

private:
  G4GenericMessenger* pMessenger;

  G4bool StatusGunParticle=true;
  G4String MacroChargeFilename = "PALLAS_qm1411_preCollimateur.txt";
  G4String ParticleName="e-";
  G4double EnergyReference = 247*MeV;
  G4double YParticleGenerationOffset;

  G4double xOffset;
  G4double sOffset;
  G4double zOffset;
  
  void SetParticleName();
  void ShowProgress(double progress, std::chrono::high_resolution_clock::time_point startTime);

  G4String s_NEventsGenerated;
  G4ParticleGun* particleGun;
  G4GeneralParticleSource *particleSource;
  G4double IncidentE;
  G4ParticleDefinition* particleDefinition;
  size_t NEventsGenerated;
  size_t currentEvent;
  size_t currentParticleNumber;
  size_t numThreads;
  size_t TotalNParticles;
  G4int eventID;
  G4int nEvent;
  std::chrono::duration<double> elapsedTime;
  bool isStartTimeInitialized = false;
  std::chrono::high_resolution_clock::time_point startTime;
  double progressRemaining;
  double averageTimePerProgress;
  double estimatedRemainingTime;
  std::vector<ParticleData> fParticleData;

  std::vector<std::queue<ParticleData>> threadEventQueue;
  std::vector<std::atomic<bool>> threadRunning; // Indicateurs de continuation des threads

  
};

#endif



