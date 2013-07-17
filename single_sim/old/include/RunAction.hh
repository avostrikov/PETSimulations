#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4RunManager.hh"
#include "globals.hh"

#include "RunMessenger.hh"

class RunMessenger;
class G4Run;

class RunAction : public G4UserRunAction
{

public:
  RunAction();
  ~RunAction();
  
public:
  void BeginOfRunAction(const G4Run* aRun);
  void EndOfRunAction(const G4Run* aRun);

  void set_file_name( G4String);
  void get_file_name();

private:
  G4String filename;
  RunMessenger* run_Messenger;

};


#endif

