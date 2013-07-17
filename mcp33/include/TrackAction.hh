#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"
#include "Trajectory.hh"


class TrackAction : public G4UserTrackingAction {

public:  
  TrackAction();
  ~TrackAction(){};
  
  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);

private:
  int count;

  
};

#endif
