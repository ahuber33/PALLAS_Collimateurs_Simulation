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
      if (variable == "ScintillatorLength")
      {
        config >> value >> unit;
        ScintillatorLength = value * G4UnitDefinition::GetValueOf(unit);
      }
      if (variable == "ScintillatorWidth")
      {
        config >> value >> unit;
        ScintillatorWidth = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "ScintillatorThickness")
      {
        config >> value >> unit;
        ScintillatorThickness = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "ZnSThickness")
      {
        config >> value >> unit;
        ZnSThickness = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "DetectorLength")
      {
        config >> value >> unit;
        DetectorLength = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "DetectorWidth")
      {
        config >> value >> unit;
        DetectorWidth = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "DetectorThickness")
      {
        config >> value >> unit;
        DetectorThickness = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "AirGap")
      {
        config >> value >> unit;
        AirGap = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "GlassThickness")
      {
        config >> value >> unit;
        GlassThickness = value * G4UnitDefinition::GetValueOf(unit);
      }
      else if (variable == "WorkingDistance")
      {
        config >> value >> unit;
        WorkingDistance = value * G4UnitDefinition::GetValueOf(unit);
      }
    }
  }
  config.close();

  G4cout << "\n The Variables that we read in are: "
         << "\n ScintillatorLength = " << ScintillatorLength
         << "\n ScintillatorWidth = " << ScintillatorWidth
         << "\n ScintillatorThickness = " << ScintillatorThickness
         << "\n ZnSThickness = " << ZnSThickness
         << "\n DetectorLength = " << DetectorLength
         << "\n DetectorWidth = " << DetectorWidth
         << "\n DetectorThickness = " << DetectorThickness
         << "\n Air gap = " << AirGap
         << "\n Glass Thickness = " << GlassThickness
         << "\n WorkingDistance = " << WorkingDistance
         << "\n " << G4endl;
}
// ***********************
// Destructor
// ***********************
Geometry::~Geometry()
{
  if (scintProp)
  {
    delete scintProp;
    scintProp = 0;
  }
  if (clear)
  {
    delete clear;
    clear = 0;
  }
}

G4LogicalVolume *Geometry::GetSc()
{

  Material = scintProp->GetMaterial("EJ212");

  G4Box *Box = new G4Box("Box",                                                                     // its name
                         ScintillatorLength / 2, ScintillatorWidth / 2, ScintillatorThickness / 2); // its size

  LogicalVolume = new G4LogicalVolume(Box, Material, "Sc_Test", 0, 0, 0);

  return LogicalVolume;
}
