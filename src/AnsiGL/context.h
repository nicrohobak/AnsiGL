#ifndef __ANSIGL_CONTEXT__
#define __ANSIGL_CONTEXT__

/*
 * context.h
 *
 * A context is what gives content position relative to one another...essentially a container with spacial coordinates.
 */


#include <list>
#include <map>

#include "content.h"
#include "point.h"		// for tPointType


namespace AnsiGL
{
	//
	// Content markers serve as a handle to our content
	//
	class ContentMarker : public Point3D
	{
	public:
		ANSIGL_POINTERS_AND_LISTS( ContentMarker )

	public:
		ContentMarker();
		ContentMarker( Content::Ptr content, const Point3D &pos = Point3D() );
		virtual ~ContentMarker();

		Content::Ptr Target() const;
		void Target( Content::Ptr content );

	protected:
		Content::Ptr			_Content;
	};


	// Contexts are a type of content...virtual inheritance is for our Screen class (and others like it) that need to be both a surface as well as context.
	class Context : virtual public Content
	{
	protected:
		typedef std::map< tPointType, ContentMarker::List >	tContentList;

	public:
		ANSIGL_POINTERS( Context )

		typedef tContentList::iterator				depth_iterator;
		typedef tContentList::const_iterator			const_depth_iterator;
		typedef tContentList::reverse_iterator			reverse_depth_iterator;
		typedef tContentList::const_reverse_iterator		const_reverse_depth_iterator;

		typedef ContentMarker::List::iterator			content_iterator;
		typedef ContentMarker::List::const_iterator		const_content_iterator;
		typedef ContentMarker::List::reverse_iterator		reverse_content_iterator;
		typedef ContentMarker::List::const_reverse_iterator	const_reverse_content_iterator;

	public:
		Context();
		Context( const Area2D &viewportSize, const Point3D &viewportPos = Point3D() );
		virtual ~Context();

		virtual const Point3D &CurViewportPos() const;
		virtual void MoveViewportTo( const Point3D &pos );
		virtual void MoveViewportTo( const Point2D &pos );

		virtual void ResizeViewport( const Area2D &size );	// Simply calls Resize()

		virtual void AddContent( Content::Ptr content, const Point3D &pos = Point3D() );
		virtual void RemoveContent( Content::Ptr content );
		virtual void RemoveContentFrom( tPointType depth, Content::Ptr content );	// Removes content from a specific depth
		virtual const FixedArea3D &TotalContentSize();
		virtual void RecalculateTotalContentSize();			// Recalculates _TotalContentSize from scratch
		virtual bool Contains( Content::Ptr content ) const;	// Checks to see if specific content exists within this context

		virtual std::string str();
		virtual std::string Render() const;
		virtual void RenderToSurface( Surface::Ptr dest, const Point2D &pos = Point() ) const;

	protected:
		tContentList		_Contents;						// int for z position, with each one being a list of contents

		Point3D				_ViewportPos;					// The position of the viewport within this context-space

		FixedArea3D			_TotalContentSize;				// Effectively a bounding box that encompasses all of the contents

		bool				_NeedsSizeRecalc;

		void resetTotalContentSize();
		void expandTotalContentSize( ContentMarker::Ptr content );	// Expands _TotalContentSize (if needed) to fit the content
		tSizeType cappedAdd( tSizeType first, tSizeType second );	// Adds the two together, making sure the variable doesn't wrap
		tSizeType cappedSub( tSizeType first, tSizeType second );	// Subtracts 'second' from 'first', making sure the variable doesn't wrap

		// Allow content to access _Contents directly so it can perform move/remove/etc. operations in a programatically-sane fashion
		friend class Content;
	};
}


#endif // __ANSIGL_CONTEXT__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


