// ------------------------------------------------------------
//      GEANT 4 class header file
//
//      History: first implementation, based on object model of
//      4-th April 1996, G.Cosmo
// ****************************************************************
//      New implementation as a utility class  M.Asai, 26 July 2004
//
//      2008-05  Implementation for muonium by Toni Shiroka 
// ----------------------------------------------------------------

#ifndef MuZero_h
#define MuZero_h 1

#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"

// ######################################################################
// ###                         MUONIUM                               ###
// ######################################################################

class MuZero : public G4ParticleDefinition
{
private:
  static MuZero* theInstance;
  MuZero(){}
  ~MuZero(){}

public:
  static MuZero* Definition();
  static MuZero* MuoniumDefinition();
  static MuZero* Muonium();
};

#endif
