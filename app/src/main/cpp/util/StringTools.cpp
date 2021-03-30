//
// Created by dawnoftherent on 3/21/21.
//

#include "StringTools.h"
#include "../debug.h"

#define LOOPKILLER 4096

#define TAG "StoolCpp"

namespace com_curiousorigins_simplegroupcommserver {
    char StringTools::US = 31;
    char StringTools::RS = 30;
    char StringTools::GS = 29;

    void StringTools::setGroupChars(char unitSeparator, char recordSeparator, char groupSeparator) {
        US=unitSeparator;
        RS=recordSeparator;
        GS=groupSeparator;
    }

    void StringTools::fill(const char *source, char *target, size_t targetLen) {

//        int targetI=0;
//
//        //iterate through elements of transmission
//        for(int elementI = 0; elementI<sourceElements; ++elementI) {
//            const char** element = source[elementI];
//            const int elementLength = element[0][0];
//            //iterate through components of element
//            for(int componentI = 1; componentI < elementLength; ++componentI){
//                //iterate through characters in a component
//                const char* component = element[componentI];
//                const int componentLength = len(component);
//
//                int charI=0;
//                for(charI=0; charI<componentLength; ++charI){
//                    if(targetI > targetLen -2){
//                        PDBG(TAG,"not enough room to copy array")
//                        return;
//                    }
//                    char c = component[charI];
//                    if(c==0)
//                        break;
//                    target[targetI++] = c;
//                }
//                target[targetI++] = US;
//            }
//        }

        int i;
        for(i=0; i<targetLen-2; ++i){
            char c = source[i];
            if(c==0 || c==GS){
                target[i]=GS;
                target[i+1]=0;
                return;
            }
            else{
                target[i]=c;
            }
        }
    }

    size_t StringTools::len(const char *source) {
        for(size_t i=0; i<LOOPKILLER; ++i){
            char c = source[i];
            if(c==0 || c==GS){
                return i+1;
            }
        }
        PDBG(TAG,"could not find end of string");
    }

}