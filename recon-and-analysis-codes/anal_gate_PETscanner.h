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

// Hit tree variables
struct hit_tree {
	int	PDGEncoding;
	char	RayleighVolName[40];
	float	axialPos;
	int	baseID;
	char	comptVolName[40];
	float	edep;
	int	eventID;
	int	layerID;
	int	level1ID;
	int	level2ID;
	int	level3ID;
	int	level4ID;
	float	localPosX;
	float	localPosY;
	float	localPosZ;
	int	nCrystalCompton;
	int	nCrystalRayleigh;
	int	nPhantomCompton;
	int	nPhantomRayleigh;
	int	parentID;
	int	photonID;
	float	posX;
	float	posY;
	float	posZ;
	int	primaryID;
	char	processName[40];
	float	rotationAngle;
	int	runID;
	int	sourceID;
	float	sourcePosX;
	float	sourcePosY;
	float	sourcePosZ;
	float	stepLength;
	double	time;
	int	trackID;
	int	volumeID[10];
} hit;

// Single tree variables
struct single_tree {
	int	RayleighCrystal;
	int	RayleighPhantom;
	char	RayleighVolName[40];
	float	axialPos;
	int	baseID;
	char	comptVolName[40];
	int	comptonCrystal;
	int	comptonPhantom;
	float	energy;
	int	eventID;
	float	globalPosX;
	float	globalPosY;
	float	globalPosZ;
	int	layerID;
	int	level1ID;
	int	level2ID;
	int	level3ID;
	int	level4ID;
	float	rotationAngle;
	int	runID;
	int	sourceID;
	float	sourcePosX;
	float	sourcePosY;
	float	sourcePosZ;
	double	time;
} single;

// Coincidence tree variables
struct coinc_tree {
	int	RayleighCrystal1;
	int	RayleighCrystal2;
	int	RayleighPhantom1;
	int	RayleighPhantom2;
	char	RayleighVolName1[40];
	char	RayleighVolName2[40];
	float	axialPos;
	int	baseID1;
	int	baseID2;
	char	comptVolName1[40];
	char	comptVolName2[40];
	int	comptonCrystal1;
	int	comptonCrystal2;
	int	comptonPhantom1;
	int	comptonPhantom2;
	float	energy1;
	float	energy2;
	int	eventID1;
	int	eventID2;
	float	globalPosX1;
	float	globalPosX2;
	float	globalPosY1;
	float	globalPosY2;
	float	globalPosZ1;
	float	globalPosZ2;
	int	layerID1;
	int	layerID2;
	int	level1ID1;
	int	level1ID2;
	int	level2ID1;
	int	level2ID2;
	int	level3ID1;
	int	level3ID2;
	int	level4ID1;
	int	level4ID2;
	float	rotationAngle;
	int	runID;
	float	sinogramS;
	float	sinogramTheta;
	int	sourceID1;
	int	sourceID2;
	float	sourcePosX1;
	float	sourcePosX2;
	float	sourcePosY1;
	float	sourcePosY2;
	float	sourcePosZ1;
	float	sourcePosZ2;
	double	time1;
	double	time2;
} coinc;

// Gate variables
struct gate_tree {
	float     event;
	float     iontime;
	float     poskinene;
	float     posannihildist;
} gate;

