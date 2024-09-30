/// Geometry.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

// This class is used to create a plastic Geometry.

#ifndef Geometry_h
#define Geometry_h

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Polyhedra.hh"
#include <G4Polycone.hh>
#include "G4Sphere.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4UnionSolid.hh"
#include "G4NistManager.hh"
#include "G4UnitsTable.hh"
#include "../PALLAS_CollSimGlobal.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImessenger.hh"

//#ifndef disable_gdml
#include "G4GDMLParser.hh"
//#endif

class PALLAS_CollSimMaterials;
class PALLAS_CollSimGeometryConstruction;
class G4GenericMessenger;
class Geometry

{
protected:

public:
  // constructor, builds from keys specified in buildfile
  Geometry();
  ~Geometry();

public:
  G4LogicalVolume* GetCollimator(G4String name);
  G4LogicalVolume *GetOutputCollimator();
  G4LogicalVolume *GetRoundCollimator();
  G4LogicalVolume *GetBFieldVolume();
  G4LogicalVolume *GetOutputSpectrometer();

  G4LogicalVolume *GetPALLAS_Assemblage2Cellules();
  //LIF
  G4LogicalVolume *GetPALLAS_Croix();
  G4LogicalVolume *GetPALLAS_LIFHublot1();
  G4LogicalVolume *GetPALLAS_LIFHublot2();
  G4LogicalVolume *GetPALLAS_LIFHublot3();
  G4LogicalVolume *GetPALLAS_LIFHublot4();
  G4LogicalVolume *GetPALLAS_LIFHublot5();
  G4LogicalVolume *GetPALLAS_LIF_IBX_DD();
  G4LogicalVolume *GetPALLAS_LIF_SQLT();
  G4LogicalVolume *GetPALLAS_MarbreBreadboard1();
  G4LogicalVolume *GetPALLAS_MarbreBreadboard2();
  G4LogicalVolume *GetPALLAS_OptoMeK();
  //SECTION 1 ASM Q1 - Chamber - Q2
  G4LogicalVolume *GetPALLAS_ATH500_DN100();
  G4LogicalVolume *GetPALLAS_BaseMarbre();
  G4LogicalVolume *GetPALLAS_ChambreISO();
  G4LogicalVolume *GetPALLAS_QuadrupoleQ1();
  G4LogicalVolume *GetPALLAS_QuadrupoleQ2();
  //SECTION 2 ASM Poutre Q3 - Removal Chamber - Q4
  G4LogicalVolume *GetPALLAS_QuadrupoleQ3();
  G4LogicalVolume *GetPALLAS_QuadrupoleQ4();
  G4LogicalVolume *GetPALLAS_ASMRemovalChamber();
  G4LogicalVolume *GetPALLAS_BreadboardRemovalChamber();
  G4LogicalVolume *GetPALLAS_ChassisRemovalChamber();
  G4LogicalVolume *GetPALLAS_TubeISO1();
  G4LogicalVolume *GetPALLAS_TubeISO2();
  //SECTION 3
  G4LogicalVolume *GetPALLAS_ASMPoutre();
  G4LogicalVolume *GetPALLAS_StationYAG();
  //SECTION 4
  G4LogicalVolume *GetPALLAS_BlindageBD();
  G4LogicalVolume *GetPALLAS_BlindageCBD();
  G4LogicalVolume *GetPALLAS_ChambreDipole();
  G4LogicalVolume *GetPALLAS_ChassisDipoleYAG();
  G4LogicalVolume *GetPALLAS_DiagsChamber();
  G4LogicalVolume *GetFakeDiagsChamber();
  G4LogicalVolume *GetPALLAS_Dipole();
  G4LogicalVolume *GetPALLAS_BS1YAG();
  G4LogicalVolume *GetPALLAS_BSPEC1YAG();
  G4LogicalVolume *GetPALLAS_S4Tube();
  G4LogicalVolume *GetPALLAS_S4Tube1();
  G4LogicalVolume *GetPALLAS_S4Soufflet();
  G4LogicalVolume *GetPALLAS_S4Croix();

  void Cleanup();
  

  //****************COMMON********************
  G4double GetCollimatorThickness() { return CollimatorThickness; }
  G4double GetOutputThickness() { return OutputThickness; }
  //void SetCollimatorInternalRadius(G4double CollimatorInternalRadius);
  //G4double GetCollimatorInternalRadius() { return CollimatorInternalRadius; }

private:
  Geometry *Geom;
  PALLAS_CollSimMaterials *scintProp;
  PALLAS_CollSimGeometryConstruction *SimGeometry;
  G4GenericMessenger* gMessenger;
  G4Material *Material;
  G4LogicalVolume* LogicalVolume;  // Garde la trace de l'allocation
  G4Box* Box;  // Garde la trace de l'allocation
  G4Tubs* Tubs;  // Garde la trace de l'allocation
  G4GDMLParser* parser;

  static const G4String path_bin;

  // Physical Dimensions
  G4double CollimatorInternalRadius;
  G4double CollimatorExternalRadius;
  G4double CollimatorThickness=5.0*mm;
  G4double CollimatorLength;
  G4double CollimatorWidth;
  G4double OutputThickness;
  
  // Other
  G4VisAttributes *clear;
};
#endif
