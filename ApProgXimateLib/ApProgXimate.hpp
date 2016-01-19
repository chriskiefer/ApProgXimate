//
//  ApProgXimate.hpp
//  ApProgXimateLib
//
//  Created by Chris Kiefer on 01/10/2015.
//  Copyright © 2015 Chris Kiefer. All rights reserved.
//

#ifndef ApProgXimate_hpp
#define ApProgXimate_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include "fdef.hpp"




class apProgXimate {
public:
    apProgXimate();
    void addFuncDef(fdef f) {
        funcDefs[f.functionName]= f;
    };
    virtual void enableFDef(std::string name, bool state);
//    virtual void enableFDefByID(std::string name, bool state);
    virtual std::string genCode(std::vector<float> &gene, std::vector<std::string> &geneInfo, std::vector<unsigned int> &constIndexes, bool clear=true) = 0;
    virtual void collectDataTypes();
    virtual void getFunctionNames(std::vector<std::string> &names);
    virtual std::string getCode(std::string functionName);
    virtual bool isEnabled(std::string functionName);
    virtual void removeFDef(std::string name);
    virtual void removeFDefByID(int id);
    
protected:
    
    class codeTreeNode {
    public:
        std::vector<codeTreeNode*> paramNodes;
        virtual bool isConst() {return false;}
        unsigned int id;
        unsigned int fromGenePos;
    };
    
    class codeNode : public codeTreeNode {
    public:
        fdef *funcDef;
    };
    
    class constNode : public codeTreeNode {
    public:
        float value;
        unsigned dType;
        bool isConst() {return true;}
    };
    
    class constNodeDescriptor {
    public:
        codeTreeNode * parent;  // a pointer to the parent of the consts
        int idx;  // the index of the const within the parent node
    };
    
//    std::vector<fdef> funcDefs;
    std::map<std::string, fdef> funcDefs;
    codeTreeNode *root;
    std::vector<constNodeDescriptor> constNodes;
    
    virtual void traverse(codeTreeNode *node, std::stringstream &code, int level=0);
    void delSubTree(codeTreeNode *node);
    virtual std::string dataTypeToString(unsigned int t);
    virtual unsigned int getFirstDataType();
    virtual unsigned int getNumDataTypes();
    virtual void dataTypeToCode(unsigned int dataType, std::stringstream &code, float value);
    void clearTree();
    void geneToTree(std::vector<float> &gene, std::vector<std::vector<std::string> > &dataTypeFuncs, std::vector<std::string> &geneInfo);
    std::vector<std::vector<std::string> > dataTypeFuncs;
    std::vector<unsigned int> funcIndexMap;

    
};

class apProgXimateGLSL : public apProgXimate {
public:
    apProgXimateGLSL();
    std::string genCode(std::vector<float> &gene, std::vector<std::string> &geneInfo, std::vector<unsigned int> &constIndexes, bool clear=true) override;
protected:
    std::string dataTypeToString(unsigned int t) override;
    unsigned int getFirstDataType() override;
    unsigned int getNumDataTypes() override;
    void dataTypeToCode(unsigned int dataType, std::stringstream &code, float value) override;
};

enum GLSLDataTypes {
    GLSL_FLOAT //, GLSL_VEC3
};

enum JavascriptDataTypes {
    JS_VAR
};

class apProgXimateJS : public apProgXimate {
public:
    apProgXimateJS();
    std::string genCode(std::vector<float> &gene, std::vector<std::string> &geneInfo, std::vector<unsigned int> &constIndexes, bool clear=true) override;
    void addFuncDef(std::string fName, std::string fDef, unsigned int numArgs, int id = -1, unsigned int lim=9999999);
    void collectDataTypes() override;
    void enableFDef(std::string name, bool state) override;
    void getFunctionNames(std::vector<std::string> &names) override;
    std::string getCode(std::string functionName) override;
    void removeFDef(std::string name) override;
    void removeFDefByID(int id) override;
    bool isEnabled(std::string functionName) override;
protected:
    std::string dataTypeToString(unsigned int t) override;
    unsigned int getFirstDataType() override;
    unsigned int getNumDataTypes() override;
    void dataTypeToCode(unsigned int dataType, std::stringstream &code, float value) override;
    void traverseJS(codeTreeNode *node, std::stringstream &codeDecls, std::vector<float> &codeConsts, std::stringstream &codeBody, std::stringstream &cleanupCode, std::vector<unsigned int> &constIndexes, std::vector<std::string> &geneInfo, int level=0);
};


#endif /* ApProgXimate_hpp */
