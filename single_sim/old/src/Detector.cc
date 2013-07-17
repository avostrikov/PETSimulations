#include "Detector.hh"
#include "DetectorMessenger.hh"
#include "Parameterisation.hh"

#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"

#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4RotationMatrix.hh"
#include "G4GeometryManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RegionStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"


Detector::Detector()
{
  exp_hall_log  = 0;
  exp_hall_phys = 0;

  scint_slab_log_up  = 0;
  scint_slab_phys_up = 0;

  scint_slab_log_down  = 0;
  scint_slab_phys_down = 0;

  light_guide_log_up = 0;
  light_guide_phys_up = 0;

  light_guide_log_down = 0;
  light_guide_phys_down = 0;

  pmt_log_up = 0;
  pmt_phys_up = 0;

  pmt_log_down = 0;
  pmt_phys_down = 0;

  scint_sd = 0;
  light_sd = 0;
  pmt_sd = 0;

  hall_x = hall_y = hall_z = 100.0*mm;
  slab_x = 1.0*mm; slab_y = 1.0*mm; slab_z = 5.0*mm;
  guide_x1 = 1.0*mm;  guide_y1 = 1.0*mm; 
  guide_x2 = 1.0*mm;  guide_y2 = 1.0*mm;
  guide_z  = 0.1*mm;
  sipm_x   = 1.0*mm;  sipm_y = 1.0*mm;  sipm_z = 1.0*mm;

  distance = 10.0*mm;
  detectorMessenger = new DetectorMessenger( this);
}



Detector::~Detector()
{
  delete  detectorMessenger;
}


G4VPhysicalVolume* Detector::Construct()
{

  Materials();

  Geometry();

  ConstructSD();

  print_parameter();

  return exp_hall_phys;
}


