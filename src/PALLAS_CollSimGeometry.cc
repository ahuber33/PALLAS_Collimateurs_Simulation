//// PALLAS_CollSimGeometry.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimGeometry.hh"

using namespace CLHEP;

const G4String PALLAS_CollSimGeometry::path_bin = "../bin/";
const G4String PALLAS_CollSimGeometry::path = "../simulation_input_files/";

// Constructor
PALLAS_CollSimGeometry::PALLAS_CollSimGeometry(){}

// Destructor
PALLAS_CollSimGeometry::~PALLAS_CollSimGeometry()
{
}

// Main Function: Builds Full block, coupling, and PMT geometries
G4VPhysicalVolume* PALLAS_CollSimGeometry::Construct( ){
// Initialize scint classes
scintProp = new PALLAS_CollSimMaterials(path_bin+"Materials.cfg");
Vacuum = scintProp->GetMaterial("Vacuum");
theScint = new Geometry(path_bin+"PALLAS_CollSim.cfg");


// ***********************
// Visualization Colors
// ***********************
// Create some colors for visualizations
invis = new G4VisAttributes( G4Colour(255/255. ,255/255. ,255/255. ));
invis->SetVisibility(false);

white = new G4VisAttributes(G4Colour(1,1,1,1.)); // Sets the color (can be looked up online)
//white->SetForceWireframe(true); // Sets to wire frame mode for coloring the volume
white->SetForceSolid(true); // Sets to solid mode for coloring the volume
white->SetVisibility(true); // Makes color visible in visualization

gray = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.5));
//  gray->SetForceWireframe(true);
gray->SetForceSolid(true);
gray->SetVisibility(true);

gray_bis = new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.05));
//  gray->SetForceWireframe(true);
gray_bis->SetForceSolid(true);
gray_bis->SetVisibility(true);

black = new G4VisAttributes(G4Colour(0,0,0,0.7));
//  black->SetForceWireframe(true);
black->SetForceSolid(true);
black->SetVisibility(true);

black_bis = new G4VisAttributes(G4Colour(0,0,0,0.4));
//  black->SetForceWireframe(true);
black_bis->SetForceSolid(true);
black_bis->SetVisibility(true);

red = new G4VisAttributes(G4Colour(1,0,0,0.5));
//  red->SetForceWireframe(true);
red->SetForceSolid(true);
red->SetVisibility(true);

red_hot = new G4VisAttributes(G4Colour(1,0,0,1));
//  red->SetForceWireframe(true);
red_hot->SetForceSolid(true);
red_hot->SetVisibility(true);

orange = new G4VisAttributes(G4Colour(1,0.5,0,0.1));
//  orange->SetForceWireframe(true);
orange->SetForceSolid(true);
orange->SetVisibility(true);

yellow = new G4VisAttributes(G4Colour(1,1,0,0.1));
//  yellow->SetForceWireframe(true);
yellow->SetForceSolid(true);
yellow->SetVisibility(true);

green = new G4VisAttributes(G4Colour(0,1,0,0.35));
//  green->SetForceWireframe(true);
green->SetForceSolid(true);
green->SetVisibility(true);

green_hot = new G4VisAttributes(G4Colour(0,1,0,1));
//  green_hot->SetForceWireframe(true);
green_hot->SetForceSolid(true);
green_hot->SetVisibility(true);

cyan = new G4VisAttributes(G4Colour(0,1,1,0.1));
//  cyan->SetForceWireframe(true);
cyan->SetForceSolid(true);
cyan->SetVisibility(true);

blue = new G4VisAttributes(G4Colour(0,0,1,0.5));
//  blue->SetForceWireframe(true);
blue->SetForceSolid(true);
blue->SetVisibility(true);

magenta = new G4VisAttributes(G4Colour(1,0,1,0.85));
//  magenta->SetForceWireframe(true);
//magenta->SetForceSolid(true);
magenta->SetVisibility(true);

// Define common rotations
G4RotationMatrix DontRotate;
DontRotate.rotateX(0.0*deg);
G4RotationMatrix Flip;
Flip.rotateZ(0*deg);
Flip.rotateX(180*deg);
Flip.rotateY(0*deg);

// ***********************
// Various dimensions
// ***********************
ScintillatorThickness = theScint->GetScintillatorThickness();
ZnSThickness = theScint->GetZnSThickness();
DetectorThickness = theScint->GetDetectorThickness();
AirGap = theScint->GetAirGap();
GlassThickness = theScint->GetGlassThickness();
WorkingDistance = theScint->GetWorkingDistance();

//#########################
// DEFINE GEOMETRY VOLUMES#
//#########################

// Create World Volume
// This is just a big box to place all other logical volumes inside
G4Box *SolidWorld = new G4Box("SolidWorld", 1100*cm, 1100*cm, 1100*cm );
LogicalWorld = new G4LogicalVolume(SolidWorld, Vacuum,"LogicalWorld",0,0,0);
LogicalWorld->SetVisAttributes(invis);

G4Box *SolidHolder = new G4Box("SolidWorld", 1000*cm, 1000*cm, 1000*cm );
LogicalHolder = new G4LogicalVolume(SolidHolder, Vacuum,"LogicalHolder",0,0,0);
LogicalHolder->SetVisAttributes(invis);

// Place the world volume: center of world at origin (0,0,0)
PhysicalWorld = new G4PVPlacement(G4Transform3D(DontRotate,G4ThreeVector(0,0,0)),"World",LogicalWorld,NULL,false,0);

//*********************************
// Build scint et wrapping volumes*
//*********************** *********
//Simply calls functions from Scintillator() class
LogicalSc = theScint->GetSc();

// Set colors of various block materials
LogicalSc->SetVisAttributes(cyan);


//############################
// DEFINE GEOMETRY PLACEMENTS#
//############################

#ifndef disable_gdml

PhysicalHolder = new G4PVPlacement(G4Transform3D
(DontRotate,G4ThreeVector(0*mm, 0*mm, 0*mm)),
LogicalHolder,"Holder",
LogicalWorld,false,0);

PhysicalSc = new G4PVPlacement(G4Transform3D
(DontRotate,G4ThreeVector(0*mm, 0*mm, Z_Position_Sc)), 
LogicalSc,"Scintillator",
LogicalHolder,false,0);

#else

#endif





// Returns world with everything in it and all properties set
return PhysicalWorld;
}
