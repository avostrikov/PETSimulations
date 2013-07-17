#ifndef Detector_h
#define Detector_h 1

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VUserDetectorConstruction.hh"

#include "DetectorMessenger.hh"
#include "ScintSD.hh"
#include "LightSD.hh"
#include "PmtSD.hh"

class DetectorMessenger;

class Detector: public  G4VUserDetectorConstruction
{

  public:
    Detector();
   ~Detector();

  public:
    G4VPhysicalVolume* Construct();


  private:
    void Materials();
    void Geometry();
    void ConstructSD();

    void Setup_ExpHall();
    void Setup_Scintillator();
    void Setup_Lightguide();
    void Setup_MCP();
    void Setup_Surface();
    void Setup_Visual();

  public:
    void set_hall_x( G4double);
    void set_hall_y( G4double);
    void set_hall_z( G4double);

    void set_slab_x( G4double);
    void set_slab_y( G4double);
    void set_slab_z( G4double);

    void set_guide_x( G4double);
    void set_guide_y( G4double);
    void set_guide_z( G4double);

    void set_sipm_x( G4double);
    void set_sipm_y( G4double);
    void set_sipm_z( G4double);
    void set_sipm_r( G4double);

    void set_distance( G4double);

    void update_geometry();
    void print_parameter();

  public:
    G4double get_hall_x() { return hall_x;}; 
    G4double get_hall_y() { return hall_y;}; 
    G4double get_hall_z() { return hall_z;}; 

    G4double get_slab_x() { return slab_x;}; 
    G4double get_slab_y() { return slab_y;}; 
    G4double get_slab_z() { return slab_z;}; 

    G4double get_guide_x() { return guide_x;}; 
    G4double get_guide_y() { return guide_y;}; 
    G4double get_guide_z() { return guide_z;}; 

    G4double get_sipm_x() { return sipm_x;}; 
    G4double get_sipm_y() { return sipm_y;}; 
    G4double get_sipm_z() { return sipm_z;}; 

    G4double get_distance() { return distance;}; 

  private:
    G4LogicalVolume*   exp_hall_log;
    G4VPhysicalVolume* exp_hall_phys;

    // up
    G4LogicalVolume*   scint_slab_log_up[5][24][24];
    G4VPhysicalVolume* scint_slab_phys_up[5][24][24];

    G4LogicalVolume*   light_guide_log_up[5][2];
    G4VPhysicalVolume* light_guide_phys_up[5][2];

    G4LogicalVolume*   pmt_log_up[5][2];
    G4VPhysicalVolume* pmt_phys_up[5][2];

    G4LogicalVolume*   glass_log_up[5][2];
    G4VPhysicalVolume* glass_phys_up[5][2];

    G4LogicalVolume*   photocathode_log_up[5][2];
    G4VPhysicalVolume* photocathode_phys_up[5][2];

    G4LogicalVolume*   mcp_log_up[5][2];
    G4VPhysicalVolume* mcp_phys_up[5][2];

    G4LogicalVolume*   tl_log_up[5][2];
    G4VPhysicalVolume* tl_phys_up[5][2];

    G4LogicalVolume*   isolator_log_up[5][2];
    G4VPhysicalVolume* isolator_phys_up[5][2];

    G4LogicalVolume*   ground_log_up[5][2];
    G4VPhysicalVolume* ground_phys_up[5][2];

    G4LogicalVolume*   space0_log_up[5][2];
    G4VPhysicalVolume* space0_phys_up[5][2];

    G4LogicalVolume*   space1_log_up[5][2];
    G4VPhysicalVolume* space1_phys_up[5][2];

    // down
    G4LogicalVolume*   scint_slab_log_down[5][24][24];
    G4VPhysicalVolume* scint_slab_phys_down[5][24][24];

    G4LogicalVolume*   light_guide_log_down[5][2];
    G4VPhysicalVolume* light_guide_phys_down[5][2];

    G4LogicalVolume*   pmt_log_down[5][2];
    G4VPhysicalVolume* pmt_phys_down[5][2];

    G4LogicalVolume*   glass_log_down[5][2];
    G4VPhysicalVolume* glass_phys_down[5][2];

    G4LogicalVolume*   photocathode_log_down[5][2];
    G4VPhysicalVolume* photocathode_phys_down[5][2];


    G4LogicalVolume*   mcp_log_down[5][2];
    G4VPhysicalVolume* mcp_phys_down[5][2];

    G4LogicalVolume*   tl_log_down[5][2];
    G4VPhysicalVolume* tl_phys_down[5][2];

    G4LogicalVolume*   isolator_log_down[5][2];
    G4VPhysicalVolume* isolator_phys_down[5][2];

    G4LogicalVolume*   ground_log_down[5][2];
    G4VPhysicalVolume* ground_phys_down[5][2];

    G4LogicalVolume*   space0_log_down[5][2];
    G4VPhysicalVolume* space0_phys_down[5][2];

    G4LogicalVolume*   space1_log_down[5][2];
    G4VPhysicalVolume* space1_phys_down[5][2];

    //
    ScintSD*           scint_sd;
    LightSD*           light_sd;
    PmtSD*             pmt_sd;

    DetectorMessenger* detectorMessenger;
  
    G4double hall_x, hall_y, hall_z;
    G4double slab_x, slab_y, slab_z;
    G4double guide_x, guide_y, guide_z;
    G4double sipm_x, sipm_y, sipm_z, sipm_r;
    G4double cathode_z, cathode_r;
    G4double glass_z, mcp_z, tl_z, isolator_z, ground_z;
    G4double space0_z, space1_z;

    G4double distance;
    G4double block_width, block_space;

    G4int    nlayer;
};

#endif

