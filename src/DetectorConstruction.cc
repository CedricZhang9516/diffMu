//...oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "HoleParameterisation.hh"
#include "Field.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Transportation.hh"
//#include "G4EqEMFieldWithSpin.hh"
#include "G4Mag_SpinEqRhs.hh"
#include "G4ClassicalRK4.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4ChordFinder.hh"
#include "G4PropagatorInField.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  :fCheckOverlaps(false),
   fDefaultMaterial(0),fBaseLyrMaterial(0),fPerfLyrMaterial(0),fBeamstopMaterial(0),
   fSolidWorld(0),fLogicWorld(0),fPhysiWorld(0),
   fSolidTarget(0),fLogicTarget(0),fPhysiTarget(0),
   fSolidBaseLyr(0),fLogicBaseLyr(0),fPhysiBaseLyr(0),
   fSolidPerfLyr(0),fLogicPerfLyr(0),fPhysiPerfLyr(0),
   fSolidBeamstop(0),fLogicBeamstop(0),fPhysiBeamstop(0),
   fStepLimit(NULL)
{
  // default parameter values of geometry
  fMylarSzD  = 32.*CLHEP::mm;
  fMylarTh  = 0.05*CLHEP::mm;   // 50 um est.
  fMylarPsZ  = -65.*CLHEP::mm;

  fMylarOFlangeSzD  = 70.*CLHEP::mm;
  fMylarIFlangeSzD  = fMylarSzD;
  fMylarFlangeTh  = 25.*CLHEP::mm;
  //fMylarFlangeTh  = 0.*CLHEP::mm;
  fMylarFlangePsZ  = fMylarPsZ - 2.5*CLHEP::mm;

  fVetoOFrameSzXY  = 50.*CLHEP::mm;
  fVetoIFrameSzD  = 10.*CLHEP::mm;
  fVetoFrameTh  = 3.*CLHEP::mm;
  fVetoFramePsZ  = -51.5*CLHEP::mm;

  fBCSzXY  = 18.*CLHEP::mm;  // 2017 run, new BC
  fBCTh  = 0.330*CLHEP::mm;    // 0.0341(3) g/cm^2, density 1.032
  fBCPsZ  = -12.*CLHEP::mm;

  fBCOFrameSzXY  = 26.*CLHEP::mm;
  fBCIFrameSzXY  = fBCSzXY;
  fBCFrameTh  = 3.*CLHEP::mm;
  fBCFramePsZ  = fBCPsZ;
  fBCFrameWrapTh  = 0.006*CLHEP::mm;

  // default target values, to be changed in input files or parameters
  fTargetSzXY  = 50.*CLHEP::mm;  // 2017 nominal

  fPerfLyrTh   = 8.8*CLHEP::mm;  // 2017 Sample 10, MSR12-2d-T2, Y=16,19 ppt
  fPerfLyrDn   = 23.9*CLHEP::mg/CLHEP::cm3;
  fPerfLyrFrameSz = 10.*CLHEP::mm;

  fBaseLyrTh   = 0.*CLHEP::mm;   // not used if zero
  fBaseLyrDn   = 30.*CLHEP::mg/CLHEP::cm3;

  //fPerfDiameter = 0.*CLHEP::mm;   // zero to ignore perforations, uniform

  fPerfDiameter = 0.100*CLHEP::mm;  //2017 Sample 10, MSR12-2d-T2, Y=16,19 ppt
  fPerfDepth =1.5*CLHEP::mm;  
  fPerfXPitch = 0.2*CLHEP::mm;

  //  fWorldSzXY   = 2.*120.*CLHEP::mm;  //240
  //  fWorldSzZ    = 2.*200.*CLHEP::mm;
  fWorldSzXY   = 2.*100.*CLHEP::mm;  //200
  fWorldSzZ    = 2.*100.*CLHEP::mm;

  fVacsysOSzXY  = fWorldSzXY - 2.*CLHEP::mm;
  fVacsysTh     = 2.*CLHEP::mm;
  fVacsysISzXY  = fVacsysOSzXY - 2.*fVacsysTh;
  //  fVacsysIPsZ  = -1.*fVacsysTh;
  fVacsysIPsZ  = 0.*CLHEP::mm;
  fVacsysSzZ   = fWorldSzZ - 1.*CLHEP::mm;
  
  fLaserRegionSzX  = 60*CLHEP::mm;
  fLaserRegionSzY  = 60*CLHEP::mm;
  fLaserRegionTh   = 5.*CLHEP::mm;
  fLaserRegionSepZ   = 1.*CLHEP::mm;
  fLaserRegionPsZ   = fLaserRegionSepZ + 0.5*fLaserRegionTh;

  fBeamstopSzXY  = fVacsysISzXY - 2.*fVacsysTh;
  fBeamstopTh   = 10.*CLHEP::mm;
  fBeamstopPsZ   = 80.*CLHEP::mm + 0.5*fBeamstopTh;
  fBeamstopDn   = 2200.*CLHEP::mg/CLHEP::cm3;

  //  fColl1OSzD  = 60.0*CLHEP::mm;
  //  fColl1ISzD  = 16.0*CLHEP::mm;
  //  fColl1Th    = 50.0*CLHEP::mm;
  //  fColl1PsZ   = -155.0*CLHEP::mm;
  //
  //  fColl2OSzD  = 60.0*CLHEP::mm;
  //  fColl2ISzD  = 12.0*CLHEP::mm;
  //  fColl2Th    = 50.0*CLHEP::mm;
  //  fColl2PsZ   = -105.0*CLHEP::mm;

  // materials
  DefineMaterials();
  SetPerfLyrMaterial("Aerogel1");
  SetBaseLyrMaterial("Aerogel2");
  SetBeamstopMaterial("BeamstopMat");
  
  // create commands for interactive definition of the target
  fDetectorMessenger = new DetectorMessenger(this);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ 
  delete fDetectorMessenger;
  if ( fField ) delete fField;
  delete fStepLimit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  return ConstructTarget();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
  // use G4-NIST materials data base
  //
  G4NistManager* man = G4NistManager::Instance();
  fDefaultMaterial = man->FindOrBuildMaterial("G4_Galactic");

  // Aerogel
  G4Element* Si = man->FindOrBuildElement("Si");
  G4Element* O  = man->FindOrBuildElement("O");
  fPerfLyrMaterial = new G4Material("Aerogel1", density= fPerfLyrDn, nel=2);
  fPerfLyrMaterial->AddElement(Si, natoms=1);
  fPerfLyrMaterial->AddElement(O, natoms=2);
  fBaseLyrMaterial = new G4Material("Aerogel2", density= fBaseLyrDn, nel=2);
  fBaseLyrMaterial->AddElement(Si, natoms=1);
  fBaseLyrMaterial->AddElement(O, natoms=2);

  // Stainless steel
  G4Element* C  = man->FindOrBuildElement("C");
  G4Element* Co = man->FindOrBuildElement("Co");
  G4Element* Fe = man->FindOrBuildElement("Fe");
  fSSMaterial = new G4Material("SSteel", density=7.7*CLHEP::g/CLHEP::cm3, nel=3);
  fSSMaterial->AddElement(C,  0.04);
  fSSMaterial->AddElement(Fe, 0.88);
  fSSMaterial->AddElement(Co, 0.08);

  fBeamstopMaterial = new G4Material("BeamstopMat", density= fBeamstopDn, nel=2);
  fBeamstopMaterial->AddElement(Si, natoms=1);
  fBeamstopMaterial->AddElement(O, natoms=2);

  fMylarMaterial = man->FindOrBuildMaterial("G4_MYLAR");
  fVetoFrameMaterial = man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  fBCMaterial = man->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  fBCFrameMaterial = man->FindOrBuildMaterial("G4_PLEXIGLASS");
  //fCollMaterial = man->FindOrBuildMaterial("G4_Pb");

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructTarget()
{

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // complete the Target parameters definition
  ComputeTargetParameters();

  //     
  // World
  //
  fSolidWorld = new G4Box("World",                                //its name
			  fWorldSzXY/2,fWorldSzXY/2,fWorldSzZ/2);  //its size
                         
  fLogicWorld = new G4LogicalVolume(fSolidWorld,            //its solid
				    fDefaultMaterial,        //its material
				    "World");                //its name
                                   
  fPhysiWorld = new G4PVPlacement(0,                        //no rotation
				  G4ThreeVector(),           //at (0,0,0)
				  fLogicWorld,               //its logical volume                                 
				  "World",                   //its name
				  0,                         //its mother  volume
				  false,                     //no boolean operation
				  0,                        //copy number
				  fCheckOverlaps);
  
  //                                 
  // Vacsys
  //
  fSolidVacsysO=0; fSolidVacsysI=0; fSolidVacsys=0; fLogicVacsys=0; fPhysiVacsys=0;

  fSolidVacsysO = new G4Box("VacsysO",                        //its name
			    fVacsysOSzXY/2,fVacsysOSzXY/2,fVacsysSzZ/2); //size
                   
  fSolidVacsysI = new G4Box("VacsysI",                        //its name
			    fVacsysISzXY/2,fVacsysISzXY/2,fVacsysSzZ/2); //size

  G4RotationMatrix NullRotation;
  G4ThreeVector VacsysItrans(0,0,fVacsysIPsZ);
  G4Transform3D VacsysTransform(NullRotation,VacsysItrans);
                   
  fSolidVacsys = new G4SubtractionSolid("Vacsys",fSolidVacsysO,fSolidVacsysI,VacsysTransform);

  fLogicVacsys = new G4LogicalVolume(fSolidVacsys,        //its solid
				     fSSMaterial,    //its material
				     "Vacsys");            //its name

  fPhysiVacsys = new G4PVPlacement(0,                  //no rotation
				   G4ThreeVector(0,0,0),       //
				   fLogicVacsys,           //its logical volume                                     
				   "Vacsys",               //its name
				   fLogicWorld,           //its mother  volume
				   false,                 //no boolean operation
				   0);                    //copy number     
                                     
  //                                 
  // Mylar
  //
  if (fMylarTh > 0.) { 
    fSolidMylar=0; fLogicMylar=0; fPhysiMylar=0;

    fSolidMylar = new G4Tubs("Mylar",                        //its name
			     0.,fMylarSzD/2,fMylarTh/2,0.,twopi); //size
                   
    fLogicMylar = new G4LogicalVolume(fSolidMylar,        //its solid
				      fMylarMaterial,    //its material
				      "Mylar");            //its name

    fPhysiMylar = new G4PVPlacement(0,                  //no rotation
				    G4ThreeVector(0,0,fMylarPsZ),       //
				    fLogicMylar,           //its logical volume                                     
				    "Mylar",               //its name
				    fLogicWorld,           //its mother  volume
				    false,                 //no boolean operation
				    0);  //copy number     
  }
  
  //                                 
  // MylarFlange
  //
  fSolidMylarFlange=0; fLogicMylarFlange=0; fPhysiMylarFlange=0;

  if (fMylarFlangeTh > 0.) {
    fSolidMylarFlange = new G4Tubs("MylarFlange",                        //its name
				   fMylarIFlangeSzD/2,fMylarOFlangeSzD/2,fMylarFlangeTh/2,0.,twopi); //size
                   
    fLogicMylarFlange = new G4LogicalVolume(fSolidMylarFlange,        //its solid
					    fSSMaterial,    //its material
					    "MylarFlange");            //its name

    fPhysiMylarFlange = new G4PVPlacement(0,                  //no rotation
					  G4ThreeVector(0,0,fMylarFlangePsZ),       //
					  fLogicMylarFlange,           //its logical volume                                     
					  "MylarFlange",               //its name
					  fLogicWorld,           //its mother  volume
					  false,                 //no boolean operation
					  0);                    //copy number
  }     
                                     
  //                                 
  // VetoFrame
  //
  fSolidVetoOFrame=0; fSolidVetoIFrame=0; fSolidVetoFrame=0; fLogicVetoFrame=0; fPhysiVetoFrame=0;

  fSolidVetoOFrame = new G4Box("VetoOFrame",                        //its name
			       fVetoOFrameSzXY/2,fVetoOFrameSzXY/2,fVetoFrameTh/2); //size
                   
  fSolidVetoIFrame = new G4Tubs("VetoIFrame",                        //its name
				0.,fVetoIFrameSzD/2,fVetoFrameTh/2+0.005*CLHEP::mm,0.,twopi); //size
                   
  fSolidVetoFrame = new G4SubtractionSolid("VetoFrame",fSolidVetoOFrame,fSolidVetoIFrame);


  fLogicVetoFrame = new G4LogicalVolume(fSolidVetoFrame,        //its solid
					fVetoFrameMaterial,    //its material
					"VetoFrame");            //its name

  fPhysiVetoFrame = new G4PVPlacement(0,                  //no rotation
				      G4ThreeVector(0,0,fVetoFramePsZ),       //
				      fLogicVetoFrame,           //its logical volume                                     
				      "VetoFrame",               //its name
				      fLogicWorld,           //its mother  volume
				      false,                 //no boolean operation
				      0);                    //copy number     
                                     
  //                                 
  // BC
  //
  if (fBCTh > 0.) { 
    fSolidBC=0; fLogicBC=0; fPhysiBC=0;

    fSolidBC = new G4Box("BC",                        //its name
			 fBCSzXY/2,fBCSzXY/2,fBCTh/2); //size
                   
    fLogicBC = new G4LogicalVolume(fSolidBC,        //its solid
				   fBCMaterial,    //its material
				   "BC");            //its name

    fPhysiBC = new G4PVPlacement(0,                  //no rotation
				 G4ThreeVector(0,0,fBCPsZ),       //
				 fLogicBC,           //its logical volume
				 "BC",          //its name
				 fLogicWorld,   //its mother volume
				 false,         //no boolean operation
				 0);            //copy number 
  }
    
  //                                 
  // BCFrame
  //
  fSolidBCOFrame=0; fSolidBCIFrame=0; fSolidBCFrame=0; fLogicBCFrame=0; fPhysiBCFrame=0;

  fSolidBCOFrame = new G4Box("BCOFrame",                        //its name
			     fBCOFrameSzXY/2,fBCOFrameSzXY/2,fBCFrameTh/2); //size
                   
  fSolidBCIFrame = new G4Box("BCIFrame",                        //its name
			     fBCIFrameSzXY/2,fBCIFrameSzXY/2,fBCFrameTh/2+0.005*CLHEP::mm); //size
                   
  fSolidBCFrame = new G4SubtractionSolid("BCFrame",fSolidBCOFrame,fSolidBCIFrame);


  fLogicBCFrame = new G4LogicalVolume(fSolidBCFrame,        //its solid
				      fBCFrameMaterial,    //its material
				      "BCFrame");            //its name

  fPhysiBCFrame = new G4PVPlacement(0,                  //no rotation
				    G4ThreeVector(0,0,fBCFramePsZ),       //
				    fLogicBCFrame,           //its logical volume                                     
				    "BCFrame",               //its name
				    fLogicWorld,           //its mother  volume
				    false,                 //no boolean operation
				    0);                    //copy number     
                                     
  //                                 
  // BCFrameWrap
  //
  if (fBCFrameWrapTh > 0.) { 
    fSolidBCFrameWrap = new G4Box("BCFrameWrap",
				  fBCOFrameSzXY/2,fBCOFrameSzXY/2,fBCFrameWrapTh); //size
    fLogicBCFrameWrap = new G4LogicalVolume(fSolidBCFrameWrap,  //its solid
					    fMylarMaterial,    //its material
					    "BCFrameWrap");    //its name

    fPhysiBCFrameWrap = new G4PVPlacement(0,     //no rotation
					  G4ThreeVector(0,0,fBCFramePsZ-fBCFrameTh/2-fBCFrameWrapTh/2),
					  fLogicBCFrameWrap, //its logical volume              
					  "BCFrameWrap", //its name
					  fLogicWorld, //its mother  volume
					  false,      //no boolean operation
					  0);     //copy number     
                                     
    fPhysiBCFrameWrap = new G4PVPlacement(0,                  //no rotation
					  G4ThreeVector(0,0,fBCFramePsZ+fBCFrameTh/2+fBCFrameWrapTh/2),
					  fLogicBCFrameWrap, //its logical volume
					  "BCFrameWrap",  //its name
					  fLogicWorld,  //its mother  volume
					  false,  //no boolean operation
					  1);     //copy number
  }                                     

  //                               
  // Target
  //  
  fSolidTarget=0; fLogicTarget=0; fPhysiTarget=0;
  
  if (fTargetTh > 0.) { 
    fSolidTarget = new G4Box("Target",                //its name
			     fTargetSzXY/2,fTargetSzXY/2,fTargetTh/2);//size
                                 
    fLogicTarget = new G4LogicalVolume(fSolidTarget,        //its solid
				       fDefaultMaterial,   //its material
				       "Target");     //its name
                                           
    fPhysiTarget = new G4PVPlacement(0,                    //no rotation
				     G4ThreeVector(0.,0.,-fTargetTh/2),       //
				     fLogicTarget,           //its logical volume
				     "Target",         //its name
				     fLogicWorld,           //its mother  volume
				     false,                 //no boolean operation
				     0);                    //copy number
  }

  //                                 
  // LaserRegion
  //
  if (fLaserRegionTh > 0.) {
    fSolidLaserRegion=0; fLogicLaserRegion=0; fPhysiLaserRegion=0;

    fSolidLaserRegion = new G4Box("LaserRegion",                        //its name
				  fLaserRegionSzX/2,fLaserRegionSzY/2,fLaserRegionTh/2); //size
                   
    fLogicLaserRegion = new G4LogicalVolume(fSolidLaserRegion,        //its solid
					    fDefaultMaterial,    //its material
					    "LaserRegion");            //its name

    fPhysiLaserRegion = new G4PVPlacement(0,                  //no rotation
					  G4ThreeVector(0,0,fLaserRegionPsZ),       //
					  fLogicLaserRegion,           //its logical volume                                     
					  "LaserRegion",               //its name
					  fLogicWorld,           //its mother  volume
					  false,                 //no boolean operation
					  0);                    //copy number
  }
  
  //                                 
  // Beamstop
  //
  fSolidBeamstop=0; fLogicBeamstop=0; fPhysiBeamstop=0;

  fSolidBeamstop = new G4Box("Beamstop",                        //its name
			     fBeamstopSzXY/2,fBeamstopSzXY/2,fBeamstopTh/2); //size
                   
  fLogicBeamstop = new G4LogicalVolume(fSolidBeamstop,        //its solid
				       fBeamstopMaterial,    //its material
				       "Beamstop");            //its name

  fPhysiBeamstop = new G4PVPlacement(0,                  //no rotation
				     G4ThreeVector(0,0,fBeamstopPsZ),       //
				     fLogicBeamstop,           //its logical volume                                     
				     "Beamstop",               //its name
				     fLogicWorld,           //its mother  volume
				     false,                 //no boolean operation
				     0);                    //copy number     
                                     
  
  //  //                                 
  //  // Coll1
  //  //
  //  fSolidColl1=0; fLogicColl1=0; fPhysiColl1=0;
  //
  //  if (fColl1Th > 0.) 
  //    {fSolidColl1 = new G4Tubs("Coll1",                        //its name
  //				  fColl1ISzD/2,fColl1OSzD/2,fColl1Th/2,0.,twopi); //size
  //                   
  //    fLogicColl1 = new G4LogicalVolume(fSolidColl1,        //its solid
  //                                  fCollMaterial,    //its material
  //                                  "Coll1");            //its name
  //
  //    fPhysiColl1 = new G4PVPlacement(0,                  //no rotation
  //				  G4ThreeVector(0,0,fColl1PsZ),       //
  //                                  fLogicColl1,           //its logical volume                                     
  //                                  "Coll1",               //its name
  //                                  fLogicWorld,           //its mother  volume
  //                                  false,                 //no boolean operation
  //                                  0);                    //copy number
  //    }     
  //                                     
  //  //                                 
  //  // Coll2
  //  //
  //  fSolidColl2=0; fLogicColl2=0; fPhysiColl2=0;
  //
  //  if (fColl2Th > 0.) 
  //    {fSolidColl2 = new G4Tubs("Coll2",                        //its name
  //				  fColl2ISzD/2,fColl2OSzD/2,fColl2Th/2,0.,twopi); //size
  //                   
  //    fLogicColl2 = new G4LogicalVolume(fSolidColl2,        //its solid
  //                                  fCollMaterial,    //its material
  //                                  "Coll2");            //its name
  //
  //    fPhysiColl2 = new G4PVPlacement(0,                  //no rotation
  //				  G4ThreeVector(0,0,fColl2PsZ),       //
  //                                  fLogicColl2,           //its logical volume                                     
  //                                  "Coll2",               //its name
  //                                  fLogicWorld,           //its mother  volume
  //                                  false,                 //no boolean operation
  //                                  0);                    //copy number
  //    }     
                                     
  // Set User Limits for aerogel and hole logical volumes
  // Sets a max step length in the aerogel region, with G4StepLimiter
  // Set additional contraints on the track, with G4UserSpecialCuts

//  G4double maxStep = 0.005*mm;
//  G4double maxLength = 2*maxStep, maxTime = 0.1*ns, minEkin = 100.*eV;

  /// fStepLimit = new G4UserLimits(maxStep);  // or,
  /// LOGICAL_VOLUME->SetUserLimits(new G4UserLimits(maxStep,
  ///                                           maxLength,
  ///                                           maxTime,
  ///                                           minEkin));

  //                                 
  // PerfLyr
  //
  fSolidPerfLyr=0; fLogicPerfLyr=0; fPhysiPerfLyr=0; 
  fSolidPerfHole=0; fLogicPerfHole=0;
  
  if (fPerfLyrTh > 0.) {
    fSolidPerfLyr = new G4Box("PerfLyr",
			      fTargetSzXY/2,fTargetSzXY/2,fPerfLyrTh/2); 
                               
    fLogicPerfLyr = new G4LogicalVolume(fSolidPerfLyr,
					fPerfLyrMaterial,
					//fPerfLyrMaterial->GetName());
					"PerfLyr"); //name
                                           
    fPhysiPerfLyr = new G4PVPlacement(0,                      //no rotation
				      G4ThreeVector(0.,0.,fBaseLyrTh/2),   //its position
				      fLogicPerfLyr,               //its logical volume               
				      //fPerfLyrMaterial->GetName(), //its name
				      "PerfLyr", //its name	
				      fLogicTarget,             //its mother
				      false,                   //no boolean operat
				      0);                      //copy number

//    fLogicPerfLyr->SetUserLimits(new G4UserLimits(maxStep,
//						  maxLength,
//						  maxTime,
//						  minEkin));
    
    //                                 
    // PerfHole parametrized volumes
    //
  
    if ( (fPerfDiameter > 0.) && (fPerfXPitch > 0.) && (fPerfDepth > 0.) ) {
      fSolidPerfHole = new G4Tubs("PerfHole",
				  0.,fPerfDiameter/2,
				  fPerfDepth/2.,
				  0.*CLHEP::deg,
				  360.*CLHEP::deg); // dummy values
	  
      fLogicPerfHole = new G4LogicalVolume(fSolidPerfHole,
					   fDefaultMaterial,
					   "PerfHole");
      G4VPVParameterisation* holeParam =
	new HoleParameterisation(fnHoles,
				 fnHX,   
				 fnHY,   
				 fPerfDiameter,
				 fPerfDepth,
				 (fPerfLyrTh-fPerfDepth)/2.,
				 fPerfXPitch,
				 fPerfYPitch,
				 fPerfStartX,
				 fPerfStartY,
				 fPerfXoffset);
                           
      // dummy value : kZAxis -- modified by parameterised volume

      new G4PVParameterised("PerfHole",       // their name
			    fLogicPerfHole,   // their logical volume
			    fLogicPerfLyr,       // Mother logical volume
			    kUndefined,          // Are placed along this axis (none) 
			    fnHoles,    // Number of holes
			    holeParam,    // The parametrisation
			    fCheckOverlaps); // checking overlaps

//      fLogicPerfHole->SetUserLimits(new G4UserLimits(maxStep,
//						     maxLength,
//						     maxTime,
//						     minEkin));
                  
    }
    else {
      fPerfDiameter = 0.;  
      fPerfDepth = 0.;  
      fPerfXPitch = 1.;
      fPerfYPitch = 1.;
      fPerfXoffset = 0.;
      fnHX = 0;  // integer values
      fnHY = 0;
      fPerfStartX = 0.; 
      fPerfStartY = 0.;
      fnHoles = fnHX*fnHY;
    }
  }
    
  //                               
  // BaseLyr
  //
  fSolidBaseLyr=0; fLogicBaseLyr=0; fPhysiBaseLyr=0;  
  
  if (fBaseLyrTh > 0.) {
    fSolidBaseLyr = new G4Box("BaseLyr",                //its name
			      fTargetSzXY/2,fTargetSzXY/2,fBaseLyrTh/2); 
                          
    fLogicBaseLyr = new G4LogicalVolume(fSolidBaseLyr,    //its solid
					fBaseLyrMaterial, //its material
					//fBaseLyrMaterial->GetName()); //name
					"BaseLyr"); //name
                                               
    fPhysiBaseLyr = new G4PVPlacement(0,                   //no rotation
				      G4ThreeVector(0.,0.,-fPerfLyrTh/2),  //its position
				      fLogicBaseLyr,       //its logical volume                    
				      //fBaseLyrMaterial->GetName(), //its name
				      "BaseLyr", //its name
				      fLogicTarget,          //its mother
				      false,                //no boolean op
				      0);                   //copy number

//    fLogicBaseLyr->SetUserLimits(new G4UserLimits(maxStep,
//						  maxLength,
//						  maxTime,
//						  minEkin));
    
  }

  G4cout << "\n\nDetectorConstruction - target parameters (may be modified in target.mac update) \n" << G4endl;
  PrintTargetParameters();     
  
  //                                        
  // Visualization attributes
  //
  fLogicWorld->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  //fLogicTarget->SetVisAttributes(simpleBoxVisAtt);

  //
  //always return the physical World
  //
  return fPhysiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal Field* DetectorConstruction::fField = 0;

void DetectorConstruction::ConstructSDandField()
//  from test_examples/extended/field/field05, version for G4v10

{
  if (!fField) {

    fField = new Field();

    G4Mag_SpinEqRhs* fEquation = new G4Mag_SpinEqRhs(fField);
    //G4EqEMFieldWithSpin* fEquation = new G4EqEMFieldWithSpin(fField);
    //     fEquation->SetBField();
    //     //fEquation->SetEField();
    //     fEquation->SetSpin();

    G4FieldManager* fieldManager
      = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fieldManager->SetDetectorField(fField);

    G4MagIntegratorStepper* fStepper = new G4ClassicalRK4(fEquation,12);

    G4double minStep           = 0.01*mm;

    G4ChordFinder* chordFinder =
      new G4ChordFinder((G4MagneticField*)fField,minStep,fStepper);

    // Set accuracy parameters
    G4double deltaChord        = 3.0*mm;
    chordFinder->SetDeltaChord( deltaChord );

    G4double deltaOneStep      = 0.01*mm;
    fieldManager->SetAccuraciesWithDeltaOneStep(deltaOneStep);

    G4double deltaIntersection = 0.1*mm;
    fieldManager->SetDeltaIntersection(deltaIntersection);

    G4TransportationManager* transportManager =
      G4TransportationManager::GetTransportationManager();

    G4PropagatorInField* fieldPropagator =
      transportManager->GetPropagatorInField();

    G4double epsMin            = 2.5e-7*mm;
    G4double epsMax            = 0.05*mm;

    fieldPropagator->SetMinimumEpsilonStep(epsMin);
    fieldPropagator->SetMaximumEpsilonStep(epsMax);

    fieldManager->SetChordFinder(chordFinder);

    G4Transportation::EnableUseMagneticMoment(true);
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintTargetParameters()
{
  G4cout << "\n------------------------------------------------------------"
         << "\n---> The Al/Mylar layer is: \n"
         << "  Material:" << fMylarMaterial->GetName()
         << "  Thickness(mm):" << fMylarTh/CLHEP::mm 
         << "  Density: from G4_MYLAR"
         << "  Position(mm):" << fMylarPsZ/CLHEP::mm 
         << "\n---> The BC beam counter scintillator position is: \n"
         << "  Position(mm):" << fBCPsZ/CLHEP::mm 
         << "\n---> The target is: \n"
         << "  Size (X,Y)(mm):" << fTargetSzXY/CLHEP::mm
         << "  Thickness(mm):" << fTargetTh/CLHEP::mm 
         << "  Position(mm):" << fTargetPsZ/CLHEP::mm 
         << "\n---> The target layers are: \n"
	 << " -- Perforated (ablated) layer"
         << "  Material:" << fPerfLyrMaterial->GetName()
         << "  Thickness(mm):" << fPerfLyrTh/CLHEP::mm 
         << "  Density(mg/cm3):" << fPerfLyrDn/(CLHEP::mg/CLHEP::cm3)
         << "\n---> The holes or perforations in this layer: \n"
         << "  Holes(total):" << fnHoles
         << "  Diameter:" << fPerfDiameter/CLHEP::mm
         << "  Length:" << fPerfDepth/CLHEP::mm
         << "\n"
         << "  Holes(X):" << fnHX
         << "  Hex Pitch(X):" << fPerfXPitch/CLHEP::mm
         << "  Hex Origin(X):" << fPerfStartX/CLHEP::mm
         << "\n"
         << "  Holes(Y):" << fnHY
         << "  Hex Pitch(Y):" << fPerfYPitch/CLHEP::mm
         << "  Hex Origin(Y):" << fPerfStartY/CLHEP::mm
         << "\n"
	 << " -- Base layer (different material or density)"
         << "  Material:" << fBaseLyrMaterial->GetName()
         << "  Thickness(mm):" << fBaseLyrTh/CLHEP::mm 
         << "  Density(mg/cm3):" << fBaseLyrDn/(CLHEP::mg/CLHEP::cm3) 
         << "\n---> The laser region is: \n"
         << "  Material:" << fDefaultMaterial->GetName()
         << "  Size in X (mm):" << fLaserRegionSzX/CLHEP::mm 
         << "  Size in Y (mm):" << fLaserRegionSzY/CLHEP::mm 
         << "  Thickness(mm):" << fLaserRegionTh/CLHEP::mm 
	 << "  Position(mm):" << fLaserRegionPsZ/CLHEP::mm 
         << "\n---> The beamstop layer is: \n"
         << "  Material:" << fBeamstopMaterial->GetName()
         << "  Thickness(mm):" << fBeamstopTh/CLHEP::mm 
         << "  Density(mg/cm3):" << fBeamstopDn/(CLHEP::mg/CLHEP::cm3) 
         << "  Position(mm):" << fBeamstopPsZ/CLHEP::mm 
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMylarTh(G4double val)
{
  // change Mylar thickness and recompute the target parameters
  fMylarTh = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetBCPsZ(G4double val)
{
  // change the beam counter position in z and recompute the target parameters
  fBCPsZ = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetSzXY(G4double val)
{
  // change the transverse target size and recompute the target parameters
  fTargetSzXY = val;
}

void DetectorConstruction::SetBaseLyrMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial =
    G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);      
  if (pttoMaterial) fBaseLyrMaterial = pttoMaterial;  
}

void DetectorConstruction::SetBaseLyrDn(G4double val)
{
  // change BaseLyr density and recompute the target parameters
  fBaseLyrDn = val;
  // define material  
  G4Element* Si = new G4Element("Silicon", "Si", z=14., a= 28.085*CLHEP::g/CLHEP::mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*CLHEP::g/CLHEP::mole);
  G4Material* pttoMaterial = new G4Material("Aerogel2", density= fBaseLyrDn, nel=2);
  pttoMaterial->AddElement(Si, natoms=1);
  pttoMaterial->AddElement(O, natoms=2);
  if (pttoMaterial) fBaseLyrMaterial = pttoMaterial;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetBaseLyrTh(G4double val)
{
  // change BaseLyr thickness and recompute the target parameters
  fBaseLyrTh = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetPerfLyrMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =  
    G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);   
  if (pttoMaterial) fPerfLyrMaterial = pttoMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetPerfLyrDn(G4double val)
{
  // change PerfLyr density and recompute the target parameters
  fPerfLyrDn = val;
  G4Element* Si = new G4Element("Silicon", "Si", z=14., a= 28.085*CLHEP::g/CLHEP::mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*CLHEP::g/CLHEP::mole);
  //  if ( fPerfLyrMaterial ) {
  //    delete fPerfLyrMaterial;   // segfault
  //  }  
  G4Material* pttoMaterial = new G4Material("Aerogel1", density= fPerfLyrDn, nel=2);
  pttoMaterial->AddElement(Si, natoms=1);
  pttoMaterial->AddElement(O, natoms=2);
  if (pttoMaterial) fPerfLyrMaterial = pttoMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetPerfLyrTh(G4double val)
{
  // change PerfLyr thickness and recompute the target parameters
  fPerfLyrTh = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetPerfDiameter(G4double val)
{
  // change the ablated hole diameter and recompute the target parameters
  fPerfDiameter = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetPerfPitch(G4double val)
{
  // change the ablated hole pitch and recompute the target parameters
  fPerfXPitch = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetPerfDepth(G4double val)
{
  // change the ablated hole depth and recompute the target parameters
  fPerfDepth = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetLaserRegionSzX(G4double val)
{
  // change the laser region size and recompute the target parameters
  fLaserRegionSzX = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetLaserRegionSzY(G4double val)
{
  // change the laser region size and recompute the target parameters
  fLaserRegionSzY = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetLaserRegionTh(G4double val)
{
  // change the laser region size and recompute the target parameters
  fLaserRegionTh = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetLaserRegionSepZ(G4double val)
{
  // change the laser region size and recompute the target parameters
  fLaserRegionSepZ = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetBeamstopMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial =  
    G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);   
  if (pttoMaterial) fBeamstopMaterial = pttoMaterial;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetBeamstopDn(G4double val)
{
  // change Beamstop density and recompute the target parameters
  fBeamstopDn = val;
  G4Element* Si = new G4Element("Silicon", "Si", z=14., a= 28.085*CLHEP::g/CLHEP::mole);
  G4Element* O = new G4Element("Oxygen"  , "O", z=8., a= 16.00*CLHEP::g/CLHEP::mole);
  G4Material* pttoMaterial = new G4Material("BeamstopLyr", density= fBeamstopDn, nel=2);
  pttoMaterial->AddElement(Si, natoms=1);
  pttoMaterial->AddElement(O, natoms=2);
  if (pttoMaterial) fBeamstopMaterial = pttoMaterial;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetBeamstopTh(G4double val)
{
  // change PerfLyr thickness and recompute the target parameters
  fBeamstopTh = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry()
{
  G4cout << "\n\nDetectorConstruction::UpdateGeometry - updating target parameters \n" << G4endl;
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructTarget());

  //PrintTargetParameters();   //done at end of ConstructTarget 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ComputeTargetParameters()
{
  G4cout << "\n\nDetectorConstruction::ComputeTargetParameters - derived parameters of the target\n" << G4endl;

  fBCFramePsZ  = fBCPsZ;

  fTargetTh =  fBaseLyrTh + fPerfLyrTh;
  fTargetPsZ = -fTargetTh/2.;

  fPerfYPitch = std::sqrt(3.)*fPerfXPitch/2.;
  fPerfXoffset = fPerfXPitch/2.;
  fnHX = (fTargetSzXY - 2.*fPerfLyrFrameSz)/fPerfXPitch;  // integer values
  fnHY = (fTargetSzXY - 2.*fPerfLyrFrameSz)/fPerfYPitch;
  fPerfStartX = -(fnHX - 1)*fPerfXPitch/2 - fPerfXoffset/2; // make hole pattern symmetric about axes
  fPerfStartY = -(fnHY - 1)*fPerfYPitch/2;
  fnHoles = fnHX*fnHY;

  fLaserRegionPsZ   = fLaserRegionSepZ + 0.5*fLaserRegionTh;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
