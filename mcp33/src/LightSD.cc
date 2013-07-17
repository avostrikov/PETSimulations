#include "LightSD.hh"
#include "LightHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"


LightSD::LightSD( G4String name)
  :G4VSensitiveDetector(name)  
{
  collectionName.insert("lightCollection");
}



LightSD::~LightSD()
{}



void LightSD::Initialize(G4HCofThisEvent* HCE){

  
  lightCollection = 
    new LightHitsCollection( SensitiveDetectorName, collectionName[0]); 

  //A way to keep all the hits of this event in one place if needed
  static G4int HCID = -1;

  if(HCID<0){ 
    HCID = GetCollectionID(0); 
  }

  HCE->AddHitsCollection( HCID, lightCollection );

  //  printf("---> LightSD  :  Initialize.  %d\n", HCID);
}



G4bool LightSD::ProcessHits( G4Step* aStep, G4TouchableHistory*){ 

  G4double edep = aStep->GetTotalEnergyDeposit();

  G4TouchableHistory* theTouchable = 
    (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();


  G4StepPoint* pre_point  = aStep->GetPreStepPoint();  
  G4StepPoint* post_point = aStep->GetPostStepPoint();

  G4String process = post_point->GetProcessDefinedStep()->GetProcessName();

  G4Track* Track = aStep->GetTrack();

  if(0)
    printf("---> LightSD  :  Process Hits,  trackid = %d\n", 
	   aStep->GetTrack()->GetTrackID());

  if(0)
    printf(" pre_point  = ( %f, %f, %f)\n", 
	   pre_point->GetPosition().x(),
	   pre_point->GetPosition().y(),
	   pre_point->GetPosition().z());

  if(0)
    printf(" post_point = ( %f, %f, %f), process = %s, edep = %f, %f, %f\n", 
	   post_point->GetPosition().x(),
	   post_point->GetPosition().y(),
	   post_point->GetPosition().z(),
	   process.c_str(),
	   edep,
	   Track->GetKineticEnergy(), 
	   Track->GetVertexKineticEnergy());

  // No edep so dont count as hit
  if(edep==0. || edep ==  Track->GetKineticEnergy()) 
    return false; 


  // Get the average position of the hit
  G4ThreeVector pos = pre_point->GetPosition() + post_point->GetPosition();
  pos/=2.;

  LightHit* lightHit = new LightHit( thePrePV);

  lightHit->SetEdep( edep);
  lightHit->SetPos( pos);

  lightCollection->insert( lightHit);

  return true;
}


void LightSD::EndOfEvent(G4HCofThisEvent* ){
}

void LightSD::clear(){
} 

void LightSD::DrawAll(){
} 

void LightSD::PrintAll(){
} 

