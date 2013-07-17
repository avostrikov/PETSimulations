#include "EventAction.hh"
#include "Hist.hh"
#include "ScintHit.hh"
#include "LightHit.hh"
#include "PmtHit.hh"
#include "UserEventInformation.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"


EventAction::EventAction()
{
  scintCollID = -1;
  lightCollID = -1;
  pmtCollID = -1;

}

 
EventAction::~EventAction()
{
}

 
void EventAction::BeginOfEventAction( const G4Event* evt)
{
  printf("---> Event# %6d  Start\n", evt->GetEventID());

  G4EventManager::GetEventManager()->SetUserInformation( new UserEventInformation);
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

  if(scintCollID<0)
    scintCollID=SDman->GetCollectionID("scintCollection");

  if(lightCollID<0)
    lightCollID=SDman->GetCollectionID("lightCollection");

  if(pmtCollID<0)
    pmtCollID=SDman->GetCollectionID("pmtCollection");

}


 
void EventAction::EndOfEventAction( const G4Event* evt)
{    

  HistManager* hist = HistManager::getInstance();  

  int evtid = evt->GetEventID();

  if(1) printf("---> Event# %6d  End\n", evtid);

  UserEventInformation* eventInformation
    = (UserEventInformation*)evt->GetUserInformation();

  ScintHitsCollection* SHC = 0;
  PmtHitsCollection* PHC = 0;

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();

  // Get the hit collections
  if( HCE){
    if( pmtCollID >= 0)
      PHC = (PmtHitsCollection*)(HCE->GetHC(pmtCollID));
    if( scintCollID >= 0)
      SHC = (ScintHitsCollection*)(HCE->GetHC(scintCollID));
  }


  if( PHC){
    if(1) printf("# of PMT Hits = %d\n", PHC->entries());

    for( int i = 0; i < PHC->entries(); i++){
      
      G4ThreeVector pos =  (*PHC)[i]->GetPos();
      double t = (*PHC)[i]->GetTime();
      double e = (*PHC)[i]->GetEdep();

      if(0) G4cout << " Position  :  " << pos << G4endl;

      hist->fill_sipm( evtid, pos.x(), pos.y(), pos.z(), t, e);
    }
  }

  
  double energy_dep = 0;

  // Hits in scintillator
  if( SHC){

    if(1) printf("# of Scintillation Hits = %d\n", SHC->entries());

    G4ThreeVector  eWeightPos(0.);     
    G4double edepMax = 0;

    for( int i = 0; i < SHC->entries(); i++){

      G4ThreeVector pos=(*SHC)[i]->GetPos();
      double t = (*SHC)[i]->GetTime();
      double e = (*SHC)[i]->GetEdep();

      energy_dep += e;
      eventInformation->IncEDep( e); 
      //calculate energy weighted pos
      eWeightPos += (*SHC)[i]->GetPos()*e;

      //store max energy deposit
      if( e > edepMax){
	edepMax = e;
	eventInformation->SetPosMax( pos, e);
      }

      hist->fill_scint( evtid, pos.x(), pos.y(), pos.z(), t, e); 
    }


    if( eventInformation->GetEDep() == 0.){
	G4cout<<"No hits in the scintillator this event."<< G4endl;
    }    
    else{
      // Finish calculation of energy weighted position
      eWeightPos /= eventInformation->GetEDep();
      eventInformation->SetEWeightPos( eWeightPos);      
    }

    if(1)
      G4cout << "     Total Energy Deposit : "
	     << eventInformation->GetEDep() / keV << " (keV)" << G4endl;
  }
  
  hist->fill_h1( energy_dep/keV);
}
