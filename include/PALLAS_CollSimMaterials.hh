/// PALLAS_CollSimMaterials.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

// This class is used to create a plastic scintillator.

#ifndef PALLAS_CollSimMaterials_h
#define PALLAS_CollSimMaterials_h

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"

class PALLAS_CollSimMaterials
{
public:

  PALLAS_CollSimMaterials(G4String prop_buildfile);
  ~PALLAS_CollSimMaterials();
  void Construct();

public:
  G4Material *GetMaterial(G4String);


private:

  static const G4String path;

  G4Element* elementH;
  G4Element* elementC;
  G4Element* elementO;
  G4Element* elementF;
  G4Element* elementTi;
  G4Element* elementSi;
  G4Element* elementB;
  G4Element* elementNa;
  G4Element* elementAl;
  G4Element* elementN;
  G4Element* elementFe;
  G4Element* elementNi;
  G4Element* elementCr;
  G4Element* elementMn;
  G4Element* elementCu;
  G4Element* elementSn;
  G4Element* elementSb;
  G4Element* elementMg;
  G4Element* elementZn;
  G4Element* elementCl;
  G4Element* elementK;
  G4Element* elementPb;
  G4Element* elementLa;
  G4Element* elementBr;
  G4Element* elementS;
  G4Element* elementLu;
  G4Element* elementY;
  G4Element* elementCs;
  G4Element* elementI;
  G4Element* elementGd;
  G4Element* elementGa;

  G4Material *SiO2;
  G4Material *Silicium;
  G4Material *B2O3;
  G4Material *Na2O;
  G4Material *Al2O3;
  G4Material *Air;
  G4Material *Fer;
  G4Material *Inox;
  G4Material *Cuivre;
  G4Material *Carbon;
  G4Material *Kapton;
  G4Material *Acier;
  G4Material *N2;
  G4Material *H2O;
  G4Material *Polypropylene;
  G4Material *Compensation;
  G4Material *Papier;
  G4Material *Nylon;
  G4Material *teflon;
  G4Material *Neoprene;
  G4Material *Mica;
  G4Material *Plomb_Antimoine;
  G4Material *bs_glass;
  G4Material *polysty;
  G4Material *scintillator;
  G4Material *Vacuum;
  G4Material *plastic;
  G4Material *mylar;
  G4Material *PMMA;
  G4Material *Alu;
  G4Material *EJ212;
  
  
  G4double scintAbsorbconst;
  G4double scintIndexconst;
  G4double lightyield;
  G4double lightyieldZnS;
  G4double pWavelength;
  G4double ratio;
  G4float Air_Index;


};
#endif
