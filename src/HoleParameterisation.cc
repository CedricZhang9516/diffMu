
#include "HoleParameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"

//#include <stdlib.h>     /* div, div_t */

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HoleParameterisation::HoleParameterisation(  
					   G4int    noHoles, 
					   G4int    noHx, 
					   G4int    noHy, 
					   G4double diameterHole,       
					   G4double depthHole,      
					   G4double zlocHole,      
					   G4double pitchHx,       
					   G4double pitchHy,       
					   G4double startHx,       
					   G4double startHy,       
					   G4double offsetHx)
{
  fnoHoles =    noHoles; 
  fnoHx =       noHx;      
  fnoHy =       noHy;
  fdiameterHole = diameterHole;       
  fdepthHole =  depthHole;   
  fzlocHole =  zlocHole;   
  fpitchHx =    pitchHx;      
  fpitchHy =    pitchHy;      
  fstartHx =    startHx;     
  fstartHy =    startHy;     
  foffsetHx =   offsetHx;
  if( noHoles > 0 && diameterHole > 0.){
    if (diameterHole >= pitchHx) {
      G4Exception("HoleParameterisation::HoleParameterisation()",
		  "InvalidSetup", FatalException,
		  "Diameter>Pitch");
    }
    if (noHoles != noHx*noHy) {
      G4Exception("HoleParameterisation::HoleParameterisation()",
		  "InvalidSetup", FatalException,
		  "noHoles!=noHx*noHy");
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HoleParameterisation::~HoleParameterisation()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HoleParameterisation::ComputeTransformation
(const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  // Note: copyNo will start with zero!
  std::div_t XYind = std::div(copyNo,fnoHx);   // quotient is row in Y, remainer is "column" in X
  G4int Xind = XYind.rem;   // index from low to high in X and Y
  G4int Yind = XYind.quot;
  G4int Xswitch = std::div(Yind,2).rem;
  G4double Xoffset = foffsetHx*Xswitch; // offset every second column to get hex pattern
  G4double Xposition = fstartHx + Xind*fpitchHx + Xoffset;
  G4double Yposition = fstartHy + Yind*fpitchHy;
  //G4cout << "Hole position x,y: " << Xposition/mm << "," << Yposition/CLHEP::mm << G4endl; 
  G4ThreeVector origin(Xposition,Yposition,fzlocHole);
  physVol->SetTranslation(origin);
  physVol->SetRotation(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HoleParameterisation::ComputeDimensions
(G4Tubs& perfHole, const G4int /* copyNo */, const G4VPhysicalVolume*) const
{
  // Note: copyNo will start with zero!
  // No parameterization of hole dimensions
  perfHole.SetInnerRadius(0);
  perfHole.SetOuterRadius(fdiameterHole/2);
  perfHole.SetZHalfLength(fdepthHole/2);
  perfHole.SetStartPhiAngle(0.*CLHEP::deg);
  perfHole.SetDeltaPhiAngle(360.*CLHEP::deg);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
