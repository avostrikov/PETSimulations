#include <fstream>
#include <iomanip>
#include <string>
#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TPostScript.h>
#include <TAxis.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TF1.h>
#include <TStyle.h>
#include <TProfile.h>
#include <TMapFile.h>
#include <TMath.h>
#include <TPaletteAxis.h>
#include <TPaveStats.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>  
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

using namespace::std;

TCanvas*      c1;
TPad*         pad1;
TStyle*       style;
TPostScript*  psf1;

TFile* file;
TTree* tree_h, *tree_s, *tree_c, *tree_g, *tree_d;

// Hit tree variables
int   sourceID_h; 
int   eventID_h;
int   runID_h; 
int   PDGEncoding_h;
int   trackID_h;
int   parentID_h;
int   photonID_h; 
int   nPhantomCompton_h; 
int   nCrystalCompton_h; 
int   nPhantomRayleigh_h;
int   nCrystalRayleigh_h;
int   primaryID_h; 
int   volumeID_h[10]; 
int   gantryID_h;
int   rsectorID_h;
int   moduleID_h;
int   submoduleID_h;
int   crystalID_h;
int   layerID_h;

double time_h; 

float  edep_h;
float  stepLength_h;
float  posX_h; 
float  posY_h; 
float  posZ_h; 
float  localPosX_h; 
float  localPosY_h; 
float  localPosZ_h; 
float  sourcePosX_h; 
float  sourcePosY_h; 
float  sourcePosZ_h; 
float  axialPos_h; 
float  rotationAngle_h; 

char  processName_h[40]; 
char  comptVolName_h[40]; 
char  RayleighVolName_h[40]; 

// Single tree variables
int runID_s; 
int eventID_s;
int sourceID_s;
int comptonPhantom_s;
int comptonCrystal_s;
int RayleighPhantom_s;
int RayleighCrystal_s;
int gantryID_s;
int rsectorID_s;
int moduleID_s;
int submoduleID_s;
int crystalID_s;
int layerID_s;

double time_s;  

float sourcePosX_s;
float sourcePosY_s;
float sourcePosZ_s;
float energy_s; 
float globalPosX_s; 
float globalPosY_s; 
float globalPosZ_s; 

float axialPos_s;  
float rotationAngle_s; 
char  comptVolName_s[40]; 
char  RayleighVolName_s[40];  

// Coincidence tree variables
int           runID;
float         axialPos;
float         rotationAngle;
int           eventID1;
int           sourceID1;
float         sourcePosX1;
float         sourcePosY1;
float         sourcePosZ1;
double        time1;
float         energy1;
float         globalPosX1;
float         globalPosY1;
float         globalPosZ1;
int           gantryID1;
int           rsectorID1;
int           moduleID1;
int           submoduleID1;
int           crystalID1;
int           layerID1;
int           comptonPhantom1;
int           comptonCrystal1;
int           RayleighPhantom1;
int           RayleighCrystal1;

int           eventID2;
int           sourceID2;
float         sourcePosX2;
float         sourcePosY2;
float         sourcePosZ2;
double        time2;
float         energy2;   
float         globalPosX2;
float         globalPosY2;
float         globalPosZ2;
int           gantryID2;
int           rsectorID2;
int           moduleID2;
int           submoduleID2;
int           crystalID2;
int           layerID2;
int           comptonPhantom2;
int           comptonCrystal2;
int           RayleighCrystal2;
int           RayleighPhantom2;

float         sinogramTheta;
float         sinogramS;
char          comptVolName1[40];
char          comptVolName2[40];
char          RayleighVolName1[40];
char          RayleighVolName2[40];

// Gate variables
float     event_g;
float     iontime_g;
float     poskinene_g;
float     posannihildist_g;

