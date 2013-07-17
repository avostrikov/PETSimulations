#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "ScintHit.hh"

G4Allocator<ScintHit> ScintHitAllocator;

ScintHit::ScintHit()
  :physVol(0)
{}


ScintHit::ScintHit( G4VPhysicalVolume* pVol)
  :physVol(pVol)
{}


ScintHit::ScintHit( const ScintHit &right)
  : G4VHit()
{
  edep    = right.edep;
  pos     = right.pos;
  physVol = right.physVol;
}


ScintHit::~ScintHit()
{}





const ScintHit& ScintHit::operator=( const ScintHit &right){
  edep    = right.edep;
  pos     = right.pos;
  physVol = right.physVol;

  return *this;
}


G4int ScintHit::operator==( const ScintHit&) const{
  return false;
}



void ScintHit::Draw(){
}



void ScintHit::Print(){
}









