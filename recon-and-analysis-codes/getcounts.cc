#include "anal_gate_PETscanner.h"

int main(int argc, char **argv)
{
  //
  // get root filename
  //
  char title[100];
  sprintf( title, "%s.root", argv[1]);
  init_data( title);

  //
  // get energy window
  //
  float lld=0, uld=750;
  if (argc>=4) {
     sscanf(argv[2], "%f", &lld);
     sscanf(argv[3], "%f", &uld);
  }
  lld /= 1000., uld /= 1000.;            // convert from keV to MeV

  // 
  // count events
  //
  float ntrue = 0;
  float nscat = 0;  
  float nrand = 0;

  for (long loop=0; loop<tree_c->GetEntries(); ++loop)
  {
     tree_c->GetEntry(loop);
     if (coinc.energy1>=lld && coinc.energy1<=uld &&
         coinc.energy2>=lld && coinc.energy2<=uld)
     {
        if (coinc.eventID1 != coinc.eventID2) nrand+=1;
        else if (coinc.comptonPhantom1!=0 || coinc.comptonPhantom2!=0
           || coinc.RayleighPhantom1!=0 || coinc.RayleighPhantom2!=0) nscat+=1;
        else ntrue+=1;
     }
  }
  file->Close();
  float nec = ntrue? ntrue*ntrue/(ntrue+nscat+nrand): 0;
  cout << ntrue  << " " << nscat << " " << nrand << " " << nec;
  return 0;
}
