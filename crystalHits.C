#include "crystalHits.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
using namespace std;
void crystalHits::ChangeFile(TTree * tree, TDirectory * file){
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

void crystalHits::Loop(int entries_debug)
{
   if (fChain == 0) return;   
   
   Long64_t nentries = fChain->GetEntriesFast();      
   if(entries_debug!=-1){
      nentries = entries_debug;
   }
   
   Long64_t nbytes = 0, nb = 0;

   std::string histName;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {

      Long64_t ientry = LoadTree(jentry);

      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if (jentry%100000==0) std::cout << "processed " << jentry << " events" << std::endl;
      if(entries_debug!=-1){
         cout <<"Calo"<< caloNum << " xtal" << xtalNum << "  event "<< eventNum <<"     island "<< islandNum << endl;
      }
      
      if(!Cut_time()) continue;

      std::string calo_tag = std::to_string(caloNum);
      std::string xstal_tag = std::to_string(xtalNum);      

      //overall for cluster tree      
      
      // cout << "debug 1 " << endl;
      // histograms for clustering tree:
      // energy_time
      // overall
      // calo
      // calo+xtal
      if(!isXtalHitTree) {
         // over all, energy, energy_time
         histName = std::string("hist_timewindow_statusTrue");
         histSvc->SetProcessTag(histName);
         // histSvc->BookFillHist("energy_time",5000,0,0.1492*5000,6200,0,6200,(time-start_time)*1.25/1.e3,energy/1.e3); //\mu s, GeV
         histSvc->BookFillHist("energy",3000,0,3000,energy);
         histSvc->BookFillHist("energy_time",5000,0,0.1492*5000,6200,0,6200,(time-start_time)*1.25/1.e3,energy/1.e3); 
         //calo, energy, energy_time
         histName = std::string("hist_timewindow_statusTrue_calo") + calo_tag;
         histSvc->SetProcessTag(histName);
         histSvc->BookFillHist("energy",3000,0,3000,energy);
         // histSvc->BookFillHist("energy_time",5000,0,0.1492*5000,6200,0,6200,(time-start_time)*1.25/1.e3,energy/1.e3);
         //calo + xtal, energy
         histName = std::string("hist_timewindow_statusTrue_calo") + calo_tag + std::string("_xtal") + xstal_tag;
         histSvc->SetProcessTag(histName);   
         histSvc->BookFillHist("energy",3000,0,3000,energy);
      }
      // cout << "debug 3 " << endl;

      //histograms for xtal hit tree:
      //energy spectrum - overall 
      //energy spectrum - calo+xtal
      if(isXtalHitTree) {
         if(useStatusCut && status!=1) continue;

         std::string status_str = "True";
         if(status!=1) {
            status_str = "False";
         }
         //overall hists
         histName = std::string("hist_timewindow_status") + status_str;
         histSvc->SetProcessTag(histName);
         histSvc->BookFillHist("energy",3000,0,3000,energy);
         //calo+xtal info
         histName = std::string("hist_timewindow_status") + status_str + std::string("_calo")+ calo_tag + std::string("_xtal") + xstal_tag;
         histSvc->SetProcessTag(histName);
         histSvc->BookFillHist("energy",3000,0,3000,energy);
      }
   }
}

void crystalHits::FillHists() {
      
      histSvc->BookFillHist("energy",400,0,4000,energy);
      
      histSvc->BookFillHist("time",1000,-200*1e3,800*1e3,time);
      
      histSvc->BookFillHist("energy_time",400,0,4000,1000,-200*1e3,800*1e3,energy,time);
      
}

bool crystalHits::Cut_time() {
   
   if(isXtalHitTree && laserHit) { 
      return false;
   }
   bool pass = false;   
   if(time<start_time || time>end_time) {
      return false;
   }
   return true;
}

crystalHits::crystalHits(std::string name) : 
   fChain(0),
   method_name(name),
   useStatusCut(true)
   {
      size_t len = method_name.length();
      if(method_name.find("islandFitterDAQ")<len) {
         start_time = 130*1.e3;
         end_time = 660*1.e3;
         isXtalHitTree = true;
      } 
      else if (method_name.find("inFillGainCorrector")<len) {
         start_time = 30*1.e3;
         end_time = 560*1.e3;
         isXtalHitTree = true;
      }
      else if (method_name.find("hitClusterDAQ")<len) {
         start_time = 30*1.e3;
         end_time = 560*1.e3;
         isXtalHitTree = false;
      }
      else {
         cout << "method: " << method_name << endl;
         throw std::string("unknow method");
      }
   }

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