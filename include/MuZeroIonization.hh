/// MuZeroIonization.hh
/// model for charge-changing collisions (0->1) for muonium to muonplus
//


#ifndef MuZeroIonization_h
#define MuZeroIonization_h 1
#define LON 0

#include "G4ios.hh"
#include "globals.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ParticleTable.hh"

class MuZeroIonization : public G4VDiscreteProcess 
{
public:


  MuZeroIonization(const G4String& processName ="MuZeroIonization",
		   G4ProcessType aType = fElectromagnetic);

  virtual ~MuZeroIonization();

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
  // hide initialization of doubles inside function
  static G4double GetLogEevxslow() {return 2.0 ; }
  static G4double GetLogEevxshigh() {return 4.5 ; }
  static G4double Getxcpar(G4int index) {
    static const G4double xcpar[]
      = {-2.2822,  0.60035,  3.8729,  2.0031,  0.13332};  // fit to Ar sigma01 wide from Tawara et al. 
    return xcpar[index];
  }
  

};

#endif
