// plot simulation results

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



TCanvas*      c1;
TPad*         pad1;
TStyle*       style;
TPostScript*  psf1;

TFile* file;
TTree* tree;

int graphic_init();


int 
main(int argc, char **argv){

  graphic_init();

  c1 = new TCanvas("canvas", "TITLE", 10, 10, 700, 500);
  c1->SetFillStyle( 4000);
  c1->Divide( 2, 2);

  // histogram # of photon hits in each photodetector
  // 0 -> up (z>0) ; 1 -> down (z<0)
  TH1F* hit_0 = new TH1F("hit_0","# hits 0", 500,0.,5000.);
  TH1F* hit_1 = new TH1F("hit_1","# hits 1", 500,0.,5000.);
  
  // histogram x position of hits
  //TH1F* x_0 = new TH1F("x_0","x position 0", 100,-15.,15.);
  //TH1F* x_1 = new TH1F("x_1","x position 1", 100,-15.,15.);
  
  //histogram z position
  TH1F* z_pos = new TH1F("z_pos", "z position" , 120, -60.,60.);
  
  //histogram arrival time at detector
  TH1F* h_t = new TH1F("time", "timing", 5050/2,-5.,500.); 

  //energy histogram...
  TH1F* E_photon = new TH1F("E_photon","Photon Spectrum", 100,250.,750.);


  int evt;
  double tt, xx, yy, zz, en;

  TBranch* t_evt;
  TBranch* t_tt;
  //TBranch* t_xx;
  //TBranch* t_yy;
  TBranch* t_zz;
  TBranch* t_en;

  int event = 0;
  int count = 0;
  int count_up =0;
  int count_down = 0;
  int total_count = 0;

  //implement SiPM QE
  //




  //loop for files if necessary
  //

  char title[100];
  sprintf( title,"vis.root");
  printf("data read from = %s \n" ,title);
  
  file = new TFile(title);
  
  //printf("file name = %s \n",file);
  
  tree = (TTree*)file->Get("sipm");
  t_evt = tree->GetBranch("evt");
  //t_xx = tree->GetBranch("xx"); 
  // t_yy = tree->GetBranch("yy");
  t_zz = tree->GetBranch("zz");
  t_en = tree->GetBranch("en");
  t_tt = tree->GetBranch("tt");
  
  t_evt->SetAddress( &evt);
  //t_xx->SetAddress( &xx);
  //t_yy->SetAddress(&yy);
  t_zz->SetAddress( &zz);
  t_tt->SetAddress( &tt);
  t_en->SetAddress( &en);
  

  for(int loop=0; loop < tree->GetEntries(); loop ++){
    tree->GetEntry(0);
    //printf("loop = %i, energy = %f  \n",loop, en);

    E_photon->Fill(en);
    
    if(evt == 0){
      h_t->Fill( tt);
    }


    printf("event = %i, evt = %i \n", event, evt);
    printf("count  %i \n", count);
    
    if(evt != event){
      total_count += count;
      
      printf("total count = %i, count = %i \n", total_count, count);
      printf("count up = %i,  count down = %i \n\n", count_up, count_down);

      if(count_up > 0 && count_down >0){
	
	hit_0->Fill( count_up);
	hit_1->Fill( count_down);
      }
      count_up = 0;
      count_down = 0;
      count = 0;
      
      event++;
    }

    else{
      z_pos->Fill( zz);

       if(zz>0){
	count_up++;
	//x_0->Fill( xx);
      }
      
      if(zz<0){
	count_down++;
	//x_1->Fill (xx);
	}
    }
    count++;
   }

 char stitle[100];
 sprintf( stitle, "%s.ps", argv[0]);
 psf1 = new TPostScript( stitle, 112);
 
 printf("data here: %s \n",stitle);
 psf1->NewPage();

 c1->cd(1);
 E_photon->Draw();
 E_photon->GetXaxis()->SetTitle("nm");
 c1->Update();

 c1->cd(2);
 h_t->Draw();
 h_t->GetXaxis()->SetTitle("Time (ns)");
 c1->Update();
 
 file->Close();
 psf1->Close();
  
 //return 0 ;
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

  style->SetStatW(0.3);
  style->SetStatH(0.3);

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
  //  style->SetTitleX(0.3);
  //  style->SetTitleY(0.06);
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

  //  style->SetPadGridX(1);
  //  style->SetPadGridY(1);


  style->cd();

  return 0;
}

