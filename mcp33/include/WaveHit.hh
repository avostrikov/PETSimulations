#ifndef WaveHit_h
#define WaveHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

class WaveHit : public G4VHit
{
public:
  
  WaveHit();
  WaveHit( G4VPhysicalVolume* pVol);
  WaveHit( const WaveHit &right);

  ~WaveHit();

  const WaveHit& operator=(const WaveHit &right);
  G4int operator==(const WaveHit &right) const;

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




typedef G4THitsCollection<WaveHit> WaveHitsCollection;

extern G4Allocator<WaveHit> WaveHitAllocator;

inline void* WaveHit::operator new( size_t)
{
  void *aHit;
  aHit = (void *) WaveHitAllocator.MallocSingle();
  return aHit;
}


inline void WaveHit::operator delete( void *aHit)
{
  WaveHitAllocator.FreeSingle((WaveHit*) aHit);
}

#endif


