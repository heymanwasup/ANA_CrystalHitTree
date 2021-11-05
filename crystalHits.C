#include "crystalHits.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
void crystalHits::ChangeFile(TTree * tree, TFile * file){
   fChain = tree;
   output_file = file;   
   Init(tree);
   // std::cout <<"after init" <<std::endl;
   histSvc->BookFile(file);
   // std::cout <<"after BookFile" <<std::endl;
}

void crystalHits::WriteToFile() {
   histSvc->Write();
   // delete histSvc;
}

void crystalHits::Loop()
{
   if (fChain == 0) return;   
   
   Long64_t nentries = fChain->GetEntriesFast();      
   
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      
      Long64_t ientry = LoadTree(jentry);
      
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if (jentry%100000==0) std::cout << "processed " << jentry << " events" << std::endl;
      
      std::string calo_tag = std::to_string(caloNum);
      std::string xstal_tag = std::to_string(xtalNum);
      //overall            
      histSvc->SetProcessTag(std::string("hist"));
      FillHists();

      //set calorimeter info
      histSvc->SetProcessTag(std::string("hist_calo") + calo_tag);
      FillHists();

      //set calorimeter and xtal info
      histSvc->SetProcessTag(std::string("hist_calo") + calo_tag + std::string("_xtal") + xstal_tag);
      FillHists();

      //Pass time cut
      if(Cut_time()) {
         //overall            
         histSvc->SetProcessTag(std::string("hist_timewindow"));
         FillHists();

         //set calorimeter info
         histSvc->SetProcessTag(std::string("hist_timewindow_calo") + calo_tag);
         FillHists();

         //set calorimeter and xtal info
         histSvc->SetProcessTag(std::string("hist_timewindow_calo") + calo_tag + std::string("_xtal") + xstal_tag);
         FillHists();
      }
      
   }
}

void crystalHits::FillHists() {
      
      histSvc->BookFillHist("energy",400,0,4000,energy);
      
      histSvc->BookFillHist("time",1000,-200*1e3,800*1e3,time);
      
      histSvc->BookFillHist("energy_time",400,0,4000,1000,-200*1e3,800*1e3,energy,time);
      
}

bool crystalHits::Cut_time() {
   bool pass = false;
   float start;
   float end;
   
   unsigned long len = method_name.length();
   if(method_name.find("islandFitDAQ")<len) {
      start = 50*1.e3;
      end = 550*1.e3;
   } 
   else if (method_name.find("inFillGainCorrector")<len) {
      start = -50*1.e3;
      end = 450*1.e3;
   }
   else {
      return false;
   }

   if(time>start && time<end) {
      pass = true;
   }
   return pass;
}

crystalHits::crystalHits(std::string name) : 
   fChain(0),
   method_name(name) {}

crystalHits::~crystalHits()
{
   if (!fChain) return;
   // delete fChain->GetCurrentFile();
   delete histSvc;
}

Int_t crystalHits::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t crystalHits::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void crystalHits::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   // std::cout << "init tree" << tree << std::endl;
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runStartUnixTimeSeconds", &runStartUnixTimeSeconds, &b_runStartUnixTimeSeconds);
   fChain->SetBranchAddress("energy", &energy, &b_energy);
   fChain->SetBranchAddress("time", &time, &b_time);
   fChain->SetBranchAddress("chi2", &chi2, &b_chi2);
   fChain->SetBranchAddress("status", &status, &b_status);
   fChain->SetBranchAddress("pedestal", &pedestal, &b_pedestal);
   fChain->SetBranchAddress("caloNum", &caloNum, &b_caloNum);
   fChain->SetBranchAddress("xtalNum", &xtalNum, &b_xtalNum);
   fChain->SetBranchAddress("islandNum", &islandNum, &b_islandNum);
   fChain->SetBranchAddress("laserHit", &laserHit, &b_laserHit);
   fChain->SetBranchAddress("waveformIndex", &waveformIndex, &b_waveformIndex);
   fChain->SetBranchAddress("eventNum", &eventNum, &b_eventNum);
   fChain->SetBranchAddress("subRunNum", &subRunNum, &b_subRunNum);
   fChain->SetBranchAddress("runNum", &runNum, &b_runNum);
   // std::cout << "new - " <<std::endl;
   histSvc = new SimpleHistSVC();
   // std::cout << "new + " <<std::endl;
   Notify();
}

Bool_t crystalHits::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void crystalHits::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t crystalHits::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}