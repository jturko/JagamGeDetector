#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
//#include "SensitiveDetector.hh"

#include "G4Material.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4AssemblyVolume.hh"
#include "G4SubtractionSolid.hh"

//#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "DetectionSystemGermaniumCylinder.hh"

DetectionSystemGermaniumCylinder::DetectionSystemGermaniumCylinder() :
    // LogicalVolumes 
    detector_volume_log(0),    
    crystal_block_log(0),
    can_cylinder_log(0),
    can_front_lid_log(0),
    can_back_lid_log(0),
    packing_cylinder_log(0),
    packing_front_lid_log(0),
    copper_cylinder_log(0),
    core_log(0)
{ 

    inch2cm = 2.54;

    G4double sixteenthInch = 0.15875*cm;
    G4double fifthInch = 0.508*cm;

  this->detail_view_end_angle	 	= 360.0*deg;     
  this->crystal_material            = "G4_Ge";
  this->can_material                = "G4_Al";
  this->packing_material            = "G4_Galactic";
  this->copper_material             = "G4_Cu";

  double length = 0.0419*cm;
  
  //this->crystal_length_z            = 1.5*inch2cm*cm;
  this->crystal_length_z            = length;
  this->crystal_inner_radius 		= 0.25*inch2cm*cm;
  this->crystal_outer_radius 		= 0.75*inch2cm*cm;

  this->crystal_core_length_z          = length;
  //this->crystal_core_length_z          = (1.5)*inch2cm*cm;
  this->crystal_core_inner_radius 		= 0.0*cm;
  this->crystal_core_outer_radius 		= 0.25*inch2cm*cm;



  this->can_outer_radius            = 1.5*inch2cm*cm;
  this->can_inner_radius            = this->can_outer_radius - sixteenthInch;


  this->can_lid_inner_radius        = 0.0*cm;
  this->can_lid_outer_radius        = this->can_outer_radius;
  this->can_front_lid_thickness     = sixteenthInch;
  this->can_back_lid_thickness      = sixteenthInch;

  this->can_length_z                = (3.0*inch2cm*cm) - this->can_front_lid_thickness - this->can_back_lid_thickness;


  this->copper_length_z             = this->crystal_length_z;
  this->copper_inner_radius 		= this->crystal_outer_radius;
  this->copper_outer_radius 		= this->copper_inner_radius + sixteenthInch;

  this->packing_length_z            = this->crystal_length_z;
  this->packing_inner_radius 		= this->copper_outer_radius;
  this->packing_outer_radius 		= this->can_inner_radius;


  this->packing_lid_inner_radius    = 0.0*cm;
  this->packing_lid_outer_radius 	= this->packing_outer_radius;
  this->packing_front_lid_thickness	= fifthInch - can_front_lid_thickness;
  this->packing_back_lid_thickness	= this->can_length_z  - (
                                        this->packing_front_lid_thickness +
                                        this->crystal_length_z);

  this->detector_length_z           = this->crystal_length_z +
                                      this->packing_front_lid_thickness +
                                      this->packing_back_lid_thickness +
                                      this->can_front_lid_thickness +
                                      this->can_back_lid_thickness;



}

DetectionSystemGermaniumCylinder::~DetectionSystemGermaniumCylinder()
{
    // LogicalVolumes 
    delete detector_volume_log;    
    delete crystal_block_log;
    delete can_cylinder_log;
    delete can_front_lid_log;
    delete can_back_lid_log;
    delete packing_cylinder_log;
    delete packing_front_lid_log;
    delete copper_cylinder_log;
    delete core_log;

//    delete crystal_block_SD;
}

