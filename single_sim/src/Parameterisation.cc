#include "Parameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"



Parameterisation::Parameterisation(  
	G4int    ncopy, 
        G4int    nx,   
        G4int    ny,   
        G4double space,
	G4double x,
	G4double y,
	G4double z)
{
  fncopy = ncopy;
  fnx = nx;
  fny = ny;
  fspace = space; 

  x0 = x;
  y0 = y;
  z0 = z;
}


Parameterisation::~Parameterisation()
{}



void Parameterisation::ComputeTransformation( const G4int copyNo, G4VPhysicalVolume* physVol) const
{
  G4double xpos = (copyNo%fnx)*fspace + x0;
  G4double ypos = (copyNo/fnx)*fspace + y0;
  G4double zpos = z0;

  G4ThreeVector origin( xpos, ypos, zpos);

  physVol->SetTranslation( origin);
  physVol->SetRotation(0);
}



void Parameterisation::ComputeDimensions
(G4Box&, const G4int, const G4VPhysicalVolume*) const
{

}


void Parameterisation::ComputeDimensions
(G4Trd&, const G4int, const G4VPhysicalVolume*) const
{

}


