/// PALLAS_CollSimPhysics.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimPhysics.hh"

using namespace CLHEP;

// Taken from N06 and LXe examples in GEANT4

PALLAS_CollSimPhysics::PALLAS_CollSimPhysics() : G4VModularPhysicsList()
{
  // Here used the default cut value you have typed in
  defaultCutValue = 1 * mm; // 0.001

  SetVerboseLevel(1);

  // default physics
  particleList = new G4DecayPhysics();

  // default physics
  raddecayList = new G4RadioactiveDecayPhysics();

  // EM physics
  emPhysicsList = new G4EmStandardPhysics_option3();
  // emPhysicsList = new G4EmStandardPhysics();
}

PALLAS_CollSimPhysics::~PALLAS_CollSimPhysics()
{
  delete raddecayList;
  delete emPhysicsList;
}

void PALLAS_CollSimPhysics::ConstructParticle()
{
  // Here are constructed all particles you have chosen
  particleList->ConstructParticle();
  //  ions
  // G4IonConstructor iConstructor;
  // iConstructor.ConstructParticle();
}

void PALLAS_CollSimPhysics::ConstructProcess()
{
  // Transportation, electromagnetic and general processes
  AddTransportation();
  //  Electromagnetic physics list
  emPhysicsList->ConstructProcess();
  particleList->ConstructProcess();
  raddecayList->ConstructProcess();
}

void PALLAS_CollSimPhysics::SetCuts()
{
  if (verboseLevel > 1)
  {
    G4cout << "opticalPhysics::SetCuts:";
  }
  SetCutsWithDefault();
  //  SetCutValue(5*mm,"opticalphoton");
}
