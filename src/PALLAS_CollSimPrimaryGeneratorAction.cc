/// PALLAS_CollSimPrimaryGeneratorAction.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimPrimaryGeneratorAction.hh"

std::atomic<size_t> currentParticleNumber{0};

G4UImanager *UI = G4UImanager::GetUIpointer();
PALLAS_CollSimPrimaryGeneratorAction::PALLAS_CollSimPrimaryGeneratorAction(size_t N, size_t numThreads, std::vector<std::queue<ParticleData>> threadEventQueue, const std::vector<ParticleData> &particleData, G4bool pFileReader, G4bool pMT)
    : NEventsGenerated(N),
      numThreads(numThreads),
      G4VUserPrimaryGeneratorAction(),
      particleGun(nullptr),
      threadEventQueue(threadEventQueue),
      fParticleData(particleData),
      flag_FileReader(pFileReader),
      flag_MT(pMT),
      currentEvent(0)

{

  pMessenger = new G4GenericMessenger(this, "/gun/", "Control commands for my application");

  pMessenger->DeclareProperty("SetStatusGunParticle", StatusGunParticle)
      .SetGuidance("Set the boolean parameter.")
      .SetParameterName("StatusGunParticle", false)
      .SetDefaultValue("false");

  pMessenger->DeclareProperty("SetStatusONNX", StatusONNX)
      .SetGuidance("Set the boolean parameter.")
      .SetParameterName("StatusONNX", false)
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

  lMessenger = new G4GenericMessenger(this, "/laser/", "Control commands for my application");

  lMessenger->DeclareProperty("SetOffsetLaserFocus", fXof)
      .SetGuidance("Offset of the laser focus")
      .SetParameterName("fXof", false)
      .SetDefaultValue("0");

  lMessenger->DeclareProperty("SetNormVecPotential", fA0)
      .SetGuidance("Normalized vector potential")
      .SetParameterName("fA0", false)
      .SetDefaultValue("1");

  lMessenger->DeclareProperty("SetFracDopTargetChamber", fCN2)
      .SetGuidance("Fraction of dopant in the 1st chamber of the target")
      .SetParameterName("fCN2", false)
      .SetDefaultValue("0");

  lMessenger->DeclareProperty("SetPressure", fP1)
      .SetGuidance("Pressure in the second chamber")
      .SetParameterName("fP1", false)
      .SetDefaultValue("1");

  G4cout << "\n\n Lasser focus offset = " << fXof << G4endl;

  particleGun = new G4ParticleGun(1);
  particleSource = new G4GeneralParticleSource();

  // G4cout << "StatusGun = " << StatusGunParticle << G4endl;

  // Neural network: create onnx session
  Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "plasma");
  Ort::SessionOptions session_options;
  session_options.SetIntraOpNumThreads(1);
  auto sessionLocal =
      std::make_unique<Ort::Session>(env, "model2.onnx", session_options);
  fSession = std::move(sessionLocal);

  // Get input node information
  fMemory_info = Ort::MemoryInfo::CreateCpu(
      OrtAllocatorType::OrtArenaAllocator, OrtMemTypeDefault);
}

