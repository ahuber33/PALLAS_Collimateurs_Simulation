// Geometry.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "Geometry.hh"

using namespace CLHEP;


// ***********************
// Constructor
// ***********************
const G4String Geometry::path_bin = "../bin/";

Geometry::Geometry(G4String buildfile)
{

  // Read keys and values from file buildfile defined in x_blockGeometry.cc
  // Some of these variables may not be needed.
  std::ifstream config;
  config.open(buildfile);
  if (!config.is_open())
    G4cout << "Error opening file " << buildfile << G4endl;
  else
  {
    while (config.is_open())
    {
      G4String variable;
      G4String unit;
      G4double value;

      config >> variable;
      if (!config.good())
        break;
      // ####################### COMMON variables ###########################
      if (variable == "CollimatorInternalRadius")
      {
        config >> value >> unit;
        //CollimatorInternalRadius = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "CollimatorExternalRadius")
      {
        config >> value >> unit;
        CollimatorExternalRadius = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "CollimatorThickness")
      {
        config >> value >> unit;
        CollimatorThickness = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "CollimatorLength")
      {
        config >> value >> unit;
        CollimatorLength = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "CollimatorWidth")
      {
        config >> value >> unit;
        CollimatorWidth = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "OutputThickness")
      {
        config >> value >> unit;
        OutputThickness = value * G4UnitDefinition::GetValueOf(unit);
      }
    }
    config.close();

    G4cout << "\n The Variables that we read in are: "
           << "\n CollimatorInternalRadius = " << CollimatorInternalRadius
           << "\n CollimatorExternalRadius = " << CollimatorExternalRadius
           << "\n CollimatorThickness = " << CollimatorThickness
           << "\n CollimatorLength = " << CollimatorLength
           << "\n CollimatorWidth = " << CollimatorWidth
           << "\n OutputThickness = " << OutputThickness
           << "\n " << G4endl;
  }
}
  // ***********************
  // Destructor
  // ***********************
  Geometry::~Geometry()
  {
  }

  G4LogicalVolume *Geometry::GetCollimator()
  {

    Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_BRASS");

    G4Box *Box = new G4Box("Box",                                                                     // its name
                           CollimatorLength / 2, CollimatorWidth / 2, CollimatorThickness / 2); // its size

    LogicalVolume = new G4LogicalVolume(Box, Material, "Collimator", 0, 0, 0);

    return LogicalVolume;
  }


  G4LogicalVolume *Geometry::GetOutputCollimator()
  {

    Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    G4Box *Box = new G4Box("Box",                                                                     // its name
                           1*m / 2, 1*m / 2, OutputThickness/2); // its size


    LogicalVolume = new G4LogicalVolume(Box, Material, "OutputCollimator", 0, 0, 0);

    return LogicalVolume;
  }


  G4LogicalVolume *Geometry::GetRoundCollimator(G4double CollimatorInternalRadius)
  {
    //SimGeometry = new PALLAS_CollSimGeometry();
    //CollimatorInternalRadius = SimGeometry->GetCollimatorInternalRadius();
    Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_BRASS");

    //SetCollimatorInternalRadius(5*mm);

    G4cout << "\n\nCollimatorInternal Radius = " << CollimatorInternalRadius << G4endl;

    G4Tubs *Tubs = new G4Tubs("Tubs",                                                                                     // its name
                              CollimatorInternalRadius, CollimatorExternalRadius, CollimatorThickness / 2, 0, 360 * deg); // its size

    LogicalVolume = new G4LogicalVolume(Tubs, Material, "RoundCollimator", 0, 0, 0);

    //G4RunManager::GetRunManager()->GeometryHasBeenModified();

    return LogicalVolume;
  }

  // Alu = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  // Inox = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  // Water = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
  // Laiton = G4NistManager::Instance()->FindOrBuildMaterial("G4_BRASS");
  // Kapton = G4NistManager::Instance()->FindOrBuildMaterial("G4_KAPTON");
  // Air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  // Cuivre = G4NistManager::Instance()->FindOrBuildMaterial("G4_Cu");
  // Fer = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
  // Tantale = G4NistManager::Instance()->FindOrBuildMaterial("G4_Ta");
  // Tungstene = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
  // Polystyrene = G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYSTYRENE");
