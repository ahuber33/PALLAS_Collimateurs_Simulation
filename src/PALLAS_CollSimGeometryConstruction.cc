//// PALLAS_CollSimGeometryConstruction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimGeometryConstruction.hh"

using namespace CLHEP;

// Constructor
PALLAS_CollSimGeometryConstruction::PALLAS_CollSimGeometryConstruction() : G4VUserDetectorConstruction(), Geom()
{
    Geom = new Geometry();
    fMessenger = new G4GenericMessenger(this, "/display/", "Control commands for my application");
    gMessenger = new G4GenericMessenger(this, "/geometry/", "Control commands for my application");
    bMessenger = new G4GenericMessenger(this, "/field/", "Control commands for my application");

    // Commande DISPLAY
    fMessenger->DeclareProperty("SetStatusDisplayLIFGeometry", StatusDisplayLIFGeometry)
        .SetGuidance("Set the boolean parameter.")
        .SetParameterName("StatusDisplayLIFGeometry", false)
        .SetDefaultValue("false");

    fMessenger->DeclareProperty("SetStatusDisplayCelluleGeometry", StatusDisplayCelluleGeometry)
        .SetGuidance("Set the boolean parameter.")
        .SetParameterName("StatusDisplayCelluleGeometry", false)
        .SetDefaultValue("false");

    fMessenger->DeclareProperty("SetStatusDisplaySection1Geometry", StatusDisplaySection1Geometry)
        .SetGuidance("Set the boolean parameter.")
        .SetParameterName("StatusDisplaySection1Geometry", false)
        .SetDefaultValue("false");

    fMessenger->DeclareProperty("SetStatusDisplaySection2Geometry", StatusDisplaySection2Geometry)
        .SetGuidance("Set the boolean parameter.")
        .SetParameterName("StatusDisplaySection2Geometry", false)
        .SetDefaultValue("false");

    fMessenger->DeclareProperty("SetStatusDisplaySection3Geometry", StatusDisplaySection3Geometry)
        .SetGuidance("Set the boolean parameter.")
        .SetParameterName("StatusDisplaySection3Geometry", false)
        .SetDefaultValue("false");

    fMessenger->DeclareProperty("SetStatusDisplaySection4Geometry", StatusDisplaySection4Geometry)
        .SetGuidance("Set the boolean parameter.")
        .SetParameterName("StatusDisplaySection4Geometry", false)
        .SetDefaultValue("false");

    fMessenger->DeclareProperty("SetStatusDisplaySection4DumpGeometry", StatusDisplaySection4DumpGeometry)
        .SetGuidance("Set the boolean parameter.")
        .SetParameterName("StatusDisplaySection4DumpGeometry", false)
        .SetDefaultValue("false");

    // Commande GEOMETRY
    gMessenger->DeclarePropertyWithUnit("SetCollimatorThickness", "mm", CollimatorThickness)
        .SetGuidance("Set the collimator thickness parameter.")
        .SetParameterName("CollimatorThickness", false)
        .SetDefaultValue("5.0 mm")
        .SetRange("CollimatorThickness >=0.0");

    gMessenger->DeclarePropertyWithUnit("SetCollimatorInternalRadius", "mm", CollimatorInternalRadius)
        .SetGuidance("Set the collimator internal radius parameter.")
        .SetParameterName("CollimatorInternalRadius", false)
        .SetDefaultValue("1.0 mm")
        .SetRange("CollimatorInternalRadius >=0.0");

    gMessenger->DeclarePropertyWithUnit("SetCollimatorExternalRadius", "mm", CollimatorExternalRadius)
        .SetGuidance("Set the collimator external radius parameter.")
        .SetParameterName("CollimatorExternalRadius", false)
        .SetDefaultValue("50.0 mm")
        .SetRange("CollimatorExternalRadius >=0.0");

    gMessenger->DeclarePropertyWithUnit("SetCollimatorSpectrometerDistance", "mm", CollimatorSpectrometerDistance)
        .SetGuidance("Set the collimator spectrometer distance parameter.")
        .SetParameterName("CollimatorSpectrometerDistance", false)
        .SetDefaultValue("100.0 mm")
        .SetRange("CollimatorSpectrometerDistance >=0.0");

    // Commande FIELD
    bMessenger->DeclareProperty("SetStatusMapBField", StatusMapBField)
        .SetGuidance("Set the boolean parameter.")
        .SetParameterName("StatusMapBField", false)
        .SetDefaultValue("false");

    bMessenger->DeclarePropertyWithUnit("SetConstantBField", "tesla", ConstantBField)
        .SetGuidance("Set the constant value of BField without map parameter.")
        .SetParameterName("ConstantBField", false)
        .SetDefaultValue("0.4 tesla")
        .SetRange("ConstantBField >=0.0");
}

