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

void apProgXimate::geneToTree(std::vector<float> &gene, vector<vector<std::string> > &dataTypeFuncs) {
    int genePos = 0;
    unsigned int nodeId=0;
    while(genePos < gene.size()) {
        //choose a function
        //        cout << gene[genePos] << endl;
        int constToReplace = 0;
        codeTreeNode *constParent;
        unsigned int searchForDataType;
        if (root == NULL) {
            searchForDataType = getFirstDataType();
        }else{
            constToReplace = floor(gene[genePos] * constNodes.size() * 0.99999);
            constParent = constNodes[constToReplace].parent;
            searchForDataType = ((codeNode*)constParent)->funcDef->argTypes[constNodes[constToReplace].idx];
            genePos++;
        }
        vector<std::string> *fMap = &dataTypeFuncs[searchForDataType];
        int functionIndex = floor(gene[genePos] * fMap->size() * 0.99999);
        //functionIndex = fMap->at(functionIndex);
        string fdefName = fMap->at(functionIndex);
//        fdef* newFunc = &funcDefs[functionIndex];
        fdef* newFunc = &funcDefs[fdefName];
        if(genePos + newFunc->argTypes.size() + 1 < gene.size()) {
            if (root != NULL) {
            }
            codeNode *newCodeNode = new codeNode();
            newCodeNode->funcDef = newFunc;
            newCodeNode->id = nodeId++;
            for(int p=0; p < newFunc->argTypes.size(); p++) {
                constNode *newConstNode = new constNode();
                newConstNode->id = nodeId++;
                genePos++;
                newConstNode->value = gene[genePos];
                newConstNode->dType = newFunc->argTypes[p];
                newCodeNode->paramNodes.push_back(newConstNode);
                constNodeDescriptor newDesc;
                newDesc.parent = newCodeNode;
                newDesc.idx = p;
                constNodes.push_back(newDesc);
                //limited function?
                //                funcDefs[functionIndex].count--;
                //                if(funcDefs[functionIndex].count == 0) {
                //                    funcIndexMap.erase(find(funcIndexMap.begin(), funcIndexMap.end(), functionIndex));
                //                    cout << "Removed function: " << funcDefs[functionIndex].functionName << endl;
                //                }
            }
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


std::string apProgXimateGLSL::genCode(std::vector<float> &gene, bool clear) {
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
    
    geneToTree(gene, dataTypeFuncs);
    
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


std::string apProgXimateJS::genCode(std::vector<float> &gene, bool clear) {
    if (clear) {
        clearTree();
    }
    
    
    //functions
    stringstream code, codeDecls, codeBody;
    
    for(auto it = funcDefs.begin(); it != funcDefs.end(); ++it) {
        code << it->second.functionDef << endl;
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
    
    
    geneToTree(gene, dataTypeFuncs);
    
    traverseJS(root, codeDecls, codeBody);
    
    code << codeDecls.str() << endl;
    code << "function approxRenderer() {\n"
    "   var w;\n"
    "   w=";
    code << codeBody.str();
    code << ";\n";
    code << "   return w;\n";
    code << "}";
    
    
    return code.str();
}

void apProgXimateJS::traverseJS(codeTreeNode *node, std::stringstream &codeDecls, std::stringstream &codeBody, int level) {
    if (node->isConst()) {
        dataTypeToCode(((constNode*)node)->dType, codeBody, (((constNode*)node)->value-0.5) * 2);
    }else{
        codeBody << endl;
        stringstream varName;
        varName << ((codeNode*)node)->funcDef->functionName << ((codeNode*)node)->id;
        codeDecls << "var " << varName.str() << " = new " << ((codeNode*)node)->funcDef->functionName << "();\n";
        for(int i=0; i <= level; i++) codeBody << "\t";
        codeBody << varName.str() << ".play(";
        for(int i=0; i < node->paramNodes.size(); i++) {
            traverseJS(node->paramNodes[i], codeDecls, codeBody, level+1);
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
