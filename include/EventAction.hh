//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

//class DetectorConstruction;
class RunAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
public:
  EventAction( HistoManager*);
  virtual ~EventAction();

  void  BeginOfEventAction(const G4Event*);
  void    EndOfEventAction(const G4Event*);
    
  void SaveInit(G4double x, G4double y, G4double z,
		G4double dx, G4double dy, G4double dz,
		G4double px, G4double py, G4double pz) 
  {
    xInit = x; yInit = y; zInit = z;
    dxInit = dx; dyInit = dy; dzInit = dz;
    pxInit = px; pyInit = py; pzInit = pz; 
  };

  void SaveTgtStop(G4double z, G4double ek, G4double t) 
  {
    zTgtStop = z; ekTgtStop = ek; tTgtStop = t; 
  };

  void SavemuonDecay(G4double x, G4double y, G4double z, G4double px, G4double py, G4double pz, G4double t) 
  {
    xmuonDecay = x; ymuonDecay = y; zmuonDecay = z;
    pxmuonDecay = px; pymuonDecay = py; pzmuonDecay = pz;
    tmuonDecay = t;
  };

  void SaveMuInit(G4double x, G4double y, G4double z, 
		  G4double dx, G4double dy, G4double dz,
		  G4double px, G4double py, G4double pz,
		  G4double ek, G4double t) 
  {
    xMuInit = x; yMuInit = y; zMuInit = z; 
    dxMuInit = dx; dyMuInit = dy; dzMuInit = dz; 
    pxMuInit = px; pyMuInit = py; pzMuInit = pz; 
    ekMuInit = ek; tMuInit = t;
  };

  void SaveMuEmit(G4double x, G4double y, G4double z, 
		  G4double dx, G4double dy, G4double dz,
		  G4double ek, G4double t) 
  {
    xMuEmit = x; yMuEmit = y; zMuEmit = z; 
    dxMuEmit = dx; dyMuEmit = dy; dzMuEmit = dz; 
    ekMuEmit = ek; tMuEmit = t;
  };

  void SaveMuLRIn(G4double x, G4double y, G4double z, G4double t) 
  {
    xMuLRIn = x; yMuLRIn = y; zMuLRIn = z; tMuLRIn = t;
  };

  void SaveMuLROut(G4double x, G4double y, G4double z, G4double t) 
  {
    xMuLROut = x; yMuLROut = y; zMuLROut = z; tMuLROut = t;
  };

  void SaveMuDecay(G4double x, G4double y, G4double z,
		   G4double px, G4double py, G4double pz,
		   G4double r, G4double t) 
  {
    xMuDecay = x; yMuDecay = y; zMuDecay = z;
    pxMuDecay = px; pyMuDecay = py; pzMuDecay = pz;
    rMuDecay = r; tMuDecay = t;
  };

  void SavePosi(G4double x, G4double y, G4double z,  
		G4double dx, G4double dy, G4double dz,
		G4double t) 
  {
    xPosi = x; yPosi = y; zPosi = z; 
    dxPosi = dx; dyPosi = dy; dzPosi = dz; 
    tPosi = t;
  };

  void ResetEvent();

  void SetIfNewEvent(G4bool);
  G4bool GetIfNewEvent() {return fNewEvent;};
  void SetIfTargetStop(G4bool);
  G4bool GetIfTargetStop() {return fTargetStop;};
  void SetIfNewMuInit(G4bool);
  G4bool GetIfNewMuInit() {return fNewMuInit;};
  void SetIfNewMuEmit(G4bool);
  G4bool GetIfNewMuEmit() {return fNewMuEmit;};
 
  void Fill1D(G4int, G4double);
  void Fill2D(G4int, G4double, G4double);
    
private:
  HistoManager* fHistoManager;

  G4int     runNumber, evtNb;
  G4double  xInit, yInit, zInit;
  G4double  dxInit, dyInit, dzInit;
  G4double  pxInit, pyInit, pzInit;
  G4double  zTgtStop, ekTgtStop, tTgtStop;
  G4double  xmuonDecay, ymuonDecay, zmuonDecay;
  G4double  pxmuonDecay, pymuonDecay, pzmuonDecay;
  G4double  tmuonDecay;
  G4double  xMuInit, yMuInit, zMuInit;
  G4double  dxMuInit, dyMuInit, dzMuInit;
  G4double  pxMuInit, pyMuInit, pzMuInit;
  G4double  ekMuInit, tMuInit;
  G4double  xMuEmit, yMuEmit, zMuEmit;
  G4double  dxMuEmit, dyMuEmit, dzMuEmit;
  G4double  ekMuEmit, tMuEmit;
  G4double  xMuLRIn, yMuLRIn, zMuLRIn, tMuLRIn;
  G4double  xMuLROut, yMuLROut, zMuLROut, tMuLROut;
  G4double  tMuLRDis;
  G4double  xMuDecay, yMuDecay, zMuDecay;
  G4double  pxMuDecay, pyMuDecay, pzMuDecay;
  G4double  rMuDecay, tMuDecay;
  G4double  xPosi, yPosi, zPosi;
  G4double  dxPosi, dyPosi, dzPosi, tPosi;
                     
  G4bool fNewEvent;
  G4bool fTargetStop;
  G4bool fNewMuInit, fNewMuEmit;

  G4int     fPrintModulo;                             
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
