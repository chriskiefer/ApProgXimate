//
//  ApProgXimate.cpp
//  ApProgXimateLib
//
//  Created by Chris Kiefer on 01/10/2015.
//  Copyright © 2015 Chris Kiefer. All rights reserved.
//

#include "ApProgXimate.hpp"
#include <cmath>

using namespace std;



apProgXimate::apProgXimate() {
    srand(static_cast<unsigned int>(time(NULL)));
}

void apProgXimate::collectDataTypes() {
//    funcIndexMap.resize(funcDefs.size());
    dataTypeFuncs.clear();
    dataTypeFuncs.resize(getNumDataTypes());
//    int i=0;
    for(auto it = funcDefs.begin(); it != funcDefs.end(); ++it) {
        if (it->second.enabled) {
            it->second.count = it->second.limit;
    //        funcIndexMap[i] = i;
//            i++;
            dataTypeFuncs[it->second.returnType].push_back(it->first);
        }
    }
//    for(int i=0; i < funcDefs.size(); i++) {
//        funcDefs[i].count = funcDefs[i].limit;
//        funcIndexMap[i] = i;
//        dataTypeFuncs[funcDefs[i].returnType].push_back(i);
//    }

}

void apProgXimate::dataTypeToCode(unsigned int dataType, std::stringstream &code, float value) {
    code << value;
}

void apProgXimate::traverse(codeTreeNode *node, std::stringstream &code, int level) {
    if (node->isConst()) {
        dataTypeToCode(((constNode*)node)->dType, code, ((constNode*)node)->value);
    }else{
        code << endl;
        for(int i=0; i <= level; i++) code << "\t";
        code << ((codeNode*)node)->funcDef->functionName << "(";
        for(int i=0; i < node->paramNodes.size(); i++) {
            traverse(node->paramNodes[i], code, level+1);
            if (i+1 < node->paramNodes.size()) {
                code << ", ";
            }
        }
        code << endl;
        for(int i=0; i <= level; i++) code << "\t";
        code << ")";
    }
};

void apProgXimate::delSubTree(codeTreeNode *node) {
    if (node->isConst()) {
    }else{
        for(int i=0; i < node->paramNodes.size(); i++) {
            delSubTree(node->paramNodes[i]);
            delete node->paramNodes[i];
        }
    }
};

std::string apProgXimate::dataTypeToString(unsigned int t) {
    return "";
}

unsigned int apProgXimate::getNumDataTypes() {
    return 1;
}

void apProgXimate::clearTree() {
    if (root != NULL) {
        delSubTree(root);
        constNodes.clear();
        root = NULL;
    }
    
}

void apProgXimate::enableFDef(std::string name, bool state) {
    funcDefs[name].enabled = state;
}

void apProgXimate::removeFDef(std::string name) {
    funcDefs.erase(name);
}

void apProgXimate::geneToTree(std::vector<float> &gene, vector<vector<std::string> > &dataTypeFuncs, std::vector<std::string> &geneInfo) {
    int genePos = 0;
    unsigned int nodeId=0;
    while(genePos < gene.size()) {
        //--choose where to put the next function
        int constToReplace = 0;
        codeTreeNode *constParent = NULL;
        unsigned int searchForDataType;
        float funcChoice;
        int funcGenePos;
        if (root == NULL) {
            //first one?
            searchForDataType = getFirstDataType();
            funcChoice = gene[genePos];
            
//            geneInfo[genePos] = string("root ").append(std::to_string(funcChoice));
            funcGenePos = genePos;
            genePos++;
        }else{
            //otherwise choose a constant to replace
            constToReplace = floor(gene[genePos] * constNodes.size() * 0.99999);
            constParent = constNodes[constToReplace].parent;
            searchForDataType = ((codeNode*)constParent)->funcDef->argTypes[constNodes[constToReplace].idx];
            funcChoice = static_cast<constNode*>(constParent->paramNodes[constNodes[constToReplace].idx])->value;
            funcGenePos = constParent->paramNodes[constNodes[constToReplace].idx]->fromGenePos;
            geneInfo[genePos] = "pos";
            genePos++;
        }
        
        //--choose a function
        vector<std::string> *fMap = &dataTypeFuncs[searchForDataType];
//        int functionIndex = floor(gene[genePos] * fMap->size() * 0.99999);
        int functionIndex = floor(funcChoice * fMap->size() * 0.99999);
        string fdefName = fMap->at(functionIndex);
        fdef* newFunc = &funcDefs[fdefName];
//        geneInfo[genePos] = fdefName;
//        genePos++;
        if(genePos + newFunc->argTypes.size() - 1 < gene.size()) {
            //make new constant nodes
            codeNode *newCodeNode = new codeNode();
            newCodeNode->funcDef = newFunc;
            newCodeNode->id = nodeId++;
            newCodeNode->fromGenePos = funcGenePos;
            for(int p=0; p < newFunc->argTypes.size(); p++) {
                constNode *newConstNode = new constNode();
                newConstNode->id = nodeId++;
                newConstNode->value = gene[genePos];
                newConstNode->dType = newFunc->argTypes[p];
                newConstNode->fromGenePos = genePos;
                newCodeNode->paramNodes.push_back(newConstNode);
                constNodeDescriptor newDesc;
                newDesc.parent = newCodeNode;
                newDesc.idx = p;
                constNodes.push_back(newDesc);
//                geneInfo[genePos] = std::to_string(2*(gene[genePos]-0.5));
                //limited function?
                //                funcDefs[functionIndex].count--;
                //                if(funcDefs[functionIndex].count == 0) {
                //                    funcIndexMap.erase(find(funcIndexMap.begin(), funcIndexMap.end(), functionIndex));
                //                    cout << "Removed function: " << funcDefs[functionIndex].functionName << endl;
                //                }
                genePos++;
            }
            //put the new function in the tree
            if (root == NULL) {
                root = newCodeNode;
            }else{
                codeTreeNode *oldConst = constParent->paramNodes[constNodes[constToReplace].idx];
                delete oldConst;
                constParent->paramNodes[constNodes[constToReplace].idx] = newCodeNode;
                constNodes.erase(constNodes.begin() + constToReplace);
            }
        }else{
            break;
        }
    }
}

