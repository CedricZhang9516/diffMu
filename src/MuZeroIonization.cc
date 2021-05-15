/// MuZeroIonization.cc
/// model for charge-changing collisions (0->1) for muonium to muonplus
//

#include "MuZeroIonization.hh"
#include "MuZero.hh"

#include "G4SystemOfUnits.hh"

// initialize static doubles outside of functions. is there a better way? not g4 approved.
//G4double MuZeroIonization::pMuMassRatio = 8.8802434;  // codata
//G4double MuZeroIonization::logEevxslow = 2.0;    // 
//G4double MuZeroIonization::logEevxshigh = 4.5;
  

MuZeroIonization::MuZeroIonization(const G4String& aName, G4ProcessType aType)
  : G4VDiscreteProcess(aName, aType) 
{
  if (verboseLevel>1) {
    G4cout << GetProcessName() << " is created "<< G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


MuZeroIonization::~MuZeroIonization()
{;}


G4double MuZeroIonization::GetMeanFreePath( 
					   const G4Track& aTrack, G4double /*previousStepSize*/, G4ForceCondition* condition)
{
  G4String p_name = aTrack.GetDefinition()->GetParticleName();
  // G4String lv_name = aTrack.GetVolume()->GetLogicalVolume()->GetName();
  G4String mat_name = aTrack.GetVolume()->GetLogicalVolume()->GetMaterial()->GetName();
  G4double mfp;
  if (p_name == "Mu") {
    if (mat_name.compare(0,7,"Aerogel") == 0) {  // Mu should have MFP for ionization
      G4double EkMu= aTrack.GetKineticEnergy();
      G4double lyr_density = aTrack.GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity();
      //Calculate mean free path
      //G4double EprotonEquiv = pMuMassRatio*E;
      G4double xsection = MuZeroIonization::FindCrossSection(EkMu);
      mfp =  MuZeroIonization::FindMFP(xsection, lyr_density);
      *condition = NotForced;
      if (verboseLevel>1) {
	G4cout << "\n++++++++ " << G4endl;
	//G4cout << "\nMuZeroIonization::GetMeanFreePath EprotonEquiv/CLHEP::eV "
	//       << EprotonEquiv/eV << G4endl;
	G4cout << "MuZeroIonization::GetMeanFreePath xsection[cm2], density[g/cm3]: "
	       << xsection/CLHEP::cm2 << ", " << lyr_density/(CLHEP::gram/CLHEP::cm3) << G4endl;
	G4cout << "MuZeroIonization::GetMeanFreePath EkMu[eV], FindMFP[mm] "
	       << EkMu/CLHEP::eV << ", " << mfp/CLHEP::mm << G4endl;
	G4cout << "++++++++ " << G4endl;
      }
    }
    else if (mat_name == "G4_Galactic")  { // Mu should fly
      mfp = DBL_MAX;
      *condition = NotForced;
    }
    else  {     // Mu should stop and become muon
      mfp = DBL_MIN;
      *condition = NotForced;   // should this be forced?
    }
  }
  else  {   // not Mu, so should not be affected; does this happen? IsApplicable should prevent it.
    mfp = DBL_MAX;
    *condition = NotForced;
    G4cout << "\nMuZeroIonization::GetMeanFreePath NOT Mu! mfp/mm set  " << mfp/mm << G4endl;
  }
  return mfp;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


G4VParticleChange*
MuZeroIonization::PostStepDoIt( const G4Track& aTrack,
				const G4Step& aStep)
{

  //G4StepPoint* postStepPoint = aStep.GetPostStepPoint();
  //if(postStepPoint->GetStepStatus()==fGeomBoundary)
  // { return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);}

  particleTable=G4ParticleTable::GetParticleTable();
 
  //Initialize particle change by setting member properties of new mu+ particle to the Mu properties
  aParticleChange.Initialize(aTrack);

  particle = particleTable->FindParticle("mu+");
  // Set the new dynamic particle
  DP = new G4DynamicParticle(particle, 
			     aStep.GetTrack()->GetDynamicParticle()->GetMomentumDirection(),
			     aStep.GetTrack()->GetDynamicParticle()->GetKineticEnergy());
  
  DP->SetProperTime(aStep.GetTrack()->GetDynamicParticle()->GetProperTime());
  DP->SetPolarization(aStep.GetTrack()->GetDynamicParticle()->GetPolarization().x(),
		      aStep.GetTrack()->GetDynamicParticle()->GetPolarization().y(),
		      aStep.GetTrack()->GetDynamicParticle()->GetPolarization().z());
  DP->SetPreAssignedDecayProperTime(aStep.GetTrack()->GetDynamicParticle()->GetPreAssignedDecayProperTime());

  aSecondary = new G4Track(DP,aTrack.GetGlobalTime(),aTrack.GetPosition());
      
  aParticleChange.AddSecondary(aSecondary);

  // Kill current Mu track
  aParticleChange.ProposeTrackStatus(fStopAndKill);
  if (verboseLevel>1) {
    G4cout << "\n +++ MuZeroIonization::PostStepDoIt formed mu+ from Mu \n" << G4endl;
  }

  return &aParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


G4bool MuZeroIonization::IsApplicable(const G4ParticleDefinition& aPD)
{
  return (aPD.GetParticleName() == "Mu");
}

G4double MuZeroIonization::FindCrossSection(G4double ek)
{
  // use parametrization of plot of log(xc,cm^2) vs log(ek,eV)
  logEk = std::log10(ek/CLHEP::eV);
  if ( logEk > GetLogEevxshigh() ) {
    return DBL_MIN;
  }
  else if ( logEk < GetLogEevxslow() ) {
    return DBL_MIN;
  }
  else {
  G4double xgmax = logEk - Getxcpar(2);
  G4double varsigmasq = Getxcpar(3)*Getxcpar(3)*(1. + Getxcpar(4)*xgmax);
  logxs = Getxcpar(0) + (Getxcpar(1)-Getxcpar(0))*std::exp((-1.*xgmax*xgmax)/(2*varsigmasq));
  G4double xs = std::pow(10.,logxs)*1.e-16*CLHEP::cm2;  // output in G4 area units
  return xs;
  }
}

G4double MuZeroIonization::FindMFP(G4double xs, G4double density)
{  // convert xs to mfp with simple formula: mfp = 1./(n*xs) = (molar wt)/(density * xs * N_Avogadro)
  G4double mfpinv = CLHEP::Avogadro * density * xs / (60.08*CLHEP::gram) ;
  return 1./mfpinv;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

