/// PALLAS_CollSimGeometryConstruction.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimGeometryConstruction_h
#define PALLAS_CollSimGeometryConstruction_h 1

#include "Geometry.hh"
#include "G4GenericMessenger.hh"
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
#include "G4ElectroMagneticField.hh"
#include "G4MagneticField.hh"
#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4EquationOfMotion.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4UniformElectricField.hh"
#include "G4EqMagElectricField.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"
#include "G4ConstRK4.hh"
#include "G4NystromRK4.hh"
#include "G4HelixMixedStepper.hh"
#include "G4ExactHelixStepper.hh"
#include "G4InterpolationDriver.hh"
#include "G4IntegrationDriver.hh"
#include "G4VIntegrationDriver.hh"
#include "G4BFieldIntegrationDriver.hh"
#include <G4HelixHeum.hh>
// Newest steppers - from Release 10.3-beta (June 2013)
#include "G4BogackiShampine23.hh"
#include "G4BogackiShampine45.hh"
#include "G4DormandPrince745.hh"
#include "G4DormandPrinceRK56.hh"
#include "G4DormandPrinceRK78.hh"
#include "G4TsitourasRK45.hh"

class Geometry;

class PALLAS_CollSimGeometryConstruction : public G4VUserDetectorConstruction
{
public:
  PALLAS_CollSimGeometryConstruction();
  ~PALLAS_CollSimGeometryConstruction();

public:
  void SetLogicalVolumeColor(G4LogicalVolume* LogicalVolume, G4String color);
  void CreateWorldAndHolder();
  void ConstructCollimator();
  void ConstructCollimatorWithOutput();
  void ConstructBField();
  void ConstructCellulePart();
  void ConstructLIFPart();
  void ConstructSection1Part();
  void ConstructSection2Part();
  void ConstructSection3Part();
  void ConstructSection4Part();
  void ConstructSection4DumpPart();
  G4VPhysicalVolume *Construct();
  

private:
  Geometry *Geom;
  G4Material *Vacuum;
  
  G4GenericMessenger* fMessenger;
  G4GenericMessenger* gMessenger;
  G4GenericMessenger* bMessenger;

  G4bool StatusDisplayCelluleGeometry;
  G4bool StatusDisplayLIFGeometry;
  G4bool StatusDisplaySection1Geometry;
  G4bool StatusDisplaySection2Geometry;
  G4bool StatusDisplaySection3Geometry;
  G4bool StatusDisplaySection4Geometry;
  G4bool StatusDisplaySection4DumpGeometry;
  G4bool StatusMapBField;

  // Dimension values
  G4double CollimatorThickness=1.0*mm;
  G4double OutputThickness;
  G4double CollimatorInternalRadius=0*mm;
  G4double CollimatorExternalRadius=20*mm;
  G4double CollimatorSpectrometerDistance=0*mm;
  G4double ConstantBField =0.4*tesla;

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
  G4LogicalVolume *LogicalBFieldVolume;
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
  G4LogicalVolume *LogicalPALLAS_BS1YAG;
  G4LogicalVolume *LogicalPALLAS_BSPEC1YAG;
  G4LogicalVolume *LogicalPALLAS_Assemblage2Cellules;


  // Physical volumes
  G4VPhysicalVolume *PhysicalWorld;
  G4VPhysicalVolume *PhysicalHolder;
  G4VPhysicalVolume *PhysicalCollimator;
  G4VPhysicalVolume *PhysicalFrontOutput;
  G4VPhysicalVolume *PhysicalBackOutput;
  G4VPhysicalVolume *PhysicalBFieldVolume; 
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
  G4VPhysicalVolume *PhysicalPALLAS_BS1YAG;
  G4VPhysicalVolume *PhysicalPALLAS_BSPEC1YAG;
  G4VPhysicalVolume *PhysicalPALLAS_Assemblage2Cellules;

  // Dimensions PLACEMENTS
  G4double Y_FrontOutput;
  G4double Y_BackOutput;
  G4double Y_Collimator;

  //Rotation Matrix
  G4RotationMatrix DontRotate;
  G4RotationMatrix Flip;
  G4RotationMatrix* RotationMatrix;

  G4MagneticField *localmagField;
  G4Mag_UsualEqRhs* fEquationlocal;
  G4MagIntegratorStepper* localfStepperMag;



};
#endif
