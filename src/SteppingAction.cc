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
/// \file analysis/shared/src/SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//
// $Id: SteppingAction.cc 68015 2013-03-13 13:27:27Z gcosmo $
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4Step.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* detcon,
                                         EventAction* evt)
: G4UserSteppingAction(), 
  fDetector(detcon), fEventAction(evt)
{
    stepNumber = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{

  G4int particleType = 0;
  G4int processType = 0;
  G4int evntNb;
  G4int systemID;

  det = 0;
  cry = 0;

  G4String particleName;
  G4String mnemonic = "XXX00XX00X";

  // Get volume of the current step
  G4VPhysicalVolume* volume = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  G4String volname = volume->GetName();

  // collect energy and track length step by step
  // As it's called more than once, get the Track and assign to variable
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4double ekin = aStep->GetPreStepPoint()->GetKineticEnergy();

    G4Track* theTrack = aStep->GetTrack();
  G4double stepl = 0.;
  if (theTrack->GetDefinition()->GetPDGCharge() != 0.)
    stepl = aStep->GetStepLength();

  stepNumber = theTrack->GetCurrentStepNumber();

  // Track particle type in EVERY step
  //G4cout << "Particle name = " << aStep->GetTrack()->GetParticleDefinition()->GetParticleName() << G4endl;
  particleName = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
  if (particleName == "gamma")         particleType = 1;
  else if (particleName == "e-")       particleType = 2;
  else if (particleName == "e+")       particleType = 3;
  else if (particleName == "proton")   particleType = 4;
  else if (particleName == "neutron")  particleType = 5;
  else particleType = 0;

  // this can be modified to add more processes
//  if (theTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay")      processType = 1;
//  else if (theTrack->GetCreatorProcess()->GetProcessName() == "eIoni")            processType = 2;
//  else if (theTrack->GetCreatorProcess()->GetProcessName() == "msc")              processType = 3;
//  else if (theTrack->GetCreatorProcess()->GetProcessName() == "Scintillation")    processType = 4;
//  else if (theTrack->GetCreatorProcess()->GetProcessName() == "Cerenkov")         processType = 5;
//  else  processType = 0;

  fEventAction->AddParticleType(particleType);
  evntNb =  fEventAction->GetEventNumber();

  //G4cout << "Found Edep = " << edep/keV << " keV in " << volname << G4endl;
  // example volname
  //volname = av_1_impr_6_sodium_iodide_crystal_block_log_pv_0

  // Get initial momentum direction & energy of particle
  G4int trackID = theTrack->GetTrackID();
  G4int parentID = theTrack->GetParentID();

  G4StepPoint* point1 = aStep->GetPreStepPoint();
  G4StepPoint* point2 = aStep->GetPostStepPoint();

  G4ThreeVector pos1 = point1->GetPosition();
  G4ThreeVector pos2 = point2->GetPosition();

  G4double time1 = point1->GetGlobalTime();
  G4double time2 = point2->GetGlobalTime();

  size_t found;

  // LaBr detector energy deposits ////////////////////////////////////////////////////////////////////////////////
  found = volname.find("lanthanum_bromide_crystal_block");
  if (edep != 0 && found!=G4String::npos) {
      SetDetNumberForGenericDetector(volname);
      fEventAction->AddLaBrCrystDet(edep,stepl,det-1);
      mnemonic.replace(0,3,"LAB");
      mnemonic.replace(3,2,G4intToG4String(det));
      mnemonic.replace(5,1,GetCrystalColour(cry));
      systemID = 2000;
      fEventAction->AddHitTracker(mnemonic, evntNb, trackID, parentID, stepNumber, particleType, processType, systemID, cry-1, det-1, edep, pos2.x(), pos2.y(), pos2.z(), time2);
  }

  // NaI energy deposits ////////////////////////////////////////////////////////////////////////////////
  found = volname.find("sodium_iodide_crystal_block");
  if (edep != 0 && found!=G4String::npos) {
      SetDetNumberForGenericDetector(volname);
      mnemonic.replace(0,3,"NAI");
      mnemonic.replace(3,2,G4intToG4String(det));
      mnemonic.replace(5,1,GetCrystalColour(cry));
      systemID = 4000;
      fEventAction->AddHitTracker(mnemonic, evntNb, trackID, parentID, stepNumber, particleType, processType, systemID, cry-1, det-1, edep, pos2.x(), pos2.y(), pos2.z(), time2);
  }

  // Germanium Cylinder
  found = volname.find("germanium_cylinder_crystal_block");
  if ( (edep != 0 || ekin != 0) && found!=G4String::npos) {
      SetDetNumberForGenericDetector(volname);
      fEventAction->AddGermaniumCylinderCrystDet(edep,stepl,ekin,det-1);
      mnemonic.replace(0,3,"LAB"); // LOOKS LIKE LABR FOR NOW!
      mnemonic.replace(3,2,G4intToG4String(det));
      mnemonic.replace(5,1,GetCrystalColour(cry));
      systemID = 2000;
      fEventAction->AddHitTracker(mnemonic, evntNb, trackID, parentID, stepNumber, particleType, processType, systemID, cry-1, det-1, edep, pos2.x(), pos2.y(), pos2.z(), time2);
  }

  // Grid Cell
  found = volname.find("gridcell");
  if (ekin != 0 && found!=G4String::npos && particleType == 1) {
      SetDetNumberForGenericDetector(volname);
      fEventAction->SetGridEKinGammaDet(ekin,stepl,det-1);
  }

  found = volname.find("gridcell");
  if (ekin != 0 && found!=G4String::npos && particleType == 2) {
      SetDetNumberForGenericDetector(volname);
      fEventAction->SetGridEKinElectronDet(ekin,stepl,det-1);
  }

//  // gamma angular correlations in world
//  found = volname.find("World");
//  if (ekin != 0 && found!=G4String::npos && particleType == 1) {
//      if(ekin <= (100.+2.)*keV && ekin >= (100.-2.)*keV) {
//          det = 1;
//      }
//      else if(ekin <= (200.+2.)*keV && ekin >= (200.-2.)*keV) {
//          det = 2;
//      }
//      else if(ekin <= (300.+2.)*keV && ekin >= (300.-2.)*keV) {
//          det = 3;
//      }
//      else if(ekin <= (400.+2.)*keV && ekin >= (400.-2.)*keV) {
//          det = 4;
//      }
//      else if(ekin <= (500.+2.)*keV && ekin >= (500.-2.)*keV) {
//          det = 5;
//      }
//      else if(ekin <= (600.+2.)*keV && ekin >= (600.-2.)*keV) {
//          det = 6;
//      }
//      else if(ekin <= (700.+2.)*keV && ekin >= (700.-2.)*keV) {
//          det = 7;
//      }
//      else if(ekin <= (800.+2.)*keV && ekin >= (800.-2.)*keV) {
//          det = 8;
//      }
//      else if(ekin <= (900.+2.)*keV && ekin >= (900.-2.)*keV) {
//          det = 9;
//      }
//      else if(ekin <= (1173.2+2.)*keV && ekin >= (1173.2-2.)*keV) { //60Co decay
//          det = 10;
//      }
//      else if(ekin <= (1332.5+2.)*keV && ekin >= (1332.5-2.)*keV) {
//          det = 11;
//      }
//      else if(ekin <= (1808.660+2.)*keV && ekin >= (1808.660-2.)*keV) { //26Na decay
//          det = 12;
//      }
//      else if(ekin <= (1896.720+2.)*keV && ekin >= (1896.720-2.)*keV) {
//          det = 13;
//      }
//      else if(ekin <= (2541.220+2.)*keV && ekin >= (2541.220-2.)*keV) {
//          det = 14;
//      }
//      else if(ekin <= (1129.580+2.)*keV && ekin >= (1129.580-2.)*keV) {
//          det = 15;
//      }
//      else if(ekin <= (953.8+2.)*keV && ekin >= (953.8-2.)*keV) { //62Ga decay
//          det = 16;
//      }
//      else if(ekin <= (850.8+2.)*keV && ekin >= (850.8-2.)*keV) {
//          det = 17;
//      }
//      else if(ekin <= (1388.300+2.)*keV && ekin >= (1388.300-2.)*keV) {
//          det = 18;
//      }
//      else {
//          det = 0;
//      }
//      cry = 1;
//      systemID = 9999;
//      mnemonic.replace(0,3,"GAC");
//      mnemonic.replace(3,2,G4intToG4String(det));
//      mnemonic.replace(5,1,GetCrystalColour(cry));
//      fEventAction->AddHitTracker(mnemonic, evntNb, trackID, parentID, stepNumber, particleType, processType, systemID, cry-1, det-1, ekin, pos2.x(), pos2.y(), pos2.z(), time2);
//  }

}

void SteppingAction::SetDetNumberForGenericDetector(G4String volname)
{
    const char *cstr = volname.c_str();
    G4int volNameOver9;
    G4int avOver9 = cstr[4]-'0';
    G4int avOver99 = cstr[5]-'0';
    if(avOver9 == 47) { // under 10
        volNameOver9 = cstr[11]-'0';
        if(volNameOver9 == 47) {
            det = cstr[10]-'0';
        }
        else {
            det = ((cstr[10]-'0')*10)+volNameOver9 ;
        }
    }
    else if(avOver99 == 47) { // under 100
        volNameOver9 = cstr[12]-'0';
        if(volNameOver9 == 47) {
            det = cstr[11]-'0';
        }
        else {
            det = ((cstr[11]-'0')*10)+volNameOver9 ;
        }
    }
    else { // OVER 100
        volNameOver9 = cstr[13]-'0';
        if(volNameOver9 == 47) {
            det = cstr[12]-'0';
        }
        else {
            det = ((cstr[12]-'0')*10)+volNameOver9 ;
        }
    }
    //G4cout << "Stepping Action :: Found electron ekin in " << volname << " det = " << det << G4endl;
}

G4String SteppingAction::G4intToG4String(G4int value)
{
  G4String theString;
  G4String output = "00";
  std::stringstream out;
  out << value;
  theString = out.str();

  if(value < 10) {
      output.replace(1,1,theString);
  }
  else {
      output = theString;
  }

  return output;
}

G4String SteppingAction::GetCrystalColour(G4int value)
{
  G4String output = "X";
  if(value == 1) {
      output = "B";
  }
  else if(value == 2) {
      output = "G";
  }
  else if(value == 3) {
      output = "R";
  }
  else if(value == 4) {
      output = "W";
  }
  return output;
}
