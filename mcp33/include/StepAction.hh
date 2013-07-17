#ifndef SteppingAction_H
#define SteppingACtion_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"



class StepAction : public G4UserSteppingAction
{
public:
  StepAction();
  ~StepAction();
  void UserSteppingAction(const G4Step*);

};

#endif