// Destructor
PALLAS_CollSimGeometryConstruction::~PALLAS_CollSimGeometryConstruction()
{
    delete Geom;
    delete fMessenger;
    delete gMessenger;
    delete bMessenger;
}

void PALLAS_CollSimGeometryConstruction::SetLogicalVolumeColor(G4LogicalVolume *LogicalVolume, G4String Color)
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

    blue = new G4VisAttributes(G4Colour(0, 0, 1, 0.02));
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

void PALLAS_CollSimGeometryConstruction::CreateWorldAndHolder()
{
    Vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    // Create World Volume
    // This is just a big box to place all other logical volumes inside
    G4Box *SolidWorld = new G4Box("SolidWorld", 10.1 * m, 10.1 * m, 10.1 * m);
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

void PALLAS_CollSimGeometryConstruction::ConstructBField()
{

    if (StatusMapBField)
    {
        LogicalBFieldVolume = Geom->GetBFieldVolume();

        // // LogicalCollimator->SetVisAttributes(black);
        SetLogicalVolumeColor(LogicalBFieldVolume, "gray");

        PhysicalBFieldVolume = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(-0.152 * mm, 3449.5 * mm, 10.855 * mm)),
                                                 LogicalBFieldVolume, "BFieldVolume",
                                                 LogicalHolder, false, 0);




        // TO DO !!!!
    }

    else
    {
        LogicalBFieldVolume = Geom->GetBFieldVolume();

        // // LogicalCollimator->SetVisAttributes(black);
        SetLogicalVolumeColor(LogicalBFieldVolume, "gray");

        PhysicalBFieldVolume = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(-0.152 * mm, 3449.5 * mm, 10.855 * mm)),
                                                 LogicalBFieldVolume, "BFieldVolume",
                                                 LogicalHolder, false, 0);

        // #########################
        //  DEFINE EM FIELD PART   #
        // #########################

        G4double fMinStep = 0.010 * mm; // minimal step of 1 mm is default
        G4bool useFSALstp = false;

        localmagField = new G4UniformMagField(G4ThreeVector(-ConstantBField, 0.0, 0.0));
        // G4Mag_UsualEqRhs* fEquation = new G4Mag_UsualEqRhs(magField);
        fEquationlocal = new G4Mag_UsualEqRhs(localmagField);

        // G4FieldManager* MagFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
        //  fieldMgr->SetDetectorField(magField);
        //  fieldMgr->CreateChordFinder(magField);

        // Update field
        // G4MagIntegratorStepper*fStepper;

        // localfStepperMag = new G4ClassicalRK4( fEquationlocal ,8);
        // localfStepperMag = new G4DormandPrince745( fEquationlocal,8 );
        // localfStepperMag = new G4NystromRK4(fEquationlocal);
        // localfStepperMag = new G4HelixExplicitEuler( fEquationlocal );
        // localfStepperMag = new G4ExactHelixStepper( fEquationlocal );

        // auto pIntegrationDriver = new G4IntegrationDriver<G4DormandPrince745>(fMinStep, localfStepperMag, 8);

        // fFieldManager->SetDetectorField(magField);
        G4FieldManager *LocalMagFieldManager = new G4FieldManager(localmagField);

        // G4ChordFinder*fLocalChordFinder = new G4ChordFinder( localmagField, fMinStep, localfStepperMag);
        // G4ChordFinder*fLocalChordFinder = new G4ChordFinder(pIntegrationDriver);
        G4int driverId = 3; // B-field driver =3
        auto pChordFinder = new G4ChordFinder(localmagField, fMinStep, nullptr, driverId);

        // Set accuracy parameters
        G4double deltaChord = 0.25 * um; // 3.0 mm by default
        G4double epsMax = 1.0e-4;        // Pure number -- maximum relative integration error
        G4double epsMin = 1e-5;          //
        G4double deltaOneStep = 1 * um;
        G4double deltaIntersection = 1 * um;

        // fChordFinder->SetDeltaChord( deltaChord );
        pChordFinder->SetDeltaChord(deltaChord);
        LocalMagFieldManager->SetAccuraciesWithDeltaOneStep(deltaOneStep);
        LocalMagFieldManager->SetMinimumEpsilonStep(epsMin);
        LocalMagFieldManager->SetMaximumEpsilonStep(epsMax);
        LocalMagFieldManager->SetDeltaIntersection(deltaIntersection);
        LocalMagFieldManager->SetDeltaOneStep(0.01 * um);

        // fFieldManager->CreateChordFinder(magField);
        // fFieldManager->SetChordFinder(fChordFinder);
        LocalMagFieldManager->SetChordFinder(pChordFinder);

        LogicalBFieldVolume->SetFieldManager(LocalMagFieldManager, true);
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructCollimator()
{
    LogicalCollimator = Geom->GetRoundCollimator();
    G4GeometryManager::GetInstance()->OpenGeometry();

    auto pipe = static_cast<G4Tubs *>(G4LogicalVolumeStore::GetInstance()->GetVolume("RoundCollimator")->GetSolid());
    pipe->SetInnerRadius(CollimatorInternalRadius);
    pipe->SetOuterRadius(CollimatorExternalRadius);
    pipe->SetZHalfLength(CollimatorThickness / 2);

    SetLogicalVolumeColor(LogicalCollimator, "black");

    G4RotationMatrix *rotationMatrix = new G4RotationMatrix();
    rotationMatrix->rotateX(90.0 * deg);

    G4ThreeVector translation(0.152 * mm, 3109.5 - CollimatorSpectrometerDistance, 0.08 * mm);

    PhysicalCollimator = new G4PVPlacement(rotationMatrix,
                                           translation,
                                           LogicalCollimator,
                                           "Collimator",
                                           LogicalHolder,
                                           false,
                                           0);
}

