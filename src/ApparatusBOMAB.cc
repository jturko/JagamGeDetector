#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"

#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Cons.hh"
#include "G4Trd.hh"
#include "G4EllipticalTube.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4AssemblyVolume.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "ApparatusBOMAB.hh"

#include "G4SystemOfUnits.hh" // new version geant4.10 requires units

ApparatusBOMAB::ApparatusBOMAB() :
   // LogicalVolumes
  head_block_log(0),
  thorax_block_log(0),
  abdomen_block_log(0),
  neck_block_log(0),
  arms_right_block_log(0),
  arms_left_block_log(0),
  thighs_right_block_log(0),
  thighs_left_block_log(0),
  legs_right_block_log(0),
  legs_left_block_log(0)
{
  /////////////////////////////////////////////////////////////////////
  // ApparatusBOMAB Physical Properties
  /////////////////////////////////////////////////////////////////////
  this->detail_view_end_angle	 	= 360.0*deg;

  this->material_inside_BOMAB = "Water"; // This is overwritten in the Build method

  // The BOMAB will be constructed "layin down" in the xz-plane. So that if you
  // think of the human head as being a cylinder which is squished by between
  // the ears, such that the length from ear-to-ear is less than the length from
  // face to the back of the head. The ear-to-ear length is in the x-plane (dx), and
  // the face to the back of the head length is in the y-plane (dy). The height of
  // the head is in the z-plane.

  this->head_dx = 13.5*cm;
  this->head_dy = 18.5*cm;
  this->head_dz = 19.5*cm;

  this->thorax_dx = 28.0*cm;
  this->thorax_dy = 19.5*cm;
  this->thorax_dz = 39.2*cm;

  this->abdomen_dx = 34.0*cm;
  this->abdomen_dy = 19.5*cm;
  this->abdomen_dz = 19.0*cm;

  this->neck_dx = 12.0*cm;
  this->neck_dz = 9.5*cm;

  this->arms_dx = 12.0*cm;
  this->arms_dz = 58.0*cm;

  this->thighs_dx = 14.5*cm;
  this->thighs_dz = 38.5*cm;

  this->legs_dx = 12.0*cm;
  this->legs_dz = 38.5*cm;

}// end ::ApparatusBOMAB

ApparatusBOMAB::~ApparatusBOMAB()
{
   // LogicalVolumes in ConstructApparatusBOMAB
   delete head_block_log;
   delete thorax_block_log;
   delete abdomen_block_log;
   delete neck_block_log;
   delete arms_right_block_log;
   delete arms_left_block_log;
   delete thighs_right_block_log;
   delete thighs_left_block_log;
   delete legs_right_block_log;
   delete legs_left_block_log;
}// end ::~ApparatusBOMAB

G4int ApparatusBOMAB::Build( G4String materialBOMAB )
{

  this->material_inside_BOMAB = materialBOMAB;

  // Build assembly volume
  G4AssemblyVolume* myAssembly = new G4AssemblyVolume();
  this->assembly = myAssembly;

  G4cout << "BuildHeadVolume" << G4endl;
  BuildHeadVolume();
  G4cout << "BuildThoraxVolume" << G4endl;
  BuildThoraxVolume();
  G4cout << "BuildAbdomenVolume" << G4endl;
  BuildAbdomenVolume();
  G4cout << "BuildNeckVolume" << G4endl;
  BuildNeckVolume();
  G4cout << "BuildArmsVolume" << G4endl;
  BuildArmsVolume();
  G4cout << "BuildThighsVolume" << G4endl;
  BuildThighsVolume();
  G4cout << "BuildLegsVolume" << G4endl;
  BuildLegsVolume();

  return 1;
}

G4int ApparatusBOMAB::PlaceBOMAB(G4LogicalVolume* exp_hall_log, G4ThreeVector move, G4RotationMatrix* rotate)
{
  G4int copy_number = 99999;
  assembly->MakeImprint(exp_hall_log, move, rotate, copy_number);
  return 1;
}

