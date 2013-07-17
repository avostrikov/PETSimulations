#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

#include "PmtHit.hh"

G4Allocator<PmtHit> PmtHitAllocator;

PmtHit::PmtHit()
  :physVol(0)
{}


PmtHit::PmtHit( G4VPhysicalVolume* pVol)
  :physVol(pVol)
{}


PmtHit::PmtHit( const PmtHit &right)
  : G4VHit()
{
  edep    = right.edep;
  pos     = right.pos;
  physVol = right.physVol;
}


PmtHit::~PmtHit()
{}





const PmtHit& PmtHit::operator=( const PmtHit &right){
  edep    = right.edep;
  pos     = right.pos;
  physVol = right.physVol;

  return *this;
}


G4int PmtHit::operator==( const PmtHit&) const{
  return false;
}



void PmtHit::Draw(){
}



void PmtHit::Print(){
}