void PALLAS_CollSimGeometryConstruction::ConstructCollimatorWithOutput()
{
    LogicalCollimator = Geom->GetRoundCollimator();
    LogicalFrontOutput = Geom->GetOutputCollimator();
    LogicalBackOutput = Geom->GetOutputCollimator();
    G4GeometryManager::GetInstance()->OpenGeometry();

    auto pipe = static_cast<G4Tubs *>(G4LogicalVolumeStore::GetInstance()->GetVolume("RoundCollimator")->GetSolid());
    pipe->SetInnerRadius(CollimatorInternalRadius);
    pipe->SetOuterRadius(CollimatorExternalRadius);
    pipe->SetZHalfLength(CollimatorThickness / 2);

    SetLogicalVolumeColor(LogicalCollimator, "black");
    SetLogicalVolumeColor(LogicalFrontOutput, "yellow");
    SetLogicalVolumeColor(LogicalBackOutput, "red");

    G4RotationMatrix *rotationMatrix = new G4RotationMatrix();
    rotationMatrix->rotateX(90.0 * deg);

    G4ThreeVector translation(0.152 * mm, 3109.5 - CollimatorSpectrometerDistance, 0.08 * mm);

    Y_FrontOutput = (3109.5 - CollimatorSpectrometerDistance) + 0.5 + CollimatorThickness / 2;
    Y_BackOutput = (3109.5 - CollimatorSpectrometerDistance) - 0.5 - CollimatorThickness / 2;

    PhysicalCollimator = new G4PVPlacement(rotationMatrix,
                                           translation,
                                           LogicalCollimator,
                                           "Collimator",
                                           LogicalHolder,
                                           false,
                                           0);

    G4ThreeVector translationFront(0.152 * mm, Y_FrontOutput, 0.08 * mm);
    G4ThreeVector translationBack(0.152 * mm, Y_BackOutput, 0.08 * mm);

    PhysicalFrontOutput = new G4PVPlacement(rotationMatrix,
                                            translationFront,
                                            LogicalFrontOutput,
                                            "FrontOutput",
                                            LogicalHolder,
                                            false,
                                            0);

    PhysicalBackOutput = new G4PVPlacement(rotationMatrix,
                                           translationBack,
                                           LogicalBackOutput,
                                           "BackOutput",
                                           LogicalHolder,
                                           false,
                                           0);
}

