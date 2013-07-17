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
    void Setup_ScintSlab();
    void Setup_Lightguide();
    void Setup_SiPM();
    void Setup_Surface();
    void Setup_Visual();

  public:
    void set_hall_x( G4double);
    void set_hall_y( G4double);
    void set_hall_z( G4double);

    void set_slab_x( G4double);
    void set_slab_y( G4double);
    void set_slab_z( G4double);

    void set_guide_x1( G4double);
    void set_guide_y1( G4double);
    void set_guide_x2( G4double);
    void set_guide_y2( G4double);
    void set_guide_z( G4double);

    void set_sipm_x( G4double);
    void set_sipm_y( G4double);
    void set_sipm_z( G4double);

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

    G4double get_guide_x1() { return guide_x1;}; 
    G4double get_guide_y1() { return guide_y1;}; 
    G4double get_guide_x2() { return guide_x2;}; 
    G4double get_guide_y2() { return guide_y2;}; 
    G4double get_guide_z() { return guide_z;}; 

    G4double get_sipm_x() { return sipm_x;}; 
    G4double get_sipm_y() { return sipm_y;}; 
    G4double get_sipm_z() { return sipm_z;}; 

    G4double get_distance() { return distance;}; 

  private:
    G4LogicalVolume*   exp_hall_log;
    G4VPhysicalVolume* exp_hall_phys;

    G4LogicalVolume*   scint_slab_log_up;
    G4VPhysicalVolume* scint_slab_phys_up;

    G4LogicalVolume*   light_guide_log_up;
    G4VPhysicalVolume* light_guide_phys_up;

    G4LogicalVolume*   pmt_log_up;
    G4VPhysicalVolume* pmt_phys_up;
  
    G4LogicalVolume*   scint_slab_log_down;
    G4VPhysicalVolume* scint_slab_phys_down;

    G4LogicalVolume*   light_guide_log_down;
    G4VPhysicalVolume* light_guide_phys_down;

    G4LogicalVolume*   pmt_log_down;
    G4VPhysicalVolume* pmt_phys_down;

    ScintSD*           scint_sd;
    LightSD*           light_sd;
    PmtSD*             pmt_sd;

    DetectorMessenger* detectorMessenger;
  
    G4double hall_x, hall_y, hall_z;
    G4double slab_x, slab_y, slab_z;
    G4double guide_x1, guide_x2, guide_y1, guide_y2, guide_z;
    G4double sipm_x, sipm_y, sipm_z;
   
    G4double distance;

};

#endif

