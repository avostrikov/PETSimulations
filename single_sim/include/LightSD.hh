#ifndef LightSD_h
#define LightSD_h 1

#include "LightHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class LightSD : public G4VSensitiveDetector
{
public:
  LightSD(G4String name);
  ~LightSD();
  
  void Initialize(G4HCofThisEvent* HCE);
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  void EndOfEvent(G4HCofThisEvent* HCE);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  LightHitsCollection* lightCollection;
  
};

#endif

