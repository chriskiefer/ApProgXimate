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
        funcDefs[f.id]= f;
    };
    virtual void enableFDef(int id, bool state);
//    virtual void enableFDefByID(std::string name, bool state);
    virtual std::string genCode(std::vector<float> &gene, std::vector<std::string> &geneInfo, std::vector<unsigned int> &constIndexes, bool clear=true) = 0;
    virtual void collectDataTypes();
    virtual void getFunctionNames(std::vector<std::string> &names);
    virtual void getFunctionIds(std::vector<int> &ids);
    virtual std::string getCode(int id);
    virtual bool isEnabled(int id);
    virtual void removeFDef(int id);
    
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
    std::map<int, fdef> funcDefs;
    codeTreeNode *root;
    std::vector<constNodeDescriptor> constNodes;
    
    virtual void traverse(codeTreeNode *node, std::stringstream &code, int level=0);
    void delSubTree(codeTreeNode *node);
    virtual std::string dataTypeToString(unsigned int t);
    virtual unsigned int getFirstDataType();
    virtual unsigned int getNumDataTypes();
    virtual void dataTypeToCode(unsigned int dataType, std::stringstream &code, float value);
    void clearTree();
    void geneToTree(std::vector<float> &gene, std::vector<std::vector<int> > &dataTypeFuncs, std::vector<std::string> &geneInfo);
    std::vector<std::vector<int> > dataTypeFuncs;
    std::vector<unsigned int> funcIndexMap;
    
    int lastID=0;
    int getNextID(){return lastID++;}

    
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
    
    int addFuncDef(std::string fName, std::string fDef, unsigned int numArgs, unsigned int lim=9999999);
    void updateFuncDef(std::string fName, std::string fDef, unsigned int numArgs, int id, unsigned int lim=9999999);
    void collectDataTypes() override;
    void enableFDef(int id, bool state) override;
    void getFunctionNames(std::vector<std::string> &names) override;
    void getFunctionIds(std::vector<int> &ids) override;
    std::string getCode(int id) override;
    void removeFDef(int id) override;
//    void removeFDefByID(int id) override;
    bool isEnabled(int id) override;
protected:
    std::string dataTypeToString(unsigned int t) override;
    unsigned int getFirstDataType() override;
    unsigned int getNumDataTypes() override;
    void dataTypeToCode(unsigned int dataType, std::stringstream &code, float value) override;
    void traverseJS(codeTreeNode *node, std::stringstream &codeDecls, std::vector<float> &codeConsts, std::stringstream &codeBody, std::stringstream &cleanupCode, std::vector<unsigned int> &constIndexes, std::vector<std::string> &geneInfo, int level=0);
};


#endif /* ApProgXimate_hpp */
