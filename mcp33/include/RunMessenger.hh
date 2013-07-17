#ifndef RunMessenger_h
#define RunMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RunAction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


class RunMessenger: public G4UImessenger
{
  public:
    RunMessenger(RunAction*);
   ~RunMessenger();
    
    void SetNewValue( G4UIcommand*, G4String);
    
  private:
    RunAction*             run_action;
    G4UIdirectory*         dir;
    G4UIcmdWithAString*    file_name_cmd;
};

#endif

