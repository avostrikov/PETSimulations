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

  if(0) printf("---> Event# %6d  End\n", evtid);

  UserEventInformation* eventInformation
    = (UserEventInformation*)evt->GetUserInformation();

  ScintHitsCollection* SHC = 0;
  PmtHitsCollection* PHC = 0;
  LightHitsCollection* LHC = 0;

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();

  // Get the hit collections
  if( HCE){
    if( pmtCollID >= 0)
      PHC = (PmtHitsCollection*)(HCE->GetHC(pmtCollID));
    if( scintCollID >= 0)
      SHC = (ScintHitsCollection*)(HCE->GetHC(scintCollID));
    if( lightCollID >= 0)
      LHC = (LightHitsCollection*)(HCE->GetHC(lightCollID));
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

  
  if( LHC){
    if(1) printf("# of Light Hits = %d\n", LHC->entries()); 
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
 


  G4TrajectoryContainer* trajectoryContainer= evt->GetTrajectoryContainer();
  
  G4int n_traj = 0;
  if( trajectoryContainer) 
    n_traj = trajectoryContainer->entries();

  // extract the trajectories 
  int ndet = 0;
  int nbulk = 0;
  int nside = 0;
  int nopt  = 0;

  for( G4int i = 0; i < n_traj; i++){ 

    Trajectory* trj = (Trajectory*)
      ((*(evt->GetTrajectoryContainer()))[i]);
            
    int lastid =  trj->GetPointEntries();
      
    G4VTrajectoryPoint* tpoint = trj->GetPoint(lastid-1);
            
    if(0)G4cout << tpoint->GetPosition() << G4endl;
      
    if(0)printf("TrackID = %d, Name = %s, npoint = %d, "
		"Status = %d, PreVolume  = %s,   PostVolume  = %s, Process = %s\n",
		trj->GetTrackID(), trj->GetParticleName().c_str(),
		trj->GetPointEntries(),
		trj->GetStatus(), 
		trj->GetPreVolume().c_str(), 
		trj->GetPostVolume().c_str(),
		trj->GetProcess().c_str());
      
    if( trj->GetParticleName() != "opticalphoton"){
      nopt++;
      continue;
    }

    hist->fill_length2( trj->GetLength());

    int flag1, flag2, flag3;
    flag1 = flag2 = flag3 = 0;
      
    // detected
    if( strncmp( trj->GetPreVolume().c_str(), "glass", 5) == 0 &&
	strncmp( trj->GetPostVolume().c_str(), "photocathode", 12) == 0){
      ndet++;
      flag1 = 1;
    }

    // bulk absorption
    if(  trj->GetProcess() == "OpAbsorption"){
      nbulk++;
      flag2 = 1;

      if(0)
	printf("TrackID = %d, ParentID = %d, npoint = %d, "
	       "PreVolume  = %s,   PostVolume  = %s\n",
	       trj->GetTrackID(), 
	       trj->GetParentID(), 
	       trj->GetPointEntries(),
	       trj->GetPreVolume().c_str(), 
	       trj->GetPostVolume().c_str());

      if(0){
        printf("%7.2f,   %d\n", trj->GetLength(), trj->GetPointEntries());
	G4cout << tpoint->GetPosition() << G4endl;
      }

      
      // absorption except scintillator
      if( 0 && (
		strncmp( trj->GetPostVolume().c_str(), "scint_slab", 10) != 0 &&
		strncmp( trj->GetPostVolume().c_str(), "exp_hall", 8) != 0)){
	printf("TrackID = %d, ParentID = %d, npoint = %d, "
	       "PreVolume  = %s,   PostVolume  = %s, Process = %s\n",
	       trj->GetTrackID(), 
	       trj->GetParentID(),
	       trj->GetPointEntries(),
	       trj->GetPreVolume().c_str(), 
	       trj->GetPostVolume().c_str(),
	       trj->GetProcess().c_str());
      }

      hist->fill_length( trj->GetLength());
    }
	
    // absorption at sides
    if(  trj->GetProcess() == "Transportation" &&
	 (strncmp( trj->GetPreVolume().c_str(), "scint_slab", 10) == 0 ||
	  strncmp( trj->GetPreVolume().c_str(), "glass", 5) == 0 )&&
	 strncmp( trj->GetPostVolume().c_str(), "exp_hall", 8) == 0){
      nside++;
      flag3 = 1;

      if(0){
      printf("TrackID = %d, Name = %s, npoint = %d, "
	     "Status = %d, PreVolume  = %s,   PostVolume  = %s, Process = %s\n",
	     trj->GetTrackID(), trj->GetParticleName().c_str(),
	     trj->GetPointEntries(),
	     trj->GetStatus(), 
	     trj->GetPreVolume().c_str(), 
	     trj->GetPostVolume().c_str(),
	     trj->GetProcess().c_str());

      G4cout << tpoint->GetPosition() << G4endl;
      }

    }

    if( 1 && flag1 == 0 && flag2 == 0 && flag3 == 0){
      printf("TrackID = %d, Name = %s, npoint = %d, "
	     "Status = %d, PreVolume  = %s,   PostVolume  = %s, Process = %s\n",
	     trj->GetTrackID(), trj->GetParticleName().c_str(),
	     trj->GetPointEntries(),
	     trj->GetStatus(), 
	     trj->GetPreVolume().c_str(), 
	     trj->GetPostVolume().c_str(),
	     trj->GetProcess().c_str());

      G4cout << tpoint->GetPosition() << G4endl;
    }
  }

  hist->fill_ntrack( n_traj);
  hist->fill_ndet( ndet);
  hist->fill_nside( nside);
  hist->fill_nbulk( nbulk);

  int nsum = nopt+ ndet + nside + nbulk;

  if(1)
  printf("ntrack = %d, nsum = %d, nopt = %d, ndet = %d, nside = %d, nbulk = %d, rate = %5.2f%%\n",
	 n_traj, nsum, nopt, ndet, nside, nbulk, (double)ndet/(double)nside*100.);


  if( ndet == 0 &&  n_traj == 1 )
    hist->fill_nint0();

  if( ndet == 0 &&  n_traj >  1 )
    hist->fill_nint1();

  if( ndet != 0)
    hist->fill_nint2();

  

  if( ndet == 0 && 0)
    for( G4int i = 0; i < n_traj; i++){ 

      Trajectory* trj = (Trajectory*)
	((*(evt->GetTrajectoryContainer()))[i]);
      
      int lastid =  trj->GetPointEntries();      
      G4VTrajectoryPoint* tpoint = trj->GetPoint(lastid-1);
      
      
      if(1)printf("TrackID = %d, Name = %s, npoint = %d, "
		  "Status = %d, PreVolume  = %s,   PostVolume  = %s, Process = %s\n",
		  trj->GetTrackID(), trj->GetParticleName().c_str(),
		  trj->GetPointEntries(),
		  trj->GetStatus(), 
		  trj->GetPreVolume().c_str(), 
		  trj->GetPostVolume().c_str(),
		  trj->GetProcess().c_str());

      if(1)G4cout << tpoint->GetPosition() << G4endl;
    }

  
  



}
