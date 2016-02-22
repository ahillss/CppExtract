#include <string>
#include <list>
#include <map>

class XmlNode {
 private:
  std::string name;
  std::map<std::string,std::string> attribs;
  std::list<XmlNode*> nodes;
 public:
  XmlNode(const std::string &name);
  void setAttrib(const std::string &name, const std::string &value);
  std::string getAttrib(const std::string &name);
  XmlNode *addNode(XmlNode *node);
  std::string toXml(int depth, bool pretty);
  std::string toSxml();
};
