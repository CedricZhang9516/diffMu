/// based on examples/basic/B4/B4a/src/B4aActionInitialization.cc

#include "ActionInitialization.hh"
#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "StackingAction.hh"
//#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization(HistoManager* histo)
  : G4VUserActionInitialization()
{
  fHistoManager = histo;
  G4cout << "\n finish ActionInitialization \n" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
  //  auto gen_action = new PrimaryGeneratorAction(histo);
  //  auto run_action = new RunAction(histo);
  //  auto event_action = new EventAction(histo);
  //  auto stepping_action = new SteppingAction(event_action);
  //  auto stacking_action = new StackingAction();
  G4cout << "\n ActionInitialization### 1 \n" << G4endl;
  SetUserAction(new PrimaryGeneratorAction(fHistoManager));
  G4cout << "\n ActionInitialization### 2 \n" << G4endl;
  SetUserAction(new RunAction(fHistoManager));
  G4cout << "\n ActionInitialization### 3 \n" << G4endl;
  auto eventAction = new EventAction(fHistoManager);
  G4cout << "\n ActionInitialization### 4 \n" << G4endl;
  SetUserAction(eventAction);
  G4cout << "\n ActionInitialization### 5 \n" << G4endl;
  SetUserAction(new SteppingAction(eventAction));
  G4cout << "\n ActionInitialization### 6 \n" << G4endl;
  SetUserAction(new StackingAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction(fHistoManager));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
