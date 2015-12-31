//
//  fdef.hpp
//  ApProgXimateLib
//
//  Created by Chris Kiefer on 29/12/2015.
//  Copyright © 2015 Chris Kiefer. All rights reserved.
//

#ifndef fdef_h
#define fdef_h

class fdef {
public:
    std::string functionName;
    std::string functionDef;
    unsigned int limit;
    unsigned int count;
    std::vector<unsigned int> argTypes;
    unsigned int returnType;
    //    static std::vector<T> makeTypeList() {return std::vector<int>();}
    bool enabled;
    fdef() {limit=99999;}
    fdef(std::string fName, std::string fDef, unsigned int numArgs, unsigned int argt[], unsigned int rType, unsigned int lim=9999999)
    : functionName(fName), functionDef(fDef), returnType(rType), limit(lim), enabled(true) {
        for(int i=0; i < numArgs; i++)  {
            argTypes.push_back(argt[i]);
        }
    }
    
};

#endif /* fdef_h */
