#include "SimpleHistSVC.h"
#include <iostream>

SimpleHistSVC::SimpleHistSVC() :
    default_int(-3443)
{
    Init();
}
SimpleHistSVC::~SimpleHistSVC() {
    histsDB_1d.clear();
    histsDB_2d.clear();
}

void SimpleHistSVC::SetProcessTag(std::string name) {
    processName = name;
}

void SimpleHistSVC::SetCaloTag(int caloIndex) {
    this->caloIndex = caloIndex;
}

void SimpleHistSVC::SetXtalTag(int xtalIndex) {
    this->xtalIndex = xtalIndex;
}

void SimpleHistSVC::SetStatusTag(int hitStatus) {
    this->hitStatus = hitStatus;
}

void SimpleHistSVC::SetTimeTag(int timeTag) {
    this->timeTag = timeTag;
}

std::string SimpleHistSVC::GetFullName(std::string name) {
    //process name 
    std::string full_name = std::string("hist");
    if(processName.size()!=0) {
      full_name += std::string("_") + processName;
    }

    //status
    if(hitStatus!=default_int) {
        std::string status = "True";
        if(hitStatus==1) {
            full_name += std::string("_StatusTrue");
        }
        else {
            full_name += std::string("_StatusFalse");
        }
    }

    //time tag: time<30 or time>30
    if(timeTag != default_int) {
        if(timeTag==1) {
            full_name += std::string("_timeLT10");
        }
        else if(timeTag==2){
            full_name += std::string("_timeGT10LT30");   
        }
        else if(timeTag==3){
            full_name += std::string("_timeGT30");   
        }
        else {
            std::cout << timeTag << std::endl;
            throw "unknow time tag";
        }
    }

    //calo & xtal index
    if(caloIndex != default_int){
        full_name += std::string("_calo") + std::to_string(caloIndex);
        if(xtalIndex != default_int){
            full_name += std::string("_xtal") + std::to_string(xtalIndex);
        }
    }

    //full name
    full_name += std::string("_") + name;

    return full_name;
}

void SimpleHistSVC::BookFillHist(std::string name, int nbins, float start, float end, float value) {
    
    std::string fullname = GetFullName(name);
    

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
    std::string fullname = GetFullName(name);
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

void SimpleHistSVC::InitNameTags() {
    processName = std::string("");
    caloIndex = default_int;
    xtalIndex = default_int;
    hitStatus = default_int;
    timeTag = default_int;
}

void SimpleHistSVC::Init() {
    InitNameTags();
    histsDB_1d.clear();    
    histsDB_2d.clear();
}