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

//#ifndef disable_gdml
#include "G4GDMLParser.hh"
//#endif

class PALLAS_CollSimMaterials;
class PALLAS_CollSimGeometry;
class G4GenericMessenger;
class Geometry

{
protected:
G4GenericMessenger *fMessenger;
double thickness_catcher;

public:
  // constructor, builds from keys specified in buildfile
  Geometry(G4String buildfile);
  ~Geometry();

public:
  G4LogicalVolume *GetCollimator();
  G4LogicalVolume *GetOutputCollimator();
  G4LogicalVolume *GetRoundCollimator(G4double CollimatorInternalRadius);

  //****************COMMON********************
  G4double GetCollimatorThickness() { return CollimatorThickness; }
  G4double GetOutputThickness() { return OutputThickness; }
  //void SetCollimatorInternalRadius(G4double CollimatorInternalRadius);
  //G4double GetCollimatorInternalRadius() { return CollimatorInternalRadius; }

private:
  Geometry *Geom;
  PALLAS_CollSimMaterials *scintProp;
  PALLAS_CollSimGeometry *SimGeometry;

  static const G4String path_bin;

  // Materials
  G4Material *Material;

  // Logical Volumes
  G4LogicalVolume *LogicalVolume;

  // Physical Dimensions
  G4double CollimatorInternalRadius=0*mm;
  G4double CollimatorExternalRadius;
  G4double CollimatorThickness;
  G4double CollimatorLength;
  G4double CollimatorWidth;
  G4double OutputThickness;
  
  // Other
  G4VisAttributes *clear;
};
#endif
