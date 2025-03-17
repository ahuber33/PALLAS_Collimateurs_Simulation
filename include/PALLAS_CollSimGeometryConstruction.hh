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
#include "G4GeometryTolerance.hh"
#include "G4AutoLock.hh"
#include "PALLAS_CollSimMagneticField.hh"
#include "G4SystemOfUnits.hh"

class Geometry;
class G4FieldManager;

class PALLAS_CollSimMagneticField;

class PALLAS_CollSimGeometryConstruction : public G4VUserDetectorConstruction
{
public:
  PALLAS_CollSimGeometryConstruction();
  ~PALLAS_CollSimGeometryConstruction();

public:
  void SetLogicalVolumeColor(G4LogicalVolume* LogicalVolume, G4String color);
  void CreateWorldAndHolder();
  void ConstructCollimatorWithOutput();
  void ConstructVerticalCollimator();
  void ConstructHorizontalCollimator();
  void ConstructCellulePart();
  void ConstructLIFPart();
  void ConstructSection1Part();
  void ConstructSection2Part();
  void ConstructSection3Part();
  void ConstructSection4Part();
  void ConstructSection4DumpPart();
  void ConstructSDandField() override;
  G4VPhysicalVolume *Construct() override;
  

private:
  Geometry *Geom;
  G4Material *Vacuum;
  G4Material *Material;
  
  G4GenericMessenger* fMessenger;
  G4GenericMessenger* gMessenger;
  G4GenericMessenger* bMessenger;

  G4bool StatusDisplayCelluleGeometry=false;
  G4bool StatusDisplayLIFGeometry=false;
  G4bool StatusDisplaySection1Geometry=false;
  G4bool StatusDisplaySection2Geometry=false;
  G4bool StatusDisplaySection3Geometry=false;
  G4bool StatusDisplaySection4Geometry=false;
  G4bool StatusDisplaySection4DumpGeometry=false;
  G4bool StatusRoundCollimator=false;
  G4bool StatusMapBField=false;
  G4String CollimatorMaterial = "G4_Al";
  G4String VerticalCollimatorMaterial = "G4_Al";
  G4String HorizontalCollimatorMaterial = "G4_Al";

  // Dimension values
  G4double CollimatorThickness=1.0*mm;
  G4double VerticalCollimatorThickness=1.0*mm;
  G4double HorizontalCollimatorThickness=1.0*mm;
  G4double OutputThickness;
  G4double CollimatorInternalRadius=0*mm;
  G4double CollimatorExternalRadius=20*mm;
  G4double CollimatorSpectrometerDistance=0*mm;
  G4double ConstantDipoleBField =0.4*tesla;
  G4double Q1Gradient =0.0;
  G4double Q2Gradient =0.0;
  G4double Q3Gradient =0.0;
  G4double Q4Gradient =0.0;
  G4double CollimatorLength=200*mm;
  G4double CollimatorDistanceBetweenPlates = 10*mm;
  G4double OpenVerticalCollimator = 10*mm;
  G4double OpenHorizontalCollimator = 10*mm;
  G4double CollimatorVHDistance = 0*mm;

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
  G4LogicalVolume *LogicalWorld=nullptr;
  G4LogicalVolume *LogicalHolder=nullptr;
  G4LogicalVolume *LogicalCollimator=nullptr;
  G4LogicalVolume *LogicalVerticalCollimator=nullptr;
  G4LogicalVolume *LogicalHorizontalCollimator=nullptr;
  G4LogicalVolume *LogicalFrontOutput=nullptr;
  G4LogicalVolume *LogicalBackOutput=nullptr;
  G4LogicalVolume *LogicalFakeDiagsChamber=nullptr;
  G4LogicalVolume *LogicalPALLAS_QuadrupoleQ3=nullptr;
  G4LogicalVolume *LogicalPALLAS_QuadrupoleQ4=nullptr;
  G4LogicalVolume *LogicalPALLAS_ASMRemovalChamber=nullptr;
  G4LogicalVolume *LogicalPALLAS_BreadboardRemovalChamber=nullptr;
  G4LogicalVolume *LogicalPALLAS_ChassisRemovalChamber=nullptr;
  G4LogicalVolume *LogicalPALLAS_TubeISO1=nullptr;
  G4LogicalVolume *LogicalPALLAS_TubeISO2=nullptr;
  G4LogicalVolume *LogicalPALLAS_ATH500_DN100=nullptr;
  G4LogicalVolume *LogicalPALLAS_BaseMarbre=nullptr;
  G4LogicalVolume *LogicalPALLAS_ChambreISO=nullptr;
  G4LogicalVolume *LogicalPALLAS_QuadrupoleQ1=nullptr;
  G4LogicalVolume *LogicalPALLAS_QuadrupoleQ2=nullptr;
  G4LogicalVolume *LogicalPALLAS_Croix=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIFHublot1=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIFHublot2=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIFHublot3=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIFHublot4=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIFHublot5=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIF_IBX_DD=nullptr;
  G4LogicalVolume *LogicalPALLAS_LIF_SQLT=nullptr;
  G4LogicalVolume *LogicalPALLAS_MarbreBreadboard1=nullptr;
  G4LogicalVolume *LogicalPALLAS_MarbreBreadboard2=nullptr;
  G4LogicalVolume *LogicalPALLAS_OptoMeK=nullptr;
  G4LogicalVolume *LogicalPALLAS_ASMPoutre=nullptr;
  G4LogicalVolume *LogicalPALLAS_StationYAG=nullptr;
  G4LogicalVolume *LogicalPALLAS_BlindageBD=nullptr;
  G4LogicalVolume *LogicalPALLAS_BlindageCBD=nullptr;
  G4LogicalVolume *LogicalPALLAS_ChambreDipole=nullptr;
  G4LogicalVolume *LogicalPALLAS_ChassisDipoleYAG=nullptr;
  G4LogicalVolume *LogicalPALLAS_DiagsChamber=nullptr;
  G4LogicalVolume *LogicalPALLAS_Dipole=nullptr;
  G4LogicalVolume *LogicalPALLAS_BS1YAG=nullptr;
  G4LogicalVolume *LogicalPALLAS_BSPEC1YAG=nullptr;
  G4LogicalVolume *LogicalPALLAS_S4Tube=nullptr;
  G4LogicalVolume *LogicalPALLAS_S4Tube1=nullptr;
  G4LogicalVolume *LogicalPALLAS_S4Soufflet=nullptr;
  G4LogicalVolume *LogicalPALLAS_S4Croix=nullptr;
  G4LogicalVolume *LogicalPALLAS_Assemblage2Cellules=nullptr;


