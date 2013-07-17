#include "Trajectory.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Trajectory.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4ThreeVector.hh"
#include "G4Polyline.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"
#include "G4Polymarker.hh"


G4Allocator<Trajectory> TrajectoryAllocator;

Trajectory::Trajectory()
  :G4Trajectory()
{
  Status = 0;
}



Trajectory::Trajectory(const G4Track* aTrack)
  :G4Trajectory(aTrack)
{
  Status = 0;
}



Trajectory::Trajectory(Trajectory &right)
  :G4Trajectory(right)
{
  Status = right.GetStatus();
  PreVol = right.GetPreVolume();
  PostVol = right.GetPostVolume();
  Process = right.GetProcess();
  Length = right.GetLength();
}



Trajectory::~Trajectory()
{

}


//void Trajectory::DrawTrajectory(G4int i_mode) const{
//  
//}



void Trajectory::SetStatus(G4int istat){
  Status = istat;
}
 
void Trajectory::SetPreVolume(G4String svol){
  PreVol = svol;
}


void Trajectory::SetPostVolume(G4String svol){
  PostVol = svol;
}


void Trajectory::SetProcess(G4String process){
  Process = process;
}

void Trajectory::SetLength(G4double length){
  Length = length;
}




