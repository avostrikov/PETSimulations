#ifndef ScintHit_h
#define ScintHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

class ScintHit : public G4VHit
{
public:
  
  ScintHit();
  ScintHit( G4VPhysicalVolume* pVol);
  ScintHit( const ScintHit &right);

  ~ScintHit();

  const ScintHit& operator=(const ScintHit &right);
  G4int operator==(const ScintHit &right) const;

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

  inline void     SetTime( G4double gtime) { time = gtime;}  
  inline G4double GetTime() { return time;}

  inline const G4VPhysicalVolume * GetPhysV() { return physVol;}

private:
  G4double edep;
  G4double time;
  G4ThreeVector pos;

  const G4VPhysicalVolume* physVol;
};




typedef G4THitsCollection<ScintHit> ScintHitsCollection;

extern G4Allocator<ScintHit> ScintHitAllocator;

inline void* ScintHit::operator new( size_t)
{
  void *aHit;
  aHit = (void *) ScintHitAllocator.MallocSingle();
  return aHit;
}


inline void ScintHit::operator delete( void *aHit)
{
  ScintHitAllocator.FreeSingle((ScintHit*) aHit);
}

#endif


