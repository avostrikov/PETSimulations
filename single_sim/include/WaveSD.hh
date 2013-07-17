#ifndef WaveSD_h
#define WaveSD_h 1

#include "WaveHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class WaveSD : public G4VSensitiveDetector
{
public:
  WaveSD(G4String name);
  ~WaveSD();
  
  void Initialize(G4HCofThisEvent* HCE);
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  void EndOfEvent(G4HCofThisEvent* HCE);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  WaveHitsCollection* waveCollection;
  
};

#endif

