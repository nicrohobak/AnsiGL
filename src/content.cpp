/*
 * content.cpp
 *
 * Content is the base class for all visible AnsiGL objets and object containers...anything that can be on-screen "content" should be based on this class.
 */


#include "AnsiGL/content.h"
#include "AnsiGL/context.h"


namespace AnsiGL
{
    Content::Content():
	m_Visible(true)
    {
    }

    Content::~Content()
    {
    }

    Context::Ptr Content::Container() const
    {
	return m_Container.lock();
    }

    const Point3D Content::CurPos() const
    {
	ContentMarker::Ptr MyMarker = m_ContentMarker.lock();
	Point3D CurrentPos;

	if ( MyMarker )
	    CurrentPos = *MyMarker;

	return CurrentPos;
    }

    const tPointType Content::X() const
    {
	Point3D CurrentPos = CurPos();
	return CurrentPos.X();
    }

    const tPointType Content::Y() const
    {
	Point3D CurrentPos = CurPos();
	return CurrentPos.Y();
    }

    const tPointType Content::Z() const
    {
	Point3D CurrentPos = CurPos();
	return CurrentPos.Z();
    }

    // Relative move
    void Content::Move( const Point3D &pos )
    {
	ContentMarker::Ptr MyMarker = m_ContentMarker.lock();

	if ( MyMarker )
	{
	    if ( pos.Z() != 0 )
	    {
		Context::Ptr MyContainer = m_Container.lock();

		// Move the content within the container
		if ( MyContainer )
		{
		    MyContainer->RemoveContentFrom( MyMarker->Z(), shared_from_this() );
		    MyContainer->AddContent( shared_from_this(), pos );
		}
		else
		{
		    // Shouldn't ever get here, but just in case
		    MyMarker->X( MyMarker->X() + pos.X() );
		    MyMarker->Y( MyMarker->Y() + pos.Y() );
		    MyMarker->Z( MyMarker->Z() + pos.Z() );
		}
	    }
	    else
	    {
		MyMarker->X( MyMarker->X() + pos.X() );
		MyMarker->Y( MyMarker->Y() + pos.Y() );
	    }
	}

	triggerContextRecalc();
    }

    // Relative move
    void Content::Move( const Point2D &pos )
    {
	ContentMarker::Ptr MyMarker = m_ContentMarker.lock();

	if ( MyMarker )
	{
	    MyMarker->X( MyMarker->X() + pos.X() );
	    MyMarker->Y( MyMarker->Y() + pos.Y() );
	}

	triggerContextRecalc();
    }
 
    // Absolute move
    void Content::MoveTo( const Point3D &pos )
    {
	ContentMarker::Ptr MyMarker = m_ContentMarker.lock();

	if ( MyMarker )
	{
	    if ( MyMarker->Z() != pos.Z() )
	    {
		Context::Ptr MyContainer = m_Container.lock();

		// Move the content within the container
		if ( MyContainer )
		{
		    MyContainer->RemoveContentFrom( MyMarker->Z(), shared_from_this() );
		    MyContainer->AddContent( shared_from_this(), pos );
		}
		else
		{
		    // Shouldn't ever get here, but just in case
		    MyMarker->X( pos.X() );
		    MyMarker->Y( pos.Y() );
		    MyMarker->Z( pos.Z() );
		}
	    }
	    else
	    {
		MyMarker->X( pos.X() );
		MyMarker->Y( pos.Y() );
	    }
	}

	triggerContextRecalc();
    }

    // Absolute move
    void Content::MoveTo( const Point2D &pos )
    {
	ContentMarker::Ptr MyMarker = m_ContentMarker.lock();

	if ( MyMarker )
	{
	    MyMarker->X( pos.X() );
	    MyMarker->Y( pos.Y() );
	}

	triggerContextRecalc();
    }

    const Area2D &Content::Size() const
    {
	return m_Size;
    }

    const tSizeType &Content::Width() const
    {
	return m_Size.Width();
    }

    const tSizeType &Content::Height() const
    {
	return m_Size.Height();
    }

    void Content::Width( tSizeType width )
    {
	m_Size.Width( width );
	triggerContextRecalc();
    }

    void Content::Height( tSizeType height )
    {
	m_Size.Height( height );
	triggerContextRecalc();
    }

    void Content::Resize( const Area2D &size )
    {
	m_Size = size;
	triggerContextRecalc();
    }

    bool Content::Visible() const
    {
	return m_Visible;
    }

    void Content::Visible( bool visible )
    {
	m_Visible = visible;
    }

    void Content::RemoveFromContext()
    {
	Context::Ptr CurContext = Container();

	if ( CurContext )
	    CurContext->RemoveContent( shared_from_this() );
    }

    void Content::triggerContextRecalc() const
    {
	Context::Ptr MyParent = m_Container.lock();

	if ( MyParent )
	    MyParent->RecalculateTotalContentSize();
    }


    std::ostream &operator<<( std::ostream &left, const Content &right )
    {
	left << right.Render();
	return left;
    }
}




