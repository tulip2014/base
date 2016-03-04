
#pragma once

#include <windows.h>
#include <tchar.h>
#include <string>
#include <map>
#include <Shlwapi.h>

#ifdef UNICODE
#define TString std::wstring
#else
#define TString std::string
#endif // UNICODE


namespace base
{
	namespace strings
	{
		TString trim_space(TString s)
		{
			if (s.empty())
			{
				return s;
			}

			std::wstring szTmp = s;
			std::wstring::size_type iPos = szTmp.find_first_not_of(_T(" "));
			if (iPos != 0)
			{
				szTmp.erase(0, iPos);
			}

			iPos = szTmp.find_last_not_of(_T(" "));
			if (iPos != szTmp.length() - 1)
			{
				szTmp.erase(iPos + 1);
			}

			iPos = szTmp.find_first_not_of(_T("	"));
			if (iPos != 0)
			{
				szTmp.erase(0, iPos);
			}

			iPos = szTmp.find_last_not_of(_T("	"));
			if (iPos != szTmp.length() - 1)
			{
				szTmp.erase(iPos + 1);
			}

			return szTmp;
		}

		class CMsCmdLine
		{
		public:
			CMsCmdLine(LPCTSTR lpszCmdLine = NULL)
			{
				Analyze(lpszCmdLine);
			}

			~CMsCmdLine()
			{
			}

			class CParam
			{
			public:
				CParam(LPCTSTR lpszValue)
				{
					m_strValue = lpszValue;
				}

				~CParam()
				{
				}

				TString String()
				{
					return m_strValue;
				}

				operator TString()
				{
					return m_strValue;
				}

				operator int()
				{
					return ::StrToInt(m_strValue.c_str());
				}

				operator bool()
				{
					return ::StrToInt(m_strValue.c_str()) ? true : false;
				}

			private:
				TString m_strValue;

			};

			BOOL Analyze(LPCTSTR lpszCmdLine)
			{
				// "{-|/}key1{=/:}value1" "{-|/}key2{=/:}value2" ...
				int num_args = 0;
				_TCHAR** args = CommandLineToArgvW(lpszCmdLine, &num_args);

				for (int i = 0; i < num_args; i++)
				{
					std::wstring szArg(args[i]);
					szArg = trim_space(szArg);

					if (szArg.size() > 1 && (szArg[0] == _T('-') || szArg[0] == _T('/')))
					{
						std::wstring szKey, szValue;

						std::wstring::size_type iPos = szArg.find(_T("="));
						if (iPos != std::wstring::npos)
						{
							if (iPos > 1)
							{
								szKey = szArg.substr(1, iPos - 1);
								if (iPos + 1 < szArg.length())
								{
									szValue = szArg.substr(iPos + 1, szArg.length());
								}
								else
								{
									szValue = _T("");
								}
							}
							else
							{
								_ASSERT(0);
								continue;
							}
						}
						else
						{
							iPos = szArg.find(_T(":"));
							if (iPos != std::wstring::npos)
							{
								if (iPos > 1)
								{
									szKey = szArg.substr(1, iPos - 1);
									if (iPos + 1 < szArg.length())
									{
										szValue = szArg.substr(iPos + 1, szArg.length());
									}
									else
									{
										szValue = _T("");
									}
								}
								else
								{
									_ASSERT(0);
									continue;
								}
							}
							else
							{
								szKey = szArg.substr(1, szArg.length());
								szValue = _T("");
							}
						}

						if (szKey.empty() == false)
						{
							SetParam(szKey.c_str(), szValue.c_str());
						}
						else
						{
							_ASSERT(0);
						}
					}
					else
					{
						_ASSERT(0);
						continue;
					}
				}

				LocalFree( args );

				return m_mapParams.size() > 0;
			}

			BOOL HasParam(LPCTSTR lpszParamName)
			{
				if (m_mapParams.find(lpszParamName) != m_mapParams.end( ))
					return TRUE;

				return FALSE;
			}

			BOOL SetParam(LPCTSTR lpszParamName, LPCTSTR lpszParamValue)
			{
				m_mapParams[lpszParamName] = lpszParamValue;

				return TRUE;
			}

			BOOL SetParam(LPCTSTR lpszParamName, int nValue)
			{
				TCHAR strNumber[1024] = {0};
				_itow_s(nValue,strNumber,10);
				m_mapParams[lpszParamName] = strNumber;

				return TRUE;
			}

			CParam operator[](LPCTSTR lpszParamName)
			{
				return CParam(getParam(lpszParamName).c_str());
			}

			std::wstring getParam(LPCTSTR sKey)
			{
				_CmdParamMap::iterator mapiter = m_mapParams.find(sKey);
				if (mapiter != m_mapParams.end())
				{
					return mapiter->second;
				}

				return L"";
			}

		protected:
			TString m_strCmdLine;

			typedef std::map<TString, TString> _CmdParamMap;
			_CmdParamMap m_mapParams;

		};

	}
}
