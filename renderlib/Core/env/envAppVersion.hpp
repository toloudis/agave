/****************************************************************************\
**	envAppVersion.hpp
**
**		envAppVersion.hpp supplies a class that can be used to version software.
**	The implementation is 4 integers: major, minor, revision, and build numbers.
**
**	Major.Minor.Revision.Build
**
**
**	Copyright(C) 2008-10 - All Rights Reserved
\****************************************************************************/
#ifdef ENV_APPVERSION_HPP
#error envAppVersion.hpp multiply included
#endif
#define ENV_APPVERSION_HPP

#ifndef ENV_TYPE_HPP
#include "Core/env/envType.hpp"
#endif

#include <string>

//============================================================================
//============================================================================
class envAppVersion
{
public:
  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  envAppVersion();
  envAppVersion(envType::Int32 i_vMaj, envType::Int32 i_vMin, envType::Int32 i_vRevision, envType::Int32 i_vBuild);
  explicit envAppVersion(const std::string& i_VerString);
  explicit envAppVersion(const char* i_VerString);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void Set(envType::Int32 i_vMaj, envType::Int32 i_vMin, envType::Int32 i_vRevision, envType::Int32 i_vBuild);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void Get(envType::Int32& o_vMaj, envType::Int32& o_vMin, envType::Int32& o_vRevision, envType::Int32& o_vBuild) const;
  envType::Int32 GetMajor() const;
  envType::Int32 GetMinor() const;
  envType::Int32 GetRevision() const;
  envType::Int32 GetBuild() const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  bool operator<(const envAppVersion& i_Other) const;
  bool operator<=(const envAppVersion& i_Other) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  bool operator>(const envAppVersion& i_Other) const;
  bool operator>=(const envAppVersion& i_Other) const;

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  bool operator==(const envAppVersion& i_Other);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  void SetFromString(const std::string& i_VerString);

  //--------------------------------------------------------------------
  //--------------------------------------------------------------------
  std::string GetString() const;

  //------------------------------------------------------------------------
  //	operator << performs writing to an ostream
  //------------------------------------------------------------------------
  friend std::ostream& operator<<(std::ostream& io_os, const envAppVersion& i_Version);

private:
  envType::Int32 m_Major, m_Minor, m_Revision, m_Build;
};
