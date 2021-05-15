/// MuZeroDiffusion.hh
/// model for diffusion of muoniium
//

#ifndef MuZeroDiffusion_h
#define MuZeroDiffusion_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VDiscreteProcess.hh"
#include "G4ParticleTable.hh"

class MuZeroDiffusion : public G4VDiscreteProcess 
{
public:

  MuZeroDiffusion(const G4String& processName ="MuZeroDiffusion",
                  G4ProcessType aType = fElectromagnetic);

  virtual ~MuZeroDiffusion();

  virtual G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step& );
  //  virtual G4VParticleChange* AlongStepDoIt(const G4Track&, const G4Step& );
  virtual G4bool IsApplicable(const G4ParticleDefinition&);

  G4ParticleTable* particleTable; 
  G4ParticleDefinition* particle;
  G4Track* aSecondary;

protected:

  virtual G4double GetMeanFreePath(const G4Track&, G4double, G4ForceCondition* );

private:

  static G4double MaxwellSpeedNorm();
  // parameters for diffusion are stored in inline functions
  static G4double GetMuMass() {     // in MeV/c^2
    return G4ParticleTable::GetParticleTable()->FindParticle("Mu")->GetPDGMass()*CLHEP::MeV;
  }
  static G4double GetTempK() {return 300. * CLHEP::kelvin; }
  static G4double GetDiffCon() {return 10. * CLHEP::cm2 / CLHEP::second ; }
  static G4double GetMuMeanSpeedSq() {
    return 8. * CLHEP::k_Boltzmann * GetTempK() / ( CLHEP::pi * GetMuMass() / CLHEP::c_squared ) ;
  }
  static G4double GetMuMPSpeedSq()   {
    return 2. * CLHEP::k_Boltzmann * GetTempK() / ( GetMuMass() / CLHEP::c_squared ) ;
  }
  static G4double GetMuMeanSpeed()   {return std::sqrt( GetMuMeanSpeedSq() ) ; }
  // coefficient of 3 -> 12/pi for convolution of exponential mfp with Maxwell speed distributions
  static G4double GetDiffFreePath()  {
    return ( (12. * GetDiffCon()/(CLHEP::cm2/CLHEP::second) )  /
	     ( CLHEP::pi * GetMuMeanSpeed()/(CLHEP::cm/CLHEP::second) ) ) ;  // diff mfp in G4 units
  }
  static G4double GetHighELimit() {return 1.0 * CLHEP::eV; }
  // test this
  //static G4double GetHighELimit() {return 1000. * CLHEP::eV; }

};

#endif