void PALLAS_CollSimGeometryConstruction::ConstructCellulePart()
{
    if (StatusDisplayCelluleGeometry)
    {
        LogicalPALLAS_Assemblage2Cellules = Geom->GetPALLAS_Assemblage2Cellules();

        SetLogicalVolumeColor(LogicalPALLAS_Assemblage2Cellules, "magenta");

        PhysicalPALLAS_Assemblage2Cellules = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                               LogicalPALLAS_Assemblage2Cellules, "Assemblage2Cellules",
                                                               LogicalHolder, false, 0);
    }
    else
    {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructLIFPart()
{
    if (StatusDisplayLIFGeometry)
    {
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

        PhysicalPALLAS_Croix = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                 LogicalPALLAS_Croix, "Croix",
                                                 LogicalHolder, false, 0);

        PhysicalPALLAS_OptoMeK = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                   LogicalPALLAS_OptoMeK, "OptoMeK",
                                                   LogicalHolder, false, 0);

        PhysicalPALLAS_LIFHublot1 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                      LogicalPALLAS_LIFHublot1, "LIFHublot1",
                                                      LogicalHolder, false, 0);

        PhysicalPALLAS_LIFHublot2 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                      LogicalPALLAS_LIFHublot2, "LIFHublot2",
                                                      LogicalHolder, false, 0);

        PhysicalPALLAS_LIFHublot3 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                      LogicalPALLAS_LIFHublot3, "LIFHublot3",
                                                      LogicalHolder, false, 0);

        PhysicalPALLAS_LIFHublot4 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                      LogicalPALLAS_LIFHublot4, "LIFHublot4",
                                                      LogicalHolder, false, 0);

        PhysicalPALLAS_LIFHublot5 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                      LogicalPALLAS_LIFHublot5, "LIFHublot5",
                                                      LogicalHolder, false, 0);

        PhysicalPALLAS_LIF_IBX_DD = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                      LogicalPALLAS_LIF_IBX_DD, "LIF_IBX_DD",
                                                      LogicalHolder, false, 0);

        PhysicalPALLAS_LIF_SQLT = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                    LogicalPALLAS_LIF_SQLT, "LIF_SQLT",
                                                    LogicalHolder, false, 0);

        PhysicalPALLAS_MarbreBreadboard1 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                             LogicalPALLAS_MarbreBreadboard1, "MarbreBreadboard1",
                                                             LogicalHolder, false, 0);

        PhysicalPALLAS_MarbreBreadboard2 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                             LogicalPALLAS_MarbreBreadboard2, "MarbreBreadboard2",
                                                             LogicalHolder, false, 0);
    }
    else
    {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructSection1Part()
{
    if (StatusDisplaySection1Geometry)
    {
        LogicalPALLAS_QuadrupoleQ1 = Geom->GetPALLAS_QuadrupoleQ1();
        LogicalPALLAS_QuadrupoleQ2 = Geom->GetPALLAS_QuadrupoleQ2();
        LogicalPALLAS_ATH500_DN100 = Geom->GetPALLAS_ATH500_DN100();
        LogicalPALLAS_ChambreISO = Geom->GetPALLAS_ChambreISO();
        LogicalPALLAS_BaseMarbre = Geom->GetPALLAS_BaseMarbre();

        SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ1, "red");
        SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ2, "red");
        SetLogicalVolumeColor(LogicalPALLAS_ATH500_DN100, "red");
        SetLogicalVolumeColor(LogicalPALLAS_ChambreISO, "red");
        SetLogicalVolumeColor(LogicalPALLAS_BaseMarbre, "red");

        PhysicalPALLAS_QuadrupoleQ1 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                        LogicalPALLAS_QuadrupoleQ1, "QuadrupoleQ1",
                                                        LogicalHolder, false, 0);

        PhysicalPALLAS_QuadrupoleQ2 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                        LogicalPALLAS_QuadrupoleQ2, "QuadrupoleQ2",
                                                        LogicalHolder, false, 0);

        PhysicalPALLAS_ATH500_DN100 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                        LogicalPALLAS_ATH500_DN100, "ATH500_DN100",
                                                        LogicalHolder, false, 0);

        PhysicalPALLAS_BaseMarbre = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                      LogicalPALLAS_BaseMarbre, "BaseMarbre",
                                                      LogicalHolder, false, 0);

        PhysicalPALLAS_ChambreISO = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                      LogicalPALLAS_ChambreISO, "ChambreISO",
                                                      LogicalHolder, false, 0);
    }
    else
    {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructSection2Part()
{
    if (StatusDisplaySection2Geometry)
    {
        LogicalPALLAS_QuadrupoleQ3 = Geom->GetPALLAS_QuadrupoleQ3();
        LogicalPALLAS_QuadrupoleQ4 = Geom->GetPALLAS_QuadrupoleQ4();
        LogicalPALLAS_ASMRemovalChamber = Geom->GetPALLAS_ASMRemovalChamber();
        LogicalPALLAS_BreadboardRemovalChamber = Geom->GetPALLAS_BreadboardRemovalChamber();
        LogicalPALLAS_ChassisRemovalChamber = Geom->GetPALLAS_ChassisRemovalChamber();
        LogicalPALLAS_TubeISO1 = Geom->GetPALLAS_TubeISO1();
        LogicalPALLAS_TubeISO2 = Geom->GetPALLAS_TubeISO2();

        SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ3, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_QuadrupoleQ4, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_ASMRemovalChamber, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_BreadboardRemovalChamber, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_ChassisRemovalChamber, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_TubeISO1, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_TubeISO2, "cyan");

        PhysicalPALLAS_QuadrupoleQ3 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                        LogicalPALLAS_QuadrupoleQ3, "QuadrupoleQ3",
                                                        LogicalHolder, false, 0);

        PhysicalPALLAS_QuadrupoleQ4 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                        LogicalPALLAS_QuadrupoleQ4, "QuadrupoleQ4",
                                                        LogicalHolder, false, 0);

        PhysicalPALLAS_ASMRemovalChamber = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                             LogicalPALLAS_ASMRemovalChamber, "ASMRemovalChamber",
                                                             LogicalHolder, false, 0);

        PhysicalPALLAS_BreadboardRemovalChamber = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                                    LogicalPALLAS_BreadboardRemovalChamber, "BreadboardRemovalChamber",
                                                                    LogicalHolder, false, 0);

        PhysicalPALLAS_ChassisRemovalChamber = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                                 LogicalPALLAS_ChassisRemovalChamber, "ChassisRemovalChamber",
                                                                 LogicalHolder, false, 0);

        PhysicalPALLAS_TubeISO1 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                    LogicalPALLAS_TubeISO1, "TubeISO1",
                                                    LogicalHolder, false, 0);

        PhysicalPALLAS_TubeISO2 = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                    LogicalPALLAS_TubeISO2, "TubeISO2",
                                                    LogicalHolder, false, 0);
    }
    else
    {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructSection3Part()
{
    if (StatusDisplaySection3Geometry)
    {
        LogicalPALLAS_ASMPoutre = Geom->GetPALLAS_ASMPoutre();
        LogicalPALLAS_StationYAG = Geom->GetPALLAS_StationYAG();

        SetLogicalVolumeColor(LogicalPALLAS_ASMPoutre, "orange");
        SetLogicalVolumeColor(LogicalPALLAS_StationYAG, "orange");

        PhysicalPALLAS_ASMPoutre = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                     LogicalPALLAS_ASMPoutre, "ASMPoutre",
                                                     LogicalHolder, false, 0);

        PhysicalPALLAS_StationYAG = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, -300 * mm, 0 * mm)),
                                                      LogicalPALLAS_StationYAG, "StationYAG",
                                                      LogicalHolder, false, 0);
    }
    else
    {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructSection4Part()
{
    if (StatusDisplaySection4Geometry)
    {
        LogicalPALLAS_ChambreDipole = Geom->GetPALLAS_ChambreDipole();
        LogicalPALLAS_Dipole = Geom->GetPALLAS_Dipole();
        LogicalPALLAS_BS1YAG = Geom->GetPALLAS_BS1YAG();
        LogicalPALLAS_BSPEC1YAG = Geom->GetPALLAS_BSPEC1YAG();

        SetLogicalVolumeColor(LogicalPALLAS_ChambreDipole, "cyan");
        SetLogicalVolumeColor(LogicalPALLAS_Dipole, "red");
        SetLogicalVolumeColor(LogicalPALLAS_BS1YAG, "yellow");
        SetLogicalVolumeColor(LogicalPALLAS_BSPEC1YAG, "yellow");

        PhysicalPALLAS_ChambreDipole = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0.152 * mm, -3449.5 * mm, -10.855 * mm)),
                                                         LogicalPALLAS_ChambreDipole, "ChambreDipole",
                                                         LogicalBFieldVolume, false, 0);

        PhysicalPALLAS_Dipole = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0.152 * mm, -3449.5 * mm, -10.855 * mm)),
                                                  LogicalPALLAS_Dipole, "Dipole",
                                                  LogicalBFieldVolume, false, 0);

        PhysicalPALLAS_BS1YAG = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                  LogicalPALLAS_BS1YAG, "BS1_YAG",
                                                  LogicalHolder, false, 0);

        PhysicalPALLAS_BSPEC1YAG = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                     LogicalPALLAS_BSPEC1YAG, "BSPEC1_YAG",
                                                     LogicalHolder, false, 0);
    }
    else
    {
        // DO NOTHING
    }
}