int init_data( char* filename){

  file = new TFile( filename);
  
  // Hits
  tree_h = (TTree*)file->Get("Hits");
  tree_h->SetBranchAddress("PDGEncoding", &PDGEncoding_h);
  tree_h->SetBranchAddress("trackID", &trackID_h);
  tree_h->SetBranchAddress("parentID", &parentID_h);
  tree_h->SetBranchAddress("time", &time_h);
  tree_h->SetBranchAddress("edep", &edep_h);
  tree_h->SetBranchAddress("stepLength", &stepLength_h);
  tree_h->SetBranchAddress("posX", &posX_h);
  tree_h->SetBranchAddress("posY", &posY_h);
  tree_h->SetBranchAddress("posZ", &posZ_h);
  tree_h->SetBranchAddress("localPosX", &localPosX_h);
  tree_h->SetBranchAddress("localPosY", &localPosY_h);
  tree_h->SetBranchAddress("localPosZ", &localPosZ_h);
  tree_h->SetBranchAddress("gantryID", &gantryID_h);
  tree_h->SetBranchAddress("rsectorID", &rsectorID_h);
  tree_h->SetBranchAddress("moduleID", &moduleID_h);
  tree_h->SetBranchAddress("submoduleID", &submoduleID_h);
  tree_h->SetBranchAddress("crystalID", &crystalID_h);
  tree_h->SetBranchAddress("layerID", &layerID_h);
  tree_h->SetBranchAddress("photonID", &photonID_h);
  tree_h->SetBranchAddress("nPhantomCompton", &nPhantomCompton_h);
  tree_h->SetBranchAddress("nCrystalCompton", &nCrystalCompton_h);
  tree_h->SetBranchAddress("nPhantomRayleigh", &nPhantomRayleigh_h);
  tree_h->SetBranchAddress("nCrystalRayleigh", &nCrystalRayleigh_h);
  tree_h->SetBranchAddress("primaryID", &primaryID_h);
  tree_h->SetBranchAddress("sourcePosX", &sourcePosX_h);
  tree_h->SetBranchAddress("sourcePosY", &sourcePosY_h);
  tree_h->SetBranchAddress("sourcePosZ", &sourcePosZ_h);
  tree_h->SetBranchAddress("sourceID", &sourceID_h);
  tree_h->SetBranchAddress("eventID", &eventID_h);
  tree_h->SetBranchAddress("runID", &runID_h);
  tree_h->SetBranchAddress("axialPos", &axialPos_h);
  tree_h->SetBranchAddress("rotationAngle", &rotationAngle_h);
  tree_h->SetBranchAddress("volumeID", volumeID_h);
  tree_h->SetBranchAddress("processName", processName_h);
  tree_h->SetBranchAddress("comptVolName", comptVolName_h);
  tree_h->SetBranchAddress("RayleighVolName", RayleighVolName_h);

  // Singles
  tree_s = (TTree*)file->Get("Singles");
  tree_s->SetBranchAddress("runID",          &runID_s);
  tree_s->SetBranchAddress("eventID",        &eventID_s);
  tree_s->SetBranchAddress("sourceID",       &sourceID_s);
  tree_s->SetBranchAddress("sourcePosX",     &sourcePosX_s);
  tree_s->SetBranchAddress("sourcePosY",     &sourcePosY_s);
  tree_s->SetBranchAddress("sourcePosZ",     &sourcePosZ_s);
  tree_s->SetBranchAddress("time",           &time_s);
  tree_s->SetBranchAddress("energy",         &energy_s);
  tree_s->SetBranchAddress("globalPosX",     &globalPosX_s);
  tree_s->SetBranchAddress("globalPosY",     &globalPosY_s);
  tree_s->SetBranchAddress("globalPosZ",     &globalPosZ_s);
  tree_s->SetBranchAddress("gantryID", &gantryID_s);
  tree_s->SetBranchAddress("rsectorID", &rsectorID_s);
  tree_s->SetBranchAddress("moduleID", &moduleID_s);
  tree_s->SetBranchAddress("submoduleID", &submoduleID_s);
  tree_s->SetBranchAddress("crystalID", &crystalID_s);
  tree_s->SetBranchAddress("layerID", &layerID_s);  
  tree_s->SetBranchAddress("comptonPhantom", &comptonPhantom_s);
  tree_s->SetBranchAddress("comptonCrystal", &comptonCrystal_s);
  tree_s->SetBranchAddress("RayleighPhantom", &RayleighPhantom_s);
  tree_s->SetBranchAddress("RayleighCrystal", &RayleighCrystal_s);
  tree_s->SetBranchAddress("axialPos",       &axialPos_s);
  tree_s->SetBranchAddress("rotationAngle",  &rotationAngle_s);
  tree_s->SetBranchAddress("comptVolName",   comptVolName_s);
  tree_s->SetBranchAddress("RayleighVolName",   RayleighVolName_s);

  // Coincidences
  tree_c = (TTree*)file->Get("Coincidences");
  tree_c->SetBranchAddress("runID",&runID);
  tree_c->SetBranchAddress("axialPos",&axialPos);
  tree_c->SetBranchAddress("rotationAngle",&rotationAngle);
  tree_c->SetBranchAddress("eventID1",&eventID1);
  tree_c->SetBranchAddress("sourceID1",&sourceID1);
  tree_c->SetBranchAddress("sourcePosX1",&sourcePosX1);
  tree_c->SetBranchAddress("sourcePosY1",&sourcePosY1);
  tree_c->SetBranchAddress("sourcePosZ1",&sourcePosZ1);
  tree_c->SetBranchAddress("time1",&time1);
  tree_c->SetBranchAddress("energy1",&energy1);
  tree_c->SetBranchAddress("globalPosX1",&globalPosX1);
  tree_c->SetBranchAddress("globalPosY1",&globalPosY1);
  tree_c->SetBranchAddress("globalPosZ1",&globalPosZ1);
  tree_c->SetBranchAddress("gantryID1",&gantryID1);
  tree_c->SetBranchAddress("rsectorID1",&rsectorID1);
  tree_c->SetBranchAddress("moduleID1",&moduleID1);
  tree_c->SetBranchAddress("submoduleID1",&submoduleID1);
  tree_c->SetBranchAddress("crystalID1",&crystalID1);
  tree_c->SetBranchAddress("layerID1",&layerID1);
  tree_c->SetBranchAddress("comptonPhantom1",&comptonPhantom1);
  tree_c->SetBranchAddress("comptonCrystal1",&comptonCrystal1);
  tree_c->SetBranchAddress("RayleighPhantom1",&RayleighPhantom1);
  tree_c->SetBranchAddress("RayleighCrystal1",&RayleighCrystal1);

  tree_c->SetBranchAddress("eventID2",&eventID2);
  tree_c->SetBranchAddress("sourceID2",&sourceID2);
  tree_c->SetBranchAddress("sourcePosX2",&sourcePosX2);
  tree_c->SetBranchAddress("sourcePosY2",&sourcePosY2);
  tree_c->SetBranchAddress("sourcePosZ2",&sourcePosZ2);
  tree_c->SetBranchAddress("time2",&time2);
  tree_c->SetBranchAddress("energy2",&energy2);   
  tree_c->SetBranchAddress("globalPosX2",&globalPosX2);
  tree_c->SetBranchAddress("globalPosY2",&globalPosY2);      
  tree_c->SetBranchAddress("globalPosZ2",&globalPosZ2);
  tree_c->SetBranchAddress("gantryID2",&gantryID2);
  tree_c->SetBranchAddress("rsectorID2",&rsectorID2);
  tree_c->SetBranchAddress("moduleID2",&moduleID2);
  tree_c->SetBranchAddress("submoduleID2",&submoduleID2);
  tree_c->SetBranchAddress("crystalID2",&crystalID2);
  tree_c->SetBranchAddress("layerID2",&layerID2);
  tree_c->SetBranchAddress("comptonPhantom2",&comptonPhantom2);
  tree_c->SetBranchAddress("comptonCrystal2",&comptonCrystal2);
  tree_c->SetBranchAddress("RayleighPhantom2",&RayleighPhantom2);
  tree_c->SetBranchAddress("RayleighCrystal2",&RayleighCrystal2);

  tree_c->SetBranchAddress("sinogramTheta",&sinogramTheta);
  tree_c->SetBranchAddress("sinogramS",&sinogramS);

  tree_c->SetBranchAddress("comptVolName1",&comptVolName1);
  tree_c->SetBranchAddress("comptVolName2",&comptVolName2);
  tree_c->SetBranchAddress("RayleighVolName1",&RayleighVolName1);
  tree_c->SetBranchAddress("RayleighVolName2",&RayleighVolName2);

  tree_g = (TTree*)file->Get("Gate");
  tree_g->SetBranchAddress("event",&event_g);
  tree_g->SetBranchAddress("iontime",&iontime_g);
  tree_g->SetBranchAddress("poskinene",&poskinene_g);
  tree_g->SetBranchAddress("posannihildist",&posannihildist_g);
  
  return 0;
}

