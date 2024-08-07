/// PALLAS_CollSim.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "G4UIterminal.hh"
#include "PALLAS_CollSimSteppingAction.hh"
#include "PALLAS_CollSimPhysics.hh"
#include "PALLAS_CollSimPrimaryGeneratorAction.hh"
#include "PALLAS_CollSimActionInitialization.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Geometry.hh"
#include "G4MTRunManager.hh"
#include "ParticleData.hh"
#include <thread>
#include <mutex>

std::vector<ParticleData> ReadParticleData(const std::string &filename, size_t maxLines)
{
  std::ifstream infile(filename);
  std::vector<ParticleData> data;
  data.clear();

  if (!infile.is_open())
  {
    G4cerr << "Error opening file : " << filename << G4endl;
    return data;
  }

  std::string line;
  int lineCount = 0;
  double macrocharge = 0;
  double totparticles = 0;

  while (std::getline(infile, line) && lineCount < maxLines)
  {
    if (line[0] == '#')
      continue; // Skip comments
    std::istringstream iss(line);
    ParticleData pdata;
    iss >> pdata.x >> pdata.xp >> pdata.z >> pdata.zp >> pdata.s >> pdata.delta >> macrocharge;
    pdata.n = macrocharge / (1.6e-17); // SCALE 100 !!!!!
                                       // G4cout << "\nx : " << pdata.x << G4endl;
    // G4cout << "xp : " << pdata.xp << G4endl;
    // G4cout << "z : " << pdata.z << G4endl;
    // G4cout << "zp : " << pdata.zp << G4endl;
    // G4cout << "s : " << pdata.s << G4endl;
    // G4cout << "delta : " << pdata.delta << G4endl;
    // G4cout << "n : " << pdata.n << G4endl;
    data.push_back(pdata);
    lineCount++;
    totparticles += pdata.n;
  }

  infile.close();

  G4cout << "N TOT PARTICLES GENERATED WILL BE : " << totparticles << G4endl;

  return data;
}

size_t GetTotalNParticlesGenerated(std::vector<ParticleData> &particleDataList)
{
  size_t TotalNParticles = 0;
  for (const auto &pdata : particleDataList)
  {
    TotalNParticles += pdata.n;
    // G4cout << "pdata.n = " << pdata.n << G4endl;
  }

  return TotalNParticles;
}

