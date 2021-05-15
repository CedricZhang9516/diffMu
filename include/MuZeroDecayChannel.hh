
// $Id: G4MuonDecayChannel.hh 67971 2013-03-13 10:13:24Z gcosmo $
//
// MuZeroDecayChannel.hh
//
// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      History: first implementation, based on object model of
//      30 May 1997 H.Kurashige
// ------------------------------------------------------------
#ifndef MuZeroDecayChannel_h
#define MuZeroDecayChannel_h 1

#include "G4ios.hh"
#include "globals.hh"
#include "G4VDecayChannel.hh"

class MuZeroDecayChannel :public G4VDecayChannel
{
  // Class Decription
  //  This class describes muon decay kinemtics.
  //  This version neglects muon polarization  
  //              assumes the pure V-A coupling
  //              gives incorrect energy spectrum for neutrinos
  //

public:  // With Description
  //Constructors 
  MuZeroDecayChannel(const G4String& theParentName,
		     G4double        theBR);
  //  Destructor
  virtual ~MuZeroDecayChannel();

protected:
  // Copy constructor and assignment operator
  MuZeroDecayChannel(const MuZeroDecayChannel &);
  MuZeroDecayChannel & operator=(const MuZeroDecayChannel &);

protected:
  MuZeroDecayChannel();

public:  // With Description
  virtual G4DecayProducts *DecayIt(G4double);     
  
};  


#endif
