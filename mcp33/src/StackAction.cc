#include "StackAction.hh"

#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"


StackAction::StackAction()
{}

StackAction::~StackAction()
{}


G4ClassificationOfNewTrack
StackAction::ClassifyNewTrack(const G4Track * aTrack){
  
  if( 0)
    printf("Classification,  trackid = %d, parentid = %d, status = %d\n", 
	   aTrack->GetTrackID(), aTrack->GetParentID(), aTrack->GetTrackStatus());

  return fUrgent;
}



void StackAction::NewStage(){

  //  printf("NewStage\n");
}


void StackAction::PrepareNewEvent(){

  //  printf("Prepare New Event\n");
}
