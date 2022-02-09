#include "crystalHits.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <TRandom3.h>



using namespace std;
double crystalHits::Randomize(double t) {
   return t+r->Uniform(-1,1)*0.1492/2;
}
void crystalHits::ChangeFile(TTree * tree, TDirectory * file){
   fChain = tree;
   output_file = file;   
   Init(tree);   
   histSvc->BookFile(file);
   
}

void crystalHits::WriteToFile() {
   histSvc->Write();
   // delete histSvc;
}

void crystalHits::AnaCrystalHits() {
   if(timeTag==2) return;
   histSvc->SetTimeTag(timeTag);
   histSvc->SetStatusTag(status);
   //overall hists   
   histSvc->BookFillHist("energy",3000,0,3000,energy);
   
   //calo info
   histSvc->SetCaloTag(caloNum);
   histSvc->BookFillHist("energy",3000,0,3000,energy);
   
   //calo+xtal info
   histSvc->SetXtalTag(xtalNum);   
   histSvc->BookFillHist("energy",3000,0,3000,energy);
}



void crystalHits::AnaClusteredHits() {
   
   //E-t hist: all calos
   histSvc->BookFillHist("energy_time",5000*6,0,0.1492*5000,104*9,0,9.36,Randomize(time*1.25/1.e3),energy/1.e3);

   //E-t hist: each calo
   histSvc->SetCaloTag(caloNum);
   histSvc->BookFillHist("energy_time",5000*6,0,0.1492*5000,104*9,0,9.36,Randomize(time*1.25/1.e3),energy/1.e3);

   if(timeTag==2) return;
   histSvc->ResetCaloTag();
   histSvc->SetTimeTag(timeTag);

   //Energy spectrum: all calos   
   histSvc->BookFillHist("energy",4000,0,4000,energy);

   //Energy spectrum: each calo
   histSvc->SetCaloTag(caloNum);
   histSvc->BookFillHist("energy",4000,0,4000,energy);
   
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
      
      //exclude the laser hits
      if(!Cut_laserHit()) continue;

      double time_range = time*1.25-offset_time;
      if(time_range<10.e3) {
         timeTag = 1;
      }
      else if(time_range<30.e3) {
         timeTag = 2;  
      }
      else {
         timeTag = 3;
      }

      histSvc->InitNameTags();
      histSvc->SetProcessTag("timewindow");
      //xtal hits
      if(isXtalHitTree) {
         if(useStatusCut && status!=1) continue;
         AnaCrystalHits();
      }
      //clustered hits
      else {
         AnaClusteredHits();
      }
   }
}

bool crystalHits::Cut_laserHit() {
   
   if(isXtalHitTree && laserHit) { 
      return false;
   }

   bool pass = false;
   if(time*1.25<start_time || time*1.25>end_time) {
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
         start_time = 100*1.e3;
         end_time = 800*1.e3;
         offset_time = 130.e3;
         isXtalHitTree = true;
      } 
      else if (method_name.find("inFillGainCorrector")<len) {
         start_time = 0;
         end_time = 700*1.e3;
         offset_time = 0.;
         isXtalHitTree = true;
      }
      else if (method_name.find("hitClusterDAQ")<len) {
         start_time = 0*1.e3;
         end_time = 700*1.e3;
         offset_time = 0.;
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
   delete r;
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
   r = new TRandom3();
   // std::cout << "new + " <<std::endl;   
}