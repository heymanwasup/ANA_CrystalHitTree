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
    cout << "Usage: \n";    
    cout << "/Path/to/" << main_exe << " -i input.list -o output.root -hit hit_name -jobs 1,3,5" << endl;    
    cout << "/Path/to/" << main_exe << " -i input.list -o output.root -hit hit_name" << endl;    
}

map<string,string> parse_argv(int argc,char *argv[]) {
    map<string,string> config;        
    config["jobs"] = "null";
    config["events"] = "-1";

    map<string,string> name_map = {{"-i","input_list"},{"-o","output_file"},{"-hit","hit_name"},{"-jobs","jobs"},{"-events","events"}};
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

    return config;
}

vector<int> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<int> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (atoi(token.c_str()));        
    }

    res.push_back (atoi(s.substr (pos_start).c_str()));
    return res;
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
    else if(config["hit_name"]=="hitClusterDAQ") {
        tree_name = "corClusterHitTree/clusters";
    }
    else{
        cout <<" hit_name should be inFillGainCorrector or islandFitterDAQ or hitClusterDAQ" << endl;
    }

    string file_list = config["input_list"];
    vector<string> list_of_files;
    if(file_list.find(".list")>file_list.length() && file_list.find(".root")<file_list.length()) {
        list_of_files.push_back(file_list);
    }
    else {
        // cout << "file_list " <<  file_list << endl;
        ifstream file(file_list.c_str());
        string str;
        while (std::getline(file, str))
        {
         if(str.find("#")==0) {
            continue;
         }     
         list_of_files.push_back(str);
         // cout <<"got "<< str << endl;
        }    
    }


    vector<string> run_jobs;
    if(config["jobs"]!="null"){
        cout <<"jobs: "<<config["jobs"]<<endl;
        vector<int> jobs = split(config["jobs"],",");
        for(int n : jobs){
            run_jobs.push_back(list_of_files[n]);            
        }
    }
    else{
        // cout << "here 1"<<endl;
        run_jobs = list_of_files;
    }

    TChain * fChain = new TChain(tree_name.c_str());
    for(string file_path : run_jobs){
        fChain->Add(file_path.c_str());
        cout <<"adding: "<<file_path<<endl;
    }

    //Book analyses and TTree    
    TFile * output_file = new TFile(config["output_file"].c_str(),"RECREATE");

    cout << "\nTotal entries: "<<fChain->GetEntries() << endl;
    cout << "Handle crytsl hits type: " << config["hit_name"] << endl;
    cout <<"Output file: "<< config["output_file"] << endl;

    int entries_debug = -1;
    entries_debug = atoi(config["events"].c_str());
    crystalHits analyser(config["hit_name"]);
    analyser.ChangeFile(fChain,output_file);
    analyser.Loop(entries_debug);
    analyser.WriteToFile();
    output_file->Close();

    return 0;
}