  // Physical volumes
  G4VPhysicalVolume *PhysicalWorld=nullptr;
  G4VPhysicalVolume *PhysicalHolder=nullptr;
  G4VPhysicalVolume *PhysicalCollimator=nullptr;
  G4VPhysicalVolume *PhysicalCollimator1=nullptr;
  G4VPhysicalVolume *PhysicalCollimator2=nullptr;
  G4VPhysicalVolume *PhysicalVerticalCollimator=nullptr;
  G4VPhysicalVolume *PhysicalVerticalCollimator1=nullptr;
  G4VPhysicalVolume *PhysicalVerticalCollimator2=nullptr;
  G4VPhysicalVolume *PhysicalHorizontalCollimator=nullptr;
  G4VPhysicalVolume *PhysicalHorizontalCollimator1=nullptr;
  G4VPhysicalVolume *PhysicalHorizontalCollimator2=nullptr;
  G4VPhysicalVolume *PhysicalFrontOutput=nullptr;
  G4VPhysicalVolume *PhysicalBackOutput=nullptr;
  G4VPhysicalVolume *PhysicalFakeDiagsChamber=nullptr; 
  G4VPhysicalVolume *PhysicalPALLAS_QuadrupoleQ3=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_QuadrupoleQ4=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ASMRemovalChamber=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_BreadboardRemovalChamber=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ChassisRemovalChamber=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_TubeISO1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_TubeISO2=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ATH500_DN100=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_BaseMarbre=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ChambreISO=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_QuadrupoleQ1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_QuadrupoleQ2=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Croix=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot2=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot3=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot4=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIFHublot5=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIF_IBX_DD=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_LIF_SQLT=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_MarbreBreadboard1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_MarbreBreadboard2=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_OptoMeK=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ASMPoutre=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_StationYAG=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_BlindageBD=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_BlindageCBD=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ChambreDipole=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_ChassisDipoleYAG=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_DiagsChamber=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Dipole=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_BS1YAG=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_BSPEC1YAG=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_S4Tube=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_S4Tube1=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_S4Soufflet=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_S4Croix=nullptr;
  G4VPhysicalVolume *PhysicalPALLAS_Assemblage2Cellules=nullptr;

  // Dimensions PLACEMENTS
  G4double Y_FrontOutput;
  G4double Y_BackOutput;
  G4double Y_Collimator;

  //Rotation Matrix
  G4RotationMatrix DontRotate;
  G4RotationMatrix Flip;
  G4RotationMatrix* RotationMatrix;

  static G4ThreadLocal PALLAS_CollSimMagneticField* fMagneticField;
  static G4ThreadLocal G4FieldManager* fFieldMgr;




};
#endif
