#include "Physics.hh"
#include "PhysicsMessenger.hh"

#include "G4ios.hh"
#include <iomanip>

#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"

#include "G4LossTableManager.hh"
// #include "G4EmSaturation.hh"
 
#include "G4Decay.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4MultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"



Physics::Physics() :  G4VUserPhysicsList()
{
  theCerenkovProcess           = 0;
  theScintillationProcess      = 0;
  theAbsorptionProcess         = 0;
  theRayleighScatteringProcess = 0;
  theBoundaryProcess           = 0;
  theWLSProcess                = 0;

  pMessenger = new PhysicsMessenger(this);  
  SetVerboseLevel(0);
}




Physics::~Physics() { 
  delete pMessenger;
}



// In this method, static member functions should be called
// for all particles which you want to use.
// This ensures that objects of these particle types will be
// created in the program.
void Physics::ConstructParticle()
{
  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
}


// pseudo-particles, gamma & optical photon
void Physics::ConstructBosons()
{
  G4Geantino::GeantinoDefinition();
  G4ChargedGeantino::ChargedGeantinoDefinition();

  G4Gamma::GammaDefinition();

  G4OpticalPhoton::OpticalPhotonDefinition();
}


// leptons
void Physics::ConstructLeptons()
{
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}


//  mesons
void Physics::ConstructMesons()
{
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
}


//  barions
void Physics::ConstructBaryons()
{
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();
}



void Physics::ConstructProcess()
{
  AddTransportation();
  ConstructGeneral();
  ConstructEM();
  //  ConstructOp();
}




// Add Decay Process
void Physics::ConstructGeneral()
{
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();

  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if( theDecayProcess->IsApplicable(*particle)){
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager->SetProcessOrdering( theDecayProcess, idxPostStep);
      pmanager->SetProcessOrdering( theDecayProcess, idxAtRest);
    }
  }
}



void Physics::ConstructEM()
{
  theParticleIterator->reset();

  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
      // Construct processes for gamma
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());
    } 
    // Construct processes for electron
    else if (particleName == "e-") {
      pmanager->AddProcess(new G4MultipleScattering(),-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(),       -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(),   -1, 3, 3);
      
    } 
    else if (particleName == "e+") {
      // Construct processes for positron
      pmanager->AddProcess(new G4MultipleScattering(),-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(),       -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(),   -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation(),  0,-1, 4);      
    } 
    else if( particleName == "mu+" ||
	     particleName == "mu-"    ) {
      // Construct processes for muon
      pmanager->AddProcess(new G4MultipleScattering(),-1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation(),      -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung(),  -1, 3, 3);
      pmanager->AddProcess(new G4MuPairProduction(),  -1, 4, 4);      
    } 
    else {
      if ((particle->GetPDGCharge() != 0.0) &&
          (particle->GetParticleName() != "chargedgeantino")) {
	// all others charged particles except geantino
	pmanager->AddProcess(new G4MultipleScattering(),-1,1,1);
	pmanager->AddProcess(new G4hIonisation(),       -1,2,2);
      }
    }
  }
}



void Physics::ConstructOp()
{
  printf("--- Inside ConstructOP \n");

  theCerenkovProcess           = new G4Cerenkov("Cerenkov");
  theScintillationProcess      = new G4Scintillation("Scintillation");
  theAbsorptionProcess         = new G4OpAbsorption();
  theRayleighScatteringProcess = new G4OpRayleigh();
  theBoundaryProcess           = new G4OpBoundaryProcess();
  theWLSProcess                = new G4OpWLS();

  //  theCerenkovProcess->DumpPhysicsTable();
  //  theScintillationProcess->DumpPhysicsTable();
  //  theAbsorptionProcess->DumpPhysicsTable();
  //  theRayleighScatteringProcess->DumpPhysicsTable();

  SetVerbose(0);
  
  theCerenkovProcess->SetMaxNumPhotonsPerStep(20);
  //  theCerenkovProcess->SetMaxBetaChangePerStep(10.0);
  theCerenkovProcess->SetTrackSecondariesFirst(true);
  
  theScintillationProcess->SetScintillationYieldFactor(1.);
  theScintillationProcess->SetScintillationExcitationRatio(0.0); 
  theScintillationProcess->SetTrackSecondariesFirst(true);

  // Use Birks Correction in the Scintillation process
  //  G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
  //  theScintillationProcess->AddSaturation(emSaturation);

  G4OpticalSurfaceModel themodel = unified;
  theBoundaryProcess->SetModel(themodel);

  theWLSProcess->UseTimeProfile("delta");
  //theWLSProcess->UseTimeProfile("exponential");


  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (theCerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theCerenkovProcess);
      pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
    }

    if (theScintillationProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theScintillationProcess);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
      pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
    }

    if (particleName == "opticalphoton") {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(theAbsorptionProcess);
      pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
      pmanager->AddDiscreteProcess(theBoundaryProcess);
      pmanager->AddDiscreteProcess( theWLSProcess);
    }
  }
}



void Physics::SetVerbose(G4int verbose)
{
  theCerenkovProcess->SetVerboseLevel(verbose);
  theScintillationProcess->SetVerboseLevel(verbose);
  theAbsorptionProcess->SetVerboseLevel(verbose);
  theRayleighScatteringProcess->SetVerboseLevel(verbose);
  theBoundaryProcess->SetVerboseLevel(verbose);  
}



void Physics::SetNbOfPhotonsCerenkov(G4int MaxNumber)
{  
  theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumber);
}


void 
Physics::enable_optical( G4bool flag) 
{  
  flag_optical = flag;
  if( flag_optical) ConstructOp();
}


//  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
//   the default cut value for all particle types
void Physics::SetCuts()
{

  SetCutsWithDefault();
  
  if( verboseLevel>0) 
    DumpCutValuesTable();   
}


