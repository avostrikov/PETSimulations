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
  if(0) printf("---> org PmtSD  :  Process Hits\n"); 
  // return ProcessHits_constStep(aStep,ROhist);
  return true;
}



G4bool PmtSD::ProcessHits_constStep( const G4Step* aStep, G4TouchableHistory*){ 

  if(0) printf("---> PmtSD  :  Process Hits\n");

  G4double edep = aStep->GetTotalEnergyDeposit();
  G4Track* Track = aStep->GetTrack();

  if(0){
  printf("--> ProcessHits, TrackID = %d,  Kinetic Energy = %e,  VertexKinetic = %e\n", 
	 Track->GetTrackID(), Track->GetKineticEnergy(),  Track->GetVertexKineticEnergy());

  G4cout << "Momentum : " << Track->GetMomentum() << G4endl;
  G4cout << "Momentum.mag : " << Track->GetMomentum().mag() << G4endl;
  }


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


  //  pmtHit->SetEdep( edep);
  //  record wavelength of photon
  pmtHit->SetEdep( 1240./(Track->GetKineticEnergy()*1000000.));
  pmtHit->SetPos( pos);
  pmtHit->SetTime( post_point->GetGlobalTime());
  //  pmtHit->SetX( 0.);
  //  pmtHit->SetY( 0.);
  //  pmtHit->SetZ( 0.);

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
