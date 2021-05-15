
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "Field.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

//#include "G4EqEMFieldWithSpin.hh"
#include "G4Mag_SpinEqRhs.hh"
#include "G4ChordFinder.hh"
#include "G4PropagatorInField.hh"

#include "G4MagIntegratorStepper.hh"
#include "G4ClassicalRK4.hh"

//#include <RooMathMoreReg.h>
//#include <Math/SpecFuncMathMore.h>   // part of ROOT libraries - for elliptic integrals in Math/SpecFuncMathMore.h

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Field::Field() : G4ElectroMagneticField()
Field::Field() : G4MagneticField()
{
//  //  fEquation = new G4EqEMFieldWithSpin(this);
//  fEquation = new G4EqEMFieldWithSpin(this);
//
//  G4FieldManager* fieldMgr
//    = G4TransportationManager::GetTransportationManager()->GetFieldManager();
//
//  fieldMgr->SetDetectorField(this);
//
//  fStepper = new G4ClassicalRK4(fEquation,12);
//
//  G4double minStep           = 0.01*CLHEP::mm;
//
//  fChordFinder = new G4ChordFinder((G4MagneticField*)this,minStep,fStepper);
// 
//  // Set accuracy parameters
//  G4double deltaChord        = 3.0*CLHEP::mm;
//  fChordFinder->SetDeltaChord( deltaChord );
//
//  G4double deltaOneStep      = 0.01*CLHEP::mm; 
//  fieldMgr->SetAccuraciesWithDeltaOneStep(deltaOneStep);
//
//  G4double deltaIntersection = 0.1*CLHEP::mm; 
//  fieldMgr->SetDeltaIntersection(deltaIntersection);
//
//  G4TransportationManager* fTransportManager =
//    G4TransportationManager::GetTransportationManager();
//
//  fieldPropagator = fTransportManager->GetPropagatorInField();
//
//  G4double epsMin            = 2.5e-7*CLHEP::mm;
//  G4double epsMax            = 0.05*CLHEP::mm;
// 
//  fieldPropagator->SetMinimumEpsilonStep(epsMin);
//  fieldPropagator->SetMaximumEpsilonStep(epsMax);
// 
//  fieldMgr->SetChordFinder(fChordFinder);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Field::~Field()
{
//  if (fEquation)    delete fEquation;
//  if (fStepper)     delete fStepper;
//  if (fChordFinder) delete fChordFinder;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Field::GetFieldValue( const G4double Point[4], G4double* Bfield ) const
{
  // Point[0],Point[1],Point[2] are x-, y-, z-cordinates 

  //const G4double Bz = 0.24*CLHEP::tesla;
  //const G4double Er = 2.113987E+6*CLHEP::volt/CLHEP::m;
  //const G4double By = 0.007*CLHEP::tesla;
  //const G4double Er = 0.*CLHEP::volt/CLHEP::m;
  //  const G4double By = 0.0*CLHEP::tesla;
  //  const G4double Er = 0.0*CLHEP::volt/CLHEP::m;
  //
  //  G4double Ex,Ey;
  // 
  //  G4double posR = std::sqrt(std::pow(Point[0],2) + std::pow(Point[1],2));
  //  G4double cos_theta, sin_theta;
  //
  //  if (posR>0){
  //     cos_theta = Point[0]/(G4double)posR;
  //     sin_theta = Point[1]/(G4double)posR;
  //     Ex = -1*Er*cos_theta;//apply radial electric field
  //     Ey = -1*Er*sin_theta;
  //  }else{
  //     Ex=0;
  //     Ey=0;
  //  }
  //  
  //  Bfield[0]=0;
  //  Bfield[1]=By;
  //  Bfield[2]=0;
  //
  //  Bfield[3]=Ex;
  //  Bfield[4]=Ey;
  //  Bfield[5]=0;

  // set fields
  //const G4double By = -0.5*CLHEP::tesla;  // for test
  // from JHB post of 20170710: https://daq11.triumf.ca/EL/170710.8621
  // -1.4 G, similar to 2017 run but in -z (down) direction rather than 45 deg
  const G4double By = -0.00014*CLHEP::tesla;  
  //const G4double By = 0.001726*CLHEP::tesla;   //17.26 G (B_p, applied positive field) 
  const G4double xymax = 0.5*CLHEP::meter;
  const G4double zmax = 1.0*CLHEP::meter;
  Bfield[0]=0;
  if ( (std::abs(Point[0]) < xymax ) && (std::abs(Point[1]) < xymax ) && (std::abs(Point[2]) < zmax ) ) {
    Bfield[1]=By;
  }
  else {
    Bfield[1]=0;
  }    
  Bfield[2]=0;
  Bfield[3]=0;
  Bfield[4]=0;
  Bfield[5]=0;

  return;
}

//void Field::GetCoilFieldValue( const G4ThreeVector ptfield, G4ThreeVector* BThreeField ) const
//{
//  // ptfield is vector from center of circular coil of radius GetCoilRadius() in xy plane, field in z direction
//  G4double rsq = ptfield.dot(ptfield);
//  G4double rhosq = rsq - ptfield[2]*ptfield[2];
//  G4double rho = std::sqrt(rhosq);
//  G4double rcoilsq = GetCoilRadius() * GetCoilRadius();
//  G4double alphasq = rcoilsq + rsq - 2.*GetCoilRadius()*rho;
//  G4double betasq = rcoilsq + rsq + 2.*GetCoilRadius()*rho;
//  G4double beta = std::sqrt(betasq);
//  G4double ksq = 1. - alphasq/betasq;
//  G4double kellipt = std::sqrt(ksq);
//  G4double gamma = (ptfield[0] + ptfield[1])*(ptfield[0] - ptfield[1]);
//  G4double elle = ROOT::Math::comp_ellint_2(kellipt);
//  G4double ellk = ROOT::Math::comp_ellint_1(kellipt);
//  G4double xfac, yfac, zfac;
//  if ( rhosq > 0. ) {
//    xfac = 2.*ptfield[0]*ptfield[2]/(alphasq*beta*rhosq)*( (rcoilsq + rsq)*elle - alphasq*ellk );
//    yfac = 2.*ptfield[1]*ptfield[2]/(alphasq*beta*rhosq)*( (rcoilsq + rsq)*elle - alphasq*ellk );
//  }
//  else {
//    xfac = 0.;
//    yfac = 0.;
//  }
//  zfac = 2./(alphasq*beta)*( (rcoilsq - rsq)*elle + alphasq*ellk );
//  BThreeField->set( xfac, yfac, zfac );
//}
  
  
