/// MuZeroElasticScatter.hh
/// model for charge-changing collisions (1->0) for muonplus to muoniium
//

#ifndef MuZeroElasticScatter_h
#define MuZeroElasticScatter_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ParticleTable.hh"

class MuZeroElasticScatter : public G4VDiscreteProcess 
{
public:


  MuZeroElasticScatter(const G4String& processName ="MuZeroElasticScatter",
		       G4ProcessType aType = fElectromagnetic);

  virtual ~MuZeroElasticScatter();

  virtual G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step& );
  virtual G4bool IsApplicable(const G4ParticleDefinition&);

  G4ParticleTable* particleTable; 
  G4ParticleDefinition* particle;
  G4DynamicParticle *DP;
  G4Track* aSecondary;
                           
protected:

  virtual G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition* );
                           
private:
  G4double FindCrossSection(G4double ekin);
  G4double FindMFP(G4double xsection, G4double density);

  G4double logEk, logxs;
  // hide initialization of doubles inside function
  // Eloss fraction = 1. - 2.*m_Mu*m_tgt/(m_Mu + m_tgt)*2, for m_tgt = 60. amu
  static G4double GetElossFraction() {return 0.9963; }
  static G4double GetLogEevxslow() {return 0.; }  // down to 1 eV
  static G4double GetLogEevxshigh() {return 5.5; }
  static G4double Getxcpar(G4int index) {
    static const G4double xcpar[]
      = { 0.8153, -0.33406 };  // fit to Ar sigma00 elastic from Senba 
    return xcpar[index];
  }
  

};

#endif
