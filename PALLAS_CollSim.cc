/// PALLAS_CollSim.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "G4UIterminal.hh"
#include "PALLAS_CollSimSteppingAction.hh"
#include "PALLAS_CollSimPhysics.hh"
#include "PALLAS_CollSimPrimaryGeneratorAction.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Geometry.hh"


int main(int argc, char **argv)
{

  char *suff = argv[1];
  char* NEvents = argv[2];

  // Use SteppingVerbose with Unit
  // G4int precision = 4;
  // G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager
  G4RunManager *runManager = new G4RunManager;

  // set mandatory initialization classes
  Geometry *Geom = new Geometry();
  PALLAS_CollSimGeometryConstruction *GeomCons = new PALLAS_CollSimGeometryConstruction;

  G4cout << "Geometry given to PALLAS_CollSim.cc" << G4endl;

  // initialize the geometry
  runManager->SetUserInitialization(GeomCons);
  G4cout << "Geometry set in PALLAS_CollSim.cc given to Runman" << G4endl;

  // initialize the physics
  runManager->SetUserInitialization(new PALLAS_CollSimPhysics);

  // set mandatory user action class
  PALLAS_CollSimPrimaryGeneratorAction *Generator = new PALLAS_CollSimPrimaryGeneratorAction(NEvents);
  runManager->SetUserAction(Generator);

  // set Run Event and Stepping action classes
  runManager->SetUserAction(new PALLAS_CollSimRunAction(suff));
  G4cout << "Initialized new Run Action" << G4endl;

  runManager->SetUserAction(new PALLAS_CollSimEventAction(suff));
  G4cout << "Initialized new EventAction" << G4endl;
  runManager->SetUserAction(new PALLAS_CollSimSteppingAction);
  G4cout << "Initialized new SteppingAction" << G4endl;
  // new PALLAS_CollSimMessenger(OptGeom, Generator);
  // G4cout << "Initialized new Messenger" << G4endl;

  // #ifdef G4VIS_USE
  G4VisManager *visManager = new G4VisExecutive;
  visManager->Initialize();
  // #endif

  // Initialize G4 kernel
  runManager->Initialize();

  G4cout << "Initialized new Run Manager" << G4endl;

  // get the pointer to the User Interface manager
  G4UImanager *UI = G4UImanager::GetUIpointer();
  char movefile[100];

  G4UIExecutive *ui = 0;

  if (argc == 4) // batch mode
  {
    G4cout << "Batch MODE" << G4endl;
    G4String command = "/control/execute ";
    G4String fileName = argv[3];
    UI->ApplyCommand(command + fileName);
    UI->ApplyCommand("control/suppressAbortion");

    char startcommand[100];
    sprintf(startcommand, "/run/beamOn %s", argv[2]);
    UI->ApplyCommand(startcommand);
    // G4cout << "3" << G4endl;
    // sprintf(writefile,"/control/shell mv %s.root ../Resultats/",argv[2]);
    // UI->ApplyCommand(writefile);

    sprintf(movefile, "/control/shell mv %s.root ../Resultats", argv[1]);
    UI->ApplyCommand(movefile);
    G4cout << "Output saved to file " << argv[1] << ".root" << G4endl;
  }
  
  else // define visualization and UI terminal for interactive mode
  {

    G4cout << "Interactive MODE" << G4endl;

    // #ifdef G4UI_USE
    ui = new G4UIExecutive(argc, argv);
    UI->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
    // #endif

    sprintf(movefile, "/control/shell mv %s.root ../Resultats", argv[1]);
    UI->ApplyCommand(movefile);
    G4cout << "Output saved to file " << argv[1] << ".root" << G4endl;
  }

#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;
  return 0;
}
