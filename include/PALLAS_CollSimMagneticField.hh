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

// Taille du tableau de gradients
const size_t NumQuadrupoles = 4;

class PALLAS_CollSimMagneticField : public G4MagneticField
{
  public:
    PALLAS_CollSimMagneticField();
    ~PALLAS_CollSimMagneticField() override;

    void GetFieldValue(const G4double point[4], double* bField) const override;

    void SetDipoleField(G4double val);
        // Méthode généralisée pour définir le gradient d'un quadrupole
    void SetGradient(size_t index, G4double gradient);

    // Méthode pour obtenir le gradient d'un quadrupole (pour vérifier)
    G4double GetGradient(size_t index) const;
    void SetMapBFieldStatus(G4bool val);

  private:

    void DefineCommands();
    G4double ConstantDipoleBField = 0.0*CLHEP::tesla;
        // Tableau des gradients pour les quadrupoles Q1 à Q4
    std::array<G4double, NumQuadrupoles> gradients = {0.0, 0.0, 0.0, 0.0}; // tesla/meter
    G4bool StatusMapBField=false;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
