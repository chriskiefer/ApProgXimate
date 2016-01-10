//
//  main.cpp
//  ApProgXimateLib
//
//  Created by Chris Kiefer on 01/10/2015.
//  Copyright © 2015 Chris Kiefer. All rights reserved.
//

#include <iostream>
#include "ApProgXimate.hpp"
#include <random>
using namespace std;

void glslGen() {
    apProgXimateGLSL apx;
    
    
    apx.addFuncDef(
                   fdef("smooth2", "{return (j0 + j1) / 2.0;}",
                        2, (unsigned int[]) {GLSL_FLOAT, GLSL_FLOAT}, GLSL_FLOAT)
                   );
    apx.addFuncDef(
                   fdef("diff2", "{return abs(j0 - j1);}",
                        2, (unsigned int[]) {GLSL_FLOAT, GLSL_FLOAT}, GLSL_FLOAT)
                   );
    apx.addFuncDef(
                   fdef("distToP", "{return distance(vec2(j0,j1),p);}",
                        2, (unsigned int[]) {GLSL_FLOAT, GLSL_FLOAT}, GLSL_FLOAT)
                   );
    apx.addFuncDef(
                   fdef("xdist", "{return abs(p.x - j0);}",
                        1, (unsigned int[]) {GLSL_FLOAT}, GLSL_FLOAT)
                   );
    
    //    apx.addFuncDef(
    //                       fdef("vec3Dist", "{return vec3(distance(j0.xy,p), distance(j0.yz,p), distance(j0.xz,p));}",
    //                            1, (unsigned int[]) {GLSL_VEC3}, GLSL_VEC3)
    //                       );
    
    
    apx.collectDataTypes();
    
    std::vector<std::string> geneInfo;
    std::vector<float> gene;
    for(int i=0; i < 50; i++) {
        gene.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
    }
    
    std::string code = apx.genCode(gene, geneInfo);
    
}

int main(int argc, const char * argv[]) {
    srand((unsigned int)time(NULL));
//    int x = rand();
//    for(int i=0; i < 100; i++) {
//        x += rand();
//    }
//    cout << x << endl;
    std::cout << "ApProgXimate Programming!\n";
    
    apProgXimateJS apx;
    
    
    
    apx.addFuncDef("saw",
                        "function saw() {\n"
                        "   this.osc = new Module.maxiOsc();\n"
                        "   this.play = function(j0) {\n"
                        "       return this.osc.sawn(Module.maxiMap.linexp(j0,-1,1,20,20000));\n"
                        "   }\n"
                        "}\n"
                        ,
                        1, 99999
                   );
//    apx.apProgXimate::addFuncDef(
//                   fdef("square",
//                        "function square() {\n"
//                        "   this.osc = new Module.maxiOsc();\n"
//                        "   this.play = function(j0) {\n"
//                        "       return this.osc.square(Module.maxiMap.linexp(j0,-1,1,20,20000));\n"
//                        "   }\n"
//                        "}\n"
//                        ,
//                        1, (unsigned int[]) {JS_VAR}, JS_VAR)
//                   );
    apx.addFuncDef("add",
                        "function add() {\n"
                        "   this.play = function(j1, j2) {\n"
                        "       return j1 + j2;\n"
                        "   }\n"
                        "}\n"
                        ,
                        2, 99999
                   );
    //redefine
    apx.addFuncDef("add",
                   "function add() {\n"
                   "   this.play = function(j1, j0) {\n"
                   "       return j1 + j0;\n"
                   "   }\n"
                   "}\n"
                   ,
                   2, 99999
                   );
    apx.addFuncDef("pow",
                   "function pow() {\n"
                   "   this.play = function(j1, j0) {\n"
                   "       return Math.pow(j1, j0);\n"
                   "   }\n"
                   "}\n"
                   ,
                   2, 99999
                   );

    //    apx.apProgXimate::addFuncDef(
//                   fdef("mult",
//                        "function mult() {\n"
//                        "   this.play = function(j1, j2) {\n"
//                        "       return j1 * j2;\n"
//                        "   }\n"
//                        "}\n"
//                        ,
//                        2, (unsigned int[]) {JS_VAR,JS_VAR}, JS_VAR)
//                   );
//    apx.addFuncDef(
//                   fdef("sin",
//                        "function sin() {\n"
//                        "   this.play = function(j0) {\n"
//                        "       return sin(200 + (j0*100));\n"
//                        "   }\n"
//                        "}\n"
//                        ,
//                        1, (unsigned int[]) {JS_VAR}, JS_VAR)
//                   );
//    apx.enableFDef("add", false);
//    apx.removeFDef("pow");
//
//    std::vector<std::string> funcNames;
//    apx.getFunctionNames(funcNames);
//    for(auto it=funcNames.begin(); it != funcNames.end(); ++it) {
//        cout << *it << endl;
//    }
//
//    apx.collectDataTypes();
//    
//
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);
    std::vector<float> gene;
    for(int i=0; i < 10; i++) {
//        float val = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float val = dis(gen);
//        cout << val << endl;
        gene.push_back(val);
    }
    std::string code;
//    code = apx.genCode(gene);
//    std::cout << code << std::endl;
    
    apx.enableFDef("add", true);
    apx.collectDataTypes();
    std::vector<std::string> geneInfo;
    code = apx.genCode(gene, geneInfo);
    std::cout << code << std::endl;
    return 0;
}
