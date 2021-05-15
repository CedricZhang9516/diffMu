//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger( DetectorConstruction* detConstruction)
  :fDetConstruction(detConstruction)
{ 
  fDiffMuDir = new G4UIdirectory("/DiffMu/");
  fDiffMuDir->SetGuidance("UI commands for aerogel target");
  
  fTgtDir = new G4UIdirectory("/DiffMu/tgt/");
  fTgtDir->SetGuidance("target control");

  fSizeXYCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setTargetSizeXY",this);
  fSizeXYCmd->SetGuidance("Set tranverse size of the target");
  fSizeXYCmd->SetParameterName("Size",false);
  fSizeXYCmd->SetRange("Size>=0.");
  fSizeXYCmd->SetUnitCategory("Length");    
  fSizeXYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fMylarThickCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setMylarThick",this);
  fMylarThickCmd->SetGuidance("Set thickness of the mylar");
  fMylarThickCmd->SetParameterName("Size",false);
  fMylarThickCmd->SetRange("Size>=0.");
  fMylarThickCmd->SetUnitCategory("Length");
  fMylarThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fBCPsZCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setBCPsZ",this);
  fBCPsZCmd->SetGuidance("Set z position of the BC muon scintillator");
  fBCPsZCmd->SetParameterName("Position",false);
  fBCPsZCmd->SetUnitCategory("Length");
  fBCPsZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fBaseLyrMaterCmd = new G4UIcmdWithAString("/DiffMu/tgt/setBaseLyrMat",this);
  fBaseLyrMaterCmd->SetGuidance("Select material of the BaseLyr");
  fBaseLyrMaterCmd->SetParameterName("choice",false);
  fBaseLyrMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fBaseLyrDensCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setBaseLyrDens",this);
  fBaseLyrDensCmd->SetGuidance("Set density of the BaseLyr");
  fBaseLyrDensCmd->SetParameterName("Density",false);
  fBaseLyrDensCmd->SetRange("Density>=0.");
  fBaseLyrDensCmd->SetUnitCategory("Volumic Mass");
  fBaseLyrDensCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fBaseLyrThickCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setBaseLyrThick",this);
  fBaseLyrThickCmd->SetGuidance("Set thickness of the BaseLyr");
  fBaseLyrThickCmd->SetParameterName("Size",false);
  fBaseLyrThickCmd->SetRange("Size>=0.");
  fBaseLyrThickCmd->SetUnitCategory("Length");
  fBaseLyrThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fPerfLyrMaterCmd = new G4UIcmdWithAString("/DiffMu/tgt/setPerfLyrMat",this);
  fPerfLyrMaterCmd->SetGuidance("Select material of the PerfLyr");
  fPerfLyrMaterCmd->SetParameterName("choice",false);
  fPerfLyrMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
         
  fPerfLyrDensCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setPerfLyrDens",this);
  fPerfLyrDensCmd->SetGuidance("Set density of the PerfLyr");
  fPerfLyrDensCmd->SetParameterName("Density",false);
  fPerfLyrDensCmd->SetRange("Density>=0.");
  fPerfLyrDensCmd->SetUnitCategory("Volumic Mass");  
  fPerfLyrDensCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fPerfLyrThickCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setPerfLyrThick",this);
  fPerfLyrThickCmd->SetGuidance("Set thickness of the PerfLyr");
  fPerfLyrThickCmd->SetParameterName("Size",false);
  fPerfLyrThickCmd->SetRange("Size>=0.");
  fPerfLyrThickCmd->SetUnitCategory("Length");  
  fPerfLyrThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fPerfDiameterCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setPerfDiameter",this);
  fPerfDiameterCmd->SetGuidance("Set diameter of holes in PerfLyr");
  fPerfDiameterCmd->SetParameterName("Size",false);
  fPerfDiameterCmd->SetRange("Size>=0.");
  fPerfDiameterCmd->SetUnitCategory("Length");    
  fPerfDiameterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fPerfPitchCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setPerfPitch",this);
  fPerfPitchCmd->SetGuidance("Set pitch of holes in PerfLyr");
  fPerfPitchCmd->SetParameterName("Size",false);
  fPerfPitchCmd->SetRange("Size>=0.");
  fPerfPitchCmd->SetUnitCategory("Length");    
  fPerfPitchCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fPerfDepthCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setPerfDepth",this);
  fPerfDepthCmd->SetGuidance("Set pitch of holes in PerfLyr");
  fPerfDepthCmd->SetParameterName("Size",false);
  fPerfDepthCmd->SetRange("Size>=0.");
  fPerfDepthCmd->SetUnitCategory("Length");    
  fPerfDepthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fLaserRegionSzXCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setLaserSzX",this);
  fLaserRegionSzXCmd->SetGuidance("Set size in X of laser irradiation region");
  fLaserRegionSzXCmd->SetParameterName("Size",false);
  fLaserRegionSzXCmd->SetRange("Size>=0.");
  fLaserRegionSzXCmd->SetUnitCategory("Length");    
  fLaserRegionSzXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fLaserRegionSzYCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setLaserSzY",this);
  fLaserRegionSzYCmd->SetGuidance("Set size in Y of laser irradiation region");
  fLaserRegionSzYCmd->SetParameterName("Size",false);
  fLaserRegionSzYCmd->SetRange("Size>=0.");
  fLaserRegionSzYCmd->SetUnitCategory("Length");    
  fLaserRegionSzYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fLaserRegionThCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setLaserTh",this);
  fLaserRegionThCmd->SetGuidance("Set thickness in beam direction of laser irradiation region");
  fLaserRegionThCmd->SetParameterName("Size",false);
  fLaserRegionThCmd->SetRange("Size>=0.");
  fLaserRegionThCmd->SetUnitCategory("Length");    
  fLaserRegionThCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fLaserRegionSepZCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setLaserSepZ",this);
  fLaserRegionSepZCmd->SetGuidance("Set separation of laser irradiation region from surface z=0");
  fLaserRegionSepZCmd->SetParameterName("Size",false);
  fLaserRegionSepZCmd->SetRange("Size>=0.");
  fLaserRegionSepZCmd->SetUnitCategory("Length");    
  fLaserRegionSepZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fBeamstopMaterCmd = new G4UIcmdWithAString("/DiffMu/tgt/setBeamstopMat",this);
  fBeamstopMaterCmd->SetGuidance("Select Material of the Beamstop");
  fBeamstopMaterCmd->SetParameterName("choice",false);
  fBeamstopMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fBeamstopDensCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setBeamstopDens",this);
  fBeamstopDensCmd->SetGuidance("Set density of the Beamstop");
  fBeamstopDensCmd->SetParameterName("Density",false);
  fBeamstopDensCmd->SetRange("Density>=0.");
  fBeamstopDensCmd->SetUnitCategory("Volumic Mass");  
  fBeamstopDensCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fBeamstopThickCmd = new G4UIcmdWithADoubleAndUnit("/DiffMu/tgt/setBeamstopThick",this);
  fBeamstopThickCmd->SetGuidance("Set thickness of the Beamstop");
  fBeamstopThickCmd->SetParameterName("Size",false);
  fBeamstopThickCmd->SetRange("Size>=0.");
  fBeamstopThickCmd->SetUnitCategory("Length");  
  fBeamstopThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fUpdateCmd = new G4UIcmdWithoutParameter("/DiffMu/tgt/update",this);
  fUpdateCmd->SetGuidance("Update target geometry.");
  fUpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  fUpdateCmd->SetGuidance("if you changed geometrical value(s).");
  fUpdateCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fSizeXYCmd;
  delete fBaseLyrMaterCmd; delete fPerfLyrMaterCmd; delete fBeamstopMaterCmd;
  delete fMylarThickCmd; delete fBCPsZCmd;
  delete fBaseLyrThickCmd; delete fPerfLyrThickCmd; delete fBeamstopThickCmd;
  delete fBaseLyrDensCmd; delete fPerfLyrDensCmd; delete fBeamstopDensCmd;
  delete fPerfDiameterCmd; delete fPerfPitchCmd; delete fPerfDepthCmd;
  delete fLaserRegionSzXCmd; delete fLaserRegionSzYCmd; delete fLaserRegionThCmd; delete fLaserRegionSepZCmd;
  delete fUpdateCmd;
  delete fTgtDir;
  delete fDiffMuDir;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == fSizeXYCmd )
    { fDetConstruction->SetTargetSzXY(fSizeXYCmd->GetNewDoubleValue(newValue));}
   
  if( command == fMylarThickCmd )
    { fDetConstruction->SetMylarTh(fMylarThickCmd
			   ->GetNewDoubleValue(newValue));}

  if( command == fBCPsZCmd )
    { fDetConstruction->SetBCPsZ(fBCPsZCmd
			   ->GetNewDoubleValue(newValue));}

  if( command == fBaseLyrMaterCmd )
    { fDetConstruction->SetBaseLyrMaterial(newValue);}
   
  if( command == fBaseLyrDensCmd )
    { fDetConstruction->SetBaseLyrDn(fBaseLyrDensCmd
			     ->GetNewDoubleValue(newValue));}  

  if( command == fBaseLyrThickCmd )
    { fDetConstruction->SetBaseLyrTh(fBaseLyrThickCmd
			     ->GetNewDoubleValue(newValue));}

  if( command == fPerfLyrMaterCmd )
    { fDetConstruction->SetPerfLyrMaterial(newValue);}
  
  if( command == fPerfLyrDensCmd )
    { fDetConstruction->SetPerfLyrDn(fPerfLyrDensCmd->GetNewDoubleValue(newValue));}
   
  if( command == fPerfLyrThickCmd )
    { fDetConstruction->SetPerfLyrTh(fPerfLyrThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == fPerfDiameterCmd )
    { fDetConstruction->SetPerfDiameter(fPerfDiameterCmd->GetNewDoubleValue(newValue));}
   
  if( command == fPerfPitchCmd )
    { fDetConstruction->SetPerfPitch(fPerfPitchCmd->GetNewDoubleValue(newValue));}
   
  if( command == fPerfDepthCmd )
    { fDetConstruction->SetPerfDepth(fPerfDepthCmd->GetNewDoubleValue(newValue));}
   
  if( command == fLaserRegionSzXCmd )
    { fDetConstruction->SetLaserRegionSzX(fLaserRegionSzXCmd->GetNewDoubleValue(newValue));}
   
  if( command == fLaserRegionSzYCmd )
    { fDetConstruction->SetLaserRegionSzY(fLaserRegionSzYCmd->GetNewDoubleValue(newValue));}
   
  if( command == fLaserRegionThCmd )
    { fDetConstruction->SetLaserRegionTh(fLaserRegionThCmd->GetNewDoubleValue(newValue));}
   
  if( command == fLaserRegionSepZCmd )
    { fDetConstruction->SetLaserRegionSepZ(fLaserRegionSepZCmd->GetNewDoubleValue(newValue));}
   
  if( command == fBeamstopMaterCmd )
    { fDetConstruction->SetBeamstopMaterial(newValue);}
  
  if( command == fBeamstopDensCmd )
    { fDetConstruction->SetBeamstopDn(fBeamstopDensCmd->GetNewDoubleValue(newValue));}
   
  if( command == fBeamstopThickCmd )
    { fDetConstruction->SetBeamstopTh(fBeamstopThickCmd->GetNewDoubleValue(newValue));}
   

  if( command == fUpdateCmd )
    { fDetConstruction->UpdateGeometry(); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
