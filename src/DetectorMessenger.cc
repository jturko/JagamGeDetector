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
// $Id: DetectorMessenger.cc,v 1.1 2010-10-18 15:56:17 maire Exp $
// GEANT4 tag $Name: geant4-09-04-patch-02 $
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWith3Vector.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithADouble.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
:Detector(Det)
{
  DetSysDir = new G4UIdirectory("/DetSys/");
  DetSysDir->SetGuidance("UI commands of this example");

  detDir = new G4UIdirectory("/DetSys/det/");
  detDir->SetGuidance("detector control");

  appDir = new G4UIdirectory("/DetSys/app/");
  appDir->SetGuidance("apparatus control");

  worldDir = new G4UIdirectory("/DetSys/world/");
  worldDir->SetGuidance("world control");



  // Box Stuff
  addBoxMatCmd = new G4UIcmdWithAString("/DetSys/det/boxMat",this);
  addBoxMatCmd->SetGuidance("Set box material.");
  addBoxMatCmd->SetParameterName("choice",false);
  addBoxMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  addBoxThicknessCmd = new G4UIcmdWithADoubleAndUnit("/DetSys/det/boxThickness",this);
  addBoxThicknessCmd->SetGuidance("Set box thickness.");
  addBoxThicknessCmd->SetUnitCategory("Length");
  addBoxThicknessCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  addBoxInnerDimensionsCmd = new G4UIcmdWith3VectorAndUnit("/DetSys/det/boxInnerDimensions",this);
  addBoxInnerDimensionsCmd->SetGuidance("Set box inner dimensions.");
  addBoxInnerDimensionsCmd->SetUnitCategory("Length");
  addBoxInnerDimensionsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  addBoxColourCmd = new G4UIcmdWith3Vector("/DetSys/det/boxColour",this);
  addBoxColourCmd->SetGuidance("Set box colour.");
  addBoxColourCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  addBoxCmd = new G4UIcmdWithoutParameter("/DetSys/det/addBox",this);
  addBoxCmd->SetGuidance("Add a box.");
  addBoxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Germanium Cylinder
  addGermaniumCylinder = new G4UIcmdWithAnInteger("/DetSys/det/addGeCylinder",this);
  addGermaniumCylinder->SetGuidance("Add Ge cylinder detectors");
  addGermaniumCylinder->AvailableForStates(G4State_PreInit,G4State_Idle);

  // Grid Stuff
  addGridMatCmd = new G4UIcmdWithAString("/DetSys/det/gridMat",this);
  addGridMatCmd->SetGuidance("Set grid material.");
  addGridMatCmd->SetParameterName("choice",false);
  addGridMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  addGridSizeCmd = new G4UIcmdWithADoubleAndUnit("/DetSys/det/gridSize",this);
  addGridSizeCmd->SetGuidance("Set grid size.");
  addGridSizeCmd->SetUnitCategory("Length");
  addGridSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  addGridDimensionsCmd = new G4UIcmdWith3VectorAndUnit("/DetSys/det/gridDimensions",this);
  addGridDimensionsCmd->SetGuidance("Set grid dimensions.");
  addGridDimensionsCmd->SetUnitCategory("Length");
  addGridDimensionsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  addGridColourCmd = new G4UIcmdWith3Vector("/DetSys/det/gridColour",this);
  addGridColourCmd->SetGuidance("Set grid colour.");
  addGridColourCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  addGridCmd = new G4UIcmdWithoutParameter("/DetSys/det/addGrid",this);
  addGridCmd->SetGuidance("Add a grid.");
  addGridCmd->AvailableForStates(G4State_PreInit,G4State_Idle);


  WorldMaterialCmd = new G4UIcmdWithAString("/DetSys/world/material",this);
  WorldMaterialCmd->SetGuidance("Select material for the world.");
  WorldMaterialCmd->SetParameterName("choice",false);
  WorldMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  WorldDimensionsCmd = new G4UIcmdWith3VectorAndUnit("/DetSys/world/dimensions",this);
  WorldDimensionsCmd->SetGuidance("Set world dimensions - x y z unit.");
  WorldDimensionsCmd->SetUnitCategory("Length");
  WorldDimensionsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  WorldVisCmd = new G4UIcmdWithABool("/DetSys/world/vis",this);
  WorldVisCmd->SetGuidance("Set the visulization of the world");
  WorldVisCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  WorldMagneticFieldCmd = new G4UIcmdWith3VectorAndUnit("/DetSys/world/magneticField",this);
  WorldMagneticFieldCmd->SetGuidance("Set world magnetic field - x y z unit.");
  WorldMagneticFieldCmd->SetUnitCategory("Magnetic flux density");
  WorldMagneticFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  GenericTargetCmd = new G4UIcmdWithAString("/DetSys/app/genericTarget",this);
  GenericTargetCmd->SetGuidance("Select material of the target.");
  GenericTargetCmd->SetParameterName("choice",false);
  GenericTargetCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  GenericTargetDimensionsCmd = new G4UIcmdWith3VectorAndUnit("/DetSys/app/genericTargetDimensions",this);
  GenericTargetDimensionsCmd->SetGuidance("Set target dimensions - x y z unit.");
  GenericTargetDimensionsCmd->SetUnitCategory("Length");
  GenericTargetDimensionsCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  GenericTargetPositionCmd = new G4UIcmdWith3VectorAndUnit("/DetSys/app/genericTargetPosition",this);
  GenericTargetPositionCmd->SetGuidance("Set target position - x y z unit.");
  GenericTargetPositionCmd->SetUnitCategory("Length");
  GenericTargetPositionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  AddApparatus8piVacuumChamberCmd = new G4UIcmdWithoutParameter("/DetSys/app/add8piVacuumChamber",this);
  AddApparatus8piVacuumChamberCmd->SetGuidance("Add 8pi vacuum chamber.");
  AddApparatus8piVacuumChamberCmd->AvailableForStates(G4State_Idle);

  AddApparatus8piVacuumChamberAuxMatShellCmd = new G4UIcmdWithADoubleAndUnit("/DetSys/app/add8piVacuumChamberAuxMatShell",this);
  AddApparatus8piVacuumChamberAuxMatShellCmd->SetGuidance("Add AuxMat shell around 8pi vacuum chamber");
  AddApparatus8piVacuumChamberAuxMatShellCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  AddApparatusBOMABCmd = new G4UIcmdWith3VectorAndUnit("/DetSys/app/addBOMAB",this);
  AddApparatusBOMABCmd->SetGuidance("Add BOMAB.");
  AddApparatusBOMABCmd->AvailableForStates(G4State_Idle);

  AddApparatusBOMABMaterialCmd = new G4UIcmdWithAString("/DetSys/app/addBOMABMaterial",this);
  AddApparatusBOMABMaterialCmd->SetGuidance("BOMAB Material");
  AddApparatusBOMABMaterialCmd->SetParameterName("choice",false);
  AddApparatusBOMABMaterialCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/DetSys/det/update",this);
  UpdateCmd->SetGuidance("Update geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);


  AddDetectionSystemSodiumIodideCmd = new G4UIcmdWithAnInteger("/DetSys/det/addSodiumIodide",this);
  AddDetectionSystemSodiumIodideCmd->SetGuidance("Add Detection System SodiumIodide");
  AddDetectionSystemSodiumIodideCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  AddDetectionSystemLanthanumBromideCmd = new G4UIcmdWith3Vector("/DetSys/det/addLanthanumBromide",this);
  AddDetectionSystemLanthanumBromideCmd->SetGuidance("Add Detection System LanthanumBromide - number of dets, radius in cm, empty");
  AddDetectionSystemLanthanumBromideCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete detDir;
  delete appDir;
  delete worldDir;
  delete WorldMaterialCmd;
  delete WorldDimensionsCmd;
  delete WorldVisCmd;
  delete WorldMagneticFieldCmd;
  delete DetSysDir;
  delete UpdateCmd;
  delete GenericTargetCmd;
  delete GenericTargetDimensionsCmd;
  delete GenericTargetPositionCmd;

    delete addBoxMatCmd;
    delete addBoxThicknessCmd;
    delete addBoxInnerDimensionsCmd;
    delete addBoxColourCmd;
    delete addBoxCmd;
    delete addGermaniumCylinder;
    delete addGridMatCmd;
    delete addGridSizeCmd;
    delete addGridDimensionsCmd;
    delete addGridColourCmd;
    delete addGridCmd;

  delete AddApparatus8piVacuumChamberCmd;
  delete AddApparatus8piVacuumChamberAuxMatShellCmd;
  delete AddApparatusBOMABCmd;
  delete AddApparatusBOMABMaterialCmd;
  delete AddDetectionSystemSodiumIodideCmd;
  delete AddDetectionSystemLanthanumBromideCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == WorldMaterialCmd ) {
    Detector->SetWorldMaterial(newValue);
  }
  if( command == WorldDimensionsCmd ) {
    Detector->SetWorldDimensions(WorldDimensionsCmd->GetNew3VectorValue(newValue));
  }
  if( command == WorldVisCmd ) {
    Detector->SetWorldVis(WorldVisCmd->GetNewBoolValue(newValue));
  }
//  if( command == WorldMagneticFieldCmd ) {
//    Detector->SetWorldMagneticField(WorldMagneticFieldCmd->GetNew3VectorValue(newValue));
//  }
  if( command == UpdateCmd ) {
    Detector->UpdateGeometry();
  }

  if( command == AddApparatus8piVacuumChamberCmd ) {
    Detector->AddApparatus8piVacuumChamber();
  }
  if( command == AddApparatus8piVacuumChamberAuxMatShellCmd ) {
    Detector->AddApparatus8piVacuumChamberAuxMatShell(AddApparatus8piVacuumChamberAuxMatShellCmd->GetNewDoubleValue(newValue));
  }
  if( command == AddApparatusBOMABCmd ) {
    Detector->AddApparatusBOMAB(AddApparatusBOMABCmd->GetNew3VectorValue(newValue));
  }
  if( command == AddApparatusBOMABMaterialCmd ) {
    Detector->AddApparatusBOMABMaterial(newValue);
  }
  if( command == AddDetectionSystemSodiumIodideCmd ) {
    Detector->AddDetectionSystemSodiumIodide(AddDetectionSystemSodiumIodideCmd->GetNewIntValue(newValue));
  }
  if( command == AddDetectionSystemLanthanumBromideCmd ) {
    Detector->AddDetectionSystemLanthanumBromide(AddDetectionSystemLanthanumBromideCmd->GetNew3VectorValue(newValue));
  }

  if( command == addBoxMatCmd ) {
      Detector->SetBoxMat(newValue);
  }
  if( command == addBoxThicknessCmd ) {
      Detector->SetBoxThickness(addBoxThicknessCmd->GetNewDoubleValue(newValue));
  }
  if( command == addBoxInnerDimensionsCmd ) {
      Detector->SetBoxInnerDimensions(addBoxInnerDimensionsCmd->GetNew3VectorValue(newValue));
  }
  if( command == addBoxColourCmd ) {
      Detector->SetBoxColour(addBoxColourCmd->GetNew3VectorValue(newValue));
  }
  if( command == addBoxCmd ) {
      Detector->AddBox();
  }
  if( command == addGermaniumCylinder ) {
      Detector->AddGermaniumCylinder(addGermaniumCylinder->GetNewIntValue(newValue));
  }
  if( command == addGridMatCmd ) {
      Detector->SetGridMat(newValue);
  }
  if( command == addGridSizeCmd ) {
      Detector->SetGridSize(addGridSizeCmd->GetNewDoubleValue(newValue));
  }
  if( command == addGridDimensionsCmd ) {
      Detector->SetGridDimensions(addGridDimensionsCmd->GetNew3VectorValue(newValue));
  }
  if( command == addGridColourCmd ) {
      Detector->SetGridColour(addGridColourCmd->GetNew3VectorValue(newValue));
  }
  if( command == addGridCmd ) {
      Detector->AddGrid();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
