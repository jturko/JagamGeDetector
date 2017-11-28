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
//
// $Id: DetectorConstruction.hh,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: geant4-09-04-patch-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef ApparatusBOMAB_h
#define ApparatusBOMAB_h 1

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;
class G4AssemblyVolume;
class G4EllipticalTube;

///////////////////////////////////////////////////////////////////////
// ApparatusBOMAB
///////////////////////////////////////////////////////////////////////
class ApparatusBOMAB
{
  public:
    ApparatusBOMAB();
    ~ApparatusBOMAB();

  public:
    G4int Build( G4String );
    G4int PlaceBOMAB(G4LogicalVolume* exp_hall_log, G4ThreeVector move, G4RotationMatrix* rotate);

  private:
    G4LogicalVolume* expHallLog;

  private:
    // LogicalVolumes used in ApparatusBOMAB()
    G4LogicalVolume* head_block_log;
    G4LogicalVolume* thorax_block_log;
    G4LogicalVolume* abdomen_block_log;
    G4LogicalVolume* neck_block_log;
    G4LogicalVolume* arms_block_log;
    G4LogicalVolume* arms_right_block_log;
    G4LogicalVolume* arms_left_block_log;
    G4LogicalVolume* thighs_right_block_log;
    G4LogicalVolume* thighs_left_block_log;
    G4LogicalVolume* legs_right_block_log;
    G4LogicalVolume* legs_left_block_log;

    // Assembly volumes
    G4AssemblyVolume* assembly;

private:
    ///////////////////////////////////////////////////////////////////
    // ApparatusBOMABCylinder Properties
    ///////////////////////////////////////////////////////////////////

    G4double detail_view_end_angle;

    // Materials
    G4String material_inside_BOMAB;

    // Elliptic Cylinders
    G4double head_dx;
    G4double head_dy;
    G4double head_dz;

    G4double thorax_dx;
    G4double thorax_dy;
    G4double thorax_dz;

    G4double abdomen_dx;
    G4double abdomen_dy;
    G4double abdomen_dz;

    // Cylinders
    G4double neck_dx;
    G4double neck_dz;

    G4double arms_dx;
    G4double arms_dz;

    G4double thighs_dx;
    G4double thighs_dz;

    G4double legs_dx;
    G4double legs_dz;

    G4int               BuildHeadVolume();
    G4int               BuildThoraxVolume();
    G4int               BuildAbdomenVolume();
    G4int               BuildNeckVolume();
    G4int               BuildArmsVolume();
    G4int               BuildThighsVolume();
    G4int               BuildLegsVolume();

    G4EllipticalTube*   BuildHead();
    G4EllipticalTube*   BuildThorax();
    G4EllipticalTube*   BuildAbdomen();
    G4Tubs*             BuildNeck();
    G4Tubs*             BuildArms();
    G4Tubs*             BuildThighs();
    G4Tubs*             BuildLegs();


};

#endif
