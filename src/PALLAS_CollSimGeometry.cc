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

    red = new G4VisAttributes(G4Colour(1, 0, 0, 0.5));
    //  red->SetForceWireframe(true);
    red->SetForceSolid(true);
    red->SetVisibility(true);

    orange = new G4VisAttributes(G4Colour(1, 0.5, 0, 0.1));
    //  orange->SetForceWireframe(true);
    orange->SetForceSolid(true);
    orange->SetVisibility(true);

    yellow = new G4VisAttributes(G4Colour(1, 1, 0, 0.7));
    //  yellow->SetForceWireframe(true);
    yellow->SetForceSolid(true);
    yellow->SetVisibility(true);

    green = new G4VisAttributes(G4Colour(0, 1, 0, 0.35));
    //  green->SetForceWireframe(true);
    green->SetForceSolid(true);
    green->SetVisibility(true);

    cyan = new G4VisAttributes(G4Colour(0, 1, 1, 0.1));
    //  cyan->SetForceWireframe(true);
    cyan->SetForceSolid(true);
    cyan->SetVisibility(true);

    blue = new G4VisAttributes(G4Colour(0, 0, 1, 0.5));
    //  blue->SetForceWireframe(true);
    blue->SetForceSolid(true);
    blue->SetVisibility(true);

    magenta = new G4VisAttributes(G4Colour(1, 0, 1, 0.85));
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
    G4Box *SolidWorld = new G4Box("SolidWorld", 1.1 * m, 1.1 * m, 1.1 * m);
    LogicalWorld = new G4LogicalVolume(SolidWorld, Vacuum, "LogicalWorld", 0, 0, 0);
    SetLogicalVolumeColor(LogicalWorld, "invis");

    G4Box *SolidHolder = new G4Box("SolidHolder", 1.05 * m, 1.05 * m, 1.05 * m);
    LogicalHolder = new G4LogicalVolume(SolidHolder, Vacuum, "LogicalHolder", 0, 0, 0);
    SetLogicalVolumeColor(LogicalHolder, "invis");

    // Place the world volume: center of world at origin (0,0,0)
    PhysicalWorld = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0, 0, 0)), "World", LogicalWorld, NULL, false, 0);

    PhysicalHolder = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                       LogicalHolder, "Holder",
                                       LogicalWorld, false, 0);
}

G4VPhysicalVolume *PALLAS_CollSimGeometry::Construct()
{
    // ***********************
    // Various dimensions
    // ***********************
    Geom = new Geometry(path_bin + "PALLAS_CollSim.cfg");
    CollimatorThickness = Geom->GetCollimatorThickness();
    OutputThickness = Geom->GetOutputThickness();

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
    LogicalCollimator = Geom->GetRoundCollimator();
    LogicalFrontOutput = Geom->GetOutputCollimator();
    LogicalBackOutput = Geom->GetOutputCollimator();

    // Set colors of various block materials
    // LogicalCollimator->SetVisAttributes(black);
    SetLogicalVolumeColor(LogicalCollimator, "red");
    SetLogicalVolumeColor(LogicalFrontOutput, "yellow");
    SetLogicalVolumeColor(LogicalBackOutput, "red");

    // ############################
    //  DEFINE GEOMETRY PLACEMENTS#
    // ############################

    Z_Collimator = 0;
    Z_FrontOutput = Z_Collimator - OutputThickness / 2 - CollimatorThickness / 2;
    Z_BackOutput = Z_Collimator + OutputThickness / 2 + CollimatorThickness / 2;

#ifndef disable_gdml

    CreateWorldAndHolder();

    PhysicalFrontOutput = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, Z_FrontOutput)),
                                            LogicalFrontOutput, "FrontOutput",
                                            LogicalHolder, false, 0);

    PhysicalCollimator = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, Z_Collimator)),
                                           LogicalCollimator, "Collimator",
                                           LogicalHolder, false, 0);

    PhysicalBackOutput = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, Z_BackOutput)),
                                           LogicalBackOutput, "BackOutput",
                                           LogicalHolder, false, 0);

#else

#endif

    // Returns world with everything in it and all properties set
    return PhysicalWorld;
}
