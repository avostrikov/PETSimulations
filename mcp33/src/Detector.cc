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
#include "G4PVDivision.hh"
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

G4double inch = 25.4*mm;

Detector::Detector()
{

  //  G4double inch = 25.4*mm;

  exp_hall_log  = 0;
  exp_hall_phys = 0;

  for( int i = 0; i < 5; i++){

    for( int j = 0; j < 24; j++)
      for( int k = 0; k < 24; k++){
	scint_slab_log_up[i][j][k]  = 0;
	scint_slab_log_down[i][j][k]  = 0;
	
	scint_slab_phys_up[i][j][k] = 0;
	scint_slab_phys_down[i][j][k] = 0;
      }

    for( int j = 0; j < 2; j++){
      pmt_log_up[i][j] = 0;    
      pmt_log_down[i][j] = 0;

      pmt_phys_up[i][j] = 0;
      pmt_phys_down[i][j] = 0;

      glass_log_up[i][j] = 0;    
      glass_log_down[i][j] = 0;

      glass_phys_up[i][j] = 0;
      glass_phys_down[i][j] = 0;

      photocathode_log_up[i][j] = 0;
      photocathode_log_down[i][j] = 0;
      
      photocathode_phys_up[i][j] = 0;
      photocathode_phys_down[i][j] = 0;    

      light_guide_log_up[i][j] = 0;
      light_guide_log_down[i][j] = 0;

      light_guide_phys_up[i][j] = 0;
      light_guide_phys_down[i][j] = 0;

      space0_log_up[i][j] = 0;    
      space0_log_down[i][j] = 0;

      space0_phys_up[i][j] = 0;
      space0_phys_down[i][j] = 0;

      space1_log_up[i][j] = 0;    
      space1_log_down[i][j] = 0;

      space1_phys_up[i][j] = 0;
      space1_phys_down[i][j] = 0;

      mcp_log_up[i][j] = 0;    
      mcp_log_down[i][j] = 0;

      mcp_phys_up[i][j] = 0;
      mcp_phys_down[i][j] = 0;

      tl_log_up[i][j] = 0;    
      tl_log_down[i][j] = 0;

      tl_phys_up[i][j] = 0;
      tl_phys_down[i][j] = 0;

      isolator_log_up[i][j] = 0;    
      isolator_log_down[i][j] = 0;

      isolator_phys_up[i][j] = 0;
      isolator_phys_down[i][j] = 0;

      ground_log_up[i][j] = 0;    
      ground_log_down[i][j] = 0;

      ground_phys_up[i][j] = 0;
      ground_phys_down[i][j] = 0;


    }
  }


  scint_sd = 0;
  light_sd = 0;
  pmt_sd = 0;

  hall_x = hall_y = hall_z = 200.0*mm;


  //  slab_x = 2.0*mm; slab_y = 2.0*mm; slab_z = 12.5*mm;
  slab_x = 2.0*mm; slab_y = 2.0*mm; slab_z = 12.5*mm;


  block_width = 51.00*mm*2.;
  block_space = 0.25*mm;
 
  guide_x  = block_width/2.;  
  guide_y  = block_width/2.;
  guide_z  = 0.001*inch/2.;

  sipm_r   = 12.7*mm;
  sipm_x   = block_width/2.;  
  sipm_y   = block_width/2.;  


  cathode_r  = sipm_r;
  cathode_z  = 0.002*inch/2.;               // photocathode  thickness
  glass_z    = (0.062*inch/2. - cathode_z); // glass  thickness
  space0_z   = 0.040*inch/2.; 
  space1_z   = 0.040*inch/2.; 
  mcp_z      = 0.2*inch/2.;                    // mcp  thickness
  tl_z       = 0.035*mm/2.;                 // trans. line thickness
  isolator_z = 0.400*mm/2.;                 // isolator thickness
  ground_z   = 0.025*mm/2.;                 // ground line thickness

  
  sipm_z =   glass_z + cathode_z 
           + space0_z + mcp_z + space1_z 
           + tl_z + isolator_z + ground_z;


  distance   = 25.0*mm;

  nlayer = 1;

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

  G4double  a, z, density, pressure, temperature;  
  G4int     ncomp, natoms;
  G4String  name, symbol;
  G4double  fmass;
                                           
  G4Element* H  = new G4Element( "H", "H", z=1., a=1.00794*g/mole);
  G4Element* B  = new G4Element( "B", "B", z=5., a=10.811*g/mole);
  G4Element* C  = new G4Element( "C", "C", z=6., a=12.01*g/mole);
  G4Element* N  = new G4Element( "N", "N", z=7., a=14.00674*g/mole);
  G4Element* O  = new G4Element( "O", "O", z=8., a=16.00*g/mole);
  G4Element* Na = new G4Element( "Na", "Na", z=11., a=22.99*g/mole);
  G4Element* Al = new G4Element( "Al", "Al", z=13., a=26.98*g/mole);
  G4Element* Si = new G4Element( "Si", "Si", z=14., a=28.09*g/mole);  
  G4Element* K  = new G4Element( "K", "K", z=19., a=39.098*g/mole);
  G4Element* Lu = new G4Element( "Lu", "Lu", z=71., a=174.97*g/mole);

  // Air
  G4Material* Air = new G4Material( "Air", density=1.290*mg/cm3, ncomp=2);
  Air->AddElement( N, fmass=70*perCent);
  Air->AddElement( O, fmass=30*perCent);

  // Optical Gel
  G4Material* Gel = new G4Material( "Gel", density=1.0*g/cm3, ncomp=3);
  Gel->AddElement(C, natoms=1);
  Gel->AddElement(H, natoms=1);
  Gel->AddElement(O, natoms=1);


  // LSO
  G4Material* LSO = new G4Material( "LSO", density=7.4*g/cm3, ncomp=3);
  LSO->AddElement( Si,  natoms=1);
  LSO->AddElement( Lu,  natoms=2);
  LSO->AddElement( O,   natoms=5);

  // Glass
  G4Material* Glass = new G4Material( "Glass", density=1.032*g/cm3, ncomp = 2);
  Glass->AddElement( C, 91.533*perCent);
  Glass->AddElement( H, 8.467*perCent);

  // Borosilicate Glass
  G4Material* Borosilicate = new G4Material( "Borosilicate", density=2.23*g/cm3, ncomp = 6);
  Borosilicate->AddElement( B,   4.0064*perCent);
  Borosilicate->AddElement( O,  53.9562*perCent);
  Borosilicate->AddElement( Na,  2.8191*perCent);
  Borosilicate->AddElement( Al,  1.1644*perCent);
  Borosilicate->AddElement( Si, 37.7220*perCent);
  Borosilicate->AddElement( K,   0.3321*perCent);

  // Carbon
  G4Material* Carbon;
  Carbon = new G4Material( "Carbon", z=6., a=12.01*g/mole, density=2.3*g/cm3);


  // Silicon
  G4Material* Silicon;
  Silicon = new G4Material( "Silicon", z=14., a=28.09*g/mole, density=2.33*g/cm3);

  // Aluminum
  G4Material* Aluminum;
  Aluminum = new G4Material( "Aluminum", z=13., a=26.98*g/mole, density=2.7*g/cm3);

 
  // Gold
  G4Material* Gold;
  Gold = new G4Material( "Gold", z=79., a=196.96*g/mole, density=19.3*g/cm3);

  // Cu
  G4Material* Copper;
  Copper = new G4Material( "Copper", z=29., a=63.54*g/mole, density=8.96*g/cm3);

  // Alumina
  G4Material* Alumina = new G4Material( "Alumina", density=3.97*g/cm3, ncomp = 2);
  Alumina->AddElement( Al,  natoms=2);
  Alumina->AddElement( O,   natoms=3);

  // Vacuum  
  density     = universe_mean_density;
  pressure    = 3.e-18*pascal;
  temperature = 2.73*kelvin;

  G4Material* Vacuum = new G4Material( "Vacuum", z=1., a=1.01*g/mole,
				       density, kStateGas, temperature, pressure);
 
  // Material Property Tables
  const int numentry = 2;
  G4double air_pp[numentry]            = { 2.275*eV, 3.307*eV };
  G4double air_rindex[numentry]        = { 1.000277, 1.000277};
  G4double air_abslength[numentry]     = { 50.*m, 50.*m};

  G4MaterialPropertiesTable* air_mt = new G4MaterialPropertiesTable();

  air_mt->AddProperty("RINDEX",        air_pp, air_rindex, numentry);
  air_mt->AddProperty("ABSLENGTH",     air_pp, air_abslength, numentry);

  Air->SetMaterialPropertiesTable(air_mt);


  // Glass
  G4double glass_pp[numentry]            = { 2.275*eV, 3.307*eV };
  G4double glass_rindex[numentry]        = { 1.5, 1.5};
  G4double glass_abslength[numentry]     = { 4.2*m, 4.2*m};
  
  G4MaterialPropertiesTable* glass_mt = new G4MaterialPropertiesTable();

  glass_mt->AddProperty("RINDEX",        glass_pp, glass_rindex, numentry);
  glass_mt->AddProperty("ABSLENGTH",     glass_pp, glass_abslength, numentry);

  Glass->SetMaterialPropertiesTable(glass_mt);


  // Borosilicated
  G4double boros_pp[numentry]            = { 2.275*eV, 3.307*eV };
  G4double boros_rindex[numentry]        = { 1.5, 1.5};
  G4double boros_abslength[numentry]     = { 4.2*m, 4.2*m};
  
  G4MaterialPropertiesTable* boros_mt = new G4MaterialPropertiesTable();

  boros_mt->AddProperty("RINDEX",        boros_pp, boros_rindex, numentry);
  boros_mt->AddProperty("ABSLENGTH",     boros_pp, boros_abslength, numentry);

  Borosilicate->SetMaterialPropertiesTable( boros_mt);


  // LSO
  const int numentry2 = 35;
  G4double lso_pp2[numentry2] = { 2.275*eV, 2.296*eV, 2.318*eV, 2.340*eV, 2.362*eV, 
				  2.385*eV, 2.408*eV, 2.431*eV, 2.455*eV, 2.480*eV,
				  2.505*eV, 2.531*eV, 2.557*eV, 2.583*eV, 2.611*eV, 
				  2.638*eV, 2.667*eV, 2.696*eV, 2.725*eV, 2.756*eV,
				  2.787*eV, 2.818*eV, 2.851*eV, 2.884*eV, 2.918*eV,
				  2.952*eV, 2.988*eV, 3.024*eV, 3.062*eV, 3.100*eV,
				  3.139*eV, 3.179*eV, 3.221*eV, 3.263*eV, 3.307*eV};

  G4double lso_fastcomponent[numentry2] = { 0.010, 0.020, 0.030, 0.040, 0.050,
					    0.060, 0.070, 0.080, 0.090, 0.100,
					    0.125, 0.150, 0.175, 0.200, 0.250,
					    0.300, 0.350, 0.400, 0.450, 0.500,
					    0.550, 0.600, 0.650, 0.700, 0.750, 
					    0.800, 0.725, 0.650, 0.575, 0.500, 
					    0.375, 0.250, 0.175, 0.100, 0.050};
  G4double lso_rindex[numentry2];
  G4double lso_abslength[numentry2];


  for( int i = 0; i < numentry2; i++){
    lso_rindex[i] = 1.82;
    lso_abslength[i] = 20.*cm;
  }


  G4MaterialPropertiesTable* lso_mt = new G4MaterialPropertiesTable();

  lso_mt->AddConstProperty("SCINTILLATIONYIELD", 30000/MeV); 
  lso_mt->AddConstProperty("RESOLUTIONSCALE", 4.41);
  lso_mt->AddConstProperty("FASTTIMECONSTANT", 40.*ns);
  lso_mt->AddConstProperty("YIELDRATIO", 1.0);

  lso_mt->AddProperty("FASTCOMPONENT", lso_pp2, lso_fastcomponent, numentry2); 
  lso_mt->AddProperty("RINDEX",        lso_pp2, lso_rindex, numentry2);
  lso_mt->AddProperty("ABSLENGTH",     lso_pp2, lso_abslength, numentry2);

  LSO->SetMaterialPropertiesTable(lso_mt);


  // Silicon
  G4double silicon_pp[numentry]            = { 2.275*eV, 3.307*eV };
  G4double silicon_rindex[numentry]        = { 3.42, 3.42};
  G4double silicon_abslength[numentry]     = { 10.*mm, 10.*mm};
  
  G4MaterialPropertiesTable* silicon_mt = new G4MaterialPropertiesTable();

  silicon_mt->AddProperty("RINDEX",        silicon_pp, silicon_rindex, numentry);
  silicon_mt->AddProperty("ABSLENGTH",     silicon_pp, silicon_abslength, numentry);

  Silicon->SetMaterialPropertiesTable( silicon_mt);

  // Gel
  G4double gel_pp[numentry]            = { 2.275*eV, 3.307*eV };
  G4double gel_rindex[numentry]        = { 1.5, 1.5};
  G4double gel_abslength[numentry]     = { 4.2*m, 4.2*m};
  
  G4MaterialPropertiesTable* gel_mt = new G4MaterialPropertiesTable();

  gel_mt->AddProperty("RINDEX",        gel_pp, gel_rindex, numentry);
  gel_mt->AddProperty("ABSLENGTH",     gel_pp, gel_abslength, numentry);

  Gel->SetMaterialPropertiesTable( gel_mt);



  // Include Bialkali photo-cathode

  
  // Aluminum : temporary photocathode
  G4double aluminum_pp[numentry]            = { 2.275*eV, 3.307*eV };
  G4double aluminum_rindex[numentry]        = { 3.42, 3.42};
  G4double aluminum_abslength[numentry]     = { 0.01*mm, 0.01*mm};
  
  G4MaterialPropertiesTable* aluminum_mt = new G4MaterialPropertiesTable();

  aluminum_mt->AddProperty("RINDEX",        aluminum_pp, aluminum_rindex, numentry);
  aluminum_mt->AddProperty("ABSLENGTH",     aluminum_pp, aluminum_abslength, numentry);

  Aluminum->SetMaterialPropertiesTable( aluminum_mt);

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
  Setup_Scintillator();

  // Light-guide
  Setup_Lightguide();

  // SiPM
  if(1) Setup_MCP();

  // Surface
  if(1) Setup_Surface();

  // 
  if(1) Setup_Visual();
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



void Detector::Setup_Scintillator(){

  char stitle[100];
  double x, y, z;

  // experimental hall
  G4ThreeVector v3( 0, 0, slab_z+distance);

  G4Material* LSO = G4Material::GetMaterial("LSO");

  G4Box* scint_slab_box = 
    new G4Box( "scint_slab_box", slab_x, slab_y, slab_z);

  for( int i = 0; i < nlayer; i++)
    for( int j = 0; j < 24; j++)
      for( int k = 0; k < 24; k++){

	sprintf( stitle, "scint_slab_log_up_%d_%d_%d", i, j, k);
	
	scint_slab_log_up[i][j][k] = 
	  new G4LogicalVolume( scint_slab_box, LSO, stitle, 0, 0, 0);
	
        x = j*(slab_x*2+block_space) - block_width/2.0 + (slab_x + block_space/2.);
        y = k*(slab_y*2+block_space) - block_width/2.0 + (slab_y + block_space/2.);
      
        z = distance + slab_z + 2*guide_z + 2*sipm_z + 2*i*(slab_z+2*guide_z+2*sipm_z);
	
	v3 = G4ThreeVector( x, y, z);
	sprintf( stitle, "scint_slab_up_%d_%d_%d", i, j, k);
	scint_slab_phys_up[i][j][k] = 
	  new G4PVPlacement( 0, v3, stitle, scint_slab_log_up[i][j][k], exp_hall_phys, false, 0);
	
	if(0) printf("scint z(%d) = %9.4f, (%9.4f, %9.4f)\n", i, z, z-slab_z,  z+slab_z);
      }

  for( int i = 0; i < nlayer; i++)
    for( int j = 0; j < 24; j++)
      for( int k = 0; k < 24; k++){

	sprintf( stitle, "scint_slab_log_down_%d_%d_%d", i, j, k);
	
	scint_slab_log_down[i][j][k] = 
	  new G4LogicalVolume( scint_slab_box, LSO, stitle, 0, 0, 0);
	
        x = j*(slab_x*2+block_space) - block_width/2.0 + (slab_x + block_space/2.);
        y = k*(slab_y*2+block_space) - block_width/2.0 + (slab_y + block_space/2.);
      
        z = distance + slab_z + 2*guide_z + 2*sipm_z + 2*i*(slab_z+2*guide_z+2*sipm_z);
	
	z *= -1.;

	v3 = G4ThreeVector( x, y, z);
	sprintf( stitle, "scint_slab_down_%d_%d_%d", i, j, k);
	scint_slab_phys_down[i][j][k] = 
	  new G4PVPlacement( 0, v3, stitle, scint_slab_log_down[i][j][k], exp_hall_phys, false, 0);
	
	if(0) printf("scint z(%d) = %9.4f, (%9.4f, %9.4f)\n", i, z, z-slab_z,  z+slab_z);
      }
  
}



void Detector::Setup_Lightguide(){
  
  // lightguide
  char stitle[100];

  G4ThreeVector v3;

  G4Material* Air = G4Material::GetMaterial("Air");
  G4Material* Gel = G4Material::GetMaterial("Gel");
  
  G4Box* light_guide_box = 
    new G4Box( "light_guide_box", guide_x, guide_y, guide_z);


  for( int i = 0; i < nlayer; i++){
    for( int j = 0; j < 2; j++){
      sprintf( stitle, "light_guide_log_up_%d_%d", i, j);

      light_guide_log_up[i][j] = 
	new G4LogicalVolume( light_guide_box, Gel, stitle, 0, 0, 0);
      //	new G4LogicalVolume( light_guide_box, Air, stitle, 0, 0, 0);
      

      double z =   distance + 2*sipm_z + guide_z + 2*i*(slab_z + 2*guide_z + 2*sipm_z);
      if( j == 1) 
	z =  distance + 2*sipm_z + 2*guide_z+ 2*slab_z + guide_z + 2*i*(slab_z + 2*guide_z + 2*sipm_z);

      v3 = G4ThreeVector( 0., 0., z);
      sprintf( stitle, "light_guide_up_%d_%d", i, j);
      
      light_guide_phys_up[i][j] = 
	new G4PVPlacement( 0, v3, stitle, light_guide_log_up[i][j], exp_hall_phys, false, 0);      

      if(0) printf("light_guide  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z, z-guide_z, z+guide_z);
    }
  }


  for( int i = 0; i < nlayer; i++){
    for( int j = 0; j < 2; j++){
      sprintf( stitle, "light_guide_log_down_%d_%d", i, j);

      light_guide_log_down[i][j] = 
	new G4LogicalVolume( light_guide_box, Gel, stitle, 0, 0, 0);
      //	new G4LogicalVolume( light_guide_box, Air, stitle, 0, 0, 0);
      

      double z =   distance + 2*sipm_z + guide_z + 2*i*(slab_z + 2*guide_z + 2*sipm_z);
      if( j == 1) 
	z =  distance + 2*sipm_z + 2*guide_z+ 2*slab_z + guide_z + 2*i*(slab_z + 2*guide_z + 2*sipm_z);


      z *= -1.;

      v3 = G4ThreeVector( 0., 0., z);
      sprintf( stitle, "light_guide_down_%d_%d", i, j);
      
      light_guide_phys_down[i][j] = 
	new G4PVPlacement( 0, v3, stitle, light_guide_log_down[i][j], exp_hall_phys, false, 0);      

      if(0) printf("light_guide  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z, z-guide_z, z+guide_z);
    }
  }


}



void Detector::Setup_MCP(){

  char stitle[100];

  G4Material* Glass        = G4Material::GetMaterial("Glass");
  G4Material* Silicon      = G4Material::GetMaterial("Silicon");
  G4Material* Aluminum     = G4Material::GetMaterial("Aluminum");
  G4Material* Borosilicate = G4Material::GetMaterial("Borosilicate");
  G4Material* Vacuum       = G4Material::GetMaterial("Vacuum");
  G4Material* Alumina      = G4Material::GetMaterial("Alumina");
  G4Material* Gold         = G4Material::GetMaterial("Gold");
  G4Material* Copper       = G4Material::GetMaterial("Copper");
  G4Material* Carbon       = G4Material::GetMaterial("Carbon");
  
  G4Box* pmt_box       = new G4Box( "pmt_box", sipm_x, sipm_y, sipm_z);
  G4Box* glass_box     = new G4Box( "glass_box", sipm_x, sipm_y, glass_z);
  G4Box* photo_cathode = new G4Box( "photo_cathode", sipm_x, sipm_y, cathode_z);
  G4Box* space0_box    = new G4Box( "space0_box", sipm_x, sipm_y, space0_z);
  G4Box* mcp_box       = new G4Box( "mcp_box", sipm_x, sipm_y, mcp_z);
  G4Box* space1_box    = new G4Box( "space1_box", sipm_x, sipm_y, space1_z);
  G4Box* tl_box        = new G4Box( "tl_box", sipm_x, sipm_y, tl_z);
  G4Box* isolator_box  = new G4Box( "isolator_box", sipm_x, sipm_y, isolator_z);
  G4Box* ground_box    = new G4Box( "ground_box", sipm_x, sipm_y, ground_z);

  G4ThreeVector v3;

  for( int i = 0; i < nlayer; i++){

    for( int j = 0; j < 2; j++){

      sprintf( stitle, "pmt_log_up_%d_%d", i, j);   
      pmt_log_up[i][j] = 
	new G4LogicalVolume( pmt_box, Borosilicate, stitle, 0, 0, 0);

      sprintf( stitle, "glass_log_up_%d_%d", i, j);   
      glass_log_up[i][j] = 
	new G4LogicalVolume( glass_box, Borosilicate, stitle, 0, 0, 0);
    
      sprintf( stitle, "photocathode_log_up_%d_%d", i, j);    
      photocathode_log_up[i][j] = 
	new G4LogicalVolume( photo_cathode, Carbon, stitle, 0, 0, 0);
    
      sprintf( stitle, "space0_log_up_%d_%d", i, j);   
      space0_log_up[i][j] = 
	new G4LogicalVolume( space0_box, Vacuum, stitle, 0, 0, 0);

      sprintf( stitle, "mcp_log_up_%d_%d", i, j);   
      mcp_log_up[i][j] = 
	new G4LogicalVolume( mcp_box, Alumina, stitle, 0, 0, 0);

      sprintf( stitle, "space1_log_up_%d_%d", i, j);   
      space1_log_up[i][j] = 
	new G4LogicalVolume( space1_box, Vacuum, stitle, 0, 0, 0);

      sprintf( stitle, "tl_log_up_%d_%d", i, j);   
      tl_log_up[i][j] = 
	new G4LogicalVolume( tl_box, Gold, stitle, 0, 0, 0);

      sprintf( stitle, "iolator_log_up_%d_%d", i, j);   
      isolator_log_up[i][j] = 
	new G4LogicalVolume( isolator_box, Alumina, stitle, 0, 0, 0);

      sprintf( stitle, "ground_log_up_%d_%d", i, j);   
      ground_log_up[i][j] = 
	new G4LogicalVolume( ground_box, Copper, stitle, 0, 0, 0);


      //
      sprintf( stitle, "pmt_up_%d_%d", i, j);
    
      double z =   distance + sipm_z + 2*i*(slab_z+2*guide_z+2*sipm_z);
      if( j == 1)	
	z =  distance + 2*sipm_z + 2*guide_z+ 2*slab_z + 2*guide_z + sipm_z + 2*i*(slab_z+2*guide_z+2*sipm_z);

      double pmtz = z;

      v3 = G4ThreeVector( 0*mm, 0*mm,  z);
      pmt_phys_up[i][j] = new G4PVPlacement( 0, v3, stitle, pmt_log_up[i][j], exp_hall_phys, false, 0);

      if(0) printf("pmt  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z, z-sipm_z, z+sipm_z);
        
      //
      sprintf( stitle, "glass_%d_%d", i, j);
      z =  sipm_z-glass_z;
      if( j == 1) z = -sipm_z+glass_z;

      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      glass_phys_up[i][j] = new G4PVPlacement( 0, v3, glass_log_up[i][j], stitle, pmt_log_up[i][j], false, 0);      
        
      if(0) printf("glass  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-glass_z, z+pmtz+glass_z);

      //
      sprintf( stitle, "photocathode_%d_%d", i, j);
      z =  sipm_z - cathode_z - 2*glass_z;
      if( j == 1) z = -sipm_z+ cathode_z + 2*glass_z;

      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      photocathode_phys_up[i][j] = new G4PVPlacement( 0, v3, photocathode_log_up[i][j], stitle, pmt_log_up[i][j], false, 0);      

      if(0) printf("photocathode  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-cathode_z, z+pmtz+cathode_z);


      //
      sprintf( stitle, "space0_%d_%d", i, j);
      z =  sipm_z - 2*cathode_z - 2*glass_z - space0_z ;
      if( j == 1) z = -sipm_z+ 2*cathode_z + 2*glass_z + space0_z;

      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      space0_phys_up[i][j] = new G4PVPlacement( 0, v3, space0_log_up[i][j], stitle, pmt_log_up[i][j], false, 0);      

      if(0) printf("space0  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-space0_z, z+pmtz+space0_z);


      //
      sprintf( stitle, "mcp_%d_%d", i, j);
      z =  sipm_z - 2*cathode_z - 2*glass_z - 2*space0_z - mcp_z;
      if( j == 1) z = -sipm_z+ 2*cathode_z + 2*glass_z + 2*space0_z + mcp_z;

      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      mcp_phys_up[i][j] = new G4PVPlacement( 0, v3, mcp_log_up[i][j], stitle, pmt_log_up[i][j], false, 0);      


      if(0) printf("mcp  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-mcp_z, z+pmtz+mcp_z);


      //
      sprintf( stitle, "space1_%d_%d", i, j);
      z =  sipm_z - 2*cathode_z - 2*glass_z - 2*space0_z - 2*mcp_z - space1_z ;
      if( j == 1) z = -sipm_z+ 2*cathode_z + 2*glass_z + 2*space0_z + 2*mcp_z + space1_z;

      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      space1_phys_up[i][j] = new G4PVPlacement( 0, v3, space1_log_up[i][j], stitle, pmt_log_up[i][j], false, 0);      

      if(0)printf("space1  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-space1_z, z+pmtz+space1_z);


      //
      sprintf( stitle, "tl_%d_%d", i, j);
      z =  -sipm_z + 2*ground_z + 2*isolator_z + tl_z;
      if( j == 1) z = sipm_z - 2*ground_z - 2*isolator_z - tl_z;

      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      tl_phys_up[i][j] = new G4PVPlacement( 0, v3, tl_log_up[i][j], stitle, pmt_log_up[i][j], false, 0);      


      if(0)printf("trans. line  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-tl_z, z+pmtz+tl_z);

      //
      sprintf( stitle, "isolator_%d_%d", i, j);
      z =  -sipm_z + 2*ground_z + isolator_z;
      if( j == 1) z = sipm_z - 2*ground_z - isolator_z;

      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      isolator_phys_up[i][j] = new G4PVPlacement( 0, v3, isolator_log_up[i][j], stitle, pmt_log_up[i][j], false, 0);      


      if(0) printf("isolator  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-isolator_z, z+pmtz+isolator_z);

      //
      sprintf( stitle, "ground_%d_%d", i, j);
      z =  -sipm_z + ground_z;
      if( j == 1) z = sipm_z - ground_z;

      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      ground_phys_up[i][j] = new G4PVPlacement( 0, v3, ground_log_up[i][j], stitle, pmt_log_up[i][j], false, 0);      

      if(0) printf("ground  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-ground_z, z+pmtz+ground_z);
    }
  }




  for( int i = 0; i < nlayer; i++){

    for( int j = 0; j < 2; j++){

      sprintf( stitle, "pmt_log_down_%d_%d", i, j);   
      pmt_log_down[i][j] = 
	new G4LogicalVolume( pmt_box, Borosilicate, stitle, 0, 0, 0);

      sprintf( stitle, "glass_log_down_%d_%d", i, j);   
      glass_log_down[i][j] = 
	new G4LogicalVolume( glass_box, Borosilicate, stitle, 0, 0, 0);
    
      sprintf( stitle, "photocathode_log_down_%d_%d", i, j);    
      photocathode_log_down[i][j] = 
	new G4LogicalVolume( photo_cathode, Carbon, stitle, 0, 0, 0);
    
      sprintf( stitle, "space0_log_down_%d_%d", i, j);   
      space0_log_down[i][j] = 
	new G4LogicalVolume( space0_box, Vacuum, stitle, 0, 0, 0);

      sprintf( stitle, "mcp_log_down_%d_%d", i, j);   
      mcp_log_down[i][j] = 
	new G4LogicalVolume( mcp_box, Alumina, stitle, 0, 0, 0);

      sprintf( stitle, "space1_log_down_%d_%d", i, j);   
      space1_log_down[i][j] = 
	new G4LogicalVolume( space1_box, Vacuum, stitle, 0, 0, 0);

      sprintf( stitle, "tl_log_down_%d_%d", i, j);   
      tl_log_down[i][j] = 
	new G4LogicalVolume( tl_box, Gold, stitle, 0, 0, 0);

      sprintf( stitle, "iolator_log_down_%d_%d", i, j);   
      isolator_log_down[i][j] = 
	new G4LogicalVolume( isolator_box, Alumina, stitle, 0, 0, 0);

      sprintf( stitle, "ground_log_down_%d_%d", i, j);   
      ground_log_down[i][j] = 
	new G4LogicalVolume( ground_box, Copper, stitle, 0, 0, 0);


      //
      sprintf( stitle, "pmt_down_%d_%d", i, j);
    
      double z =   distance + sipm_z + 2*i*(slab_z+2*guide_z+2*sipm_z);
      if( j == 1)	
	z =  distance + 2*sipm_z + 2*guide_z+ 2*slab_z + 2*guide_z + sipm_z + 2*i*(slab_z+2*guide_z+2*sipm_z);
      z *= -1;

      double pmtz = z;

      v3 = G4ThreeVector( 0*mm, 0*mm,  z);
      pmt_phys_down[i][j] = new G4PVPlacement( 0, v3, stitle, pmt_log_down[i][j], exp_hall_phys, false, 0);

      if(0) printf("pmt  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z, z-sipm_z, z+sipm_z);
        
      //
      sprintf( stitle, "glass_%d_%d", i, j);
      z =  sipm_z-glass_z;
      if( j == 1) z = -sipm_z+glass_z;
      z *= -1;

      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      glass_phys_down[i][j] = new G4PVPlacement( 0, v3, glass_log_down[i][j], stitle, pmt_log_down[i][j], false, 0);      
        
      if(0) printf("glass  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-glass_z, z+pmtz+glass_z);

      //
      sprintf( stitle, "photocathode_%d_%d", i, j);
      z =  sipm_z - cathode_z - 2*glass_z;
      if( j == 1) z = -sipm_z+ cathode_z + 2*glass_z;
      z *= -1;


      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      photocathode_phys_down[i][j] = new G4PVPlacement( 0, v3, photocathode_log_down[i][j], stitle, pmt_log_down[i][j], false, 0);      

      if(0) printf("photocathode  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-cathode_z, z+pmtz+cathode_z);


      //
      sprintf( stitle, "space0_%d_%d", i, j);
      z =  sipm_z - 2*cathode_z - 2*glass_z - space0_z ;
      if( j == 1) z = -sipm_z+ 2*cathode_z + 2*glass_z + space0_z;
      z *= -1;


      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      space0_phys_down[i][j] = new G4PVPlacement( 0, v3, space0_log_down[i][j], stitle, pmt_log_down[i][j], false, 0);      

      if(0) printf("space0  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-space0_z, z+pmtz+space0_z);


      //
      sprintf( stitle, "mcp_%d_%d", i, j);
      z =  sipm_z - 2*cathode_z - 2*glass_z - 2*space0_z - mcp_z;
      if( j == 1) z = -sipm_z+ 2*cathode_z + 2*glass_z + 2*space0_z + mcp_z;
      z *= -1;

      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      mcp_phys_down[i][j] = new G4PVPlacement( 0, v3, mcp_log_down[i][j], stitle, pmt_log_down[i][j], false, 0);      


      if(0) printf("mcp  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-mcp_z, z+pmtz+mcp_z);


      //
      sprintf( stitle, "space1_%d_%d", i, j);
      z =  sipm_z - 2*cathode_z - 2*glass_z - 2*space0_z - 2*mcp_z - space1_z ;
      if( j == 1) z = -sipm_z+ 2*cathode_z + 2*glass_z + 2*space0_z + 2*mcp_z + space1_z;
      z *= -1;

      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      space1_phys_down[i][j] = new G4PVPlacement( 0, v3, space1_log_down[i][j], stitle, pmt_log_down[i][j], false, 0);      

      if(0)printf("space1  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-space1_z, z+pmtz+space1_z);


      //
      sprintf( stitle, "tl_%d_%d", i, j);
      z =  -sipm_z + 2*ground_z + 2*isolator_z + tl_z;
      if( j == 1) z = sipm_z - 2*ground_z - 2*isolator_z - tl_z;
      z *= -1;


      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      tl_phys_down[i][j] = new G4PVPlacement( 0, v3, tl_log_down[i][j], stitle, pmt_log_down[i][j], false, 0);      


      if(0)printf("trans. line  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-tl_z, z+pmtz+tl_z);

      //
      sprintf( stitle, "isolator_%d_%d", i, j);
      z =  -sipm_z + 2*ground_z + isolator_z;
      if( j == 1) z = sipm_z - 2*ground_z - isolator_z;
      z *= -1;


      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      isolator_phys_down[i][j] = new G4PVPlacement( 0, v3, isolator_log_down[i][j], stitle, pmt_log_down[i][j], false, 0);      


      if(0) printf("isolator  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-isolator_z, z+pmtz+isolator_z);

      //
      sprintf( stitle, "ground_%d_%d", i, j);
      z =  -sipm_z + ground_z;
      if( j == 1) z = sipm_z - ground_z;
      z *= -1;

      v3 = G4ThreeVector( 0*mm, 0*mm, z);
      ground_phys_down[i][j] = new G4PVPlacement( 0, v3, ground_log_down[i][j], stitle, pmt_log_down[i][j], false, 0);      

      if(0) printf("ground  z(%d, %d) = %9.4f, (%9.4f, %9.4f)\n", i, j, z+pmtz, z+pmtz-ground_z, z+pmtz+ground_z);
    }
  }





}



void  Detector::ConstructSD(){  
  G4SDManager* sd_manager = G4SDManager::GetSDMpointer();


  G4String  scintSD_name = "ScintSD"; 

  if( !scint_sd){
    scint_sd = new ScintSD( scintSD_name);
    sd_manager->AddNewDetector( scint_sd);    
  }
  for( int i = 0; i < nlayer; i++)
    for( int j = 0; j < 24; j++)
      for( int k = 0; k < 24; k++){
	scint_slab_log_up[i][j][k]->SetSensitiveDetector( scint_sd);
	scint_slab_log_down[i][j][k]->SetSensitiveDetector( scint_sd);
      }


  //
  
  G4String  lightSD_name = "LightSD"; 

  if( !light_sd){
    light_sd = new LightSD( lightSD_name);
    sd_manager->AddNewDetector( light_sd);    
  }

  for( int i = 0; i < nlayer; i++)
    for( int j = 0; j < 2; j++){
      glass_log_up[i][j]->SetSensitiveDetector( light_sd);
      glass_log_down[i][j]->SetSensitiveDetector( light_sd);
    }

  //
  G4String  pmtSD_name = "PmtSD"; 
  
  if( !pmt_sd){
    pmt_sd = new PmtSD( pmtSD_name);
    sd_manager->AddNewDetector( pmt_sd);    
  }

  for( int i = 0; i < nlayer; i++)
    for( int j = 0; j < 2; j++){
      photocathode_log_up[i][j]->SetSensitiveDetector( pmt_sd);
      photocathode_log_down[i][j]->SetSensitiveDetector( pmt_sd);
    }
}



void Detector::Setup_Surface(){

  char stitle[100];

  G4double sigma_alpha = 6.0*deg;
  const int num        = 2;
  const int num2       = 35;
  G4double pp[num]     = { 2.275*eV, 3.307*eV};
  
  G4double pp2[num2]   = { 2.275*eV, 2.296*eV, 2.318*eV, 2.340*eV, 2.362*eV, 
			   2.385*eV, 2.408*eV, 2.431*eV, 2.455*eV, 2.480*eV,
			   2.505*eV, 2.531*eV, 2.557*eV, 2.583*eV, 2.611*eV, 
			   2.638*eV, 2.667*eV, 2.696*eV, 2.725*eV, 2.756*eV,
			   2.787*eV, 2.818*eV, 2.851*eV, 2.884*eV, 2.918*eV,
			   2.952*eV, 2.988*eV, 3.024*eV, 3.062*eV, 3.100*eV,
			   3.139*eV, 3.179*eV, 3.221*eV, 3.263*eV, 3.307*eV};

  // 1. Scintillator / Air   -> Teflon Wrapping
  if(1){  

    sigma_alpha = 6.0*deg;
    G4OpticalSurface* OpScintSurface = new G4OpticalSurface("ScintSurface");
    OpScintSurface->SetType( dielectric_dielectric);
    OpScintSurface->SetFinish( groundbackpainted);
    //OpScintSurface->SetType( dielectric_metal);
    //OpScintSurface->SetFinish( ground);
    OpScintSurface->SetModel( unified);
    OpScintSurface->SetSigmaAlpha( sigma_alpha);

    G4double rindex[num2];
    G4double specularlobe[num2];
    G4double specularspike[num2];
    G4double backscatter[num2]; 
    G4double reflectivity[num2];
    G4double efficiency[num2];  
  
    for( int i = 0; i < num2; i++){
      rindex[i] = 1.;
      specularlobe[i] = 1.;
      specularspike[i] = 0.;
      backscatter[i] = 0.;
      reflectivity[i] = 0.95;   // diffusion reflection
      efficiency[i] = 0.;
    }


    G4MaterialPropertiesTable* surface_mt = new G4MaterialPropertiesTable();
    surface_mt->AddProperty("RINDEX", pp2, rindex, num2);
    surface_mt->AddProperty("SPECULARLOBECONSTANT", pp2, specularlobe, num2);
    surface_mt->AddProperty("SPECULARSPIKECONSTANT", pp2, specularspike, num2);
    surface_mt->AddProperty("BACKSCATTERCONSTANT", pp2, backscatter, num2);
    surface_mt->AddProperty("REFLECTIVITY", pp2, reflectivity, num2);
    surface_mt->AddProperty("EFFICIENCY", pp2, efficiency, num2);
  
    OpScintSurface->SetMaterialPropertiesTable( surface_mt);
  
 
    for( int i = 0; i < nlayer; i++)
      for( int j = 0; j < 24; j++)
	for( int k = 0; k < 24; k++){
	  sprintf( stitle, "ScintSurfacedbd_up_%d_%d_%d", i, j, k);
	  new G4LogicalBorderSurface( stitle,				 
				      scint_slab_phys_up[i][j][k], exp_hall_phys, OpScintSurface);

	  sprintf( stitle, "ScintSurfacedbd_down_%d_%d_%d", i, j, k);
	  new G4LogicalBorderSurface( stitle,				 
				      scint_slab_phys_down[i][j][k], exp_hall_phys, OpScintSurface);
	}      

  }



  
  //  2. Scintillator / Lightguide(Air) -> ( Grounded Crystal) ->Polished
  if(1){
    sigma_alpha = 0.0*deg;
    G4OpticalSurface* OpScintSurface4 = new G4OpticalSurface("ScintSurface4");
    OpScintSurface4->SetType( dielectric_dielectric);
    OpScintSurface4->SetFinish( polished);
    OpScintSurface4->SetModel( glisur);

    //    OpScintSurface4->SetFinish( ground);
    //    OpScintSurface4->SetModel( unified);
    //    OpScintSurface4->SetSigmaAlpha( sigma_alpha);
    
    G4double rindex4[num2];
    G4double specularlobe4[num2];
    G4double specularspike4[num2];
    G4double backscatter4[num2]; 
    G4double reflectivity4[num2];
    G4double efficiency4[num2];  
    
    for( int i = 0; i < num2; i++){
      rindex4[i] = 1.;
      specularlobe4[i] = 1.;
      specularspike4[i] = 0.;
      backscatter4[i] = 0.;
      reflectivity4[i] = 0.0;
      efficiency4[i] = 0.;
    }
    
    
    G4MaterialPropertiesTable* surface_mt4 = new G4MaterialPropertiesTable();
    //    surface_mt4->AddProperty("RINDEX", pp2, rindex4, num2);
    surface_mt4->AddProperty("SPECULARLOBECONSTANT", pp2, specularlobe4, num2);
    surface_mt4->AddProperty("SPECULARSPIKECONSTANT", pp2, specularspike4, num2);
    surface_mt4->AddProperty("BACKSCATTERCONSTANT", pp2, backscatter4, num2);
    // surface_mt4->AddProperty("REFLECTIVITY", pp2, reflectivity4, num2);
    // surface_mt4->AddProperty("EFFICIENCY", pp2, efficiency4, num2);
    
    //    OpScintSurface4->SetMaterialPropertiesTable( surface_mt4);
    
    
    for( int i = 0; i<nlayer; i++)
      for( int j = 0; j<24; j++)
	for( int k = 0; k<24; k++)
	  for( int l = 0; l<2; l++){
	    sprintf( stitle, "ScintSurfacedbd_up_%d_%d_%d_%d", i, j, k, l);
	    new G4LogicalBorderSurface( stitle,				 
					scint_slab_phys_up[i][j][k], light_guide_phys_up[i][l], OpScintSurface4);	 

	    sprintf( stitle, "ScintSurfacedbd_down_%d_%d_%d_%d", i, j, k, l);
	    new G4LogicalBorderSurface( stitle,				 
					scint_slab_phys_down[i][j][k], light_guide_phys_down[i][l], OpScintSurface4);	 
	  }    
        
  }

  
  // 5.  Lightguide(Air) / PMT Glass(Window) -> ( Polished PMT Glass)
  if(1){

    G4double reflectivity0[num2];
    G4double efficiency0[num2];  
  
    for( int i = 0; i < num2; i++){
      reflectivity0[i] = 0;
      efficiency0[i]  = 0;
    }

    G4OpticalSurface* ScintPMT_surface = new  G4OpticalSurface("ScintPMT_surface");
    
    ScintPMT_surface->SetType( dielectric_dielectric);
    ScintPMT_surface->SetFinish( polished);
    ScintPMT_surface->SetModel( glisur);

    for( int i = 0; i < nlayer; i++)
      for( int j = 0; j < 2; j++){
	sprintf( stitle, "LightSurfacebd_up_%d_%d", i, j);		
	new G4LogicalBorderSurface( stitle,
				    light_guide_phys_up[i][j], glass_phys_up[i][j], ScintPMT_surface);       

	sprintf( stitle, "LightSurfacebd_down_%d_%d", i, j);		
	new G4LogicalBorderSurface( stitle,
				    light_guide_phys_down[i][j], glass_phys_down[i][j], ScintPMT_surface);       
      }
  }
  


  
  // 6. PMT Glass / Air (Side) -> ( Black)
  if(1){
    sigma_alpha = 0.0*deg;

    G4OpticalSurface* OpPMTSurface = new G4OpticalSurface("PMTSurface");
    // OpPMTSurface->SetType(dielectric_dielectric);
    // OpPMTSurface->SetFinish(groundbackpainted);
    OpPMTSurface->SetType(dielectric_metal);
    OpPMTSurface->SetFinish(ground);
    OpPMTSurface->SetModel(unified);
    OpPMTSurface->SetSigmaAlpha( sigma_alpha);

    //    G4double rindex1[num]          = { 1., 1.};
    G4double specularlobe1[num]    = { 1., 1.};
    G4double specularspike1[num]   = { 0., 0.};
    G4double backscatter1[num]     = { 0., 0.};
    G4double reflectivity1[num]    = { 0., 0.};
    G4double efficiency1[num]      = { 0., 0.};    

    G4MaterialPropertiesTable* sipm_mt = new G4MaterialPropertiesTable();
    //    sipm_mt->AddProperty("RINDEX", pp, rindex1, num);
    sipm_mt->AddProperty("SPECULARLOBECONSTANT",  pp, specularlobe1, num);
    sipm_mt->AddProperty("SPECULARSPIKECONSTANT", pp, specularspike1, num);
    sipm_mt->AddProperty("BACKSCATTERCONSTANT",   pp, backscatter1, num);
 
    sipm_mt->AddProperty("EFFICIENCY",   pp, efficiency1, num);
    sipm_mt->AddProperty("REFLECTIVITY", pp, reflectivity1, num);
  
    OpPMTSurface->SetMaterialPropertiesTable( sipm_mt);


    for( int i = 0; i < nlayer; i++)
      for( int j = 0; j < 2; j++){
	sprintf( stitle, "SiPMSurfacebd_up_%d_%d", i, j);      
	new G4LogicalBorderSurface( stitle,
				    pmt_phys_up[i][j], exp_hall_phys, OpPMTSurface);  

	sprintf( stitle, "SiPMSurfacebd2_up_%d_%d", i, j);      
	new G4LogicalBorderSurface( stitle,
				    glass_phys_up[i][j], exp_hall_phys, OpPMTSurface);  

	sprintf( stitle, "SiPMSurfacebd_down_%d_%d", i, j);      
	new G4LogicalBorderSurface( stitle,
				    pmt_phys_down[i][j], exp_hall_phys, OpPMTSurface);  

	sprintf( stitle, "SiPMSurfacebd2_down_%d_%d", i, j);      
	new G4LogicalBorderSurface( stitle,
				    glass_phys_down[i][j], exp_hall_phys, OpPMTSurface);  
      }
  }


  
  // 7. PMT Glass / Photocathode -> ( Detected)
  if(1){
    sigma_alpha = 0.0*deg;
    G4OpticalSurface* OpSiPMSurface = new G4OpticalSurface("SiPMSurface");
    OpSiPMSurface->SetType( dielectric_metal);
    OpSiPMSurface->SetFinish( ground);
    OpSiPMSurface->SetModel( unified);
    OpSiPMSurface->SetSigmaAlpha( sigma_alpha);

    const int num3 = 2;
    G4double pp3[num3]            = { 2.275*eV, 3.307*eV };
    G4double specularlobe3[num3]    = { 1., 1.};
    G4double specularspike3[num3]   = { 0., 0.};
    G4double backscatter3[num3]     = { 0., 0.};
    G4double reflectivity3[num3]    = { 0., 0.};
    G4double efficiency3[num3]      = { 1., 1.};

    G4MaterialPropertiesTable* photocathode_mt = new G4MaterialPropertiesTable();
    photocathode_mt->AddProperty("SPECULARLOBECONSTANT", pp3, specularlobe3, num3);
    photocathode_mt->AddProperty("SPECULARSPIKECONSTANT", pp3, specularspike3, num3);
    photocathode_mt->AddProperty("BACKSCATTERCONSTANT", pp3, backscatter3, num3);
    photocathode_mt->AddProperty("REFLECTIVITY", pp3, reflectivity3, num3);
    photocathode_mt->AddProperty("EFFICIENCY", pp3, efficiency3, num3);

    OpSiPMSurface->SetMaterialPropertiesTable( photocathode_mt);
    

    for( int i = 0; i < nlayer; i++)
      for( int j = 0; j < 2; j++){
	sprintf( stitle, "SiPMSurfacebd2_up_%d_%d", i, j);
	new G4LogicalBorderSurface( stitle,
				    glass_phys_up[i][j], photocathode_phys_up[i][j], OpSiPMSurface);  

	sprintf( stitle, "SiPMSurfacebd2_down_%d_%d", i, j);
	new G4LogicalBorderSurface( stitle,
				    glass_phys_down[i][j], photocathode_phys_down[i][j], OpSiPMSurface);  
      }
  }


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
Detector::set_guide_x( G4double val)
{ 
  guide_x = val;
}

void 
Detector::set_guide_y( G4double val)
{ 
  guide_y = val;
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
Detector::set_sipm_r( G4double val)
{ 
  sipm_r = val;
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
  //  printf("--> guide_x1 = %6.2f, guide_y1 = %6.2f\n",  guide_x1, guide_y1);
  //  printf("--> guide_x2 = %6.2f, guide_y2 = %6.2f\n",  guide_x2, guide_y2);
  //  printf("--> guide_z = %6.2f\n",  guide_z);
  printf("--> sipm_x = %6.2f, sipm_y = %6.2f, sipm_z = %6.2f, %9.4f\n",  sipm_x, sipm_y, sipm_z, sipm_z*2);
  printf("--> distance = %6.2f\n", distance);
}



void Detector::Setup_Visual(){

  //  Colors    
  G4Colour  white   ( 1.0, 1.0, 1.0);
  G4Colour  magenta ( 1.0, 0.0, 1.0); 
  G4Colour  lblue   ( 0.2, 0.5, 0.8);
  G4Colour  red     ( 1.0, 0.0, 0.0);
  G4Colour  green     ( 0.0, 1.0, 0.0);

  //  Visualization attributes         
  G4VisAttributes* simpleH2OVisAtt = new G4VisAttributes( lblue);
  simpleH2OVisAtt->SetVisibility( true);
  simpleH2OVisAtt->SetForceWireframe( true);
  for( int i = 0; i < nlayer; i++)
    for( int j = 0; j < 24; j++)
      for( int k = 0; k < 24; k++){
	scint_slab_log_up[i][j][k]->SetVisAttributes( simpleH2OVisAtt);
	scint_slab_log_down[i][j][k]->SetVisAttributes( simpleH2OVisAtt);
      }


  
  G4VisAttributes* lightguide_vis = new G4VisAttributes( green);
  lightguide_vis->SetVisibility( true);
  lightguide_vis->SetForceWireframe( true);
  for( int i = 0; i < nlayer; i++)  
    for( int j = 0; j < 2; j++){
      light_guide_log_up[i][j]->SetVisAttributes( lightguide_vis);    
      light_guide_log_down[i][j]->SetVisAttributes( lightguide_vis);    
    }

  G4VisAttributes* photocathode_vis = new G4VisAttributes( red);
  photocathode_vis->SetVisibility( true);
  photocathode_vis->SetForceWireframe( true);
  for( int i = 0; i < nlayer; i++)  
    for( int j = 0; j < 2; j++){
      photocathode_log_up[i][j]->SetVisAttributes( photocathode_vis);    
      photocathode_log_down[i][j]->SetVisAttributes( photocathode_vis);    
    }


  G4VisAttributes* simpleTungstenSVisAtt = new G4VisAttributes( magenta);
  simpleTungstenSVisAtt->SetVisibility( true);
  simpleTungstenSVisAtt->SetForceWireframe( true);
  for( int i = 0; i < nlayer; i++)  
    for( int j = 0; j < 2; j++){
      pmt_log_up[i][j]->SetVisAttributes( simpleTungstenSVisAtt);
      pmt_log_down[i][j]->SetVisAttributes( simpleTungstenSVisAtt);
    }

  G4VisAttributes* simpleWorldVisAtt= new G4VisAttributes( white);
  simpleWorldVisAtt->SetVisibility( false);
  exp_hall_log->SetVisAttributes( simpleWorldVisAtt);
  

}




