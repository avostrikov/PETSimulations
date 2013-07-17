#ifndef PmtSD_h
#define PmtSD_h 1

#include "PmtHit.hh"

#include "G4VSensitiveDetector.hh"

class G4Step;
class G4HCofThisEvent;

class PmtSD : public G4VSensitiveDetector
{
public:
  PmtSD(G4String name);
  ~PmtSD();
  
  void Initialize(G4HCofThisEvent* HCE);
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);

  G4bool ProcessHits_constStep(const G4Step* aStep,
			       G4TouchableHistory* ROhist);

  void EndOfEvent(G4HCofThisEvent* HCE);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  PmtHitsCollection* pmtCollection;
  
};

#endif

