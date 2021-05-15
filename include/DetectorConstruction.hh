
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4Box;
class G4Tubs;
class G4SubtractionSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

class G4UserLimits;

class Field;

class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  DetectorConstruction();
  virtual ~DetectorConstruction();

public:
     
  void SetMylarMaterial (G4String);     
  void SetMylarTh(G4double);     

  void SetBCPsZ(G4double);

  void SetTargetSzXY(G4double);          
     
  void SetBaseLyrMaterial (G4String);     
  void SetBaseLyrTh(G4double);     
  void SetBaseLyrDn(G4double);     

  void SetPerfLyrMaterial (G4String);     
  void SetPerfLyrTh(G4double);
  void SetPerfLyrDn(G4double);
     
  void SetPerfDiameter(G4double);
  void SetPerfPitch(G4double);
  void SetPerfDepth(G4double);

  void SetLaserRegionSzX(G4double);
  void SetLaserRegionSzY(G4double);
  void SetLaserRegionTh(G4double);
  void SetLaserRegionSepZ(G4double);
  
  void SetBeamstopMaterial (G4String);     
  void SetBeamstopTh(G4double);
  void SetBeamstopDn(G4double);
     
     
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();

  void UpdateGeometry();
     
public:
  
  void PrintTargetParameters(); 
                    
  G4double GetWorldSzZ()           {return fWorldSzZ;}; 
  G4double GetWorldSzXY()          {return fWorldSzXY;};
     
  G4Material* GetMylarMaterial()  {return fMylarMaterial;};
  G4double    GetMylarTh() {return fMylarTh;};      
     
  G4double GetBCSzXY()          {return fBCSzXY;};
  G4double GetBCTh()       {return fBCTh;}; 
  G4double GetBCPsZ()       {return fBCPsZ;}; 
      
  G4double GetTargetSzXY()          {return fTargetSzXY;};
  G4double GetTargetTh()       {return fTargetTh;}; 
  G4double GetTargetPsZ()       {return fTargetPsZ;}; 
      
  G4Material* GetBaseLyrMaterial()  {return fBaseLyrMaterial;};
  G4double    GetBaseLyrTh() {return fBaseLyrTh;};      
  G4double    GetBaseLyrDn() {return fBaseLyrDn;};      
     
  G4Material* GetPerfLyrMaterial()       {return fPerfLyrMaterial;};
  G4double    GetPerfLyrTh()      {return fPerfLyrTh;};
  G4double    GetPerfLyrDn()      {return fPerfLyrDn;};
  G4double    GetPerfDiameter()      {return fPerfDiameter;};
  G4double    GetPerfPitch()      {return fPerfXPitch;};
  G4double    GetPerfDepth()      {return fPerfDepth;};
     
  G4double    GetLaserRegionSzX()      {return fLaserRegionSzX;};
  G4double    GetLaserRegionSzY()      {return fLaserRegionSzY;};
  G4double    GetLaserRegionTh()      {return fLaserRegionTh;};
  G4double    GetLaserRegionSepZ()      {return fLaserRegionSepZ;};
     
  G4Material* GetBeamstopMaterial()       {return fBeamstopMaterial;};
  G4double    GetBeamstopTh()      {return fBeamstopTh;};
  G4double    GetBeamstopDn()      {return fBeamstopDn;};
  G4double    GetBeamstopPsZ()      {return fBeamstopPsZ;};
     
  const G4VPhysicalVolume* GetphysiWorld() {return fPhysiWorld;};           
  const G4VPhysicalVolume* GetMylar()   {return fPhysiMylar;};
  const G4VPhysicalVolume* GetBaseLyr()   {return fPhysiBaseLyr;};
  const G4VPhysicalVolume* GetPerfLyr()        {return fPhysiPerfLyr;};
  const G4VPhysicalVolume* GetBeamstop()        {return fPhysiBeamstop;};

  Field* field;
                 