TFile* file;
TTree* tree_h, *tree_s, *tree_c, *tree_g, *tree_d;
int init_data( char* filename)
{
  // open file
  file = new TFile( filename);
  
  // get a hit
  tree_h = (TTree*)file->Get("Hits");
  tree_h->SetBranchAddress("PDGEncoding",      &hit.PDGEncoding);
  tree_h->SetBranchAddress("RayleighVolName",  hit.RayleighVolName);
  tree_h->SetBranchAddress("axialPos",         &hit.axialPos);
  tree_h->SetBranchAddress("baseID",           &hit.baseID);
  tree_h->SetBranchAddress("comptVolName",     hit.comptVolName);
  tree_h->SetBranchAddress("edep",             &hit.edep);
  tree_h->SetBranchAddress("eventID",          &hit.eventID);
  tree_h->SetBranchAddress("layerID",          &hit.layerID);
  tree_h->SetBranchAddress("level1ID",         &hit.level1ID);
  tree_h->SetBranchAddress("level2ID",         &hit.level2ID);
  tree_h->SetBranchAddress("level3ID",         &hit.level3ID);
  tree_h->SetBranchAddress("level4ID",         &hit.level4ID);
  tree_h->SetBranchAddress("localPosX",        &hit.localPosX);
  tree_h->SetBranchAddress("localPosY",        &hit.localPosY);
  tree_h->SetBranchAddress("localPosZ",        &hit.localPosZ);
  tree_h->SetBranchAddress("nCrystalCompton",  &hit.nCrystalCompton);
  tree_h->SetBranchAddress("nCrystalRayleigh", &hit.nCrystalRayleigh);
  tree_h->SetBranchAddress("nPhantomCompton",  &hit.nPhantomCompton);
  tree_h->SetBranchAddress("nPhantomRayleigh", &hit.nPhantomRayleigh);
  tree_h->SetBranchAddress("parentID",         &hit.parentID);
  tree_h->SetBranchAddress("photonID",         &hit.photonID);
  tree_h->SetBranchAddress("posX",             &hit.posX);
  tree_h->SetBranchAddress("posY",             &hit.posY);
  tree_h->SetBranchAddress("posZ",             &hit.posZ);
  tree_h->SetBranchAddress("primaryID",        &hit.primaryID);
  tree_h->SetBranchAddress("processName",      hit.processName);
  tree_h->SetBranchAddress("rotationAngle",    &hit.rotationAngle);
  tree_h->SetBranchAddress("runID",            &hit.runID);
  tree_h->SetBranchAddress("sourceID",         &hit.sourceID);
  tree_h->SetBranchAddress("sourcePosX",       &hit.sourcePosX);
  tree_h->SetBranchAddress("sourcePosY",       &hit.sourcePosY);
  tree_h->SetBranchAddress("sourcePosZ",       &hit.sourcePosZ);
  tree_h->SetBranchAddress("stepLength",       &hit.stepLength);
  tree_h->SetBranchAddress("time",             &hit.time);
  tree_h->SetBranchAddress("trackID",          &hit.trackID);
  tree_h->SetBranchAddress("volumeID",         &hit.volumeID);

  // Singles
  tree_s = (TTree*)file->Get("Singles");
  tree_s->SetBranchAddress("RayleighCrystal", &single.RayleighCrystal);
  tree_s->SetBranchAddress("RayleighPhantom", &single.RayleighPhantom);
  tree_s->SetBranchAddress("RayleighVolName", single.RayleighVolName);
  tree_s->SetBranchAddress("axialPos",        &single.axialPos);
  tree_s->SetBranchAddress("baseID",          &single.baseID);
  tree_s->SetBranchAddress("comptVolName",    single.comptVolName);
  tree_s->SetBranchAddress("comptonCrystal",  &single.comptonCrystal);
  tree_s->SetBranchAddress("comptonPhantom",  &single.comptonPhantom);
  tree_s->SetBranchAddress("energy",          &single.energy);
  tree_s->SetBranchAddress("eventID",         &single.eventID);
  tree_s->SetBranchAddress("globalPosX",      &single.globalPosX);
  tree_s->SetBranchAddress("globalPosY",      &single.globalPosY);
  tree_s->SetBranchAddress("globalPosZ",      &single.globalPosZ);
  tree_s->SetBranchAddress("layerID",         &single.layerID);
  tree_s->SetBranchAddress("level1ID",        &single.level1ID);
  tree_s->SetBranchAddress("level2ID",        &single.level2ID);
  tree_s->SetBranchAddress("level3ID",        &single.level3ID);
  tree_s->SetBranchAddress("level4ID",        &single.level4ID);
  tree_s->SetBranchAddress("rotationAngle",   &single.rotationAngle);
  tree_s->SetBranchAddress("runID",           &single.runID);
  tree_s->SetBranchAddress("sourceID",        &single.sourceID);
  tree_s->SetBranchAddress("sourcePosX",      &single.sourcePosX);  
  tree_s->SetBranchAddress("sourcePosY",      &single.sourcePosY);  
  tree_s->SetBranchAddress("sourcePosZ",      &single.sourcePosZ);  
  tree_s->SetBranchAddress("time",            &single.time);

  // Coincidences
  tree_c = (TTree*)file->Get("Coincidences");
  tree_c->SetBranchAddress("RayleighCrystal1", &coinc.RayleighCrystal1);
  tree_c->SetBranchAddress("RayleighCrystal2", &coinc.RayleighCrystal2);
  tree_c->SetBranchAddress("RayleighPhantom1", &coinc.RayleighPhantom1);
  tree_c->SetBranchAddress("RayleighPhantom2", &coinc.RayleighPhantom2);
  tree_c->SetBranchAddress("RayleighVolName1", coinc.RayleighVolName1);
  tree_c->SetBranchAddress("RayleighVolName2", coinc.RayleighVolName2);
  tree_c->SetBranchAddress("axialPos",         &coinc.axialPos);
  tree_c->SetBranchAddress("baseID1",          &coinc.baseID1);
  tree_c->SetBranchAddress("baseID2",          &coinc.baseID2);
  tree_c->SetBranchAddress("comptonCrystal1",  &coinc.comptonCrystal1);
  tree_c->SetBranchAddress("comptonCrystal2",  &coinc.comptonCrystal2);
  tree_c->SetBranchAddress("comptonPhantom1",  &coinc.comptonPhantom1);
  tree_c->SetBranchAddress("comptonPhantom2",  &coinc.comptonPhantom2);
  tree_c->SetBranchAddress("comptVolName1",    coinc.comptVolName1);
  tree_c->SetBranchAddress("comptVolName2",    coinc.comptVolName2);
  tree_c->SetBranchAddress("energy1",          &coinc.energy1);
  tree_c->SetBranchAddress("energy2",          &coinc.energy2);
  tree_c->SetBranchAddress("eventID1",         &coinc.eventID1);
  tree_c->SetBranchAddress("eventID2",         &coinc.eventID2);
  tree_c->SetBranchAddress("globalPosX1",      &coinc.globalPosX1);
  tree_c->SetBranchAddress("globalPosX2",      &coinc.globalPosX2);
  tree_c->SetBranchAddress("globalPosY1",      &coinc.globalPosY1);
  tree_c->SetBranchAddress("globalPosY2",      &coinc.globalPosY2);
  tree_c->SetBranchAddress("globalPosZ1",      &coinc.globalPosZ1);
  tree_c->SetBranchAddress("globalPosZ2",      &coinc.globalPosZ2);
  tree_c->SetBranchAddress("layerID1",         &coinc.layerID1);
  tree_c->SetBranchAddress("layerID2",         &coinc.layerID2);
  tree_c->SetBranchAddress("level1ID1",        &coinc.level1ID1);
  tree_c->SetBranchAddress("level1ID2",        &coinc.level1ID2);
  tree_c->SetBranchAddress("level2ID1",        &coinc.level2ID1);
  tree_c->SetBranchAddress("level2ID2",        &coinc.level2ID2);
  tree_c->SetBranchAddress("level3ID1",        &coinc.level3ID1);
  tree_c->SetBranchAddress("level3ID2",        &coinc.level3ID2);
  tree_c->SetBranchAddress("level4ID1",        &coinc.level4ID1);
  tree_c->SetBranchAddress("level4ID2",        &coinc.level4ID2);
  tree_c->SetBranchAddress("rotationAngle",    &coinc.rotationAngle);
  tree_c->SetBranchAddress("runID",            &coinc.runID);
  tree_c->SetBranchAddress("sinogramS",        &coinc.sinogramS);
  tree_c->SetBranchAddress("sinogramTheta",    &coinc.sinogramTheta);
  tree_c->SetBranchAddress("sourceID1",        &coinc.sourceID1);
  tree_c->SetBranchAddress("sourceID2",        &coinc.sourceID2);
  tree_c->SetBranchAddress("sourcePosX1",      &coinc.sourcePosX1);
  tree_c->SetBranchAddress("sourcePosX2",      &coinc.sourcePosX2);
  tree_c->SetBranchAddress("sourcePosY1",      &coinc.sourcePosY1);
  tree_c->SetBranchAddress("sourcePosY2",      &coinc.sourcePosY2);
  tree_c->SetBranchAddress("sourcePosZ1",      &coinc.sourcePosZ1);
  tree_c->SetBranchAddress("sourcePosZ2",      &coinc.sourcePosZ2);
  tree_c->SetBranchAddress("time1",            &coinc.time1);
  tree_c->SetBranchAddress("time2",            &coinc.time2);

  // gate tree
  tree_g = (TTree*)file->Get("Gate");
  tree_g->SetBranchAddress("event",          &gate.event);
  tree_g->SetBranchAddress("iontime",        &gate.iontime);
  tree_g->SetBranchAddress("poskinene",      &gate.poskinene);
  tree_g->SetBranchAddress("posannihildist", &gate.posannihildist);
  
  return 0;
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

TCanvas*      c1;
TPad*         pad1;
TStyle*       style;
TPostScript*  psf1;

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
