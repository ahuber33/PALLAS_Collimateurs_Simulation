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
void PALLAS_CollSimSteppingAction::UserSteppingAction(const G4Step *aStep)
{

  // ###################################
  //  Déclaration of functions/variables
  // ###################################
  G4Track *theTrack = aStep->GetTrack();
  //PALLAS_CollSimTrackInformation *trackInformation = (PALLAS_CollSimTrackInformation *)theTrack->GetUserInformation();
  G4String partname = aStep->GetTrack()->GetDefinition()->GetParticleName();
  G4int partID = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
  PALLAS_CollSimRunAction *runac = (PALLAS_CollSimRunAction *)(G4RunManager::GetRunManager()->GetUserRunAction());
  G4EventManager *evtman = G4EventManager::GetEventManager();
  PALLAS_CollSimEventAction *evtac = (PALLAS_CollSimEventAction *)evtman->GetUserEventAction();
  //PALLAS_CollSimTrackInformation *info = (PALLAS_CollSimTrackInformation *)(aStep->GetTrack()->GetUserInformation());
  G4String endproc = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  G4int Parent_ID = aStep->GetTrack()->GetParentID();
  G4int StepNo = aStep->GetTrack()->GetCurrentStepNumber();

  G4double x = aStep->GetTrack()->GetPosition().x();
  G4double y = aStep->GetTrack()->GetPosition().y();
  G4double z = aStep->GetTrack()->GetPosition().z();
  G4double zpre = aStep->GetPreStepPoint()->GetPosition().z();
  G4double px = aStep->GetPreStepPoint()->GetMomentumDirection().x();
  G4double py = aStep->GetPreStepPoint()->GetMomentumDirection().y();
  G4double pz = aStep->GetPreStepPoint()->GetMomentumDirection().z();
  G4double px_exit;
  G4double py_exit;
  G4double pz_exit;
  G4double r = sqrt(x * x + y * y);
  G4double angle = acos((z - zpre) / aStep->GetStepLength());
  G4String VolumeNamePreStep = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
  G4String VolumeNamePostStep = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
  
  // #######################################################################
  // #######################################################################
  // ###########################START EM INFOS PART#########################
  // #######################################################################
  // #######################################################################

  if (Parent_ID == 0 && StepNo == 1)
  {
    evtac->SetEstartTP(aStep->GetPreStepPoint()->GetKineticEnergy() / MeV);
    evtac->SetParticuleID(partID);
    evtac->SetCharge(aStep->GetPostStepPoint()->GetCharge());
  }

  if (((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Scintillator") || (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "ZnS") || (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Core_Fiber")) && ((aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "Scintillator") || (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "ZnS") || (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "Core_Fiber")) && partname == "proton")
  {
    evtac->AddTrackLength(aStep->GetTrack()->GetStepLength() / mm);
    // G4cout << "Track Length = " << evtac->GetTotalTrackLength() << G4endl;
    evtac->AddEdepTP(aStep->GetTotalEnergyDeposit() / keV);
  }

  // Be careful here !!! If Zns in here, put ZnS. If not, put Scintillator or Core_Fiber!!!!
  if (Parent_ID == 0 && aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "Scintillator" && evtac->GetTPPositionZ() < 0)
  {
    evtac->SetTPPositionX(x);
    evtac->SetTPPositionY(y);
    evtac->SetTPPositionZ(z);
    evtac->SetTPTime(aStep->GetPostStepPoint()->GetGlobalTime() / ns);
    // G4cout << "x = " << x << G4endl;
    // G4cout << "y = " << y << G4endl;
    // G4cout << "z = " << z << G4endl;

    // if(x <-25){G4cout << "HERE X" << G4endl;}
    // if(y <-10){G4cout << "HERE Y" << G4endl;}
  }


  if (aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "World")
  {
    theTrack->SetTrackStatus(fStopAndKill);
  }

  // G4cout << "Charge = " << aStep->GetPostStepPoint()->GetCharge() << G4endl;
  // G4cout<< "Charge 2 = " << aStep->GetTrack()->GetDefinition()->GetPDGCharge()<<G4endl;
}
