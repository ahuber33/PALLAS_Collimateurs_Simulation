/// PALLAS_CollSimActionInitialization.hh
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#ifndef PALLAS_CollSimActionInitialization_h
#define PALLAS_CollSimActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"
#include "ParticleData.hh"

class PALLAS_CollSimGeometryConstruction;
class PALLAS_CollSimPrimaryGeneratorAction;

class PALLAS_CollSimActionInitialization : public G4VUserActionInitialization
{
public:
  PALLAS_CollSimActionInitialization(const char*, size_t, size_t, std::vector<std::queue<ParticleData>>, const std::vector<ParticleData>&, bool, bool);
  virtual ~PALLAS_CollSimActionInitialization();
  size_t charToSizeT(G4String str);

  virtual void BuildForMaster() const;
  virtual void Build() const;
  char* NEvents;
    G4String suffixe;
    size_t NEventsGenerated;
    size_t numThreads;
    G4bool flag_FileReader=false;
    G4bool flag_MT=false;
    std::vector<ParticleData> fParticleData;
    std::vector<std::queue<ParticleData>> threadEventQueues;
};

#endif