unsigned int apProgXimate::getFirstDataType() {
    return 0;
}

void apProgXimate::getFunctionNames(std::vector<std::string> &names) {
    names.clear();
    for(auto it = funcDefs.begin(); it != funcDefs.end(); ++it) {
        names.push_back(it->first);
    }
}

std::string apProgXimate::getCode(std::string functionName) {
    return funcDefs[functionName].functionDef;
}

bool apProgXimate::isEnabled(std::string functionName) {
    return funcDefs[functionName].enabled;
}


//////////////

apProgXimateGLSL::apProgXimateGLSL() {
}


std::string apProgXimateGLSL::dataTypeToString(unsigned int t) {
    string s;
    switch(t) {
        case GLSL_FLOAT:
            s = "float";
            break;
//        case GLSL_VEC3:
//            s = "vec3";
//            break;
    };
    return s;
}

unsigned int apProgXimateGLSL::getFirstDataType() {
    return GLSL_FLOAT;
}

unsigned int apProgXimateGLSL::getNumDataTypes() {
    return 3;
}

void apProgXimateGLSL::dataTypeToCode(unsigned int dataType, std::stringstream &code, float value) {
    switch(dataType) {
        case GLSL_FLOAT:
            code << value;
            break;
//        case GLSL_VEC3:
//            code << "vec3(" << value << "," << value << "," << value << ")";
//            break;
    };
}


std::string apProgXimateGLSL::genCode(std::vector<float> &gene, std::vector<std::string> &geneInfo, bool clear) {
    if (clear) {
        clearTree();
    }
    
    
    //functions
    stringstream code;
    code << "#extension GL_ARB_texture_rectangle : enable\n"
    "#define PI 3.14159265358\n"
    "uniform sampler2DRect tex0;"
    "uniform float time;\n"
    "uniform vec2 resolution;\n"
    "vec2 p;"
    "vec4 inCol;"
    ;

    for(auto it = funcDefs.begin(); it != funcDefs.end(); ++it) {
        code << dataTypeToString(it->second.returnType) << " " << it->second.functionName << "(";
        for(int j=0; j < it->second.argTypes.size(); j++) {
            code << dataTypeToString(it->second.argTypes[j]);
            code << " j" << j << (j+1 < it->second.argTypes.size() ? "," : "");
        }
        code << ")" << it->second.functionDef << endl;
    }
//    for(int i=0; i < funcDefs.size(); i++) {
//        code << dataTypeToString(funcDefs[i].returnType) << " " << funcDefs[i].functionName << "(";
//        for(int j=0; j < funcDefs[i].argTypes.size(); j++) {
//            code << dataTypeToString(funcDefs[i].argTypes[j]);
//            code << " j" << j << (j+1 < funcDefs[i].argTypes.size() ? "," : "");
//        }
//        code << ")" << funcDefs[i].functionDef << endl;
//    }
    
    code << "void main(){\n"
    "\tinCol = texture2DRect(tex0,  gl_FragCoord.xy);"
    "\tp = gl_FragCoord.xy / resolution.xy;\n"
    "\tvec3 col =";
    
    geneToTree(gene, dataTypeFuncs, geneInfo);
    
    traverse(root, code);
    
    code << ";\n"
    "\tvec4 newCol =vec4(col.x,col.y,col.z,1.0);\n"
    "\tgl_FragColor = newCol;\n"
    "}\n";;
    
    
    return code.str();
}

//////////////

apProgXimateJS::apProgXimateJS() {
}


std::string apProgXimateJS::dataTypeToString(unsigned int t) {
    string s;
    switch(t) {
        case JS_VAR:
            s = "float";
            break;
    };
    return s;
}

unsigned int apProgXimateJS::getFirstDataType() {
    return JS_VAR;
}

