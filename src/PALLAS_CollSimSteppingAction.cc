/// PALLAS_CollSimSteppingAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimSteppingAction.hh"

using namespace CLHEP;

const G4String PALLAS_CollSimSteppingAction::path = "../simulation_input_files/";

PALLAS_CollSimSteppingAction::PALLAS_CollSimSteppingAction()
{
}

PALLAS_CollSimSteppingAction::~PALLAS_CollSimSteppingAction() {}

bool PALLAS_CollSimSteppingAction::SetFlagGammaEnergyDeposition()
{
  if (particleID == 22)
    flag_Gamma = true;

  if (creatorProcessName == "phot" || creatorProcessName == "compt" || creatorProcessName == "conv")
    flag_Gamma = true;
  else
    flag_Gamma = false;

  return flag_Gamma;
}

void PALLAS_CollSimSteppingAction::UpdateCollimatorInformations(PALLAS_CollSimEventAction *evtac)
{
  evtac->AddEdepCollimator(energyDeposited);
  if (flag_Gamma == false)
    evtac->AddEdepElectronCollimator(energyDeposited);
  else
    evtac->AddEdepGammaCollimator(energyDeposited);

  // G4cout << "Edep = " << energyDeposited << G4endl;
  // G4cout << "Edep tot = " << evtac->GetEdepCollimator() << G4endl;
  // G4cout << "Edep tot e- = " << evtac->GetEdepElectronCollimator() << G4endl;
  // G4cout << "Edep tot gamma = " << evtac->GetEdepGammaCollimator() << G4endl;
}

void PALLAS_CollSimSteppingAction::UpdateFrontCollimatorInformations(PALLAS_CollSimEventAction *evtac)
{
  evtac->AddParticleIDFront(particleID);
  evtac->AddParentIDFront(parentID);
  evtac->AddEnergyExitFront(energy);
  evtac->AddXExitFront(x);
  evtac->AddYExitFront(y);
  evtac->AddZExitFront(z);
  evtac->AddPxExitFront(px);
  evtac->AddPyExitFront(py);
  evtac->AddPzExitFront(pz);

  // G4cout << "Part ID = " << particleID << G4endl;
  // G4cout << "E exit = " << energy << G4endl;
  // G4cout << "X exit = " << x << G4endl;
  // G4cout << "Y exit = " << y << G4endl;
  // G4cout << "Z exit = " << z << G4endl;
  // G4cout << "Px exit = " << px << G4endl;
  // G4cout << "Py exit = " << py << G4endl;
  // G4cout << "Pz exit = " << pz << G4endl;
}

void PALLAS_CollSimSteppingAction::UpdateBackCollimatorInformations(PALLAS_CollSimEventAction *evtac)
{
  evtac->AddParticleIDBack(particleID);
  evtac->AddParentIDBack(parentID);
  evtac->AddEnergyExitBack(energy);
  evtac->AddXExitBack(x);
  evtac->AddYExitBack(y);
  evtac->AddZExitBack(z);
  evtac->AddPxExitBack(px);
  evtac->AddPyExitBack(py);
  evtac->AddPzExitBack(pz);

  // G4cout << "Part ID = " << particleID << G4endl;
  // G4cout << "E exit = " << energy << G4endl;
  // G4cout << "X exit = " << x << G4endl;
  // G4cout << "Y exit = " << y << G4endl;
  // G4cout << "Z exit = " << z << G4endl;
  // G4cout << "Px exit = " << px << G4endl;
  // G4cout << "Py exit = " << py << G4endl;
  // G4cout << "Pz exit = " << pz << G4endl;
}

void PALLAS_CollSimSteppingAction::UpdateBremInformations(PALLAS_CollSimEventAction *evtac)
{
  if (creatorProcessName == "eBrem" && stepNo == 1)
  {
    evtac->AddEBremCreated(energy);
    //G4cout << "Brem[" << evtac->GetEBremCreatedSize() << "]= " << evtac->GetEBremCreatedEnergy(evtac->GetEBremCreatedSize() - 1) << " MeV" << G4endl;
  }
}

void PALLAS_CollSimSteppingAction::UserSteppingAction(const G4Step *aStep)
{

  // ###################################
  //  Déclaration of functions/variables
  // ###################################
  PALLAS_CollSimRunAction *runac = (PALLAS_CollSimRunAction *)(G4RunManager::GetRunManager()->GetUserRunAction());
  G4EventManager *evtman = G4EventManager::GetEventManager();
  PALLAS_CollSimEventAction *evtac = (PALLAS_CollSimEventAction *)evtman->GetUserEventAction();

  theTrack = aStep->GetTrack();
  particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();
  particleID = aStep->GetTrack()->GetDefinition()->GetPDGEncoding(); // PART ID => https://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf
  endproc = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  auto creatorProcess = theTrack->GetCreatorProcess();
  parentID = aStep->GetTrack()->GetParentID();
  stepNo = aStep->GetTrack()->GetCurrentStepNumber();
  energy = aStep->GetPreStepPoint()->GetKineticEnergy() / MeV;
  volumeNamePreStep = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  volumeNamePostStep = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();

  energyDeposited = aStep->GetTotalEnergyDeposit() / MeV;
  x = aStep->GetTrack()->GetPosition().x();
  y = aStep->GetTrack()->GetPosition().y();
  z = aStep->GetTrack()->GetPosition().z();
  px = aStep->GetPreStepPoint()->GetMomentumDirection().x();
  py = aStep->GetPreStepPoint()->GetMomentumDirection().y();
  pz = aStep->GetPreStepPoint()->GetMomentumDirection().z();

  // #######################################################################
  // #######################################################################
  // ###########################START EM INFOS PART#########################
  // #######################################################################
  // #######################################################################

  if (parentID == 0 && stepNo == 1)
    evtac->SetEstartCollimator(energy);

  if (creatorProcess != NULL)
  {
    creatorProcessName = creatorProcess->GetProcessName();
    SetFlagGammaEnergyDeposition();
    UpdateBremInformations(evtac);
  }

  if (volumeNamePreStep == "Collimator")
    UpdateCollimatorInformations(evtac);

  if (volumeNamePreStep == "Collimator" && volumeNamePostStep == "FrontOutput")
    UpdateFrontCollimatorInformations(evtac);

  if (volumeNamePreStep == "Collimator" && volumeNamePostStep == "BackOutput")
    UpdateBackCollimatorInformations(evtac);

  if (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "World")
    theTrack->SetTrackStatus(fStopAndKill);
}