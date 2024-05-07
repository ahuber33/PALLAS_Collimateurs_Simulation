/// PALLAS_CollSimGeometry.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimGeometry_h
#define PALLAS_CollSimGeometry_h 1

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
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"


class PALLAS_CollSimGeometry : public G4VUserDetectorConstruction
{
public:
  PALLAS_CollSimGeometry();
  ~PALLAS_CollSimGeometry();

public:
  void SetLogicalVolumeColor(G4LogicalVolume* LogicalVolume, G4String color);
  void CreateWorldAndHolder();
  void SetCollimatorInternalRadius(G4double CollimatorInternalRadius);
  G4double GetCollimatorInternalRadius() { return CollimatorInternalRadius; }
  G4VPhysicalVolume *Construct();
  

private:
  static const G4String path_bin;
  static const G4String path;
  PALLAS_CollSimMaterials *scintProp;
  Geometry *Geom;
  G4Material *Vacuum;

  // Colors for visualizations
  G4VisAttributes *invis;
  G4VisAttributes *white;
  G4VisAttributes *gray;
  G4VisAttributes *black;
  G4VisAttributes *red;
  G4VisAttributes *orange;
  G4VisAttributes *yellow;
  G4VisAttributes *green;
  G4VisAttributes *cyan;
  G4VisAttributes *blue;
  G4VisAttributes *magenta;

  // Logical Volumes
  G4LogicalVolume *LogicalWorld;
  G4LogicalVolume *LogicalHolder;
  G4LogicalVolume *LogicalCollimator;
  G4LogicalVolume *LogicalFrontOutput;
  G4LogicalVolume *LogicalBackOutput;
  G4LogicalVolume *LogicalPALLAS_QuadrupoleQ3;
  G4LogicalVolume *LogicalPALLAS_QuadrupoleQ4;
  G4LogicalVolume *LogicalPALLAS_ASMRemovalChamber;
  G4LogicalVolume *LogicalPALLAS_BreadboardRemovalChamber;
  G4LogicalVolume *LogicalPALLAS_ChassisRemovalChamber;
  G4LogicalVolume *LogicalPALLAS_TubeISO1;
  G4LogicalVolume *LogicalPALLAS_TubeISO2;
  G4LogicalVolume *LogicalPALLAS_ATH500_DN100;
  G4LogicalVolume *LogicalPALLAS_BaseMarbre;
  G4LogicalVolume *LogicalPALLAS_ChambreISO;
  G4LogicalVolume *LogicalPALLAS_QuadrupoleQ1;
  G4LogicalVolume *LogicalPALLAS_QuadrupoleQ2;
  G4LogicalVolume *LogicalPALLAS_Croix;
  G4LogicalVolume *LogicalPALLAS_LIFHublot1;
  G4LogicalVolume *LogicalPALLAS_LIFHublot2;
  G4LogicalVolume *LogicalPALLAS_LIFHublot3;
  G4LogicalVolume *LogicalPALLAS_LIFHublot4;
  G4LogicalVolume *LogicalPALLAS_LIFHublot5;
  G4LogicalVolume *LogicalPALLAS_LIF_IBX_DD;
  G4LogicalVolume *LogicalPALLAS_LIF_SQLT;
  G4LogicalVolume *LogicalPALLAS_MarbreBreadboard1;
  G4LogicalVolume *LogicalPALLAS_MarbreBreadboard2;
  G4LogicalVolume *LogicalPALLAS_OptoMeK;
  G4LogicalVolume *LogicalPALLAS_ASMPoutre;
  G4LogicalVolume *LogicalPALLAS_StationYAG;
  G4LogicalVolume *LogicalPALLAS_BlindageBD;
  G4LogicalVolume *LogicalPALLAS_BlindageCBD;
  G4LogicalVolume *LogicalPALLAS_ChambreDipole;
  G4LogicalVolume *LogicalPALLAS_ChassisDipoleYAG;
  G4LogicalVolume *LogicalPALLAS_DiagsChamber;
  G4LogicalVolume *LogicalPALLAS_Dipole;
  G4LogicalVolume *LogicalPALLAS_Assemblage2Cellules;


  // Physical volumes
  G4VPhysicalVolume *PhysicalWorld;
  G4VPhysicalVolume *PhysicalHolder;
  G4VPhysicalVolume *PhysicalCollimator;
  G4VPhysicalVolume *PhysicalFrontOutput;
  G4VPhysicalVolume *PhysicalBackOutput;
  G4VPhysicalVolume *PhysicalPALLAS_QuadrupoleQ3;
  G4VPhysicalVolume *PhysicalPALLAS_QuadrupoleQ4;
  G4VPhysicalVolume *PhysicalPALLAS_ASMRemovalChamber;
  G4VPhysicalVolume *PhysicalPALLAS_BreadboardRemovalChamber;
  G4VPhysicalVolume *PhysicalPALLAS_ChassisRemovalChamber;
  G4VPhysicalVolume *PhysicalPALLAS_TubeISO1;
  G4VPhysicalVolume *PhysicalPALLAS_TubeISO2;
  G4VPhysicalVolume *PhysicalPALLAS_ATH500_DN100;
  G4VPhysicalVolume *PhysicalPALLAS_BaseMarbre;
  G4VPhysicalVolume *PhysicalPALLAS_ChambreISO;
  G4VPhysicalVolume *PhysicalPALLAS_QuadrupoleQ1;
  G4VPhysicalVolume *PhysicalPALLAS_QuadrupoleQ2;
  G4VPhysicalVolume *PhysicalPALLAS_Croix;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot1;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot2;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot3;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot4;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot5;
  G4VPhysicalVolume *PhysicalPALLAS_LIF_IBX_DD;
  G4VPhysicalVolume *PhysicalPALLAS_LIF_SQLT;
  G4VPhysicalVolume *PhysicalPALLAS_MarbreBreadboard1;
  G4VPhysicalVolume *PhysicalPALLAS_MarbreBreadboard2;
  G4VPhysicalVolume *PhysicalPALLAS_OptoMeK;
  G4VPhysicalVolume *PhysicalPALLAS_ASMPoutre;
  G4VPhysicalVolume *PhysicalPALLAS_StationYAG;
  G4VPhysicalVolume *PhysicalPALLAS_BlindageBD;
  G4VPhysicalVolume *PhysicalPALLAS_BlindageCBD;
  G4VPhysicalVolume *PhysicalPALLAS_ChambreDipole;
  G4VPhysicalVolume *PhysicalPALLAS_ChassisDipoleYAG;
  G4VPhysicalVolume *PhysicalPALLAS_DiagsChamber;
  G4VPhysicalVolume *PhysicalPALLAS_Dipole;
  G4VPhysicalVolume *PhysicalPALLAS_Assemblage2Cellules;
  
  // Dimension values
  G4double CollimatorThickness;
  G4double OutputThickness;
  G4double CollimatorInternalRadius;

  // Dimensions PLACEMENTS
  G4double Z_FrontOutput;
  G4double Z_BackOutput;
  G4double Z_Collimator;

  //Rotation Matrix
  G4RotationMatrix DontRotate;
  G4RotationMatrix Flip;

};
#endif
