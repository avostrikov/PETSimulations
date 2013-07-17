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
  


  if( 0 && edep > 0)
    printf("--> User Stepping, TrackID = %d, Status = %d, Def = %s\n", 
	   Track->GetTrackID(), Track->GetTrackStatus(), Track->GetDefinition()->GetParticleName().c_str());

    
  G4StepPoint* pre_point  = Step->GetPreStepPoint();
  G4StepPoint* post_point = Step->GetPostStepPoint();


  G4String process = post_point->GetProcessDefinedStep()->GetProcessName();

  if( 0 && edep != 0)
    printf(" pre_point  = ( %f, %f, %f)\n", 
	   pre_point->GetPosition().x(),
	   pre_point->GetPosition().y(),
	   pre_point->GetPosition().z());
  
  if( 0 && edep != 0)
    printf(" post_point = ( %f, %f, %f)\n edep = %f, K.E= %f(%f), process = %s\n", 
	   post_point->GetPosition().x(),
	   post_point->GetPosition().y(),
	   post_point->GetPosition().z(),
	   edep, Track->GetKineticEnergy(), Track->GetVertexKineticEnergy(), 
	   process.c_str());

  if(0){
    G4cout << " pre momentum : " << pre_point->GetMomentum() << "   : " << pre_point->GetMomentum().mag() << G4endl; 
    G4cout << " post momentum : " << post_point->GetMomentum() << "   : " << post_point->GetMomentum().mag() << G4endl; 
  }

  G4VPhysicalVolume* pre_volume 
    = pre_point->GetPhysicalVolume();

  G4VPhysicalVolume* post_volume 
    = post_point->GetPhysicalVolume();



  if( 0 && Track->GetTrackStatus() == 2){
    printf("Stopped : Volume =  %s , Process = %s\n",
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
  

  if(0)
    printf("--> User Stepping, TrackID = %d, Status = %d, Def = %s\n", 
	   Track->GetTrackID(), Track->GetTrackStatus(), 
	   Track->GetDefinition()->GetParticleName().c_str());
  


  // collect energy and track length step by step
  G4double x, y, z, tlength;
  
  x = Track->GetPosition().x();
  y = Track->GetPosition().y();
  z = Track->GetPosition().z();
  tlength = Step->GetStepLength(); 

  G4ParticleDefinition* par_type = Track->GetDefinition();

 
  if( 1 && par_type == G4OpticalPhoton::OpticalPhotonDefinition()){
    if( 1  &&
	strncmp( pre_volume->GetName().c_str(), "glass", 5) == 0 &&
	strncmp( post_volume->GetName().c_str(), "photocathode", 12) == 0 ){
      
      if(0)
	printf("Now enters from %s -> %s. \n", 
	       pre_volume->GetName().c_str(), 
	       post_volume->GetName().c_str());
      
      
      G4OpBoundaryProcessStatus boundaryStatus = Undefined;
      boundaryStatus = boundary->GetStatus();

      if( post_point->GetStepStatus() != fGeomBoundary) return;
      if( boundaryStatus != Detection && boundaryStatus != Absorption) return;

      if(0)
      printf(" Optical Photon Detected! from %s to %s, boundaryStatus = %d\n",
	     pre_volume->GetName().c_str(), 
	     post_volume->GetName().c_str(),
	     boundaryStatus);
      
      
      // printf("  boundaryStatus = %d\n", boundaryStatus);      
      
      G4SDManager* SDman = G4SDManager::GetSDMpointer();
      G4String     sdname="PmtSD";
      PmtSD* pmtSD = (PmtSD*)SDman
	->FindSensitiveDetector(sdname);
      
      if( pmtSD) pmtSD->ProcessHits_constStep( Step, NULL);
      
      //      Track->SetTrackStatus( fStopAndKill);
    }
  }
}


/*
enum G4OpBoundaryProcessStatus {  
0  Undefined,
1  FresnelRefraction, 
2  FresnelReflection,
3  TotalInternalReflection,
4  LambertianReflection,
5  LobeReflection,
6  SpikeReflection, 
7  BackScattering,
8  Absorption, 
9  Detection, 
10 NotAtBoundary,
11 SameMaterial, 
12 StepTooSmall, 
13 NoRINDEX };
*/
