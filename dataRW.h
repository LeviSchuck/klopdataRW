/* 
 * File:   dataRW.h
 * Author: kloplop321
 *
 * Created on July 5, 2011, 12:38 PM
 */

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