void Detector::Materials()
{

  G4double  a, z, density;  
  G4int     ncomp, natoms;
  G4String  name, symbol;
  G4double  fmass;
                                           
  G4Element* H = new G4Element( "H", "H", z=1., a=1.00794*g/mole);
  G4Element* C = new G4Element( "C", "C", z=6., a=12.01*g/mole);
  G4Element* N = new G4Element( "N", "N", z=7., a=14.00674*g/mole);
  G4Element* O = new G4Element( "O", "O", z=8., a=16.00*g/mole);
  G4Element* Lu = new G4Element( "Lu", "Lu", z=71., a=174.97*g/mole);
  G4Element* Si = new G4Element( "Si", "Si", z=14., a=28.09*g/mole);


  // Air
  G4Material* Air = new G4Material( "Air", density=1.290*mg/cm3, ncomp=2);
  Air->AddElement( N, fmass=70*perCent);
  Air->AddElement( O, fmass=30*perCent);

  // LSO
  G4Material* LSO = new G4Material( "LSO", density=7.4*g/cm, ncomp=3);
  LSO->AddElement( Si,  natoms=1);
  LSO->AddElement( Lu,  natoms=2);
  LSO->AddElement( O,   natoms=5);

  // Glass
  G4Material* Glass = new G4Material( "Glass", density=1.032*g/cm3, ncomp = 2);
  Glass->AddElement( C, 91.533*perCent);
  Glass->AddElement( H, 8.467*perCent);

  // Silicon
  G4Material* Silicon;
  Silicon = new G4Material( "Silicon", z=14., a=28.09*g/mole, density=2.33*g/cm3);


  // Material Property Tables
  const int numentry = 2;
  G4double air_pp[numentry]            = { 1.84*eV, 4.08*eV };
  G4double air_rindex[numentry]        = { 1.000277, 1.000277};
  G4double air_abslength[numentry]     = { 50.*m, 50.*m};

  G4MaterialPropertiesTable* air_mt = new G4MaterialPropertiesTable();

  air_mt->AddProperty("RINDEX",        air_pp, air_rindex, numentry);
  air_mt->AddProperty("ABSLENGTH",     air_pp, air_abslength, numentry);

  Air->SetMaterialPropertiesTable(air_mt);


  //
  G4double glass_pp[numentry]            = { 1.84*eV, 4.08*eV };
  G4double glass_rindex[numentry]        = { 1.4, 1.4};
  G4double glass_abslength[numentry]     = { 50.*m, 50.*m};
  
  G4MaterialPropertiesTable* glass_mt = new G4MaterialPropertiesTable();

  glass_mt->AddProperty("RINDEX",        glass_pp, glass_rindex, numentry);
  glass_mt->AddProperty("ABSLENGTH",     glass_pp, glass_abslength, numentry);

  Glass->SetMaterialPropertiesTable(glass_mt);

  //
  //  const int numentry1 = 1;
  //  G4double lso_pp1[numentry1]            = { 2.95167*eV };
  //  G4double lso_fastcomponent1[numentry1] = { 1.0};

  G4double lso_pp[numentry]            = { 1.84*eV, 4.08*eV};
  G4double lso_fastcomponent[numentry] = { 0.5, 0.5};
  G4double lso_rindex[numentry]        = { 1.82, 1.82};
  G4double lso_abslength[numentry]     = { 50.*m, 50.*m};

  G4MaterialPropertiesTable* lso_mt = new G4MaterialPropertiesTable();

  lso_mt->AddConstProperty("SCINTILLATIONYIELD", 26000/MeV); 
  lso_mt->AddConstProperty("RESOLUTIONSCALE", 4.41);
  lso_mt->AddConstProperty("FASTTIMECONSTANT", 40.*ns);
  lso_mt->AddConstProperty("YIELDRATIO", 1.0);

  //  lso_mt->AddProperty("FASTCOMPONENT", lso_pp1, lso_fastcomponent1, numentry1); 
  lso_mt->AddProperty("FASTCOMPONENT", lso_pp, lso_fastcomponent, numentry); 
  lso_mt->AddProperty("RINDEX",        lso_pp, lso_rindex, numentry);
  lso_mt->AddProperty("ABSLENGTH",     lso_pp, lso_abslength, numentry);

  LSO->SetMaterialPropertiesTable(lso_mt);

  //

  G4double silicon_pp[numentry]            = { 1.84*eV, 4.08*eV };
  G4double silicon_rindex[numentry]        = { 3.42, 3.42};
  G4double silicon_abslength[numentry]     = { 1.0*mm, 1.0*m};
  
  G4MaterialPropertiesTable* silicon_mt = new G4MaterialPropertiesTable();

  silicon_mt->AddProperty("RINDEX",        silicon_pp, silicon_rindex, numentry);
  silicon_mt->AddProperty("ABSLENGTH",     silicon_pp, silicon_abslength, numentry);

  Silicon->SetMaterialPropertiesTable(glass_mt);

}


void Detector::Geometry()
{

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
  

  // Experimental Hall
  Setup_ExpHall();

  // Crystal Slab
  Setup_ScintSlab();

  // Light-guide
  Setup_Lightguide();

  // SiPM
  Setup_SiPM();

  // Surface
  Setup_Surface();

  // 
  Setup_Visual();
}


// experimental hall
void Detector::Setup_ExpHall(){

  G4Material* Air = G4Material::GetMaterial("Air");

  G4ThreeVector v3( 0, 0, 0);

  G4Box* exp_hall_box = 
    new G4Box( "exp_hall_box", hall_x, hall_y, hall_z);

  exp_hall_log = 
    new G4LogicalVolume( exp_hall_box, Air, "exp_hall_log", 0, 0, 0);

  exp_hall_phys = 
    new G4PVPlacement( 0, v3, "exp_hall", exp_hall_log, 0, false, 0);
}



