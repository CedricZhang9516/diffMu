//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"

#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(HistoManager* histo)
  :fHistoManager(histo)
{
  fPrintModulo = 100;
  G4cout << "\n initialization of EventAction " << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{
  G4cout << "\n BeginOfEventAction " << G4endl;
  runNumber = fHistoManager->GetRunNumber();
  evtNb = evt->GetEventID();
  fHistoManager->SetEventNumber(evtNb);
  ResetEvent();
  if (evtNb%fPrintModulo == 0)
    G4cout << "\n---> Begin of event: " << evtNb << G4endl;

  //  This doesn't seem to work...
  //  memset(nhits, 0, sizeof(nhits[0]*nscints));
  //  memset(tdc, -1.0*CLHEP::nanosecond, sizeof(tdc[0][0]*nscints*MAX_NHITS));
  //  memset(edep, -1.0*CLHEP::keV, sizeof(edep[0][0]*nscints*MAX_NHITS));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
  //fill histograms
  //
  G4int ih = 0;
  fHistoManager->FillHisto(ih++, xInit/CLHEP::mm);
  fHistoManager->FillHisto(ih++, yInit/CLHEP::mm);
  fHistoManager->FillHisto(ih++, zInit/CLHEP::mm);
  fHistoManager->FillHisto(ih++, dxInit);
  fHistoManager->FillHisto(ih++, dyInit);
  fHistoManager->FillHisto(ih++, dzInit);
  fHistoManager->FillHisto(ih++, pxInit);
  fHistoManager->FillHisto(ih++, pyInit);
  fHistoManager->FillHisto(ih++, pzInit);

  if (tmuonDecay > 0.) {
    ih = 9;
    fHistoManager->FillHisto(ih++, xmuonDecay/CLHEP::mm);
    fHistoManager->FillHisto(ih++, ymuonDecay/CLHEP::mm);
    fHistoManager->FillHisto(ih++, zmuonDecay/CLHEP::mm);
    fHistoManager->FillHisto(ih++, tmuonDecay/CLHEP::microsecond, pxmuonDecay);
    fHistoManager->FillHisto(ih++, tmuonDecay/CLHEP::microsecond, pymuonDecay);
    fHistoManager->FillHisto(ih++, tmuonDecay/CLHEP::microsecond, pzmuonDecay);
    fHistoManager->FillHisto(ih++, tmuonDecay/CLHEP::microsecond);
    if (dxPosi > 0.) {
      fHistoManager->FillHisto(16, tmuonDecay/CLHEP::microsecond);
    }
    else if (dxPosi < 0.) {
      fHistoManager->FillHisto(17, tmuonDecay/CLHEP::microsecond);
    }
  }

  if ( !fNewMuInit) {             // Mu was formed
    ih = 18;
    fHistoManager->FillHisto(ih++, xMuInit/CLHEP::mm);
    fHistoManager->FillHisto(ih++, yMuInit/CLHEP::mm);
    fHistoManager->FillHisto(ih++, zMuInit/CLHEP::mm);
    fHistoManager->FillHisto(ih++, dxMuInit);
    fHistoManager->FillHisto(ih++, dyMuInit);
    fHistoManager->FillHisto(ih++, dzMuInit);
    fHistoManager->FillHisto(ih++, pxMuInit);
    fHistoManager->FillHisto(ih++, pyMuInit);
    fHistoManager->FillHisto(ih++, pzMuInit);
    fHistoManager->FillHisto(ih++, std::log10(ekMuInit/CLHEP::eV));
    fHistoManager->FillHisto(ih++, tMuInit/CLHEP::microsecond);
    if ( !fNewMuEmit ) {          // Mu was emitted
      ih = 29;
      fHistoManager->FillHisto(ih++, xMuEmit/CLHEP::mm);
      fHistoManager->FillHisto(ih++, yMuEmit/CLHEP::mm);
      fHistoManager->FillHisto(ih++, zMuEmit/CLHEP::mm);
      fHistoManager->FillHisto(ih++, dxMuEmit);
      fHistoManager->FillHisto(ih++, dyMuEmit);
      fHistoManager->FillHisto(ih++, dzMuEmit);
      fHistoManager->FillHisto(ih++, std::log10(ekMuEmit/CLHEP::eV));
      fHistoManager->FillHisto(ih++, tMuEmit/CLHEP::microsecond);
    }
    ih = 37;
    fHistoManager->FillHisto(ih++, xMuDecay/CLHEP::mm);
    fHistoManager->FillHisto(ih++, yMuDecay/CLHEP::mm);
    fHistoManager->FillHisto(ih++, zMuDecay/CLHEP::mm);
    fHistoManager->FillHisto(ih++, tMuDecay/CLHEP::microsecond, pxMuDecay);
    fHistoManager->FillHisto(ih++, tMuDecay/CLHEP::microsecond, pyMuDecay);
    fHistoManager->FillHisto(ih++, tMuDecay/CLHEP::microsecond, pzMuDecay);
    fHistoManager->FillHisto(ih++, rMuDecay/CLHEP::mm);
    fHistoManager->FillHisto(ih++, tMuDecay/CLHEP::microsecond);
    if ( !fNewMuEmit ) {          // Mu was emitted
      ih = 45;
      fHistoManager->FillHisto(ih++, zMuDecay/CLHEP::mm);
      fHistoManager->FillHisto(ih++, tMuDecay/CLHEP::microsecond);
    }
    if (dxPosi > 0.) {
      fHistoManager->FillHisto(47, tMuDecay/CLHEP::microsecond);
    }
    else if (dxPosi < 0.) {
      fHistoManager->FillHisto(48, tMuDecay/CLHEP::microsecond);
    }
  }

  //    fHistoManager->FillNtuple1(sumedep,wtavg,nscints);

  //  for (G4int iposi = 0; iposi < nscints; iposi++)
  //    {
  //      if (feScintInc[iposi])
  //	{
  //	  fHistoManager->FillHisto(7 + iposi, tHit[iposi]/CLHEP::microsecond);
  //	}
  //    }

  fHistoManager->Fill2DHisto(0, xInit/CLHEP::mm, yInit/CLHEP::mm);
  if (tmuonDecay > 0.) {
    fHistoManager->Fill2DHisto(1, zmuonDecay/CLHEP::mm, ymuonDecay/CLHEP::mm);
  }
  if (tMuDecay > 0.) {
    fHistoManager->Fill2DHisto(1, zMuDecay/CLHEP::mm, yMuDecay/CLHEP::mm);   // filled with muon and Mu
    fHistoManager->Fill2DHisto(2, xMuDecay/CLHEP::mm, yMuDecay/CLHEP::mm);
    fHistoManager->Fill2DHisto(3, zMuDecay/CLHEP::mm, yMuDecay/CLHEP::mm);
    fHistoManager->Fill2DHisto(4, zMuDecay/CLHEP::mm, tMuDecay/CLHEP::microsecond);
    tMuLRDis = std::min(tMuLROut,tMuDecay);
  }
  else {
    tMuLRDis = tMuLROut;
  }
  if (fTargetStop) {
    fHistoManager->Fill2DHisto(5, tTgtStop/CLHEP::microsecond, zTgtStop/CLHEP::mm);
  }
  // 2D hists 5,6 is filled in SteppingAction
  if ( (tMuLRIn < 20.*CLHEP::microsecond) && (tMuLRDis < 20.*CLHEP::microsecond) ) {
    fHistoManager->Fill2DHisto(6, tMuLRIn/CLHEP::microsecond, tMuLRDis/CLHEP::microsecond);
  }

  //fill ntuple(s)
  //
  //  fHistoManager->FillNtuple1(runNumber,evtNb,xInit/CLHEP::mm,yInit/CLHEP::mm,zInit/CLHEP::mm,
  //			     xmuDecay/CLHEP::mm,ymuDecay/CLHEP::mm,zmuDecay/CLHEP::cm,tmuDecay/CLHEP::microsecond,
  //			     nhits,tdc,edep,nscints);
}

void EventAction::SetIfNewEvent(G4bool lval)
{  //set flag to identify a new event for step evaluation
  fNewEvent = lval;
}

void EventAction::SetIfTargetStop(G4bool lval)
{  //set flag to identify muons stopping (E<Elow) in target volume
  fTargetStop = lval;
}

void EventAction::SetIfNewMuInit(G4bool lval)
{  //set flag to identify a new event for step evaluation
  fNewMuInit = lval;
}

void EventAction::SetIfNewMuEmit(G4bool lval)
{  //set flag to identify the first emission of Mu from aerogel to vacuum
  fNewMuEmit = lval;
}

void EventAction::ResetEvent()
{
  tmuonDecay = -1.*CLHEP::nanosecond;
  tMuLRIn = DBL_MAX;
  tMuLROut = DBL_MAX;
  tMuDecay = -1.*CLHEP::nanosecond;
  tPosi = -1.*CLHEP::nanosecond;
  fNewEvent = true;  // new event, flag for actions taken only once at beginning of event
  fTargetStop = false;
  fNewMuInit = true;   // new Mu created, flag for actions at initial Mu creation
  fNewMuEmit = true;   // initial Mu emission, flag for actions at initial Mu emission
}

void EventAction::Fill1D(G4int ihist, G4double xbin)
{
  fHistoManager->FillHisto(ihist, xbin);
}

void EventAction::Fill2D(G4int ihist, G4double xbin, G4double ybin)
{
  fHistoManager->Fill2DHisto(ihist, xbin, ybin);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
