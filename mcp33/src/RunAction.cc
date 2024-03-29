#include "RunAction.hh"
#include "RunMessenger.hh"
#include "Hist.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4Timer.hh"



RunAction::RunAction()
{
  run_Messenger = new RunMessenger( this);
}


RunAction::~RunAction()
{ 
  delete run_Messenger;
}



void 
RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " starts." << G4endl;

  HistManager* hist = HistManager::getInstance();
  hist->set_file_name( filename);
  hist->book();
}


void 
RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " ends." << G4endl;

  HistManager* hist = HistManager::getInstance();
  hist->finish();
}


void 
RunAction::set_file_name(  G4String name) 
{  
  filename = name;
}

void
RunAction::get_file_name() 
{  
  printf("Hist file name = %s\n", filename.c_str());
}


