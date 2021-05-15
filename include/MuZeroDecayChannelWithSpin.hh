// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      History:
//               17 August 2004 P.Gumplinger and T.MacPhail
//               samples Michel spectrum including 1st order
//               radiative corrections
//               Reference: Florian Scheck "Muon Physics", in Physics Reports
//                          (Review Section of Physics Letters) 44, No. 4 (1978)
//                          187-248. North-Holland Publishing Company, Amsterdam
//                          at page 210 cc.
//
//                          W.E. Fisher and F. Scheck, Nucl. Phys. B83 (1974) 25.
//
// ------------------------------------------------------------
#ifndef MuZeroDecayChannelWithSpin_hh
#define MuZeroDecayChannelWithSpin_hh 1

#include <CLHEP/Units/PhysicalConstants.h>

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "MuZeroDecayChannel.hh"

class MuZeroDecayChannelWithSpin : public MuZeroDecayChannel
{
  // Class Decription
  // This class describes muon decay kinemtics.
  // This version assumes V-A coupling with 1st order radiative correctons,
  //              the standard model Michel parameter values, but 
  //              gives incorrect energy spectrum for neutrinos

public:  // With Description

  //Constructors 
  MuZeroDecayChannelWithSpin(const G4String& theParentName,
	  		     G4double        theBR);
  //  Destructor
  virtual ~MuZeroDecayChannelWithSpin();

protected:
  // Copy constructor and assignment operator
  MuZeroDecayChannelWithSpin(const MuZeroDecayChannelWithSpin &);
  MuZeroDecayChannelWithSpin & operator=(const MuZeroDecayChannelWithSpin &);
  
private:
  MuZeroDecayChannelWithSpin();

public:  // With Description

  virtual G4DecayProducts *DecayIt(G4double);

private:
  // Radiative Correction Factors

  G4double F_c(G4double x, G4double x0 , G4double omega);
  G4double F_theta(G4double x, G4double x0 , G4double omega);
  G4double R_c(G4double x , G4double omega);

};

inline G4double MuZeroDecayChannelWithSpin::F_c(G4double x, G4double x0 , G4double omega)
{

  G4double f_c;

  f_c = (5.+17.*x-34.*x*x)*(omega+std::log(x))-22.*x+34.*x*x;
  f_c = (1.-x)/(3.*x*x)*f_c;
  f_c = (6.-4.*x)*R_c(x,omega)+(6.-6.*x)*std::log(x) + f_c;
  f_c = (CLHEP::fine_structure_const/CLHEP::twopi) * (x*x-x0*x0) * f_c;

  return f_c;
}

inline G4double MuZeroDecayChannelWithSpin::F_theta(G4double x, G4double x0,G4double omega)
{
  G4double f_theta;

  f_theta = (1.+x+34*x*x)*(omega+std::log(x))+3.-7.*x-32.*x*x;
  f_theta = f_theta + ((4.*(1.-x)*(1.-x))/x)*std::log(1.-x);
  f_theta = (1.-x)/(3.*x*x) * f_theta;
  f_theta = (2.-4.*x)*R_c(x,omega)+(2.-6.*x)*std::log(x)-f_theta;
  f_theta = (CLHEP::fine_structure_const/CLHEP::twopi) * (x*x-x0*x0) * f_theta;

  return f_theta;
}

#endif