void Detector::Setup_ScintSlab(){

  // experimental hall
  G4ThreeVector v3( 0, 0, slab_z+distance);

  G4Material* LSO = G4Material::GetMaterial("LSO");

  G4Box* scint_slab_box = 
    new G4Box( "scint_slab_box", slab_x, slab_y, slab_z);

  scint_slab_log_up = 
    new G4LogicalVolume( scint_slab_box, LSO, "scint_slab_log_up", 0, 0, 0);

  scint_slab_phys_up = 
    new G4PVPlacement( 0, v3, "scint_slab_up", scint_slab_log_up, exp_hall_phys, false, 0);

  //
  v3 = G4ThreeVector( 0*mm, 0*mm, -(slab_z+distance));

  scint_slab_log_down = 
    new G4LogicalVolume( scint_slab_box, LSO, "scint_slab_log_down", 0, 0, 0);

  scint_slab_phys_down = 
    new G4PVPlacement( 0, v3, "scint_slab_down", scint_slab_log_down, exp_hall_phys, false, 0);

}




void Detector::Setup_Lightguide(){

  // experimental hall


  G4ThreeVector v3( 0*mm, 0*mm, (distance + 2*slab_z + guide_z));

  G4Material* Glass = G4Material::GetMaterial("Glass");
  
  G4Trd* light_guide_trd = 
    new G4Trd( "light_guide_trd", guide_x1, guide_x2, guide_y1, guide_y2, guide_z);

  light_guide_log_up = 
    new G4LogicalVolume( light_guide_trd, Glass, "light_guide_log_up", 0, 0, 0);

  light_guide_phys_up = 
    new G4PVPlacement( 0, v3, "light_guide_up", light_guide_log_up, exp_hall_phys, false, 0);

  //
  v3 =  G4ThreeVector( 0*mm, 0*mm, -(distance + 2*slab_z + guide_z));

  light_guide_log_down = 
    new G4LogicalVolume( light_guide_trd, Glass, "light_guide_log_down", 0, 0, 0);

  light_guide_phys_down = 
    new G4PVPlacement( 0, v3, "light_guide_down", light_guide_log_down, exp_hall_phys, false, 0);

}


void Detector::Setup_SiPM(){



  G4ThreeVector v3( 0*mm, 0*mm,  (distance + 2*slab_z + 2*guide_z + sipm_z));

  G4Material* Silicon = G4Material::GetMaterial("Silicon");

  G4Box* pmt_box = new G4Box( "pmt_tube", sipm_x, sipm_y, sipm_z);

  pmt_log_up = 
    new G4LogicalVolume( pmt_box, Silicon, "pmt_log_up", 0, 0, 0);

  pmt_phys_up = new G4PVPlacement( 0, v3, "pmt_up", pmt_log_up, exp_hall_phys, false, 0);

  //
  v3 = G4ThreeVector( 0*mm, 0*mm, -(distance + 2*slab_z + 2*guide_z + sipm_z));

  pmt_log_down = 
    new G4LogicalVolume( pmt_box, Silicon, "pmt_log_down", 0, 0, 0);

  pmt_phys_down = new G4PVPlacement( 0, v3, "pmt_up_down", pmt_log_down, exp_hall_phys, false, 0);
  
}



void  Detector::ConstructSD(){  
  G4SDManager* sd_manager = G4SDManager::GetSDMpointer();


  G4String  scintSD_name = "ScintSD"; 

  if( !scint_sd){
    scint_sd = new ScintSD( scintSD_name);
    sd_manager->AddNewDetector( scint_sd);    
  }
  scint_slab_log_up->SetSensitiveDetector( scint_sd);
  scint_slab_log_down->SetSensitiveDetector( scint_sd);



  G4String  lightSD_name = "LightSD"; 

  if( !light_sd){
    light_sd = new LightSD( lightSD_name);
    sd_manager->AddNewDetector( light_sd);    
  }

  light_guide_log_up->SetSensitiveDetector( light_sd);
  light_guide_log_down->SetSensitiveDetector( light_sd);



  G4String  pmtSD_name = "PmtSD"; 

  if( !pmt_sd){
    pmt_sd = new PmtSD( pmtSD_name);
    sd_manager->AddNewDetector( pmt_sd);    
  }

  pmt_log_up->SetSensitiveDetector( pmt_sd);
  pmt_log_down->SetSensitiveDetector( pmt_sd);

}



