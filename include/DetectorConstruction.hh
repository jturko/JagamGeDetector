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
/// \file analysis/shared/include/DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
//
// $Id: DetectorConstruction.hh 77256 2013-11-22 10:10:23Z gcosmo $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

class DetectionSystemSodiumIodide;
class DetectionSystemLanthanumBromide;

class DetectionSystemGermaniumCylinder;
class DetectionSystemGrid;
class DetectionSystemBox;

//class MagneticField;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
   ~DetectorConstruction();

    G4int griffinDetectorsMapIndex;
    G4int griffinDetectorsMap[16];

    void SetWorldMaterial( G4String );
    void SetWorldDimensions( G4ThreeVector );
    void SetWorldVis( G4bool );

    void AddApparatus8piVacuumChamber();
    void AddApparatus8piVacuumChamberAuxMatShell(G4double thickness);
    void AddApparatusBOMAB(G4ThreeVector input);
    void AddApparatusBOMABMaterial( G4String );

    G4double GetWorldSizeX()           {return WorldSizeX;};
    G4double GetWorldSizeY()           {return WorldSizeY;};
    G4double GetWorldSizeZ()           {return WorldSizeZ;};

    const G4VPhysicalVolume* GetphysiWorld() {return physiWorld;};

    G4VPhysicalVolume* Construct();

    void UpdateGeometry();

    void AddDetectionSystemSodiumIodide(G4int ndet);
    void AddDetectionSystemLanthanumBromide(G4ThreeVector input);

    G4double GetLanthanumBromideCrystalRadius();
    G4double GetLanthanumBromideCrystalLength();
    G4double GetLanthanumBromideR();
    G4double GetLanthanumBromideTheta(G4int i);
    G4double GetLanthanumBromidePhi(G4int i);
    G4double GetLanthanumBromideYaw(G4int i);
    G4double GetLanthanumBromidePitch(G4int i);
    G4double GetLanthanumBromideRoll(G4int i);
    G4double GetLanthanumBromideCrystalRadialPosition();

    // Box Functions
    void SetBoxMat( G4String input )                   {box_mat = input;};
    void SetBoxThickness( G4double input )             {box_thickness = input;};
    void SetBoxInnerDimensions( G4ThreeVector input )  {box_inner_dimensions = input;};
    void SetBoxColour( G4ThreeVector input )           {box_colour = input;};
    void AddBox();
    // Germanium Cylinder Functions
    void AddGermaniumCylinder( G4int ndet );
    // Grid Functions
    void SetGridMat( G4String input )                  {grid_mat = input;};
    void SetGridSize( G4double input )                 {grid_size = input;};
    void SetGridDimensions( G4ThreeVector input )      {grid_dimensions = input;};
    void SetGridColour( G4ThreeVector input )          {grid_colour = input;};
    void AddGrid();

  private:

//    MagneticField* worldMagField;

    G4double  WorldSizeX;
    G4double  WorldSizeY;
    G4double  WorldSizeZ;
    G4bool    world_vis;
    G4bool    builtDetectors;
    G4double  griffinFwdBackPosition;
    G4int     detectorShieldSelect ;
    G4double  detectorRadialDistance ;
    G4int     extensionSuppressorLocation ;
    G4int     customDetectorNumber ;
    G4int     customDetectorPosition ;
    G4int     customDetectorVal ;
    G4int     hevimetSelector ;
    G4bool    useTigressPositions;

    // Box
    G4String           box_mat;
    G4double           box_thickness;
    G4ThreeVector      box_inner_dimensions;
    G4ThreeVector      box_colour;

    G4Box*             solidWorld;    //pointer to the solid World
    G4LogicalVolume*   logicWorld;    //pointer to the logical World
    G4VPhysicalVolume* physiWorld;    //pointer to the physical World

    // Grid
    G4String           grid_mat;
    G4double           grid_size;
    G4ThreeVector      grid_dimensions;
    G4ThreeVector      grid_colour;

    void DefineSuppressedParameters();
    void DefineMaterials();

    G4double coords[20][5];
    G4bool        setGenericTargetMaterial;
    G4bool        setGenericTargetDimensions;


    G4bool        setGenericTargetPosition;
    G4bool        setBOMABMaterial;
    G4String      genericTargetMaterial;
    G4String      theBOMABMaterial;

    G4ThreeVector genericTargetDimensions;
    G4ThreeVector genericTargetPosition;

    G4bool        setFieldBoxMaterial;
    G4bool        setFieldBoxDimensions;
    G4bool        setFieldBoxPosition;
    G4bool        setFieldBoxMagneticField;
    G4String      fieldBoxMaterial;
    G4ThreeVector fieldBoxDimensions;
    G4ThreeVector fieldBoxPosition;
    G4ThreeVector fieldBoxMagneticField;

    G4String matWorldName;

    DetectorMessenger* detectorMessenger;

};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


