#ifndef PhysicsMessenger_h
#define PhysicsMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class Physics;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;
class G4UIcmdWithoutParameter;


class PhysicsMessenger: public G4UImessenger
{
  public:
    PhysicsMessenger(Physics*);
   ~PhysicsMessenger();
    
    void SetNewValue( G4UIcommand*, G4String);
    
  private:
    Physics*               physics;
    G4UIdirectory*         dir;
    G4UIcmdWithABool*      enable_optical_cmd;
    G4UIcmdWithAnInteger*  verbose_cmd;
    G4UIcmdWithAnInteger*  cerenkov_cmd;
};

#endif