//G4int DetectionSystemGermaniumCylinder::Build(G4SDManager* mySDman)
G4int DetectionSystemGermaniumCylinder::Build()
{ 
  // Build assembly volume
  G4AssemblyVolume* myAssembly = new G4AssemblyVolume();
  this->assembly = myAssembly;

  G4cout << "BuildCrystalVolume" << G4endl;
  BuildCrystalVolume();      
  
  //G4cout << "BuildAluminumCanVolume" << G4endl;
  //BuildAluminumCanVolume(); 
  
  //G4cout << "BuildPackingVolume" << G4endl;
  //BuildPackingVolume();
  
  //G4cout << "BuildCopperClampVolume" << G4endl;
  //BuildCopperClampVolume();
  
  //G4cout << "BuildCoreVolume" << G4endl;
  //BuildCoreVolume();
  
  return 1;
}

G4int DetectionSystemGermaniumCylinder::PlaceDetector(G4LogicalVolume* exp_hall_log, G4ThreeVector move, G4RotationMatrix* rotate, G4int detector_number)
{
  G4int detector_copy_ID = 0;

  G4cout << "GermaniumCylinder Detector Number = " << detector_number << G4endl;

  G4int copy_number = detector_copy_ID + detector_number;

  assembly->MakeImprint(exp_hall_log, move, rotate, copy_number);

  return 1;
}

G4int DetectionSystemGermaniumCylinder::BuildCrystalVolume()
{
  G4Material* material = G4Material::GetMaterial(this->crystal_material);
  if( !material ) {
    G4cout << " ----> Material " << this->crystal_material << " not found, cannot build the detector shell! " << G4endl;
    return 0;
  }

  // Set visualization attributes
  G4VisAttributes* vis_att = new G4VisAttributes(G4Colour(0.2,1.0,0.3));
  vis_att->SetVisibility(true);

  G4Tubs* crystal_block = BuildCrystal();

  // Define rotation and movement objects
  G4ThreeVector direction 	= G4ThreeVector(0,0,1);
  //G4double z_position		= ( (can_front_lid_thickness + packing_front_lid_thickness) - (can_back_lid_thickness + packing_back_lid_thickness) )/2.0;
  G4double z_position = crystal_length_z/2.;
  G4ThreeVector move 		= z_position * direction;
  G4RotationMatrix* rotate = new G4RotationMatrix;
  
  //logical volume
  if( crystal_block_log == NULL )
  {
    crystal_block_log = new G4LogicalVolume(crystal_block, material, "germanium_cylinder_crystal_block_log", 0, 0, 0);
    crystal_block_log->SetVisAttributes(vis_att);
  }

  this->assembly->AddPlacedVolume(crystal_block_log, move, rotate);

  return 1;
}

G4int DetectionSystemGermaniumCylinder::BuildAluminumCanVolume()
{
  G4Material* material = G4Material::GetMaterial(this->can_material);
  if( !material ) {
    G4cout << " ----> Material " << this->can_material << " not found, cannot build the detector shell! " << G4endl;
    return 0;
  }
  
  // Set visualization attributes
  G4VisAttributes* vis_att = new G4VisAttributes(G4Colour(0.6,0.6,0.6));
  vis_att->SetVisibility(true);  

  G4ThreeVector direction =  G4ThreeVector(0,0,1);
  G4double z_position;
  G4ThreeVector move; 
  G4RotationMatrix* rotate = new G4RotationMatrix;

  /////////////////////////////////////////////////////////////////////
  // Build and Place Aluminum Can
  /////////////////////////////////////////////////////////////////////

  G4Tubs* can_cylinder = BuildAluminumCan(); 

  //logical volume
  if( can_cylinder_log == NULL )
  {
    can_cylinder_log = new G4LogicalVolume(can_cylinder, material, "can_cylinder_log", 0, 0, 0);
    can_cylinder_log->SetVisAttributes(vis_att);
  }

  // place front can_lid
  z_position 	= 0;
  move 		= z_position * direction;
  
  //add physical cylinder
  this->assembly->AddPlacedVolume(can_cylinder_log, move, rotate);

  /////////////////////////////////////////////////////////////////////
  // Build and Place Aluminum Front Lid
  /////////////////////////////////////////////////////////////////////  
  G4Tubs* can_front_lid = BuildAluminumCanFrontLid();

  // logical volume
  if( can_front_lid_log == NULL )
  {
    can_front_lid_log = new G4LogicalVolume(can_front_lid, material, "can_front_lid_log", 0, 0, 0);
    can_front_lid_log->SetVisAttributes(vis_att);
  }

  // place front can_lid

  z_position 	= -(detector_length_z/2.0) + (can_front_lid_thickness/2.0);
  move 		= z_position * direction;
  
  //add physical front can_lid
  this->assembly->AddPlacedVolume(can_front_lid_log, move, rotate);
    
  /////////////////////////////////////////////////////////////////////
  // Build and Place Aluminum Back Lid
  /////////////////////////////////////////////////////////////////////
  G4Tubs* can_back_lid = BuildAluminumCanBackLid();

  // logical volume
  if( can_back_lid_log == NULL )
  {
    can_back_lid_log = new G4LogicalVolume(can_back_lid, material, "can_back_lid_log", 0, 0, 0);
    can_back_lid_log->SetVisAttributes(vis_att);
  }

  // place back can_lid
  z_position 	= (detector_length_z/2.0) - (can_back_lid_thickness/2.0);
  move 		= z_position * direction;

  // add physical back can_lid
  this->assembly->AddPlacedVolume(can_back_lid_log, move, rotate);
  
  return 1;
}