void PALLAS_CollSimGeometryConstruction::ConstructSection4DumpPart()
{
    if (StatusDisplaySection4DumpGeometry)
    {
        LogicalPALLAS_BlindageBD = Geom->GetPALLAS_BlindageBD();
        LogicalPALLAS_BlindageCBD = Geom->GetPALLAS_BlindageCBD();
        LogicalPALLAS_ChassisDipoleYAG = Geom->GetPALLAS_ChassisDipoleYAG();
        LogicalPALLAS_DiagsChamber = Geom->GetPALLAS_DiagsChamber();

        SetLogicalVolumeColor(LogicalPALLAS_BlindageBD, "blue");
        SetLogicalVolumeColor(LogicalPALLAS_BlindageCBD, "blue");
        SetLogicalVolumeColor(LogicalPALLAS_ChassisDipoleYAG, "blue");
        SetLogicalVolumeColor(LogicalPALLAS_DiagsChamber, "blue");

        PhysicalPALLAS_BlindageBD = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                      LogicalPALLAS_BlindageBD, "BlindageBD",
                                                      LogicalHolder, false, 0);

        PhysicalPALLAS_BlindageCBD = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                       LogicalPALLAS_BlindageCBD, "BlindageCBD",
                                                       LogicalHolder, false, 0);

        PhysicalPALLAS_ChassisDipoleYAG = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                            LogicalPALLAS_ChassisDipoleYAG, "ChassisDipoleYAG",
                                                            LogicalHolder, false, 0);

        PhysicalPALLAS_DiagsChamber = new G4PVPlacement(G4Transform3D(DontRotate, G4ThreeVector(0 * mm, 0 * mm, 0 * mm)),
                                                        LogicalPALLAS_DiagsChamber, "DiagsChamber",
                                                        LogicalHolder, false, 0);
    }
    else
    {
        // DO NOTHING
    }
}

G4VPhysicalVolume *PALLAS_CollSimGeometryConstruction::Construct()
{
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
    ConstructBField();
    // ConstructCollimator();
    ConstructCollimatorWithOutput();
    ConstructCellulePart();
    ConstructLIFPart();
    ConstructSection1Part();
    ConstructSection2Part();
    ConstructSection3Part();
    ConstructSection4Part();
    ConstructSection4DumpPart();

    // Returns world with everything in it and all properties set
    return PhysicalWorld;
}
