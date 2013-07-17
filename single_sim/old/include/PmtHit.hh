#ifndef PmtHit_h
#define PmtHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

class PmtHit : public G4VHit
{
public:
  
  PmtHit();
  PmtHit( G4VPhysicalVolume* pVol);
  PmtHit( const PmtHit &right);

  ~PmtHit();

  const PmtHit& operator=(const PmtHit &right);
  G4int operator==(const PmtHit &right) const;

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


  // inline void     SetX( G4int Xin) { X = Xin;}  
  // inline void     SetY( G4int Yin) { Y = Yin;}  
  // inline void     SetZ( G4int Zin) { Z = Zin;}  

  // inline G4int    GetX() { return X;}
  // inline G4int    GetY() { return Y;}
  // inline G4int    GetZ() { return Z;}

private:
  G4double edep;
  G4double time;
  G4ThreeVector pos;

  const G4VPhysicalVolume* physVol;

  //  G4int X, Y, Z;
};




typedef G4THitsCollection<PmtHit> PmtHitsCollection;

extern G4Allocator<PmtHit> PmtHitAllocator;

inline void* PmtHit::operator new( size_t)
{
  void *aHit;
  aHit = (void *) PmtHitAllocator.MallocSingle();
  return aHit;
}


inline void PmtHit::operator delete( void *aHit)
{
  PmtHitAllocator.FreeSingle((PmtHit*) aHit);
}

#endif


