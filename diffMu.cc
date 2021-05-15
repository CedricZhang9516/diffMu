// diffMu
// 
// simulation of muons in aerogel target
//   with two layers of different densities, 
//   where the second layer is a perforated or drilled layer
// adapted from example/extended/AnaEx02 for using ROOT histograms
//   and detector (i.e., target) geometry adjustable via input .mac files
// command line input style from examples/basic/B4/B4a
// uses gps particle source



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "PhysicsList.hh"
#include "HistoManager.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIcommand.hh"
#include "G4PhysListFactory.hh"
#include "G4ParticleTable.hh"
#include "G4AnyType.hh"
#include "G4MuonPlus.hh"

#include "Randomize.hh"
// #include "time.h"

#ifdef G4VIS_USE
 #include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif
//#include "G4VisExecutive.hh"
//#include "G4UIExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " ./diffMu # default is interactive session;" << G4endl;
    G4cerr << "          [-h] # for help on input parameters;" << G4endl;
    G4cerr << "          [-n nEvents] # set this for batch (non-interactive) run" << G4endl;
    G4cerr << "          [-p momentum_(MeV/c)] [-dp dp/p_(rms)] # beam parameters" << G4endl;
    G4cerr << "          [-phi diameter] [-pit pitch] [-dep depth] # target parameters (mm)" << G4endl;
    G4cerr << "          [-den ablated_layer_density_(mg/cm^3)] # target parameter" << G4endl;
    G4cerr << "          [-r run_number] # for output file name only; default 0" << G4endl;
    G4cerr << "          [-seed random_seed] # or default derived from time" << G4endl;
    G4cerr << "          [-dir output_dir ]  #  for output root and log file directory; default ./" << G4endl;
    G4cerr << "          [-log logfile_name ]  #  for output special root and log file name; default nil " << G4endl;
    G4cerr << "          [-m macro ]  # or use a macro for custom batch input" << G4endl;
    G4cerr << "          [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << " # beam parameters -p and -dp must be both set or both absent" << G4endl;
    G4cerr << " # command line options override defaults in beam.mac and target.mac" << G4endl;
    G4cerr << " # macro file options override defaults and input lines" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{     
  // Evaluate arguments
  //
  if ( argc > 23 ) {
    PrintUsage();
    return 1;
  }

  G4bool fBatch = false;
  G4String sNumEvents;
  G4String sPmom;
  G4String sDpg;
  G4String sDiameter;
  G4String sPitch;
  G4String sDepth;
  G4String sDensity;
  G4String sRunNumber = "0";
  G4String sSeed;  
  G4String output_dir = "./";
  G4String logfile;
  G4String macro;
  G4String session;
#ifdef G4MULTITHREADED
  G4int nThreads = 0;
#endif

  G4int NumEvents;
  G4double pmom, dpg; 
  G4double abdiameter, abpitch, abdepth, abdensity;
   
  for ( G4int i=1; i<argc; i=i+2 ) {
    if ( G4String(argv[i]) == "-h" ) {
      PrintUsage();
      return 1;
    }
    else if ( G4String(argv[i]) == "-n" ) {
      sNumEvents = G4String(argv[i+1]);
      NumEvents =  G4UIcommand::ConvertToInt(sNumEvents);
      fBatch = true;
    }
    else if ( G4String(argv[i]) == "-p" ) {
      sPmom = G4String(argv[i+1]);
      pmom =  G4UIcommand::ConvertToDouble(sPmom);
    }
    else if ( G4String(argv[i]) == "-dp" ) {
      sDpg = G4String(argv[i+1]);
      dpg =  G4UIcommand::ConvertToDouble(sDpg);
    }
    else if ( G4String(argv[i]) == "-phi" ) {
      sDiameter = G4String(argv[i+1]);
      abdiameter =  G4UIcommand::ConvertToDouble(sDiameter);
    }
    else if ( G4String(argv[i]) == "-pit" ) {
      sPitch = G4String(argv[i+1]);
      abpitch =  G4UIcommand::ConvertToDouble(sPitch);
    }
    else if ( G4String(argv[i]) == "-dep" ) {
      sDepth = G4String(argv[i+1]);
      abdepth =  G4UIcommand::ConvertToDouble(sDepth);
    }
    else if ( G4String(argv[i]) == "-den" ) {
      sDensity = G4String(argv[i+1]);
      abdensity =  G4UIcommand::ConvertToDouble(sDensity);
    }
    else if ( G4String(argv[i]) == "-r" ) {
      sRunNumber = G4String(argv[i+1]);
    }
    else if ( G4String(argv[i]) == "-seed" ) {
      sSeed = G4String(argv[i+1]);
    }
    else if ( G4String(argv[i]) == "-dir" ) {
      output_dir = G4String(argv[i+1])+"/";
    }
    else if ( G4String(argv[i]) == "-log" ) {
      logfile = G4String(argv[i+1]);
    }
    else if ( G4String(argv[i]) == "-m" ) {
      macro = G4String(argv[i+1]);
      fBatch = true;
    }
    else if ( G4String(argv[i]) == "-u" ) {
      G4String(session = argv[i+1]);
    }
#ifdef G4MULTITHREADED
    else if ( G4String(argv[i]) == "-t" ) {
      nThreads = G4UIcommand::ConvertToInt(argv[i+1]);
    }
#endif
    else {
      PrintUsage();
      return 1;
    }
  }  
  
  if ( (sPmom.size() == 0) ^ (sDpg.size() == 0)) {
    PrintUsage();
    return 1;
  }

  G4int RunNumber = G4UIcommand::ConvertToInt(sRunNumber);

  // Detect interactive mode (if number of events or macro name not entered) and define UI session
  //
#ifdef G4UI_USE
  G4UIExecutive* ui = nullptr;
  if ( ! fBatch ) {
    ui = new G4UIExecutive(argc, argv, session);
  }
#endif  
  // Seed the random number according to time
  //
  G4Random::setTheEngine(new CLHEP::RanecuEngine());
  G4long seed;
  if ( sSeed.size() ) {
    seed = std::stol(sSeed);
  }
  else {
    seed = time(NULL);
  }
  G4Random::setTheSeed(seed);
  G4cout << "Setting random seed: " << seed << G4endl;

  // Construct the default run manager
  //
#ifdef G4MULTITHREADED
  auto runManager = new G4MTRunManager;
  if ( nThreads > 0 ) { 
    runManager->SetNumberOfThreads(nThreads);
  }  
#else
  auto runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  //
  auto detector = new DetectorConstruction();
  runManager->SetUserInitialization(detector);

  auto physicslist = new PhysicsList();
  runManager->SetUserInitialization(physicslist);

  // set a HistoManager
  // - usually in ActionInitialization, but histo methods used here,
  // e.g., SetNumEvents, SetRunNummber, set root and log file names.
  //
  HistoManager*  histo = new HistoManager(detector);
  //auto  histo = new HistoManager(detector);
      
  // Set user action classes
  //
  //auto actionInitialization = new ActionInitialization(detector,histo);
  auto actionInitialization = new ActionInitialization(histo);
  runManager->SetUserInitialization(actionInitialization);

  // Initialize G4 kernel
  //
  //runManager->Initialize();  // do this in setup.mac

#ifdef G4VIS_USE
  // Initialize visualization
  auto visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  visManager->Initialize();
#endif
  

  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();


  // =======================================================
  // // // diffMu commands and control from command line
  
  // dffMu input
  G4String command, gpspar;
  G4String rootfilebase;
  G4double mmuon =
    G4ParticleTable::GetParticleTable()->FindParticle("mu+")->GetPDGMass()/CLHEP::MeV;
  G4double ekin;
  std::stringstream ss;

  // mac files have some defaults; other default values are hard-coded
  command = "/control/execute setup.mac";
  UImanager->ApplyCommand(command);    

  // // the run initialize command is in setup.mac
  //command = "/run/initialize";
  //UImanager->ApplyCommand(command);

  command = "/control/execute target.mac";
  UImanager->ApplyCommand(command);

  command = "/control/execute beam.mac";
  UImanager->ApplyCommand(command);

  G4cout.precision(5);

  G4cout << " " << G4endl;
  G4cout << "+----------> diffMu command line input parameters: "
	 << G4endl;
  if ( sNumEvents.size() ) {
    // set number of events
    histo->SetEventsRequested(NumEvents);
    rootfilebase.append("_n"+sNumEvents);
    G4cout << "Events " << sNumEvents << G4endl;
  }

  // beam characteristics
  if ( sPmom.size() & sDpg.size() ) {
    // calculate and set beam energy
    ekin = sqrt(pmom*pmom + mmuon*mmuon) - mmuon;
    command = "/gps/energy ";
    ss.str(std::string());
    ss << ekin;
    gpspar = ss.str();
    UImanager->ApplyCommand(command+gpspar+" MeV");    
    ss.str(std::string());
    ss << std::setfill('0') << std::setw(4) << int(100.*pmom);
    rootfilebase.append("_p"+ss.str());
    // calculate and set beam rms energy width, approximated by dE=E*2.*dp/p
    G4double dekinrms = 2.*ekin*dpg;   // dE in MeV
    command = "/gps/ene/type Gauss";
    UImanager->ApplyCommand(command);    
    command = "/gps/ene/sigma ";
    ss.str(std::string());
    ss << dekinrms;
    gpspar = ss.str();
    UImanager->ApplyCommand(command+gpspar+" MeV");
    ss.str(std::string());
    ss << std::setfill('0') << std::setw(4) << int(1000.*dpg);
    rootfilebase.append("_dp"+ss.str());
    G4cout << "muon mass (MeV/csq), p (MeV/c), dp/p(Gaussian rms) "
	   << mmuon << ", " << pmom << ", " << dpg << G4endl;
    G4cout << "ekin (MeV), dekin(rms) " << ekin << ", " << dekinrms << G4endl;
  }

  // aerogel target parameters
  if (sDiameter.size() ) {
    command = "/DiffMu/tgt/setPerfDiameter "+sDiameter+" mm";
    UImanager->ApplyCommand(command);    
    ss.str(std::string());
    ss << std::setfill('0') << std::setw(4) << int(1000.*abdiameter);
    rootfilebase.append("_phi"+ss.str());
    G4cout << "Ablation diameter " << abdiameter << " mm" << G4endl;
  }
  if (sPitch.size() ) {
    command = "/DiffMu/tgt/setPerfPitch "+sPitch+" mm";
    UImanager->ApplyCommand(command);    
    ss.str(std::string());
    ss << std::setfill('0') << std::setw(4) << int(1000.*abpitch);
    rootfilebase.append("_pit"+ss.str());
    G4cout << "Ablation pitch " << abpitch << " mm" << G4endl;
  }
  if (sDepth.size() ) {
    command = "/DiffMu/tgt/setPerfDepth "+sDepth+" mm";
    UImanager->ApplyCommand(command);    
    ss.str(std::string());
    ss << std::setfill('0') << std::setw(4) << int(1000.*abdepth);
    rootfilebase.append("_dep"+ss.str());
    G4cout << "Ablation depth " << abdepth << " mm" << G4endl;
  }
  if (sDensity.size() ) {
    command = "/DiffMu/tgt/setPerfLyrDens "+sDensity+" mg/cm3";
    UImanager->ApplyCommand(command);    
    ss.str(std::string());
    ss << std::setfill('0') << std::setw(5) << int(10.*abdensity);
    rootfilebase.append("_den"+ss.str());
    G4cout << "Ablation material density " << abdensity << " mg/cm3" << G4endl;
  }
  if ( ( sDiameter.size() ) ||  ( sPitch.size() ) || ( sDepth.size() ) || ( sDensity.size() ) ) {
    command = "/DiffMu/tgt/update";
    UImanager->ApplyCommand(command);
  }
      
  // set run number; "0" is default if not on command line
  runManager->SetRunIDCounter(RunNumber);
  histo->SetRunNumber(RunNumber);
  rootfilebase.append("_r"+sRunNumber);
  G4cout << "run number " << RunNumber << G4endl;

  if ( logfile.size() ) {
    // make root file have same name as log file with run-specific concatenation
    histo->rootFileName = output_dir+logfile+rootfilebase+".root"; 
    G4cout << "output root file name " << output_dir+logfile+"_"+rootfilebase+".root" << G4endl;
    // add output at end of logfile ...
    // (make sure identifier, e.g. run number, is included in HistoManager::PrintStatistic() )
    G4String outputLogFile = output_dir+logfile+".log";
    // ... or make a unique name for each output file
    //G4String outputLogFile = output_dir+logfile+"_"+rootfilebase+".log";
    histo->logFileName = outputLogFile; 
    G4cout << "output log file name " << outputLogFile << G4endl;
  }
  else {
    histo->rootFileName = output_dir+"diffMu"+rootfilebase+".root"; 
    G4cout << "output root file name " << output_dir+"diffMu"+rootfilebase+".root" << G4endl;
  }
  G4cout << "<----------+ " << G4endl;
  G4cout << " " << G4endl;

  // =======================================================


  // Process macro or start UI session
  //
  if ( fBatch ) {     //batch mode
    if ( macro.size() ) {
      command = "/control/execute ";
      UImanager->ApplyCommand(command+macro);
    }
    if ( sNumEvents.size() ) {
      command = "/run/beamOn ";
      UImanager->ApplyCommand(command+sNumEvents);
    }
  }
  else
    {  // interactive mode : define visualization and UI session
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute init.mac"); 
      UImanager->ApplyCommand("/control/execute vis.mac");
#endif
#ifdef G4UI_USE
      if (ui->IsGUI())
	UImanager->ApplyCommand("/control/execute gui.mac");
      ui->SessionStart();
      delete ui;
#endif
    }

  
  // Job termination
#ifdef G4VIS_USE
  delete visManager;
#endif  
  delete histo;                
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
