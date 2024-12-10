/// PALLAS_CollSimPhysics.cc
//// Auteur: Arnaud HUBER for ENL group <huber@lp2ib.in2p3.fr>
//// Copyright: 2024 (C) Projet PALLAS

#include "PALLAS_CollSimPhysics.hh"

using namespace CLHEP;

// Taken from N06 and LXe examples in GEANT4

PALLAS_CollSimPhysics::PALLAS_CollSimPhysics()// : G4VModularPhysicsList()
{
  // // Here used the default cut value you have typed in
  // defaultCutValue = 1 * mm; // 0.001

  // SetVerboseLevel(1);

  // // default physics
  // particleList = new G4DecayPhysics();

  // // default physics
  // raddecayList = new G4RadioactiveDecayPhysics();

  // // EM physics
  // emPhysicsList = new G4EmStandardPhysics_option3();
  // //emPhysicsList = new G4EmStandardPhysics();

  // // RegisterPhysics( new G4HadronElasticPhysicsHP(verb));
  // RegisterPhysics( new G4HadronElasticPhysicsXS(1));  

  // // Hadron Inelastic physics
  // //
  // ////RegisterPhysics( new G4HadronPhysicsFTFP_BERT_HP(verb));
  // RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP(1));





  G4int verb = 1;
  SetVerboseLevel(verb);
  
  // mandatory for G4NuclideTable
  //
  const G4double meanLife = 1*nanosecond, halfLife = meanLife*std::log(2);
  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(halfLife);
     
  // Hadron Elastic scattering
  //RegisterPhysics( new G4HadronElasticPhysicsXS(verb) );  
  RegisterPhysics( new G4HadronElasticPhysicsHP(verb) );
  
  // Hadron Inelastic Physics
  //RegisterPhysics( new G4HadronPhysicsFTFP_BERT(verb));
  ////RegisterPhysics( new G4HadronPhysicsQGSP_BIC(verb));
  RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP(verb));
  ////RegisterPhysics( new G4HadronPhysicsQGSP_BIC_AllHP(verb));
  ////RegisterPhysics( new G4HadronInelasticQBBC(verb));
  ////RegisterPhysics( new G4HadronPhysicsINCLXX(verb));
  
  // Ion Elastic scattering
  //
  RegisterPhysics( new G4IonElasticPhysics(verb));
  
  // Ion Inelastic physics
  RegisterPhysics( new G4IonPhysicsXS(verb));
  ////RegisterPhysics( new G4IonINCLXXPhysics(verb));
  
  // stopping Particles
  RegisterPhysics( new G4StoppingPhysics(verb));
      
  // Gamma-Nuclear Physics
  //RegisterPhysics( new GammaNuclearPhysics("gamma"));
  ////RegisterPhysics( new GammaNuclearPhysicsLEND("gamma"));
  RegisterPhysics( new G4EmExtraPhysics());
      
  // EM physics
  //RegisterPhysics(new ElectromagneticPhysics());
  RegisterPhysics(new G4EmStandardPhysics_option3());
  
  // Decay
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  //RegisterPhysics(new RadioactiveDecayPhysics());
  RegisterPhysics(new G4RadioactiveDecayPhysics());  


}

PALLAS_CollSimPhysics::~PALLAS_CollSimPhysics()
{
  // delete raddecayList;
  // delete emPhysicsList;
  // delete particleList;
  //delete fHadronPhys;
}

// void PALLAS_CollSimPhysics::ConstructParticle()
// {
//   // Here are constructed all particles you have chosen
//   particleList->ConstructParticle();
//   //  ions
//   // G4IonConstructor iConstructor;
//   // iConstructor.ConstructParticle();
// }

// void PALLAS_CollSimPhysics::ConstructProcess()
// {
//   // Transportation, electromagnetic and general processes
//   AddTransportation();
//   //  Electromagnetic physics list
//   emPhysicsList->ConstructProcess();
//   particleList->ConstructProcess();
//   raddecayList->ConstructProcess();
//   fHadronPhys->ConstructProcess();
  
//   //ConstructNeutron();
  


//     G4HadronInelasticProcess* process
//   = new G4HadronInelasticProcess("photonNuclear", G4Gamma::Definition());
//   process->AddDataSet( new G4PhotoNuclearCrossSection );

//   // to not register a model, set Emax=0; eg. Emax1 = 0.
//   const G4double Emax1 = 200*MeV, Emax2 = 0*GeV;//10*GeV

//   if (Emax1 > 0.) {  // model 1
//     G4LowEGammaNuclearModel* model1 = new G4LowEGammaNuclearModel();
//     model1->SetMaxEnergy(Emax1);
//     process->RegisterMe(model1);
//   }

