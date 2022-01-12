#include "SimpleHistSVC.h"
#include <iostream>

SimpleHistSVC::SimpleHistSVC() {
    Init();
}
SimpleHistSVC::~SimpleHistSVC() {
    // std::cout << "call ~SimpleHistSVC()" << std::endl;
    // for(auto itr : histsDB_1d) {
    //     delete (itr.second);        
    // }
    histsDB_1d.clear();
    // std::cout<< "2d:" << histsDB_2d.size() << std::endl;
    // for(auto itr : histsDB_2d) {
    //     delete (itr.second);        
    // }
    histsDB_2d.clear();
}

void SimpleHistSVC::SetProcessTag(std::string name) {
    processName = name;
}

void SimpleHistSVC::BookFillHist(std::string name, int nbins, float start, float end, float value) {
    
    std::string fullname = processName + "_" + name;
    

    std::map<std::string,TH1F*>::iterator itr = histsDB_1d.find(fullname);
    
    TH1F * hist;
    if(itr == histsDB_1d.end()) {
        
        hist = new TH1F(fullname.c_str(),fullname.c_str(),nbins,start,end);
        
        hist->SetDirectory(output_file);
        
        histsDB_1d[fullname] = hist;
    } else {
        hist = itr->second;
    }
    hist->Fill(value);
}

void SimpleHistSVC::BookFillHist(std::string name, int nbinsX, float startX, float endX, int nbinsY, float startY, float endY,float x, float y) {    
    std::string fullname = processName + "_" + name;
    auto itr = histsDB_2d.find(fullname);
    TH2F * hist;
    if(itr == histsDB_2d.end()) {
        hist = new TH2F(fullname.c_str(),fullname.c_str(),nbinsX,startX,endX,nbinsY,startY,endY);
        hist->SetDirectory(output_file);
        histsDB_2d[fullname] = hist;
    } else {
        hist = itr->second;
    }
    hist->Fill(x,y);
}

void SimpleHistSVC::BookFile(TDirectory *file) {
    output_file = file;
}

void SimpleHistSVC::Write() {
    for(auto itr : histsDB_1d) {
        (itr.second)->Write();
    }
    
    for(auto itr : histsDB_2d) {
        (itr.second)->Write();        
    }    
}

void SimpleHistSVC::Init() {
    processName = std::string("hist");
    
    histsDB_1d.clear();
    
    histsDB_2d.clear();
}