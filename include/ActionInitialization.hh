/// based on examples/basic/B4/B4a/include/B4aActionInitialization.hh

#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class DetectorConstruction;
class HistoManager;

/// Action initialization class.
///

class ActionInitialization : public G4VUserActionInitialization
{
public:
  //ActionInitialization(DetectorConstruction*,HistoManager*);
  ActionInitialization(HistoManager*);
  virtual ~ActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

private:
  //DetectorConstruction* fDetConstruction;
  HistoManager* fHistoManager;
};

#endif
