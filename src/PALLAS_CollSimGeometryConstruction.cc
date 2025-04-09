//// PALLAS_CollSimGeometryConstruction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimGeometryConstruction.hh"

using namespace CLHEP;
G4Mutex fieldManagerMutex = G4MUTEX_INITIALIZER;
G4ThreadLocal PALLAS_CollSimMagneticField
*PALLAS_CollSimGeometryConstruction::fMagneticField = nullptr;
G4ThreadLocal G4FieldManager *PALLAS_CollSimGeometryConstruction::fFieldMgr =
    nullptr;

// Constructor
PALLAS_CollSimGeometryConstruction::PALLAS_CollSimGeometryConstruction()
    : G4VUserDetectorConstruction(), Geom() {
    Geom = new Geometry();
    fMessenger = new G4GenericMessenger(this, "/display/",
                                        "Control commands for my application");
    gMessenger = new G4GenericMessenger(this, "/geometry/",
                                        "Control commands for my application");
    bMessenger = new G4GenericMessenger(this, "/field/",
                                        "Control commands for my application");

    // Commande DISPLAY
    fMessenger
    ->DeclareProperty("SetStatusDisplayLIFGeometry", StatusDisplayLIFGeometry)
    .SetGuidance("Set the boolean parameter.")
    .SetParameterName("StatusDisplayLIFGeometry", false)
    .SetDefaultValue("false");

    fMessenger
    ->DeclareProperty("SetStatusDisplayCelluleGeometry",
                      StatusDisplayCelluleGeometry)
    .SetGuidance("Set the boolean parameter.")
    .SetParameterName("StatusDisplayCelluleGeometry", false)
    .SetDefaultValue("false");

    fMessenger
    ->DeclareProperty("SetStatusDisplaySection1Geometry",
                      StatusDisplaySection1Geometry)
    .SetGuidance("Set the boolean parameter.")
    .SetParameterName("StatusDisplaySection1Geometry", false)
    .SetDefaultValue("false");

    fMessenger
    ->DeclareProperty("SetStatusDisplaySection2Geometry",
                      StatusDisplaySection2Geometry)
    .SetGuidance("Set the boolean parameter.")
    .SetParameterName("StatusDisplaySection2Geometry", false)
    .SetDefaultValue("false");

    fMessenger
    ->DeclareProperty("SetStatusDisplaySection3Geometry",
                      StatusDisplaySection3Geometry)
    .SetGuidance("Set the boolean parameter.")
    .SetParameterName("StatusDisplaySection3Geometry", false)
    .SetDefaultValue("false");

    fMessenger
    ->DeclareProperty("SetStatusDisplaySection4Geometry",
                      StatusDisplaySection4Geometry)
    .SetGuidance("Set the boolean parameter.")
    .SetParameterName("StatusDisplaySection4Geometry", false)
    .SetDefaultValue("false");

    fMessenger
    ->DeclareProperty("SetStatusDisplaySection4DumpGeometry",
                      StatusDisplaySection4DumpGeometry)
    .SetGuidance("Set the boolean parameter.")
    .SetParameterName("StatusDisplaySection4DumpGeometry", false)
    .SetDefaultValue("false");

    // Commande GEOMETRY
    gMessenger->DeclareProperty("SetStatusRoundCollimator", StatusRoundCollimator)
    .SetGuidance("Set the boolean parameter.")
    .SetParameterName("StatusRoundCollimator", false)
    .SetDefaultValue("true");

    gMessenger
    ->DeclareProperty("SetVerticalCollimatorMaterial",
                      VerticalCollimatorMaterial)
    .SetGuidance("Set the boolean parameter.")
    .SetParameterName("VerticalCollimatorMaterial", false)
    .SetDefaultValue("G4_W");

    gMessenger
    ->DeclareProperty("SetHorizontalCollimatorMaterial",
                      HorizontalCollimatorMaterial)
    .SetGuidance("Set the boolean parameter.")
    .SetParameterName("HorizontalCollimatorMaterial", false)
    .SetDefaultValue("G4_Pb");

    gMessenger
    ->DeclarePropertyWithUnit("SetCollimatorThickness", "mm",
                              CollimatorThickness)
    .SetGuidance("Set the collimator thickness parameter.")
    .SetParameterName("CollimatorThickness", false)
    .SetDefaultValue("5.0 mm")
    .SetRange("CollimatorThickness >=0.0");

    gMessenger
    ->DeclarePropertyWithUnit("SetVerticalCollimatorThickness", "mm",
                              VerticalCollimatorThickness)
    .SetGuidance("Set the vertical collimator thickness parameter.")
    .SetParameterName("VerticalCollimatorThickness", false)
    .SetDefaultValue("5.0 mm")
    .SetRange("VerticalCollimatorThickness >=0.0");

    gMessenger
    ->DeclarePropertyWithUnit("SetHorizontalCollimatorThickness", "mm",
                              HorizontalCollimatorThickness)
    .SetGuidance("Set the horizonntal collimator thickness parameter.")
    .SetParameterName("HorizontalCollimatorThickness", false)
    .SetDefaultValue("5.0 mm")
    .SetRange("HorizontalCollimatorThickness >=0.0");

    gMessenger
    ->DeclarePropertyWithUnit("SetCollimatorVHDistance", "mm",
                              CollimatorVHDistance)
    .SetGuidance("Set the collimator distance between Vert & Hor plates.")
    .SetParameterName("CollimatorVHDistance", false)
    .SetDefaultValue("200.0 mm")
    .SetRange("CollimatorVHDistance >=0.0");

    gMessenger
    ->DeclareProperty("SetCollimatorInternalRadius", CollimatorInternalRadius)
    .SetGuidance("Set the collimator internal radius parameter.")
    .SetParameterName("CollimatorInternalRadius", false)
    .SetDefaultValue("1.0");

    gMessenger
    ->DeclarePropertyWithUnit("SetCollimatorLength", "mm", CollimatorLength)
    .SetGuidance("Set the collimator length parameter.")
    .SetParameterName("CollimatorLength", false)
    .SetDefaultValue("200.0 mm")
    .SetRange("CollimatorLength >=0.0");

    gMessenger
    ->DeclarePropertyWithUnit("SetCollimatorDistanceBetweenPlates", "mm",
                              CollimatorDistanceBetweenPlates)
    .SetGuidance("Set the collimator distance between plates parameter.")
    .SetParameterName("CollimatorDistanceBetweenPlates", false)
    .SetDefaultValue("200.0 mm")
    .SetRange("CollimatorDistanceBetweenPlates >=0.0");

    gMessenger
    ->DeclarePropertyWithUnit("SetOpenVerticalCollimator", "mm",
                              OpenVerticalCollimator)
    .SetGuidance("Set the open vertical collimator.")
    .SetParameterName("OpenVerticalCollimator", false)
    .SetDefaultValue("200.0 mm")
    .SetRange("OpenVerticalCollimator >=0.0");

    gMessenger
    ->DeclarePropertyWithUnit("SetOpenHorizontalCollimator", "mm",
                              OpenHorizontalCollimator)
    .SetGuidance("Set the open Horizontal collimator.")
    .SetParameterName("OpenHorizontalCollimator", false)
    .SetDefaultValue("10.0 mm")
    .SetRange("OpenHorizontalCollimator >=0.0");

    gMessenger
    ->DeclarePropertyWithUnit("SetCollimatorExternalRadius", "mm",
                              CollimatorExternalRadius)
    .SetGuidance("Set the collimator external radius parameter.")
    .SetParameterName("CollimatorExternalRadius", false)
    .SetDefaultValue("50.0 mm")
    .SetRange("CollimatorExternalRadius >=0.0");

    gMessenger
    ->DeclarePropertyWithUnit("SetCollimatorSpectrometerDistance", "mm",
                              CollimatorSpectrometerDistance)
    .SetGuidance("Set the collimator spectrometer distance parameter.")
    .SetParameterName("CollimatorSpectrometerDistance", false)
    .SetDefaultValue("100.0 mm")
    .SetRange("CollimatorSpectrometerDistance >=0.0");

    bMessenger
    ->DeclarePropertyWithUnit("SetConstantDipoleBField", "tesla",
                              ConstantDipoleBField)
    .SetGuidance("Set the constant Dipole BField value.")
    .SetParameterName("ConstantDipoleBField", false)
    .SetDefaultValue("0.0 tesla")
    .SetRange("ConstantDipoleBField >=0.0");

    bMessenger->DeclareProperty("SetQ1Gradient", Q1Gradient)
    .SetGuidance("Set the Q1 Gradient BField value.")
    .SetParameterName("Q1Gradient", false)
    .SetDefaultValue("0.0");

    bMessenger->DeclareProperty("SetQ2Gradient", Q2Gradient)
    .SetGuidance("Set the Q2 Gradient BField value.")
    .SetParameterName("Q2Gradient", false)
    .SetDefaultValue("0.0");

    bMessenger->DeclareProperty("SetQ3Gradient", Q3Gradient)
    .SetGuidance("Set the Q3 Gradient BField value.")
    .SetParameterName("Q3Gradient", false)
    .SetDefaultValue("0.0");

    bMessenger->DeclareProperty("SetQ4Gradient", Q4Gradient)
    .SetGuidance("Set the Q4 Gradient BField value.")
    .SetParameterName("Q4Gradient", false)
    .SetDefaultValue("0.0");

    bMessenger->DeclareProperty("SetStatusMapBField", StatusMapBField)
    .SetGuidance("Set the boolean parameter.")
    .SetParameterName("StatuMapBField", false)
    .SetDefaultValue("false");

    G4cout << "MESSENGER GEOMETRY BASE" << G4endl;
}