int
graphic_init( void){

  style = new TStyle("style", "style");
  
  style->SetLabelFont(132,"X");
  style->SetLabelFont(132,"Y");
  style->SetTitleFont(132,"X");
  style->SetTitleFont(132,"Y");
  style->SetTitleFont(132,"");
  style->SetTitleFontSize( 0.07);
  style->SetStatFont(132);
  style->GetAttDate()->SetTextFont(132);

  style->SetStatW(0.23);
  style->SetStatH(0.25);

  style->SetFuncColor(2);
  style->SetFuncWidth(2);
  style->SetLineWidth(2);
  
  style->SetOptFile(0);
  style->SetOptTitle(1);

  style->SetFrameBorderMode(0);
  style->SetCanvasBorderMode(0);
  style->SetPadBorderMode(0);
  style->SetTitleStyle(4000);
  style->SetPadColor(0);
  style->SetCanvasColor(0);

  style->SetTitleFillColor(0);

  style->SetTitleBorderSize(0);
  style->SetStatColor(0);
  style->SetStatBorderSize(1);

  style->SetOptStat("emri");
  // style->SetOptStat(0);
  style->SetOptFit(1);
  style->SetTitleOffset( 1.0,"Y");

  style->SetMarkerStyle(20);
  style->SetMarkerSize( 1.0);
  style->SetMarkerColor(4);

  // style->SetOptDate(21);
  // style->SetPadGridX(1);
  // style->SetPadGridY(1);

  style->cd();

  return 0;
}


TH1F *h_energy1, *h_energy2, *h_edep;
TH1F *h_en1, *h_en2;

int hist_init( void){

  char stitle[100];

  sprintf( stitle, "h_energy1");
  h_energy1 = new TH1F( stitle, stitle, 200, 0, 1.0);

  sprintf( stitle, "h_energy2");
  h_energy2 = new TH1F( stitle, stitle, 200, 0, 1.0);

  sprintf( stitle, "h_en1");
  h_en1 = new TH1F( stitle, stitle, 200, 0, 1.0);

  sprintf( stitle, "h_en2");
  h_en2 = new TH1F( stitle, stitle, 200, 0, 1.0);

  sprintf( stitle, "h_edep");
  h_edep = new TH1F( stitle, stitle, 200, 0, 1.0);

  return 0;
}
