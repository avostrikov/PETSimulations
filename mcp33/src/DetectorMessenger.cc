#include "DetectorMessenger.hh"
#include "Detector.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"


DetectorMessenger::DetectorMessenger( Detector* Det)
  :detector(Det)
{   
  dir = new G4UIdirectory("/detector/");
  dir->SetGuidance("detector setup");
  
  hall_x_cmd = new G4UIcmdWithADoubleAndUnit("/detector/hall_x",this);
  hall_x_cmd->SetGuidance("Set hall_x (positive)");
  hall_x_cmd->SetParameterName("hall_x",false);
  hall_x_cmd->SetRange("hall_x>=0. && hall_x<10000.");
  hall_x_cmd->SetDefaultUnit( "mm" );
  hall_x_cmd->SetUnitCategory("Length");
  hall_x_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  hall_y_cmd = new G4UIcmdWithADoubleAndUnit("/detector/hall_y",this);
  hall_y_cmd->SetGuidance("Set hall_y (positive)");
  hall_y_cmd->SetParameterName("hall_y",false);
  hall_y_cmd->SetRange("hall_y>=0. && hall_y<10000.");
  hall_y_cmd->SetDefaultUnit( "mm" );
  hall_y_cmd->SetUnitCategory("Length");
  hall_y_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  hall_z_cmd = new G4UIcmdWithADoubleAndUnit("/detector/hall_z",this);
  hall_z_cmd->SetGuidance("Set hall_z (positive)");
  hall_z_cmd->SetParameterName("hall_z",false);
  hall_z_cmd->SetRange("hall_z>=0. && hall_z<10000.");
  hall_z_cmd->SetDefaultUnit( "mm" );
  hall_z_cmd->SetUnitCategory("Length");
  hall_z_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  slab_x_cmd = new G4UIcmdWithADoubleAndUnit("/detector/slab_x",this);
  slab_x_cmd->SetGuidance("Set slab_x (positive)");
  slab_x_cmd->SetParameterName("slab_x",false);
  slab_x_cmd->SetRange("slab_x>=0. && slab_x<1000.");
  slab_x_cmd->SetDefaultUnit( "mm" );
  slab_x_cmd->SetUnitCategory("Length");
  slab_x_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  slab_y_cmd = new G4UIcmdWithADoubleAndUnit("/detector/slab_y",this);
  slab_y_cmd->SetGuidance("Set slab_y (positive)");
  slab_y_cmd->SetParameterName("slab_y",false);
  slab_y_cmd->SetRange("slab_y>=0. && slab_y<1000.");
  slab_y_cmd->SetDefaultUnit( "mm" );
  slab_y_cmd->SetUnitCategory("Length");
  slab_y_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  slab_z_cmd = new G4UIcmdWithADoubleAndUnit("/detector/slab_z",this);
  slab_z_cmd->SetGuidance("Set slab_z (positive)");
  slab_z_cmd->SetParameterName("slab_z",false);
  slab_z_cmd->SetRange("slab_z>=0. && slab_z<1000.");
  slab_z_cmd->SetDefaultUnit( "mm" );
  slab_z_cmd->SetUnitCategory("Length");
  slab_z_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  sipm_x_cmd = new G4UIcmdWithADoubleAndUnit("/detector/sipm_x",this);
  sipm_x_cmd->SetGuidance("Set sipm_x (positive)");
  sipm_x_cmd->SetParameterName("sipm_x",false);
  sipm_x_cmd->SetRange("sipm_x>=0. && sipm_x<1000.");
  sipm_x_cmd->SetDefaultUnit( "mm" );
  sipm_x_cmd->SetUnitCategory("Length");
  sipm_x_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  sipm_y_cmd = new G4UIcmdWithADoubleAndUnit("/detector/sipm_y",this);
  sipm_y_cmd->SetGuidance("Set sipm_y (positive)");
  sipm_y_cmd->SetParameterName("sipm_y",false);
  sipm_y_cmd->SetRange("sipm_y>=0. && sipm_y<1000.");
  sipm_y_cmd->SetDefaultUnit( "mm" );
  sipm_y_cmd->SetUnitCategory("Length");
  sipm_y_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  sipm_z_cmd = new G4UIcmdWithADoubleAndUnit("/detector/sipm_z",this);
  sipm_z_cmd->SetGuidance("Set sipm_z (positive)");
  sipm_z_cmd->SetParameterName("sipm_z",false);
  sipm_z_cmd->SetRange("sipm_z>=0. && sipm_z<1000.");
  sipm_z_cmd->SetDefaultUnit( "mm" );
  sipm_z_cmd->SetUnitCategory("Length");
  sipm_z_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  sipm_r_cmd = new G4UIcmdWithADoubleAndUnit("/detector/sipm_r",this);
  sipm_r_cmd->SetGuidance("Set sipm_r (positive)");
  sipm_r_cmd->SetParameterName("sipm_r",false);
  sipm_r_cmd->SetRange("sipm_r>=0. && sipm_r<1000.");
  sipm_r_cmd->SetDefaultUnit( "mm" );
  sipm_r_cmd->SetUnitCategory("Length");
  sipm_r_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);


  guide_x_cmd = new G4UIcmdWithADoubleAndUnit("/detector/guide_x",this);
  guide_x_cmd->SetGuidance("Set guide_x (positive)");
  guide_x_cmd->SetParameterName("guide_x",false);
  guide_x_cmd->SetRange("guide_x>=0. && guide_x<1000.");
  guide_x_cmd->SetDefaultUnit( "mm" );
  guide_x_cmd->SetUnitCategory("Length");
  guide_x_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  guide_y_cmd = new G4UIcmdWithADoubleAndUnit("/detector/guide_y",this);
  guide_y_cmd->SetGuidance("Set guide_y (positive)");
  guide_y_cmd->SetParameterName("guide_y",false);
  guide_y_cmd->SetRange("guide_y>=0. && guide_y<1000.");
  guide_y_cmd->SetDefaultUnit( "mm" );
  guide_y_cmd->SetUnitCategory("Length");
  guide_y_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);


  guide_z_cmd = new G4UIcmdWithADoubleAndUnit("/detector/guide_z",this);
  guide_z_cmd->SetGuidance("Set guide_z (positive)");
  guide_z_cmd->SetParameterName("guide_z",false);
  guide_z_cmd->SetRange("guide_z>=0. && guide_z<1000.");
  guide_z_cmd->SetDefaultUnit( "mm" );
  guide_z_cmd->SetUnitCategory("Length");
  guide_z_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);


  distance_cmd = new G4UIcmdWithADoubleAndUnit("/detector/distance",this);
  distance_cmd->SetGuidance("Set distance (positive)");
  distance_cmd->SetParameterName("distance",false);
  distance_cmd->SetRange("distance>=0. && distance<1000.");
  distance_cmd->SetDefaultUnit( "mm" );
  distance_cmd->SetUnitCategory("Length");
  distance_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);


  update_cmd = new G4UIcmdWithoutParameter("/detector/update",this);
  update_cmd->SetGuidance("Update geometry.");
  update_cmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  update_cmd->SetGuidance("if you changed geometrical value(s).");
  update_cmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}





