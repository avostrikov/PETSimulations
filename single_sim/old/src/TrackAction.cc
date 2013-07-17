#include "TrackAction.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"


TrackAction::TrackAction()
{}


void TrackAction::PreUserTrackingAction(const G4Track* aTrack)
{
  if(0) printf("PreUserTracking,  ID = %d, parent ID = %d\n", 
	       aTrack->GetTrackID(), aTrack->GetParentID());
}


void TrackAction::PostUserTrackingAction(const G4Track* aTrack){ 

  if(0) printf("PostUserTracking,  ID = %d, parent ID = %d\n", 
	       aTrack->GetTrackID(), aTrack->GetParentID());

}
