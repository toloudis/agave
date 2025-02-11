/*****************************************************************************
**  envSystemData.hpp
**
**      envSystemData exists to provide certain platform specific data about
**	system configuration.  The platform independent portion of this component
**	provides only Init and CleanUp functions.  PAC components may provide
**	other interfaces.
**
**
**
\****************************************************************************/

#ifdef ENV_SYSTEMDATA_HPP
#error envSystemData.hpp multiply included
#endif
#define ENV_SYSTEMDATA_HPP

//============================================================================
//============================================================================
namespace envSystemData {
//------------------------------------------------------------------------
//	Don't call Init() and CleanUp() yourself; they are called
//	by the package Init and Cleanup.
//------------------------------------------------------------------------
void
Init();

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
CleanUp() throw();
}
