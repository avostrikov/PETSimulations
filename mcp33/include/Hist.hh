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

  void fill_ntrack( G4int);
  void fill_ndet( G4int);
  void fill_nside( G4int);
  void fill_nbulk( G4int);
  void fill_nint0( );
  void fill_nint1( );
  void fill_nint2( );

  void fill_length( G4double);
  void fill_length2( G4double);

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

  TH1F *h1, *h_ncompt, *h_nphtoe;
  TH1F *h_ntrack, *h_ndet, *h_nbulk, *h_nside;
  TH1F *h_nint0, *h_nint1, *h_nint2;
  TH1F *h_length, *h_length2;

};
#endif



