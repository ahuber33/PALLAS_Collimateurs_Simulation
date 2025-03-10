/// PALLAS_CollSimRunAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimActionInitialization.hh"
#include "PALLAS_CollSimPrimaryGeneratorAction.hh"
#include "PALLAS_CollSimRunAction.hh"
#include "PALLAS_CollSimEventAction.hh"
#include "PALLAS_CollSimSteppingAction.hh"
#include "PALLAS_CollSimGeometryConstruction.hh"
#include "G4MTRunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PALLAS_CollSimActionInitialization::PALLAS_CollSimActionInitialization(const char *suff, size_t N, size_t Ncores, std::vector<std::queue<ParticleData>> threadEventQueues, const std::vector<ParticleData>& data, G4bool pFileReader, G4bool pMT)
    : G4VUserActionInitialization(), 
      suffixe(suff),
      threadEventQueues(threadEventQueues),
      NEventsGenerated(N),
      numThreads(Ncores),
      fParticleData(data),
      flag_FileReader(pFileReader),
      flag_MT(pMT)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PALLAS_CollSimActionInitialization::~PALLAS_CollSimActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PALLAS_CollSimActionInitialization::BuildForMaster() const
{
    // Action pour le processus maître (uniquement utile en mode multithreading)
    SetUserAction(new PALLAS_CollSimRunAction(suffixe, flag_MT));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PALLAS_CollSimActionInitialization::Build() const
{
    // Initialisation du générateur primaire des événements
    SetUserAction(new PALLAS_CollSimPrimaryGeneratorAction(NEventsGenerated, numThreads, threadEventQueues, fParticleData, flag_FileReader, flag_MT));

    // Création et affectation des actions pour la simulation
    auto *runAction = new PALLAS_CollSimRunAction(suffixe, flag_MT);
    auto *eventAction = new PALLAS_CollSimEventAction(suffixe);
    
    // Assigner les actions utilisateur
    SetUserAction(runAction);
    SetUserAction(eventAction);
    SetUserAction(new PALLAS_CollSimSteppingAction());
}
