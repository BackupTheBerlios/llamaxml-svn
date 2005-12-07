#include "LlamaXML/XMLReader.h"
#include "LlamaXML/XMLException.h"
#include <string>

// This code compiles but is untested

std::string ParseRSS(LlamaXML::InputStream & input, LlamaXML::TextEncoding desiredEncoding) {
    std::string output;
    
    try {
        LlamaXML::XMLReader reader(input);
        
        // Root node must be "rss"
        reader.ReadStartElement("rss");
        // rss node must contain exactly one channel node
        reader.ReadStartElement("channel");
        
        if (reader.IsNotEmptyElementRead()) {
            while (reader.MoveToSubElement()) {
                if (reader.IsStartElement("item")) {
                    if (reader.IsNotEmptyElementRead()) {
				        output += "--- item:\n";
                        while (reader.MoveToSubElement()) {
                            if (reader.IsStartElement("title")) {
    							output += "title: ";
    							output += reader.ReadElementString(desiredEncoding);
    							output += "\n";
								reader.Read();	// end element or empty element
                            }
                            else if (reader.IsStartElement("severity")) {
                                output += "sev: ";
							    output += reader.ReadElementString(desiredEncoding);
							    output += "\n";
								reader.Read(); // end element or empty element
                            }
                            else {
                                reader.Skip();
                            }
                        }
                    }
                }
                else {
                    reader.Skip();
                }
            }
        }
        
        reader.ReadEndElement();    // </channel>
        reader.ReadEndElement();    // </rss>
    }
    catch (const LlamaXML::XMLException & err) {
        output = "XMLException thrown from file ";
        output += err.File();
    }
    
    return output;
}
