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
#include <fstream>

std::vector<ParticleData> ReadParticleData(const std::string &filename, size_t maxLines)
{
  std::ifstream infile(filename);
  std::vector<ParticleData> data;

  G4cout << "maxLines = " << maxLines << G4endl;

  if (!infile.is_open())
  {
    G4cerr << "Error opening file: " << filename << G4endl;
    return data;
  }

  std::string line;
  size_t lineCount = 0;
  double macrocharge = 0;
  double totparticles = 0;

  while (std::getline(infile, line) && lineCount < maxLines)
  {
    if (line[0] == '#')
      continue; // Skip comments

    std::istringstream iss(line);
    ParticleData pdata;
    if (!(iss >> pdata.x >> pdata.xp >> pdata.z >> pdata.zp >> pdata.s >> pdata.delta >> macrocharge))
    {
      G4cerr << "Error reading line " << lineCount + 1 << G4endl;
      continue;
    }

    pdata.n = macrocharge / 1.6e-17; // SCALE 100 !!!!!
    data.push_back(pdata);
    lineCount++;
    totparticles += pdata.n;
  }

  G4cout << "Total particles generated: " << totparticles << G4endl;
  return data;
}

size_t GetTotalNParticlesGenerated(const std::vector<ParticleData> &particleDataList)
{
  size_t totalParticles = 0;
  for (const auto &pdata : particleDataList)
  {
    totalParticles += pdata.n;
  }
  return totalParticles;
}

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    G4Exception("Main", "main0004", FatalException,
                "Insufficient input arguments. Usage: ./PALLAS_CollSim [ROOT file name] [events] [macro] [FileReader ON/OFF] [MT ON/OFF] [threads (if MT ON)]");
    return 1;
  }

  char *outputFile = argv[1];
  bool flag_FileReader = false;
  bool flag_MT = false;
  size_t NEvents = 1;
  size_t Ncores = std::thread::hardware_concurrency();
  G4RunManager *runManager;
  std::vector<ParticleData> particleData;
  size_t TotalNParticles = 0;
  std::vector<std::queue<ParticleData>> threadEventQueues(Ncores);

  if (argc == 2)  // VISUALIZATION MODE
  {
    runManager = new G4RunManager;
  }
  else if (argc >= 6)  // BATCH MODE
  {
    NEvents = std::stoul(argv[2]);
    G4String pFileReader = argv[4];
    G4String pMT = argv[5];

    if (pFileReader == "ON")
    {
      flag_FileReader = true;
      particleData = ReadParticleData("PALLAS_phasespace_qm(1411)_preCollimateur.txt", NEvents);
      TotalNParticles = GetTotalNParticlesGenerated(particleData);
    }
    else if (pFileReader == "OFF")
    {
      flag_FileReader = false;
      TotalNParticles = NEvents;
    }
    else
    {
      G4Exception("Main", "main0001", FatalException,
                  "FileReader parameter (5th argument) must be ON or OFF.");
    }

    if (pMT == "ON")
    {
      flag_MT = true;
      runManager = new G4MTRunManager;
      if (argc == 7)
        Ncores = std::stoul(argv[6]);

      runManager->SetNumberOfThreads(Ncores);
    }
    else if (pMT == "OFF")
    {
      flag_MT = false;
      runManager = new G4RunManager;
    }
    else
    {
      G4Exception("Main", "main0002", FatalException,
                  "MT parameter (6th argument) must be ON or OFF.");
    }

    if (flag_FileReader && flag_MT)
    {
      std::mutex queueMutex;
      std::vector<std::thread> threads;

      // Distribuer les événements aux threads
      for (size_t i = 0; i < particleData.size(); ++i)
      {
        threadEventQueues[i % Ncores].push(particleData[i]);
      }
    }
    else if (flag_FileReader && !flag_MT)
    {
      for (size_t i = 0; i < particleData.size(); ++i)
      {
        threadEventQueues[0].push(particleData[i]);
      }
    }
  }
  else
  {
    G4Exception("Main", "main0003", FatalException,
                "Incorrect number of input parameters.");
  }

  // Setup geometry
  Geometry *Geom = new Geometry();
  PALLAS_CollSimGeometryConstruction *GeomCons = new PALLAS_CollSimGeometryConstruction;
  runManager->SetUserInitialization(GeomCons);

  // Initialize physics
  runManager->SetUserInitialization(new PALLAS_CollSimPhysics);

  // Initialize user actions
  runManager->SetUserInitialization(new PALLAS_CollSimActionInitialization(outputFile, TotalNParticles, Ncores, threadEventQueues, particleData, flag_FileReader, flag_MT));

  // Visualization
  G4VisManager *visManager = new G4VisExecutive;
  visManager->Initialize();

  // Initialize the kernel
  runManager->Initialize();

  G4UImanager *UI = G4UImanager::GetUIpointer();

  if (argc == 2)  // VISUALIZATION MODE
  {
    G4UIExecutive *ui = new G4UIExecutive(argc, argv);
    UI->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
  }
  else if (argc >= 6)  // BATCH MODE
  {
    G4String command = "/control/execute ";
    G4String macro = argv[3];
    UI->ApplyCommand(command + macro);

    std::string runCommand = "/run/beamOn " + std::string(argv[2]);
    UI->ApplyCommand(runCommand);

    if (flag_MT)
    {
      std::string mergeCommand = "/control/shell hadd -k -f " + std::string(outputFile) + ".root";
      for (size_t i = 1; i <= Ncores; ++i)
      {
        mergeCommand += " " + std::string(outputFile) + "_" + std::to_string(i) + ".root";
      }
      UI->ApplyCommand(mergeCommand);

      // Clean up temporary files
      for (size_t i = 1; i <= Ncores; ++i)
      {
        std::string removeCommand = "/control/shell rm -f " + std::string(outputFile) + "_" + std::to_string(i) + ".root";
        UI->ApplyCommand(removeCommand);
      }
    }

    std::string movefile = "/control/shell mv " + std::string(outputFile) + ".root ../Resultats";
    UI->ApplyCommand(movefile);
    G4cout << "Output saved in Resultats fodler to file " << outputFile << ".root" << G4endl;
  }

  // Final clean-up
  delete visManager;
  delete runManager;

  return 0;
}
