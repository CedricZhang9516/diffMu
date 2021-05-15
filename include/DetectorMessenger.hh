//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
public:
  DetectorMessenger(DetectorConstruction* );
  ~DetectorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  DetectorConstruction* fDetConstruction;
    
  G4UIdirectory*             fDiffMuDir;
  G4UIdirectory*             fTgtDir;

  G4UIcmdWithADoubleAndUnit* fSizeXYCmd;

  G4UIcmdWithADoubleAndUnit* fMylarThickCmd;

  G4UIcmdWithADoubleAndUnit* fBCPsZCmd;

  G4UIcmdWithAString*        fBaseLyrMaterCmd;
  G4UIcmdWithADoubleAndUnit* fBaseLyrDensCmd;
  G4UIcmdWithADoubleAndUnit* fBaseLyrThickCmd;

  G4UIcmdWithAString*        fPerfLyrMaterCmd;
  G4UIcmdWithADoubleAndUnit* fPerfLyrDensCmd;
  G4UIcmdWithADoubleAndUnit* fPerfLyrThickCmd;
  G4UIcmdWithADoubleAndUnit* fPerfDiameterCmd;
  G4UIcmdWithADoubleAndUnit* fPerfPitchCmd;
  G4UIcmdWithADoubleAndUnit* fPerfDepthCmd;

  G4UIcmdWithADoubleAndUnit* fLaserRegionSzXCmd;
  G4UIcmdWithADoubleAndUnit* fLaserRegionSzYCmd;
  G4UIcmdWithADoubleAndUnit* fLaserRegionThCmd;
  G4UIcmdWithADoubleAndUnit* fLaserRegionSepZCmd;

  G4UIcmdWithAString*        fBeamstopMaterCmd;
  G4UIcmdWithADoubleAndUnit* fBeamstopDensCmd;
  G4UIcmdWithADoubleAndUnit* fBeamstopThickCmd;
 
  G4UIcmdWithoutParameter*   fUpdateCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

