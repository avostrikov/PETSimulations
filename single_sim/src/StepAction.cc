#include "StepAction.hh"
#include "PmtSD.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4OpBoundaryProcess.hh"



StepAction::StepAction()
{}


StepAction::~StepAction()
{}


void 
StepAction::UserSteppingAction(const G4Step * Step){

  G4double edep = Step->GetTotalEnergyDeposit();

  G4Track* Track = Step->GetTrack();
  

  if( 1 && edep > 0)
    printf("--> User Stepping, TrackID = %d, Status = %d\n", 
	   Track->GetTrackID(), Track->GetTrackStatus());


  G4StepPoint* pre_point  = Step->GetPreStepPoint();
  G4StepPoint* post_point = Step->GetPostStepPoint();

  G4String process = post_point->GetProcessDefinedStep()->GetProcessName();

  if( 1 && edep != 0)
    printf(" pre_point  = ( %f, %f, %f)\n", 
	   pre_point->GetPosition().x(),
	   pre_point->GetPosition().y(),
	   pre_point->GetPosition().z());
  
  if( 1 && edep != 0)
    printf(" post_point = ( %f, %f, %f), process = %s\n", 
	   post_point->GetPosition().x(),
	   post_point->GetPosition().y(),
	   post_point->GetPosition().z(),
	   process.c_str());


  G4VPhysicalVolume* pre_volume 
    = pre_point->GetPhysicalVolume();

  G4VPhysicalVolume* post_volume 
    = post_point->GetPhysicalVolume();


  if( 0&& Track->GetTrackStatus() == 2){
    printf("Stopped : Volume = %s   , Process = %s\n",
	   post_volume->GetName().c_str(), process.c_str());
  }
    


  static G4OpBoundaryProcess* boundary = NULL;

  if( !boundary){
    G4ProcessManager* pm 
      = Track->GetDefinition()->GetProcessManager();
    int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    for( int i = 0; i < nprocesses; i++){
      if((*pv)[i]->GetProcessName() == "OpBoundary"){
	boundary = (G4OpBoundaryProcess*)(*pv)[i];
	break;
      }
    }
  }

  // out of world
  if(!post_volume)
    return;
  
  // collect energy and track length step by step
  G4double x, y, z, tlength;
  
  x = Track->GetPosition().x();
  y = Track->GetPosition().y();
  z = Track->GetPosition().z();
  tlength = Step->GetStepLength();
 

  G4ParticleDefinition* par_type = Track->GetDefinition();

  if( par_type == G4OpticalPhoton::OpticalPhotonDefinition()){
    
    /*
    if( pre_volume->GetName() == "light_guide_0_0")
      if( post_volume->GetName() == "pmt_0_0"){

	Track->SetTrackStatus( fStopAndKill);
      }
    */
    
    if( 0 && 
	strncmp( pre_volume->GetName().c_str(), "light_guide", 9) == 0 &&
	strncmp( post_volume->GetName().c_str(), "pmt", 3) == 0 ){

      //      printf("Now enters %s\n", pre_volume->GetName().c_str());
      G4SDManager* SDman = G4SDManager::GetSDMpointer();
      G4String     sdname="PmtSD";
      PmtSD* pmtSD = (PmtSD*)SDman
	->FindSensitiveDetector(sdname);
      
      if( pmtSD) pmtSD->ProcessHits_constStep( Step, NULL);
      
      Track->SetTrackStatus( fStopAndKill);
    }
  }

 

 
  /*
  if( par_type == G4OpticalPhoton::OpticalPhotonDefinition()){

    G4OpBoundaryProcessStatus boundaryStatus = Undefined;
    boundaryStatus = boundary->GetStatus();
    
    if( post_point->GetStepStatus() == fGeomBoundary){

      //      printf("boundaryStatus = %d \n", boundaryStatus);
      switch(boundaryStatus){
      case Absorption:
	  break;

      case Detection: 
	{
	  G4SDManager* SDman = G4SDManager::GetSDMpointer();
	  G4String     sdName="pmtSD";
	  PmtSD* pmtSD = (PmtSD*)SDman
	    ->FindSensitiveDetector(sdName);
	  if(pmtSD) pmtSD->ProcessHits_constStep( Step, NULL);
	  break;
	}
      case FresnelReflection:
	break;
      case TotalInternalReflection:
	break;
      case SpikeReflection:
	break;
      default:
	break;
      }	
    }
  }


  // if( strcmp( process.c_str(), "Transportation") != 0)
  printf("Edep  = %5.2f, at %15s,   "
	 "x = %5.2f, y = %5.2f, z = %5.2f, length = %5.2f, %s\n",
	 edep, pre_volume->GetName().c_str(), x, y, z, tlength, 
	 process.c_str());  
  */

}
