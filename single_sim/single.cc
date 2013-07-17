#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4SDManager.hh"
#include "G4UImessenger.hh"
#include "G4VisExecutive.hh"
#include "G4ios.hh"

#include "Randomize.hh" 
#include "globals.hh"

#include "Detector.hh"
#include "Physics.hh"
#include "Generator.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "StackAction.hh"
#include "TrackAction.hh"
#include "StepAction.hh"
#include "Hist.hh"

#include "DetectorMessenger.hh"
#include "RunMessenger.hh"

int
main( int argc, char** argv){

  // choose the Random engine
  CLHEP::HepRandom::setTheEngine( new CLHEP::RanecuEngine);
  G4int seed = time( 0);
  CLHEP::HepRandom :: setTheSeed( seed);

  printf("Seed = %d\n", seed);


  G4RunManager* runManager = new G4RunManager;
  Detector*     pDetector  = new Detector();
  Physics*      pPhysics   = new Physics();

  // set mandatory initialization
  runManager->SetUserInitialization( pDetector);
  runManager->SetUserInitialization( pPhysics);


  // set mandatory user action class  
  Generator* pGenerator = new Generator();
  runManager->SetUserAction( pGenerator);

  RunAction* pRunAction = new RunAction();
  runManager->SetUserAction( pRunAction);  

  EventAction* pEventAction = new EventAction();
  runManager->SetUserAction( pEventAction);

  StackAction* pStackAction =  new StackAction();
  runManager->SetUserAction( pStackAction);

  TrackAction* pTrackAction =  new TrackAction();
  runManager->SetUserAction( pTrackAction);

  StepAction* pStepAction =  new StepAction();
  runManager->SetUserAction( pStepAction);

  
  // initialize  
  runManager->Initialize();


  // visualization 
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UI = G4UImanager::GetUIpointer();
     
  if( argc==1){
    G4UIsession* session = 0;
    session = new G4UIterminal();      
    // session = new G4UIterminal(new G4UItcsh);      
    
    UI->ApplyCommand("/control/execute vis.mac"); 
    session->SessionStart();
    delete session;
  }   
  else{
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand( command + fileName);
  }
  
  // job termination
  delete visManager;
  delete runManager;

  return 0;
}