G4int DetectionSystemGermaniumCylinder::BuildPackingVolume()
{
  G4Material* material = G4Material::GetMaterial(this->packing_material);
  if( !material ) {
    G4cout << " ----> Material " << this->packing_material << " not found, cannot build the detector shell! " << G4endl;
    return 0;
  }

  // Set visualization attributes
  G4VisAttributes* vis_att = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  vis_att->SetVisibility(true);

  G4ThreeVector direction =  G4ThreeVector(0,0,1);
  G4double z_position;
  G4ThreeVector move;
  G4RotationMatrix* rotate = new G4RotationMatrix;

  /////////////////////////////////////////////////////////////////////
  // Build and Place Packing
  /////////////////////////////////////////////////////////////////////
  G4Tubs* packing_cylinder = BuildPacking();

  //logical volume
  if( packing_cylinder_log == NULL )
  {
    packing_cylinder_log = new G4LogicalVolume(packing_cylinder, material, "packing_cylinder_log", 0, 0, 0);
    packing_cylinder_log->SetVisAttributes(vis_att);
  }

  // place cylinder
  z_position 	= ( (can_front_lid_thickness + packing_front_lid_thickness) - (can_back_lid_thickness + packing_back_lid_thickness) )/2.0;
  move          = z_position * direction;

  //add physical cylinder
  this->assembly->AddPlacedVolume(packing_cylinder_log, move, rotate);

  /////////////////////////////////////////////////////////////////////
  // Build and Place Front Lid
  /////////////////////////////////////////////////////////////////////
  G4Tubs* packing_front_lid = BuildPackingFrontLid();

  // logical volume
  if( packing_front_lid_log == NULL )
  {
    packing_front_lid_log = new G4LogicalVolume(packing_front_lid, material, "packing_front_lid_log", 0, 0, 0);
    packing_front_lid_log->SetVisAttributes(vis_att);
  }

  // place front packing_lid
  z_position 	= -(detector_length_z/2.0) + (can_front_lid_thickness + (packing_front_lid_thickness/2.0));
  move 		= z_position * direction;

  //add physical front packing_lid
  this->assembly->AddPlacedVolume(packing_front_lid_log, move, rotate);



  /////////////////////////////////////////////////////////////////////
  // Build and Place Back Lid
  /////////////////////////////////////////////////////////////////////
  G4Tubs* packing_back_lid = BuildPackingBackLid();

  // logical volume
  if( packing_back_lid_log == NULL )
  {
    packing_back_lid_log = new G4LogicalVolume(packing_back_lid, material, "packing_back_lid_log", 0, 0, 0);
    packing_back_lid_log->SetVisAttributes(vis_att);
  }

  // place back packing_lid
  z_position 	= (detector_length_z/2.0) - (can_back_lid_thickness + (packing_back_lid_thickness/2.0));
  move 		= z_position * direction;

  //add physical back packing_lid
  this->assembly->AddPlacedVolume(packing_back_lid_log, move, rotate);

  return 1;
}






