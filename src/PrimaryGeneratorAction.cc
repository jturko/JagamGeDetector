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
/// \file analysis/shared/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
//
// $Id: PrimaryGeneratorAction.cc 68015 2013-03-13 13:27:27Z gcosmo $
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4Geantino.hh"
#include "Randomize.hh"

#include "G4GeneralParticleSource.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC)
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fDetector(DC)
{
    fUseGPS = true;
    fGPS = new G4GeneralParticleSource();

    G4int n_particle = 1;
    fParticleGun  = new G4ParticleGun(n_particle);
    //create a messenger for this class
    fGunMessenger = new PrimaryGeneratorMessenger(this);

    fParticleGun->SetParticleEnergy(0*eV);
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

    // defaults
    numberOfDecayingLaBrDetectors = 0;
    effEnergy = 0.0;
    effDirectionBool = false;
    effPositionBool = false;
    effParticleBool = false;
    effDirection = G4ThreeVector(0.0*mm,0.0*mm,0.0*mm);

    effBOMABBool = false;
    effBOMABPosition = G4ThreeVector(0.0*cm,0.0*cm,0.0*cm);

    // LaBr properties
    G4double triangleThetaAngle = 54.735610317245360*deg;
    // theta
    this->detectorAnglesLaBr3[0][0] 	= triangleThetaAngle;
    this->detectorAnglesLaBr3[1][0] 	= triangleThetaAngle;
    this->detectorAnglesLaBr3[2][0] 	= triangleThetaAngle;
    this->detectorAnglesLaBr3[3][0] 	= triangleThetaAngle;
    this->detectorAnglesLaBr3[4][0] 	= 180.0*deg - triangleThetaAngle;
    this->detectorAnglesLaBr3[5][0] 	= 180.0*deg - triangleThetaAngle;
    this->detectorAnglesLaBr3[6][0] 	= 180.0*deg - triangleThetaAngle;
    this->detectorAnglesLaBr3[7][0] 	= 180.0*deg - triangleThetaAngle;
    // phi
    this->detectorAnglesLaBr3[0][1] 	= 22.5*deg;
    this->detectorAnglesLaBr3[1][1] 	= 112.5*deg;
    this->detectorAnglesLaBr3[2][1] 	= 202.5*deg;
    this->detectorAnglesLaBr3[3][1] 	= 292.5*deg;
    this->detectorAnglesLaBr3[4][1] 	= 22.5*deg;
    this->detectorAnglesLaBr3[5][1] 	= 112.5*deg;
    this->detectorAnglesLaBr3[6][1] 	= 202.5*deg;
    this->detectorAnglesLaBr3[7][1] 	= 292.5*deg;
    // yaw (alpha)
    this->detectorAnglesLaBr3[0][2] 	= 0.0*deg;
    this->detectorAnglesLaBr3[1][2] 	= 0.0*deg;
    this->detectorAnglesLaBr3[2][2] 	= 0.0*deg;
    this->detectorAnglesLaBr3[3][2] 	= 0.0*deg;
    this->detectorAnglesLaBr3[4][2] 	= 0.0*deg;
    this->detectorAnglesLaBr3[5][2] 	= 0.0*deg;
    this->detectorAnglesLaBr3[6][2] 	= 0.0*deg;
    this->detectorAnglesLaBr3[7][2] 	= 0.0*deg;
    // pitch (beta)
    this->detectorAnglesLaBr3[0][3] 	= triangleThetaAngle;
    this->detectorAnglesLaBr3[1][3] 	= triangleThetaAngle;
    this->detectorAnglesLaBr3[2][3] 	= triangleThetaAngle;
    this->detectorAnglesLaBr3[3][3] 	= triangleThetaAngle;
    this->detectorAnglesLaBr3[4][3] 	= 180.0*deg - triangleThetaAngle;
    this->detectorAnglesLaBr3[5][3] 	= 180.0*deg - triangleThetaAngle;
    this->detectorAnglesLaBr3[6][3] 	= 180.0*deg - triangleThetaAngle;
    this->detectorAnglesLaBr3[7][3] 	= 180.0*deg - triangleThetaAngle;
    // roll (gamma)
    this->detectorAnglesLaBr3[0][4] 	= 22.5*deg;
    this->detectorAnglesLaBr3[1][4] 	= 112.5*deg;
    this->detectorAnglesLaBr3[2][4] 	= 202.5*deg;
    this->detectorAnglesLaBr3[3][4] 	= 292.5*deg;
    this->detectorAnglesLaBr3[4][4] 	= 22.5*deg;
    this->detectorAnglesLaBr3[5][4] 	= 112.5*deg;
    this->detectorAnglesLaBr3[6][4] 	= 202.5*deg;
    this->detectorAnglesLaBr3[7][4] 	= 292.5*deg;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
  delete fGunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    G4ThreeVector thiseffposition = G4ThreeVector(0.0*mm,0.0*mm,0.0*mm);

    if(numberOfDecayingLaBrDetectors != 0) {
        G4cout << "Decaying LaBr3 (wtf?)" << G4endl;
        G4double crystalRadius    = 2.54*cm;
        G4double crystalLength    = 5.08*cm;
        // The detector material material is LaBr3:Ce. 95% is LaBr3 and 5% is Ce.
        // Of the 95% that is LaBr3, there is one atom of La for every 3 of Br.
        // La has a molar mass of 138.9055*g/mole,
        // Br has a molar mass of 79.904*g/mole,
        // Ce has a molar mass of 140.116*g/mole.

        // We need to figure out how atoms of La are in our simulation.
        // Of the total number of La atoms, 0.08881(71)% are 139La and are radioative.
        // Calculated activity = 146.82908820569 Bq
        G4double prob, sumProb;
        G4int detnumber;

        // Choose random detector
        prob = 1.0/((G4double)(numberOfDecayingLaBrDetectors));
        sumProb = 0.0;
        G4double random_det = G4UniformRand();
        for( G4int j = 0 ; j < numberOfDecayingLaBrDetectors ; j++ ) {  // get the number of particles in decay and loop over them
            sumProb = sumProb + prob;
            if(random_det <= sumProb ) {
                detnumber = j;
                break;
            }
        }

        // Choose energy
        // 33.6% Beta Decay 788.742 keV Gamma-ray and 66.4% EC Decay 1435.795 keV Gamma-Ray
        prob = 0.336;
        sumProb = 0;
        G4double thisenergy;
        G4double random_energy = G4UniformRand();

        if(random_energy <= prob ) {
            thisenergy = 788.742*keV;
        }
        else {
            thisenergy = 1435.795*keV;
        }

        G4double detTheta       = this->detectorAnglesLaBr3[detnumber][0];
        G4double detPhi         = this->detectorAnglesLaBr3[detnumber][1];
        G4double detRadialPos   = 12.82385*cm;

        G4double random_z   = crystalLength*G4UniformRand() + detRadialPos;
        G4double random_phi = 2.0*(360.*deg)*G4UniformRand();
        G4double random_r   = crystalRadius*pow(G4UniformRand(),0.5);

        G4double x2=random_r*sin(random_phi);
        G4double y2=random_r*cos(random_phi);
        G4double z2=random_z;

        G4ThreeVector pos2 = G4ThreeVector(x2,y2,z2);
        pos2.rotateY(M_PI);
        pos2.rotateY(M_PI+this->detectorAnglesLaBr3[detnumber][3]);
        pos2.rotateZ(this->detectorAnglesLaBr3[detnumber][4]);

        //cart
        G4double x = pos2.x();
        G4double y = pos2.y();
        G4double z = pos2.z();

        G4ThreeVector thisposition(transX(x,y,z,detTheta,detPhi), transY(x,y,z,detTheta,detPhi), transZ(x,y,z,detTheta,detPhi));

        G4ParticleDefinition* agamma = G4ParticleTable::GetParticleTable()->FindParticle("gamma");

        // random direction
        G4double randcostheta = 2.*G4UniformRand()-1.0;
        G4double randsintheta = sqrt( 1. - randcostheta*randcostheta );
        G4double randphi      = (360.*deg)*G4UniformRand();
        G4ThreeVector thisdirection = G4ThreeVector(randsintheta*cos(randphi), randsintheta*sin(randphi), randcostheta);

        fParticleGun->SetParticleDefinition(agamma);
        fParticleGun->SetParticlePosition(thisposition);
        fParticleGun->SetParticleMomentumDirection(thisdirection);
        fParticleGun->SetParticleEnergy(thisenergy);
    } // end of decayig LaBr3 detectors
    else if(effEnergy != 0.0) {
        G4ParticleDefinition* effpart;
        if(effParticleBool) {

            if(effParticle.contains('electron') || effParticle.contains('e-') ) {
                effpart = G4ParticleTable::GetParticleTable()->FindParticle("e-");
            }
            else {
                effpart = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
            }
        }
        else {
            effpart = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
        }

        if(effPositionBool) thiseffposition = effPosition;

        G4double effrandcostheta, effrandsintheta, effrandphi;
        G4ThreeVector effdirection;
        if(effDirectionBool) {
            //G4cout << "effDirectionBool is true" << G4endl;
            effdirection = effDirection;
        }
        else {
            //G4cout << "effDirectionBool is false" << G4endl;
            if(effBOMABBool) {
                //G4cout << "effBOMABBool is true, were doing some calculations..." << G4endl;
                G4double head_dx = 13.5*cm;
                G4double head_dy = 18.5*cm;
                G4double head_dz = 19.5*cm;

                G4double thorax_dx = 28.0*cm;
                G4double thorax_dy = 19.5*cm;
                G4double thorax_dz = 39.2*cm;

                G4double abdomen_dx = 34.0*cm;
                G4double abdomen_dy = 19.5*cm;
                G4double abdomen_dz = 19.0*cm;

                G4double neck_dx = 12.0*cm;
                G4double neck_dz = 9.5*cm;

                G4double arms_dx = 12.0*cm;
                G4double arms_dz = 58.0*cm;

                G4double thighs_dx = 14.5*cm;
                G4double thighs_dz = 38.5*cm;

                G4double legs_dx = 12.0*cm;
                G4double legs_dz = 38.5*cm;

                G4double head_vol = M_PI*(head_dx/2.0)*(head_dy/2.0)*(head_dz);
                G4double thorax_vol = M_PI*(thorax_dx/2.0)*(thorax_dy/2.0)*(thorax_dz);
                G4double abdomen_vol = M_PI*(abdomen_dx/2.0)*(abdomen_dy/2.0)*(abdomen_dz);
                G4double neck_vol = M_PI*(neck_dx/2.0)*(neck_dx/2.0)*(neck_dz);
                G4double arms_vol = 2.0*(M_PI*(arms_dx/2.0)*(arms_dx/2.0)*(arms_dz));  // two arms
                G4double thighs_vol = 2.0*(M_PI*(thighs_dx/2.0)*(thighs_dx/2.0)*(thighs_dz)); // two thighs
                G4double legs_vol = 2.0*(M_PI*(legs_dx/2.0)*(legs_dx/2.0)*(legs_dz)); // two legs

                G4double total_vol = head_vol + thorax_vol + abdomen_vol + neck_vol + arms_vol + thighs_vol + legs_vol;

                G4double randomNumber = G4UniformRand();
                G4double a,b,c; // a is on x-axis, b is on y-axis, c is left or right side
                G4double side_dx;

                G4ThreeVector coord = G4ThreeVector(0.0*cm,0.0*cm,0.0*cm);
                G4ThreeVector leftRightSide = G4ThreeVector(0.0*cm,0.0*cm,0.0*cm);

                if(randomNumber >= 0 && randomNumber < ((head_vol)/total_vol) ) {
                  // decay in head_vol - elliptic cylinder
                  a = head_dx/2.0;
                  b = head_dy/2.0;
                  coord = MonteCarloAreaOfEllipse(a, b);
                  coord.setZ((thorax_dz/2.0)+neck_dz+(G4UniformRand()*head_dz));
                  thiseffposition = coord + effBOMABPosition;
                }
                else if(randomNumber >= ((head_vol)/total_vol) && randomNumber < ((head_vol+thorax_vol)/total_vol) ) {
                  // decay in thorax_vol - elliptic cylinder
                  a = thorax_dx/2.0;
                  b = thorax_dy/2.0;
                  coord = MonteCarloAreaOfEllipse(a, b);
                  coord.setZ((-1.0*(thorax_dz/2.0)) + (G4UniformRand()*thorax_dz) );
                  thiseffposition = coord + effBOMABPosition;
                }
                else if(randomNumber >= ((head_vol+thorax_vol)/total_vol) && randomNumber < ((head_vol+thorax_vol+abdomen_vol)/total_vol) ) {
                  // decay in abdomen_vol - elliptic cylinder
                   a = abdomen_dx/2.0;
                   b = abdomen_dy/2.0;
                   coord = MonteCarloAreaOfEllipse(a, b);
                   coord.setZ((-1.0*((thorax_dz/2.0)+(abdomen_dz))) + (G4UniformRand()*abdomen_dz) );
                   thiseffposition = coord + effBOMABPosition;
                }
                else if(randomNumber >= ((head_vol+thorax_vol+abdomen_vol)/total_vol) && randomNumber < ((head_vol+thorax_vol+abdomen_vol+neck_vol)/total_vol) ) {
                  // decay in neck_vol - cylinder
                  a = neck_dx/2.0;
                  coord = MonteCarloAreaOfCircle(a);
                  coord.setZ( (thorax_dz/2.0) + (G4UniformRand()*neck_dz) );
                  thiseffposition = coord + effBOMABPosition;
                }
                else if(randomNumber >= ((head_vol+thorax_vol+abdomen_vol+neck_vol)/total_vol) && randomNumber < ((head_vol+thorax_vol+abdomen_vol+neck_vol+arms_vol)/total_vol) ) {
                  // decay in arms_vol - cylinders
                  a = arms_dx/2.0;
                  coord = MonteCarloAreaOfCircle(a);
                  coord.setZ( (thorax_dz/2.0 - arms_dz) + (G4UniformRand()*arms_dz) );

                  if(G4UniformRand() < 0.5 ){ // right arm
                    c = 1;
                  }
                  else { // left arm
                    c = -1;
                  }
                  if( arms_dz > thorax_dz && abdomen_dx > thorax_dx ) {
                      side_dx = abdomen_dx;
                  }
                  else {
                      side_dx = thorax_dx;
                  }
                  leftRightSide.setX(c*((side_dx + arms_dx)/2.0));
                  thiseffposition = coord + effBOMABPosition + leftRightSide;
                }
                else if(randomNumber >= ((head_vol+thorax_vol+abdomen_vol+neck_vol+arms_vol)/total_vol) && randomNumber < ((head_vol+thorax_vol+abdomen_vol+neck_vol+arms_vol+thighs_vol)/total_vol) ) {
                  // decay in thighs_vol - cylinders
                  a = thighs_dx/2.0;
                  coord = MonteCarloAreaOfCircle(a);
                  coord.setZ( (-1.0*(abdomen_dz + (thorax_dz/2.0 + thighs_dz))) + (G4UniformRand()*thighs_dz) );

                  if(G4UniformRand() < 0.5 ){ // right
                    c = 1;
                  }
                  else { // left
                    c = -1;
                  }
                  leftRightSide.setX(c*((abdomen_dx - thighs_dx)/2.0));
                  thiseffposition = coord + effBOMABPosition + leftRightSide;
                }
                else if(randomNumber >= ((head_vol+thorax_vol+abdomen_vol+neck_vol+arms_vol+thighs_vol)/total_vol) && randomNumber < ((head_vol+thorax_vol+abdomen_vol+neck_vol+arms_vol+thighs_vol+legs_vol)/total_vol) ) {
                  // decay in legs_vol - cylinders
                  a = legs_dx/2.0;
                  coord = MonteCarloAreaOfCircle(a);
                  coord.setZ( (-1.0*(abdomen_dz + thighs_dz + (thorax_dz/2.0 + legs_dz))) + (G4UniformRand()*legs_dz) );

                  if(G4UniformRand() < 0.5 ){ // right
                    c = 1;
                  }
                  else { // left
                    c = -1;
                  }
                  leftRightSide.setX(c*((abdomen_dx - thighs_dx)/2.0));
                  thiseffposition = coord + effBOMABPosition + leftRightSide;
                }
                else {
                  // decay nowhere
                  G4cout << "BOMAB: Couldn't find a volume to decay from!" << G4endl;
                }

                // random direction
                effrandcostheta = 2.*G4UniformRand()-1.0;
                effrandsintheta = sqrt( 1. - effrandcostheta*effrandcostheta );
                effrandphi      = (360.*deg)*G4UniformRand();
                effdirection = G4ThreeVector(effrandsintheta*cos(effrandphi), effrandsintheta*sin(effrandphi), effrandcostheta);
            }
            else {
                // random direction
                effrandcostheta = 2.*G4UniformRand()-1.0;
                effrandsintheta = sqrt( 1. - effrandcostheta*effrandcostheta );
                effrandphi      = (360.*deg)*G4UniformRand();
                effdirection = G4ThreeVector(effrandsintheta*cos(effrandphi), effrandsintheta*sin(effrandphi), effrandcostheta);
            }
        }

        fParticleGun->SetParticleDefinition(effpart);
        fParticleGun->SetParticlePosition(thiseffposition);
        fParticleGun->SetParticleMomentumDirection(effdirection);
        fParticleGun->SetParticleEnergy(effEnergy);
    }
    else if (fParticleGun->GetParticleDefinition() == G4Geantino::Geantino()) {
//        G4int Z = 10, A = 24;
//        G4double ionCharge   = 0.*eplus;
//        G4double excitEnergy = 0.*keV;

//        G4ParticleDefinition* ion = G4ParticleTable::GetParticleTable()->GetIon(Z,A,excitEnergy);
//        fParticleGun->SetParticleDefinition(ion);
//        fParticleGun->SetParticleCharge(ionCharge);
    }

  //create vertex
  //
  if(fUseGPS && !effBOMABBool) {
        //G4cout << "USING GPS AS PRIMARY GENERATOR" << G4endl;
        fGPS->GeneratePrimaryVertex(anEvent);
  }
  else {          
        //G4cout << "USING ParticleGun AS PRIMARY GENERATOR (maybe BOMAB?)" << G4endl;
        fParticleGun->GeneratePrimaryVertex(anEvent);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double PrimaryGeneratorAction::transX(G4double x, G4double y, G4double z, G4double theta, G4double phi){
  return ( pow(x*x+y*y+z*z,0.5)*sin(theta)*cos(phi) );
}

G4double PrimaryGeneratorAction::transY(G4double x, G4double y, G4double z, G4double theta, G4double phi){
  return ( pow(x*x+y*y+z*z,0.5)*sin(theta)*sin(phi) );
}

G4double PrimaryGeneratorAction::transZ(G4double x, G4double y, G4double z, G4double theta, G4double phi){
  return ( pow(x*x+y*y+z*z,0.5)*cos(theta) );
}

G4ThreeVector PrimaryGeneratorAction::MonteCarloAreaOfCircle( G4double radius ){
    G4double rand1 = G4UniformRand();
    G4double rand2 = G4UniformRand();
    G4double randbuff;

    if(rand2 < rand1) {
       randbuff = rand1;
       rand1 = rand2;
       rand2 = randbuff;
    }

    G4ThreeVector result = G4ThreeVector(rand2*radius*cos(2*M_PI*(rand1/rand2)), rand2*radius*sin(2*M_PI*(rand1/rand2)), 0.0*cm);
    return result;
}

G4ThreeVector PrimaryGeneratorAction::MonteCarloAreaOfEllipse( G4double a, G4double b ){ // a is on x-axis
    G4bool foundPoint = false;

    G4double rand1, rand2, randbuff, largerSide, xpoint, ypoint, ytest;

    if( a > b ) {
      largerSide = a;
    }
    else {
      largerSide = b;
    }

    while(!foundPoint) {
        rand1 = G4UniformRand();
        rand2 = G4UniformRand();

        if(rand2 < rand1) {
           randbuff = rand1;
           rand1 = rand2;
           rand2 = randbuff;
        }

        xpoint = rand2*largerSide*cos(2*M_PI*(rand1/rand2));
        ypoint = rand2*largerSide*sin(2*M_PI*(rand1/rand2));

        ytest = fabs(sqrt((b*b)*(1-((xpoint*xpoint)/(a*a)))));

        if(fabs(ypoint) < ytest) {
          foundPoint = true;
        }
        //else outside ellipse, redo!
    }

    G4ThreeVector result = G4ThreeVector(xpoint, ypoint, 0.0*cm);
    return result;
}
