
#include "MuZero.hh"
#include "G4ParticleTable.hh"

#include "MuZeroDecayChannel.hh"
#include "MuZeroDecayChannelWithSpin.hh"
#include "G4DecayTable.hh"

// ######################################################################
// ###                          MUONIUM                               ###
// ######################################################################
MuZero* MuZero::theInstance = 0;

MuZero* MuZero::Definition()
{
  if (theInstance !=0) return theInstance;
  const G4String name = "Mu";
  // search in particle table]
  G4ParticleTable* pTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* anInstance = pTable->FindParticle(name);
  if (anInstance ==0)
    {
      // create particle
      //
      //    Arguments for constructor are as follows
      //               name             mass          width         charge
      //             2*spin           parity  C-conjugation
      //          2*Isospin       2*Isospin3       G-parity
      //               type    lepton number  baryon number   PDG encoding
      //             stable         lifetime    decay table
      //             shortlived      subType    anti_encoding
      anInstance = new G4ParticleDefinition(
					    name,   0.1056584*CLHEP::GeV, 2.99591e-16*CLHEP::MeV,   0.*CLHEP::eplus, 
					    1,               0,             0,          
					    0,               0,             0,             
					    "lepton",              -1,             0,         -1311,
					    false,      2196.98*CLHEP::ns,          NULL,
					    false,           "mu"
					    );
      // Bohr magnetron of Muonium - T. Shiroka
      // The magnetic moment of Mu is the sum of those of mu+ and e- with
      // the respective gyromagnetic ratio anomalies as coefficients
   
      G4double muBmu =  0.5*CLHEP::eplus*CLHEP::hbar_Planck/(0.10565840*CLHEP::GeV/CLHEP::c_squared);
      G4double muBel = -0.5*CLHEP::eplus*CLHEP::hbar_Planck/(0.51099906*CLHEP::MeV/CLHEP::c_squared);
      G4double muB   =  1.0011659208*muBmu + 1.0011596521859*muBel;
   
      anInstance->SetPDGMagneticMoment( muB );

      //create Decay Table 
      G4DecayTable* MuZeroDecayTable = new G4DecayTable();

      // create single decay channel - 100% polarization
      G4VDecayChannel* modefull = new MuZeroDecayChannelWithSpin("Mu",1.0);
      MuZeroDecayTable->Insert(modefull);

//      //// or create two decay channels
//      G4VDecayChannel* mode1 = new MuZeroDecayChannel("Mu",0.5);
//      MuZeroDecayTable->Insert(mode1);
//      // one has spin-dependent decay
//      G4VDecayChannel* mode2 = new MuZeroDecayChannelWithSpin("Mu",0.5);
//      MuZeroDecayTable->Insert(mode2);

      anInstance->SetDecayTable(MuZeroDecayTable);
      MuZeroDecayTable->DumpInfo();
    }
  theInstance = reinterpret_cast<MuZero*>(anInstance);
  return theInstance;
}

MuZero*  MuZero::MuoniumDefinition()
{
  return Definition();
}

MuZero*  MuZero::Muonium()
{
  return Definition();
}

