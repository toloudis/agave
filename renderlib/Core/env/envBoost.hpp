/****************************************************************************\
**  envBoost.hpp
**
**      envBoost.hpp includes the boost smart pointer classes and
**	sets the using namespace so that we can use the classes directly.
**	The	idea is to avoid using "boost::" in our code so that we could
**	change it to use "std::" when it gets smart pointers, or to use
**	our own templates.
**
**
**
\****************************************************************************/
#pragma once
#ifdef ENV_BOOST_HPP
#error envBoost.hpp multiply included
#endif
#define ENV_BOOST_HPP

// Turn off managed compiler complaints about boost library

// Exposing just the shared and weak pointers for now.
#include <memory>
using std::shared_ptr;
using std::weak_ptr;
