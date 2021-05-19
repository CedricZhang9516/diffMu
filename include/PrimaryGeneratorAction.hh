//
// from B1PrimaryGeneratorAction.hh
// 20180227: from tgtdiffMu, removing G4ParticleGun and using G4 gps

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4ParticleGun;
class G4Event;
class HistoManager;

/// The primary generator action class with particle gun.

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction( HistoManager* );
  virtual ~PrimaryGeneratorAction();

  // method from the base class
  virtual void GeneratePrimaries(G4Event*);

  //  // method to access particle gun
  //  const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

private:
  //G4GeneralParticleSource* fParticleGun;
  G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
  HistoManager* fHistoManager;

  //  // distribution in this volume
  //  const G4double MuInitSizeXY;
  //  const G4double MuInitSizeZ;
  //  const G4double MuInitOffsZ;
  //  // position in this volume
  //  G4double x0;
  //  G4double y0;
  //  G4double z0;
  //
  //  G4double SpeedSq;
  //  G4double EkMu;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
