#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "LightHit.hh"

G4Allocator<LightHit> LightHitAllocator;

LightHit::LightHit()
  :physVol(0)
{}


LightHit::LightHit( G4VPhysicalVolume* pVol)
  :physVol(pVol)
{}


LightHit::LightHit( const LightHit &right)
  : G4VHit()
{
  edep    = right.edep;
  pos     = right.pos;
  physVol = right.physVol;
}


LightHit::~LightHit()
{}





const LightHit& LightHit::operator=( const LightHit &right){
  edep    = right.edep;
  pos     = right.pos;
  physVol = right.physVol;

  return *this;
}


G4int LightHit::operator==( const LightHit&) const{
  return false;
}



void LightHit::Draw(){
}



void LightHit::Print(){
}









