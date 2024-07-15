//// PALLAS_CollSimMagneticfield.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimMagneticField.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PALLAS_CollSimMagneticField::PALLAS_CollSimMagneticField()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PALLAS_CollSimMagneticField::~PALLAS_CollSimMagneticField()
{}

void PALLAS_CollSimMagneticField::GetFieldValue(const G4double[4], double* bField) const
{
  bField[0] = -ConstantBField;
  bField[1] = 0.;
  bField[2] = 0.;

  //G4cout << "\n\n\nBField value = " << ConstantBField << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PALLAS_CollSimMagneticField::SetField(G4double fieldValue)
{
  ConstantBField = fieldValue;
  //G4cout << "SETFIELD FONCTION : " << ConstantBField << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


