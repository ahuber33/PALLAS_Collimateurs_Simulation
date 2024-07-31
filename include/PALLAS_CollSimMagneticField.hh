/// PALLAS_CollSimMagneticField.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimMagneticField_h
#define PALLAS_CollSimMagneticfield_h 1

#include "G4MagneticField.hh"
#include "globals.hh"
#include "G4GenericMessenger.hh"

#include <CLHEP/Units/SystemOfUnits.h>
#include <TF1.h>
#include <TMath.h>


/// Magnetic field

class PALLAS_CollSimMagneticField : public G4MagneticField
{
  public:
    PALLAS_CollSimMagneticField();
    ~PALLAS_CollSimMagneticField() override;

    void GetFieldValue(const G4double point[4], double* bField) const override;

    void SetField(G4double val);
    void SetMapBFieldStatus(G4bool val);

  private:

    void DefineCommands();
    G4double ConstantBField = 0.0*CLHEP::tesla;
    G4bool StatusMapBField=false;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
