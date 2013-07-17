//
// filename:
//		PETscanner.h
// author:
//		Chien-Min Kao
// date:
//		May 19, 2012
// description:
//		definitions for handling coincidences events
//		produced by GATE simulation by PETscanner system
//

#ifndef __PETscanner__
#define __PETscanner__

#include <TClassTable.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <iostream>
#include <cmath>
#include <string.h>

using namespace std;

// 3d Vector class
class Vec3
{
public:
	Float_t x;
	Float_t y;
	Float_t z;
	
	// define constructors
	Vec3(void) { x=y=z=0; }
	Vec3(Float_t xx, Float_t yy, Float_t zz) { x=xx, y=yy, z=zz; }
	Vec3(const Vec3& p) { x=p.x, y=p.y, z=p.z; }
	Vec3 &operator=(const Vec3& p)
	{
		x=p.x, y=p.y, z=p.z;
		return *this;
	}
	
	// destructor, do nothing
	~Vec3(void) {}
	
	// add and subtract two points
	Vec3 &operator+=(const Vec3 &p)
	{
		x+=p.x, y+=p.y, z+=p.z;
		return *this;
	}
	Vec3 &operator-=(const Vec3 &p)
	{
		x-=p.x, y-=p.y, z-=p.z;
		return *this;
	}
	Vec3 operator+(const Vec3& p) const
	{
		Vec3 t(*this);
		return (t+=p);
	}
	Vec3 operator-(const Vec3& p) const
	{
		Vec3 t(*this);
		return (t-=p);
	}
	Vec3 &operator+=(Float_t a)
	{
		x+=a, y+=a, z+=a;
		return *this;
	}
	Vec3 &operator-=(Float_t a)
	{
		x-=a, y-=a, z-=a;
		return *this;
	}
	Vec3 &operator*=(Float_t a)
	{
		x*=a, y*=a, z*=a;
		return *this;
	}
	Vec3 &operator/=(Float_t a)
	{
		x/=a, y/=a, z/=a;
		return *this;
	}
	Vec3 operator*(Float_t a) const
	{
		Vec3 t(*this);
		return (t*=a);
	}
	Vec3 operator/(Float_t a) const
	{
		Vec3 t(*this);
		return (t/=a);
	}
	Vec3 operator+(Float_t a) const
	{
		Vec3 t(*this);
		return (t+=a);
	}
	Vec3 operator-(Float_t a) const
	{
		Vec3 t(*this);
		return (t-=a);
	}
	friend Vec3 operator*(Float_t a, const Vec3& v);
	friend Vec3 operator+(Float_t a, const Vec3& v);
	
	// logic testing
	bool operator==(const Vec3& p) const
	{
		return ((x==p.x) && (y==p.y) && (z==p.z));
	}
	bool operator!=(const Vec3& p) const
	{
		return ((x!=p.x) || (y!=p.y) || (z!=p.z));
	}
	
	// norm and len
	Float_t norm2(void) const { return (x*x+y*y+z*z); }
	Float_t norm(void) const { return sqrt(x*x+y*y+z*z); }
	
	// print
	friend ostream& operator<<(ostream &out, const Vec3& p);
};

class DU
{
public:
	Int_t 	base;	// baseID
	Int_t 	l1; 	// level1ID
	Int_t 	l2; 	// level2ID
	Int_t 	l3; 	// level3ID
	Int_t 	l4; 	// level4ID
	Int_t 	layer;	// layerID
	
	// constructors
	DU(void) { base=l1=l2=l3=l4=layer=-1; }
	DU(Int_t bb, Int_t ll1, Int_t ll2, Int_t ll3, Int_t ll4, Int_t llayer)
	{
		base=bb, l1=ll1, l2=ll2, l3=ll3, l4=ll4, layer=llayer;
	}
	DU &operator=(const DU& du)
	{
		base=du.base, l1=du.l1, l2=du.l2, l3=du.l3, l4=du.l4, layer=du.layer;
		return *this;
	}
	
	// destructor, do nothing
	~DU(void) {}
	
	// print
	friend ostream& operator<<(ostream &, const DU &);
};

