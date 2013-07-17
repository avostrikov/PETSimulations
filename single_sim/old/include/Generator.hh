
#ifndef Generator_h
#define Generator_h 1

#include "G4GeneralParticleSource.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4ParticleGun;
class G4Event;

class Generator : public G4VUserPrimaryGeneratorAction
{
public:
  Generator();
  ~Generator();
  
public:
  void GeneratePrimaries(G4Event* anEvent);
  
private:
  //    G4ParticleGun* particleGun;
    G4GeneralParticleSource* particleGun;
};

#endif


