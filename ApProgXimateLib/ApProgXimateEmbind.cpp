//
//  ApProgXimateEmbind.cpp
//  ApProgXimateLib
//
//  Created by Chris Kiefer on 10/11/2015.
//  Copyright © 2015 Chris Kiefer. All rights reserved.
//

/*
 
 http://kripken.github.io/emscripten-site/docs/porting/connecting_cpp_and_javascript/embind.html
 
 emcc --bind -O2 --memory-init-file 0 -o apProgXimate.js ApProgXimateEmbind.cpp ApProgXimate.cpp

 emcc --bind -O2 --memory-init-file 0 -o apProgXimate.js -I/Volumes/LocalDataHD/src/Maximilian/ofxMaxim/ofxMaxim/libs  ApProgXimateEmbind.cpp ApProgXimate.cpp /Volumes/LocalDataHD/src/Maximilian/ofxMaxim/ofxMaxim/libs/maximilian.cpp
 
 */

#include <emscripten/bind.h>

using namespace emscripten;

#include "ApProgXimate.hpp"

#include "maximilian.h"


EMSCRIPTEN_BINDINGS(my_module) {
    
    //need to test if this will work, might need to do something with val::global?
    class_<maxiSettings>("maxiSettings")
    .constructor<>()
    .class_function("setup", &maxiSettings::setup)
    ;
    
    
    class_<maxiOsc>("maxiOsc")
    .constructor<>()
    .function("sawn", &maxiOsc::sawn)
    .function("sinewave", &maxiOsc::sinewave)
    .function("coswave", &maxiOsc::coswave)
    .function("phasor", select_overload<double(double)>(&maxiOsc::phasor))
    .function("phasorBounded", select_overload<double(double,double,double)>(&maxiOsc::phasor))
    .function("saw", &maxiOsc::saw)
    .function("triangle", &maxiOsc::triangle)
    .function("square", &maxiOsc::square)
    .function("pulse", &maxiOsc::pulse)
    .function("noise", &maxiOsc::noise)
    .function("sinebuf", &maxiOsc::sinebuf)
    .function("sinebuf4", &maxiOsc::sinebuf4)
    .function("phaseReset", &maxiOsc::phaseReset)
    ;
    
    class_<maxiFilter>("maxiFilter")
    .constructor<>()
    .function("lores", &maxiFilter::lores)
    .function("hires", &maxiFilter::hires)
    .function("bandpass", &maxiFilter::bandpass)
    .function("lopass", &maxiFilter::lopass)
    .function("hipass", &maxiFilter::hipass)
    ;
    
    class_<maxiMap>("maxiMap")
    .constructor<>()
    .class_function("linlin", &maxiMap::linlin)
    .class_function("linexp", &maxiMap::linexp)
    .class_function("explin", &maxiMap::explin)
    .class_function("clamp", &maxiMap::clamp<double>)
    ;
    
//}
//
//
////std::string genCode(std::vector<float> &gene, bool clear=true) override;
//
//EMSCRIPTEN_BINDINGS(my_module) {
    
//    class_<fdef>("fdef")
//    .constructor<>()
//    //    .class_function("init", &fdef::init)
//    .property("fName", &fdef::functionName)
//    .property("def", &fdef::functionDef)
//    .property("limit", &fdef::limit)
//    .property("argTypes", &fdef::argTypes)
//    ;

    class_<apProgXimateJS>("approgx")
    .constructor<>()
    .function("genCode", &apProgXimateJS::genCode)
    .function("addFuncDef", &apProgXimateJS::addFuncDef)
    .function("collectDataTypes", &apProgXimateJS::collectDataTypes)
    .function("enableFDef", &apProgXimateJS::enableFDef)
    .function("removeFDef", &apProgXimateJS::removeFDef)
    .function("getFunctionNames", &apProgXimateJS::getFunctionNames)
    .function("getCode", &apProgXimateJS::getCode)
    .function("isEnabled", &apProgXimateJS::isEnabled)
    ;
    
//    fdef(std::string fName, std::string fDef, unsigned int numArgs, unsigned int argt[], unsigned int rType, unsigned int lim=9999999)
//    void init(std::string fName, std::string fd, unsigned int numArgs, unsigned int argt[], unsigned int rType, unsigned int lim=9999999) {


    register_vector<float>("FloatVector");
    register_vector<unsigned int>("UnsignedIntVector");
    register_vector<std::string>("StringVector");

    
    
    
}


