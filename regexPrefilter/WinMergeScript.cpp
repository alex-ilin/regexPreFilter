// WinMergeScript.cpp : Implementation of CWinMergeScript
#include "stdafx.h"
#include "regexPrefilter.h"
#include "WinMergeScript.h"
#include <string>
#include <iostream>
#include <sstream>
#include "regexfilter.h"

std::vector<REGEXVARIANT *> regexStore;
/////////////////////////////////////////////////////////////////////////////
// CWinMergeScript


STDMETHODIMP CWinMergeScript::get_PluginEvent(BSTR *pVal)
{
  *pVal = SysAllocString(L"BUFFER_PREDIFF");
  return S_OK;
}

STDMETHODIMP CWinMergeScript::get_PluginDescription(BSTR *pVal)
{
  *pVal = SysAllocString(L"Prediffer plug-in that uses Regular Expressions to hide unimportant differences in your sources.");
  return S_OK;
}

STDMETHODIMP CWinMergeScript::get_PluginFileFilters(BSTR *pVal)
{
  *pVal = SysAllocString(L"\\.cpp$;\\.cxx$;\\.h$;\\.hxx$;\\.c$;\\.php$;\\.js$");
  return S_OK;
}

STDMETHODIMP CWinMergeScript::get_PluginIsAutomatic(VARIANT_BOOL *pVal)
{
  *pVal = VARIANT_TRUE;
  return S_OK;
}


STDMETHODIMP CWinMergeScript::UnpackBufferA(SAFEARRAY **pBuffer, INT *pSize, VARIANT_BOOL *pbChanged, INT *pSubcode, VARIANT_BOOL *pbSuccess)
{
  // We don't need it
  return S_OK;
}

STDMETHODIMP CWinMergeScript::PackBufferA(SAFEARRAY **pBuffer, INT *pSize, VARIANT_BOOL *pbChanged, INT subcode, VARIANT_BOOL *pbSuccess)
{
  // We don't need it
  return S_OK;
}


STDMETHODIMP CWinMergeScript::UnpackFile(BSTR fileSrc, BSTR fileDst, VARIANT_BOOL *pbChanged, INT *pSubcode, VARIANT_BOOL *pbSuccess)
{
  // We don't need it
  return S_OK;
}



STDMETHODIMP CWinMergeScript::PackFile(BSTR fileSrc, BSTR fileDst, VARIANT_BOOL *pbChanged, INT pSubcode, VARIANT_BOOL *pbSuccess)
{
  // always return error so the users knows we can not repack
  *pbChanged = VARIANT_FALSE;
  *pbSuccess = VARIANT_FALSE;
  return S_OK;
}





bool cvtLPW2stdstring(std::string& s, const LPWSTR pw,
                      UINT codepage = CP_ACP)
{
    bool res = false;
    char* p = 0;
    int bsz;

    bsz = WideCharToMultiByte(codepage,
        0,
        pw,-1,
        0,0,
        0,0);
    if (bsz > 0) {
        p = new char[bsz];
        int rc = WideCharToMultiByte(codepage,
            0,
            pw,-1,
            p,bsz,
            0,0);
        if (rc != 0) {
            p[bsz-1] = 0;
            s = p;
            res = true;
        }
    }
    delete [] p;
    return res;
}

int main()
{
    wchar_t msg[] = L"Hello, world!";
    std::string s("\?");

    cvtLPW2stdstring(s,msg);
    std::cout << s << std::endl;

    return 0;
}

STDMETHODIMP CWinMergeScript::PrediffBufferW(BSTR *pText, INT *pSize, VARIANT_BOOL *pbChanged, VARIANT_BOOL *pbHandled)
{
#ifdef _DEBUG
  __asm  { int 3 };
#endif
  
  LPWSTR p;
  p=*pText;

  UINT N=*pSize;

  UINT j=0;
  UINT bol=0, sbol=0;

  std::wstring filteredInput;
  std::wistringstream  instrm(p);

  // filter the string now
  bool res = filterInput(&regexStore, instrm,filteredInput);

  // check the processing result
  if (res == false)
  {
    *pbChanged = VARIANT_FALSE;

    *pbHandled = VARIANT_TRUE;
    return S_OK;
  }

  // You asked for a pointer
  const wchar_t *pout = filteredInput.c_str();

  // copy the output
  wcscpy(p, pout);

  // set the length
  j = wcslen(pout);

  // write the output back
  p[j]=0;
#ifdef _DEBUG

  if(OpenClipboard(NULL))
  {
    EmptyClipboard();

    HGLOBAL hG=GlobalAlloc(GMEM_DDESHARE|GMEM_MOVEABLE, (j+1)*2);
    CopyMemory(GlobalLock(hG), p,(j+1)*2);
    GlobalUnlock(hG);
    SetClipboardData(CF_UNICODETEXT, (HANDLE)hG);
    CloseClipboard();
  }
#endif

  // set the new size
  *pSize = j;

  if (j == N)
    *pbChanged = VARIANT_FALSE;
  else
    *pbChanged = VARIANT_TRUE;

  *pbHandled = VARIANT_TRUE;
  return S_OK;
}