private:
     
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4int nel;  // nel=number of elements;
  G4int natoms; // natoms=number of atoms
  G4double density; 
  G4bool fCheckOverlaps;

  G4Material*        fDefaultMaterial;
  G4double           fWorldSzXY;
  G4double           fWorldSzZ;
            
  G4Material*        fSSMaterial;
  G4Material*        fMylarMaterial;

  G4double           fVacsysOSzXY;
  G4double           fVacsysTh;
  G4double           fVacsysISzXY;
  G4double           fVacsysDn;
  G4double           fVacsysIPsZ;
  G4double           fVacsysSzZ;

  //     G4Material*        fCollMaterial;
  //     G4double           fColl1OSzD;
  //     G4double           fColl1ISzD;
  //     G4double           fColl1Th;
  //     G4double           fColl1PsZ;
  //     G4double           fColl2OSzD;
  //     G4double           fColl2ISzD;
  //     G4double           fColl2Th;
  //     G4double           fColl2PsZ;
     
  G4double           fMylarSzD;
  G4double           fMylarTh;
  G4double           fMylarPsZ;
     
  G4double           fMylarOFlangeSzD;
  G4double           fMylarIFlangeSzD;
  G4double           fMylarFlangeTh;
  G4double           fMylarFlangePsZ;
     
  G4Material*        fVetoFrameMaterial;
  G4double           fVetoOFrameSzXY;
  G4double           fVetoIFrameSzD;
  G4double           fVetoFrameTh;
  G4double           fVetoFramePsZ;
     
  G4Material*        fBCMaterial;
  G4double           fBCSzXY;
  G4double           fBCTh;
  G4double           fBCPsZ;
     
  G4Material*        fBCFrameMaterial;
  G4double           fBCOFrameSzXY;
  G4double           fBCIFrameSzXY;
  G4double           fBCFrameTh;
  G4double           fBCFramePsZ;
  G4double           fBCFrameWrapTh;
     
  G4double           fTargetSzXY;
  G4double           fTargetTh;
  G4double           fTargetPsZ;
            
  G4Material*        fBaseLyrMaterial;
  G4double           fBaseLyrTh;
  G4double           fBaseLyrDn;
     
  G4Material*        fPerfLyrMaterial;
  G4double           fPerfLyrTh;
  G4double           fPerfLyrDn;
  G4double           fPerfLyrFrameSz;

  G4int              fnHoles;
  G4int              fnHX;
  G4int              fnHY;
  G4double           fPerfDiameter;
  G4double           fPerfDepth;
  G4double           fPerfXPitch;
  G4double           fPerfYPitch;
  G4double           fPerfStartX;
  G4double           fPerfStartY;
  G4double           fPerfXoffset;
     
  G4double           fLaserRegionSzX;
  G4double           fLaserRegionSzY;
  G4double           fLaserRegionTh;
  G4double           fLaserRegionSepZ;
  G4double           fLaserRegionPsZ;

  G4Material*        fBeamstopMaterial;
  G4double           fBeamstopSzXY;
  G4double           fBeamstopTh;
  G4double           fBeamstopDn;
  G4double           fBeamstopPsZ;

  G4Box*             fSolidWorld;    //pointer to the solid World 
  G4LogicalVolume*   fLogicWorld;    //pointer to the logical World
  G4VPhysicalVolume* fPhysiWorld;    //pointer to the physical World

  G4Tubs*            fSolidMylar;    //pointer to the solid Mylar 
  G4LogicalVolume*   fLogicMylar;    //pointer to the logical Mylar
  G4VPhysicalVolume* fPhysiMylar;    //pointer to the physical Mylar
         
  G4Tubs*            fSolidMylarFlange;    //pointer to the solid MylarFlange 
  G4LogicalVolume*   fLogicMylarFlange;    //pointer to the logical MylarFlange
  G4VPhysicalVolume* fPhysiMylarFlange;    //pointer to the physical MylarFlange
         
  G4Box*             fSolidVetoOFrame;    //pointer to the solid VetoOFrame 
  G4Tubs*            fSolidVetoIFrame;    //pointer to the solid VetoIFrame 
  G4SubtractionSolid*  fSolidVetoFrame;    //pointer to the solid VetoFrame 
  G4LogicalVolume*   fLogicVetoFrame;    //pointer to the logical VetoFrame
  G4VPhysicalVolume* fPhysiVetoFrame;    //pointer to the physical VetoFrame
         
  G4Box*             fSolidVacsysO;    //pointer to the solid VacsysO 
  G4Box*             fSolidVacsysI;    //pointer to the solid VacsysI 
  G4SubtractionSolid*  fSolidVacsys;    //pointer to the solid Vacsys 
  G4LogicalVolume*   fLogicVacsys;    //pointer to the logical Vacsys
  G4VPhysicalVolume* fPhysiVacsys;    //pointer to the physical Vacsys
         
  G4Box*             fSolidBC;    //pointer to the solid BC 
  G4LogicalVolume*   fLogicBC;    //pointer to the logical BC
  G4VPhysicalVolume* fPhysiBC;    //pointer to the physical BC
         
  G4Box*             fSolidBCOFrame;    //pointer to the solid BCOFrame 
  G4Box*             fSolidBCIFrame;    //pointer to the solid BCIFrame 
  G4SubtractionSolid*  fSolidBCFrame;    //pointer to the solid BCFrame 
  G4LogicalVolume*   fLogicBCFrame;    //pointer to the logical BCFrame
  G4VPhysicalVolume* fPhysiBCFrame;    //pointer to the physical BCFrame
         
  G4Box*             fSolidBCFrameWrap;    //pointer to the solid BCIFrame mylar wrap
  G4LogicalVolume*   fLogicBCFrameWrap;    //pointer to the logical BCFrame mylar wrap
  G4VPhysicalVolume* fPhysiBCFrameWrap;    //pointer to the physical BCFrame mylar wrap
         
  G4Box*             fSolidTarget;    //pointer to the solid Target 
  G4LogicalVolume*   fLogicTarget;    //pointer to the logical Target
  G4VPhysicalVolume* fPhysiTarget;    //pointer to the physical Target
     
  G4Box*             fSolidBaseLyr; //pointer to the solid BaseLyr
  G4LogicalVolume*   fLogicBaseLyr; //pointer to the logical BaseLyr
  G4VPhysicalVolume* fPhysiBaseLyr; //pointer to the physical BaseLyr
     
  G4Box*             fSolidPerfLyr;      //pointer to the solid PerfLyr
  G4LogicalVolume*   fLogicPerfLyr;      //pointer to the logical PerfLyr
  G4VPhysicalVolume* fPhysiPerfLyr;      //pointer to the physical PerfLyr
     
  G4Tubs*            fSolidPerfHole;      //pointer to the solid PerfHole
  G4LogicalVolume*   fLogicPerfHole;      //pointer to the logical PerfHole (parameterised)
     
  G4Box*             fSolidLaserRegion;    //pointer to the solid LaserRegion 
  G4LogicalVolume*   fLogicLaserRegion;    //pointer to the logical LaserRegion
  G4VPhysicalVolume* fPhysiLaserRegion;    //pointer to the physical LaserRegion
         
  G4Box*             fSolidBeamstop;    //pointer to the solid Beamstop 
  G4LogicalVolume*   fLogicBeamstop;    //pointer to the logical Beamstop
  G4VPhysicalVolume* fPhysiBeamstop;    //pointer to the physical Beamstop
         
  //     G4Tubs*            fSolidColl1;    //pointer to the solid Coll1 
  //     G4LogicalVolume*   fLogicColl1;    //pointer to the logical Coll1
  //     G4VPhysicalVolume* fPhysiColl1;    //pointer to the physical Coll1
  //         
  //     G4Tubs*            fSolidColl2;    //pointer to the solid Coll2 
  //     G4LogicalVolume*   fLogicColl2;    //pointer to the logical Coll2
  //     G4VPhysicalVolume* fPhysiColl2;    //pointer to the physical Coll2
         
  G4UserLimits* fStepLimit;            // pointer to user step limits

  DetectorMessenger* fDetectorMessenger;  //pointer to the Messenger
  static G4ThreadLocal Field* fField;
      
private:
    
  void DefineMaterials();
  void ComputeTargetParameters();
  G4VPhysicalVolume* ConstructTarget();     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif

