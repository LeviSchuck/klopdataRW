/* 
 * File:   dataRW.h
 * Author: kloplop321
 *
 * Created on July 5, 2011, 12:38 PM
 */
/*Copyright (c) <2011> <Levi (kloplop321.com)>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyomakne to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.*/
#ifndef DATARW_H
#define	DATARW_H
#include "data.h"
#ifndef NO_XML
#include "irrxml/irrXML.h"
#endif
namespace klop {
    class dataRW {
    public:
#ifndef NO_XML
        static void xmlToData(const std::string& location, Data& rootData);
        static void DataToxml(const std::string& location, Data& rootData);
        static const std::string dataToXML(const Data& root);
        static const std::string dataNodeToXML(const Data* node);
#endif
    private:
    };

}

#endif	/* DATARW_H */

