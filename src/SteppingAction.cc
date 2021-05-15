//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"

//#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4UnitsTable.hh"
#include "G4Step.hh"
#include "G4SteppingManager.hh"
#include "G4String.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* evt)
  :fEventAction(evt)                                         
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  G4int eahists = 49; // number of histograms filled in EventAction [0, eahists-1]
  //G4int eascats = 7; // number of 2d histograms filled in EventAction
    
  // get the track information to make sure it is a muon
  G4Track* aTrack = aStep->GetTrack();
  G4String particlename 
    = aTrack->GetDefinition()->GetParticleName();

  // get step number to identify generation variables from first step
  G4int currentTrackID
    = aTrack->GetTrackID();
  G4int currentStepNumber
    = aTrack->GetCurrentStepNumber();

  // get step points and touchables

  G4StepPoint* preStepPoint
    = aStep->GetPreStepPoint();
  G4TouchableHandle touchpre = preStepPoint->GetTouchableHandle();

  G4StepPoint* postStepPoint
    = aStep->GetPostStepPoint();
  G4TouchableHandle touchpost = postStepPoint->GetTouchableHandle();

  // get the volume of pre-step point
  G4VPhysicalVolume* volumepre = touchpre->GetVolume();
  G4String volnamepre = volumepre->GetName();
  G4String matnamepre = volumepre->GetLogicalVolume()->GetMaterial()->GetName();

  // get the volume of post-step point
  //  G4VPhysicalVolume* volumepost = touchpost->GetVolume();
  //  G4String matnamepost = volumepost->GetLogicalVolume()->GetMaterial()->GetName(); // causes segfault
  //  G4String volnamepost = volumepost->GetName();   // causes segfault - undefined?

  // get processes corresponding to this step to check formation or decay
  //  G4String preStepProcess 
  //    = preStepPoint->GetProcessDefinedStep()->GetProcessName();   // causes segfault - undefined for first step?
  G4String postStepProcess 
    = postStepPoint->GetProcessDefinedStep()->GetProcessName();

  // *** Initial particle *** //
  
  if (currentTrackID == 1 && currentStepNumber == 1) {
    G4double eki = aStep->GetTrack()->GetVertexKineticEnergy();
    G4double tti = aStep->GetTrack()->GetGlobalTime();
    fEventAction->Fill2D(7, tti/CLHEP::microsecond, std::log10(eki/CLHEP::eV));

    if (fEventAction->GetIfNewEvent() ) {    // only do this once per event
      G4double xxi = aStep->GetTrack()->GetVertexPosition().x();
      G4double yyi = aStep->GetTrack()->GetVertexPosition().y();
      G4double zzi = aStep->GetTrack()->GetVertexPosition().z();
      G4double dxi = aStep->GetTrack()->GetVertexMomentumDirection().x();
      G4double dyi = aStep->GetTrack()->GetVertexMomentumDirection().y();
      G4double dzi = aStep->GetTrack()->GetVertexMomentumDirection().z();
      G4double pxi = postStepPoint->GetPolarization().x();
      G4double pyi = postStepPoint->GetPolarization().y();
      G4double pzi = postStepPoint->GetPolarization().z();
      //    G4double eki = aStep->GetTrack()->GetVertexKineticEnergy();
      //    G4double ppi = preStepPoint->GetMomentum().r();

      fEventAction->SaveInit(xxi,yyi,zzi,dxi,dyi,dzi,pxi,pyi,pzi);
      fEventAction->SetIfNewEvent(false);  // any further steps in this event not included
    }
  }

  // *** Muon (mu+) *** //

  if ( particlename == "mu+" ) {
    // find target stops
    if ( matnamepre.compare(0,7,"Aerogel") == 0 ) {   // step starts in Aerogel (1 or 2)
      G4double ektlow = preStepPoint->GetKineticEnergy();
      // if first step of mu+ below this energy, this is going to stop as mu+
      if ((ektlow < 10.*CLHEP::eV) && not fEventAction->GetIfTargetStop()) { 
	G4double zztlow = preStepPoint->GetPosition().z();
	G4double tttlow = preStepPoint->GetGlobalTime();

	fEventAction->SaveTgtStop(zztlow, ektlow, tttlow);
	fEventAction->SetIfTargetStop(true);                // any further mu+ steps in this event are ignored
      }
    }

    // find Decay or DecayWithSpin
    if ( ( postStepProcess.compare(0,5,"Decay" ) == 0 ) ) {
      G4double xxdmuon = postStepPoint->GetPosition().x();
      G4double yydmuon = postStepPoint->GetPosition().y();
      G4double zzdmuon = postStepPoint->GetPosition().z();
      G4double pxdmuon = postStepPoint->GetPolarization().x();
      G4double pydmuon = postStepPoint->GetPolarization().y();
      G4double pzdmuon = postStepPoint->GetPolarization().z();
      G4double ttdmuon = postStepPoint->GetGlobalTime();

      fEventAction->SavemuonDecay(xxdmuon,yydmuon,zzdmuon,pxdmuon,pydmuon,pzdmuon,ttdmuon);
    }
  }
  // *** Muonium (Mu) *** //
  
  if ( particlename == "Mu" ) {
    // step information
    G4double ekMustep = aStep->GetTrack()->GetKineticEnergy();
    G4double slMustep = aStep->GetStepLength();
    // all steps as Mu
    eahists = 49;
    fEventAction->Fill1D(eahists++ , ekMustep/(1.e-3*CLHEP::eV));
    fEventAction->Fill1D(eahists++ , slMustep/CLHEP::um);
    fEventAction->Fill1D(eahists++ , slMustep/CLHEP::um);
    fEventAction->Fill1D(eahists++ , std::log10(DBL_MIN + slMustep/CLHEP::um) );
    //      G4cout << "SteppingAction: Mu " << postStepProcess << " E(meV), ds(um): " 
    //	     << ekMustep/( 1.e-3*CLHEP::eV ) << ", " 
    //	     << slMustep/( CLHEP::micrometer ) << G4endl;

    // initial step information after Mu initialized or created in process
    if ( currentStepNumber == 1 ) {
      //G4double ekMui = aStep->GetTrack()->GetVertexKineticEnergy();
      //G4double ppMui = preStepPoint->GetMomentum().r();
      //G4double ttMui = aStep->GetTrack()->GetGlobalTime();
      G4double ekMui = preStepPoint->GetKineticEnergy();
      G4double ttMui = preStepPoint->GetGlobalTime();
      fEventAction->Fill2D(8, ttMui/CLHEP::microsecond, std::log10(ekMui/CLHEP::eV));
      //G4String fromProcess = aStep->GetTrack()->GetCreatorProcess()->GetProcessName();
      //G4cout << "SteppingAction: Mu from " << fromProcess << G4endl;

      // only if this is the first Mu step in the event
      if ( fEventAction->GetIfNewMuInit() ) {
	// && fromProcess == "MuZeroFormation") {
	G4double xxMui = preStepPoint->GetPosition().x();
	G4double yyMui = preStepPoint->GetPosition().y();
	G4double zzMui = preStepPoint->GetPosition().z();
	G4double dxMui = preStepPoint->GetMomentumDirection().x();
	G4double dyMui = preStepPoint->GetMomentumDirection().y();
	G4double dzMui = preStepPoint->GetMomentumDirection().z();
	G4double pxMui = preStepPoint->GetPolarization().x();
	G4double pyMui = preStepPoint->GetPolarization().y();
	G4double pzMui = preStepPoint->GetPolarization().z();

	fEventAction->SaveMuInit(xxMui,yyMui,zzMui,dxMui,dyMui,dzMui,pxMui,pyMui,pzMui,ekMui,ttMui);
	fEventAction->SetIfNewMuInit(false);   //

	//	  G4cout << "Mu step 1" << " z,E,t " 
	//		 << zzMui/CLHEP::mm << " " 
	//		 << ekMui/CLHEP::keV << " " 
	//		 << ttMui/CLHEP::microsecond << G4endl;
      }
    }
    // Mu entry into vacuum from aerogel
    if ( matnamepre.compare(0,7,"Aerogel") == 0 && postStepPoint->GetStepStatus() == fGeomBoundary ) {
      if ( postStepPoint->GetMaterial()->GetName() == "G4_Galactic" ) {
	G4double zzMuem = postStepPoint->GetPosition().z();
	G4double ttMuem = postStepPoint->GetGlobalTime();
	fEventAction->Fill1D(eahists++ , zzMuem/CLHEP::mm);
	fEventAction->Fill1D(eahists++ , ttMuem/CLHEP::microsecond);
	if ( fEventAction->GetIfNewMuEmit() ) {   // first emission of Mu in this event
	  G4double xxMuem = postStepPoint->GetPosition().x();
	  G4double yyMuem = postStepPoint->GetPosition().y();
	  G4double dxMuem = postStepPoint->GetMomentumDirection().x();
	  G4double dyMuem = postStepPoint->GetMomentumDirection().y();
	  G4double dzMuem = postStepPoint->GetMomentumDirection().z();
	  G4double ekMuem = postStepPoint->GetKineticEnergy();

	  fEventAction->SaveMuEmit(xxMuem,yyMuem,zzMuem,dxMuem,dyMuem,dzMuem,ekMuem,ttMuem);
	  fEventAction->SetIfNewMuEmit(false);    // any further Mu emission steps in this event not included
	}	  
      }
    }
    // Mu entry into laser region
    if ( volnamepre == "LaserRegion" && preStepPoint->GetStepStatus() == fGeomBoundary ) {
      G4double xxMuLRIn = preStepPoint->GetPosition().x();
      G4double yyMuLRIn = preStepPoint->GetPosition().y();
      G4double zzMuLRIn = preStepPoint->GetPosition().z();
      G4double ttMuLRIn = preStepPoint->GetGlobalTime();

      fEventAction->SaveMuLRIn(xxMuLRIn,yyMuLRIn,zzMuLRIn,ttMuLRIn);	
    }
    // Mu exit from laser region
    if (volnamepre == "LaserRegion" && postStepPoint->GetStepStatus() == fGeomBoundary) {
      G4double xxMuLROut = postStepPoint->GetPosition().x();
      G4double yyMuLROut = postStepPoint->GetPosition().y();
      G4double zzMuLROut = postStepPoint->GetPosition().z();
      G4double ttMuLROut = postStepPoint->GetGlobalTime();

      fEventAction->SaveMuLROut(xxMuLROut,yyMuLROut,zzMuLROut,ttMuLROut);	
    }
    // Mu decay
    if (postStepProcess.compare(0,5,"Decay") == 0) {
      G4ThreeVector initPos(0., 0., 0.*CLHEP::mm);
      G4double xxMud = postStepPoint->GetPosition().x();
      G4double yyMud = postStepPoint->GetPosition().y();
      G4double zzMud = postStepPoint->GetPosition().z();
      G4double pxMud = postStepPoint->GetPolarization().x();
      G4double pyMud = postStepPoint->GetPolarization().y();
      G4double pzMud = postStepPoint->GetPolarization().z();
      G4double rrMud = (postStepPoint->GetPosition() - initPos).r();
      G4double ttMud = postStepPoint->GetGlobalTime();

      fEventAction->SaveMuDecay(xxMud,yyMud,zzMud,pxMud,pyMud,pzMud,rrMud,ttMud);
      //	  G4cout << "Mu " << postStepProcess << " z,t " 
      //		 << zzMud/mm << " " 
      //		 << ttMud/microsecond << G4endl;
    }
  }

  // *** Positron (e+) *** //
  // make sure positrons are not killed in StackingAction.cc, otherwise these variables are not filled
  
  //  if (particlename == "e+" && currentStepNumber == 1) {
  if (particlename == "e+" && volnamepre == "Vacsys" && postStepPoint->GetStepStatus() == fGeomBoundary) {
    G4double xxe = aStep->GetTrack()->GetVertexPosition().x();
    G4double yye = aStep->GetTrack()->GetVertexPosition().y();
    G4double zze = aStep->GetTrack()->GetVertexPosition().z();
    G4double dxe = aStep->GetTrack()->GetVertexMomentumDirection().x();
    G4double dye = aStep->GetTrack()->GetVertexMomentumDirection().y();
    G4double dze = aStep->GetTrack()->GetVertexMomentumDirection().z();
    G4double tte = preStepPoint->GetGlobalTime();
    //      G4double eke = aStep->GetTrack()->GetVertexKineticEnergy();
    //      G4double ppe = preStepPoint->GetMomentum().r();

    fEventAction->SavePosi(xxe,yye,zze,dxe,dye,dze,tte);
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
