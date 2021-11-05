#include "crystalHits.h"
#include "SimpleHistSVC.h"
#include "TChain.h"
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>


using namespace std;



void ShowUsage(string main_exe) {
    cout << "Usage: \n" <<
        "/Path/to/" << main_exe << " -i input.list -d /input/dir(optional) -o /output/file.root -n hit_name " << endl;
}

map<string,string> parse_argv(int argc,char *argv[]) {
    map<string,string> config;
    map<string,string> name_map = {{"-i","input_list"},{"-o","output_file"},{"-n","hit_name"},{"-d","input_path"}};
    int n_arg = 1;
    while(n_arg<argc){
        if(name_map.find(argv[n_arg])!=name_map.end()){
            config[name_map[argv[n_arg]]] = argv[n_arg+1];
            n_arg += 2;
        }
        else {
            ShowUsage(argv[0]);
        }
    }
    if(config.find("input_path")==config.end()){
        config["input_path"] = "null";
    }
    for(auto item : name_map){
        if(config.find(item.second)==config.end()){
            ShowUsage(argv[0]);
            exit(-1);
        }
    }
    return config;
}



int main(int argc,char *argv[]) {
    map<string,string> config = parse_argv(argc,argv);    
    string tree_name;

    if(config["hit_name"]=="islandFitterDAQ") {
        tree_name = "CrystalHitTree1/crystalHits";
    }
    else if(config["hit_name"]=="inFillGainCorrector") {
        tree_name = "CrystalHitTree2/crystalHits";
    }
    else{
        cout <<" hit_name should be inFillGainCorrector or islandFitterDAQ " << endl;

    }

    string file_list = config["input_list"];

    TFile * output_file = new TFile(config["output_file"].c_str(),"RECREATE");
    TChain * fChain = new TChain(tree_name.c_str());

    ifstream file(file_list.c_str());
    string str;
    string file_contents;
    while (std::getline(file, str))
    {
     if(str.find("#")==0) {
        continue;
     }
     string file_path = str;
     if(config["input_path"]!="null"){
        file_path = config["input_path"] + file_path;
     }

     fChain->Add(file_path.c_str());
     cout <<"adding: "<<file_path<<endl;
    }
            
    //Book analyses and TTree
    
    cout << "\nTotal entries: "<<fChain->GetEntries() << endl;
    cout << "Handle crytsl hits type: " << config["hit_name"] << endl;
            
    crystalHits analyser(config["hit_name"]);
    analyser.ChangeFile(fChain,output_file);
    analyser.Loop();
    analyser.WriteToFile();
    output_file->Close();

    return 0;
}