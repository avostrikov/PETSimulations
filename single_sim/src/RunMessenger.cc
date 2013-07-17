#include "RunMessenger.hh"
#include "RunAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"


RunMessenger::RunMessenger( RunAction* Run_action)
 :run_action(Run_action)
{   

  dir = new G4UIdirectory("/histo/");
  dir->SetGuidance("histogram setup");
  
  file_name_cmd = new G4UIcmdWithAString("/histo/file",this);
  file_name_cmd->SetGuidance("Set file name.");
}





RunMessenger::~RunMessenger()
{

  delete file_name_cmd;
  delete dir;  
}




void 
RunMessenger::SetNewValue( G4UIcommand* command, G4String newValue)
{ 

  if( command == file_name_cmd ){
    run_action->set_file_name( newValue);
  }  
}
