#include "anal_gate.h"


int make_plot( char**);
int save_hist( char**);

int
main(int argc, char **argv){

  graphic_init();

  hist_init();

  TCanvas* c2 = new TCanvas("canvas2", "canvas2", 10, 10, 700, 500);
  c2->SetFillStyle( 4000);
  c2->Divide( 3, 3);

  char title[100];
  sprintf( title, "%s.root", argv[1]);

  init_data( title);

  for( int loop = 0; loop < tree_c->GetEntries(); loop++){
    tree_c->GetEntry(loop);

    // printf("loop = %6d,  energy = %6.2f\n", loop, energy1);

    h_energy1->Fill( energy1);
    h_energy2->Fill( energy2);

    if( energy1 > 0.450 && energy2 > 0.450){
      if (eventID1 != eventID2)
      h_en1->Fill( energy1);
      h_en2->Fill( energy2);   
    }
  }

//  printf("Sensitivity = %6.2f\n", h_en1->GetEntries()/(10000.*5)*100.);
  printf("%6.2f\n", h_en1->GetEntries()/(10000.*5)*100.);


  make_plot( argv);

  //  save_hist( argv);

  file->Close();

  return 0;

}



int make_plot(char** argv){

  c1 = new TCanvas("canvas1", "canvas1", 10, 10, 700, 500);
  c1->SetFillStyle( 4000);
  c1->Divide( 2, 2);

  char stitle[100];

  // postscript file
  sprintf( stitle, "%s_%s.ps", argv[0], argv[1]);
  psf1 = new TPostScript( stitle, 112);


  psf1->NewPage();
  c1->cd(1);
  h_energy1->Draw();
  h_energy1->Fit("gaus", "QE", "", 0.4, 0.6);

  c1->cd(2);
  h_energy2->Draw();
  h_energy2->Fit("gaus", "QE", "", 0.4, 0.6);

  c1->cd(3);
  h_en1->Draw();

  c1->cd(4);
  h_en2->Draw();

  psf1->Close();

  return 0;

}


int save_hist( char** argv){

  char stitle[100];

  // Save Histogram into a file
  sprintf( stitle, "%s_%s.root", argv[0], argv[1]);

  TFile* outfile = new TFile( stitle, "RECREATE", "Historam File");

  // h1->Write();

  outfile->Write();
  outfile->Close();

  return 0;
}

