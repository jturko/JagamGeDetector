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
/// \file analysis/shared/src/EventAction.cc
/// \brief Implementation of the EventAction class
//
//
// $Id: EventAction.cc 68015 2013-03-13 13:27:27Z gcosmo $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"

#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Event.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* run, HistoManager* histo)
:G4UserEventAction(),
 fRunAct(run),fHistoManager(histo),
 fEnergyAbs(0.), fEnergyGap(0.),
 fTrackLAbs(0.), fTrackLGap(0.),
 fPrintModulo(0)                             
{
 fPrintModulo = 1000; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{  
  evtNb = evt->GetEventID();
  if (evtNb%fPrintModulo == 0)
//    G4cout << "\n---> Begin of event: " << evtNb << G4endl;
    if(evtNb < 1e6) { printf( " ---> Ev.# %dk\r", evtNb/1000); }
    else            { printf( " ---> Ev.# %.3fm\r", double(evtNb)/1000000.); }
    G4cout.flush();

 // initialisation per event
 fEnergyAbs = fEnergyGap = 0.;
 fTrackLAbs = fTrackLGap = 0.;

 ClearVariables();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
//    FillParticleType() ;
    FillLaBrCryst() ;
    FillGermaniumCylinderCryst();
    FillGridEkin();

    //G4cout << "numberOfHits = " << numberOfHits << G4endl;
    for (G4int i = 0 ; i < numberOfHits; i++) {
        fHistoManager->FillHitNtuple(hitTrackerI[0][i], hitTrackerI[1][i], hitTrackerI[2][i], hitTrackerI[3][i],  hitTrackerI[4][i], hitTrackerI[5][i], hitTrackerI[6][i], hitTrackerI[7][i], hitTrackerI[8][i], hitTrackerD[0][i]/keV, hitTrackerD[1][i]/mm, hitTrackerD[2][i]/mm, hitTrackerD[3][i]/mm, hitTrackerD[4][i]/second);
    }

    ClearVariables();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




void EventAction::ClearVariables()
{
  if(fHistoManager->GetStepTrackerBool()) {
      stepIndex = 0;
      for (G4int i = 0 ; i < MAXSTEPS; i++) {
        for (G4int j = 0 ; j < NUMSTEPVARS; j++) {
            stepTracker[j][i] = 0;
        }
      }
  }

  if(fHistoManager->GetHitTrackerBool()) {
      hitIndex = 0;
      numberOfHits = 0;
      pTrackID = -1;
      pParentID = -1;

      for (G4int i = 0 ; i < MAXHITS; i++) {
        pHitMnemonic[i] = "XXX00XX00X";
        for (G4int j = 0 ; j < NUMSTEPVARS; j++) {
            hitTrackerI[j][i] = 0;
            hitTrackerD[j][i] = 0.0;
        }
      }
  }

  for (G4int i = 0 ; i < NUMPARTICLETYPES; i++) {
      particleTypes[i]                  = 0;
  }
  for (G4int i = 0 ; i < MAXNUMDET; i++) {
      LaBrCrystEnergyDet[i]  = 0 ;
      LaBrCrystTrackDet[i]   = 0 ;
      GermaniumCylinderCrystEnergyDet[i]= 0;
      GermaniumCylinderCrystTrackDet[i]= 0;
      GermaniumCylinderCrystEKinDet[i] = 0;
      gridEKinGammaDet[i]               = 0;
      gridTrackGammaDet[i]              = 0;
      gridEKinElectronDet[i]            = 0;
      gridTrackElectronDet[i]           = 0;
  }
}


void EventAction::FillParticleType()
{
    G4int numParticleTypes = 0;
    for (G4int i = 0 ; i < NUMPARTICLETYPES; i++)
          {
            if (particleTypes[i] != 0)
                  { // if particle type 'i' has non-zero counts
                    for (G4int j = 0 ; j< particleTypes[i]; j++)
                          { // loop over the number of time we saw it
                            G4cout << "particleTypes[" << i << "] = " << particleTypes[i] << G4endl;
                            fHistoManager->FillHisto(astats_particle_type_in_each_step, i);
                          }
                  }
          }

    // Fill the number of particle types in the event
    for (G4int i = 0 ; i < NUMPARTICLETYPES; i++)
      {
        if (particleTypes[i] != 0)
            numParticleTypes++;
      }
    fHistoManager->FillHisto(astats_particle_type_in_each_event, numParticleTypes);
}

void EventAction::FillLaBrCryst()
{
    G4double energySumDet = 0;
    for (G4int j=0; j < MAXNUMDET; j++) {
        if(LaBrCrystEnergyDet[j] > MINENERGYTHRES) {
            if(WRITEEDEPHISTOS)     fHistoManager->FillHisto(labr_crystal_edep, LaBrCrystEnergyDet[j]);
            if(WRITEEDEPHISTOS)     fHistoManager->FillHisto(labr_crystal_edep_det0+j, LaBrCrystEnergyDet[j]);
            energySumDet += LaBrCrystEnergyDet[j];
        }
    }
    if(energySumDet > MINENERGYTHRES) {
        if(WRITEEDEPHISTOS)     fHistoManager->FillHisto(labr_crystal_edep_sum, energySumDet);
    }
}


void EventAction::FillGermaniumCylinderCryst()
{
    G4double  energySum = 0, trackSum = 0;
    for (G4int i=0; i < MAXNUMDET; i++) {
      if(GermaniumCylinderCrystEnergyDet[i] > MINENERGYTHRES) {
        // fill energies in each detector
        if(WRITEEDEPHISTOS)   fHistoManager->FillHisto(germanium_cylinder_crystal_edep_det0+i, GermaniumCylinderCrystEnergyDet[i]);
        //if(WRITETRACKLHISTOS) fHistoManager->FillHisto(germanium_cylinder_crystal_trackl_det0+i, germanium_cylinderCrystTrackDet[i]);
        // fill standard energy and track spectra
        if(WRITEEDEPHISTOS)   fHistoManager->FillHisto(germanium_cylinder_crystal_edep, GermaniumCylinderCrystEnergyDet[i]);
        //if(WRITETRACKLHISTOS) fHistoManager->FillHisto(germanium_cylinder_crystal_trackl, germanium_cylinderCrystTrackDet[i]);
        // add sum energies
        energySum    += GermaniumCylinderCrystEnergyDet[i];
        trackSum     += GermaniumCylinderCrystTrackDet[i];
      }
      if(GermaniumCylinderCrystEKinDet[i] > MINENERGYTHRES) {
        // fill energies in each detector
        if(WRITEEKINHISTOS)   fHistoManager->FillHisto(germanium_cylinder_crystal_ekin_det0+i, GermaniumCylinderCrystEKinDet[i]);
      }
    }
    if(energySum > MINENERGYTHRES) {
      if(WRITEEDEPHISTOS)     fHistoManager->FillHisto(germanium_cylinder_crystal_edep_sum, energySum);
      //if(WRITETRACKLHISTOS)   fHistoManager->FillHisto(germanium_cylinder_crystal_trackl_sum, trackSum);
    }


}


void EventAction::FillGridEkin()
{
    for (G4int i=0; i < MAXNUMDET; i++) {
      if(gridEKinGammaDet[i] > MINENERGYTHRES) {
        // fill energies in each detector
        if(WRITEEKINHISTOS)   fHistoManager->FillHisto(gridcell_gamma_ekin_det0+i, gridEKinGammaDet[i]);
        if(WRITETRACKLHISTOS) fHistoManager->FillHisto(gridcell_gamma_trackl_det0+i, gridTrackGammaDet[i]);
      }
    }
    for (G4int i=0; i < MAXNUMDET; i++) {
      if(gridEKinElectronDet[i] > MINENERGYTHRES) {
        // fill energies in each detector
        if(WRITEEKINHISTOS)   fHistoManager->FillHisto(gridcell_electron_ekin_det0+i, gridEKinElectronDet[i]);
        if(WRITETRACKLHISTOS) fHistoManager->FillHisto(gridcell_electron_trackl_det0+i, gridTrackElectronDet[i]);
      }
    }
}
void EventAction::AddHitTracker(G4String mnemonic, G4int eventNumber, G4int trackID, G4int parentID, G4int stepNumber, G4int particleType, G4int processType, G4int systemID, G4int cryNumber, G4int detNumber, G4double depEnergy, G4double posx, G4double posy, G4double posz, G4double time)
{
    G4bool newhit = true;
    for (G4int i = 0 ; i < numberOfHits; i++) {
        if(pHitMnemonic[i] == mnemonic) {
            // sum the new enery
            hitTrackerD[0][i] = hitTrackerD[0][i] + depEnergy;
            newhit = false;
            break;
        }
    }
    if (newhit) { // new hit
        pHitMnemonic[hitIndex] = mnemonic;
        pTrackID = trackID;
        pParentID = parentID;
        hitTrackerI[0][hitIndex] = eventNumber;
        hitTrackerI[1][hitIndex] = trackID;
        hitTrackerI[2][hitIndex] = parentID;
        hitTrackerI[3][hitIndex] = stepNumber;
        hitTrackerI[4][hitIndex] = particleType;
        hitTrackerI[5][hitIndex] = processType;
        hitTrackerI[6][hitIndex] = systemID;
        hitTrackerI[7][hitIndex] = cryNumber;
        hitTrackerI[8][hitIndex] = detNumber;
        hitTrackerD[0][hitIndex] = depEnergy;
        hitTrackerD[1][hitIndex] = posx;
        hitTrackerD[2][hitIndex] = posy;
        hitTrackerD[3][hitIndex] = posz;
        hitTrackerD[4][hitIndex] = time;

        hitIndex++;
        numberOfHits = hitIndex;

        if(numberOfHits >= MAXHITS) {
            G4cout << "ERROR! Too many hits!" << G4endl;
        }
    }
}

