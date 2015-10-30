struct ltstr
{
  bool operator()(std::wstring s1, std::wstring s2) const
  {
    return s1 < s2;
  }
};


bool filterInput(std::map<std::wstring, boost::wregex *, ltstr> *regexStore, std::wistream  &instrm, std::wstring &output);
bool readRules(std::map<std::wstring, boost::wregex *, ltstr> *regexStore);
void cleanRules(std::map<std::wstring, boost::wregex *, ltstr> *regexStore);

