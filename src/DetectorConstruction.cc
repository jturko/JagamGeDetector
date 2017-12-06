//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file analysis/shared/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
// $Id: DetectorConstruction.cc 77256 2013-11-22 10:10:23Z gcosmo $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"

#include "Apparatus8piVacuumChamber.hh"
#include "Apparatus8piVacuumChamberAuxMatShell.hh"
#include "ApparatusBOMAB.hh"

#include "DetectionSystemSodiumIodide.hh"
#include "DetectionSystemLanthanumBromide.hh"

#include "DetectionSystemGermaniumCylinder.hh"
#include "DetectionSystemGrid.hh"
#include "DetectionSystemBox.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction() :
    // Fields
//    expHallMagField( 0 ),
//    defaultMaterial( 0 ),
    solidWorld( 0 ),
    logicWorld( 0 ),
    physiWorld( 0 )
{

    WorldSizeX  = WorldSizeY = WorldSizeZ = 10.0*m;

  box_mat = "G4_WATER";
  box_thickness = 0.0*mm;
  box_inner_dimensions = G4ThreeVector(0.0*mm,0.0*mm,0.0*mm);
  box_colour = G4ThreeVector(0.0,0.0,1.0);

  grid_mat = "G4_WATER";
  grid_size = 0.0*mm;
  grid_dimensions = G4ThreeVector(0.0*mm,0.0*mm,0.0*mm);
  grid_colour = G4ThreeVector(1.0,0.0,0.0);

  // materials
  DefineMaterials();

//  this->builtDetectors = false;

  // ensure the global field is initialized
//  (void)GlobalField::getObject();

  this->matWorldName = "G4_AIR";

  // Generic Target Apparatus
  this->setGenericTargetMaterial   = false;
  this->setGenericTargetDimensions = false;
  this->setGenericTargetPosition   = false;

  this->setBOMABMaterial           = false;
  this->theBOMABMaterial           = "G4_AIR";

  // Field Box
  this->setFieldBoxMaterial= false;
  this->setFieldBoxDimensions= false;
  this->setFieldBoxPosition= false;
  this->setFieldBoxMagneticField= false;

  // parameters to suppress:

  DefineSuppressedParameters();

  // Shield Selection Default

  this->useTigressPositions = false;

  this->detectorShieldSelect = 1 ; // Include suppressors by default.
  this->extensionSuppressorLocation = 0 ; // Back by default (Detector Forward)
  this->hevimetSelector = 0 ; // Chooses whether or not to include a hevimet

  this->customDetectorNumber 		= 1 ; // det_num
  this->customDetectorPosition  = 1 ; // pos_num
  this->customDetectorVal				= 0 ; // Unused for now (Oct 2013)


  // create commands for interactive definition

  detectorMessenger = new DetectorMessenger(this);

  // ensure the global field is initialized
  //(void)GlobalField::getObject();

  //expHallMagField = new MagneticField(); // Global field is set to zero

  griffinDetectorsMapIndex = 0;
  for(G4int i = 0; i < 16; i++)
  {
      griffinDetectorsMap[i] = 0;
  }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete detectorMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{

    // Replaced by ConstructDetectionSystems

  // Experimental hall (world volume)
  // search the world material by its name

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4Material* matWorld = G4Material::GetMaterial(matWorldName);

    if( !matWorld ) {
    G4cout << " ----> Material " << matWorldName << " not found, cannot build world! " << G4endl;
    return 0;
  }

  solidWorld = new G4Box("World", WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);

  logicWorld = new G4LogicalVolume(solidWorld,		//its solid
                                   matWorld,	//its material
                                   "World");		//its name

  physiWorld = new G4PVPlacement(   0,                  //no rotation
                                    G4ThreeVector(),	//at (0,0,0)
                                    logicWorld,         //its logical volume
                                    "World",            //its name
                                    0,                  //its mother  volume
                                    false,              //no boolean operation
                                    0);                 //copy number

  // Visualization Attributes

  logicWorld->SetVisAttributes (G4VisAttributes::Invisible); // The following block of code works too.

//  G4VisAttributes* worldVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
//  worldVisAtt->SetForceWireframe(true);
//  worldVisAtt->SetVisibility(this->world_vis);
//  logicWorld->SetVisAttributes(worldVisAtt);
//  this->logicWorld = logicWorld;

  return physiWorld ;


}

void DetectorConstruction::SetWorldMaterial( G4String name )
{
  this->matWorldName = name;
  UpdateGeometry(); // auto update
}

void DetectorConstruction::SetWorldDimensions( G4ThreeVector vec )
{
    WorldSizeX = vec.x() ;
    WorldSizeY = vec.y() ;
    WorldSizeZ = vec.z() ;
  UpdateGeometry(); // auto update
}

void DetectorConstruction::SetWorldVis( G4bool vis )
{
  this->world_vis = vis;
  UpdateGeometry(); // auto update
}

//void DetectorConstruction::SetWorldMagneticField( G4ThreeVector vec )
//{
//    //expHallMagField->SetFieldValue(G4ThreeVector(vec.x(),vec.y(),vec.z()));
//}

void DetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

void DetectorConstruction::AddApparatus8piVacuumChamber()
{
   //Create Vacuum Chamber
   Apparatus8piVacuumChamber* pApparatus8piVacuumChamber = new Apparatus8piVacuumChamber();
   pApparatus8piVacuumChamber->Build( logicWorld );
}

void DetectorConstruction::AddApparatus8piVacuumChamberAuxMatShell(G4double thickness)
{
   //Create Shell Around Vacuum Chamber
   Apparatus8piVacuumChamberAuxMatShell* pApparatus8piVacuumChamberAuxMatShell = new Apparatus8piVacuumChamberAuxMatShell();
   pApparatus8piVacuumChamberAuxMatShell->Build( logicWorld, thickness );
}

void DetectorConstruction::AddApparatusBOMAB(G4ThreeVector input)
{
   G4double theta,phi,position;
   G4ThreeVector move,direction;

   // Now in CMD
//   G4double head_dz = 19.5*cm;
//   G4double thorax_dz = 39.2*cm;
//   G4double abdomen_dz = 19.0*cm;
//   G4double neck_dz = 9.5*cm;
//   G4double thighs_dz = 38.5*cm;
//   G4double legs_dz = 38.5*cm;

//   G4double length_BOMAB = head_dz + thorax_dz + abdomen_dz + neck_dz + thighs_dz + legs_dz;
//   G4double length_offset = ((length_BOMAB/2.0) - head_dz - neck_dz - (thorax_dz/2.0));

   G4double length_offset = input.z();

   if(!setBOMABMaterial)
       G4cout << "The BOMAB material was not set, using " << theBOMABMaterial << " in the construction." << G4endl;

   //Create Shell Around Vacuum Chamber
   ApparatusBOMAB* pApparatusBOMAB = new ApparatusBOMAB();
   pApparatusBOMAB->Build(theBOMABMaterial);

   phi = 0.0*deg;
   theta = 0.0*deg;

   direction = G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
   position = length_offset;
   move = position * direction;

   G4RotationMatrix* rotate = new G4RotationMatrix;

   pApparatusBOMAB->PlaceBOMAB( logicWorld, move, rotate ) ;


}

void DetectorConstruction::AddApparatusBOMABMaterial(G4String input)
{
   this->setBOMABMaterial = true;
   this->theBOMABMaterial = input;
}

void DetectorConstruction::AddDetectionSystemSodiumIodide(G4int ndet)
{
  // Describe Placement
  G4double detectorAngles[8][2] = {0};
  G4double theta,phi,position;
  G4ThreeVector move,direction;

  detectorAngles[0][0] 	= 0.0;
  detectorAngles[1][0] 	= 45.0;
  detectorAngles[2][0] 	= 90.0;
  detectorAngles[3][0] 	= 135.0;
  detectorAngles[4][0] 	= 180.0;
  detectorAngles[5][0] 	= 225.0;
  detectorAngles[6][0] 	= 270.0;
  detectorAngles[7][0] 	= 315.0;
  detectorAngles[0][1] 	= 90.0;
  detectorAngles[1][1] 	= 90.0;
  detectorAngles[2][1] 	= 90.0;
  detectorAngles[3][1] 	= 90.0;
  detectorAngles[4][1] 	= 90.0;
  detectorAngles[5][1] 	= 90.0;
  detectorAngles[6][1] 	= 90.0;
  detectorAngles[7][1] 	= 90.0;

  DetectionSystemSodiumIodide* pSodiumIodide = new DetectionSystemSodiumIodide() ;
  pSodiumIodide->Build() ;

  for(G4int detector_number = 0; detector_number < ndet; detector_number++)
  {
    phi = detectorAngles[detector_number][0]*deg; // Creates a ring in phi plane
    theta = detectorAngles[detector_number][1]*deg;

    direction = G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
    position = 25.0*cm + (pSodiumIodide->GetDetectorLengthOfUnitsCM()/2.0);
    move = position * direction;

    G4RotationMatrix* rotate = new G4RotationMatrix; 		//rotation matrix corresponding to direction vector
    rotate->rotateX(theta);
    rotate->rotateY(0);
    rotate->rotateZ(phi+0.5*M_PI);

    pSodiumIodide->PlaceDetector( logicWorld, move, rotate, detector_number ) ;
  }
}

void DetectorConstruction::AddDetectionSystemLanthanumBromide(G4ThreeVector input)
{
    G4int ndet = G4int(input.x());
    G4double radialpos = input.y()*cm;

    DetectionSystemLanthanumBromide* pDetectionSystemLanthanumBromide = new DetectionSystemLanthanumBromide();
    pDetectionSystemLanthanumBromide->Build();

  for(G4int detector_number = 0; detector_number < ndet; detector_number++)
  {
    pDetectionSystemLanthanumBromide->PlaceDetector(logicWorld, detector_number, radialpos);
  }
}


void DetectorConstruction::AddBox()
{
    if(box_thickness != 0.0*mm) {
        DetectionSystemBox* pBox = new DetectionSystemBox(box_inner_dimensions.x(), box_inner_dimensions.y(), box_inner_dimensions.z(), box_thickness, box_mat, box_colour);
        pBox->Build();
        pBox->PlaceDetector(logicWorld);
    }
}


void DetectorConstruction::AddGermaniumCylinder( G4int ndet )
{
    G4double detectorAngles[8][2] = {0};
    G4double theta,phi,position;
    G4ThreeVector move,direction,offset;
    offset = G4ThreeVector(0.0*cm,0.0*cm,0.0*cm);

    DetectionSystemGermaniumCylinder* pDetectionSystemGermaniumCylinder = new DetectionSystemGermaniumCylinder();
    pDetectionSystemGermaniumCylinder->Build();

    //detectorAngles[0][0] = 90.0;
    //detectorAngles[0][1] = 90.0;

    for(G4int detector_number = 0; detector_number < ndet; detector_number++)
    {
        //phi = detectorAngles[detector_number][0]*deg; // Creates a ring in phi plane
        //theta = detectorAngles[detector_number][1]*deg;
        //direction = G4ThreeVector(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
        direction = G4ThreeVector(0,0,1);
        //position = 25.0*cm + (pDetectionSystemGermaniumCylinder->GetDetectorLengthOfUnitsCM()/2.0);
        position = (25.000 - 0.008)*cm;
        //offset.setZ(33.54*cm); // offset
        move = (position * direction) + offset;
        //move = G4ThreeVector(0,0,0);

        G4RotationMatrix* rotate = new G4RotationMatrix;
        //rotate->rotateX(theta);
        //rotate->rotateY(0);
        //rotate->rotateZ(phi+0.5*M_PI);

        pDetectionSystemGermaniumCylinder->PlaceDetector(logicWorld, move, rotate, detector_number);
    }

}



void DetectorConstruction::AddGrid()
{
    if(grid_size != 0.0*mm) {
        DetectionSystemGrid* pGrid = new DetectionSystemGrid(grid_dimensions.x(), grid_dimensions.y(), grid_dimensions.z(), grid_size, grid_mat, grid_colour);
        pGrid->Build();
        pGrid->PlaceDetector(logicWorld);
    }
}
