//// PALLAS_CollSimGeometry.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimGeometry.hh"

using namespace CLHEP;

const G4String PALLAS_CollSimGeometry::path_bin = "../bin/";
const G4String PALLAS_CollSimGeometry::path = "../simulation_input_files/";

// Constructor
PALLAS_CollSimGeometry::PALLAS_CollSimGeometry() {}

// Destructor
PALLAS_CollSimGeometry::~PALLAS_CollSimGeometry()
{
}

void PALLAS_CollSimGeometry::SetLogicalVolumeColor(G4LogicalVolume *LogicalVolume, G4String Color)
{
    // ***********************
    // Visualization Colors
    // ***********************
    // Create some colors for visualizations
    invis = new G4VisAttributes(G4Colour(255 / 255., 255 / 255., 255 / 255.));
    invis->SetVisibility(false);

    black = new G4VisAttributes(G4Colour(0, 0, 0, 0.9));
    //  black->SetForceWireframe(true);
    black->SetForceSolid(true);
    black->SetVisibility(true);

    white = new G4VisAttributes(G4Colour(1, 1, 1, 1.)); // Sets the color (can be looked up online)
    // white->SetForceWireframe(true); // Sets to wire frame mode for coloring the volume
    white->SetForceSolid(true); // Sets to solid mode for coloring the volume
    white->SetVisibility(true); // Makes color visible in visualization

    gray = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
    //  gray->SetForceWireframe(true);
    gray->SetForceSolid(true);
    gray->SetVisibility(true);

    red = new G4VisAttributes(G4Colour(1, 0, 0, 0.3));
    //  red->SetForceWireframe(true);
    red->SetForceSolid(true);
    red->SetVisibility(true);

    orange = new G4VisAttributes(G4Colour(1, 0.5, 0, 0.3));
    //  orange->SetForceWireframe(true);
    orange->SetForceSolid(true);
    orange->SetVisibility(true);

    yellow = new G4VisAttributes(G4Colour(1, 1, 0, 0.7));
    //  yellow->SetForceWireframe(true);
    yellow->SetForceSolid(true);
    yellow->SetVisibility(true);

    green = new G4VisAttributes(G4Colour(0, 1, 0, 0.3));
    //  green->SetForceWireframe(true);
    green->SetForceSolid(true);
    green->SetVisibility(true);

    cyan = new G4VisAttributes(G4Colour(0, 1, 1, 0.3));
    //  cyan->SetForceWireframe(true);
    cyan->SetForceSolid(true);
    cyan->SetVisibility(true);

    blue = new G4VisAttributes(G4Colour(0, 0, 1, 0.3));
    //  blue->SetForceWireframe(true);
    blue->SetForceSolid(true);
    blue->SetVisibility(true);

    magenta = new G4VisAttributes(G4Colour(1, 0, 1, 0.3));
    //  magenta->SetForceWireframe(true);
    // magenta->SetForceSolid(true);
    magenta->SetVisibility(true);

    if (Color == "invis")
    {
        LogicalVolume->SetVisAttributes(invis);
    }
    else if (Color == "black")
    {
        LogicalVolume->SetVisAttributes(black);
    }
    else if (Color == "white")
    {
        LogicalVolume->SetVisAttributes(white);
    }
    else if (Color == "gray")
    {
        LogicalVolume->SetVisAttributes(gray);
    }
    else if (Color == "red")
    {
        LogicalVolume->SetVisAttributes(red);
    }
    else if (Color == "orange")
    {
        LogicalVolume->SetVisAttributes(orange);
    }
    else if (Color == "yellow")
    {
        LogicalVolume->SetVisAttributes(yellow);
    }
    else if (Color == "green")
    {
        LogicalVolume->SetVisAttributes(green);
    }
    else if (Color == "cyan")
    {
        LogicalVolume->SetVisAttributes(cyan);
    }
    else if (Color == "blue")
    {
        LogicalVolume->SetVisAttributes(blue);
    }
    else if (Color == "magenta")
    {
        LogicalVolume->SetVisAttributes(magenta);
    }
}

