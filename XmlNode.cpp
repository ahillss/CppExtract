#include "XmlNode.h"

XmlNode::XmlNode(const std::string &name) : name(name) {
}

void XmlNode::setAttrib(const std::string &name, const std::string &value) {
  attribs[name]=value;
}

std::string XmlNode::getAttrib(const std::string &name) {
  return attribs[name];
}

XmlNode *XmlNode::addNode(XmlNode *n) {

  nodes.push_back(n);

  return n;
}

std::string XmlNode::toXml(int depth, bool pretty) {
  std::string depthStr;

  for(int i=0;i<depth*2;i++) {
    depthStr+=" ";
  }

  std::string outStr="";

  if(pretty) {
    outStr+=depthStr;
  }

  outStr+="<" + name;

  for(std::map<std::string,std::string>::iterator i=attribs.begin();i!=attribs.end();i++) {
    if(!i->second.empty()) {
      outStr+=" "+i->first+"=\""+i->second+"\"";
    }
  }

  if(!nodes.size()) {
    outStr+= " />";
  } else {
    outStr+= ">";

    for(std::list<XmlNode*>::iterator i=nodes.begin();i!=nodes.end();i++) {
      if(pretty) {
        outStr+= "\n";
      }

      outStr+= (*i)->toXml(depth+1,pretty);
    }

    if(pretty) {
      outStr+= "\n" + depthStr;
    }

    outStr+= "</" + name + ">";
  }

  return outStr;
}

std::string XmlNode::toSxml() {
  std::string outStr= "(" + name;

  bool attribsEmpty=true;

  for(std::map<std::string,std::string>::iterator i=attribs.begin();i!=attribs.end();i++) {
    if(!i->second.empty()) {
      attribsEmpty=false;
    }
  }

  if(!attribsEmpty) {
    outStr += " (@";

    for(std::map<std::string,std::string>::iterator i=attribs.begin();i!=attribs.end();i++) {
      if(!i->second.empty()) {
        outStr+=" ("+ i->first + " \"" + i->second +"\")";
      }
    }

    outStr += ")";
  }

  for(std::list<XmlNode*>::iterator i=nodes.begin();i!=nodes.end();i++) {
    outStr+= " " + (*i)->toSxml();
  }

  outStr += ")";
  return outStr;
}
