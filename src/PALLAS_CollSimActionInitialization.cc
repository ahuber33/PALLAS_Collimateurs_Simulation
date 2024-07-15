/// PALLAS_CollSimRunAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimActionInitialization.hh"
#include "PALLAS_CollSimPrimaryGeneratorAction.hh"
#include "PALLAS_CollSimRunAction.hh"
#include "PALLAS_CollSimSteppingAction.hh"
#include "PALLAS_CollSimGeometryConstruction.hh"
#include "G4MTRunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PALLAS_CollSimActionInitialization::PALLAS_CollSimActionInitialization(const char *suff, size_t N, size_t Ncores, std::vector<std::queue<ParticleData>> threadEventQueues, const std::vector<ParticleData>& data)
                                                                        : G4VUserActionInitialization(), 
                                                                        suffixe(suff),
                                                                        threadEventQueues(threadEventQueues),
                                                                        NEventsGenerated(N),
                                                                        numThreads(Ncores),
                                                                        fParticleData(data)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PALLAS_CollSimActionInitialization::~PALLAS_CollSimActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PALLAS_CollSimActionInitialization::BuildForMaster() const
{
    SetUserAction(new PALLAS_CollSimRunAction(suffixe));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PALLAS_CollSimActionInitialization::Build() const
{

    SetUserAction(new PALLAS_CollSimPrimaryGeneratorAction(NEventsGenerated, numThreads, threadEventQueues, fParticleData));

    PALLAS_CollSimRunAction *runAction = new PALLAS_CollSimRunAction(suffixe);
    PALLAS_CollSimEventAction *eventAction = new PALLAS_CollSimEventAction(suffixe);
    SetUserAction(runAction);
    SetUserAction(eventAction);
    SetUserAction(new PALLAS_CollSimSteppingAction());
}
