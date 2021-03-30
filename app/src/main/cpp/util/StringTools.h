//
// Created by dawnoftherent on 3/21/21.
//

#ifndef SIMPLEGROUPCOMMSERVER_STRINGTOOLS_H
#define SIMPLEGROUPCOMMSERVER_STRINGTOOLS_H

#include <unistd.h>

namespace com_curiousorigins_simplegroupcommserver {
    class StringTools {
    private:
        /**  US is the unit separator, individual components for each element are separated with this
          *  RS is the row, or record separator. individual elements in each transmission are separated with this
          *  GS is the group separator, it terminates the entire transmission */
        static char US, RS, GS;

    public:
        static void setGroupChars(char unitSeparator, char recordSeparator, char groupSeparator);
        static void fill(const char* source, char* target, size_t targetLen);
        static size_t len(const char* source);
    };
}

#endif //SIMPLEGROUPCOMMSERVER_STRINGTOOLS_H