PALLAS_CollSimPrimaryGeneratorAction::~PALLAS_CollSimPrimaryGeneratorAction()
{
  delete particleGun;
  delete particleSource;
  delete pMessenger;
  delete lMessenger;
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

void PALLAS_CollSimPrimaryGeneratorAction::CalculProgress()
{
  if (flag_MT == true)
  {
    threadID = G4Threading::G4GetThreadId();
    if (threadID == 0)
    {
      ShowProgress(double(numThreads * currentParticleNumber) / double(NEventsGenerated), startTime);
    }
  }

  else
  {
    ShowProgress(double(currentParticleNumber) / double(NEventsGenerated), startTime);
  }
}

void PALLAS_CollSimPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
{
  if (!isStartTimeInitialized)
  {
    startTime = std::chrono::high_resolution_clock::now();
    isStartTimeInitialized = true;
  }

  xOffset = -0.152;                             // mm
  sOffset = 2884.5;
  //sOffset = 3114.5 - YParticleGenerationOffset; // mm
  zOffset = 0.0;                               // mm

  SetParticleName();

  if (StatusGunParticle == true)
  {

    // ######################################################################################
    // ######################## CASE 1 : INPUT FILE WITH MT ON ##############################
    // ######################################################################################

    if (flag_FileReader == true && flag_MT == true)
    {

      if (threadEventQueue[threadID].empty())
      {
        G4cerr << "Thread event queue is empty or not initialized!" << G4endl;
        G4RunManager::GetRunManager()->TerminateEventLoop();
        G4RunManager::GetRunManager()->AbortRun();

        G4cout << "STOP THREAD" << G4endl;
        return;
      }

      else
      {
      threadID = G4Threading::G4GetThreadId();

      // Extraire les données d'événements de la file d'attente
      ParticleData pdata = threadEventQueue[threadID].front();
      // G4cout << "Thread Event queue " << threadID << "  size = " << pdata.size() << G4endl;
      threadEventQueue[threadID].pop();

      G4double yp = sqrt(1 / (pdata.xp * pdata.xp + pdata.zp * pdata.zp + 1));

      particleGun->SetParticleDefinition(particleDefinition);
      particleGun->SetParticleEnergy(EnergyReference * (1 + pdata.delta));
      particleGun->SetParticlePosition(G4ThreeVector((pdata.x * 1000 + xOffset) * mm, (pdata.s * 1000 + sOffset) * mm, (pdata.z * 1000 + zOffset) * mm));
      particleGun->SetParticleMomentumDirection(G4ThreeVector(pdata.xp, yp, pdata.zp));

      for (int i = 0; i < pdata.n; i++)
      {
        particleGun->GeneratePrimaryVertex(anEvent);
      }

      currentParticleNumber += pdata.n;
      }
      
    }

    // ######################################################################################
    // ######################## CASE 2 : INPUT FILE WITH MT OFF #############################
    // ######################################################################################

    if (flag_FileReader == true && flag_MT == false)
    {
      if (threadEventQueue[0].empty())
      {
        G4cerr << "Thread event queue is empty or not initialized!" << G4endl;
        G4RunManager::GetRunManager()->TerminateEventLoop();
        G4RunManager::GetRunManager()->AbortRun();

        G4cout << "STOP Simulation" << G4endl;
        return;
      }

      else
      {
      // Extraire les données d'événements de la file d'attente
      ParticleData pdata = threadEventQueue[0].front();
      threadEventQueue[0].pop();

      G4double yp = sqrt(1 / (pdata.xp * pdata.xp + pdata.zp * pdata.zp + 1));

      particleGun->SetParticleDefinition(particleDefinition);
      particleGun->SetParticleEnergy(EnergyReference * (1 + pdata.delta));
      particleGun->SetParticlePosition(G4ThreeVector((pdata.x * 1000 + xOffset) * mm, (pdata.s * 1000 + sOffset) * mm, (pdata.z * 1000 + zOffset) * mm));
      particleGun->SetParticleMomentumDirection(G4ThreeVector(pdata.xp, yp, pdata.zp));

      for (int i = 0; i < pdata.n; i++)
      {
        particleGun->GeneratePrimaryVertex(anEvent);
      }


      currentParticleNumber += pdata.n;
      }
    }

    // ######################################################################################
    // ####################### CASE 3 : GENERATION FROM ONNX MODEL ##########################
    // ######################################################################################

    if (flag_FileReader == false && StatusONNX == true)
    {
      // Neural network: inference
      // Prepare input tensor values based on the provided numpy array
      G4double fXof_min = -399.8247;
      G4double fXof_max = 1798.325;
      G4double fXof_rescaled = fXof / (fXof_max - fXof_min) - fXof_min / (fXof_max - fXof_min);

      G4double fA0_min = 1.100516;
      G4double fA0_max = 1.849792;
      G4double fA0_rescaled = fA0 / (fA0_max - fA0_min) - fA0_min / (fA0_max - fA0_min);

      G4double fCN2_min = 0.002064164;
      G4double fCN2_max = 0.1199827;
      G4double fCN2_rescaled = fCN2 / (fCN2_max - fCN2_min) - fCN2_min / (fCN2_max - fCN2_min);

      G4double fP1_min = 10.09451;
      G4double fP1_max = 99.95741;
      G4double fP1_rescaled = fP1 / (fP1_max - fP1_min) - fP1_min / (fP1_max - fP1_min);

      G4cout << "Xof rescaled = " << fXof_rescaled << G4endl;
      G4cout << "A0 rescaled = " << fA0_rescaled << G4endl;
      G4cout << "CN2 rescaled = " << fCN2_rescaled << G4endl;
      G4cout << "P1 rescaled = " << fP1_rescaled << G4endl;

      std::vector<G4double> input_tensor_values =
          {fXof_rescaled, fA0_rescaled, fCN2_rescaled, fP1_rescaled};
      //{fP1_rescaled, fA0_rescaled, fXof_rescaled, fCN2_rescaled};
      //{0.90003744, 0.62109482, 0.49487012, 0.0943088};

      G4cout << "Input tensor values:" << G4endl;
      G4cout << "{" << fXof << " ; " << fA0 << " ; " << fCN2 << " ; " << fP1 << "}" << G4endl;

      std::vector<int64_t> input_node_dims_64 =
          {1, (unsigned)(input_tensor_values.size())};

      Ort::Value input_tensor =
          Ort::Value::CreateTensor<G4double>(fMemory_info,
                                             input_tensor_values.data(),
                                             input_tensor_values.size(),
                                             input_node_dims_64.data(),
                                             input_node_dims_64.size());

      std::vector<Ort::Value> ort_inputs;
      ort_inputs.push_back(std::move(input_tensor));

      std::vector<Ort::Value> output_tensors;
      // run the NN in inference mode
      output_tensors = fSession->Run(Ort::RunOptions{nullptr},
                                     fInput_node_names.data(),
                                     ort_inputs.data(),
                                     ort_inputs.size(),
                                     fOutput_node_names.data(),
                                     fOutput_node_names.size());

      G4cout << "Output tensor values:" << G4endl;

      // be careful with the types of variables!!! float->double -> incorrect answer!
      // the types must be compatible with the types set up in python
      G4float *out_vals = output_tensors.front().GetTensorMutableData<G4float>();

      // cycle to print the output of the NN (to be deleted in the future)
      for (size_t i = 0; i < 4; ++i)
      {
        G4cout << out_vals[i] << " ";
      }
      G4cout << G4endl;

      // To add convertion of NN output into real units!
      // the G4float value is given to double, seems working

      // kinetic energy
      G4double Ekin_min = 58.49625 * MeV;
      G4double Ekin_max = 725.2073 * MeV;
      G4double Ekin = out_vals[0] * (Ekin_max - Ekin_min) + Ekin_min;
      G4cout << "Ekin = " << Ekin << " MeV" << G4endl;

      // spread of kinetic energy
      G4double dEkin_min = 0.000826999;
      G4double dEkin_max = 0.7512749;
      G4double dEkin = out_vals[1] * (dEkin_max - dEkin_min) + dEkin_min;
      G4cout << "dEkin = " << 100 * dEkin << " %" << G4endl;

      G4double Q_min = 6.50e-17;
      G4double Q_max = 9.69e-10;
      G4double Q = out_vals[2] * (Q_max - Q_min) + Q_min;
      G4cout << "Q = " << Q << " C" << G4endl;

      // beam emittance (by now it's the same for x and y)
      G4double epsb_min = 2.50e-9;
      G4double epsb_max = 8.08e-5;
      G4double epsb = out_vals[3] * (epsb_max - epsb_min) + epsb_min;
      G4cout << "beam emittance = " << epsb << " m" << G4endl;

      // gaussian with Ekin mean and dEking standard deviation
      Ekin = G4RandGauss::shoot(Ekin, dEkin * Ekin);
      G4cout << "Ekin from random shoot = " << Ekin << G4endl;

      // set Twiss parameters
      // to check!
      G4double alphax = 0;
      G4double betax = 1;
      G4double alphay = 0;
      G4double betay = 1;

      // generate the beam distribution
      //(gaussian by hands taking into account betatron oscillations)

      // amplitude of betatron oscillations
      G4double Ax = std::sqrt(epsb * betax) * std::sqrt(-2 * std::log(G4UniformRand()));
      G4cout << "Amplitude betatron osc X = " << Ax << G4endl;
      // phase
      G4double psix = CLHEP::twopi * G4UniformRand();
      G4cout << "phase X = " << psix << G4endl;
      // generating coordinate and angle
      G4double x = Ax * std::cos(psix);
      G4double xp = -x * alphax / betax - Ax / std::sqrt(betax) * std::sin(psix);
      G4cout << "x = " << x << G4endl;
      G4cout << "xp = " << xp << G4endl;

      // amplitude of betatron oscillations
      G4double Ay = std::sqrt(epsb * betay) * std::sqrt(-2 * std::log(G4UniformRand()));
      G4cout << "Amplitude betatron osc Y = " << Ay << G4endl;
      // phase
      G4double psiy = CLHEP::twopi * G4UniformRand();
      G4cout << "phase X = " << psiy << G4endl;
      // generating coordinate and angle
      G4double y = Ay * std::cos(psiy);
      G4double yp = -y * alphay / betay - Ay / std::sqrt(betay) * std::sin(psiy);
      G4cout << "y = " << y << G4endl;
      G4cout << "yp = " << yp << G4endl;

      // set particle parameters
      particleGun->SetParticleDefinition(particleDefinition);
      particleGun->SetParticleEnergy(Ekin);
      particleGun->SetParticlePosition(G4ThreeVector(xOffset, sOffset, zOffset));

      G4double MomentumDirectionY = 1. / (1. + std::pow(std::tan(xp), 2) + std::pow(std::tan(yp), 2));
      G4cout << "Momentum Y = " << MomentumDirectionY << G4endl;
      particleGun->SetParticleMomentumDirection(G4ThreeVector(MomentumDirectionY * std::tan(xp), MomentumDirectionY, MomentumDirectionY * tan(yp)));

      particleGun->GeneratePrimaryVertex(anEvent);

      currentParticleNumber++;
    }

    // ######################################################################################
    // ####################### CASE 4 : GENERATION FROM PARTICLE GUN ########################
    // ######################################################################################

    if (flag_FileReader == false && StatusONNX == false)
    {
      // set particle parameters -> Example
      particleGun->SetParticleDefinition(particleDefinition);
      particleGun->SetParticleEnergy(247 * MeV);
      particleGun->SetParticlePosition(G4ThreeVector(xOffset, sOffset, zOffset));
      particleGun->SetParticleMomentumDirection(G4ThreeVector(0, 1, 0));

      particleGun->GeneratePrimaryVertex(anEvent);

      currentParticleNumber++;
    }
  }

  
  // ######################################################################################
  // ############################ CASE 5 : GENERATION FROM GPS ############################
  // ######################################################################################

  else
  {
    particleSource->GeneratePrimaryVertex(anEvent);
    currentParticleNumber++;
  }

  CalculProgress();
}
