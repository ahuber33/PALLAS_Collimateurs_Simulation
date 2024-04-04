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

//#ifndef disable_gdml
#include "G4GDMLParser.hh"
//#endif

class PALLAS_CollSimMaterials;
class Geometry

{
public:
  // constructor, builds from keys specified in buildfile
  Geometry(G4String buildfile);
  ~Geometry();

public:
  G4LogicalVolume *GetSc();
  //****************COMMON********************
  G4double GetAirGap() { return AirGap; }
  G4double GetScintillatorThickness() { return ScintillatorThickness; }
  G4double GetZnSThickness() { return ZnSThickness; }
  G4double GetDetectorThickness() { return DetectorThickness; }
  G4double GetWorkingDistance() { return WorkingDistance; }
  G4double GetGlassThickness() { return GlassThickness; }

private:
  Geometry *theScint;
  PALLAS_CollSimMaterials *scintProp;

  static const G4String path_bin;

  // Materials
  G4Material *Material;

  // Logical Volumes
  G4LogicalVolume *LogicalVolume;

  // Physical Dimensions
  G4double ScintillatorLength;
  G4double ScintillatorWidth;
  G4double ScintillatorThickness;
  G4double ZnSThickness;
  G4double DetectorLength;
  G4double DetectorWidth;
  G4double DetectorThickness;
  G4double WorkingDistance;
  G4double GlassThickness;
  G4double AirGap;

  // Other
  G4VisAttributes *clear;
};
#endif