void Detector::Setup_Surface(){

  // 1. Scintillator / Air
  G4double sigma_alpha = 0.1;
  G4OpticalSurface* OpScintSurface = new G4OpticalSurface("ScintSurface");
  OpScintSurface->SetType( dielectric_dielectric);
  OpScintSurface->SetFinish( groundbackpainted);
  OpScintSurface->SetModel( unified);
  OpScintSurface->SetSigmaAlpha( sigma_alpha);

  const int num = 2;
  G4double pp[num]            = { 1.84*eV, 4.08*eV};
  G4double specularlobe[num]    = { 1., 1.};
  G4double specularspike[num]   = { 0., 0.};
  G4double backscatter[num]     = { 0., 0.};
  G4double rindex[num]          = { 1., 1.};
  G4double reflectivity[num]    = { 0.95, 0.95};
  G4double efficiency[num]      = { 0., 0.};

  G4MaterialPropertiesTable* surface_mt = new G4MaterialPropertiesTable();
  surface_mt->AddProperty("RINDEX", pp, rindex, num);
  surface_mt->AddProperty("SPECULARLOBECONSTANT", pp, specularlobe, num);
  surface_mt->AddProperty("SPECULARSPIKECONSTANT", pp, specularspike, num);
  surface_mt->AddProperty("BACKSCATTERCONSTANT", pp, backscatter, num);
  surface_mt->AddProperty("REFLECTIVITY", pp, reflectivity, num);
  surface_mt->AddProperty("EFFICIENCY", pp, efficiency, num);

  OpScintSurface->SetMaterialPropertiesTable( surface_mt);

  //
  G4LogicalBorderSurface* ScintSurfacebd_up;
  ScintSurfacebd_up = 
    new G4LogicalBorderSurface("ScintSurfacebd_up",
			       scint_slab_phys_up, exp_hall_phys, OpScintSurface);


  G4LogicalBorderSurface* ScintSurfacebd_down;
  ScintSurfacebd_down = 
    new G4LogicalBorderSurface("ScintSurfacebd_down",
			       scint_slab_phys_down, exp_hall_phys, OpScintSurface);



  // 2. Lightguide / Air
  G4LogicalBorderSurface* LightSurfacebd_up;
  
  LightSurfacebd_up = 
    new G4LogicalBorderSurface( "LightSurfacebd_up",
				light_guide_phys_up, exp_hall_phys, OpScintSurface);  

  G4LogicalBorderSurface* LightSurfacebd_down;
  
  LightSurfacebd_down = 
    new G4LogicalBorderSurface( "LightSurfacebd_down",
				light_guide_phys_down, exp_hall_phys, OpScintSurface);  
  

  // 3. SiPM / Air 
  G4LogicalBorderSurface* SiPMSurfacebd_up;

  SiPMSurfacebd_up = 
    new G4LogicalBorderSurface( "SiPMSurfacebd_up",
				pmt_phys_up, exp_hall_phys, OpScintSurface);  

  G4LogicalBorderSurface* SiPMSurfacebd_down;

  SiPMSurfacebd_down = 
    new G4LogicalBorderSurface( "SiPMSurfacebd_down",
				pmt_phys_down, exp_hall_phys, OpScintSurface);  


  // 4. Lightguide / Scintillator
  sigma_alpha = 0.1;
  G4OpticalSurface* OpLightSurface = new G4OpticalSurface("LightSurface");
  OpLightSurface->SetType( dielectric_dielectric);
  OpLightSurface->SetFinish( polished);
  OpLightSurface->SetModel( unified);
  OpLightSurface->SetSigmaAlpha( sigma_alpha);
  
  /*
  const int num2 = 2;
  G4double pp2[num2]            = { 1.84*eV, 4.08*eV };
  G4double specularlobe2[num2]    = { 1., 1.};
  G4double specularspike2[num2]   = { 0., 0.};
  G4double backscatter2[num2]     = { 0., 0.};
  G4double rindex2[num2]          = { 1., 1.};
  G4double reflectivity2[num2]    = { 0.95, 0.95};
  G4double efficiency2[num2]      = { 0., 0.};

  G4MaterialPropertiesTable* light_mt = new G4MaterialPropertiesTable();
  light_mt->AddProperty("RINDEX", pp2, rindex2, num2);
  light_mt->AddProperty("SPECULARLOBECONSTANT", pp2, specularlobe2, num2);
  light_mt->AddProperty("SPECULARSPIKECONSTANT", pp2, specularspike2, num2);
  light_mt->AddProperty("BACKSCATTERCONSTANT", pp2, backscatter2, num2);
  light_mt->AddProperty("REFLECTIVITY", pp2, reflectivity2, num2);
  light_mt->AddProperty("EFFICIENCY", pp2, efficiency2, num2);

  OpLightSurface->SetMaterialPropertiesTable( light_mt);
  */

  G4LogicalBorderSurface* LightSurfacebd2_up;
  
  LightSurfacebd2_up = 
    new G4LogicalBorderSurface( "LightSurfacebd2_up",
				light_guide_phys_up, scint_slab_phys_up, OpLightSurface);  

  G4LogicalBorderSurface* LightSurfacebd2_down;

  LightSurfacebd2_down = 
    new G4LogicalBorderSurface( "LightSurfacebd2_down",
				light_guide_phys_down, scint_slab_phys_down, OpLightSurface);  



  // 5. SiPM / Light-guide
  sigma_alpha = 0.1;
  G4OpticalSurface* OpSiPMSurface = new G4OpticalSurface("SiPMSurface");
  OpSiPMSurface->SetType( dielectric_dielectric);
  OpSiPMSurface->SetFinish( ground);
  OpSiPMSurface->SetModel( unified);
  OpSiPMSurface->SetSigmaAlpha( sigma_alpha);

  const int num3 = 2;
  G4double pp3[num3]            = { 1.84*eV, 4.08*eV };
  G4double specularlobe3[num3]    = { 1., 1.};
  G4double specularspike3[num3]   = { 0., 0.};
  G4double backscatter3[num3]     = { 0., 0.};
  G4double rindex3[num3]          = { 1., 1.};
  G4double reflectivity3[num3]    = { 0., 0.};
  G4double efficiency3[num3]      = { 0., 0.};

  G4MaterialPropertiesTable* pmt_mt = new G4MaterialPropertiesTable();
  pmt_mt->AddProperty("RINDEX", pp3, rindex3, num3);
  pmt_mt->AddProperty("SPECULARLOBECONSTANT", pp3, specularlobe3, num3);
  pmt_mt->AddProperty("SPECULARSPIKECONSTANT", pp3, specularspike3, num3);
  pmt_mt->AddProperty("BACKSCATTERCONSTANT", pp3, backscatter3, num3);
  pmt_mt->AddProperty("REFLECTIVITY", pp3, reflectivity3, num3);
  pmt_mt->AddProperty("EFFICIENCY", pp3, efficiency3, num3);

  OpSiPMSurface->SetMaterialPropertiesTable( pmt_mt);
    
  G4LogicalBorderSurface* SiPMSurfacebd2_up;
  
  SiPMSurfacebd2_up = 
    new G4LogicalBorderSurface( "SiPMSurfacebd2_up",
				pmt_phys_up, light_guide_phys_up, OpSiPMSurface);  

  G4LogicalBorderSurface* SiPMSurfacebd2_down;
  
  SiPMSurfacebd2_down = 
    new G4LogicalBorderSurface( "SiPMSurfacebd2_down",
				pmt_phys_down, light_guide_phys_down, OpSiPMSurface);  
}



