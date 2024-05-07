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




G4LogicalVolume *Geometry::GetPALLAS_Assemblage2Cellules(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/Assemblage_2_Cellules.gdml", false);
  LogicalVolume = parser->GetVolume("Assemblage_2_Cellules");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


// ################################################################################################################
// ################################################################################################################
//LIF
// ################################################################################################################
// ################################################################################################################

G4LogicalVolume *Geometry::GetPALLAS_Croix(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/LIF/Croix.gdml", false);
  LogicalVolume = parser->GetVolume("Croix");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_LIFHublot1(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/LIF/LIF_Hublot1.gdml", false);
  LogicalVolume = parser->GetVolume("LIF_Hublot1");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_LIFHublot2(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/LIF/LIF_Hublot2.gdml", false);
  LogicalVolume = parser->GetVolume("LIF_Hublot2");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_LIFHublot3(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/LIF/LIF_Hublot3.gdml", false);
  LogicalVolume = parser->GetVolume("LIF_Hublot3");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}

G4LogicalVolume *Geometry::GetPALLAS_LIFHublot4(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/LIF/LIF_Hublot4.gdml", false);
  LogicalVolume = parser->GetVolume("LIF_Hublot4");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_LIFHublot5(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/LIF/LIF_Hublot5.gdml", false);
  LogicalVolume = parser->GetVolume("LIF_Hublot5");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_LIF_IBX_DD(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/LIF/LIF_IBX_DD.gdml", false);
  LogicalVolume = parser->GetVolume("LIF_IBX_DD");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_LIF_SQLT(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/LIF/LIF_SQLT.gdml", false);
  LogicalVolume = parser->GetVolume("LIF_SQLT");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_MarbreBreadboard1(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/LIF/Marbre_Breadboard1.gdml", false);
  LogicalVolume = parser->GetVolume("Marbre_Breadboard1");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_MarbreBreadboard2(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/LIF/Marbre_Breadboard2.gdml", false);
  LogicalVolume = parser->GetVolume("Marbre_Breadboard1");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_OptoMeK(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/LIF/OptoMeK.gdml", false);
  LogicalVolume = parser->GetVolume("OptoMeK");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}



// ################################################################################################################
// ################################################################################################################
//SECTION 1 ASM Q1 - Chamber - Q2
// ################################################################################################################
// ################################################################################################################

G4LogicalVolume *Geometry::GetPALLAS_ATH500_DN100(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S1/ATH500_DN100.gdml", false);
  LogicalVolume = parser->GetVolume("ATH500_DN100");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_BaseMarbre(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S1/Base_Marbre.gdml", false);
  LogicalVolume = parser->GetVolume("Base_Marbre");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_ChambreISO(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S1/Chambre_ISO.gdml", false);
  LogicalVolume = parser->GetVolume("Chambre_ISO");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_QuadrupoleQ1(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S1/Quadrupole_Q1.gdml", false);
  LogicalVolume = parser->GetVolume("Quadrupole_Q1");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}







// ################################################################################################################
// ################################################################################################################
//SECTION 2 ASM Poutre Q3 - Removal Chamber - Q4
// ################################################################################################################
// ################################################################################################################

G4LogicalVolume *Geometry::GetPALLAS_QuadrupoleQ2(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S1/Quadrupole_Q2.gdml", false);
  LogicalVolume = parser->GetVolume("Quadrupole_Q2");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


  G4LogicalVolume *Geometry::GetPALLAS_QuadrupoleQ3(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S2/QuadrupoleQ3.gdml", false);
  LogicalVolume = parser->GetVolume("Quadrupole_Q3");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_QuadrupoleQ4(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S2/QuadrupoleQ4.gdml", false);
  LogicalVolume = parser->GetVolume("Quadrupole_Q4");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_ASMRemovalChamber(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S2/ASM_Removal_Chamber.gdml", false);
  LogicalVolume = parser->GetVolume("ASMRemovalChamber");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_BreadboardRemovalChamber(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S2/Assemblage_Breadboard_Thorlabs_Removal_Chamber.gdml", false);
  LogicalVolume = parser->GetVolume("Breadboard_Removal_Chamber");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_ChassisRemovalChamber(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S2/Chassis_PALLAS_Removal_Chamber.gdml", false);
  LogicalVolume = parser->GetVolume("Chassis_PALLAS_Removal_Chamber");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_TubeISO1(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S2/Tube_ISO_1.gdml", false);
  LogicalVolume = parser->GetVolume("Tube_ISO_1");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_TubeISO2(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S2/Tube_ISO_2.gdml", false);
  LogicalVolume = parser->GetVolume("Tube_ISO_2");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}





// ################################################################################################################
// ################################################################################################################
//SECTION 3
// ################################################################################################################
// ################################################################################################################

G4LogicalVolume *Geometry::GetPALLAS_ASMPoutre(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S3/ASM_Poutre.gdml", false);
  LogicalVolume = parser->GetVolume("ASM_Poutre");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_StationYAG(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S3/Station_YAG.gdml", false);
  LogicalVolume = parser->GetVolume("Station_YAG");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}





// ################################################################################################################
// ################################################################################################################
//SECTION 4
// ################################################################################################################
// ################################################################################################################

G4LogicalVolume *Geometry::GetPALLAS_BlindageBD(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S4/Blindage_BD.gdml", false);
  LogicalVolume = parser->GetVolume("Blindage_BD");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_BlindageCBD(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S4/Blindage_CBD.gdml", false);
  LogicalVolume = parser->GetVolume("Blindage_CBD");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_ChambreDipole(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S4/Chambre_Dipole.gdml", false);
  LogicalVolume = parser->GetVolume("Chambre_Dipole");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_ChassisDipoleYAG(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S4/Chassis_Dipole_YAG.gdml", false);
  LogicalVolume = parser->GetVolume("Chassis_Dipole_YAG");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_DiagsChamber(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S4/Diags_Chamber.gdml", false);
  LogicalVolume = parser->GetVolume("Diags_Chamber");
  LogicalVolume->SetMaterial(Material);

  return LogicalVolume;
}


G4LogicalVolume *Geometry::GetPALLAS_Dipole(){

  Material = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4GDMLParser* parser = new G4GDMLParser();
  //Create Tesselated volume of "Bouchon"
  parser->Clear();
  parser->Read("../gdml_models/S4/Dipole.gdml", false);
  LogicalVolume = parser->GetVolume("Dipole");
  LogicalVolume->SetMaterial(Material);

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
