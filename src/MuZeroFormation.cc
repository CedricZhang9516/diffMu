/// MuZeroFormation.cc
/// model for charge-changing collisions (1->0) for muonplus to muonium
//

#include "MuZeroFormation.hh"
#include "MuZero.hh"

#include "G4SystemOfUnits.hh"


// initialize static doubles outside of functions; changed to inline functions
//G4double MuZeroFormation::pMuMassRatio = 8.8802434;  // codata
//G4double MuZeroFormation::logEevxslow = 2.0;
//G4double MuZeroFormation::logEevxshigh = 4.5;
  

MuZeroFormation::MuZeroFormation(const G4String& aName, G4ProcessType aType)
  : G4VDiscreteProcess(aName, aType) 
{
  if (verboseLevel>1) {
    G4cout << GetProcessName() << " is created "<< G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


MuZeroFormation::~MuZeroFormation()
{;}


G4double MuZeroFormation::GetMeanFreePath( const G4Track& aTrack,
					   G4double /*previousStepSize*/,
					   G4ForceCondition* condition)
{
  G4String p_name = aTrack.GetDefinition()->GetParticleName();
  G4String lv_name = aTrack.GetVolume()->GetLogicalVolume()->GetName();
  G4String mat_name = aTrack.GetVolume()->GetLogicalVolume()->GetMaterial()->GetName();
  G4double mfp;
  if (p_name == "mu+") {
    if (mat_name.compare(0,7,"Aerogel") == 0) {  // muon should have MFP for formation
      G4double Ekmuon= aTrack.GetKineticEnergy();
      G4double lyr_density = aTrack.GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity();
      //Calculate mean free path
      //G4double EprotonEquiv = pMuMassRatio*E;
      G4double xsection = MuZeroFormation::FindCrossSection(Ekmuon);
      mfp =  MuZeroFormation::FindMFP(xsection, lyr_density);
      *condition = NotForced;
      if (verboseLevel>1) {
	G4cout << "\n++++++++ " << G4endl;
	//G4cout << "\nMuZeroFormation::GetMeanFreePath EprotonEquiv/CLHEP::ev "
	//       << EprotonEquiv/ev << G4endl;
	G4cout << "MuZeroFormation::GetMeanFreePath xsection[cm2], density[g/cm3]: "
	       << xsection/CLHEP::cm2 << ", " << lyr_density/(CLHEP::gram/CLHEP::cm3) << G4endl;
	G4cout << "MuZeroFormation::GetMeanFreePath Ekmuon[eV], FindMFP[mm]: "
	       << Ekmuon/CLHEP::eV << ", " << mfp/CLHEP::mm << G4endl;
 	G4cout << "++++++++ \n" << G4endl;
     }
    }
    else   {    // muon does not form Mu
      mfp = DBL_MAX;
      *condition = NotForced;
    }
  }
  else  {   // not Mu, so should not be affected; does this happen? IsApplicable should prevent
    mfp = DBL_MAX;
    *condition = NotForced;
  }
  return mfp;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


G4VParticleChange*
MuZeroFormation::PostStepDoIt( const G4Track& aTrack,
			       const G4Step& aStep)
{
  particleTable=G4ParticleTable::GetParticleTable();
 
  //Initialize particle change by setting member properties of new mu+ particle to the Mu properties
  aParticleChange.Initialize(aTrack);

  particle = particleTable->FindParticle("Mu");
  // Set the new dynamic particle
//  DP = new G4DynamicParticle(particle, 
//			     aStep.GetTrack()->GetDynamicParticle()->GetMomentumDirection(),
//			     aStep.GetTrack()->GetDynamicParticle()->GetKineticEnergy());
  // GMM test this
  G4double  newKE =
    std::max(aStep.GetTrack()->GetDynamicParticle()->GetKineticEnergy(), 100.*CLHEP::eV);
  DP = new G4DynamicParticle(particle, 
			     aStep.GetTrack()->GetDynamicParticle()->GetMomentumDirection(),
			     newKE);
  
  DP->SetProperTime(aStep.GetTrack()->GetDynamicParticle()->GetProperTime());
  DP->SetPolarization(aStep.GetTrack()->GetDynamicParticle()->GetPolarization().x(),
		      aStep.GetTrack()->GetDynamicParticle()->GetPolarization().y(),
		      aStep.GetTrack()->GetDynamicParticle()->GetPolarization().z());
  DP->SetPreAssignedDecayProperTime(aStep.GetTrack()->GetDynamicParticle()->GetPreAssignedDecayProperTime());

  aSecondary = new G4Track(DP,aTrack.GetGlobalTime(),aTrack.GetPosition());
  aSecondary->SetGoodForTrackingFlag();
  aParticleChange.AddSecondary(aSecondary);

  if (verboseLevel>1) {
    G4cout << "\n +++ MuZeroFormation::PostStepDoIt formed Mu from mu+: Ek_eV, tglb_ns = "
      //<< aStep.GetTrack()->GetDynamicParticle()->GetKineticEnergy()/CLHEP::eV << ", "
	   << newKE/CLHEP::eV << ", "
	   << aTrack.GetGlobalTime()/CLHEP::ns << "\n"
	   << G4endl;
  }

  // Kill current muon track
  aParticleChange.ProposeTrackStatus(fStopAndKill);

  return &aParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


G4bool MuZeroFormation::IsApplicable(const G4ParticleDefinition& aPD)
{
  return (aPD.GetParticleName() == "mu+");
}

G4double MuZeroFormation::FindCrossSection(G4double ek)
{
  // use parametrization of plot of log(xc,cm^2) vs log(ek,ev)
  logEk = std::log10(ek/CLHEP::eV);
  if ( logEk > GetLogEevxshigh() ) {
    return DBL_MIN;
  }
  else if ( logEk < GetLogEevxslow() ) {
    return DBL_MIN;
  }
  else {
    G4double logEkminustwo = logEk - 2.;
    logxs = Getxcpar(0) + Getxcpar(1)*logEkminustwo + Getxcpar(2)*std::exp(logEkminustwo/Getxcpar(3));
    G4double xs = std::pow(10.,logxs)*1.e-16*CLHEP::cm2; // output in G4 area units
    G4double xs10scale = 1.0;  // scale tuning factor to match Mu formation probability
    return xs10scale*xs; 
  }
}

G4double MuZeroFormation::FindMFP(G4double xs, G4double density)
{  // convert xs to mfp with simple formula: mfp = 1./(n*xs) = (molar wt)/(density * xs * N_Avogadro)
  G4double mfpinv = CLHEP::Avogadro * density * xs / (60.08*CLHEP::gram) ;
  return 1./mfpinv;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

