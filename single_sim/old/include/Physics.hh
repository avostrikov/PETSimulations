#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

#include "PhysicsMessenger.hh"


class Physics: public G4VModularPhysicsList
{

public:
  Physics();
  virtual ~Physics();

public:
  virtual void SetCuts();
  void set_flag_optical( G4bool flag);


private:
  G4bool              flag_optical;
  PhysicsMessenger*   physics_Messenger;

};


#endif