G4int DetectionSystemGermaniumCylinder::BuildCopperClampVolume()
{
  G4Material* material = G4Material::GetMaterial(this->copper_material);
  if( !material ) {
    G4cout << " ----> Material " << this->copper_material << " not found, cannot build the detector shell! " << G4endl;
    return 0;
  }

  // Set visualization attributes
  G4VisAttributes* vis_att = new G4VisAttributes(G4Colour(0.5,0.0,0.5));
  vis_att->SetVisibility(true);

  G4ThreeVector direction =  G4ThreeVector(0,0,1);
  G4double z_position;
  G4ThreeVector move;
  G4RotationMatrix* rotate = new G4RotationMatrix;

  /////////////////////////////////////////////////////////////////////
  // Build and Place copper
  /////////////////////////////////////////////////////////////////////
  G4Tubs* copper_cylinder = BuildCopperClamp();

  //logical volume
  if( copper_cylinder_log == NULL )
  {
    copper_cylinder_log = new G4LogicalVolume(copper_cylinder, material, "copper_cylinder_log", 0, 0, 0);
    copper_cylinder_log->SetVisAttributes(vis_att);
  }

  // place cylinder
  z_position 	= ( (can_front_lid_thickness + packing_front_lid_thickness) - (can_back_lid_thickness + packing_back_lid_thickness) )/2.0;
  move          = z_position * direction;

  //add physical cylinder
  this->assembly->AddPlacedVolume(copper_cylinder_log, move, rotate);

  return 1;
}



G4int DetectionSystemGermaniumCylinder::BuildCoreVolume()
{
  G4Material* material = G4Material::GetMaterial(this->packing_material);
  if( !material ) {
    G4cout << " ----> Material " << this->packing_material << " not found, cannot build the detector shell! " << G4endl;
    return 0;
  }

  // Set visualization attributes
  G4VisAttributes* vis_att = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  vis_att->SetVisibility(true);

  /////////////////////////////////////////////////////////////////////
  // Build and Place core
  /////////////////////////////////////////////////////////////////////
  G4Tubs* the_core = BuildCore();

  // Define rotation and movement objects
  G4ThreeVector direction 	= G4ThreeVector(0,0,1);
  G4double z_position		= ( (can_front_lid_thickness + packing_front_lid_thickness) - (can_back_lid_thickness + packing_back_lid_thickness) )/2.0;
  G4ThreeVector move 		= z_position * direction;
  G4RotationMatrix* rotate = new G4RotationMatrix;

  //logical volume
  if( core_log == NULL )
  {
    core_log = new G4LogicalVolume(the_core, material, "core_log", 0, 0, 0);
    core_log->SetVisAttributes(vis_att);
  }

  this->assembly->AddPlacedVolume(core_log, move, rotate);

  return 1;
}