void PALLAS_CollSimGeometry::CreateWorldAndHolder()
{
    Vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    // Create World Volume
    // This is just a big box to place all other logical volumes inside
    G4Box *SolidWorld = new G4Box("SolidWorld", 10.1 * m, 10.1* m, 10.1 * m);
    LogicalWorld = new G4LogicalVolume(SolidWorld, Vacuum, "LogicalWorld", 0, 0, 0);
    SetLogicalVolumeColor(LogicalWorld, "invis");

    G4Box *SolidHolder = new G4Box("SolidHolder", 10.05 * m, 10.05 * m, 10.05 * m);
    LogicalHolder = new G4LogicalVolume(SolidHolder, Vacuum, "LogicalHolder", 0, 0, 0);
    SetLogicalVolumeColor(LogicalHolder, "invis");

    // Place the world volume: center of world at origin (0,0,0)
    PhysicalWorld = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0, 0, 0)), "World", LogicalWorld, NULL, false, 0);

    PhysicalHolder = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                       LogicalHolder, "Holder",
                                       LogicalWorld, false, 0);
}

void PALLAS_CollSimGeometry::SetCollimatorInternalRadius(G4double value)
{
    CollimatorInternalRadius = value;
    G4cout << "\n\n Internal Radius = " << CollimatorInternalRadius << "\n\n"
           << G4endl;
    //G4RunManager::GetRunManager()->GeometryHasBeenModified();
}


