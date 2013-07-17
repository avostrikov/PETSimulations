#ifndef ScintSD_h
#define ScintSD_h 1

#include "ScintHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class ScintSD : public G4VSensitiveDetector
{
public:
  ScintSD(G4String name);
  ~ScintSD();
  
  void Initialize(G4HCofThisEvent* HCE);
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  void EndOfEvent(G4HCofThisEvent* HCE);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  ScintHitsCollection* scintCollection;
  
};

#endif