// single event class
class Single
{
public:
	Int_t 		id;					// eventID
	Int_t 		src_id;				// sourceID
	Vec3		src;				// position of the source
	Vec3		pos;				// position for the Single
	Double_t	time;				// time
	Float_t		en; 				// energy
	DU			du;					// element that Single is detected
	Int_t 		comptonP;			// comptonPhantom
	Int_t 		comptonC;			// comptonCrystal
	Int_t 		rayleighP;			// RayleighPhantom
	Int_t 		rayleighC;			// RayleighCrystal
	Char_t		comptonVol[40]; 	// compVolName
	Char_t		rayleighVol[40];	// RayleighVolName
	
	// constructors
	Single(void) {}
	Single(const Single& p)
	{
		id = p.id;
		src_id = p.src_id;
		src = p.src;
		pos = p.pos;
		time = p.time;
		en = p.en;
		du = p.du;
		comptonP = p.comptonP, comptonC = p.comptonC;
		rayleighP = p.rayleighP, rayleighC = p.rayleighC;
		strcpy(comptonVol,  p.comptonVol);
		strcpy(rayleighVol, p.rayleighVol);	
	}
	Single &operator=(const Single& p)
	{
		id = p.id;
		src_id = p.src_id;
		src = p.src;
		pos = p.pos;
		time = p.time;
		en = p.en;
		du = p.du;
		comptonP = p.comptonP, comptonC = p.comptonC;
		rayleighP = p.rayleighP, rayleighC = p.rayleighC;
		strcpy(comptonVol,  p.comptonVol);
		strcpy(rayleighVol, p.rayleighVol);
		return *this;
	}
	
	// check energy and if scattered
	bool IsQualified(Float_t lld, Float_t uld) const
	{
		return ((en>=lld) && (en<=uld));
	}
	bool IsScattered(void) const
	{
		return (comptonP!=0 || rayleighP!=0);
	}
};

class Coincidence
{
private:
	TTree  *ctree;   	// coincidences tree
	
public:
	Int_t 	run;		// runID
	Float_t	z;			// axialPos
	Float_t	rot;		// rotationAngle
	Single	p1; 		// photon1
	Single	p2;			// photon2
	Float_t	sinoTheta;	// sinogramTheta
	Float_t	sinoS;		// sinogramS

	Coincidence(TFile *file)
	{
		// assume TFile has been opened and remained open during access
		
		// get tree pointer for coincidence events
		ctree = (TTree*) file->Get("Coincidences");

		//
		// setup access variables
		//
		
		ctree->SetBranchAddress("runID",         &run);
		ctree->SetBranchAddress("axialPos",      &z);
		ctree->SetBranchAddress("rotationAngle", &rot);
		ctree->SetBranchAddress("sinogramS",     &sinoS);
		ctree->SetBranchAddress("sinogramTheta", &sinoTheta);
		
		// access event 1
		ctree->SetBranchAddress("eventID1",      &p1.id);
		ctree->SetBranchAddress("sourceID1",     &p1.src_id);
		ctree->SetBranchAddress("sourcePosX1",   &p1.src.x);
		ctree->SetBranchAddress("sourcePosY1",   &p1.src.y);
		ctree->SetBranchAddress("sourcePosZ1",   &p1.src.z);
		ctree->SetBranchAddress("time1",         &p1.time);
		ctree->SetBranchAddress("energy1",       &p1.en);
		ctree->SetBranchAddress("globalPosX1",   &p1.pos.x);
		ctree->SetBranchAddress("globalPosY1",   &p1.pos.y);
		ctree->SetBranchAddress("globalPosZ1",   &p1.pos.z);
		ctree->SetBranchAddress("baseID1",       &p1.du.base);
		ctree->SetBranchAddress("level1ID1",     &p1.du.l1);
		ctree->SetBranchAddress("level2ID1",     &p1.du.l2);
		ctree->SetBranchAddress("level3ID1",     &p1.du.l3);
		ctree->SetBranchAddress("level4ID1",     &p1.du.l4);
		ctree->SetBranchAddress("layerID1",      &p1.du.layer);
		ctree->SetBranchAddress("comptonPhantom1",  &p1.comptonP);
		ctree->SetBranchAddress("comptonCrystal1",  &p1.comptonC);
		ctree->SetBranchAddress("RayleighPhantom1", &p1.rayleighP);
		ctree->SetBranchAddress("RayleighCrystal1", &p1.rayleighC);
		ctree->SetBranchAddress("comptVolName1",    &p1.comptonVol);
		ctree->SetBranchAddress("RayleighVolName1", &p1.rayleighVol);
		
		// access event 2
		ctree->SetBranchAddress("eventID2",      &p2.id);
		ctree->SetBranchAddress("sourceID2",     &p2.src_id);
		ctree->SetBranchAddress("sourcePosX2",   &p2.src.x);
		ctree->SetBranchAddress("sourcePosY2",   &p2.src.y);
		ctree->SetBranchAddress("sourcePosZ2",   &p2.src.z);
		ctree->SetBranchAddress("time2",         &p2.time);
		ctree->SetBranchAddress("energy2",       &p2.en);
		ctree->SetBranchAddress("globalPosX2",   &p2.pos.x);
		ctree->SetBranchAddress("globalPosY2",   &p2.pos.y);
		ctree->SetBranchAddress("globalPosZ2",   &p2.pos.z);
		ctree->SetBranchAddress("baseID2",       &p2.du.base);
		ctree->SetBranchAddress("level1ID2",     &p2.du.l1);
		ctree->SetBranchAddress("level2ID2",     &p2.du.l2);
		ctree->SetBranchAddress("level3ID2",     &p2.du.l3);
		ctree->SetBranchAddress("level4ID2",     &p2.du.l4);
		ctree->SetBranchAddress("layerID2",      &p2.du.layer);
		ctree->SetBranchAddress("comptonPhantom2",  &p2.comptonP);
		ctree->SetBranchAddress("comptonCrystal2",  &p2.comptonC);
		ctree->SetBranchAddress("RayleighPhantom2", &p2.rayleighP);
		ctree->SetBranchAddress("RayleighCrystal2", &p2.rayleighC);
		ctree->SetBranchAddress("comptVolName2",    &p2.comptonVol);
		ctree->SetBranchAddress("RayleighVolName2", &p2.rayleighVol);
	}
	
