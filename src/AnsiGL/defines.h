#ifndef __ANSIGL_DEFINES_H__
#define __ANSIGL_DEFINES_H__

/*
 * defines.h
 *
 * Generic, helpful definitions
 */


#include <list>
#include <memory>


namespace AnsiGL
{
#define MIN( a, b )		((a) < (b) ? (a) : (b))
#define MAX( a, b )		((a) > (b) ? (a) : (b))


#define ANSIGL_POINTERS( tType )			\
	typedef std::shared_ptr< tType >	Ptr;	\
	typedef std::weak_ptr< tType >		wPtr;

#define ANSIGL_POINTERS_AND_LISTS( tType )		\
	typedef std::shared_ptr< tType >	Ptr;	\
	typedef std::weak_ptr< tType >		wPtr;	\
	typedef std::list< Ptr >		List;	\
	typedef std::list< wPtr >		wList;

#define ANSIGL_PARENT( tType )			\
	typedef tType				tParent;
}


#endif // __ANSIGL_DEFINES_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


