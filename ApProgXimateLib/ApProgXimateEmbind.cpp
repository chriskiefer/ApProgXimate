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
 
  emcc --bind -O2 -s TOTAL_MEMORY=268435456 -o apProgXimate.js -I/Volumes/LocalDataHD/src/Maximilian/ofxMaxim/ofxMaxim/libs  ApProgXimateEmbind.cpp ApProgXimate.cpp /Volumes/LocalDataHD/src/Maximilian/ofxMaxim/ofxMaxim/libs/maximilian.cpp --preload-file samples
 
 
 https://kripken.github.io/emscripten-site/docs/getting_started/Tutorial.html#tutorial-files
 */

#include <emscripten/bind.h>

using namespace emscripten;

#include "ApProgXimate.hpp"

#include "maximilian.h"

class maxiTrig {
public:
    maxiTrig() : prev(0) {
    }
    double waitZeroX(double w) {
        bool trig = prev < 0 && w >= 0;
        prev = w;
        return trig ? 1 : 0;
    }
    double waitBoundX(double w, double boundary) {
        bool trig = prev < boundary && w >= boundary;
        prev = w;
        return trig ? 1 : 0;
    }
    double waitRangeX(double w, double boundaryLow, double boundaryHigh) {
        bool trig = prev < boundaryLow && w >= boundaryHigh;
        prev = w;
        return trig ? 1 : 0;
    }
private:
    double prev;
};

class maxiSampleHold {
public:
    maxiSampleHold() : value(0) {
    }
    bool play(double w, double trig) {
        if (trig > 0.5) {
            value = w;
        }
        return value;
    }
private:
    double value;
};


EMSCRIPTEN_BINDINGS(my_module) {
    
    //need to test if this will work, might need to do something with val::global?
    class_<maxiSettings>("maxiSettings")
    .constructor<>()
    .class_function("setup", &maxiSettings::setup)
    ;
    
    class_<maxiTrig>("maxiTrig")
    .constructor<>()
    .function("waitZeroX", &maxiTrig::waitZeroX)
    .function("waitBoundX", &maxiTrig::waitBoundX)
    .function("waitRangeX", &maxiTrig::waitRangeX)
    ;

    class_<maxiSampleHold>("maxiSampleHold")
    .constructor<>()
    .function("play", &maxiSampleHold::play)
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
    
    class_<maxiDelayline>("maxiDelayline")
    .constructor<>()
    .function("dl", select_overload<double(double,int,double)>(&maxiDelayline::dl))
    ;

    class_<maxiDyn>("maxiDyn")
    .constructor<>()
    .function("gate", &maxiDyn::gate)
    .function("compressor", &maxiDyn::compressor)
    ;

    class_<maxiEnv>("maxiEnv")
    .constructor<>()
    .function("ar", &maxiEnv::ar)
    .function("adsr", &maxiEnv::adsr)
    ;

    class_<maxiDistortion>("maxiDistortion")
    .constructor<>()
    .function("atanDist", &maxiDistortion::atanDist)
    .function("fastAtanDist", &maxiDistortion::fastAtanDist)
    ;

    class_<maxiFlanger>("maxiFlanger")
    .constructor<>()
    .function("flange", &maxiFlanger::flange)
    ;

    class_<maxiChorus>("maxiChorus")
    .constructor<>()
    .function("chorus", &maxiChorus::chorus)
    ;

    class_<maxiSVF>("maxiSVF")
    .constructor<>()
    .function("play", &maxiSVF::play)
    .function("setCutoff", &maxiSVF::setCutoff)
    .function("setResonance", &maxiSVF::setResonance)
    ;

    class_<maxiSample>("maxiSample")
    .constructor<>()
    .function("load", &maxiSample::load)
    .function("trigger", &maxiSample::trigger)
    .function("playOnce", select_overload<double(double)>(&maxiSample::playOnce))
    .function("play", select_overload<double(double)>(&maxiSample::play))
    ;

    class_<maxiDCBlocker>("maxiDCBlocker")
    .constructor<>()
    .function("play", &maxiDCBlocker::play)
    ;

    class_<apProgXimateJS>("approgx")
    .constructor<>()
    .function("genCode", &apProgXimateJS::genCode)
    .function("addFuncDef", &apProgXimateJS::addFuncDef)
    .function("updateFuncDef", &apProgXimateJS::updateFuncDef)
    .function("collectDataTypes", &apProgXimateJS::collectDataTypes)
    .function("enableFDef", &apProgXimateJS::enableFDef)
    .function("removeFDef", &apProgXimateJS::removeFDef)
    .function("removeFDefByID", &apProgXimateJS::removeFDef)
    .function("getFunctionNames", &apProgXimateJS::getFunctionNames)
    .function("getFunctionIds", &apProgXimateJS::getFunctionIds)
    .function("getCode", &apProgXimateJS::getCode)
    .function("isEnabled", &apProgXimateJS::isEnabled)
//    .function("test", &apProgXimateJS::test)
    ;
    

    register_vector<float>("FloatVector");
    register_vector<unsigned int>("UnsignedIntVector");
    register_vector<int>("IntVector");
    register_vector<std::string>("StringVector");
    register_map<std::string, std::string>("StringMap");
    

    
    
    
}