unsigned int apProgXimateJS::getNumDataTypes() {
    return 1;
}

void apProgXimateJS::dataTypeToCode(unsigned int dataType, std::stringstream &code, float value) {
    switch(dataType) {
        case JS_VAR:
            code << value;
            break;
    };
}


std::string apProgXimateJS::genCode(std::vector<float> &gene, std::vector<std::string> &geneInfo, bool clear) {
    if (clear) {
        clearTree();
    }
    
    
    //functions
    stringstream code, codeDecls, codeBody, codeCleanup;
    
    for(auto it = funcDefs.begin(); it != funcDefs.end(); ++it) {
        if (it->second.enabled) {
            code << it->second.functionDef << endl;
        }
    }
    
//    for(int i=0; i < funcDefs.size(); i++) {
////        code << dataTypeToString(funcDefs[i].returnType) << " " << funcDefs[i].functionName << "(";
////        for(int j=0; j < funcDefs[i].argTypes.size(); j++) {
////            code << dataTypeToString(funcDefs[i].argTypes[j]);
////            code << " j" << j << (j+1 < funcDefs[i].argTypes.size() ? "," : "");
////        }
////        code << ")" << funcDefs[i].functionDef << endl;
//        code << funcDefs[i].functionDef << endl;
//    }
    
    geneInfo.resize(gene.size(), "");
    
    geneToTree(gene, dataTypeFuncs, geneInfo);
    
    traverseJS(root, codeDecls, codeBody, codeCleanup, geneInfo);
    
    for(int i=0; i < gene.size(); i++) {
        printf("%d: %s\n", i, geneInfo[i].c_str());
    }

    
    code << codeDecls.str() << endl;
    code << "function approxRenderer() {\n"
    "   var w;\n"
    "   w=";
    code << codeBody.str();
    code << ";\n";
    code << "   return w;\n";
    code << "}\n";
    code << "\nfunction _cleanUp() {\n";
    code << "function _dispose(x){\n"
    "   for(k in Object.keys(x)) {\n"
    "       if (typeof x[k] == 'object' && typeof x[k].$$ == 'object') {\n"
    "           x[k].delete();\n"
    "       }\n"
    "   }\n"
    "}\n";
    code << codeCleanup.str();
    code << "}\n";
    
    
    return code.str();
}

void apProgXimateJS::traverseJS(codeTreeNode *node, std::stringstream &codeDecls, std::stringstream &codeBody, std::stringstream &cleanupCode, std::vector<std::string> &geneInfo, int level) {
    if (node->isConst()) {
        float constValue =(((constNode*)node)->value-0.5) * 2;
        dataTypeToCode(((constNode*)node)->dType, codeBody, constValue);
        geneInfo[node->fromGenePos] = std::to_string(constValue);
    }else{
        codeBody << endl;
        stringstream varName;
        varName << ((codeNode*)node)->funcDef->functionName << ((codeNode*)node)->id;
        geneInfo[node->fromGenePos] = varName.str();
        codeDecls << "var " << varName.str() << " = new " << ((codeNode*)node)->funcDef->functionName << "();\n";
        cleanupCode << "\t_dispose(" << varName.str() << ");\n";
        for(int i=0; i <= level; i++) codeBody << "\t";
        codeBody << varName.str() << ".play(";
        for(int i=0; i < node->paramNodes.size(); i++) {
            traverseJS(node->paramNodes[i], codeDecls, codeBody, cleanupCode, geneInfo, level+1);
            if (i+1 < node->paramNodes.size()) {
                codeBody << ", ";
            }
        }
        codeBody << endl;
        for(int i=0; i <= level; i++) codeBody << "\t";
        codeBody << ")";
    }
    

};

void apProgXimateJS::addFuncDef(std::string fName, std::string fDef, unsigned int numArgs, unsigned int lim) {
//    fdef(std::string fName, std::string fDef, unsigned int numArgs, unsigned int argt[], unsigned int rType, unsigned int lim=9999999)
    unsigned int funcArgs[numArgs];
    for(int i=0; i < numArgs; i++) funcArgs[i] = JavascriptDataTypes::JS_VAR;
    fdef newDef = fdef(fName, fDef, numArgs, funcArgs, JavascriptDataTypes::JS_VAR, lim);
    apProgXimate::addFuncDef(newDef);
}

void apProgXimateJS::collectDataTypes() {
    apProgXimate::collectDataTypes();
}

void apProgXimateJS::enableFDef(std::string name, bool state) {
    printf("%s %d", name.c_str(), state);
    apProgXimate::enableFDef(name, state);
}

void apProgXimateJS::getFunctionNames(std::vector<std::string> &names) {
    apProgXimate::getFunctionNames(names);
}

std::string apProgXimateJS::getCode(std::string functionName) {
    return apProgXimate::getCode(functionName);
}

void apProgXimateJS::removeFDef(std::string name) {
    apProgXimate::removeFDef(name);
}

bool apProgXimateJS::isEnabled(std::string functionName) {
    return apProgXimate::isEnabled(functionName);
}
