#include "Physics.hh"
#include "PhysicsMessenger.hh"

#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4ios.hh"
#include <iomanip>   

#include "GeneralPhysics.hh"
#include "EMPhysics.hh"
#include "MuonPhysics.hh"
#include "OpticalPhysics.hh"



Physics::Physics():  G4VModularPhysicsList()
{

  printf(" ---->  Physics  --->  flag = %d\n", flag_optical);

  physics_Messenger = new PhysicsMessenger( this);

  // default cut value  (1.0mm) 
  defaultCutValue = 1.0*mm;
  // SetVerboseLevel(1);

  // General Physics
  RegisterPhysics( new GeneralPhysics("general") );

  // EM Physics
  RegisterPhysics( new EMPhysics("standard EM"));

  // Muon Physics
  RegisterPhysics(  new MuonPhysics("muon"));

  // Optical Physics
  if( flag_optical == 0 )
    RegisterPhysics(  new OpticalPhysics("optical"));

}



Physics::~Physics()
{
 delete physics_Messenger;
}


void Physics::SetCuts(){
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 
  SetCutsWithDefault();   
}

void 
Physics::set_flag_optical(  G4bool flag) 
{  
  flag_optical = flag;

  printf("flag_optical process = %d\n", flag_optical);

}
