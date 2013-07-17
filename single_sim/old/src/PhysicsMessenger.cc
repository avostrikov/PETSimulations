#include "PhysicsMessenger.hh"
#include "Physics.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"


PhysicsMessenger::PhysicsMessenger( Physics* Physics_action)
 :physics(Physics_action)
{   

  dir = new G4UIdirectory("/physics/");
  dir->SetGuidance("Physics List Control");
  
  flag_optical_cmd = new G4UIcmdWithABool("/physics/optical",this);
  flag_optical_cmd->SetGuidance("Flag for Optical Process");
  flag_optical_cmd->SetParameterName("flag_opticcal", true);
  flag_optical_cmd->SetDefaultValue( true);
}





PhysicsMessenger::~PhysicsMessenger()
{

  delete flag_optical_cmd;
  delete dir;  
}




void 
PhysicsMessenger::SetNewValue( G4UIcommand* command, G4String newValue)
{ 

  if( command == flag_optical_cmd )
    physics->set_flag_optical( flag_optical_cmd->GetNewBoolValue(newValue));
 
}
