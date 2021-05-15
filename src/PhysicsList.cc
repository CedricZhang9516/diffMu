
#include "PhysicsList.hh"

#include "ExtraPhysics.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

//#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4LossTableManager.hh"
#include "G4ProductionCutsTable.hh"

#include "G4DecayPhysics.hh"
#include "G4ProcessTable.hh"

#include "G4PionDecayMakeSpin.hh"
#include "G4Decay.hh"
#include "G4DecayWithSpin.hh"

#include "G4DecayTable.hh"
#include "G4MuonDecayChannelWithSpin.hh"
#include "G4MuonRadiativeDecayChannelWithSpin.hh"

#include "MuZero.hh"
#include "MuZeroDecayChannel.hh"
#include "MuZeroDecayChannelWithSpin.hh"

// For Mu formation
#include "MuZeroFormation.hh" // uses proton cross section for Ar.

// For Mu ionization
#include "MuZeroIonization.hh" // uses proton cross section for Ar.

// For Mu elastic scattering
#include "MuZeroElasticScatter.hh"  // uses cross section from Senba for Ar.

// For Mu diffusion
#include "MuZeroDiffusion.hh"  // parametrized by temperature, diffusion constant

#include "G4UserSpecialCuts.hh"


PhysicsList::PhysicsList()
  :G4VModularPhysicsList() 
{
  RegisterPhysics(new G4DecayPhysics());
  RegisterPhysics(new G4EmStandardPhysics_option4(1));
  RegisterPhysics(new ExtraPhysics());
  G4LossTableManager::Instance();
  defaultCutValue = 100.0*CLHEP::micrometer;
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;
  cutForMuonMinus  = defaultCutValue;
  cutForMuonPlus  = 0.01*defaultCutValue;
  cutForMuonium  = 0.01*defaultCutValue;
  G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(25*CLHEP::eV, 100*CLHEP::MeV);
 
}

PhysicsList::~PhysicsList()
{;}

void PhysicsList::ConstructParticle()
{
  G4VModularPhysicsList::ConstructParticle();

  G4DecayTable* MuonPlusDecayTable = new G4DecayTable();
  MuonPlusDecayTable -> Insert(new
			       G4MuonDecayChannelWithSpin("mu+",0.986));
  MuonPlusDecayTable -> Insert(new
			       G4MuonRadiativeDecayChannelWithSpin("mu+",0.014));
  G4MuonPlus::MuonPlusDefinition() -> SetDecayTable(MuonPlusDecayTable);
  MuonPlusDecayTable->DumpInfo();

  G4DecayTable* MuonMinusDecayTable = new G4DecayTable();
  MuonMinusDecayTable -> Insert(new
				G4MuonDecayChannelWithSpin("mu-",0.986));
  MuonMinusDecayTable -> Insert(new
				G4MuonRadiativeDecayChannelWithSpin("mu-",0.014));
  G4MuonMinus::MuonMinusDefinition() -> SetDecayTable(MuonMinusDecayTable);
  //MuonMinusDecayTable->DumpInfo();

  muonium = MuZero::MuoniumDefinition();
  
}

void PhysicsList::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();

  //AddTransportation();  // not needed here

  G4DecayWithSpin* decayWithSpin = new G4DecayWithSpin();

  G4ProcessTable* processTable = G4ProcessTable::GetProcessTable();

  G4VProcess* decay;
  G4ProcessManager* fManager;

  decay = processTable->FindProcess("Decay",G4MuonPlus::MuonPlus());
  fManager = G4MuonPlus::MuonPlus()->GetProcessManager();
  if (fManager) {
    // Set user limits
    fManager->AddProcess(new G4UserSpecialCuts(),-1,-1,1);
    if (decay) fManager->RemoveProcess(decay);
    fManager->AddProcess(decayWithSpin);
    // set ordering for PostStepDoIt and AtRestDoIt
    fManager->SetProcessOrdering(decayWithSpin, idxPostStep);
    fManager->SetProcessOrdering(decayWithSpin, idxAtRest);
    //   *** Mu formation process added here
    fManager->AddProcess(new MuZeroFormation, -1, -1, 2);
  }
  //processTable->DumpInfo(processTable->FindProcess("Decay",G4MuonPlus::MuonPlus()));
  //processTable->DumpInfo(processTable->FindProcess("DecayWithSpin",G4MuonPlus::MuonPlus()));

  decay = processTable->FindProcess("Decay",G4MuonMinus::MuonMinus());
  fManager = G4MuonMinus::MuonMinus()->GetProcessManager();
  if (fManager) {
    if (decay) fManager->RemoveProcess(decay);
    fManager->AddProcess(decayWithSpin);
    // set ordering for PostStepDoIt and AtRestDoIt
    fManager->SetProcessOrdering(decayWithSpin, idxPostStep);
    fManager->SetProcessOrdering(decayWithSpin, idxAtRest);
  }

  fManager = muonium->GetProcessManager();
  if (fManager) {
    // Set user limits
    fManager->AddProcess(new G4UserSpecialCuts(),-1,-1,1);
    //Mu diffusion process added here
    G4VProcess* aMuDiffusion = new MuZeroDiffusion();
    fManager->AddProcess(aMuDiffusion);
    fManager->SetProcessOrdering(aMuDiffusion, idxPostStep, 1);

    //Mu ionization process added here
    G4VProcess* aMuIonize = new MuZeroIonization();
    fManager ->AddProcess(aMuIonize);
    fManager ->SetProcessOrdering(aMuIonize, idxPostStep, 2);

    //Mu scattering and "thermalization" process added here
    G4VProcess* aMuElastScatt = new MuZeroElasticScatter();
    fManager ->AddProcess(aMuElastScatt);
    fManager ->SetProcessOrdering(aMuElastScatt, idxPostStep, 3);

  }

  G4PionDecayMakeSpin* pionpoldecay = new G4PionDecayMakeSpin();

  decay = processTable->FindProcess("Decay",G4PionPlus::PionPlus());
  fManager = G4PionPlus::PionPlus()->GetProcessManager();
  if (fManager) {
    if (decay) fManager->RemoveProcess(decay);
    fManager->AddProcess(pionpoldecay);
    // set ordering for PostStepDoIt and AtRestDoIt
    fManager ->SetProcessOrdering(pionpoldecay, idxPostStep);
    fManager ->SetProcessOrdering(pionpoldecay, idxAtRest);
  }

  decay = processTable->FindProcess("Decay",G4PionMinus::PionMinus());
  fManager = G4PionMinus::PionMinus()->GetProcessManager();
  if (fManager) {
    if (decay) fManager->RemoveProcess(decay);
    fManager->AddProcess(pionpoldecay);
    // set ordering for PostStepDoIt and AtRestDoIt
    fManager ->SetProcessOrdering(pionpoldecay, idxPostStep);
    fManager ->SetProcessOrdering(pionpoldecay, idxAtRest);
  }

}

void PhysicsList::SetCuts()
{
  // SetCutsWithDefault();
  if (verboseLevel >0){
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }

  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  SetParticleCuts(cutForGamma, G4Gamma::Gamma());
  SetParticleCuts(cutForElectron, G4Electron::Electron());
  SetParticleCuts(cutForPositron, G4Positron::Positron());
  SetParticleCuts(cutForMuonMinus, G4MuonMinus::MuonMinus());
  SetParticleCuts(cutForMuonPlus, G4MuonPlus::MuonPlus());
  SetParticleCuts(cutForMuonium, MuZero::Muonium());
}
