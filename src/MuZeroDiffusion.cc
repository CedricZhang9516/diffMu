/// MuZeroDiffusion.cc
/// model for diffusion of muonium
//
//

#include "MuZero.hh"
#include "MuZeroDiffusion.hh"

#include "G4RandomDirection.hh"
#include "G4SystemOfUnits.hh"
#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandExponential.h"

///// initialize static doubles outside of functions; is there a better way? this is not G4 approved!
//const G4double MuZeroDiffusion::TempK = 300.*CLHEP::kelvin;  // 
//const G4double MuZeroDiffusion::DiffCon = 10.*CLHEP::cm2/CLHEP::second;  // diffuson constant
//
////// NOT USED
////G4double MuZeroDiffusion::Eevxslow = 1.e-6*CLHEP::eV;   // down to 0.001 meV
//const G4double MuZeroDiffusion::Eevxshigh = 1.0*CLHEP::eV;
//
////G4double MuZeroDiffusion::MuMass = MuZero::Muonium()->GetPDGMass();  // seg fault
////const G4double MuZeroDiffusion::MuMass = 105.6583715*CLHEP::MeV/CLHEP::c_squared;
//const G4double MuZeroDiffusion::MuMass_c2 = 105.6583715*CLHEP::MeV;
//
//const G4double MuZeroDiffusion::MuSpeedMPSq = (2.*CLHEP::k_Boltzmann*TempK/(MuMass_c2))*CLHEP::c_squared;
//const G4double MuZeroDiffusion::MuSpeedMnSq = (8.*CLHEP::k_Boltzmann*TempK/(CLHEP::pi*MuMass_c2 ))*CLHEP::c_squared;
//const G4double MuZeroDiffusion::MuSpeedMn = std::sqrt(MuSpeedMnSq);  // mean speed
//const G4double MuZeroDiffusion::MeanFreePath = 12.*DiffCon/(CLHEP::pi*MuSpeedMn);   
////const G4double MuZeroDiffusion::MeanFreePath = 3.*DiffCon/(0.785*MuSpeedMn);   // check factor in sim


