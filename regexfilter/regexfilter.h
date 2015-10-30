struct ltstr
{
  bool operator()(std::wstring s1, std::wstring s2) const
  {
    return s1 < s2;
  }
};

#define USE_XPRESSIVE

#include <iostream>

#ifdef USE_XPRESSIVE
#include <boost/xpressive/xpressive.hpp>
#define REGEXVARIANT boost::xpressive::wsregex
#define REGEXMATCH   boost::xpressive::smatch
#define REGEX_REPLACE boost::xpressive::regex_replace
#else
#include <boost/regex.hpp>
#define REGEXVARIANT boost::wregex
#define REGEXMATCH   boost::smatch
#define REGEX_REPLACE regex_replace
#endif


bool filterInput(std::string *rulesFileName, std::vector<REGEXVARIANT *> *regexStore, std::wistream  &instrm, std::wstring &output);
bool readRules(std::string *fileName, std::vector<REGEXVARIANT *> *regexStore);
void cleanRules(std::vector<REGEXVARIANT *> *regexStore);