// Destructor
PALLAS_CollSimGeometryConstruction::~PALLAS_CollSimGeometryConstruction() {
    delete Geom;
    delete fMessenger;
    delete gMessenger;
}

void PALLAS_CollSimGeometryConstruction::SetLogicalVolumeColor(
    G4LogicalVolume *LogicalVolume, G4String Color) {
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

    white = new G4VisAttributes(
        G4Colour(1, 1, 1, 1.)); // Sets the color (can be looked up online)
    // white->SetForceWireframe(true); // Sets to wire frame mode for coloring the
    // volume
    white->SetForceSolid(true); // Sets to solid mode for coloring the volume
    white->SetVisibility(true); // Makes color visible in visualization

    gray = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.5));
    //  gray->SetForceWireframe(true);
    // gray->SetForceSolid(true);
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

    blue = new G4VisAttributes(G4Colour(0, 0, 1, 0.5));
    //  blue->SetForceWireframe(true);
    blue->SetForceSolid(true);
    blue->SetVisibility(true);

    magenta = new G4VisAttributes(G4Colour(1, 0, 1, 0.3));
    //  magenta->SetForceWireframe(true);
    // magenta->SetForceSolid(true);
    magenta->SetVisibility(true);

    if (Color == "invis") {
        LogicalVolume->SetVisAttributes(invis);
    } else if (Color == "black") {
        LogicalVolume->SetVisAttributes(black);
    } else if (Color == "white") {
        LogicalVolume->SetVisAttributes(white);
    } else if (Color == "gray") {
        LogicalVolume->SetVisAttributes(gray);
    } else if (Color == "red") {
        LogicalVolume->SetVisAttributes(red);
    } else if (Color == "orange") {
        LogicalVolume->SetVisAttributes(orange);
    } else if (Color == "yellow") {
        LogicalVolume->SetVisAttributes(yellow);
    } else if (Color == "green") {
        LogicalVolume->SetVisAttributes(green);
    } else if (Color == "cyan") {
        LogicalVolume->SetVisAttributes(cyan);
    } else if (Color == "blue") {
        LogicalVolume->SetVisAttributes(blue);
    } else if (Color == "magenta") {
        LogicalVolume->SetVisAttributes(magenta);
    }
}

