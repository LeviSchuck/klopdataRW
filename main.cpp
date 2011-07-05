/* 
 * File:   main.cpp
 * Author: kloplop321
 *
 * Created on July 5, 2011, 11:05 AM
 */

#include <cstdlib>
#include <stdio.h>
#include "data.h"
#include "dataRW.h"
using namespace std;
using namespace klop;
/*
 * 
 */
int main(int argc, char** argv) {
    Data data;
    dataRW::xmlToData("demo.xhtml", data);
    printf("Size: %lu\n",data.size());
    printf("Physics size: %lu\n", data["physics"].size());
    printf("Physics dimensions depth : %f\n", 
            data["physics"]["dimensions"]["depth"].asDouble());
    printf("XML: %s\n",dataRW::dataToXML(data).c_str());
    dataRW::DataToxml("demo2.xml",data);
    return 0;
}