//   if (Emax2 > 0.) {  // model 2
//     G4CascadeInterface* model2 = new G4CascadeInterface();
//     G4double Emin2 = std::max(Emax1-1*MeV, 0.);
//     model2->SetMinEnergy(Emin2);
//     model2->SetMaxEnergy(Emax2);
//     process->RegisterMe(model2);
//   }

//   G4ProcessManager* pManager = G4Gamma::Gamma()->GetProcessManager();
//   pManager->AddDiscreteProcess(process);

// }


// void PALLAS_CollSimPhysics::SetBuilderList0(G4bool flagHP)
// {
//   fHadronPhys.push_back( new G4EmExtraPhysics(verboseLevel));
//   if(flagHP) {
//     fHadronPhys.push_back( new G4HadronElasticPhysicsHP(verboseLevel) );
//   } else {
//     fHadronPhys.push_back( new G4HadronElasticPhysics(verboseLevel) );
//   }
//   fHadronPhys.push_back( new G4StoppingPhysics(verboseLevel));
//   fHadronPhys.push_back( new G4IonPhysics(verboseLevel));
//   if(!flagHP) {
//     fHadronPhys.push_back( new G4NeutronTrackingCut(verboseLevel));
//   }
// }


void PALLAS_CollSimPhysics::ConstructNeutron()
{
  G4ParticleDefinition* neutron = G4Neutron::Neutron();
  G4ProcessManager* pManager = neutron->GetProcessManager();

  // delete all neutron processes if already registered
  //
  G4VProcess* process = 0;
  process = pManager->GetProcess("hadElastic");
  if (process) pManager->RemoveProcess(process);
  //
  // process = pManager->GetProcess("neutronInelastic");
  // if (process) pManager->RemoveProcess(process);
  // //
  // process = pManager->GetProcess("nCapture");
  // if (process) pManager->RemoveProcess(process);
  // //
  // process = pManager->GetProcess("nFission");
  // if (process) pManager->RemoveProcess(process);

  // (re) create process: elastic
  //
  G4HadronElasticProcess* process1 = new G4HadronElasticProcess();
  pManager->AddDiscreteProcess(process1);
  //
  // model1a
  G4ParticleHPElastic*  model1a = new G4ParticleHPElastic();
  process1->RegisterMe(model1a);
  process1->AddDataSet(new G4ParticleHPElasticData());
  //
  // model1b
  if (fThermal) {
    model1a->SetMinEnergy(4*eV);
    G4ParticleHPThermalScattering* model1b = new G4ParticleHPThermalScattering();
    process1->RegisterMe(model1b);
    process1->AddDataSet(new G4ParticleHPThermalScatteringData());
  }

  // (re) create process: inelastic
  //
  // G4HadronInelasticProcess* process2 =
  // new G4HadronInelasticProcess( "neutronInelastic", G4Neutron::Definition() );
  // pManager->AddDiscreteProcess(process2);
  // //
  // // cross section data set
  // G4ParticleHPInelasticData* dataSet2 = new G4ParticleHPInelasticData();
  // process2->AddDataSet(dataSet2);
  // //
  // // models
  // G4ParticleHPInelastic* model2 = new G4ParticleHPInelastic();
  // process2->RegisterMe(model2);

  // // (re) create process: nCapture
  // //
  // G4NeutronCaptureProcess* process3 = new G4NeutronCaptureProcess();
  // pManager->AddDiscreteProcess(process3);
  // //
  // // cross section data set
  // G4ParticleHPCaptureData* dataSet3 = new G4ParticleHPCaptureData();
  // process3->AddDataSet(dataSet3);
  // //
  // // models
  // G4ParticleHPCapture* model3 = new G4ParticleHPCapture();
  // process3->RegisterMe(model3);

  // // (re) create process: nFission
  // //
  // G4NeutronFissionProcess* process4 = new G4NeutronFissionProcess();
  // pManager->AddDiscreteProcess(process4);
  // //
  // // cross section data set
  // G4ParticleHPFissionData* dataSet4 = new G4ParticleHPFissionData();
  // process4->AddDataSet(dataSet4);
  // //
  // // models
  // G4ParticleHPFission* model4 = new G4ParticleHPFission();
  // process4->RegisterMe(model4);
}

void PALLAS_CollSimPhysics::SetCuts()
{
  if (verboseLevel > 1)
  {
    G4cout << "opticalPhysics::SetCuts:";
  }
  SetCutsWithDefault();
  //SetCutValue(0.0*mm, "gamma");
  //  SetCutValue(5*mm,"opticalphoton");
}
