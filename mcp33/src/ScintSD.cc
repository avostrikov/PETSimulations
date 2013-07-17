#include "ScintSD.hh"
#include "ScintHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"


ScintSD::ScintSD( G4String name)
  :G4VSensitiveDetector(name)  
{
  collectionName.insert("scintCollection");
}



ScintSD::~ScintSD()
{}



void ScintSD::Initialize(G4HCofThisEvent* HCE){
  
  scintCollection = 
    new ScintHitsCollection( SensitiveDetectorName, collectionName[0]); 

  static G4int HCID = -1;

  if(HCID<0){ 
    HCID = GetCollectionID(0); 
  }

  HCE->AddHitsCollection( HCID, scintCollection );

  //  printf("---> ScintSD  :  Initialize.  %d\n", HCID);
}



G4bool ScintSD::ProcessHits( G4Step* aStep, G4TouchableHistory*){ 

  G4double edep = aStep->GetTotalEnergyDeposit();

  G4TouchableHistory* theTouchable = 
    (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();


  G4StepPoint* pre_point  = aStep->GetPreStepPoint();  
  G4StepPoint* post_point = aStep->GetPostStepPoint();

  G4String process = post_point->GetProcessDefinedStep()->GetProcessName();


  if( 0 && process == "compt")
    printf(" ----> computon scattering!\n");
  

  if(0)
  printf("---> ScintSD  :  Process Hits, track = %d\n", 
	 aStep->GetTrack()->GetTrackID());

  if(0)
  printf(" pre_point  = ( %f, %f, %f)\n", 
	 pre_point->GetPosition().x(),
	 pre_point->GetPosition().y(),
	 pre_point->GetPosition().z());

  if(0 && process ==  "compt")
  printf(" post_point = ( %f, %f, %f), process = %s, edep = %f\n", 
	 post_point->GetPosition().x(),
	 post_point->GetPosition().y(),
	 post_point->GetPosition().z(),
	 process.c_str(), edep);

  // No edep so don't count as hit
  if(edep==0.) 
    return false; 

  // Get the average position of the hit
  G4ThreeVector pos = pre_point->GetPosition() + post_point->GetPosition();
  pos/=2.;

  ScintHit* scintHit = new ScintHit( thePrePV);

  scintHit->SetEdep( edep);
  scintHit->SetPos( pos);

  scintCollection->insert( scintHit);

  return true;
}


void ScintSD::EndOfEvent(G4HCofThisEvent* ){
}

void ScintSD::clear(){
} 

void ScintSD::DrawAll(){
} 

void ScintSD::PrintAll(){
} 

