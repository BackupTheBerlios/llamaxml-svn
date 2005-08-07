#ifndef LLAMAXMLWRITER_H
#define LLAMAXMLWRITER_H

#pragma once

#include "DataOutputStream.h"
#include <vector>
#include <string>

namespace LlamaXML {

	class XMLWriter {
	public:
		XMLWriter(DataOutputStream & output);
		~XMLWriter();
		
		void StartDocument(const char * version,
                           const char * encoding,
                           const char * standalone);
        void EndDocument();
        
        void StartElement(const char * name);
        void StartElement(const char * prefix, const char * name,
        	const char * namespaceURI = 0);
        void EndElement();
        
        template <class T>
        void WriteElement(const char * name, const T & value)
		{
			StartElement(name);
			*this << value;
			EndElement();
		}
        
        void WriteString(const char * content);
        
        void StartAttribute(const char * name);
        void StartAttribute(const char * prefix, const char * name,
                            const char * namespaceURI = 0);
        void EndAttribute();
        
        template <class T>
        void WriteAttribute(const char * name, const T & value)
		{
			StartAttribute(name);
			*this << value;
			EndAttribute();
		}
    
    private:
    	enum State {
    		kStateNormal,
    		kStateOpenTag,
    		kStateOpenAttribute
    	};
	
	private:
		DataOutputStream &			mOutput;
		State						mState;
		std::vector<std::string>	mElementStack;
	};

}


LlamaXML::XMLWriter & operator << (LlamaXML::XMLWriter & output,
	const char * s);
LlamaXML::XMLWriter & operator << (LlamaXML::XMLWriter & output,
	unsigned long long n);


#endif
