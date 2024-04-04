/// PALLAS_CollSimGeometry.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimGeometry_h
#define PALLAS_CollSimGeometry_h 1

class Geometry;
class PALLAS_CollSimMaterials;

#include "PALLAS_CollSimMaterials.hh"
#include "Geometry.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"


class PALLAS_CollSimGeometry : public G4VUserDetectorConstruction
{
public:
  PALLAS_CollSimGeometry();
  ~PALLAS_CollSimGeometry();

public:
  G4VPhysicalVolume *Construct();
  

private:
  static const G4String path_bin;
  static const G4String path;
  PALLAS_CollSimMaterials *scintProp;
  Geometry *theScint;
  G4Material *Vacuum;

  // Colors for visualizations
  G4VisAttributes *invis;
  G4VisAttributes *white;
  G4VisAttributes *gray;
  G4VisAttributes *gray_bis;
  G4VisAttributes *black;
  G4VisAttributes *black_bis;
  G4VisAttributes *red;
  G4VisAttributes *red_hot;
  G4VisAttributes *orange;
  G4VisAttributes *yellow;
  G4VisAttributes *green;
  G4VisAttributes *green_hot;
  G4VisAttributes *cyan;
  G4VisAttributes *blue;
  G4VisAttributes *magenta;

  // Logical Volumes
  G4LogicalVolume *LogicalWorld;
  G4LogicalVolume *LogicalHolder;
  G4LogicalVolume *LogicalSc;

  // Physical volumes
  G4VPhysicalVolume *PhysicalWorld;
  G4VPhysicalVolume *PhysicalHolder;
  G4VPhysicalVolume *PhysicalSc;
  
  // Dimension values
  G4double ScintillatorThickness;
  G4double AirGap;
  G4double ZnSThickness;
  G4double DetectorThickness;
  G4double WorkingDistance;
  G4double GlassThickness;

  // Dimensions PLACEMENTS
  G4double Z_Position_Sc;

};
#endif
