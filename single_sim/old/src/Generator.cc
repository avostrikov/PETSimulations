#include "Generator.hh"
#include "Hist.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"


Generator::Generator(){
  //  G4int n_particle = 1;
  //  particleGun = new G4ParticleGun(n_particle);

  particleGun = new G4GeneralParticleSource ();

  
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  
  G4String particleName;
  particleGun->SetParticleDefinition(particleTable->
				     FindParticle(particleName="gamma"));

  particleGun->SetNumberOfParticles(2); 

  // Default energy,position,momentum
  // particleGun->SetParticleEnergy( 511.*keV);
  // particleGun->SetParticlePosition(G4ThreeVector( 0.0*mm, 0.0*mm, -50.0*mm));
  // particleGun->SetParticleMomentumDirection(G4ThreeVector( 0., 0., 1.));
 
}




Generator::~Generator(){
    delete particleGun;
}



void Generator::GeneratePrimaries( G4Event* anEvent){

  particleGun->GeneratePrimaryVertex(anEvent);

  G4PrimaryParticle* particle0 = anEvent->GetPrimaryVertex(0)->GetPrimary(0);

  if(0){
    G4cout<<"px gamma0 = "<<(particle0->GetMomentum()).x()<<G4endl;
    G4cout<<"py gamma0 = "<<(particle0->GetMomentum()).y()<<G4endl;
    G4cout<<"pz gamma0 = "<<(particle0->GetMomentum()).z()<<G4endl;
  }


 HistManager* hist = HistManager::getInstance();  
 hist->fill_gamma0( anEvent->GetEventID(),  
		    anEvent->GetPrimaryVertex(0)->GetX0(),
		    anEvent->GetPrimaryVertex(0)->GetY0(),
		    anEvent->GetPrimaryVertex(0)->GetZ0());


 if( particleGun->GetNumberOfParticles() == 2){
   G4PrimaryParticle* particle1 = anEvent->GetPrimaryVertex(0)->GetPrimary(1);
     
   if(0){
     G4cout << "Before" << G4endl;
      
     G4cout<<"px gamma1 = "<<(particle1->GetMomentum()).x()<<G4endl;
     G4cout<<"py gamma1 = "<<(particle1->GetMomentum()).y()<<G4endl;
     G4cout<<"pz gamma1 = "<<(particle1->GetMomentum()).z()<<G4endl;
   }

   /*
     G4double dev = CLHEP::RandGauss::shoot(0.,m_accoValue/3.35);
     G4double Phi1     = (twopi * G4UniformRand())/2. ;
     G4ThreeVector DirectionPhoton (sin(dev)*cos(Phi1),sin(dev)*sin(Phi1),cos(dev));
     DirectionPhoton.rotateUz(particle->GetMomentum());
     particle->SetMomentum(-DirectionPhoton.x(),-DirectionPhoton.y(),-DirectionPhoton.z());
  
     //G4cout<<"########### px gamma2 = "<<-DirectionPhoton.x()<<G4endl;
     //G4cout<<"########### py gamma2 = "<<-DirectionPhoton.y()<<G4endl;
     //G4cout<<"########### pz gamma2 = "<<-DirectionPhoton.z()<<G4endl;
     */


   G4ThreeVector gammaMom = -particle1->GetMomentum();
   particle1->SetMomentum(gammaMom.x(),gammaMom.y(),gammaMom.z());

   
   hist->fill_gamma1( anEvent->GetEventID(),  
		      gammaMom.x(),
		      gammaMom.y(),
		      gammaMom.z());
   

   if(0){
     G4cout << "After" << G4endl;
     G4cout<<"px gamma1 = "<<(particle1->GetMomentum()).x()<<G4endl;
     G4cout<<"py gamma1 = "<<(particle1->GetMomentum()).y()<<G4endl;
     G4cout<<"pz gamma1 = "<<(particle1->GetMomentum()).z()<<G4endl;
   }
 }

 // printf("shoot\n");

}

