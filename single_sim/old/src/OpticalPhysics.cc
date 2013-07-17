#include "OpticalPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>   

#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"


#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4OpticalPhoton.hh"
#include "G4ProcessManager.hh"

OpticalPhysics::OpticalPhysics(const G4String& name)
  :  G4VPhysicsConstructor(name)
{

}

OpticalPhysics::~OpticalPhysics()
{
}

void OpticalPhysics::ConstructParticle()
{
  G4OpticalPhoton::OpticalPhotonDefinition();
}


void OpticalPhysics::ConstructProcess()
{
  theScintProcess       = new G4Scintillation();
  theCerenkovProcess    = new G4Cerenkov();
  theAbsorptionProcess  = new G4OpAbsorption();
  theRayleighScattering = new G4OpRayleigh();
  theBoundaryProcess    = new G4OpBoundaryProcess();
  theWLSProcess         = new G4OpWLS();

  theWLSProcess->UseTimeProfile("delta");
  //theWLSProcess->UseTimeProfile("exponential");

  theBoundaryProcess->SetModel( unified);
  
  theScintProcess->SetScintillationYieldFactor(1.);
  theScintProcess->SetScintillationExcitationRatio(0.0);
  theScintProcess->SetTrackSecondariesFirst(true);

  theParticleIterator->reset();

  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if(theCerenkovProcess->IsApplicable(*particle)){
      pmanager->AddProcess( theCerenkovProcess);
      pmanager->SetProcessOrdering( theCerenkovProcess, idxPostStep);
    }

    if(theScintProcess->IsApplicable(*particle)){
      pmanager->AddProcess( theScintProcess);
      pmanager->SetProcessOrderingToLast( theScintProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast( theScintProcess, idxPostStep);
    }


    if( particleName == "opticalphoton"){
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess( theAbsorptionProcess);
      pmanager->AddDiscreteProcess( theRayleighScattering);
      pmanager->AddDiscreteProcess( theBoundaryProcess);
      pmanager->AddDiscreteProcess( theWLSProcess);
    }
  
  }
}

void OpticalPhysics::SetScintYieldFactor(G4double yf){
  if(theScintProcess)
    theScintProcess->SetScintillationYieldFactor(yf);
}


