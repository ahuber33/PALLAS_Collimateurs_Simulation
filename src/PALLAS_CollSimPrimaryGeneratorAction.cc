/// PALLAS_CollSimPrimaryGeneratorAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimPrimaryGeneratorAction.hh"

G4UImanager *UI = G4UImanager::GetUIpointer();
PALLAS_CollSimPrimaryGeneratorAction::PALLAS_CollSimPrimaryGeneratorAction(const char *N) : s_NEventsGenerated(N),
                                                                                      G4VUserPrimaryGeneratorAction(),
                                                                                      particleGun(nullptr),
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

  pMessenger->DeclareProperty("SetPhaseSpaceFilename", PhaseSpaceFilename)
      .SetGuidance("Set the phase space filename.")
      .SetParameterName("PhaseSpaceFilename", true)
      .SetDefaultValue("test.txt");

  pMessenger->DeclareProperty("SetMacroChargeFilename", MacroChargeFilename)
      .SetGuidance("Set the macro charge filename.")
      .SetParameterName("MacroChargeFilename", true)
      .SetDefaultValue("test_qm.txt");

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
}

PALLAS_CollSimPrimaryGeneratorAction::~PALLAS_CollSimPrimaryGeneratorAction()
{
  delete particleGun;
  delete particleSource;
  delete pMessenger;
}

size_t PALLAS_CollSimPrimaryGeneratorAction::charToSizeT(const char *str)
{
  char *end;
  errno = 0; // Réinitialiser errno avant l'appel

  // Conversion de char* en unsigned long long (compatible avec size_t)
  unsigned long long num = std::strtoull(str, &end, 10);

  if (errno != 0 || *end != '\0')
  {
    throw std::invalid_argument("Erreur de conversion");
  }

  return static_cast<size_t>(num);
}

void PALLAS_CollSimPrimaryGeneratorAction::ReadNumberFile(const std::string &filename)
{
  std::ifstream infile(filename);
  if (!infile)
  {
    G4cerr << "Could not open file " << filename << G4endl;
    return;
  }

  int numParticles;
  int i = 0;
  double macrocharge;
  EventNumberOfParticles.clear();

  NEventsGenerated = charToSizeT(s_NEventsGenerated);

  std::string line;

  while (std::getline(infile, line))
  {
    if (line[0] == '#')
      continue; // Skip comments
    std::istringstream iss(line);
    iss >> macrocharge;
    numParticles = macrocharge / (1.6e-17); //SCALE 100 !!!!!
    // G4cout << "\n Number of PART = " << numParticles << G4endl;
    EventNumberOfParticles.push_back(numParticles);
    if (i <= NEventsGenerated)
      TotalNParticles += numParticles;
    i++;
  }

  G4cout << "\nTOTAL EVENTS = " << TotalNParticles << G4endl;
  G4cout << "\n"
         << G4endl;

  infile.close();
}

void PALLAS_CollSimPrimaryGeneratorAction::ReadParticleFile(const std::string &filename)
{
  std::ifstream infile(filename);
  particleDataList.clear();

  if (!infile.is_open())
  {
    G4cerr << "Error opening file : " << filename << G4endl;
    return;
  }

  std::string line;

  while (std::getline(infile, line))
  {
    if (line[0] == '#')
      continue; // Skip comments
    std::istringstream iss(line);
    ParticleData pdata;
    iss >> pdata.x >> pdata.xp >> pdata.z >> pdata.zp >> pdata.s >> pdata.delta;
    // G4cout << "\nx : " << pdata.x << G4endl;
    // G4cout << "xp : " << pdata.xp << G4endl;
    // G4cout << "z : " << pdata.z << G4endl;
    // G4cout << "zp : " << pdata.zp << G4endl;
    // G4cout << "s : " << pdata.s << G4endl;
    // G4cout << "delta : " << pdata.delta << G4endl;

    particleDataList.push_back(pdata);
  }

  infile.close();
}

void PALLAS_CollSimPrimaryGeneratorAction::GunParticleInitialization()
{
  currentEvent = 0;
  currentParticleNumber = 0;
  TotalNParticles = 0;

  ReadParticleFile(PhaseSpaceFilename);
  ReadNumberFile(MacroChargeFilename);

  if (particleDataList.empty())
  {
    G4cerr << "No particle data available : RUN ABORT" << G4endl;
    G4RunManager::GetRunManager()->AbortRun();
    return;
  }

  if (particleDataList.size() != EventNumberOfParticles.size())
  {
    G4cerr << "Difference between Phase space & Macrocharge vector size : RUN ABORT" << G4endl;
    G4cout << "phase size = " << particleDataList.size() << G4endl;
    G4cout << "qm size = " << EventNumberOfParticles.size() << G4endl;
    G4RunManager::GetRunManager()->AbortRun();
    return;
  }
}

void PALLAS_CollSimPrimaryGeneratorAction::GPSInitialization()
{
  TotalNParticles = charToSizeT(s_NEventsGenerated);
}

void PALLAS_CollSimPrimaryGeneratorAction::AlwaysInSetVerification()
{
  if (currentEvent >= particleDataList.size())
  {
    G4cerr << "No more particle data avalaible => ENDING RUN !!!" << G4endl;
    G4RunManager::GetRunManager()->AbortRun();
    return;
  }
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

  std::cout << "=> Estimated remaining time = " << estimatedRemainingTime/60.0 << " min" << "\r";
  std::cout.flush();

}

void PALLAS_CollSimPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  if (!isStartTimeInitialized)
  {
    startTime = std::chrono::high_resolution_clock::now();
    isStartTimeInitialized = true;
  }

  if (StatusGunParticle == true)
  {
    if (currentEvent == 0)
      GunParticleInitialization();

    SetParticleName();

    const ParticleData &pdata = particleDataList[currentEvent];
    currentEvent++;

    G4double yp = sqrt(1 / (pdata.xp * pdata.xp + pdata.zp * pdata.zp + 1));

    xOffset = -0.152; //mm
    sOffset = 3114.5 - YParticleGenerationOffset; //mm
    zOffset = 0.08; //mm

    particleGun->SetParticleDefinition(particleDefinition);
    particleGun->SetParticleEnergy(EnergyReference * (1+pdata.delta));
    particleGun->SetParticlePosition(G4ThreeVector((pdata.x*1000 + xOffset) * mm, (pdata.s*1000 + sOffset) * mm, (pdata.z*1000 + zOffset) * mm));
    particleGun->SetParticleMomentumDirection(G4ThreeVector(pdata.xp, yp, pdata.zp));

    // G4cout << "pdata.s = " << pdata.s*1000 << G4endl;
    // G4cout << "Y = " << pdata.s*1000+sOffset << G4endl;

    eventID = anEvent->GetEventID();
    nEvent = EventNumberOfParticles.at(eventID);
    //nEvent = 1;
    // G4cout << "nEvent = " << nEvent << G4endl;
    currentParticleNumber += nEvent;
    //G4cout << "N event = " << nEvent << G4endl;

    for (int i = 0; i < nEvent; i++)
    {
      particleGun->GeneratePrimaryVertex(anEvent);
    }

    AlwaysInSetVerification();
  }

  else
  {
    if (currentEvent == 0)
      GPSInitialization();

    particleSource->GeneratePrimaryVertex(anEvent);
    currentParticleNumber++;
    currentEvent++;
  }

  if (TotalNParticles >1000) ShowProgress(double(currentParticleNumber) / double(TotalNParticles), startTime);
}
