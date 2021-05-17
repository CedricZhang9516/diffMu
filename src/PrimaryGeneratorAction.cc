//
/// from B1PrimaryGeneratorAction.cc

#include "PrimaryGeneratorAction.hh"
#include "HistoManager.hh"
//#include "MuZeroDiffusion.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
//#include "G4RunManager.hh"
//#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

//#include "G4TransportationManager.hh"
//#include "G4Navigator.hh"

#include "G4SystemOfUnits.hh"
//#include "G4RandomTools.hh"
//#include "G4RandomDirection.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(HistoManager* histo)
  : G4VUserPrimaryGeneratorAction(),
    fHistoManager(histo)

{
  // use gps for beam
  fParticleGun = new G4GeneralParticleSource();

  // this commented section can be used for tests of diffusion in the aerogel
  // to start muonium diffusion in a restricted volume
  //
  //  G4int n_particle = 1;
  //  fParticleGun  = new G4ParticleGun(n_particle);
  //
  //  // default particle
  //  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  //  G4String particleName;
  //  G4ParticleDefinition* particle
  //    = particleTable->FindParticle(particleName="Mu");
  //  fParticleGun->SetParticleDefinition(particle);
  //
  //  // dimension and z placement of volume for particle generation
  //  //  const G4double MuInitSizeXY = 1.0*CLHEP::cm;
  //  //  const G4double MuInitSizeZ = 1.0*CLHEP::cm;
  //  //  const G4double MuInitOffsZ = -0.5*MuInitSizeZ;
  //  //    or
  //  // point source
  //  //const G4double MuInitSizeXY = 0.*CLHEP::cm;
  //  //const G4double MuInitSizeZ = 0.*CLHEP::cm;
  //  //const G4double MuInitOffsZ = -0.5*CLHEP::cm;
  //
  //  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,-0.5*CLHEP::cm));
  //  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  //  fParticleGun->SetParticleEnergy(0.025*CLHEP::eV);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of each event
  //
  fParticleGun->SetParticlePolarization(-(fParticleGun->GetParticleMomentumDirection()));
  fParticleGun->GeneratePrimaryVertex(anEvent);
  fHistoManager->IncEventsPrimaryAll();

  // this commented section can be used for tests of diffusion in the aerogel
  // to start muonium diffusion in a restricted volume
  //
  //  G4Navigator* theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  //
  //  // find position and make sure it is in volume of Aerogelxxxx
  //  while(true) {
  //    x0 = MuInitSizeXY * (G4UniformRand()-0.5);
  //    y0 = MuInitSizeXY * (G4UniformRand()-0.5);
  //    z0 = MuInitSizeZ * (G4UniformRand()-0.5) + MuInitOffsZ;
  //    G4VPhysicalVolume* myVolume = theNavigator->LocateGlobalPointAndSetup(G4ThreeVector(x0,y0,z0));
  //    G4String matname = myVolume->GetLogicalVolume()->GetMaterial()->GetName();
  //    //G4cout << "PrimaryGeneratorAction: matname " << matname << G4endl;
  //    fHistoManager->IncEventsPrimaryAll();
  //    if (matname.compare(0,7,"Aerogel") == 0) break;
  //    fHistoManager->IncEventsPrimaryFail();
  //  }
  //
  //  SpeedSq = MuZeroDiffusion::MuSpeedMPSq*MuZeroDiffusion::MaxwellSpeedNorm();
  //  EkMu = 0.5*MuZeroDiffusion::MuMass_c2*SpeedSq/CLHEP::c_squared;
  ////  G4cout << "PrimaryGeneratorAction GenPrim EkMu/eV, x,y,z/mm: "
  ////  	 << EkMu/( CLHEP::eV ) << ","
  //// 	 << x0/( CLHEP::mm ) << ","
  //// 	 << y0/( CLHEP::mm ) << ","
  //// 	 << z0/( CLHEP::mm ) <<  G4endl;
  //
  //  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  //  fParticleGun->SetParticleMomentumDirection(G4RandomDirection());
  //  fParticleGun->SetParticleEnergy(EkMu);
  //  fParticleGun->SetParticlePolarization(G4ThreeVector( 1.0, 0.0, 0.0 ));
  //  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

