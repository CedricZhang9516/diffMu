
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "globals.hh"
#include "G4VModularPhysicsList.hh"

class MuZero;

class G4VPhysicsConstructor;

class PhysicsList: public G4VModularPhysicsList
{
public:

  PhysicsList();
  virtual ~PhysicsList();

  void ConstructParticle();
  void ConstructProcess();

  void SetCuts();

private:

  G4double defaultCutValue;
  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
  G4double cutForMuonMinus;
  G4double cutForMuonPlus;
  G4double cutForMuonium;

  MuZero* muonium;
};

#endif