///////////////////////////////////////////////////////////////////////
// Methods used to add volumes to assembly
///////////////////////////////////////////////////////////////////////
G4int ApparatusBOMAB::BuildHeadVolume()
{
  G4Material* material = G4Material::GetMaterial(this->material_inside_BOMAB);
  if( !material ) {
    G4cout << " ----> Material " << this->material_inside_BOMAB << " not found, cannot build the detector shell! " << G4endl;
    return 0;
  }

  // Set visualization attributes
  G4VisAttributes* vis_att = new G4VisAttributes(G4Colour(1.0,1.0,0.6));
  vis_att->SetVisibility(true);

  G4EllipticalTube* head_block = BuildHead();

  // Define rotation and movement objects
  G4ThreeVector direction 	= G4ThreeVector(0,0,1);
  G4double z_position		= neck_dz + ( head_dz + thorax_dz )/2.0;
  G4ThreeVector move 		= z_position * direction;
  G4RotationMatrix* rotate = new G4RotationMatrix;

  //logical volume
  if( head_block_log == NULL )
  {
    head_block_log = new G4LogicalVolume(head_block, material, "head_block_log", 0, 0, 0);
    head_block_log->SetVisAttributes(vis_att);
  }

  this->assembly->AddPlacedVolume(head_block_log, move, rotate);

  return 1;
}

G4int ApparatusBOMAB::BuildThoraxVolume()
{
  G4Material* material = G4Material::GetMaterial(this->material_inside_BOMAB);
  if( !material ) {
    G4cout << " ----> Material " << this->material_inside_BOMAB << " not found, cannot build the detector shell! " << G4endl;
    return 0;
  }

  // Set visualization attributes
  G4VisAttributes* vis_att = new G4VisAttributes(G4Colour(1.0,1.0,0.6));
  vis_att->SetVisibility(true);

  G4EllipticalTube* thorax_block = BuildThorax();

  // Define rotation and movement objects
  G4ThreeVector direction 	= G4ThreeVector(0,0,1);
  G4double z_position		= 0.0*cm;
  G4ThreeVector move 		= z_position * direction;
  G4RotationMatrix* rotate = new G4RotationMatrix;

  //logical volume
  if( thorax_block_log == NULL )
  {
    thorax_block_log = new G4LogicalVolume(thorax_block, material, "thorax_block_log", 0, 0, 0);
    thorax_block_log->SetVisAttributes(vis_att);
  }

  this->assembly->AddPlacedVolume(thorax_block_log, move, rotate);

  return 1;
}

G4int ApparatusBOMAB::BuildAbdomenVolume()
{
  G4Material* material = G4Material::GetMaterial(this->material_inside_BOMAB);
  if( !material ) {
    G4cout << " ----> Material " << this->material_inside_BOMAB << " not found, cannot build the detector shell! " << G4endl;
    return 0;
  }

  // Set visualization attributes
  G4VisAttributes* vis_att = new G4VisAttributes(G4Colour(1.0,1.0,0.6));
  vis_att->SetVisibility(true);

  G4EllipticalTube* abdomen_block = BuildAbdomen();

  // Define rotation and movement objects
  G4ThreeVector direction 	= G4ThreeVector(0,0,1);
  G4double z_position		= -1.0*( abdomen_dz +  thorax_dz )/2.0;
  G4ThreeVector move 		= z_position * direction;
  G4RotationMatrix* rotate = new G4RotationMatrix;

  //logical volume
  if( abdomen_block_log == NULL )
  {
    abdomen_block_log = new G4LogicalVolume(abdomen_block, material, "abdomen_block_log", 0, 0, 0);
    abdomen_block_log->SetVisAttributes(vis_att);
  }

  this->assembly->AddPlacedVolume(abdomen_block_log, move, rotate);

  return 1;
}

G4int ApparatusBOMAB::BuildNeckVolume()
{
  G4Material* material = G4Material::GetMaterial(this->material_inside_BOMAB);
  if( !material ) {
    G4cout << " ----> Material " << this->material_inside_BOMAB << " not found, cannot build the detector shell! " << G4endl;
    return 0;
  }

  // Set visualization attributes
  G4VisAttributes* vis_att = new G4VisAttributes(G4Colour(1.0,1.0,0.6));
  vis_att->SetVisibility(true);

  G4Tubs* neck_block = BuildNeck();

  // Define rotation and movement objects
  G4ThreeVector direction 	= G4ThreeVector(0,0,1);
  G4double z_position		= ( neck_dz + thorax_dz )/2.0;
  G4ThreeVector move 		= z_position * direction;
  G4RotationMatrix* rotate = new G4RotationMatrix;

  //logical volume
  if( neck_block_log == NULL )
  {
    neck_block_log = new G4LogicalVolume(neck_block, material, "neck_block_log", 0, 0, 0);
    neck_block_log->SetVisAttributes(vis_att);
  }

  this->assembly->AddPlacedVolume(neck_block_log, move, rotate);

  return 1;
}


