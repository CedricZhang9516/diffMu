//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

#include <time.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(HistoManager* histo)
  :fHistoManager(histo)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  tickStart = clock();
  time(&rawtime);
  G4cout << G4endl;
  G4cout << "### Run " << aRun->GetRunID() << " started at " << ctime(&rawtime);

  //histograms
  //
  G4cout << "fHistoManager->book() start" <<G4endl;
  fHistoManager->book();
  G4cout << "fHistoManager->book() done" <<G4endl;
  fHistoManager->ZeroEventsPrimary();
  G4cout << "fHistoManager->ZeroEventsPrimary() done" <<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4cout << "start EndOfRunAction" <<G4endl;
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;

  //save histograms
  //
  fHistoManager->PrintStatistic();
  fHistoManager->save();
  tickEnd = clock() - tickStart;
  G4double telapsed = (float(tickEnd)/CLOCKS_PER_SEC);
  time(&rawtime);
  G4cout << G4endl;
  G4cout << "### Run " << aRun->GetRunID() << " ended at " << ctime(&rawtime);
  G4cout << "### Elapsed time " << telapsed << " seconds." << G4endl;
  G4cout << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
