//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HistoManager_h
#define HistoManager_h 1

#include <TVectorD.h>
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleMomentum.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction;    // needed if detector geometry determines histograms

class TH1D;
class TH2D;
class TFile;
class TTree; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HistoManager
{
public:
  HistoManager(DetectorConstruction*);
  //HistoManager();
  ~HistoManager();
   
  static const G4int Max1D = 55;
  static const G4int Max2D = 9;
  TH1D*    h1D[Max1D];
  TH2D*    h2D[Max2D];   

  void book();
  void save();
  void SetRunNumber(G4int rnmb);
  void SetEventNumber(G4int ievt);
  void SetEventsRequested(G4int ievt);
  void ZeroEventsPrimary();
  void IncEventsPrimaryAll();
  void IncEventsPrimaryFail();

  void FillHisto(G4int id, G4double bin, G4double weight = 1.0);
  void Fill2DHisto(G4int id, G4double xbin, G4double ybin, G4double weight = 1.0);
  void Normalize(G4int id, G4double fac);    

  //  void FillNtuple1(G4int rnum, G4int iev, G4double xin, G4double yin, G4double zin, 
  //		   G4double xdec, G4double ydec, G4double zdec, G4double tdec,
  //		   G4int nh[], G4double ts[][MAX_NHITS], G4double ed[][MAX_NHITS], G4int ntp);
    
  void PrintStatistic();

  G4int GetRunNumber() {return runNumber;}
  G4int GetEventsRequested() {return eventsRequested;}
  //G4int GetEventsPrimaryAll() {return eventsPrimaryAll;}
  //G4int GetEventsPrimaryFail() {return eventsPrimaryFail;}

  G4String rootFileName;
  G4String logFileName;
  G4double histo_pmom;
  G4double histo_dpft;
  G4double histo_dpfracrms;
  G4int runNumber;
  G4int eventNumber;
  G4int eventsRequested;
  G4int eventsPrimaryAll;
  G4int eventsPrimaryFail;
   
private:
  
  DetectorConstruction* fDetConstruction;     //pointer to the geometry
  TFile*   rootFile;
  //TTree*   ntupl1;

  //  G4double x_in, y_in, z_in; 
  //  G4double x_dec, y_dec, z_dec, t_dec; 

  //G4double t_s[n_tp];

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

