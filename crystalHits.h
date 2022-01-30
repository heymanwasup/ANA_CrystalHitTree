//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Sep 14 10:36:33 2021 by ROOT version 6.24/00
// from TTree crystalHits/crystalHits
// found on file: ../Data/crystalHitTree.root
//////////////////////////////////////////////////////////

#ifndef crystalHits_h
#define crystalHits_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <string>
#include "SimpleHistSVC.h"


// Header file for the classes stored in the TTree if any.

class crystalHits {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain
   std::string     method_name; //!
   bool            useStatusCut; //!
   TDirectory          *output_file; //![]
   SimpleHistSVC  *histSvc; //!
   double          start_time; //!
   double          end_time; //!
   double          offset_time; //!
   bool            isXtalHitTree; //!
   int             timeTag; //!

   


// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          runStartUnixTimeSeconds;
   Double_t        energy;
   Double_t        time;
   Double_t        chi2;
   Int_t           status;
   Double_t        pedestal;
   UInt_t          caloNum;
   UInt_t          xtalNum;
   Int_t           islandNum;
   Bool_t          laserHit;
   UInt_t          waveformIndex;
   UInt_t          eventNum;
   UInt_t          subRunNum;
   UInt_t          runNum;

   // List of branches
   TBranch        *b_runStartUnixTimeSeconds;   //!
   TBranch        *b_energy;   //!
   TBranch        *b_time;   //!
   TBranch        *b_chi2;   //!
   TBranch        *b_status;   //!
   TBranch        *b_pedestal;   //!
   TBranch        *b_caloNum;   //!
   TBranch        *b_xtalNum;   //!
   TBranch        *b_islandNum;   //!
   TBranch        *b_laserHit;   //!
   TBranch        *b_waveformIndex;   //!
   TBranch        *b_eventNum;   //!
   TBranch        *b_subRunNum;   //!
   TBranch        *b_runNum;   //!

   crystalHits(std::string name);   
   virtual ~crystalHits();   
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(int entries_debug=-1);
      
   virtual void     ChangeFile(TTree *tree, TDirectory *file);
   virtual void     WriteToFile();
   
   virtual bool     Cut_laserHit();
   virtual void     AnaCrystalHits();
   virtual void     AnaClusteredHits();

};

#endif



