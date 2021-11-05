#include "crystalHits.h"
#include "SimpleHistSVC.h"
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;
string analysis_type = "OptResidual_inFillGainCorrector";


vector<string> files = {
    // "gm2calo_reco_25473661_24688.00113_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473662_24688.00279_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473664_24688.00189_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473665_24688.00002_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473667_24688.00094_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473668_24689.00376_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473677_24690.00013_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473678_24688.00219_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473679_24688.00086_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473681_24688.00097_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473682_24688.00019_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473684_24688.00220_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473685_24688.00227_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473687_24688.00116_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473688_24688.00115_crystalHit_OptPeak.root",
    // "gm2_optThres_local_24686.00019_crystalHit_OptPeak.root",
    // "gm2_optThres_local_24688.00199_crystalHit_OptPeak.root",
    // "gm2_optThres_local_24688.00205_crystalHit_OptPeak.root",
    // "gm2_optThres_local_24688.00246_crystalHit_OptPeak.root",
    // "gm2_optThres_local_24695.00090_crystalHit_OptPeak.root",
    // "gm2calo_reco_25473595_24688.00116_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473596_24688.00220_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473597_24688.00002_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473598_24686.00019_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473600_24690.00013_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473601_24688.00115_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473603_24695.00090_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473604_24688.00086_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473606_24688.00219_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473607_24688.00246_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473608_24688.00094_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473609_24688.00199_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473611_24688.00205_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473612_24688.00227_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473614_24688.00113_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473615_24688.00189_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473616_24688.00279_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473617_24689.00376_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473619_24688.00097_crystalHit_OptBoth.root",
    // "gm2calo_reco_25473620_24688.00019_crystalHit_OptBoth.root",
    // "gm2calo_reco_49532182_24688.00094_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532183_24688.00115_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532184_24688.00279_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532185_24690.00013_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532186_24688.00227_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532187_24688.00205_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532188_24689.00376_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532189_24688.00086_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532190_24688.00220_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532191_24695.00090_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532192_24688.00097_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532193_24688.00019_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532194_24688.00189_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532195_24688.00246_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532196_24688.00113_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532198_24688.00116_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532199_24688.00219_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532200_24688.00199_crystalHit_OptPeak.root",
    // "gm2calo_reco_49532201_24686.00019_crystalHit_OptPeak.root",
    // "gm2_Resub_OptPeak_local_24688.00002_crystalHit_OptPeak.root",
     "gm2calo_reco_25132686_24688.00219_crystalHit_OptResidual.root",
     "gm2calo_reco_25132687_24688.00116_crystalHit_OptResidual.root",
     "gm2calo_reco_25132690_24686.00019_crystalHit_OptResidual.root",
     "gm2calo_reco_25132691_24688.00246_crystalHit_OptResidual.root",
     "gm2calo_reco_25132694_24695.00090_crystalHit_OptResidual.root",
     "gm2calo_reco_25132695_24688.00019_crystalHit_OptResidual.root",
     "gm2calo_reco_25132698_24688.00199_crystalHit_OptResidual.root",
     "gm2calo_reco_25132699_24688.00227_crystalHit_OptResidual.root",
     "gm2calo_reco_25132700_24688.00097_crystalHit_OptResidual.root",
     "gm2calo_reco_25132701_24690.00013_crystalHit_OptResidual.root",
     "gm2calo_reco_25132702_24688.00115_crystalHit_OptResidual.root",
     "gm2calo_reco_25132703_24688.00086_crystalHit_OptResidual.root",
     "gm2calo_reco_25132705_24688.00094_crystalHit_OptResidual.root",
     "gm2calo_reco_25132708_24688.00002_crystalHit_OptResidual.root",
     "gm2calo_reco_25132717_24688.00113_crystalHit_OptResidual.root",
     "gm2calo_reco_25132718_24688.00220_crystalHit_OptResidual.root",
     "gm2calo_reco_25132719_24688.00279_crystalHit_OptResidual.root",
     "gm2_Resub_OptResidual_local_24688.00189_crystalHit_OptResidual.root",
     "gm2_Resub_OptResidual_local_24688.00205_crystalHit_OptResidual.root",
     "gm2_Resub_OptResidual_local_24689.00376_crystalHit_OptResidual.root",
    // "gm2_optThres_local_24686.00019_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00002_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00019_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00086_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00094_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00097_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00113_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00115_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00189_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00199_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00205_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00219_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00220_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00227_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00246_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00279_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24689.00376_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24690.00013_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24695.00090_crystalHit_optThresLocal.root",
    // "gm2_optThres_local_24688.00116_crystalHit_optThresLocal.root",
};

string filePath = "../data/crystalHitTree/";



int main(int argc,char *argv[]) {
    //init files    
    string fullPath = filePath + files[atoi(argv[1])];
    TFile * tfile_optThres = TFile::Open(fullPath.c_str());
    cout <<fullPath<<endl;
    cout <<tfile_optThres<<endl;
    std::map<std::string, TTree*> analyses;

    //Book analyses and TTree
    analyses[analysis_type] = (TTree *)tfile_optThres->Get("CrystalHitTree2/crystalHits");
    cout << analyses[analysis_type]->GetEntries() << endl;

    TFile * output_file;
    for(auto analysis : analyses) {
        std::string method_name = analysis.first;
        std::cout << "in analysis " << method_name << std::endl;
        crystalHits analyser(method_name);

        output_file = new TFile((std::string("../data/hist_output_20211103/")+method_name+"_20stats_"+string(argv[1])+".root").c_str(),"RECREATE");
        analyser.ChangeFile(analysis.second,output_file);
        analyser.Loop();
        analyser.WriteToFile();
        output_file->Close();
    }
    return 0;
}