
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef Field_h
#define Field_h 1

#include "globals.hh"
//#include "G4ElectroMagneticField.hh"
#include "G4MagneticField.hh"
//#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

class G4EqEMFieldWithSpin;
class G4MagIntegratorStepper;
class G4ChordFinder;
class G4PropagatorInField;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Field : public G4MagneticField
{

public:

  Field();
  virtual ~Field();

  /// DoesFieldChangeEnergy() returns true.
  virtual G4bool DoesFieldChangeEnergy() const { return true; };

  /// GetFieldValue() returns the field value at a given point[]: see G4MagneticField.hh
  /// field is really field[6]: Bx,By,Bz,Ex,Ey,Ez.
  /// point[] is in global coordinates: x,y,z,t.
  virtual void GetFieldValue( const G4double Point[4],  G4double* Bfield ) const;

  //void GetCoilFieldValue( const G4ThreeVector ptfield,  G4ThreeVector* BThreeField ) const;

private:

//  G4EqEMFieldWithSpin*    fEquation;
//  G4MagIntegratorStepper* fStepper;
//  G4ChordFinder*          fChordFinder;
//  G4PropagatorInField*    fieldPropagator;

  //static G4double GetCoilRadius() {return 0.5*CLHEP::meter ; }

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