G4VPhysicalVolume *PALLAS_CollSimGeometry::Construct()
{
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    CreateWorldAndHolder();

    // ***********************
    // Various dimensions
    // ***********************
    Geom = new Geometry(path_bin + "PALLAS_CollSim.cfg");
    CollimatorThickness = Geom->GetCollimatorThickness();
    OutputThickness = Geom->GetOutputThickness();
    // CollimatorInternalRadius = Geom->GetCollimatorInternalRadius();

    G4cout << "Collimator Internal Radius SimGeom = " << CollimatorInternalRadius << G4endl;

    // Define common rotations
    DontRotate.rotateX(0.0 * deg);
    Flip.rotateZ(0 * deg);
    Flip.rotateX(180 * deg);
    Flip.rotateY(0 * deg);

    // #########################
    //  DEFINE GEOMETRY VOLUMES#
    // #########################

    //*********************************
    // Build scint et wrapping volumes*
    //*********************** *********
    // Simply calls functions from Scintillator() class
    LogicalCollimator = Geom->GetRoundCollimator(CollimatorInternalRadius);
    LogicalFrontOutput = Geom->GetOutputCollimator();
    LogicalBackOutput = Geom->GetOutputCollimator();

    LogicalPALLAS_Assemblage2Cellules = Geom->GetPALLAS_Assemblage2Cellules();
    //LIF
    LogicalPALLAS_Croix = Geom->GetPALLAS_Croix();
    LogicalPALLAS_LIFHublot1 = Geom->GetPALLAS_LIFHublot1();
    LogicalPALLAS_LIFHublot2 = Geom->GetPALLAS_LIFHublot2();
    LogicalPALLAS_LIFHublot3 = Geom->GetPALLAS_LIFHublot3();
    LogicalPALLAS_LIFHublot4 = Geom->GetPALLAS_LIFHublot4();
    LogicalPALLAS_LIFHublot5 = Geom->GetPALLAS_LIFHublot5();
    LogicalPALLAS_LIF_IBX_DD = Geom->GetPALLAS_LIF_IBX_DD();
    LogicalPALLAS_LIF_SQLT = Geom->GetPALLAS_LIF_SQLT();
    LogicalPALLAS_MarbreBreadboard1 = Geom->GetPALLAS_MarbreBreadboard1();
    LogicalPALLAS_MarbreBreadboard2 = Geom->GetPALLAS_MarbreBreadboard2();
    LogicalPALLAS_OptoMeK = Geom->GetPALLAS_OptoMeK();
    //SECTION 1 ASM Q1 - Chamber - Q2
    LogicalPALLAS_QuadrupoleQ1 = Geom->GetPALLAS_QuadrupoleQ1();
    LogicalPALLAS_QuadrupoleQ2 = Geom->GetPALLAS_QuadrupoleQ2();
    LogicalPALLAS_ATH500_DN100 = Geom->GetPALLAS_ATH500_DN100();
    LogicalPALLAS_ChambreISO = Geom->GetPALLAS_ChambreISO();
    LogicalPALLAS_BaseMarbre = Geom->GetPALLAS_BaseMarbre();
    //SECTION 2 ASM Poutre Q3 - Removal Chamber - Q4
    LogicalPALLAS_QuadrupoleQ3 = Geom->GetPALLAS_QuadrupoleQ3();
    LogicalPALLAS_QuadrupoleQ4 = Geom->GetPALLAS_QuadrupoleQ4();
    LogicalPALLAS_ASMRemovalChamber = Geom->GetPALLAS_ASMRemovalChamber();
    LogicalPALLAS_BreadboardRemovalChamber = Geom->GetPALLAS_BreadboardRemovalChamber();
    LogicalPALLAS_ChassisRemovalChamber = Geom->GetPALLAS_ChassisRemovalChamber();
    LogicalPALLAS_TubeISO1 = Geom->GetPALLAS_TubeISO1();
    LogicalPALLAS_TubeISO2 = Geom->GetPALLAS_TubeISO2();
    //SECTION 3
    LogicalPALLAS_ASMPoutre = Geom->GetPALLAS_ASMPoutre();
    LogicalPALLAS_StationYAG = Geom->GetPALLAS_StationYAG();
    //SECTION 4
    LogicalPALLAS_BlindageBD = Geom->GetPALLAS_BlindageBD();
    LogicalPALLAS_BlindageCBD = Geom->GetPALLAS_BlindageCBD();
    LogicalPALLAS_ChambreDipole = Geom->GetPALLAS_ChambreDipole();
    LogicalPALLAS_ChassisDipoleYAG = Geom->GetPALLAS_ChassisDipoleYAG();
    LogicalPALLAS_DiagsChamber = Geom->GetPALLAS_DiagsChamber();
    LogicalPALLAS_Dipole = Geom->GetPALLAS_Dipole();
    

    // Set colors of various block materials
    // LogicalCollimator->SetVisAttributes(black);
    SetLogicalVolumeColor(LogicalCollimator, "red");
    SetLogicalVolumeColor(LogicalFrontOutput, "yellow");
    SetLogicalVolumeColor(LogicalBackOutput, "red");

    SetLogicalVolumeColor(LogicalPALLAS_Assemblage2Cellules, "magenta");
    //LIF
    SetLogicalVolumeColor(LogicalPALLAS_Croix, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIFHublot1, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIFHublot2, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIFHublot3, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIFHublot4, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIFHublot5, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIF_IBX_DD, "green");
    SetLogicalVolumeColor(LogicalPALLAS_LIF_SQLT, "green");
    SetLogicalVolumeColor(LogicalPALLAS_MarbreBreadboard1, "green");
    SetLogicalVolumeColor(LogicalPALLAS_MarbreBreadboard2, "green");
    SetLogicalVolumeColor(LogicalPALLAS_OptoMeK, "green");
    //SECTION 1 ASM Q1 - Chamber - Q2
    SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ1, "red");
    SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ2, "red");
    SetLogicalVolumeColor(LogicalPALLAS_ATH500_DN100, "red");
    SetLogicalVolumeColor(LogicalPALLAS_ChambreISO, "red");
    SetLogicalVolumeColor(LogicalPALLAS_BaseMarbre, "red");
    //SECTION 2 ASM Poutre Q3 - Removal Chamber - Q4
    SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ3, "cyan");
    SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ4, "cyan");
    SetLogicalVolumeColor(LogicalPALLAS_ASMRemovalChamber, "cyan");
    SetLogicalVolumeColor(LogicalPALLAS_BreadboardRemovalChamber, "cyan");
    SetLogicalVolumeColor(LogicalPALLAS_ChassisRemovalChamber, "cyan");
    SetLogicalVolumeColor(LogicalPALLAS_TubeISO1, "cyan");
    SetLogicalVolumeColor(LogicalPALLAS_TubeISO2, "cyan");
    //SECTION 3
    SetLogicalVolumeColor(LogicalPALLAS_ASMPoutre, "orange");
    SetLogicalVolumeColor(LogicalPALLAS_StationYAG, "orange");
    //SECTION 4
    SetLogicalVolumeColor(LogicalPALLAS_BlindageBD, "blue");
    SetLogicalVolumeColor(LogicalPALLAS_BlindageCBD, "blue");
    SetLogicalVolumeColor(LogicalPALLAS_ChambreDipole, "blue");
    SetLogicalVolumeColor(LogicalPALLAS_ChassisDipoleYAG, "blue");
    SetLogicalVolumeColor(LogicalPALLAS_DiagsChamber, "blue");
    SetLogicalVolumeColor(LogicalPALLAS_Dipole, "blue");


    // ############################
    //  DEFINE GEOMETRY PLACEMENTS#
    // ############################

    Z_Collimator = 0;
    Z_FrontOutput = Z_Collimator - OutputThickness / 2 - CollimatorThickness / 2;
    Z_BackOutput = Z_Collimator + OutputThickness / 2 + CollimatorThickness / 2;

