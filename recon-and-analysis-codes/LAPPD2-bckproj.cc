//
// name: LAPPD2-bckproj.cc
// date: August 10, 2012
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
	if (argc<6)
	{
		cerr << "Usage: " << argv[0] << " <outfile> <lld> <uld> <tcw> <file1> [file2] [file3] ..." << endl;
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
	Long64_t imgcts = 0;
	
	// open rootfile
        for (int infile=5; infile<argc; ++infile)
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
			
			// update image
			Vec3 p = c.TOFPos();
			int ix = ((p.x+300.)/2.);
			if (ix<0 || ix>=xlen) continue;
			int iy = ((p.y+200.)/2.);
			if (iy<0 || iy>=ylen) continue;
			int iz = ((p.z+400.)/2.);
			if (iz<0 || iz>=zlen) continue;
			img[ix+(iy+iz*200L)*300L] += 1;
			++imgcts;
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