int main(int argc, char **argv)
{
  char *suff = argv[1];
  bool flag_FileReader = false;
  bool flag_MT = false;

  size_t NEvents = 1;
  size_t Ncores = std::thread::hardware_concurrency();
  size_t eventsPerThread;
  size_t remainder;
  G4RunManager *runManager;
  std::vector<ParticleData> particleData;
  ParticleData pdata;
  G4String PhaseSpaceFilename;
  size_t TotalNParticles;
  std::vector<std::queue<ParticleData>> threadEventQueues(Ncores);

  if (argc == 2)
    runManager = new G4RunManager; // VISUALIZATION MODE

  else if (argc >= 6) // BATCH MODE
  {
    NEvents = std::stoul(argv[2]);
    G4String pFileReader = argv[4];
    G4String pMT = argv[5];

    if (pFileReader == "ON")
    {
      flag_FileReader = true;
      PhaseSpaceFilename = "PALLAS_phasespace_qm(1411)_preCollimateur.txt";
      particleData = ReadParticleData(PhaseSpaceFilename, NEvents);
      TotalNParticles = GetTotalNParticlesGenerated(particleData);
    }

    else if (pFileReader == "OFF")
    {
      flag_FileReader = false;
      TotalNParticles = NEvents;
    }

    else
    {
      G4Exception("Main",
                  "main0001",     // code d'erreur unique
                  FatalException, // niveau de gravité
                  "FileReader parameter [5th parameter] ON/OFF is not well defined, Simulation will not begin.");
    }

    G4cout << "TOTAL N PARTICLES GENERATED = " << TotalNParticles << G4endl;

    if (pMT == "OFF")
    {
      flag_MT = false;
      runManager = new G4RunManager;
    }

    else if (pMT == "ON")
    {
      flag_MT = true;
      runManager = new G4MTRunManager;
      if (argc == 7)
        Ncores = std::stoul(argv[6]);

      eventsPerThread = NEvents / Ncores;
      remainder = NEvents % Ncores;
      runManager->SetNumberOfThreads(Ncores);
    }

    else
    {
      G4Exception("Main",
                  "main0002",     // code d'erreur unique
                  FatalException, // niveau de gravité
                  "MT parameter [6th parameter] ON/OFF is not well defined, Simulation will not begin.");
    }

    if (flag_FileReader == true && flag_MT == true)
    {
      std::vector<std::thread> threads(Ncores);
      std::mutex queueMutex;

      // Distribuer les événements aux files d'attente des threads
      int numThreads = static_cast<int>(Ncores);
      // G4cout << "THREAD EVENT QUEUE SIZE = " << threadEventQueues.size() << G4endl;
      // G4cout << "PARTICLE DATA SIZE = " << particleData.size() << G4endl;

      for (size_t i = 0; i < particleData.size(); i++)
      {
        threadEventQueues[i % numThreads].push(particleData[i]);
        // G4cout << "Thread Event queue " << i % numThreads << " size = " << threadEventQueues[i % numThreads].size() << G4endl;
      }
    }

    if (flag_FileReader == true && flag_MT == false)
    {
      for (size_t i = 0; i < particleData.size(); i++)
      {
        threadEventQueues[0].push(particleData[i]);
      }
    }
  }

  else
  {
    G4Exception("Main",
                "main0003",     // code d'erreur unique
                FatalException, // niveau de gravité
                "Problem with number of inputs parameter given. REMINDER : ./PALLAS_CollSim [name of ROOT file] [number of events generated] [name of macro] [ReadFile ON/OFF] [MT ON/OFF] [number of threads if MT ON]");
  }

  // Use SteppingVerbose with Unit
  // G4int precision = 4;
  // G4SteppingVerbose::UseBestUnit(precision);

  // set mandatory initialization classes
  Geometry *Geom = new Geometry();
  PALLAS_CollSimGeometryConstruction *GeomCons = new PALLAS_CollSimGeometryConstruction;

  G4cout << "Geometry given to PALLAS_CollSim.cc" << G4endl;

  // initialize the geometry
  runManager->SetUserInitialization(GeomCons);
  G4cout << "Geometry set in PALLAS_CollSim.cc given to Runman" << G4endl;

  // initialize the physics
  runManager->SetUserInitialization(new PALLAS_CollSimPhysics);

  // User action initialization
  runManager->SetUserInitialization(new PALLAS_CollSimActionInitialization(suff, TotalNParticles, Ncores, threadEventQueues, particleData, flag_FileReader, flag_MT));

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

  if (argc == 2) // VISUALIZATION MODE
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

  else if (argc >= 6) // batch mode
  {
    G4cout << "Batch MODE" << G4endl;
    G4String command = "/control/execute ";
    G4String macro = argv[3];
    G4String fileName = argv[1];
    UI->ApplyCommand(command + macro);
    UI->ApplyCommand("control/suppressAbortion");

    char startcommand[100];
    sprintf(startcommand, "/run/beamOn %s", argv[2]);
    // sprintf(startcommand, "/run/beamOn 10000000", argv[2]);
    UI->ApplyCommand(startcommand);

    if (flag_MT == true)
    {
      std::vector<std::string> generatedFiles;
      generatedFiles.clear();
      std::string stringValue;
      std::string genFile;

      for (int i = 1; i <= Ncores; i++)
      {
        stringValue = std::to_string(i);
        genFile = fileName + "_" + stringValue + ".root";
        generatedFiles.push_back(genFile);
      }

      std::string mergeFile;
      mergeFile.assign(argv[1]);
      mergeFile = mergeFile + ".root";
      G4cout << "mergeFile = " << mergeFile << G4endl;
      std::string mergecommand = "/control/shell hadd -k " + mergeFile;

      for (int i = 0; i < Ncores; i++)
      {
        // G4cout << "generated files [" << i << "] = " << generatedFiles.at(i) << G4endl;
        mergecommand += " " + generatedFiles.at(i);
        // G4cout << "Merge Command = " << mergecommand << G4endl;
      }

      UI->ApplyCommand(mergecommand);

      std::string removeFile;
      std::string removeCommand;
      for (int i = 0; i <= Ncores; i++)
      {
        stringValue = std::to_string(i);
        removeFile = fileName + "_" + stringValue + ".root";
        removeCommand = "/control/shell rm -f " + removeFile;
        G4cout << "removeCommand = " << removeCommand << G4endl;
        UI->ApplyCommand(removeCommand.c_str());
      }

      G4cout << "Thread Outputs x.root cleaned in bin folder" << G4endl;
    }

    sprintf(movefile, "/control/shell mv %s.root ../Resultats", argv[1]);
    UI->ApplyCommand(movefile);
    G4cout << "Output saved to file " << argv[1] << ".root" << G4endl;
  }

  else
  {
    G4Exception("Main",
                "main0004",     // code d'erreur unique
                FatalException, // niveau de gravité
                "Problem with number of inputs parameter given. REMINDER : ./PALLAS_CollSim [name of ROOT file] [number of events generated] [name of macro] [ReadFile ON/OFF] [MT ON/OFF] [number of threads if MT ON]");
  }

#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;
  return 0;
}
