
#ifndef HoleParameterisation_h
#define HoleParameterisation_h 1

#include "globals.hh"
#include "G4VPVParameterisation.hh"

class G4VPhysicalVolume;
class G4Box;

// Dummy declarations to get rid of warnings ...
class G4Trd;
class G4Trap;
class G4Cons;
class G4Orb;
class G4Sphere;
class G4Torus;
class G4Para;
class G4Hype;
class G4Tubs;
class G4Polycone;
class G4Polyhedra;

///  A parameterisation that describes a series of boxes along Z.
///
///  The boxes have equal width, & their lengths are a linear equation.
///  They are spaced an equal distance apart, starting from given location.

class HoleParameterisation : public G4VPVParameterisation
{ 
public:
  
  HoleParameterisation(  
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
		       G4double offsetHx);

  virtual ~HoleParameterisation();
   
  void ComputeTransformation (const G4int copyNo,
			      G4VPhysicalVolume* physVol) const;
    
  void ComputeDimensions (G4Tubs & perfLayer, const G4int copyNo,
			  const G4VPhysicalVolume* physVol) const;

private:  // Dummy declarations to get rid of warnings ...

  void ComputeDimensions (G4Box&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Trd&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Trap&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Cons&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Sphere&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Orb&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Torus&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Para&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Hype&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Polycone&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Polyhedra&,const G4int,const G4VPhysicalVolume*) const {}

private:

  G4int    fnoHoles; 
  G4int    fnoHx; 
  G4int    fnoHy; 
  G4double fdiameterHole;       
  G4double fdepthHole;      
  G4double fzlocHole;      
  G4double fpitchHx;       
  G4double fpitchHy;       
  G4double fstartHx;       
  G4double fstartHy;       
  G4double foffsetHx;   
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
