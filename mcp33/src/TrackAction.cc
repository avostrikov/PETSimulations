#include "TrackAction.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"


TrackAction::TrackAction()
{
  count = 0;
}


void TrackAction::PreUserTrackingAction(const G4Track* aTrack)
{

 
  //Use custom trajectory class
  fpTrackingManager->SetTrajectory(new Trajectory(aTrack));


  if(0) printf("PreUserTracking,  ID = %d, parent ID = %d,  name = %s\n", 
	       aTrack->GetTrackID(), 
	       aTrack->GetParentID(), 
	       aTrack->GetDefinition()->GetParticleName().c_str());

  //if( strcmp( aTrack->GetDefinition()->GetParticleName().c_str(), "opticalphoton") == 0)
  // count++;

  //  printf("count = %d\n", count);

}



void TrackAction::PostUserTrackingAction(const G4Track* aTrack){ 

  Trajectory* trajectory=(Trajectory*)fpTrackingManager->GimmeTrajectory();
  
  const G4Step* aStep = aTrack->GetStep();

  //  if(aTrack->GetTrackStatus() != 2 && 1)
  if( 1){ 
    G4StepPoint* p0  = aStep->GetPreStepPoint();
    G4StepPoint* p1  = aStep->GetPostStepPoint();
        
    G4VPhysicalVolume* pre_volume 
	= p0->GetPhysicalVolume();
    
    
    G4VPhysicalVolume* post_volume 
      = p1->GetPhysicalVolume();
    
    
    // G4String process = aTrack->GetCreatorProcess()->GetProcessName();
    G4String process = p1->GetProcessDefinedStep()->GetProcessName();
    

    trajectory->SetStatus( aTrack->GetTrackStatus());
    trajectory->SetPreVolume(  pre_volume->GetName());
    // trajectory->SetPostVolume(  post_volume->GetName());
    if( aTrack->GetNextVolume() != 0)
      trajectory->SetPostVolume( aTrack->GetNextVolume()->GetName().c_str() );

    //    trajectory->SetPostVolume( aTrack->GetVolume()->GetName().c_str() );
    trajectory->SetProcess(  process);
    trajectory->SetLength(  aTrack->GetTrackLength());
    
    
    //printf("pareent = %d\n",  aTrack->GetParentID() );

    //    if( aStep->IsLastStepInVolume() == true)
    // printf("post = %s\n",   post_volume->GetName().c_str());
    
    // printf("volume = %s\n",   aTrack->GetVolume()->GetName().c_str());


    if(0)
      printf("PostUserTracking,  ID = %d, status = %d, parent ID = %d, id = %s,"
	     "Process = %s,  pre Vol = %s,  post Vol = %s\n", 
	     aTrack->GetTrackID(), aTrack->GetTrackStatus(),  
	     aTrack->GetParentID(), 
	     aTrack->GetDefinition()->GetParticleName().c_str(),
	     process.c_str(),
	     pre_volume->GetName().c_str(), 
	     post_volume->GetName().c_str()
	     );
    

   
    //	     aTrack->GetVertexPosition().x(),
    //     aTrack->GetVertexPosition().y(),
    //     aTrack->GetVertexPosition().z()    
    

    
    if(0){
      printf("prepoint = %5.2f, %5.2f, %5.2f\n", 
	     p0->GetPosition().x(), p0->GetPosition().y(), p0->GetPosition().z());
      printf("postpoint = %5.2f, %5.2f, %5.2f\n", 
	     p1->GetPosition().x(), p1->GetPosition().y(), p1->GetPosition().z());
    }
    

  }
  
}






/*
enum G4TrackStatus
{

0  fAlive,             // Continue the tracking
1  fStopButAlive,      // Invoke active rest physics processes and
                      // and kill the current track afterward
2  fStopAndKill,       // Kill the current track

3  fKillTrackAndSecondaries,
                      // Kill the current track and also associated
                      // secondaries.
4  fSuspend,           // Suspend the current track
5  fPostponeToNextEvent
                      // Postpones the tracking of thecurrent track 
                      // to the next event.

};
*/
