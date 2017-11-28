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
/// \file analysis/shared/include/EventAction.hh
/// \brief Definition of the EventAction class
//
//
// $Id: EventAction.hh 68015 2013-03-13 13:27:27Z gcosmo $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "HistoManager.hh"

class RunAction;
class HistoManager;

static const int MAXSTEPS = 1000;
static const int MAXHITS = 100;
static const int NUMSTEPVARS = 14;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*, HistoManager*);
  virtual ~EventAction();

  virtual void  BeginOfEventAction(const G4Event*);
  virtual void    EndOfEventAction(const G4Event*);
    
  void AddAbs(G4double de, G4double dl) {fEnergyAbs += de; fTrackLAbs += dl;};
  void AddGap(G4double de, G4double dl) {fEnergyGap += de; fTrackLGap += dl;};

  //
  G4int GetEventNumber(){return evtNb;};

//  void AddStepTracker(G4double eventNumber, G4double stepNumber, G4String volume, G4double cryNumber, G4double detNumber,
//  						G4double depEnergy, G4double posx, G4double posy, G4double posz, G4double time,
//  						G4double initialDirectionX, G4double initialDirectionY, G4double initialDirectionZ,
//  						G4double initialEnergy, G4int trackID) {

//					  if(histoManager->GetStepTrackerBool())   {
//						  	stepTracker[0][stepIndex] = eventNumber;
//						  	stepTracker[1][stepIndex] = stepNumber;
//						  	stepTracker[2][stepIndex] = cryNumber;
//						  	stepTracker[3][stepIndex] = detNumber;
//						  	stepTracker[4][stepIndex] = depEnergy;
//						  	stepTracker[5][stepIndex] = posx;
//						  	stepTracker[6][stepIndex] = posy;
//						  	stepTracker[7][stepIndex] = posz;
//						  	stepTracker[8][stepIndex] = time;
//						  	stepTracker[9][stepIndex] = initialDirectionX;
//							stepTracker[10][stepIndex] = initialDirectionY;
//							stepTracker[11][stepIndex] = initialDirectionZ;
//						  	stepTracker[12][stepIndex] = initialEnergy;
//						  	stepTracker[13][stepIndex] = trackID;
//						  	stepVolume[stepIndex] = volume ;
//						  	stepIndex++;
//						  	if(stepIndex == MAXSTEPS) 	{
//						  		G4cout << "\n ----> error 13423549 \n" << G4endl;
//						  		exit(1);
//						  		}
//						  };
// 	};

  // particle types
  void AddParticleType(G4int index) {particleTypes[index] += 1;};

  void AddLaBrCrystDet(G4double de, G4double dl, G4int det) {LaBrCrystEnergyDet[det] += de; LaBrCrystTrackDet[det] += dl;} ;

  void AddHitTracker(G4String mnemonic, G4int eventNumber, G4int trackID, G4int parentID, G4int stepNumber, G4int particleType, G4int processType, G4int systemID, G4int cryNumber, G4int detNumber, G4double depEnergy, G4double posx, G4double posy, G4double posz, G4double time);

  void AddGermaniumCylinderCrystDet(G4double de, G4double dl, G4double ke, G4int det) {GermaniumCylinderCrystEnergyDet[det] += de; GermaniumCylinderCrystTrackDet[det] += dl; if(GermaniumCylinderCrystEKinDet[det] < ke) GermaniumCylinderCrystEKinDet[det] = ke;};

  // Grid kinetic energy of gammas and electrons
  void SetGridEKinElectronDet(G4double de, G4double dl, G4int det) { if(gridEKinElectronDet[det] < de) gridEKinElectronDet[det] = de; gridTrackElectronDet[det] += dl;};
  void SetGridEKinGammaDet(G4double de, G4double dl, G4int det) { if(gridEKinGammaDet[det] < de) gridEKinGammaDet[det] = de; gridTrackGammaDet[det] += dl;};

private:
   RunAction*    fRunAct;
   HistoManager* fHistoManager;

      
   G4double  fEnergyAbs, fEnergyGap;
   G4double  fTrackLAbs, fTrackLGap;
                     
   G4int     fPrintModulo;
   G4int     evtNb;

   void ClearVariables();
   void FillParticleType();
   void FillLaBrCryst() ;
   void FillGermaniumCylinderCryst();
   void FillGridEkin();


   // tracking info
   G4double stepTracker[NUMSTEPVARS][MAXSTEPS];

   G4String stepVolume[MAXSTEPS]; // volume at each step
   G4int    stepIndex;

   G4int    hitTrackerI[NUMSTEPVARS][MAXHITS];
   G4double hitTrackerD[NUMSTEPVARS][MAXHITS];
   G4int    hitIndex;
   G4int    pTrackID;
   G4int    pParentID;

   G4int    numberOfHits;
   G4String pHitMnemonic[MAXHITS];


   // Particle types in simulation
   G4int particleTypes[NUMPARTICLETYPES];

   G4double LaBrCrystEnergyDet[MAXNUMDET] ;
   G4double LaBrCrystTrackDet[MAXNUMDET] ;

   G4double GermaniumCylinderCrystEnergyDet[MAXNUMDET];
   G4double GermaniumCylinderCrystTrackDet[MAXNUMDET];
   G4double GermaniumCylinderCrystEKinDet[MAXNUMDET];

   // Grid kinetic energy / track length of gamma and electon
   G4double gridEKinElectronDet[MAXNUMDET];
   G4double gridTrackElectronDet[MAXNUMDET];
   G4double gridEKinGammaDet[MAXNUMDET];
   G4double gridTrackGammaDet[MAXNUMDET];


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
