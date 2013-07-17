//
// name: LAPPD-getwts.cc
// date: August 16, 2012
// author: Chien-Min Kao
// Description: calculate energy-based weightings
//

#include <fstream>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include "PETscanner.h"

using namespace::std;


int main(int argc, char **argv)
{
	// check syntax
	if (argc<4)
	{
		cerr << "Usage: " << argv[0] << " <weighting outfile> <ctw> <file1> <file2> ..." << endl;
		return 1;
	}

	// load the Tree library is not already loaded
	if (!TClassTable::GetDict("Tree")) gSystem->Load("libTree.so");
     
     // prepare tables for trues and background
     float lld = 0.15;                 // TFT lower energy limit, in MeV
     float uld = 0.65;                 // TFT upper energy limit, in MeV
     float ebin = 0.01;                // TFT energy bin size, in MeV
     float t = (uld-lld)/ebin;
     int nbins = int(t)+((t-int(t))!=0);     // no. of bins
     Long64_t *ca = (Long64_t *) new Long64_t[nbins*nbins];
     Long64_t *cb = (Long64_t *) new Long64_t[nbins*nbins];
     float *wts = (float *) new float[nbins*nbins];
     memset(ca, 0, sizeof(Long64_t)*nbins*nbins);
     memset(cb, 0, sizeof(Long64_t)*nbins*nbins);
     memset(wts, 0, sizeof(float)*nbins*nbins);

     // get coincidence time window, in nanoseconds
     float ctw = 0;
     sscanf(argv[2], "%f", &ctw);
     ctw *= 1.0e-9;
     if (ctw<0)
     {
          cerr << "ERROR: illegal coincidence time window" << endl;
          return 1;
     }
     
	// open rootfiles
     for (int infile=3; infile<argc; ++infile)
     {
          cerr << "Processing " << argv[infile];
		TFile *file = new TFile(argv[infile]);
		if (file->IsZombie())
		{
			cerr << "ERROR: open " << argv[infile] << " failed" << endl;
			return 2;
		}
			
		// setup access
		Coincidence c(file);
		cerr << " (" << c.GetNoOfEntries() << " entries)..." << flush;

		// get count statistcs  
		for (Long64_t n=0; n<c.GetNoOfEntries(); ++n)
		{
			// read a coincidence event
	     	c.Fetch(n);

               // check time difference
               if (!c.IsTOFWithin(ctw)) continue;
               
 			// find energy bin
               int i1 = int((c.p1.en-lld)/ebin);
               int i2 = int((c.p2.en-lld)/ebin);
               if (i1<0 || i1>=nbins || i2<0 || i2>=nbins) continue;
               int idx = i1+i2*nbins;
                              
               // add to background and total tables
               ca[idx] += 1;
               if (c.IsRandom() || c.IsScattered()) cb[idx] += 1;
		}
		// done with this input file
		file->Close();
	}

	// calculate weightings
     for (int i=0; i<nbins*nbins; ++i)
     {
          wts[i] = ca[i]? float(ca[i]-cb[i])/float(ca[i]): 0;
     }

     // save weightings
	cout << "Writing to " << argv[1] <<  "..." << flush;
	ofstream outf(argv[1], ios::out | ios::binary);
     outf.write((char *) &nbins, sizeof(int));
     outf.write((char *) &lld, sizeof(float));
     outf.write((char *) &ebin, sizeof(float));
	outf.write((char *) wts, nbins*nbins*sizeof(float));
	outf.close();
     
	// done
     cout << endl;
	return 0;
}