void PALLAS_CollSimGeometryConstruction::CreateWorldAndHolder() {
    Vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    // Create World Volume
    // This is just a big box to place all other logical volumes inside
    G4Box *SolidWorld = new G4Box("SolidWorld", 2.1 * m, 15.1 * m, 2.1 * m);
    LogicalWorld =
        new G4LogicalVolume(SolidWorld, Vacuum, "LogicalWorld", 0, 0, 0);
    SetLogicalVolumeColor(LogicalWorld, "invis");

    G4Box *SolidHolder = new G4Box("SolidHolder", 2.05 * m, 15.05 * m, 2.05 * m);
    LogicalHolder =
        new G4LogicalVolume(SolidHolder, Vacuum, "LogicalHolder", 0, 0, 0);
    SetLogicalVolumeColor(LogicalHolder, "invis");


    // Place the world volume: center of world at origin (0,0,0)
    PhysicalWorld =
        new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0, 0, 0)),
                          "World", LogicalWorld, NULL, false, 0);

    PhysicalHolder = new G4PVPlacement(
        G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        LogicalHolder, "Holder", LogicalWorld, false, 0);

}

void PALLAS_CollSimGeometryConstruction::ConstructVerticalCollimator() {

    LogicalVerticalCollimator = Geom->GetCollimator("V");
    Material = G4NistManager::Instance()->FindOrBuildMaterial(
                   VerticalCollimatorMaterial);
    LogicalVerticalCollimator->SetMaterial(Material);
    G4GeometryManager::GetInstance()->OpenGeometry();

    auto pipe1 = static_cast<G4Box *>(
                     G4LogicalVolumeStore::GetInstance()->GetVolume("V")->GetSolid());
    pipe1->SetXHalfLength(CollimatorLength / 2);
    pipe1->SetYHalfLength(CollimatorLength / 2);
    pipe1->SetZHalfLength(VerticalCollimatorThickness / 2);

    SetLogicalVolumeColor(LogicalVerticalCollimator, "blue");

    G4RotationMatrix *rotationMatrix = new G4RotationMatrix();
    rotationMatrix->rotateX(90.0 * deg);

    G4ThreeVector translation1(
        (-0.152 - OpenVerticalCollimator - CollimatorLength / 2) * mm,
        3114.5 - CollimatorSpectrometerDistance - VerticalCollimatorThickness / 2 -120,
        0.08 * mm);
    G4ThreeVector translation2(
        (-0.152 + OpenVerticalCollimator + CollimatorLength / 2) * mm,
        3114.5 - CollimatorSpectrometerDistance - VerticalCollimatorThickness / 2 -120,
        0.08 * mm);

    PhysicalVerticalCollimator1 =
        new G4PVPlacement(rotationMatrix, translation1, LogicalVerticalCollimator,
                          "VerticalCollimator", LogicalHolder, false, 0);

    PhysicalVerticalCollimator2 =
        new G4PVPlacement(rotationMatrix, translation2, LogicalVerticalCollimator,
                          "VerticalCollimator", LogicalHolder, false, 0);
}

void PALLAS_CollSimGeometryConstruction::ConstructHorizontalCollimator() {

    LogicalHorizontalCollimator = Geom->GetCollimator("H");
    Material = G4NistManager::Instance()->FindOrBuildMaterial(
                   HorizontalCollimatorMaterial);
    LogicalHorizontalCollimator->SetMaterial(Material);
    G4GeometryManager::GetInstance()->OpenGeometry();

    auto pipe1 = static_cast<G4Box *>(
                     G4LogicalVolumeStore::GetInstance()->GetVolume("H")->GetSolid());
    pipe1->SetXHalfLength(CollimatorLength / 2);
    pipe1->SetYHalfLength(CollimatorLength / 2);
    pipe1->SetZHalfLength(HorizontalCollimatorThickness / 2);

    SetLogicalVolumeColor(LogicalHorizontalCollimator, "black");

    G4RotationMatrix *rotationMatrix = new G4RotationMatrix();
    rotationMatrix->rotateX(90.0 * deg);

    G4ThreeVector translation1(
        -0.152 * mm,
        3114.5 - CollimatorSpectrometerDistance - VerticalCollimatorThickness -
        CollimatorVHDistance - HorizontalCollimatorThickness / 2,
        (0.08 - OpenHorizontalCollimator - CollimatorLength / 2) * mm);
    G4ThreeVector translation2(
        -0.152 * mm,
        3114.5 - CollimatorSpectrometerDistance - VerticalCollimatorThickness -
        CollimatorVHDistance - HorizontalCollimatorThickness / 2,
        (0.08 + OpenHorizontalCollimator + CollimatorLength / 2) * mm);

    PhysicalHorizontalCollimator1 = new G4PVPlacement(
        rotationMatrix, translation1, LogicalHorizontalCollimator,
        "HorizontalCollimator", LogicalHolder, false, 0);

    PhysicalHorizontalCollimator2 = new G4PVPlacement(
        rotationMatrix, translation2, LogicalHorizontalCollimator,
        "HorizontalCollimator", LogicalHolder, false, 0);
}

