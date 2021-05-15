/// MuZeroFormation.hh
/// model for charge-changing collisions (1->0) for muonplus to muoniium
//

#ifndef MuZeroFormation_h
#define MuZeroFormation_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ParticleTable.hh"

class MuZeroFormation : public G4VDiscreteProcess 
{
public:


  MuZeroFormation(const G4String& processName ="MuZeroFormation",
                  G4ProcessType aType = fElectromagnetic);

  virtual ~MuZeroFormation();

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
  //static G4double pMuMassRatio;
  // hide initialization of doubles inside functions
  static G4double GetLogEevxslow() {return 2.0; }
  static G4double GetLogEevxshigh() {return 4.5; }
  static G4double Getxcpar(G4int index) {
    static const G4double xcpar[]
      = { 1.3545, -0.27664, -0.0033333,  0.37552 };  // fit to Ar sigma10 lowE from Tawara et al. 
    return xcpar[index];
  }
  

};

#endif
