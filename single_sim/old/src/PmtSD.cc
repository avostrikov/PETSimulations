#include "PmtSD.hh"
#include "PmtHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"


PmtSD::PmtSD( G4String name)
  :G4VSensitiveDetector(name)  
{
  collectionName.insert("pmtCollection");
}



PmtSD::~PmtSD()
{}



void PmtSD::Initialize(G4HCofThisEvent* HCE){
  
  pmtCollection = 
    new PmtHitsCollection( SensitiveDetectorName, collectionName[0]); 

  // A way to keep all the hits of this event in one place if needed
  static G4int HCID = -1;

  if(HCID<0){ 
    HCID = GetCollectionID(0); 
  }

  HCE->AddHitsCollection( HCID, pmtCollection );

  //  printf("---> PmtSD  :  Initialize.  %d\n", HCID);
}


G4bool PmtSD::ProcessHits( G4Step* aStep, G4TouchableHistory* ROhist){ 
  if(0) printf("---> PmtSD  :  Process Hits\n"); 
  return ProcessHits_constStep(aStep,ROhist);
}



G4bool PmtSD::ProcessHits_constStep( const G4Step* aStep, G4TouchableHistory*){ 

  if(0) printf("---> PmtSD  :  Process Hits\n");

  G4double edep = aStep->GetTotalEnergyDeposit();

  //  if(edep==0.)     return false; 

  G4TouchableHistory* theTouchable = 
    (G4TouchableHistory*)(aStep->GetPostStepPoint()->GetTouchable());
  G4VPhysicalVolume* thePrePV = theTouchable->GetVolume();


  G4StepPoint* pre_point  = aStep->GetPreStepPoint();  
  G4StepPoint* post_point = aStep->GetPostStepPoint();

  // Get the average position of the hit
  G4ThreeVector pos = pre_point->GetPosition() + post_point->GetPosition();
  pos/=2.;

  PmtHit* pmtHit = new PmtHit( thePrePV);

  //  printf("Current Volume : %s\n", thePrePV->GetName().c_str());
  //  printf("Volume = %s  %s  %s\n", result[0], result[1], result[2]);


  pmtHit->SetEdep( edep);
  pmtHit->SetPos( pos);
  pmtHit->SetTime( post_point->GetGlobalTime());
  //  pmtHit->SetX( 0.);
  //  pmtHit->SetY( 0.);
  //  pmtHit->SetY( 0.);

  pmtCollection->insert( pmtHit);

  G4String process = post_point->GetProcessDefinedStep()->GetProcessName();

  if(0)
    printf(" pre_point  = ( %f, %f, %f)\n", 
	   pre_point->GetPosition().x(),
	   pre_point->GetPosition().y(),
	   pre_point->GetPosition().z());

  if(0)
    printf(" post_point = ( %f, %f, %f), time = %f, %f process = %s\n", 
	   post_point->GetPosition().x(),
	   post_point->GetPosition().y(),
	   post_point->GetPosition().z(),
	   post_point->GetGlobalTime(), 
	   post_point->GetLocalTime(), 
	   process.c_str());

  return true;
}




void PmtSD::EndOfEvent( G4HCofThisEvent*){
}

void PmtSD::clear(){
} 

void PmtSD::DrawAll(){
} 

void PmtSD::PrintAll(){
} 


/*
G4bool PmtSD::ProcessHits_constStep( const G4Step* aStep, G4TouchableHistory* ROhist){ 

  if( aStep->GetTrack()->GetDefinition() 
      != G4OpticalPhoton::OpticalPhotonDefinition()) 
    return false;
 
  //User replica number 1 since photocathode is a daughter volume
  //to the pmt which was replicated
  G4int pmtNumber=
    aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(1);
  G4VPhysicalVolume* physVol=
    aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1);

  //Find the correct hit collection
  G4int n=pmtHitCollection->entries();
  LXePMTHit* hit=NULL;
  for(G4int i=0;i<n;i++){
    if((*pmtHitCollection)[i]->GetPMTNumber()==pmtNumber){
      hit=(*pmtHitCollection)[i];
      break;
    }
  }
  
  if(hit==NULL){//this pmt wasnt previously hit in this event
    hit = new LXePMTHit(); //so create new hit
    hit->SetPMTNumber(pmtNumber);
    hit->SetPMTPhysVol(physVol);
    pmtHitCollection->insert(hit);
    hit->SetPMTPos((*pmtPositionsX)[pmtNumber],(*pmtPositionsY)[pmtNumber],
		   (*pmtPositionsZ)[pmtNumber]);
  }

  hit->IncPhotonCount(); //increment hit for the selected pmt
    

  return true;
}
*/

/*
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
13 NoRINDEX 
*/