void PALLAS_CollSimGeometryConstruction::ConstructCellulePart() {
    auto Inox =
        G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    if (StatusDisplayCelluleGeometry) {
        LogicalPALLAS_Assemblage2Cellules =
            Geom->GetGDMLVolume("../gdml_models/Assemblage_2_Cellules.gdml",
                                "Assemblage_2_Cellules", Inox);

        SetLogicalVolumeColor(LogicalPALLAS_Assemblage2Cellules, "magenta");

        PhysicalPALLAS_Assemblage2Cellules = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_Assemblage2Cellules, "Assemblage2Cellules", LogicalHolder,
            false, 0);
    } else {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructLIFPart() {
    auto Inox =
        G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    if (StatusDisplayLIFGeometry) {
        LogicalPALLAS_Croix =
            Geom->GetGDMLVolume("../gdml_models/LIF/Croix.gdml", "Croix", Inox);
        LogicalPALLAS_LIFHublot1 = Geom->GetGDMLVolume(
                                       "../gdml_models/LIF/LIF_Hublot1.gdml", "LIF_Hublot1", Inox);
        LogicalPALLAS_LIFHublot2 = Geom->GetGDMLVolume(
                                       "../gdml_models/LIF/LIF_Hublot2.gdml", "LIF_Hublot2", Inox);
        LogicalPALLAS_LIFHublot3 = Geom->GetGDMLVolume(
                                       "../gdml_models/LIF/LIF_Hublot3.gdml", "LIF_Hublot3", Inox);
        LogicalPALLAS_LIFHublot4 = Geom->GetGDMLVolume(
                                       "../gdml_models/LIF/LIF_Hublot4.gdml", "LIF_Hublot4", Inox);
        LogicalPALLAS_LIFHublot5 = Geom->GetGDMLVolume(
                                       "../gdml_models/LIF/LIF_Hublot5.gdml", "LIF_Hublot5", Inox);
        LogicalPALLAS_LIF_IBX_DD = Geom->GetGDMLVolume(
                                       "../gdml_models/LIF/LIF_IBX_DD.gdml", "LIF_IBX_DD", Inox);
        LogicalPALLAS_LIF_SQLT = Geom->GetGDMLVolume(
                                     "../gdml_models/LIF/LIF_SQLT.gdml", "LIF_SQLT", Inox);
        LogicalPALLAS_MarbreBreadboard1 =
            Geom->GetGDMLVolume("../gdml_models/LIF/Marbre_Breadboard1.gdml",
                                "Marbre_Breadboard1", Inox);
        LogicalPALLAS_MarbreBreadboard2 =
            Geom->GetGDMLVolume("../gdml_models/LIF/Marbre_Breadboard2.gdml",
                                "Marbre_Breadboard2", Inox);
        LogicalPALLAS_OptoMeK =
            Geom->GetGDMLVolume("../gdml_models/LIF/OptoMeK.gdml", "OptoMeK", Inox);

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

        PhysicalPALLAS_Croix = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_Croix, "Croix", LogicalHolder, false, 0);

        PhysicalPALLAS_OptoMeK = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_OptoMeK, "OptoMeK", LogicalHolder, false, 0);

        PhysicalPALLAS_LIFHublot1 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_LIFHublot1, "LIFHublot1", LogicalHolder, false, 0);

        PhysicalPALLAS_LIFHublot2 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_LIFHublot2, "LIFHublot2", LogicalHolder, false, 0);

        PhysicalPALLAS_LIFHublot3 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_LIFHublot3, "LIFHublot3", LogicalHolder, false, 0);

        PhysicalPALLAS_LIFHublot4 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_LIFHublot4, "LIFHublot4", LogicalHolder, false, 0);

        PhysicalPALLAS_LIFHublot5 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_LIFHublot5, "LIFHublot5", LogicalHolder, false, 0);

        PhysicalPALLAS_LIF_IBX_DD = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_LIF_IBX_DD, "LIF_IBX_DD", LogicalHolder, false, 0);

        PhysicalPALLAS_LIF_SQLT = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_LIF_SQLT, "LIF_SQLT", LogicalHolder, false, 0);

        PhysicalPALLAS_MarbreBreadboard1 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_MarbreBreadboard1, "MarbreBreadboard1", LogicalHolder,
            false, 0);

        PhysicalPALLAS_MarbreBreadboard2 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_MarbreBreadboard2, "MarbreBreadboard2", LogicalHolder,
            false, 0);
    } else {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructSection1Part() {
    auto Inox =
        G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    if (StatusDisplaySection1Geometry) {
        LogicalPALLAS_QuadrupoleQ1 = Geom->GetGDMLVolume(
                                         "../gdml_models/S1/Quadrupole_Q1.gdml", "Quadrupole_Q1", Inox);
        LogicalPALLAS_QuadrupoleQ2 = Geom->GetGDMLVolume(
                                         "../gdml_models/S1/Quadrupole_Q2.gdml", "Quadrupole_Q2", Inox);
        LogicalPALLAS_ATH500_DN100 = Geom->GetGDMLVolume(
                                         "../gdml_models/S1/ATH500_DN100.gdml", "ATH500_DN100", Inox);
        LogicalPALLAS_ChambreISO = Geom->GetGDMLVolume(
                                       "../gdml_models/S1/Base_Marbre.gdml", "Base_Marbre", Inox);
        LogicalPALLAS_BaseMarbre = Geom->GetGDMLVolume(
                                       "../gdml_models/S1/Chambre_ISO.gdml", "Chambre_ISO", Inox);

        SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ1, "red");
        SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ2, "red");
        SetLogicalVolumeColor(LogicalPALLAS_ATH500_DN100, "red");
        SetLogicalVolumeColor(LogicalPALLAS_ChambreISO, "red");
        SetLogicalVolumeColor(LogicalPALLAS_BaseMarbre, "red");

        PhysicalPALLAS_QuadrupoleQ1 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_QuadrupoleQ1, "QuadrupoleQ1", LogicalHolder, false, 0);

        PhysicalPALLAS_QuadrupoleQ2 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_QuadrupoleQ2, "QuadrupoleQ2", LogicalHolder, false, 0);

        PhysicalPALLAS_ATH500_DN100 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_ATH500_DN100, "ATH500_DN100", LogicalHolder, false, 0);

        PhysicalPALLAS_BaseMarbre = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_BaseMarbre, "BaseMarbre", LogicalHolder, false, 0);

        PhysicalPALLAS_ChambreISO = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_ChambreISO, "ChambreISO", LogicalHolder, false, 0);
    } else {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructSection2Part() {
    auto Inox =
        G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    if (StatusDisplaySection2Geometry) {
        LogicalPALLAS_QuadrupoleQ3 = Geom->GetGDMLVolume(
                                         "../gdml_models/S2/QuadrupoleQ3.gdml", "Quadrupole_Q3", Inox);
        LogicalPALLAS_QuadrupoleQ4 = Geom->GetGDMLVolume(
                                         "../gdml_models/S2/QuadrupoleQ4.gdml", "Quadrupole_Q4", Inox);
        LogicalPALLAS_ASMRemovalChamber =
            Geom->GetGDMLVolume("../gdml_models/S2/ASM_Removal_Chamber.gdml",
                                "ASMRemovalChamber", Inox);
        LogicalPALLAS_BreadboardRemovalChamber = Geom->GetGDMLVolume(
                    "../gdml_models/S2/Assemblage_Breadboard_Thorlabs_Removal_Chamber.gdml",
                    "Breadboard_Removal_Chamber", Inox);
        LogicalPALLAS_ChassisRemovalChamber = Geom->GetGDMLVolume(
                "../gdml_models/S2/Chassis_PALLAS_Removal_Chamber.gdml",
                "Chassis_PALLAS_Removal_Chamber", Inox);
        LogicalPALLAS_TubeISO1 = Geom->GetGDMLVolume(
                                     "../gdml_models/S2/Tube_ISO_1.gdml", "Tube_ISO_1", Inox);
        LogicalPALLAS_TubeISO2 = Geom->GetGDMLVolume(
                                     "../gdml_models/S2/Tube_ISO_2.gdml", "Tube_ISO_2", Inox);

        SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ3, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ4, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_ASMRemovalChamber, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_BreadboardRemovalChamber, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_ChassisRemovalChamber, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_TubeISO1, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_TubeISO2, "cyan");

        PhysicalPALLAS_QuadrupoleQ3 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_QuadrupoleQ3, "QuadrupoleQ3", LogicalHolder, false, 0);

        PhysicalPALLAS_QuadrupoleQ4 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_QuadrupoleQ4, "QuadrupoleQ4", LogicalHolder, false, 0);

        PhysicalPALLAS_ASMRemovalChamber = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_ASMRemovalChamber, "ASMRemovalChamber", LogicalHolder,
            false, 0);

        PhysicalPALLAS_BreadboardRemovalChamber = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_BreadboardRemovalChamber, "BreadboardRemovalChamber",
            LogicalHolder, false, 0);

        PhysicalPALLAS_ChassisRemovalChamber = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_ChassisRemovalChamber, "ChassisRemovalChamber",
            LogicalHolder, false, 0);

        PhysicalPALLAS_TubeISO1 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_TubeISO1, "TubeISO1", LogicalHolder, false, 0);

        PhysicalPALLAS_TubeISO2 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_TubeISO2, "TubeISO2", LogicalHolder, false, 0);
    } else {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructSection3Part() {
    auto Inox = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    if (StatusDisplaySection3Geometry) {
        LogicalPALLAS_ASMPoutre = Geom->GetGDMLVolume(
                                      "../gdml_models/S3/ASM_Poutre.gdml", "ASM_Poutre", Inox);
        LogicalPALLAS_StationYAG = Geom->GetGDMLVolume(
                                       "../gdml_models/S3/Station_YAG.gdml", "Station_YAG", Inox);

        SetLogicalVolumeColor(LogicalPALLAS_ASMPoutre, "orange");
        SetLogicalVolumeColor(LogicalPALLAS_StationYAG, "orange");

        PhysicalPALLAS_ASMPoutre = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_ASMPoutre, "ASMPoutre", LogicalHolder, false, 0);

        PhysicalPALLAS_StationYAG = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, -300 * mm, 0 * mm)),
            LogicalPALLAS_StationYAG, "StationYAG", LogicalHolder, false, 0);
    } else {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructSection4Part() {
    auto Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
    auto Fe = G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe");
    auto elementY = G4NistManager::Instance()->FindOrBuildElement("Y");
    auto elementO = G4NistManager::Instance()->FindOrBuildElement("O");
    auto elementAl = G4NistManager::Instance()->FindOrBuildElement("Al");

    auto YAG = new G4Material("YAG",
                              4.57 * g / cm3, // 1.053
                              3, kStateSolid, 273.15 * kelvin, 1.0 * atmosphere);

    YAG->AddElement(elementY, 3);
    YAG->AddElement(elementAl, 5);
    YAG->AddElement(elementO, 12);

    if (StatusDisplaySection4Geometry) {
        LogicalPALLAS_ChambreDipole = Geom->GetGDMLVolume(
                                          "../gdml_models/S4/Chambre_Dipole.gdml", "Chambre_Dipole", Al);
        LogicalPALLAS_Dipole =
            Geom->GetGDMLVolume("../gdml_models/S4/Dipole.gdml", "Dipole", Fe);
        LogicalPALLAS_BS1YAG =
            Geom->GetGDMLVolume("../gdml_models/S4/BS1_YAG.gdml", "BS1_YAG", YAG);
        LogicalPALLAS_BSPEC1YAG = Geom->GetGDMLVolume(
                                      "../gdml_models/S4/BSPEC1_YAG.gdml", "BSPEC1_YAG", YAG);
        LogicalPALLAS_DiagsChamber = Geom->GetGDMLVolume(
                                         "../gdml_models/S4/Diags_Chamber.gdml", "Diags_Chamber", Al);
        LogicalPALLAS_S4Tube =
            Geom->GetGDMLVolume("../gdml_models/S4/Tube.gdml", "Tube", Al);
        LogicalPALLAS_S4Tube1 =
            Geom->GetGDMLVolume("../gdml_models/S4/Tube1.gdml", "Tube1", Al);
        LogicalPALLAS_S4Soufflet =
            Geom->GetGDMLVolume("../gdml_models/S4/Soufflet.gdml", "Soufflet", Al);
        LogicalPALLAS_S4Croix =
            Geom->GetGDMLVolume("../gdml_models/S4/Croix.gdml", "Croix", Al);
        LogicalFakeDiagsChamber = Geom->GetFakeDiagsChamber();

        SetLogicalVolumeColor(LogicalPALLAS_ChambreDipole, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_Dipole, "red");
        SetLogicalVolumeColor(LogicalPALLAS_BSPEC1YAG, "yellow");
        SetLogicalVolumeColor(LogicalPALLAS_BS1YAG, "yellow");
        SetLogicalVolumeColor(LogicalPALLAS_DiagsChamber, "yellow");
        SetLogicalVolumeColor(LogicalPALLAS_S4Tube, "yellow");
        SetLogicalVolumeColor(LogicalPALLAS_S4Tube1, "yellow");
        SetLogicalVolumeColor(LogicalPALLAS_S4Soufflet, "yellow");
        SetLogicalVolumeColor(LogicalPALLAS_S4Croix, "yellow");
        SetLogicalVolumeColor(LogicalFakeDiagsChamber, "yellow");

        // PhysicalPALLAS_ChambreDipole = new
        // G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0.152 * mm, -3449.5
        //                             * mm, -10.855 * mm)),
        //               LogicalPALLAS_ChambreDipole, "ChambreDipole",
        //               LogicalBFieldVolume, false, 0);

        PhysicalPALLAS_ChambreDipole = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0. * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_ChambreDipole, "ChambreDipole", LogicalHolder, false, 0);

        // PhysicalPALLAS_Dipole = new G4PVPlacement(G4Transform3D(DontRotate,
        //         G4ThreeVector(0.152 * mm, -3449.5 * mm, -10.855 * mm)),
        //         LogicalPALLAS_Dipole, "Dipole",
        //         LogicalBFieldVolume, false, 0);

        PhysicalPALLAS_Dipole = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0. * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_Dipole, "Dipole", LogicalHolder, false, 0);

        PhysicalPALLAS_BS1YAG = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_BS1YAG, "BS1_YAG", LogicalHolder, false, 0);

        PhysicalPALLAS_BSPEC1YAG = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_BSPEC1YAG, "BSPEC1_YAG", LogicalHolder, false, 0);

        // PhysicalPALLAS_DiagsChamber = new G4PVPlacement(G4Transform3D(DontRotate,
        // G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
        //                                                 LogicalPALLAS_DiagsChamber,
        //                                                 "DiagsChamber",
        //                                                 LogicalHolder, false, 0);

        PhysicalFakeDiagsChamber = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 3960 * mm, 150 * mm)),
            LogicalFakeDiagsChamber, "FakeDiagsChamber", LogicalHolder, false, 0);

        PhysicalPALLAS_S4Tube = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_S4Tube, "S4Tube", LogicalHolder, false, 0);

        PhysicalPALLAS_S4Tube1 = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_S4Tube1, "S4Tube1", LogicalHolder, false, 0);

        PhysicalPALLAS_S4Soufflet = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_S4Soufflet, "S4Soufflet", LogicalHolder, false, 0);

        PhysicalPALLAS_S4Croix = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_S4Croix, "S4Croix", LogicalHolder, false, 0);
    } else {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructCollimators() {
    auto Inox = G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

    G4RotationMatrix *rotationMatrix = new G4RotationMatrix();
    //rotationMatrix->rotateX(90.0 * deg);
    rotationMatrix->rotateZ(90.0 * deg);

    LogicalPALLAS_Collimator_H1 = Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Mors_H_1.gdml", "Collimator_H1", Inox);
    LogicalPALLAS_Collimator_H2 = Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Mors_H_2.gdml", "Collimator_H2", Inox);
    LogicalPALLAS_Collimator_Arbre_H = Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Arbre_H.gdml", "Collimator_Arbre_H", Inox);
    LogicalPALLAS_Collimator_Bride_H = Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Bride_H.gdml", "Collimator_Bride_H", Inox);
    LogicalPALLAS_Collimator_Palier_H = Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Palier_H.gdml", "Collimator_Palier_H", Inox);
    //LogicalPALLAS_Collimator_Soufflet_H = Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Soufflet_H.gdml", "Collimator_Soufflet_H", Inox);
    SetLogicalVolumeColor(LogicalPALLAS_Collimator_H1, "yellow");
    SetLogicalVolumeColor(LogicalPALLAS_Collimator_H2, "yellow");
    SetLogicalVolumeColor(LogicalPALLAS_Collimator_Arbre_H, "yellow");
    SetLogicalVolumeColor(LogicalPALLAS_Collimator_Bride_H, "yellow");
    SetLogicalVolumeColor(LogicalPALLAS_Collimator_Palier_H, "yellow");
    //SetLogicalVolumeColor(LogicalPALLAS_Collimator_Soufflet_H, "yellow");

    LogicalPALLAS_Collimator_V1 = Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Mors_V_1.gdml", "Collimator_V1", Inox);
    LogicalPALLAS_Collimator_V2 = Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Mors_V_2.gdml", "Collimator_V2", Inox);
    LogicalPALLAS_Collimator_Arbre_V = Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Arbre_V.gdml", "Collimator_Arbre_V", Inox);
    LogicalPALLAS_Collimator_Bride_V = Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Bride_V.gdml", "Collimator_Bride_V", Inox);
    LogicalPALLAS_Collimator_Palier_V = Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Palier_V.gdml", "Collimator_Palier_V", Inox);
    //LogicalPALLAS_Collimator_Soufflet_V = Geom->GetGDMLVolume("../gdml_models/Collimators/Collimator_Soufflet_V.gdml", "Collimator_Soufflet_V", Inox);
    SetLogicalVolumeColor(LogicalPALLAS_Collimator_V1, "blue");
    SetLogicalVolumeColor(LogicalPALLAS_Collimator_V2, "blue");
    SetLogicalVolumeColor(LogicalPALLAS_Collimator_Arbre_V, "blue");
    SetLogicalVolumeColor(LogicalPALLAS_Collimator_Bride_V, "blue");
    SetLogicalVolumeColor(LogicalPALLAS_Collimator_Palier_V, "blue");
    //SetLogicalVolumeColor(LogicalPALLAS_Collimator_Soufflet_V, "blue");

    PhysicalPALLAS_Collimator_H1 = new G4PVPlacement(
        rotationMatrix, G4ThreeVector(0 * mm, 2884.5+122.67 * mm, 0 * mm),
        LogicalPALLAS_Collimator_H1, "HorizontalCollimator", LogicalHolder,
        false, 0);

    PhysicalPALLAS_Collimator_H2 = new G4PVPlacement(
        rotationMatrix, G4ThreeVector(0 * mm, 2884.5+122.67 * mm, 0 * mm),
        LogicalPALLAS_Collimator_H2, "HorizontalCollimator", LogicalHolder,
        false, 0);

    PhysicalPALLAS_Collimator_Arbre_H = new G4PVPlacement(
        rotationMatrix, G4ThreeVector(0 * mm, 2884.5+122.67 * mm, 0 * mm),
        LogicalPALLAS_Collimator_Arbre_H, "Arbre_H", LogicalHolder,
        false, 0);

    PhysicalPALLAS_Collimator_Bride_H = new G4PVPlacement(
        rotationMatrix, G4ThreeVector(0 * mm, 2884.5+122.67 * mm, 0 * mm),
        LogicalPALLAS_Collimator_Bride_H, "Bride_H", LogicalHolder,
        false, 0);

    PhysicalPALLAS_Collimator_Palier_H = new G4PVPlacement(
        rotationMatrix, G4ThreeVector(0 * mm, 2884.5+122.67 * mm, 0 * mm),
        LogicalPALLAS_Collimator_Palier_H, "Palier_H", LogicalHolder,
        false, 0);

    // PhysicalPALLAS_Collimator_Soufflet_H = new G4PVPlacement(
    //     rotationMatrix, G4ThreeVector(0 * mm, 2884.5+122.67 * mm, 0 * mm),
    //     LogicalPALLAS_Collimator_Soufflet_H, "Soufflet_H", LogicalHolder,
    //     false, 0);


    PhysicalPALLAS_Collimator_V1 = new G4PVPlacement(
        rotationMatrix, G4ThreeVector(-0.552 * mm, 2884.5+122.67 * mm, 0 * mm),
        LogicalPALLAS_Collimator_V1, "VerticalCollimator", LogicalHolder,
        false, 0);

    PhysicalPALLAS_Collimator_V2 = new G4PVPlacement(
        rotationMatrix, G4ThreeVector(-0.552 * mm, 2884.5+122.67 * mm, 0 * mm),
        LogicalPALLAS_Collimator_V2, "VerticalCollimator", LogicalHolder,
        false, 0);

    PhysicalPALLAS_Collimator_Arbre_V = new G4PVPlacement(
        rotationMatrix, G4ThreeVector(-0.552 * mm, 2884.5+122.67 * mm, 0 * mm),
        LogicalPALLAS_Collimator_Arbre_V, "Arbre_V", LogicalHolder,
        false, 0);

    PhysicalPALLAS_Collimator_Bride_V = new G4PVPlacement(
        rotationMatrix, G4ThreeVector(-0.552 * mm, 2884.5+122.67 * mm, 0 * mm),
        LogicalPALLAS_Collimator_Bride_V, "Bride_V", LogicalHolder,
        false, 0);

    PhysicalPALLAS_Collimator_Palier_V = new G4PVPlacement(
        rotationMatrix, G4ThreeVector(-0.552 * mm, 2884.5+122.67 * mm, 0 * mm),
        LogicalPALLAS_Collimator_Palier_V, "Palier_V", LogicalHolder,
        false, 0);

    // PhysicalPALLAS_Collimator_Soufflet_V = new G4PVPlacement(
    //     rotationMatrix, G4ThreeVector(-0.552 * mm, 2884.5+122.67 * mm, 0 * mm),
    //     LogicalPALLAS_Collimator_Soufflet_V, "Soufflet_V", LogicalHolder,
    //     false, 0);

}

void PALLAS_CollSimGeometryConstruction::ConstructSection4DumpPart() {
    auto Pb = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");
    auto Al = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

    if (StatusDisplaySection4DumpGeometry) {
        LogicalPALLAS_BlindageBD = Geom->GetGDMLVolume(
                                       "../gdml_models/S4/Blindage_BD.gdml", "Blindage_BD", Pb);
        LogicalPALLAS_BlindageCBD = Geom->GetGDMLVolume(
                                        "../gdml_models/S4/Blindage_CBD.gdml", "Blindage_CBD", Pb);
        LogicalPALLAS_ChassisDipoleYAG = Geom->GetGDMLVolume(
                                             "../gdml_models/S4/Chassis_Dipole_YAG.gdml", "Chassis_Dipole_YAG", Al);

        SetLogicalVolumeColor(LogicalPALLAS_BlindageBD, "blue");
        SetLogicalVolumeColor(LogicalPALLAS_BlindageCBD, "blue");
        SetLogicalVolumeColor(LogicalPALLAS_ChassisDipoleYAG, "blue");

        PhysicalPALLAS_BlindageBD = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_BlindageBD, "BlindageBD", LogicalHolder, false, 0);

        PhysicalPALLAS_BlindageCBD = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_BlindageCBD, "BlindageCBD", LogicalHolder, false, 0);

        PhysicalPALLAS_ChassisDipoleYAG = new G4PVPlacement(
            G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
            LogicalPALLAS_ChassisDipoleYAG, "ChassisDipoleYAG", LogicalHolder,
            false, 0);
    } else {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructSDandField() {
    // magnetic field ----------------------------------------------------------
    fMagneticField = new PALLAS_CollSimMagneticField();
    fMagneticField->SetDipoleField(ConstantDipoleBField);
    G4cout << "B = " << ConstantDipoleBField << G4endl;
    fMagneticField->SetMapBFieldStatus(StatusMapBField);
    fFieldMgr = new G4FieldManager();
    fFieldMgr->SetDetectorField(fMagneticField);
    fFieldMgr->CreateChordFinder(fMagneticField);
    G4ChordFinder* chordFinder = new G4ChordFinder(fMagneticField, 1e-3 * mm, new G4ClassicalRK4(new G4Mag_UsualEqRhs(fMagneticField)));
    fFieldMgr->SetChordFinder(chordFinder);

    G4bool forceToAllDaughters = true;
    LogicalHolder->SetFieldManager(fFieldMgr, forceToAllDaughters);
}

G4VPhysicalVolume *PALLAS_CollSimGeometryConstruction::Construct() {
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    // Define common rotations
    DontRotate.rotateX(0.0 * deg);
    Flip.rotateZ(0 * deg);
    Flip.rotateX(90 * deg);
    Flip.rotateY(0 * deg);

    // #########################
    //  DEFINE GEOMETRY VOLUMES#
    // #########################
    //*********************************
    // Build scint et wrapping volumes*
    //*********************** *********
    CreateWorldAndHolder();
    //ConstructVerticalCollimator();
    //ConstructHorizontalCollimator();
    ConstructCollimators();
    ConstructCellulePart();
    ConstructLIFPart();
    ConstructSection1Part();
    ConstructSection2Part();
    ConstructSection3Part();
    ConstructSection4Part();
    ConstructSection4DumpPart();

    G4cout << "END OF THE DETECTOR CONSTRUCTION" << G4endl;

    // Returns world with everything in it and all properties set
    return PhysicalWorld;
}
