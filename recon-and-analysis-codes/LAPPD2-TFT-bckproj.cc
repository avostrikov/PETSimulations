//
// name: LAPPD2-TFT-bckproj.cc
// date: August 13, 2012
// author: Chien-Min Kao
//

#include <fstream>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include "PETscanner.h"

using namespace::std;

float inline len(float x, int y, int z)
{
	return sqrt(x*x+y*y+z*z);
}

float *img = (float *) 0;
int main(int argc, char **argv)
{
	// check syntax
	if (argc<7)
	{
		cerr << "Usage: " << argv[0] << " <outfile> <lld> <uld> <tcw> <TFT table> <file1> [file2] [file3] ..." << endl;
		return 1;
	}

	// load the Tree library is not already loaded
	if (!TClassTable::GetDict("Tree")) gSystem->Load("libTree.so");

	// get energy and coincidence windows
	float lld, uld, cw;
	sscanf(argv[2], "%f", &lld);		// specified in keV
	sscanf(argv[3], "%f", &uld);
	lld /= 1000., uld /= 1000.;		// convert from keV to MeV
	sscanf(argv[4], "%f", &cw);	     // specified in nanosecond
	cw *= 1e-9;				    // convert from picosecond to second
	cout << "lld = " << lld << " MeV, uld = " << uld << " MeV; coinc. window = " << cw << " s" << endl;
	
	// define image voxel size and prepare image space
	float pixelsize = 2.0;               // 2 mm
	float xfov=600, yfov=400, zfov=800;  // FOV = 600mmx400mmx800mm
	int   xlen = int(xfov/pixelsize);    // number of voxels in x
	int   ylen = int(yfov/pixelsize);    // in y
	int   zlen = int(zfov/pixelsize);    // in z
	long  nimg = long(xlen)*ylen*zlen;   // total number of voxels
	img = new float[nimg];        // allocate memory
	memset(img, 0, sizeof(float)*nimg);  // clear to all elements to zero
	float imgcts = 0.0;
	
     // get TFT table
     int   tft_nbins;
     float tft_ebin, tft_lld;
     cout << "Reading TFT table from " << argv[5] <<  "..." << flush;
     ifstream tftfile(argv[5], ios::out | ios::binary);
     tftfile.read((char *) &tft_nbins, sizeof(int));
     tftfile.read((char *) &tft_lld, sizeof(float));
     tftfile.read((char *) &tft_ebin, sizeof(float));
     float *tft = new float[tft_nbins*tft_nbins];
	tftfile.read((char *) tft, tft_nbins*tft_nbins*sizeof(float));
	tftfile.close();
          
	// open rootfile
        for (int infile=6; infile<argc; ++infile)
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
		cerr << " (" << c.GetNoOfEntries() << " entries) ..." << flush;

		// process events
		for (Long64_t n=0; n<c.GetNoOfEntries(); ++n)
		{
			// read a coincidence event
                        c.Fetch(n);
		
			// apply energy window
			if (!c.IsQualified(lld, uld)) continue;
			
			// apply coincidence window
			if (!c.IsTOFWithin(cw)) continue;
			
                        // apply TFT
                        int i1 = int((c.p1.en-tft_lld)/tft_ebin);
                        int i2 = int((c.p2.en-tft_lld)/tft_ebin);
                        if (i1<0 || i1>=tft_nbins || i2<0 || i2>=tft_nbins) continue;
                        float w = tft[i1+i2*tft_nbins];
               
			// update image
			Vec3 p = c.TOFPos();
			int ix = ((p.x+300.)/2.);
			if (ix<0 || ix>=xlen) continue;
			int iy = ((p.y+200.)/2.);
			if (iy<0 || iy>=ylen) continue;
			int iz = ((p.z+400.)/2.);
			if (iz<0 || iz>=zlen) continue;
			img[ix+(iy+iz*200L)*300L] += w;
			imgcts += w;
		}
		// done with this input file
		file->Close();
	}

	// write image
	cout << "Writing to " << argv[1] <<  "(Total image counts = " << Long64_t(imgcts) << ")..." << flush;
	ofstream outf(argv[1], ios::out | ios::binary);
	outf.write((char *) img, nimg*sizeof(float));
	outf.close();
	
	// done
	cout << endl;
	return 0;
}
