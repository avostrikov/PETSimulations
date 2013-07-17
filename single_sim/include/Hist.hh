#ifndef HIST_HH
#define HIST_HH 1

#include "globals.hh"

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>


class HistManager
{

public:
  HistManager();
  ~HistManager();
  
  static HistManager* getInstance();
  
  void book();
  
  void fill_sipm( G4int, G4double, G4double, G4double, G4double, G4double);
  void fill_scint( G4int, G4double, G4double, G4double, G4double, G4double);
  void fill_gamma0( G4int, G4double, G4double, G4double);
  void fill_gamma1( G4int, G4double, G4double, G4double);
  void fill_h1( G4double);
  void finish();
  void set_file_name( G4String);
  void get_file_name();

private:
  static HistManager* instance;

  G4String filename;
  G4int evt;
  G4double xx, yy, zz, en, tt;

  TFile* tfile;
  TTree* t_sipm, *t_scint, *t_gamma0, *t_gamma1;

  TH1F *h1;

};
#endif