	// destructor, do nothing
	~Coincidence(void) {}
	
	// get number of entries
	Long64_t GetNoOfEntries(void) { return ctree->GetEntries(); }
	
	// fetch the nth entry
	void Fetch(Long64_t n) { ctree->GetEntry(n); }
	
	// check event type
	bool IsRandom(void) const
	{
		return (p1.id!=p2.id);
	}
	bool IsScattered(void) const
	{
		return (p1.IsScattered() || p2.IsScattered());
	}
	bool IsTrue(void) const
	{
		return (!IsRandom() && !IsScattered());
	}
	
	// check energy qualification
	bool IsQualified(Float_t lld, Float_t uld) const
	{
		return (p1.IsQualified(lld,uld) && p2.IsQualified(lld,uld)); 
	}
	bool IsQualified(Float_t lld1, Float_t uld1, Float_t lld2, Float_t uld2) const
	{
		return (p1.IsQualified(lld1,uld1) && p2.IsQualified(lld2,uld2));
	}
		
	// get TOF
	Double_t TOF(void) const { return (p1.time-p2.time); }
	
	// check tof qualification
	bool IsTOFWithin(Double_t tw) const
	{
		Double_t dt = TOF();
		return (dt<=tw && dt>=-tw);
	}
	
	// calculate source position based on TOF
	Vec3 TOFPos(void) const
	{
		const Float_t light_speed = 3e11; 	// speed of light in air, in mm/sec
		Vec3 dr = p1.pos-p2.pos;
		Float_t alpha = light_speed*TOF()/dr.norm();
		Vec3 v = 0.5*(p1.pos+p2.pos-alpha*dr);
		return v;
	}
};

ostream& operator<<(ostream& out, const Vec3& p)
{
    out << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    return out;
}
Vec3 operator*(Float_t a, const Vec3& v) { return (v*a); }
Vec3 operator+(Float_t a, const Vec3& v) { return (v+a); }

ostream& operator<<(ostream &out, const DU &du)
{
    out << "[" << du.base << ":" << du.l1 << ":" << du.l2;
	out << ":" << du.l3 << ":" << du.l4 << ":" << du.layer << "]";
    return out;
}

#endif
