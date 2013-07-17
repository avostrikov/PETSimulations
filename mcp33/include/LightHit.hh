#ifndef LightHit_h
#define LightHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

class LightHit : public G4VHit
{
public:
  
  LightHit();
  LightHit( G4VPhysicalVolume* pVol);
  LightHit( const LightHit &right);

  ~LightHit();

  const LightHit& operator=(const LightHit &right);
  G4int operator==(const LightHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
  
  void Draw();
  void Print();

  //
  inline void     SetEdep( G4double de) { edep = de;}  
  inline void     AddEdep( G4double de) { edep += de;}
  inline G4double GetEdep() { return edep;}
  
  inline void     SetPos( G4ThreeVector xyz) { pos = xyz;}
  inline G4ThreeVector GetPos() { return pos;}

  inline const G4VPhysicalVolume * GetPhysV() { return physVol;}

private:
  G4double edep;
  G4ThreeVector pos;
  const G4VPhysicalVolume* physVol;
};




typedef G4THitsCollection<LightHit> LightHitsCollection;

extern G4Allocator<LightHit> LightHitAllocator;

inline void* LightHit::operator new( size_t)
{
  void *aHit;
  aHit = (void *) LightHitAllocator.MallocSingle();
  return aHit;
}


inline void LightHit::operator delete( void *aHit)
{
  LightHitAllocator.FreeSingle((LightHit*) aHit);
}

#endif


