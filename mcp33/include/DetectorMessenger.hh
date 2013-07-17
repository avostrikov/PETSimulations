#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class Detector;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(Detector*);
   ~DetectorMessenger();
    
    void SetNewValue( G4UIcommand*, G4String);
    
  private:
    Detector*                  detector;
    G4UIdirectory*             dir;
    G4UIcmdWithADoubleAndUnit* hall_x_cmd, *hall_y_cmd, *hall_z_cmd;
    G4UIcmdWithADoubleAndUnit* slab_x_cmd, *slab_y_cmd, *slab_z_cmd;
    G4UIcmdWithADoubleAndUnit* guide_x_cmd, *guide_y_cmd, *guide_z_cmd;
    G4UIcmdWithADoubleAndUnit*   sipm_x_cmd, *sipm_y_cmd, *sipm_z_cmd, *sipm_r_cmd;
    G4UIcmdWithADoubleAndUnit* distance_cmd;
    G4UIcmdWithoutParameter*   update_cmd;
};

#endif