G4int ApparatusBOMAB::BuildArmsVolume()
{
  G4Material* material = G4Material::GetMaterial(this->material_inside_BOMAB);
  if( !material ) {
    G4cout << " ----> Material " << this->material_inside_BOMAB << " not found, cannot build the detector shell! " << G4endl;
    return 0;
  }

  // Set visualization attributes
  G4VisAttributes* vis_att = new G4VisAttributes(G4Colour(1.0,1.0,0.6));
  vis_att->SetVisibility(true);

  G4Tubs* arms_right_block  = BuildArms();
  G4Tubs* arms_left_block   = BuildArms();

  G4double side_dx;
  if( arms_dz > thorax_dz && abdomen_dx > thorax_dx ) {
      side_dx = abdomen_dx;
  }
  else {
      side_dx = thorax_dx;
  }

  G4double d1 = (side_dx + arms_dx)/2.0;
  G4double d2 = (thorax_dz - arms_dz)/2.0;
  G4double norm = sqrt( d1*d1 + d2*d2 );

  // Right Arm

  // Define rotation and movement objects
  G4ThreeVector direction 	= G4ThreeVector(d1/norm,0,d2/norm);
  G4double z_position		= norm;
  G4ThreeVector move 		= z_position * direction;
  G4RotationMatrix* rotate = new G4RotationMatrix;

  //logical volume
  if( arms_right_block_log == NULL )
  {
    arms_right_block_log = new G4LogicalVolume(arms_right_block, material, "arms_block_log", 0, 0, 0);
    arms_right_block_log->SetVisAttributes(vis_att);
  }

  this->assembly->AddPlacedVolume(arms_right_block_log, move, rotate);

  // Left Arm

  // Define rotation and movement objects
  direction 	= G4ThreeVector(-1.0*d1/norm,0,d2/norm);
  z_position	= norm;
  move          = z_position * direction;

  //logical volume
  if( arms_left_block_log == NULL )
  {
    arms_left_block_log = new G4LogicalVolume(arms_left_block, material, "arms_block_log", 0, 0, 0);
    arms_left_block_log->SetVisAttributes(vis_att);
  }

  this->assembly->AddPlacedVolume(arms_left_block_log, move, rotate);

  return 1;
}

G4int ApparatusBOMAB::BuildThighsVolume()
{
  G4Material* material = G4Material::GetMaterial(this->material_inside_BOMAB);
  if( !material ) {
    G4cout << " ----> Material " << this->material_inside_BOMAB << " not found, cannot build the detector shell! " << G4endl;
    return 0;
  }

  // Set visualization attributes
  G4VisAttributes* vis_att = new G4VisAttributes(G4Colour(1.0,1.0,0.6));
  vis_att->SetVisibility(true);

  G4Tubs* thighs_right_block  = BuildThighs();
  G4Tubs* thighs_left_block   = BuildThighs();

  G4double d1 = (abdomen_dx - thighs_dx)/2.0;
  G4double d2 = -1.0*(abdomen_dz + (thorax_dz + thighs_dz)/2.0);
  G4double norm = sqrt( d1*d1 + d2*d2 );

  // Right Thigh

  // Define rotation and movement objects
  G4ThreeVector direction 	= G4ThreeVector(d1/norm,0,d2/norm);
  G4double z_position		= norm;
  G4ThreeVector move 		= z_position * direction;
  G4RotationMatrix* rotate = new G4RotationMatrix;

  //logical volume
  if( thighs_right_block_log == NULL )
  {
    thighs_right_block_log = new G4LogicalVolume(thighs_right_block, material, "thighs_block_log", 0, 0, 0);
    thighs_right_block_log->SetVisAttributes(vis_att);
  }

  this->assembly->AddPlacedVolume(thighs_right_block_log, move, rotate);

  // Left Thigh

  // Define rotation and movement objects
  direction 	= G4ThreeVector(-1.0*d1/norm,0,d2/norm);
  z_position	= norm;
  move          = z_position * direction;

  //logical volume
  if( thighs_left_block_log == NULL )
  {
    thighs_left_block_log = new G4LogicalVolume(thighs_left_block, material, "thighs_block_log", 0, 0, 0);
    thighs_left_block_log->SetVisAttributes(vis_att);
  }

  this->assembly->AddPlacedVolume(thighs_left_block_log, move, rotate);

  return 1;
}

