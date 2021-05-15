/// from B3StackingAction.hh

#ifndef StackingAction_h
#define StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"

/// Stacking action class : manage the newly generated particles
///
/// Do not track secondary neutrino, so kill it 
/// immediately, before created particles are put on a stack.

class StackingAction : public G4UserStackingAction
{
public:
  StackingAction();
  virtual ~StackingAction();
     
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);        
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

