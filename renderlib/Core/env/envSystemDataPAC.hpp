/**********************************************************
**  envSystemDataPAC.hpp
**
**      envSystemDataPAC forwards the envSystemData calls
**	to the PAC components
**
**
**
\*********************************************************/

#ifdef ENV_SYSTEMDATAPAC_HPP
#error envSystemDataPAC.hpp multiply included
#endif
#define ENV_SYSTEMDATAPAC_HPP

#ifndef ENV_PLATFORM_HPP
#include "Core/env/envPlatform.hpp"
#endif

//	Here is the interface that must be supported by the PAC component:
//

// namespace envSystemDataPAC
//{
//	void Init();
//	void CleanUp();
// }

#if ENV_WINDOWS
#include "Core/env/envSystemDataPACWin.hpp"
#else
#if ENV_OS == ENV_PS2OS
#include "Core/env/envSystemDataPACPS2.hpp"
#else
#if ENV_OS == ENV_XBOXOS
#include "Core/env/envSystemDataPACXbox.hpp"
#else
#error envSystemDataPAC not defined for this platform
#endif
#endif
#endif