void 
Detector::set_hall_x( G4double val)
{ 
  hall_x = val;
}

void 
Detector::set_hall_y( G4double val)
{ 
  hall_y = val;
}

void 
Detector::set_hall_z( G4double val)
{ 
  hall_z = val;
}

void 
Detector::set_slab_x( G4double val)
{ 
  slab_x = val;
}

void 
Detector::set_slab_y( G4double val)
{ 
  slab_y = val;
}

void 
Detector::set_slab_z( G4double val)
{ 
  slab_z = val;
}



void 
Detector::set_guide_x1( G4double val)
{ 
  guide_x1 = val;
}

void 
Detector::set_guide_y1( G4double val)
{ 
  guide_y1 = val;
}


void 
Detector::set_guide_x2( G4double val)
{ 
  guide_x2 = val;
}

void 
Detector::set_guide_y2( G4double val)
{ 
  guide_y2 = val;
}


void 
Detector::set_guide_z( G4double val)
{ 
  guide_z = val;
}


void 
Detector::set_sipm_x( G4double val)
{ 
  sipm_x = val;
}

void 
Detector::set_sipm_y( G4double val)
{ 
  sipm_y = val;
}

void 
Detector::set_sipm_z( G4double val)
{ 
  sipm_z = val;
}


void 
Detector::set_distance( G4double val)
{ 
  distance = val;
}


