#ifndef Trajectory_h
#define Trajectory_h 1
#include "G4Trajectory.hh"
#include "G4Allocator.hh"
#include "G4ios.hh" 
#include "globals.hh" 
#include "G4ParticleDefinition.hh" 
#include "G4TrajectoryPoint.hh"
#include "G4Track.hh"
#include "G4Step.hh"



class Trajectory : public G4Trajectory
{
public:
  Trajectory();
  Trajectory(const G4Track* aTrack);
  Trajectory(Trajectory &);
  virtual ~Trajectory();
  
  //  virtual void DrawTrajectory(G4int i_mode=0) const;
  
  inline void* operator new(size_t);
  inline void  operator delete(void*);


public:
  void SetStatus(G4int istat);
  void SetPreVolume(G4String svol);
  void SetPostVolume(G4String svol);
  void SetProcess(G4String process);
  void SetLength(G4double length);

  inline G4int   GetStatus() {return Status;}
  inline G4String GetPreVolume() {return PreVol;}
  inline G4String GetPostVolume() {return PostVol;}
  inline G4String GetProcess() {return Process;}
  inline G4double GetLength() {return Length;}

private:
  G4int Status;
  G4String PreVol;
  G4String PostVol;
  G4String Process;
  G4double Length;
  


};



extern G4Allocator<Trajectory> TrajectoryAllocator;

inline void* Trajectory::operator new(size_t)
{
  void* aTrajectory;
  aTrajectory = (void*)TrajectoryAllocator.MallocSingle();
  return aTrajectory;
}


inline void Trajectory::operator delete(void* aTrajectory)
{
  TrajectoryAllocator.FreeSingle((Trajectory*)aTrajectory);
}


#endif
