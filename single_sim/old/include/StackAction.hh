#ifndef StackAction_H
#define StackAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

class StackAction : public G4UserStackingAction
{
public:
  StackAction();
  ~StackAction();
  
  virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
  virtual void NewStage();
  virtual void PrepareNewEvent();
  
private:
};

#endif
