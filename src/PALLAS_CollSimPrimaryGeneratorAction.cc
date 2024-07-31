/// PALLAS_CollSimPrimaryGeneratorAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimPrimaryGeneratorAction.hh"

std::atomic<size_t> currentParticleNumber{0};

G4UImanager *UI = G4UImanager::GetUIpointer();
PALLAS_CollSimPrimaryGeneratorAction::PALLAS_CollSimPrimaryGeneratorAction(size_t N, size_t numThreads, std::vector<std::queue<ParticleData>> threadEventQueue, const std::vector<ParticleData> &particleData)
    : NEventsGenerated(N),
      numThreads(numThreads),
      G4VUserPrimaryGeneratorAction(),
      particleGun(nullptr),
      threadEventQueue(threadEventQueue),
      fParticleData(particleData),
      currentEvent(0)

{

  pMessenger = new G4GenericMessenger(this, "/gun/", "Control commands for my application");

  pMessenger->DeclareProperty("SetStatusGunParticle", StatusGunParticle)
      .SetGuidance("Set the boolean parameter.")
      .SetParameterName("StatusGunParticle", false)
      .SetDefaultValue("false");

  pMessenger->DeclareProperty("SetParticleName", ParticleName)
      .SetGuidance("Set the particle name.")
      .SetParameterName("ParticleName", false)
      .SetDefaultValue("geantino");

  pMessenger->DeclarePropertyWithUnit("SetEnergyReference", "MeV", EnergyReference)
      .SetGuidance("Set the energy reference.")
      .SetParameterName("EnergyReference", false)
      .SetDefaultValue("247 MeV")
      .SetRange("EnergyReference >=0.0");

  pMessenger->DeclarePropertyWithUnit("SetYParticleGenerationOffset", "mm", YParticleGenerationOffset)
      .SetGuidance("Set the YParticleGeneration parameter.")
      .SetParameterName("YParticleGenerationOffset", false)
      .SetDefaultValue("100.0 mm")
      .SetRange("YParticleGenerationOffset >=0.0");

  particleGun = new G4ParticleGun(1);
  particleSource = new G4GeneralParticleSource();

  //G4cout << "StatusGun = " << StatusGunParticle << G4endl;
}

PALLAS_CollSimPrimaryGeneratorAction::~PALLAS_CollSimPrimaryGeneratorAction()
{
  delete particleGun;
  delete particleSource;
  delete pMessenger;
}



void PALLAS_CollSimPrimaryGeneratorAction::SetParticleName()
{
  particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(ParticleName);

  if (!particleDefinition)
  {
    G4cerr << "Particle doesnt exist : RUN ABORT" << G4endl;
    G4RunManager::GetRunManager()->AbortRun();
    return;
  }
}

void PALLAS_CollSimPrimaryGeneratorAction::ShowProgress(double progress, std::chrono::high_resolution_clock::time_point startTime)
{
  // Fonction pour afficher une barre d'avancement
  int barWidth = 70;
  std::cout << "\033[1A\033[K"; // Move up one line and clear it
  std::cout << "[";
  int pos = barWidth * progress;
  for (int i = 0; i < barWidth; ++i)
  {
    if (i < pos)
      std::cout << "=";
    else if (i == pos)
      std::cout << ">";
    else
      std::cout << " ";
  }
  std::cout << "] " << int(progress * 100.0) << " %";
  std::cout.flush();

  auto currentTime = std::chrono::high_resolution_clock::now();
  elapsedTime = currentTime - startTime;
  progressRemaining = 1 - progress;
  averageTimePerProgress = elapsedTime.count() / progress;
  estimatedRemainingTime = averageTimePerProgress * progressRemaining;

  // Set the precision for floating point output
  std::cout << std::fixed << std::setprecision(1) << std::endl;

  std::cout << "=> Estimated remaining time = " << estimatedRemainingTime / 60.0 << " min" << "\r";
  std::cout.flush();
}




void PALLAS_CollSimPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  G4int threadID = G4Threading::G4GetThreadId();

  if (!isStartTimeInitialized)
  {
    startTime = std::chrono::high_resolution_clock::now();
    isStartTimeInitialized = true;
  }

  if (StatusGunParticle == true)
  {
    // G4cout << "Thread Event queue " << threadID << "  size = " << threadEventQueue[threadID].size() << G4endl;

    // Extraire les données d'événements de la file d'attente
    ParticleData pdata = threadEventQueue[threadID].front();
    threadEventQueue[threadID].pop();

    //G4cout << "Size pdata = " << threadEventQueue[threadID].size() << G4endl;

    SetParticleName();

    G4double yp = sqrt(1 / (pdata.xp * pdata.xp + pdata.zp * pdata.zp + 1));

    xOffset = -0.152;                             // mm
    sOffset = 3114.5 - YParticleGenerationOffset; // mm
    zOffset = 0.08;                               // mm

    // G4cout << " X = " << pdata.x * 1000 + xOffset << G4endl;
    // G4cout << " N = " << pdata.n << G4endl;

    particleGun->SetParticleDefinition(particleDefinition);
    particleGun->SetParticleEnergy(EnergyReference * (1 + pdata.delta));
    particleGun->SetParticlePosition(G4ThreeVector((pdata.x * 1000 + xOffset) * mm, (pdata.s * 1000 + sOffset) * mm, (pdata.z * 1000 + zOffset) * mm));
    particleGun->SetParticleMomentumDirection(G4ThreeVector(pdata.xp, yp, pdata.zp));

    // G4cout << "pdata.s = " << pdata.s*1000 << G4endl;
    // G4cout << "Y = " << pdata.s*1000+sOffset << G4endl;

    for (int i = 0; i < pdata.n; i++)
    {
      particleGun->GeneratePrimaryVertex(anEvent);
    }

  
    if (threadEventQueue[threadID].empty())
    {
      G4cerr << "Thread event queue is empty or not initialized!" << G4endl;
      // G4RunManager::GetRunManager()->AbortEvent();
      G4RunManager::GetRunManager()->TerminateEventLoop();
      G4RunManager::GetRunManager()->AbortRun();
      // G4RunManager::GetRunManager()->RunTermination();

      G4cout << "STOP THREAD" << G4endl;
      return;
    }

    if (threadID==0)
    {
      currentParticleNumber+=pdata.n;
      ShowProgress(double(numThreads * currentParticleNumber) / double(NEventsGenerated), startTime);
      // G4cout << "Num threads = " << numThreads << G4endl;
      // G4cout << "CurrentParticle Number = " << currentParticleNumber << G4endl;
      // G4cout << "TotalNParticle = " << TotalNParticles << G4endl;
      // G4cout << "progress = " << double(numThreads * currentParticleNumber) / double(NEventsGenerated) << G4endl;
    }
  }

  else
  {
    particleSource->GeneratePrimaryVertex(anEvent);
    currentParticleNumber++;
    currentEvent++;
    ShowProgress(double(currentEvent) / double(NEventsGenerated), startTime);
  }
    

  currentEvent++;
}
