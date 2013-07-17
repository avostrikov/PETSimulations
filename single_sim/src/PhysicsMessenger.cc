#include "PhysicsMessenger.hh"
#include "Physics.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithoutParameter.hh"


PhysicsMessenger::PhysicsMessenger( Physics* pPhysicsList)
 :physics(pPhysicsList)
{   

  dir = new G4UIdirectory("/physics/");
  dir->SetGuidance("Physics List Control");
  
  enable_optical_cmd = new G4UIcmdWithABool("/physics/optical",this);
  enable_optical_cmd->SetGuidance("Flag for Optical Process");
  enable_optical_cmd->SetParameterName("flag_optical", false);
  //  flag_optical_cmd->SetDefaultValue( true);

  verbose_cmd = new G4UIcmdWithAnInteger("/physics/verbose",this);  
  verbose_cmd->SetGuidance("set verbose for physics processes");
  verbose_cmd->SetParameterName("verbose",true);
  verbose_cmd->SetDefaultValue(1);
  verbose_cmd->SetRange("verbose>=0");
  verbose_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  cerenkov_cmd = new G4UIcmdWithAnInteger("/physics/cerenkovMaxPhotons",this);  
  cerenkov_cmd->SetGuidance("set max nb of photons per step");
  cerenkov_cmd->SetParameterName("MaxNumber",false);
  cerenkov_cmd->SetRange("MaxNumber>=0");
  cerenkov_cmd->AvailableForStates(G4State_Idle);  
}


PhysicsMessenger::~PhysicsMessenger()
{
  delete dir;  

  delete enable_optical_cmd;
  delete verbose_cmd;
  delete cerenkov_cmd;

}


void 
PhysicsMessenger::SetNewValue( G4UIcommand* command, G4String newValue)
{ 

  if( command == enable_optical_cmd )
    physics->enable_optical( enable_optical_cmd->GetNewBoolValue(newValue));

  if( command == verbose_cmd )
    physics->SetVerbose(verbose_cmd->GetNewIntValue(newValue));
   
  if( command == cerenkov_cmd )
    physics->SetNbOfPhotonsCerenkov(cerenkov_cmd->GetNewIntValue(newValue));
 
}