MuZeroDiffusion::MuZeroDiffusion(const G4String& aName, G4ProcessType aType)
  : G4VDiscreteProcess(aName, aType) 
{
  G4cout.precision(5);
  G4cout << "\n==================== MuZeroDiffusion parameters ==================== "
	 << G4endl;
  G4cout << "T (K), D (cm^2/s) : " 
	 << GetTempK()/CLHEP::kelvin
	 << ", "
	 << GetDiffCon()/(CLHEP::cm2/CLHEP::second)
	 << G4endl;
  G4cout << "Mu Mass (MeV/c^2) : " 
	 << GetMuMass()/(CLHEP::MeV)
	 << G4endl;
  G4cout << "Mean speed squared, most probable speed squared (cm/us)^2 : " 
	 << GetMuMeanSpeedSq()/( CLHEP::cm2/( CLHEP::microsecond*CLHEP::microsecond) )
	 << ", "
	 << GetMuMPSpeedSq()/( CLHEP::cm2/(CLHEP::microsecond*CLHEP::microsecond) )
	 << G4endl;
  G4cout << "Mean thermal energy, most probable thermal energy (eV) : " 
	 << 0.5*GetMuMass()*GetMuMeanSpeedSq()/CLHEP::c_squared/( CLHEP::eV )
	 << ", "
	 << 0.5*GetMuMass()*GetMuMPSpeedSq()/CLHEP::c_squared/( CLHEP::eV )
	 << G4endl;
  G4cout << "Mean speed (cm/us), mean diffusion free path (um): " 
	 << GetMuMeanSpeed()/( CLHEP::cm/CLHEP::microsecond )
	 << ", "
	 << GetDiffFreePath()/( CLHEP::micrometer )
	 << G4endl;
   G4cout << "==================== MuZeroDiffusion parameters ====================\n "
	 << G4endl;

  if (verboseLevel>1) {
    G4cout << GetProcessName() << " is created "<< G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


MuZeroDiffusion::~MuZeroDiffusion()
{
  if (verboseLevel>1) {
    G4cout << GetProcessName() << " is created "<< G4endl;
  }
}


G4double MuZeroDiffusion::GetMeanFreePath( const G4Track& aTrack,
					   G4double /*previousStepSize*/,
					   G4ForceCondition* condition)
{
  G4String p_name = aTrack.GetDefinition()->GetParticleName();
  G4String mat_name = aTrack.GetVolume()->GetLogicalVolume()->GetMaterial()->GetName();
  G4double mfp;

  if (p_name == "Mu") {
    if ( mat_name == "G4_Galactic" ) {   // keep going
      mfp = DBL_MAX;
      *condition = NotForced;
    }
    else if (mat_name.compare(0,7,"Aerogel") == 0) {
      G4double EkMu = aTrack.GetKineticEnergy();
      //G4double lyr_density = aTrack.GetVolume()->GetLogicalVolume()->GetMaterial()->GetDensity();
      if ( EkMu > GetHighELimit() ) {
	mfp = DBL_MAX;
	*condition = NotForced;
	if (verboseLevel>1) {
	  G4cout << "\n ++++++++ " << G4endl;
	  G4cout << "MuZeroDiffusion::GetMeanFreePath EkMu[eV], HighELimit[eV] "
		 << EkMu/CLHEP::eV << ", " << GetHighELimit()/CLHEP::eV
		 << G4endl;
	  G4cout << "\n ++++++++ " << G4endl;
	  }
      }
      else {	
	// find diffusion mfp in aerogel layers
	mfp = GetDiffFreePath();
	*condition = NotForced;
	if (verboseLevel>1) {
	  G4cout << "\n++++++++ " << G4endl;
	  G4cout << "MuZeroDiffusion::GetMeanFreePath Aerogel EkMu[eV], DiffFreePath[mm] "
		 << EkMu/CLHEP::eV << ", " << mfp/CLHEP::mm << G4endl;
	  G4cout << "++++++++ \n " << G4endl;
	  }
      }
    }
    else  {     // no diffusion, stop
      mfp = DBL_MIN;
      *condition = NotForced;
      //G4cout << "MuZeroDiffusion::GetMeanFreePath not agel!!! mfp/mm " << mfp/CLHEP::mm << G4endl;
    }
  }
  else  {   // not Mu, so should not be affected; does this happen? IsApplicable should prevent this
    mfp = DBL_MAX;
    *condition = NotForced;
  }
  //G4cout << "MuZeroDiffusion::GetMeanFreePath out mfp/um " << mfp/CLHEP::um << G4endl;
  return mfp;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....


G4VParticleChange*
MuZeroDiffusion::PostStepDoIt( const G4Track& aTrack,
			       const G4Step& aStep)
{

  particleTable=G4ParticleTable::GetParticleTable();
 
  //Initialize particle change by setting member properties of new particle 
  //to the post-diffusion Mu properties
  //aParticleChange.Clear();
  aParticleChange.Initialize(aTrack);

  // For elastic scattering, _any_ result is considered an interaction (from G4HadronElasticProcess.cc)
  //ClearNumberOfInteractionLengthLeft();

  // step information 
  G4StepPoint* postStepPoint = aStep.GetPostStepPoint();
  // boundary step, no change in track parameters (energy, direction)
  if(postStepPoint->GetStepStatus()==fGeomBoundary)
    { return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);}

  G4TouchableHandle postTouch = postStepPoint->GetTouchableHandle();
  //G4String lv_name = postTouch->GetVolume()->GetLogicalVolume()->GetName();
  G4String mat_name = postTouch->GetVolume()->GetLogicalVolume()->GetMaterial()->GetName();

  //G4double EkMu = aTrack.GetKineticEnergy();

  // Propose particle changes
  if ( mat_name.compare(0,7,"Aerogel") == 0 ) {
    // no boundary change and we're in aerogel
    // find speed from Maxwell distribution and calculate energy
    G4double SpeedpropSq = GetMuMPSpeedSq()*MaxwellSpeedNorm();
    G4double Ekprop = 0.5*(GetMuMass()/CLHEP::MeV)*SpeedpropSq/CLHEP::c_squared;
    if (verboseLevel>1) {
      G4cout << "MuZeroDiffusion::PostStepDoIt mat_name, Ekprop[eV], SpeedpropSq[cm2/us2]: "
	     << mat_name << ", " << Ekprop/( CLHEP::eV ) << ", "
	     << SpeedpropSq/( CLHEP::cm2/CLHEP::microsecond/CLHEP::microsecond )
	     << G4endl;
    }
    //
    // set new energy and random direction;
    // this is simple "gas" diffusion model; others could be investigated
    aParticleChange.ProposeEnergy(Ekprop);
    aParticleChange.ProposeMomentumDirection(G4RandomDirection());
    aParticleChange.ProposeTrackStatus(fAlive);
  }
  else if ( mat_name == "G4_Galactic") {
    // if there is a step in vacuum, no changes to track
    aParticleChange.ProposeTrackStatus(fAlive);    
  }
  else {
    // stop muon but let it decay, if not in vacuum or target material
    aParticleChange.ProposeEnergy(0.);
    aParticleChange.ProposeTrackStatus(fStopButAlive) ;  
    //    G4cout << "MuZeroDiffusion::PostStepDoIt Not aerogel or vacuum ProposeEnergy 0 mat_name: " 
    //	   <<  mat_name << G4endl;
  }

  return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool MuZeroDiffusion::IsApplicable(const G4ParticleDefinition& aPD)
{
  return (aPD.GetParticleName() == "Mu");
}

G4double MuZeroDiffusion::MaxwellSpeedNorm()
{
  // Generate Maxwellian random variate mv^2/2kT (v^2 in units of vmp^2=2kT/m)
  // using Mohamed algorithm
  G4double g_moh_sq = 2.712609;
  G4double yreturn;
  while (true) {
    G4double rnd1 = CLHEP::RandFlat::shoot();
    G4double rnd2 = CLHEP::RandFlat::shoot();
    G4double r21ratio = rnd2/rnd1;
    yreturn = -2.*std::log(rnd1);
    if (g_moh_sq*yreturn >= r21ratio*r21ratio ) break;
  }
  return yreturn;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