DetectorMessenger::~DetectorMessenger()
{

  delete hall_x_cmd;
  delete hall_y_cmd;
  delete hall_z_cmd;

  delete slab_x_cmd;
  delete slab_y_cmd;
  delete slab_z_cmd;

  delete sipm_x_cmd;
  delete sipm_y_cmd;
  delete sipm_z_cmd;
  delete sipm_r_cmd;

  delete guide_x_cmd;
  delete guide_y_cmd;

  delete distance_cmd;
  delete update_cmd;

  delete dir;  
}




void 
DetectorMessenger::SetNewValue( G4UIcommand* command, G4String newValue)
{ 

  if( command == hall_x_cmd )
    detector->set_hall_x( hall_x_cmd->GetNewDoubleValue(newValue));

  if( command == hall_y_cmd )
    detector->set_hall_y( hall_y_cmd->GetNewDoubleValue(newValue));

  if( command == hall_z_cmd )
    detector->set_hall_z( hall_z_cmd->GetNewDoubleValue(newValue));


  if( command == slab_x_cmd )
    detector->set_slab_x( slab_x_cmd->GetNewDoubleValue(newValue));

  if( command == slab_y_cmd )
    detector->set_slab_y( slab_y_cmd->GetNewDoubleValue(newValue));

  if( command == slab_z_cmd )
    detector->set_slab_z( slab_z_cmd->GetNewDoubleValue(newValue));


  if( command == sipm_x_cmd )
    detector->set_sipm_x( sipm_x_cmd->GetNewDoubleValue(newValue));

  if( command == sipm_y_cmd )
    detector->set_sipm_y( sipm_y_cmd->GetNewDoubleValue(newValue));

  if( command == sipm_z_cmd )
    detector->set_sipm_z( sipm_z_cmd->GetNewDoubleValue(newValue));

  if( command == sipm_r_cmd )
    detector->set_sipm_r( sipm_r_cmd->GetNewDoubleValue(newValue));


  if( command == guide_x_cmd )
    detector->set_guide_x( guide_x_cmd->GetNewDoubleValue(newValue));

  if( command == guide_y_cmd )
    detector->set_guide_y( guide_y_cmd->GetNewDoubleValue(newValue));

  if( command == guide_z_cmd )
    detector->set_guide_z( guide_z_cmd->GetNewDoubleValue(newValue));

  if( command == distance_cmd )
    detector->set_distance( distance_cmd->GetNewDoubleValue(newValue));

  if( command == update_cmd )
    detector->update_geometry(); 
}
