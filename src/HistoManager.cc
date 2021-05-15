//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "DetectorConstruction.hh"

#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include <G4PhysicalConstants.hh>
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleMomentum.hh"
#include "G4ios.hh"
#include "G4Tokenizer.hh"

//#include <sstream>    // for stringstream
//#include <iomanip>   // for setfill(), setw(), 
//#include <cstdlib>   // for exit(), atof

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//HistoManager::HistoManager()
HistoManager::HistoManager(DetectorConstruction* detConstruction)
  :fDetConstruction(detConstruction),
   rootFile(0)         

   // ntupl1(0), 
   //   r_num(0), i_ev(0), x_in(0), y_in(0), z_in(0),
   //   x_dec(0), y_dec(0), z_dec(0),t_dec(0)

{
  // histograms
  for (G4int k=0; k<Max1D; k++) h1D[k] = 0;
  for (G4int k=0; k<Max2D; k++) h2D[k] = 0;

  eventsPrimaryAll = 0;
  eventsPrimaryFail = 0;

  //for (G4int k=0; k<MaxVecLen; k++) rootvec[k] = 0;
    
  // ntuples
  //ntupl1 = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  if ( rootFile ) delete rootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::book()
{ 
  // Creating a tree container to handle histograms and ntuples.
  // This tree is associated to an output file.
  //
  G4cout << "\n HistoManager: creating rootFileName " << rootFileName << G4endl;
  rootFile = new TFile(rootFileName,"RECREATE");
  if(!rootFile) 
    {
      G4cout << " HistoManager::book :" 
	     << " problem creating the ROOT TFile "
	     << G4endl;
      return;
    }

  // Book Histograms

  // Histogram parameters (1D and 2D)
  G4double xyworld = 100.;
  G4double zworld = 100.;
  G4double xyem = 25.;
  G4double zem = 50.;
  G4double xytgt = 25.;
  G4double ztgt = 10.;
  
  //G4double dxyr = 0.4;
  // Limits arrays
  //G4double mus_max = 200.;
  G4double mus_max = 10.;

  // Histogram definitions: set Max1D and Max2D in .hh
  // Title and label arrays
  G4String H1title[Max1D][2] = {
    /* 0-8 */
    {"x_init", "x (mm)"},    {"y_init", "y (mm)"},  {"z_init", "z (mm)"},
    {"dx_init", "dx"},    {"dy_init", "dy"},  {"dz_init", "dz"},
    {"Px_init", "Px"},    {"Py_init", "Py"},  {"Pz_init", "Pz"},
    /* 9-17 */
    {"x_muondecay", "x (mm)"},   {"y_muondecay", "y (mm)"},{"z_muondecay", "z (mm)"}, 
    {"Px_muondecay", "t (#mus)"}, {"Py_muondecay", "t (#mus)"}, {"Pz_muondecay", "t (#mus)"}, 
    {"t_muondecay", "t (#mus)"}, {"t_muonL", "t (#mus)"}, {"t_muonR", "t (#mus)"},
    /* 18-28 */
    {"x_Muinit", "x (mm)"},   {"y_Muinit", "y (mm)"}, {"z_Muinit", "z (mm)"}, 
    {"dx_Muinit", "dx"}, {"dy_Muinit", "dy"}, {"dz_Muinit", "dz"}, 
    {"Px_Muinit", "Px"},   {"Py_Muinit", "Py"}, {"Pz_Muinit", "Pz"}, 
    {"ek_Muinit", "logE (eV)"}, {"t_Muinit", "t (#mus)"},
    /* 29-36 */
    {"x_Muem1", "x (mm)"},   {"y_Muem1", "y (mm)"}, {"z_Muem1", "z (mm)"}, 
    {"dx_Muem1", "dx"}, {"dy_Muem1", "dy"}, {"dz_Muem1", "dz"}, 
    {"ek_Muem1", "logE (eV)"}, {"t_Muem1", "t (#mus)"},
    /* 37-44 */
    {"x_Mudecay", "x (mm)"},   {"y_Mudecay", "y (mm)"}, {"z_Mudecay", "z (mm)"},
    {"Px_Mudecay", "t (#mus)"}, {"Py_Mudecay", "t (#mus)"}, {"Pz_Mudecay", "t (#mus)"}, 
    {"r_Mudecay", "r from (0,0,-5mm) (mm)"}, {"t_Mudecay", "t (#mus)"},
    /* 45-46 */
    {"z_Mudecay_em", "z (mm)"}, {"t_Mudecay_em", "t (#mus)"},
    /* 47-48 */
    {"t_MuL", "t (#mus)"}, {"t_MuR", "t (#mus)"},
    /* 49-54, in Stepping */
    {"ek_Mustep", "E (meV)"}, {"slen_Mustep", "step length (#mum)"},
    {"small_slen_Mustep", "step length (#mum)"}, {"log_slen_Mustep", "log10 step length (#mum)"},
    {"zem","z_emission (mm)"}, {"tem","t_emission (#mus)"}, 
  }; 

  // Dimension arrays
  G4int H1dim[Max1D] = {
    /* 0-8 */
    100, 100, 100,
    100, 100, 100,
    100, 100, 100,
    /* 9-17 */
    100, 100, 400, 
    1000, 1000, 1000,
    1000, 1000, 1000,
    /* 18-28 */
    100, 100, 100, 
    100, 100, 100, 
    100, 1000,
    100, 100, 100,
    /* 29-36 */
    100, 100, 600, 
    100, 100, 100, 
    100, 1000,
    /* 37-44 */
    2000, 2000, 
    8000, 500, 
    1000,
    1000, 1000, 1000,
    /* 45-46 */
    300, 1520,
    /* 47-48 */
    1000, 1000,
    /* 49-54, in Stepping */
    250, 1000,
    1000, 600,
    600, 1000
  };

  G4double H1limits[Max1D][2] = {
    /* 0-8 */
    {-xyworld, xyworld}, {-xyworld, xyworld}, {-zworld, zworld},
    {-0.1, 0.1}, {-0.1, 0.1}, {0.95, 1.}, 
    {-1., 1.}, {-1., 1.}, {-1., 1.}, 
    /* 9-17 */
    {-xytgt, xytgt}, {-xytgt, xytgt}, {-zworld, zworld},
    {0., mus_max}, {0., mus_max}, {0., mus_max},
    {0., mus_max}, {0., mus_max}, {0., mus_max},
    /* 18-28 */
    {-xytgt, xytgt}, {-xytgt, xytgt}, {-ztgt,ztgt}, 
    {-1., 1.}, {-1., 1.}, {-1., 1.}, 
    {-1., 1.}, {-1., 1.}, {-1., 1.}, 
    {1., 5.}, {0., 0.001},
    /* 29-36 */
    {-xytgt, xytgt}, {-xytgt, xytgt}, {-ztgt,ztgt}, 
    {-1., 1.}, {-1., 1.}, {-1., 1.}, 
    {-4., 2.}, {0., 10.},
    /* 37-44 */
    {-xyem,xyem}, {-xyem, xyem}, {-zem, zem},
    {0., mus_max}, {0., mus_max}, {0., mus_max},
    {0., zem}, {0., mus_max},
    /* 45-46 */
    {-10., zem}, {-0.836, 15.048},
    /* 47-48 */
    {0., mus_max}, {0., mus_max},
    /* 49-54, in Stepping */
    {0., 250.}, {0., 50.},
    {0., 0.05}, {-4., 2.},
    {-15., 45.}, {0., mus_max}
  };

  G4String H2title[Max2D][2] = {
    /* 0-4 */
    {"y_init vs x_init", "x (mm);y (mm)"}, 
    {"y_alldecay vs z_alldecay", "z (mm);y (mm)"},
    {"y_Mudecay vs x_Mudecay", "x (mm);y (mm)"},
    {"y_Mudecay vs z_Mudecay", "z (mm);y (mm)"},
    {"t_Mudecay vs z_Mudecay", "z (mm);t (#mus)"},
    /* 5-6 */
    {"z_stop vs t_stop", "t (#mus);z (mm)"},
    {"LR out/decay vs LR in time", "time (laser entrance, #mus);time (laser exit, #mus)"},
    /* 7-8, in Stepping */
    {"E(init) vs t(init)", "t (#mus);log10 Einit (eV)"},
    {"EMu(init) vs t", "t (#mus);log10 EMu(init) (eV)"}
  }; 

  G4int H2dim[Max2D][3] = {
    {100, 100}, {400, 100}, {200,200}, {90,100}, {180,1520},
    {100,100}, {1000,1000},
    {100,100}, {100,100}
  };

  G4double H2limits[Max2D][4] = {
    {-xyworld, xyworld, -xyworld, xyworld},
    {  -xyworld, xyworld, -xyworld, xyworld},
    {  -xyem, xyem, -xyem, xyem},
    {  -zem, zem, -xyem, xyem},
    {  -zem, zem, 15.048},
    
    {  0., 0.010, -10., 0.},
    {  0., mus_max, 0., mus_max},

    {  0., 0.010, -4., 0.},
    {  0., 0.010, -4., 0.}

  };

  // Loops for booking
  G4String Htstr;
  for (G4int k=0; k<Max1D; k++)
    {
      Htstr = H1title[k][0] + ";" + H1title[k][1];
      h1D[k] = new TH1D(H1title[k][0],Htstr,H1dim[k],H1limits[k][0],H1limits[k][1]);
      if (!h1D[k]) G4cout << "\n can't create h1D " <<  H1title[k][0] << G4endl;
    }
  for (G4int k=0; k<Max2D; k++)
    {
      Htstr = H2title[k][0] + ";" + H2title[k][1];
      h2D[k] = new TH2D(H2title[k][0],Htstr,H2dim[k][0],H2limits[k][0],H2limits[k][1],
			H2dim[k][1],H2limits[k][2],H2limits[k][3]);
      if (!h2D[k]) G4cout << "\n can't create h2d " <<  H2title[k][0] << G4endl;
    }


  // create ntuple(s)
  //
  //  G4String tuplname;
  //  G4String tupltype;
  //  ntupl1 = new TTree("101", "Init and Decay");
  //  ntupl1->Branch("r_num", &r_num, "r_num/I");
  //  ntupl1->Branch("i_ev", &i_ev, "i_ev/I");
  //
  //  ntupl1->Branch("x_in", &x_in, "x_in/D");
  //  ntupl1->Branch("y_in", &y_in, "y_in/D");
  //  ntupl1->Branch("z_in", &z_in, "z_in/D");
  //
  //  ntupl1->Branch("x_dec", &x_dec, "x_dec/D");
  //  ntupl1->Branch("y_dec", &y_dec, "y_dec/D");
  //  ntupl1->Branch("z_dec", &z_dec, "z_dec/D");
  //  ntupl1->Branch("t_dec", &t_dec, "t_dec/D");
  //  ntupl1->Branch("nhits",nhits,Form("nhits[%d]/I",n_tp));
  //  ntupl1->Branch("tdc",tdc,Form("tdc[%d][%d]/D",n_tp,MAX_NHITS));
  //  ntupl1->Branch("edep",edep,Form("edep[%d][%d]/D",n_tp,MAX_NHITS));

  //  for (int itp = 0; itp < n_tp; itp++)
  //    {
  //      std::stringstream ss;
  //      ss << std::setfill('0') << std::setw(2) << itp;
  //      tuplname = "t_s" + ss.str();
  //      tupltype = tuplname + "/D";
  //      ntupl1->Branch(tuplname, &t_s[itp], tupltype);
  //    }


  G4cout << "\n----> Histogram file is opened in " << rootFileName << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::save()
{ 
  if (rootFile) {
    rootFile->Write();       // Writing the histograms to the file
    //rootFile->Write(rootvec);
    rootFile->Close();        // and closing the tree (and the file)
    G4cout << "\n----> Histogram Tree is saved \n" << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::SetRunNumber(G4int irun)
{
  runNumber = irun;
  return;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::SetEventNumber(G4int ievt)
{
  eventNumber = ievt;
  return;
}

void HistoManager::SetEventsRequested(G4int ievt)
{
  eventsRequested = ievt;
  return;
}

void HistoManager::ZeroEventsPrimary()
{
  eventsPrimaryAll = 0;
  eventsPrimaryFail = 0;
  return;
}

void HistoManager::IncEventsPrimaryAll()
{
  eventsPrimaryAll += 1;
  return;
}

void HistoManager::IncEventsPrimaryFail()
{
  eventsPrimaryFail += 1;
  return;
}

//void HistoManager::SetEMuZeroFormation(G4double EMuF)
//{
//  EMuZeroFormation = EMuF;
//  return;
//}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::FillHisto(G4int ih, G4double xbin, G4double weight)
{
  if (ih >= Max1D) {
    G4cout << "---> warning from HistoManager::FillHisto() : h1D " << ih
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << G4endl;
    return;
  }
  if  (h1D[ih]) { h1D[ih]->Fill(xbin, weight); }
}

void HistoManager::Fill2DHisto(G4int i2D, G4double xbin, G4double ybin, G4double weight)
{
  if (i2D >= Max2D) {
    G4cout << "---> warning from HistoManager::Fill2DHisto() : h2D " << i2D
           << " does not exist. (xbin=" << xbin << " weight=" << weight << ")"
           << G4endl;
    return;
  }
  if  (h2D[i2D]) { h2D[i2D]->Fill(xbin, ybin, weight); }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Normalize(G4int ih, G4double fac)
{
  if (ih >= Max1D) {
    G4cout << "---> warning from HistoManager::Normalize() : h1D " << ih
           << " does not exist. (fac=" << fac << ")" << G4endl;
    return;
  }
  if (h1D[ih]) h1D[ih]->Scale(fac);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//void HistoManager::FillNtuple1(G4int rnum, G4int iev, G4double xin, G4double yin, G4double zin,
//			       G4double xdec, G4double ydec, G4double zdec, G4double tdec, 
//			       G4int nh[], G4double ts[][MAX_NHITS], G4double ed[][MAX_NHITS], G4int ntp)
//{
//  r_num = rnum; i_ev = iev;
//  x_in = xin; y_in = yin; z_in = zin;
//  x_dec = xdec; y_dec = ydec; z_dec = zdec;
//  t_dec = tdec;
//
//  for (int itp = 0; itp < ntp; itp++)
//    {
//      nhits[itp] = nh[itp];
//      // fill _all_ elements so zeros from ts and ed 
//      // are copied to (uninitialized) tdc and edep variables
//      for (G4int ihit = 0; ihit < MAX_NHITS; ihit++)
//	{
//	  tdc[itp][ihit] = ts[itp][ihit];
//	  edep[itp][ihit] = ed[itp][ihit];
//	} 
//    }
//
//  if (ntupl1) ntupl1->Fill();
//}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::PrintStatistic()
{
  if(h1D[1]) {      
    G4cout << "\n ----> print event statistics \n" << G4endl;

    G4cout 
      << " Run number="  << runNumber
      << " Events requested="  << eventsRequested
      << G4endl;  

    G4cout 
      << " Primary events: All=" << eventsPrimaryAll
      << " Failed=" << eventsPrimaryFail
      << G4endl;  

    G4cout << "\n ----> print histograms statistics \n" << G4endl;
    
    G4cout 
      << " x_init mean: " << G4BestUnit(h1D[0]->GetMean(), "Length") 
      << " rms: " << G4BestUnit(h1D[0]->GetRMS(),  "Length") << G4endl;
    G4cout                
      << " y_init mean: " << G4BestUnit(h1D[1]->GetMean(), "Length") 
      << " rms: " << G4BestUnit(h1D[1]->GetRMS(),  "Length") << G4endl;
    G4cout 
      << " z_init mean: " << G4BestUnit(h1D[2]->GetMean(), "Length") 
      << " rms: " << G4BestUnit(h1D[2]->GetRMS(),  "Length") << G4endl;

    G4double targetDensity;
    G4int muonDecays;
    G4int MuDecays;
    G4int muonDecaysTarget;
    G4int MuDecaysTarget;
    G4int MuEmissions;

    //targetDensity = 0.;
    targetDensity = fDetConstruction->GetPerfLyrDn()/(CLHEP::mg/CLHEP::cm3);
    muonDecays = h1D[11]->GetEntries();  // zmuonDecay
    MuDecays = h1D[39]->GetEntries();   // zMuDecay
    muonDecaysTarget = h1D[11]->Integral(180,219);  // zmuonDecay [-10mm,10mm], 400ch=[-100mm,100mm]
    MuDecaysTarget = h1D[39]->Integral(3200,4799);   // zmuonDecay [-10mm,10mm], 8000ch=[-50mm,50mm]
    MuEmissions = h1D[45]->GetEntries();     // z_Mudecay_em

    G4cout 
      << " z_muondecay entries: " << muonDecays
      << " mean: " << G4BestUnit(h1D[11]->GetMean(), "Length") 
      << " rms: " << G4BestUnit(h1D[11]->GetRMS(),  "Length") << G4endl;
    G4cout 
      << " z_Mudecay entries: " << MuDecays
      << " mean: " << G4BestUnit(h1D[39]->GetMean(), "Length") 
      << " rms: " << G4BestUnit(h1D[39]->GetRMS(),  "Length") << G4endl;

    G4cout 
      << " Target muon decays: " << muonDecaysTarget << ", Target Mu decays: " << MuDecaysTarget  << G4endl;

    G4cout 
      << " Mu emissions: " << MuEmissions << G4endl;

    if ( logFileName.size() ) {
      FILE * outfile;
      outfile = fopen(logFileName,"a");
      if (outfile != NULL) {
	//fprintf(outfile,"%6.3f %6.3f %6.3f %6.3f %6.3f %d %d %d %d %d %d \n",
	fprintf(outfile, "%d %d %12.4f %d %d  %d %d %d \n", runNumber, eventsRequested,
		targetDensity, muonDecays, MuDecays, muonDecaysTarget, MuDecaysTarget, MuEmissions);
	fclose(outfile);
      }
 
    }

  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