#ifndef disable_gdml

    // PhysicalFrontOutput = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, Z_FrontOutput)),
    //                                         LogicalFrontOutput, "FrontOutput",
    //                                         LogicalHolder, false, 0);

    // PhysicalCollimator = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, Z_Collimator)),
    //                                        LogicalCollimator, "Collimator",
    //                                        LogicalHolder, false, 0);

    // PhysicalBackOutput = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, Z_BackOutput)),
    //                                        LogicalBackOutput, "BackOutput",
    //                                        LogicalHolder, false, 0);


    PhysicalPALLAS_Assemblage2Cellules = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_Assemblage2Cellules, "Assemblage2Cellules",
                                           LogicalHolder, false, 0);  

    //LIF
    PhysicalPALLAS_Croix = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_Croix, "Croix",
                                           LogicalHolder, false, 0);  

    PhysicalPALLAS_OptoMeK = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_OptoMeK, "OptoMeK",
                                           LogicalHolder, false, 0);

    PhysicalPALLAS_LIFHublot1 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_LIFHublot1, "LIFHublot1",
                                           LogicalHolder, false, 0);                                       

    PhysicalPALLAS_LIFHublot2 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_LIFHublot2, "LIFHublot2",
                                           LogicalHolder, false, 0);                                       

    PhysicalPALLAS_LIFHublot3 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_LIFHublot3, "LIFHublot3",
                                           LogicalHolder, false, 0);                                       

    PhysicalPALLAS_LIFHublot4 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_LIFHublot4, "LIFHublot4",
                                           LogicalHolder, false, 0);                                       

    PhysicalPALLAS_LIFHublot5 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_LIFHublot5, "LIFHublot5",
                                           LogicalHolder, false, 0);                                                                                                                                                                                                                   

    PhysicalPALLAS_LIF_IBX_DD = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_LIF_IBX_DD, "LIF_IBX_DD",
                                           LogicalHolder, false, 0);

    PhysicalPALLAS_LIF_SQLT = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_LIF_SQLT, "LIF_SQLT",
                                           LogicalHolder, false, 0);                                                                                  

    PhysicalPALLAS_MarbreBreadboard1 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_MarbreBreadboard1, "MarbreBreadboard1",
                                           LogicalHolder, false, 0);                                       

    PhysicalPALLAS_MarbreBreadboard2 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_MarbreBreadboard2, "MarbreBreadboard2",
                                           LogicalHolder, false, 0);                                                                                  

    //SECTION 1 ASM Q1 - Chamber - Q2
    PhysicalPALLAS_QuadrupoleQ1 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_QuadrupoleQ1, "QuadrupoleQ1",
                                           LogicalHolder, false, 0);  

    PhysicalPALLAS_QuadrupoleQ2 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_QuadrupoleQ2, "QuadrupoleQ2",
                                           LogicalHolder, false, 0);   

    PhysicalPALLAS_ATH500_DN100 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_ATH500_DN100, "ATH500_DN100",
                                           LogicalHolder, false, 0); 

    PhysicalPALLAS_BaseMarbre = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_BaseMarbre, "BaseMarbre",
                                           LogicalHolder, false, 0);    

    PhysicalPALLAS_ChambreISO = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_ChambreISO, "ChambreISO",
                                           LogicalHolder, false, 0);        


    //SECTION 2 ASM Poutre Q3 - Removal Chamber - Q4
    PhysicalPALLAS_QuadrupoleQ3 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_QuadrupoleQ3, "QuadrupoleQ3",
                                           LogicalHolder, false, 0);                                           

    PhysicalPALLAS_QuadrupoleQ4 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_QuadrupoleQ4, "QuadrupoleQ4",
                                           LogicalHolder, false, 0);                                           

    PhysicalPALLAS_ASMRemovalChamber = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_ASMRemovalChamber, "ASMRemovalChamber",
                                           LogicalHolder, false, 0);                                           

    PhysicalPALLAS_BreadboardRemovalChamber = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_BreadboardRemovalChamber, "BreadboardRemovalChamber",
                                           LogicalHolder, false, 0);                                                                                      

    PhysicalPALLAS_ChassisRemovalChamber = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_ChassisRemovalChamber, "ChassisRemovalChamber",
                                           LogicalHolder, false, 0);                                                                                                                                 

    PhysicalPALLAS_TubeISO1 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_TubeISO1, "TubeISO1",
                                           LogicalHolder, false, 0);                                           

    PhysicalPALLAS_TubeISO2 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_TubeISO2, "TubeISO2",
                                           LogicalHolder, false, 0);                                                                                      

    

    //SECTION 3
    PhysicalPALLAS_ASMPoutre = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_ASMPoutre, "ASMPoutre",
                                           LogicalHolder, false, 0);

    PhysicalPALLAS_StationYAG = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_StationYAG, "StationYAG",
                                           LogicalHolder, false, 0);   


    //SECTION 4
    PhysicalPALLAS_BlindageBD = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_BlindageBD, "BlindageBD",
                                           LogicalHolder, false, 0);                                   

    PhysicalPALLAS_BlindageCBD = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_BlindageCBD, "BlindageCBD",
                                           LogicalHolder, false, 0);  

    PhysicalPALLAS_ChambreDipole = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_ChambreDipole, "ChambreDipole",
                                           LogicalHolder, false, 0);  

    PhysicalPALLAS_ChassisDipoleYAG = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_ChassisDipoleYAG, "ChassisDipoleYAG",
                                           LogicalHolder, false, 0);   

    PhysicalPALLAS_DiagsChamber = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_DiagsChamber, "DiagsChamber",
                                           LogicalHolder, false, 0);   

    PhysicalPALLAS_Dipole = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0*mm)),
                                           LogicalPALLAS_Dipole, "Dipole",
                                           LogicalHolder, false, 0);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
                                                                                      
#else

#endif

    // Returns world with everything in it and all properties set
    return PhysicalWorld;
}
