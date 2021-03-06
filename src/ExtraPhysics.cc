// G4v10 - replace "theParticleIterator" by "aParticleIterator"
// G4v10.3 - and now GetParticleIterator

#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessManager.hh"

#include "G4UserSpecialCuts.hh"
#include "G4StepLimiter.hh"

#include "ExtraPhysics.hh"

ExtraPhysics::ExtraPhysics() 
  : G4VPhysicsConstructor("Extra") {;}

ExtraPhysics::~ExtraPhysics() {;}

void ExtraPhysics::ConstructParticle() {;}

void ExtraPhysics::ConstructProcess()
{
  GetParticleIterator()->reset();

  while ((*GetParticleIterator())()) {
    G4ParticleDefinition* particle = GetParticleIterator()->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    G4double charge = particle->GetPDGCharge();

    if (!pmanager) {
      std::ostringstream o;
      o << "Particle " << particleName << "without a Process Manager";
      G4Exception("ExtraPhysics::ConstructProcess()","",
		  FatalException,o.str().c_str());
    }

    if (charge != 0.0) {
      // All charged particles should have a step limiter
      // to make sure that the steps do not get too long.
      pmanager->AddDiscreteProcess(new G4StepLimiter());
      pmanager->AddDiscreteProcess(new G4UserSpecialCuts());
    } else if (particleName == "neutron") {
      // time cuts for ONLY neutrons:
      pmanager->AddDiscreteProcess(new G4UserSpecialCuts());
    } else {
      // Energy cuts for all other neutral particles
      pmanager->AddDiscreteProcess(new G4UserSpecialCuts());
    }
  }
}
