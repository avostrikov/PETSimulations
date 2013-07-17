#include "Hist.hh"
#include "G4UnitsTable.hh"

HistManager* HistManager::instance = 0;


HistManager::HistManager()
{
  filename = "test";  
}


HistManager::~HistManager() 
{ 
}


HistManager* HistManager::getInstance()
{
  if( instance == 0) 
    instance = new HistManager;

  return instance;
}



void 
HistManager::book() 
{
  filename += ".root";
  tfile = new TFile( filename, "recreate", "TOF-PET");

  // ntuple
  t_sipm = new TTree( "sipm", "SiPM Hits");
  t_sipm->Branch( "evt", &evt, "evt/I");
  //  t_sipm->Branch( "xx", &xx, "xx/D");
  //  t_sipm->Branch( "yy", &yy, "yy/D");
  t_sipm->Branch( "zz", &zz, "zz/D");
  t_sipm->Branch( "tt", &tt, "tt/D");
  //  t_sipm->Branch( "en", &en, "en/D");

  t_scint = new TTree( "scint", "Scintillation Hits");
  t_scint->Branch( "evt", &evt, "evt/I");
  t_scint->Branch( "xx", &xx, "xx/D");
  t_scint->Branch( "yy", &yy, "yy/D");
  t_scint->Branch( "zz", &zz, "zz/D");
  t_scint->Branch( "tt", &tt, "tt/D");
  t_scint->Branch( "en", &en, "en/D");

  t_gamma0 = new TTree( "gamma0", "Gamma0 Vertex");
  t_gamma0->Branch( "evt", &evt, "evt/I");
  t_gamma0->Branch( "xx", &xx, "xx/D");
  t_gamma0->Branch( "yy", &yy, "yy/D");
  t_gamma0->Branch( "zz", &zz, "zz/D");

  t_gamma1 = new TTree( "gamma1", "Gamma1 Vertex");
  t_gamma1->Branch( "evt", &evt, "evt/I");
  t_gamma1->Branch( "xx", &xx, "xx/D");
  t_gamma1->Branch( "yy", &yy, "yy/D");
  t_gamma1->Branch( "zz", &zz, "zz/D");

  // histogram
  h1  = new TH1F( "h_1","energy", 1000, 0., 1000.);
}


void 
HistManager::fill_scint( G4int evtid,
			  G4double i, 
			  G4double j, 
			  G4double k,
			  G4double t,
			  G4double energy)
{
  evt = evtid;
  xx = i;
  yy = j;
  zz = k;
  tt = t;
  en = energy;

  t_scint->Fill();  
}


void 
HistManager::fill_sipm( G4int evtid,
			  G4double i, 
			  G4double j, 
			  G4double k,
			  G4double t,
			  G4double energy)
{
  evt = evtid;
  xx = i;
  yy = j;
  zz = k;
  tt = t;
  en = energy;

  t_sipm->Fill();  
}

void 
HistManager::fill_gamma0( G4int evtid,
			  G4double i, 
			  G4double j, 
			  G4double k)
{
  evt = evtid;
  xx = i;
  yy = j;
  zz = k;

  t_gamma0->Fill();  
}

void 
HistManager::fill_gamma1( G4int evtid,
			  G4double i, 
			  G4double j, 
			  G4double k)
{
  evt = evtid;
  xx = i;
  yy = j;
  zz = k;

  t_gamma1->Fill();  
}

void 
HistManager::fill_h1( G4double energy)
{
  h1->Fill( energy);
}


void 
HistManager::finish() 
{  
  tfile->Write(); 
  tfile->Close();
}

void 
HistManager::set_file_name(  G4String name) 
{  
  filename = name;
}

void
HistManager::get_file_name() 
{  
  printf("Histogram file name = %s\n", filename.c_str());
}






