/* 
 * File:   dataRW.cpp
 * Author: kloplop321
 * 
 * Created on July 5, 2011, 12:38 PM
 */

#include "dataRW.h"
#include <string>
#include <sstream>
#include <stack>
#include <iostream>
#include <fstream>
namespace klop {
#ifndef NO_XML
    void dataRW::xmlToData(const std::string& location, Data& root){
        //Root is always associative.
        root.toAssoc();
        //printf("Root is setup now.\n");
        std::stack<Data*> tree;
        tree.push(&root);
        Data* currentData = &root;
        unsigned long int elementCount = 0;
        //printf("Preparing XML reader.\n");
        irr::io::IrrXMLReader* xml = 
                irr::io::createIrrXMLReader(location.c_str());
        //printf("XML Reader ready, lets go through.\n");
        while(xml && xml->read()){
            switch(xml->getNodeType()){
                case irr::io::EXN_ELEMENT:
                {
                    if(elementCount == 0){
                        elementCount++;//skip initating the root.
                        break;//this probably will screw me over for invalid
                        //xml parsed.
                    }
                    //printf("Hit element: %s\n",xml->getNodeName());
                    const char* type = xml->getAttributeValueSafe("type");
                    if(currentData->myType == kinds::nullData){
                        currentData->toAssoc();//We are by default associative
                    }
                    if(strcmp(type,"") == 0)
                    {//we either contain text, or more nodes.
                        //We can't really determine that yet
                        //but we can totally change the type from null data
                        //later as we are friendly.
                        Data* newData = new Data;
                        if(currentData->myArrayType == kinds::byAssociation){
                            currentData->addChild(
                                    std::string(xml->getNodeName()),
                                    newData);
                        }
                        if(currentData->myArrayType == kinds::byIndex){
                            currentData->addChild(newData);
                        }
                        if(!xml->isEmptyElement()){
                            currentData = newData;
                            tree.push(newData);
                        }
                    }
                    //At this point, the type must be a value, so what is it?
                    //We are limited by our reader in this case, so our 
                    //possibilities are reduced to int, double
                    if(strcmp(type,"int") == 0){
                        //printf("Got an int.\n");
                        Data newData(xml->getAttributeValueAsInt("value"));
                        if(currentData->myArrayType == kinds::byAssociation){
                            currentData->addChild(
                                    std::string(xml->getNodeName()),
                                    newData);
                        }
                        if(currentData->myArrayType == kinds::byIndex){
                            currentData->addChild(newData);
                        }
                    }
                    if(strcmp(type,"float")== 0){
                        //printf("Got a floating point value.\n");
                        Data newData((double)xml->getAttributeValueAsFloat("value"));
                        if(currentData->myArrayType == kinds::byAssociation){
                            currentData->addChild(
                                    std::string(xml->getNodeName()),
                                    newData);
                        }
                        if(currentData->myArrayType == kinds::byIndex){
                            currentData->addChild(newData);
                        }
                    }
                    if(strcmp(type,"list") == 0)
                    {//we either contain text, or more nodes.
                        //We can't really determine that yet
                        //but we can totally change the type from null data
                        //later as we are friendly.
                        Data* newData = new Data;
                        newData->toList();
                        if(currentData->myArrayType == kinds::byAssociation){
                            currentData->addChild(
                                    std::string(xml->getNodeName()),
                                    newData);
                        }
                        if(currentData->myArrayType == kinds::byIndex){
                            currentData->addChild(newData);
                        }
                        currentData = newData;
                        tree.push(newData);
                    }
                }
                    break;
                case irr::io::EXN_ELEMENT_END:
                    if(currentData != &root){
                        //printf("Element %s has ended.\n", xml->getNodeName());
                        tree.pop();
                        currentData = tree.top();
                    }
                    break;
                case irr::io::EXN_TEXT:
                {
                    std::string theValue = std::string(xml->getNodeData());
                    std::string trimmedValue = std::string(theValue);
                    trimmedValue = trimmedValue.erase(
                            trimmedValue.find_last_not_of(" \n\r\t")+1);
                    trimmedValue = trimmedValue.erase(0,
                            trimmedValue.find_first_not_of(" \n\r\t"));
                    if(trimmedValue.length() == 0)
                        break;//We have no need to put in whitespace.
                    //printf("Hit text of value \"%s\"\n",trimmedValue.c_str());
                    if(currentData->myType == kinds::nullData){
                        //Okay, we are totally cool with putting a string in.
                        currentData->stringData = trimmedValue;
                        currentData->myType = kinds::stringData;
                    }else{
                        //Hmm... Well, we must have other elements around..
                        //How do we identify it?
                        //If we are a list, this will be quite easy as they will
                        //just go through indexes anyway. 
                        //But, what if they have by association?
                        //Ignore it, that's what I'll do. Sorry folks.
                        if(currentData->myArrayType == kinds::byIndex){
                            currentData->addChild(Data(trimmedValue));
                        }
                        //Otherwise.. ignore.
                    }
                }
                    break;
            }
        }
        delete xml;
    }
    void dataRW::DataToxml(const std::string& location, Data& root){
        //Unfortunately, it seems like irrXML does not natively have a writer
        //we basically get our data from another function which makes a string 
        //instead of saving it immediately, in case it is desired to be used
        //somewhere else.
        printf("Writing to %s.\n", location.c_str());
        std::ofstream fout(location.c_str());
        fout << dataToXML(root).c_str();
        fout.close();
    }
    const std::string dataRW::dataToXML(const Data& root){
        std::stringstream xml;
        xml << "<root>";
        xml << dataNodeToXML(&root);
        xml << "</root>";
        return xml.str();
    }
    const std::string dataRW::dataNodeToXML(const Data* node){
        //printf("Dumping Node.\n");
        std::stringstream xml;
        if(node->arrayType() == kinds::byAssociation){
            //printf("Current node is association.\n");
            Data::ObjectValues::const_iterator it;
            for(it = node->myMap.begin(); it != node->myMap.end(); it++){
                const std::string elementName = (*it).first;
                //printf("Checking next child[%s].\n",elementName.c_str());
                xml << "<" << elementName.c_str();
                const Data* child = (*it).second;
                if(child->isNumeric()){
                    if(child->dataType() == kinds::doubleData){
                        //we are float
                        xml << " type=\"float\" value=\"" 
                                << child->asDouble() << "\"";
                    }else{
                        //we are int.
                        xml << " type=\"int\" value=\""
                                << child->asDouble() << "\"";
                    }
                    xml << ">";
                }
                if(child->dataType() == kinds::stringData){
                    //printf("Child Node is string.\n");
                    xml << ">" << child->asString() << "</"
                            << elementName << ">";
                }
                if(child->isData()){
                    //printf("Child node is Data.\n");
                    if(child->arrayType() == kinds::byIndex){
                        xml << " type=\"list\"";
                    }
                    xml << ">" << dataNodeToXML(child) << "</"
                            << elementName << ">";
                }
            }
        }else if(node->arrayType() == kinds::byIndex){
            //printf("Current node is a list\n");
            Data::ObjectList::const_iterator it;
            for(it = node->myList.begin(); it != node->myList.end(); it++){
                xml << "<item ";
                const Data* child = (*it).second;
                if(child->isNumeric()){
                    if(child->dataType() == kinds::doubleData){
                        //we are float
                        xml << " type=\"float\" value=\"" 
                                << child->asDouble() << "\"";
                    }else{
                        //we are int.
                        xml << " type=\"int\" value=\"" 
                                << child->asInt() << "\"";
                    }
                    xml << " />";
                }
                if(child->dataType() == kinds::stringData){
                    xml << ">" << child->asString() << "</item>";
                }
                if(child->isData()){
                    xml << ">" << dataNodeToXML(child) << "</item>";
                }
            }
        }else{
            printf("Node is unknown. Numerical: %d Reports: \"%s\"\n", 
                    node->isNumeric(),
                    node->asString().c_str());
            
        }
        
        return xml.str();
    }
#endif
}

