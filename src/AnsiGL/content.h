#ifndef __ANSIGL_CONTENT_H__
#define __ANSIGL_CONTENT_H__

/*
 * content.h
 *
 * Content is the base class for all visible AnsiGL objects and object containers...anything that will be on-screen "content" should be based on this class.
 */


#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "area.h"
#include "surface.h"


namespace AnsiGL
{
    // Forward declaration so we can have a proper pointer to our context
    class ContentMarker;
    class Context;
    typedef boost::shared_ptr< ContentMarker >	ContentMarker_Ptr;
    typedef boost::weak_ptr< ContentMarker >	ContentMarker_wPtr;
    typedef boost::shared_ptr< Context >	Context_Ptr;
    typedef boost::weak_ptr< Context >		Context_wPtr;


    class Content : public boost::enable_shared_from_this< Content >
    {
    public:
	typedef boost::shared_ptr< Content >	Ptr;
	typedef boost::weak_ptr< Content >	wPtr;

    protected:
	Context_wPtr		m_Container;		// A cached pointer to the context we reside within
	ContentMarker_wPtr	m_ContentMarker;	// For faster access to our position information

	Area2D			m_Size;			// The rough size of this content

	bool			m_Visible;		// Is this visible within the context?

    public:
	Point2D			Offset;			// X/Y offset from the origin for rendering

    public:
	Content();
	Content( Context_Ptr container );
	virtual ~Content();

	Context_Ptr Container() const;
	void Container( Context_Ptr container );

	// This function provides a nicer interface to dynamic casting
	// Ex: DesiredType::Ptr = BaseType->Morph< DesiredType >();
	template <class tContentType>
	boost::shared_ptr< tContentType > Morph()
	{
	    boost::shared_ptr< tContentType > MorphedContent = boost::dynamic_pointer_cast< tContentType >( shared_from_this() );
	    return MorphedContent;
	}

	virtual const Point3D CurPos() const;
	virtual const tPointType X() const;
	virtual const tPointType Y() const;
	virtual const tPointType Z() const;
	virtual void Move( const Point3D &pos );		// Offset move
	virtual void Move( const Point2D &pos );
	virtual void MoveTo( const Point3D &pos );		// Absolute Move
	virtual void MoveTo( const Point2D &pos );

	virtual const Area2D &Size() const;
	virtual const tSizeType &Width() const;
	virtual const tSizeType &Height() const;
	virtual void Width( tSizeType width );
	virtual void Height( tSizeType height );
	virtual void Resize( const Area2D &size );		// Must set out m_Size appropriately!

	virtual bool Visible() const;
	virtual void Visible( bool visible );

	virtual void RemoveFromContext();			// Removes ourself from our parent context, if we're within one

	virtual std::string str() = 0;				// Renders this content to a std::string, ANSI and everything off by default
	virtual std::string Render() const = 0;			// Renders this content to a std::string, but with ANSI and everything on
	virtual void RenderToSurface( Surface::Ptr dest, const Point2D &pos = Point2D() ) const = 0;	// Renders this content to a surface

    protected:
	void triggerContextRecalc() const;

	// Allow context to access m_Position to adjust the Z position
	friend class Context;
    };


    std::ostream &operator<<( std::ostream &left, const Content &right );
}


#endif // __ANSIGL_CONTENT_H__