G4int ApparatusBOMAB::BuildLegsVolume()
{
  G4Material* material = G4Material::GetMaterial(this->material_inside_BOMAB);
  if( !material ) {
    G4cout << " ----> Material " << this->material_inside_BOMAB << " not found, cannot build the detector shell! " << G4endl;
    return 0;
  }

  // Set visualization attributes
  G4VisAttributes* vis_att = new G4VisAttributes(G4Colour(1.0,1.0,0.6));
  vis_att->SetVisibility(true);

  G4Tubs* legs_right_block  = BuildLegs();
  G4Tubs* legs_left_block   = BuildLegs();

  G4double d1 = (abdomen_dx - thighs_dx)/2.0;
  G4double d2 = -1.0*(abdomen_dz + thighs_dz + (thorax_dz + legs_dz)/2.0);
  G4double norm = sqrt( d1*d1 + d2*d2 );

  // Right Leg

  // Define rotation and movement objects
  G4ThreeVector direction 	= G4ThreeVector(d1/norm,0,d2/norm);
  G4double z_position		= norm;
  G4ThreeVector move 		= z_position * direction;
  G4RotationMatrix* rotate = new G4RotationMatrix;

  //logical volume
  if( legs_right_block_log == NULL )
  {
    legs_right_block_log = new G4LogicalVolume(legs_right_block, material, "legs_block_log", 0, 0, 0);
    legs_right_block_log->SetVisAttributes(vis_att);
  }

  this->assembly->AddPlacedVolume(legs_right_block_log, move, rotate);

  // Left Leg

  // Define rotation and movement objects
  direction 	= G4ThreeVector(-1.0*d1/norm,0,d2/norm);
  z_position	= norm;
  move          = z_position * direction;

  //logical volume
  if( legs_left_block_log == NULL )
  {
    legs_left_block_log = new G4LogicalVolume(legs_left_block, material, "legs_block_log", 0, 0, 0);
    legs_left_block_log->SetVisAttributes(vis_att);
  }

  this->assembly->AddPlacedVolume(legs_left_block_log, move, rotate);

  return 1;
}


///////////////////////////////////////////////////////////////////////
// Methods used to build shapes
///////////////////////////////////////////////////////////////////////
G4EllipticalTube* ApparatusBOMAB::BuildHead()
{
  G4double half_length_x = (head_dx)/2.0;
  G4double half_length_y = (head_dy)/2.0;
  G4double half_length_z = (head_dz)/2.0;

  G4EllipticalTube* head_block = new G4EllipticalTube("head_block", half_length_x, half_length_y, half_length_z);

  return head_block;
}

G4EllipticalTube* ApparatusBOMAB::BuildThorax()
{
  G4double half_length_x = (thorax_dx)/2.0;
  G4double half_length_y = (thorax_dy)/2.0;
  G4double half_length_z = (thorax_dz)/2.0;

  G4EllipticalTube* thorax_block = new G4EllipticalTube("thorax_block", half_length_x, half_length_y, half_length_z);

  return thorax_block;
}

G4EllipticalTube* ApparatusBOMAB::BuildAbdomen()
{
  G4double half_length_x = (abdomen_dx)/2.0;
  G4double half_length_y = (abdomen_dy)/2.0;
  G4double half_length_z = (abdomen_dz)/2.0;

  G4EllipticalTube* abdomen_block = new G4EllipticalTube("abdomen_block", half_length_x, half_length_y, half_length_z);

  return abdomen_block;
}

G4Tubs* ApparatusBOMAB::BuildNeck()
{
  G4double start_phi = 0.0;
  G4double end_phi = this->detail_view_end_angle;

  G4double inner_radius = 0.0*cm;
  G4double outer_radius = (neck_dx)/2.0;
  G4double half_length_z = (neck_dz)/2.0;

  G4Tubs* neck_block = new G4Tubs("neck_block", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

  return neck_block;
}

G4Tubs* ApparatusBOMAB::BuildArms()
{
  G4double start_phi = 0.0;
  G4double end_phi = this->detail_view_end_angle;

  G4double inner_radius = 0.0*cm;
  G4double outer_radius = (arms_dx)/2.0;
  G4double half_length_z = (arms_dz)/2.0;

  G4Tubs* arms_block = new G4Tubs("arms_block", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

  return arms_block;
}

G4Tubs* ApparatusBOMAB::BuildThighs()
{
  G4double start_phi = 0.0;
  G4double end_phi = this->detail_view_end_angle;

  G4double inner_radius = 0.0*cm;
  G4double outer_radius = (thighs_dx)/2.0;
  G4double half_length_z = (thighs_dz)/2.0;

  G4Tubs* thighs_block = new G4Tubs("thighs_block", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

  return thighs_block;
}

G4Tubs* ApparatusBOMAB::BuildLegs()
{
  G4double start_phi = 0.0;
  G4double end_phi = this->detail_view_end_angle;

  G4double inner_radius = 0.0*cm;
  G4double outer_radius = (legs_dx)/2.0;
  G4double half_length_z = (legs_dz)/2.0;

  G4Tubs* legs_block = new G4Tubs("legs_block", inner_radius, outer_radius, half_length_z, start_phi, end_phi);

  return legs_block;
}
