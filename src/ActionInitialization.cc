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

  SetUserAction(new PrimaryGeneratorAction(fHistoManager));

  SetUserAction(new RunAction(fHistoManager));
  auto eventAction = new EventAction(fHistoManager);
  SetUserAction(eventAction);
  SetUserAction(new SteppingAction(eventAction));
  SetUserAction(new StackingAction());
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction(fHistoManager));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
