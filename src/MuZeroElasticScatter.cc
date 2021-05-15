/// MuZeroElasticScatter.cc
/// model for non-charge-changing collisions (0->0) for muonium energy loss
//


#include "MuZeroElasticScatter.hh"
#include "MuZero.hh"

#include "G4SystemOfUnits.hh"


MuZeroElasticScatter::MuZeroElasticScatter(const G4String& aName, G4ProcessType aType)
  : G4VDiscreteProcess(aName, aType) 
{
  if (verboseLevel>1) {
    G4cout << GetProcessName() << " is created "<< G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


MuZeroElasticScatter::~MuZeroElasticScatter()
{;}


G4double MuZeroElasticScatter::GetMeanFreePath( 
					       const G4Track& aTrack, G4double /*previousStepSize*/, G4ForceCondition* condition)
{
  G4String p_name = aTrack.GetDefinition()->GetParticleName();
  G4String lv_name = aTrack.GetVolume()->GetLogicalVolume()->GetName();
  G4String mat_name = aTrack.GetVolume()->GetLogicalVolume()->GetMaterial()->GetName();
  G4double mfp;
  if (p_name == "Mu") {
    if ((lv_name == "AgelLyr")||(lv_name == "PerfLyr")) {  // find mfp in aerogel
      G4double EkMu = aTrack.GetKineticEnergy();
      G4double lyr_density = aTrack.GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity();
      //Calculate mean free path
      G4double xsection = MuZeroElasticScatter::FindCrossSection(EkMu);
      mfp =  MuZeroElasticScatter::FindMFP(xsection, lyr_density);
      *condition = NotForced;
      if (verboseLevel>1) {
	G4cout << "\n ++++++++ " << G4endl;
	//G4cout << "\nMuZeroElasticScatter::GetMeanFreePath EprotonEquiv/CLHEP::keV "
	//       << EprotonEquiv/keV << G4endl;
	G4cout << "MuZeroElasticScatter::GetMeanFreePath xsection[cm2], density[g/cm3]: "
	       << xsection/CLHEP::cm2 << ", " << lyr_density/(CLHEP::gram/CLHEP::cm3) << G4endl;
	G4cout << "MuZeroElasticScatter::GetMeanFreePath EkMu[eV], FindMFP[mm]: "
	       << EkMu/CLHEP::eV << ", " << mfp/CLHEP::mm << G4endl;
	G4cout << "\n ++++++++ " << G4endl;
      }
    }
    else  {     // no scattering (use MuZeroIonization to make mu+ in another volume)
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
MuZeroElasticScatter::PostStepDoIt( const G4Track& aTrack,
				    const G4Step& /* aStep*/ )
{

  //G4StepPoint* postStepPoint = aStep.GetPostStepPoint();
  //if(postStepPoint->GetStepStatus()==fGeomBoundary)
  // { return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);}

  particleTable=G4ParticleTable::GetParticleTable();
 
  //Initialize particle change by setting member properties of new mu+ particle to the Mu properties
  aParticleChange.Initialize(aTrack);

  // Propose particle changes; all the same except energy (todo: add scattering angle)
  // If it falls below lower limit, stop the track (todo: create isotropic thermal distribution)

  G4double Ekprop = aTrack.GetKineticEnergy()*GetElossFraction();
  if (std::log10(Ekprop/CLHEP::eV) > GetLogEevxslow() ) {
    aParticleChange.ProposePosition(aTrack.GetPosition());
    aParticleChange.ProposeMomentumDirection(aTrack.GetMomentumDirection());
    aParticleChange.ProposeEnergy(Ekprop);
    aParticleChange.ProposeNonIonizingEnergyDeposit(aTrack.GetKineticEnergy() * (1. - GetElossFraction() ) );
    aParticleChange.ProposePolarization(aTrack.GetPolarization());
    aParticleChange.ProposeGlobalTime(aTrack.GetGlobalTime());
    aParticleChange.ProposeProperTime(aTrack.GetProperTime());
    aParticleChange.ProposeTrackStatus(aTrack.GetTrackStatus()) ;
    if (verboseLevel>1) {
      G4cout << "\n++++++++ " << G4endl;
      G4cout << "MuZeroElasticScatter::PostStepDoIt Ekprop[eV], LogEevxslow: "
	     << Ekprop/( CLHEP::eV ) << ", "
	     << GetLogEevxslow() << G4endl;
      G4cout << "++++++++ \n" << G4endl;
    }
  }
  else {   // with diffusion, the energy has to be greater than zero
    aParticleChange.ProposePosition(aTrack.GetPosition());
    aParticleChange.ProposeMomentumDirection(aTrack.GetMomentumDirection());
    // to be improved: this should sample from thermal distribution
    aParticleChange.ProposeEnergy(0.025*CLHEP::eV);
    aParticleChange.ProposeNonIonizingEnergyDeposit(aTrack.GetKineticEnergy());
    aParticleChange.ProposePolarization(aTrack.GetPolarization());
    aParticleChange.ProposeGlobalTime(aTrack.GetGlobalTime());
    aParticleChange.ProposeProperTime(aTrack.GetProperTime());
    aParticleChange.ProposeTrackStatus(aTrack.GetTrackStatus()) ;
    if (verboseLevel>1) {
      G4cout << "\n ++++++++ " << G4endl;
      G4cout << "MuZeroElasticScatter::PostStepDoIt Ekprop[eV], set to 0.025 eV: "
	     << Ekprop/( CLHEP::eV ) << G4endl;
      G4cout << "\n ++++++++ " << G4endl;
    }
  }

  return &aParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


G4bool MuZeroElasticScatter::IsApplicable(const G4ParticleDefinition& aPD)
{
  return (aPD.GetParticleName() == "Mu");
}

G4double MuZeroElasticScatter::FindCrossSection(G4double ek)
{
  // use parametrization of plot of log(xc,10^{-16}cm^2) vs log(ek,eV)
  logEk = std::log10(ek/CLHEP::eV);
  if ( logEk > GetLogEevxshigh() ) {
    return DBL_MIN;
  }
  else if ( logEk < GetLogEevxslow() ) {
    return DBL_MIN;  
  }
  else {
    logxs = Getxcpar(0) + Getxcpar(1)*logEk;
    G4double xs = std::pow(10.,logxs-16.)*CLHEP::cm2; // output in G4 area units
    return xs; 
  }
}

G4double MuZeroElasticScatter::FindMFP(G4double xs, G4double density)
{  // convert xs to mfp with simple formula: mfp = 1./(n*xs) = (molar wt)/(density * xs * N_Avogadro)
  G4double mfpinv = CLHEP::Avogadro * density * xs / (60.08*CLHEP::gram) ;
  return 1./mfpinv;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

