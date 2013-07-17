#ifndef Parameterisation_H
#define Parameterisation_H 1

#include "globals.hh"
#include "G4VPVParameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Trap.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
#include "G4Para.hh"
#include "G4Hype.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"


class Parameterisation : public G4VPVParameterisation
{ 
  public:
  
  Parameterisation(G4int,    
		   G4int,
		   G4int,
		   G4double,
		   G4double,
		   G4double,
		   G4double);

    virtual				 
   ~Parameterisation();
   
    void ComputeTransformation( const G4int copyNo, G4VPhysicalVolume* physVol) const;
    
    void ComputeDimensions( G4Box&, const G4int, const G4VPhysicalVolume*) const;
    void ComputeDimensions( G4Trd&,const G4int,const G4VPhysicalVolume*) const;

  private: 

    void ComputeDimensions( G4Trap&,const G4int,const G4VPhysicalVolume*) const {};
    void ComputeDimensions( G4Cons&,const G4int,const G4VPhysicalVolume*) const {};
    void ComputeDimensions( G4Sphere&,const G4int,const G4VPhysicalVolume*) const {};
    void ComputeDimensions( G4Orb&,const G4int,const G4VPhysicalVolume*) const {};
    void ComputeDimensions( G4Torus&,const G4int,const G4VPhysicalVolume*) const {};
    void ComputeDimensions( G4Para&,const G4int,const G4VPhysicalVolume*) const {};
    void ComputeDimensions( G4Hype&,const G4int,const G4VPhysicalVolume*) const {};
    void ComputeDimensions( G4Tubs&,const G4int,const G4VPhysicalVolume*) const {};
    void ComputeDimensions( G4Polycone&,const G4int,const G4VPhysicalVolume*) const {};
    void ComputeDimensions( G4Polyhedra&,const G4int,const G4VPhysicalVolume*) const {};

  private:
    G4int     fncopy;   
    G4int     fnx;   
    G4int     fny;
    G4double  fspace;
    G4double  x0, y0, z0;
};



#endif


