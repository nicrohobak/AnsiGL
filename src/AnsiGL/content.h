#ifndef __ANSIGL_CONTENT_H__
#define __ANSIGL_CONTENT_H__

/*
 * content.h
 *
 * Content is the base class for all visible AnsiGL objects and object containers...anything that will be on-screen "content" should be based on this class.
 */


#include <memory>

#include "area.h"
#include "surface.h"


namespace AnsiGL
{
	// Forward declaration so we can have a proper pointer to our context
	class ContentMarker;
	class Context;
	typedef std::shared_ptr< ContentMarker >	ContentMarker_Ptr;
	typedef std::weak_ptr< ContentMarker >		ContentMarker_wPtr;
	typedef std::shared_ptr< Context >			Context_Ptr;
	typedef std::weak_ptr< Context >			Context_wPtr;


	class Content : public std::enable_shared_from_this< Content >
	{
	public:
		ANSIGL_POINTERS( Content )

	public:
		Point2D				Offset;					// X/Y offset from the origin for rendering

	public:
		Content():
			_Visible(true)
		{
		}

		Content( Context_Ptr container ):
			_Container( container )
		{
		}

		virtual ~Content()
		{
		}

		Context_Ptr Container() const;
		void Container( Context_Ptr container );

		// This function provides a nicer interface to dynamic casting
		// Ex: DesiredType::Ptr = BaseType->Morph< DesiredType >();
		template <class tContentType>
		std::shared_ptr< tContentType > Morph()
		{
			std::shared_ptr< tContentType > MorphedContent = std::dynamic_pointer_cast< tContentType >( shared_from_this() );
			return MorphedContent;
		}

		virtual const Point3D CurPos() const;
		virtual const tPointType X() const;
		virtual const tPointType Y() const;
		virtual const tPointType Z() const;
		virtual void Move( const Point3D &pos );	// Offset move
		virtual void Move( const Point2D &pos );
		virtual void MoveTo( const Point3D &pos );	// Absolute Move
		virtual void MoveTo( const Point2D &pos );

		virtual const Area2D &Size() const;
		virtual const tSizeType Width() const;
		virtual const tSizeType Height() const;
		virtual void Width( tSizeType width );
		virtual void Height( tSizeType height );
		virtual void Resize( const Area2D &size );	// Must set out _Size appropriately!

		virtual bool Visible() const;
		virtual void Visible( bool visible );

		virtual void RemoveFromContext();			// Removes ourself from our parent context, if we're within one

		virtual std::string str() = 0;				// Renders this content to a std::string, ANSI and everything off by default
		virtual std::string Render() const = 0;		// Renders this content to a std::string, but with ANSI and everything on
		virtual void RenderToSurface( Surface::Ptr dest, const Point2D &pos = Point2D() ) const = 0;	// Renders this content to a surface

	protected:
		Context_wPtr		_Container;				// A cached pointer to the context we reside within
		ContentMarker_wPtr	_ContentMarker;			// For faster access to our position information

		Area2D				_Size;					// The rough size of this content

		bool				_Visible;				// Is this visible within the context?

	protected:
		void triggerContextRecalc() const;

		// Allow context to access _Position to adjust the Z position
		friend class Context;
	};


	std::ostream &operator<<( std::ostream &left, const Content &right );
}


#endif // __ANSIGL_CONTENT_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