void 
Detector::update_geometry()
{ 
  G4RunManager::GetRunManager()->DefineWorldVolume( Construct());
}


void 
Detector::print_parameter()
{ 
  printf("--> Detector Dimension\n");
  printf("--> hall_x = %6.2f, hall_y = %6.2f, hall_z = %6.2f\n",  hall_x, hall_y, hall_z);
  printf("--> slab_x = %6.2f, slab_y = %6.2f, slab_z = %6.2f\n",  slab_x, slab_y, slab_z);
  printf("--> guide_x1 = %6.2f, guide_y1 = %6.2f\n",  guide_x1, guide_y1);
  printf("--> guide_x2 = %6.2f, guide_y2 = %6.2f\n",  guide_x2, guide_y2);
  printf("--> guide_z = %6.2f\n",  guide_z);
  printf("--> sipm_x = %6.2f, sipm_y = %6.2f, sipm_z = %6.2f\n",  sipm_x, sipm_y, sipm_z);
  printf("--> distance = %6.2f\n", distance);
}



void Detector::Setup_Visual(){

  //  Colors    
  G4Colour  white   ( 1.0, 1.0, 1.0);
  G4Colour  magenta ( 1.0, 0.0, 1.0); 
  G4Colour  lblue   ( 0.2, 0.5, 0.8);
  
  //  Visualization attributes         
  G4VisAttributes* simpleH2OVisAtt = new G4VisAttributes( lblue);
  simpleH2OVisAtt->SetVisibility( true);
  simpleH2OVisAtt->SetForceWireframe( true);
  //  simpleH2OVisAtt->SetForceSolid( true);
  scint_slab_log_up->SetVisAttributes( simpleH2OVisAtt);
  scint_slab_log_down->SetVisAttributes( simpleH2OVisAtt);

  G4VisAttributes* simpleVacuumWVisAtt = new G4VisAttributes( white);
  simpleVacuumWVisAtt->SetVisibility( true);
  simpleVacuumWVisAtt->SetForceWireframe( true);
  light_guide_log_up->SetVisAttributes( simpleVacuumWVisAtt);
  light_guide_log_down->SetVisAttributes( simpleVacuumWVisAtt);
  

  G4VisAttributes* simpleTungstenSVisAtt = new G4VisAttributes( magenta);
  simpleTungstenSVisAtt->SetVisibility( true);
  simpleTungstenSVisAtt->SetForceWireframe( true);
  //  simpleTungstenSVisAtt->SetForceSolid( true);
  pmt_log_up->SetVisAttributes( simpleTungstenSVisAtt);
  pmt_log_down->SetVisAttributes( simpleTungstenSVisAtt);


  G4VisAttributes* simpleWorldVisAtt= new G4VisAttributes( white);
  simpleWorldVisAtt->SetVisibility( false);
  exp_hall_log->SetVisAttributes( simpleWorldVisAtt);
  
}




