#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "WaveHit.hh"

G4Allocator<WaveHit> WaveHitAllocator;

WaveHit::WaveHit()
  :physVol(0)
{}


WaveHit::WaveHit( G4VPhysicalVolume* pVol)
  :physVol(pVol)
{}


WaveHit::WaveHit( const WaveHit &right)
  : G4VHit()
{
  edep    = right.edep;
  pos     = right.pos;
  physVol = right.physVol;
}


WaveHit::~WaveHit()
{}





const WaveHit& WaveHit::operator=( const WaveHit &right){
  edep    = right.edep;
  pos     = right.pos;
  physVol = right.physVol;

  return *this;
}


G4int WaveHit::operator==( const WaveHit&) const{
  return false;
}



void WaveHit::Draw(){
}



void WaveHit::Print(){
}