///////////////////////////////////////////////////////////////////////
// Methods used to build shapes
///////////////////////////////////////////////////////////////////////
G4Tubs* DetectionSystemGermaniumCylinder::BuildCrystal()
{
  G4double start_phi = 0.0;
  G4double end_phi = this->detail_view_end_angle;

  G4double inner_radius = crystal_inner_radius;
  G4double outer_radius = crystal_outer_radius;
  G4double half_length_z = (crystal_length_z)/2.0;

  G4Tubs* crystal_block = new G4Tubs("crystal_block", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

  return crystal_block;
}//end ::BuildCrystal


G4Tubs* DetectionSystemGermaniumCylinder::BuildAluminumCan()
{
  G4double start_phi = 0.0;
  G4double end_phi = this->detail_view_end_angle;

  G4double inner_radius 	= can_inner_radius;  			
  G4double outer_radius 	= can_outer_radius;
  G4double half_length_z 	= can_length_z/2.0;

  G4cout << " ----> inner_radius = " << inner_radius/cm << " cm - " << " - outer_radius = " << outer_radius/cm << " cm - " << " - half_length_z = " << half_length_z/cm << " cm - " << G4endl;

  G4Tubs* can_cylinder = new G4Tubs("can_cylinder", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

  return can_cylinder;
}//end ::BuildAluminumCan

G4Tubs* DetectionSystemGermaniumCylinder::BuildAluminumCanFrontLid()
{
  G4double start_phi = 0.0;
  G4double end_phi = this->detail_view_end_angle;

  G4double inner_radius = can_lid_inner_radius;
  G4double outer_radius = can_lid_outer_radius;
  G4double half_length_z = can_front_lid_thickness/2.0;
  
  G4Tubs* can_lid = new G4Tubs("can_lid", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

  return can_lid;
}//end ::BuildAluminumFrontLid

G4Tubs* DetectionSystemGermaniumCylinder::BuildAluminumCanBackLid()
{
  G4double start_phi = 0.0;
  G4double end_phi = this->detail_view_end_angle;

  G4double inner_radius = can_lid_inner_radius;
  G4double outer_radius = can_lid_outer_radius;
  G4double half_length_z = can_back_lid_thickness/2.0;
  
  G4Tubs* can_lid = new G4Tubs("can_lid", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

  return can_lid;
}//end ::BuildAluminumBackLid

G4Tubs* DetectionSystemGermaniumCylinder::BuildPacking()
{
  G4double start_phi = 0.0;
  G4double end_phi = this->detail_view_end_angle;

  G4double inner_radius 	= packing_inner_radius;
  G4double outer_radius 	= packing_outer_radius;
  G4double half_length_z 	= packing_length_z/2.0;

  G4Tubs* packing_cylinder = new G4Tubs("packing_cylinder", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

  return packing_cylinder;
}//end ::BuildPacking

G4Tubs* DetectionSystemGermaniumCylinder::BuildPackingFrontLid()
{
  G4double start_phi = 0.0;
  G4double end_phi = this->detail_view_end_angle;

  G4double inner_radius = packing_lid_inner_radius;
  G4double outer_radius = packing_lid_outer_radius;
  G4double half_length_z = packing_front_lid_thickness/2.0;

  G4Tubs* packing_lid = new G4Tubs("packing_lid", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

  return packing_lid;
}//end ::BuildPackingFrontLid

G4Tubs* DetectionSystemGermaniumCylinder::BuildPackingBackLid()
{
  G4double start_phi = 0.0;
  G4double end_phi = this->detail_view_end_angle;

  G4double inner_radius = packing_lid_inner_radius;
  G4double outer_radius = packing_lid_outer_radius;
  G4double half_length_z = packing_back_lid_thickness/2.0;

  G4Tubs* packing_lid = new G4Tubs("packing_lid", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

  return packing_lid;
}//end ::BuildPackingBackLid

G4Tubs* DetectionSystemGermaniumCylinder::BuildCore()
{
    G4double start_phi = 0.0;
    G4double end_phi = this->detail_view_end_angle;

    G4double inner_radius = crystal_core_inner_radius;
    G4double outer_radius = crystal_core_outer_radius;
    G4double half_length_z = (crystal_core_length_z)/2.0;

    G4Tubs* crystal_core_block = new G4Tubs("crystal_core_block", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

    return crystal_core_block;
}//end ::BuildCore

G4Tubs* DetectionSystemGermaniumCylinder::BuildCopperClamp()
{
    G4double start_phi = 0.0;
    G4double end_phi = this->detail_view_end_angle;

    G4double inner_radius = copper_inner_radius;
    G4double outer_radius = copper_outer_radius;
    G4double half_length_z = (copper_length_z)/2.0;

    G4Tubs* copper_block = new G4Tubs("copper_block", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

    return copper_block;
}//end ::BuildCopperClamp
