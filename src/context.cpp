/*
 * context.cpp
 *
 *
 */


#include "AnsiGL/context.h"
#include "AnsiGL/surface.h"


namespace AnsiGL
{
	ContentMarker::ContentMarker()
	{
	}

	ContentMarker::ContentMarker( Content::Ptr content, const Point3D &pos ):
		Point3D(pos),
		m_Content(content)
	{
	}

	ContentMarker::~ContentMarker()
	{
	}

	Content::Ptr ContentMarker::Target() const
	{
		return m_Content;
	}

	void ContentMarker::Target( Content::Ptr content )
	{
		m_Content = content;
	}


	Context::Context():
		m_NeedsSizeRecalc(true)
	{
	}

	Context::Context( const Area2D &viewportSize, const Point3D &viewportPos ):
		m_NeedsSizeRecalc(true)
	{
		Resize( viewportSize );
		m_ViewportPos = viewportPos;
	}

	Context::~Context()
	{
	}

	const Point3D &Context::CurViewportPos() const
	{
		return m_ViewportPos;
	}

	void Context::MoveViewportTo( const Point3D &pos )
	{
		m_ViewportPos = pos;
	}

	void Context::MoveViewportTo( const Point2D &pos )
	{
		m_ViewportPos = pos;
	}

	void Context::ResizeViewport( const Area2D &size )
	{
		Resize( size );
	}

	void Context::AddContent( Content::Ptr content, const Point3D &pos )
	{
		if ( !content )
			return;

		ContentMarker::Ptr NewMarker = ContentMarker::Ptr( new ContentMarker(content, pos) );
		Context::Ptr ContainerPtr = std::dynamic_pointer_cast< Context >( shared_from_this() );

		// Make sure the content is updated with our info
		content->m_Container = ContainerPtr;
		content->m_ContentMarker = NewMarker;

		// Then add it to the proper list
		m_Contents[ pos.Z() ].push_back( NewMarker );

		expandTotalContentSize( NewMarker );		// Expand our content, if needed
	}

	void Context::RemoveContent( Content::Ptr content )
	{
		if ( !content )
			return;

		for ( depth_iterator CurDepth = m_Contents.begin(); CurDepth != m_Contents.end(); ++CurDepth )
		{
			for ( content_iterator CurContent = CurDepth->second.begin(); CurContent != CurDepth->second.end(); ++CurContent )
			{
				if ( (*CurContent)->Target() == content )
				{
					CurDepth->second.erase( CurContent );
					RecalculateTotalContentSize();		// Since we're removing content, we'd better recalculate the size
					return;
				}
			}
		}
	}

	void Context::RemoveContentFrom( tPointType depth, Content::Ptr content )
	{
		if ( !content || m_Contents[depth].empty() )
			return;

		for ( content_iterator CurContent = m_Contents[depth].begin(); CurContent != m_Contents[depth].end(); ++CurContent )
		{
			if ( (*CurContent)->Target() == content )
			{
				m_Contents[depth].erase( CurContent );
				RecalculateTotalContentSize();			// Since we're removing content, we'd better recalculate the size
				return;
			}
		}
	}

	const FixedArea3D &Context::TotalContentSize()
	{
		if ( m_NeedsSizeRecalc )
		{
			depth_iterator CurDepth;
			content_iterator CurContent;

			resetTotalContentSize();

			for ( CurDepth = m_Contents.begin(); CurDepth != m_Contents.end(); ++CurDepth )
			{
				for ( CurContent = CurDepth->second.begin(); CurContent != CurDepth->second.end(); ++CurContent )
					expandTotalContentSize( *CurContent );
			}

			// If we ended up with nothing, then set our position to 0,0 so we look more "null" when examined
			if ( m_TotalContentSize.Width() == 0 || m_TotalContentSize.Height() == 0 )
			{
				m_TotalContentSize.X( 0 );
				m_TotalContentSize.Y( 0 );
			}

			m_NeedsSizeRecalc = false;
		}

		return m_TotalContentSize;
	}

	void Context::RecalculateTotalContentSize()
	{
		m_NeedsSizeRecalc = true;
	}

	bool Context::Contains( Content::Ptr content ) const
	{
		if ( !content )
			return false;

		for ( const_depth_iterator CurDepth = m_Contents.begin(); CurDepth != m_Contents.end(); ++CurDepth )
		{
			for ( const_content_iterator CurContent = CurDepth->second.begin(); CurContent != CurDepth->second.end(); ++CurContent )
			{
				if ( (*CurContent)->Target() == content )
					return true;
			}
		}

		return false;
	}

	std::string Context::str()
	{
		// Create a temporary surface for rendering assistance
		Surface::Ptr Temp = Surface::Ptr( new Surface(Area2D(Content::m_Size.Width(), Content::m_Size.Height())) );

		// Render to the temp surface
		RenderToSurface( Temp );

		// Then render out to a string
		return Temp->str();
	}

	std::string Context::Render() const
	{
		// Create a temporary surface for rendering assistance
		Surface::Ptr Temp = Surface::Ptr( new Surface(m_Size) );

		// Render to the temp surface
		RenderToSurface( Temp );

		// Then render out to a string
		return Temp->Render();
	}

	void Context::RenderToSurface( Surface::Ptr dest, const Point2D &pos ) const
	{
		if ( !dest )
			return;

		Context::const_reverse_depth_iterator CurDepth;
		Context::const_content_iterator CurContent;

		// Go through each thing in our m_Contents
		for ( CurDepth = m_Contents.rbegin(); CurDepth != m_Contents.rend(); ++CurDepth )
		{
			// If we're above our viewport depth, then we can just skip it
			if ( CurDepth->first < m_ViewportPos.Z() )
				continue;

			for ( CurContent = CurDepth->second.begin(); CurContent != CurDepth->second.end(); ++CurContent )
			{
				Content::Ptr Target = (*CurContent)->Target();

				if ( !Target || !Target->Visible() )
					continue;

				// Get the content's offset position
				Point3D TargetPos = Target->CurPos();
				Point2D ActualTargetPos = (Point2D)TargetPos - Target->Offset;

				// Then adjust the render position to account for the viewport position and the desired location from the arguments
				Point2D CurRenderPos = ActualTargetPos - (Point2D)m_ViewportPos + pos;

				// And render the content to the target
				Target->RenderToSurface( dest, CurRenderPos );
			}
		}
	}

	void Context::resetTotalContentSize()
	{
		m_TotalContentSize.X( 0 );
		m_TotalContentSize.Y( 0 );
		m_TotalContentSize.Width( 0 );
		m_TotalContentSize.Height( 0 );
	}

	void Context::expandTotalContentSize( ContentMarker::Ptr content )
	{
		if ( !content || !content->Target() )
			return;

		FixedArea3D ContentArea( content->Target()->Size(), content->Target()->CurPos() );

		// Apply the content's offset so we obtain visual area
		ContentArea.Point( (Point2D)ContentArea.Point() - content->Target()->Offset );

		// If the point is less in any dimension, then we need to move the m_TotalContentSize point to match since it marks the "corner of minimums", and we need to increase the area by the difference
		if ( ContentArea.X() < m_TotalContentSize.X() )
		{
			m_TotalContentSize.X( ContentArea.X() );
			m_TotalContentSize.Width( m_TotalContentSize.Width() + (ContentArea.X() - m_TotalContentSize.X()) );
		}

		if ( ContentArea.Y() < m_TotalContentSize.Y() )
		{
			m_TotalContentSize.Y( ContentArea.Y() );
			m_TotalContentSize.Height( m_TotalContentSize.Height() + (ContentArea.Y() - m_TotalContentSize.Y()) );
		}

		if ( ContentArea.Z() < m_TotalContentSize.Z() )
		{
			m_TotalContentSize.Z( ContentArea.Z() );
			m_TotalContentSize.Depth( m_TotalContentSize.Depth() + (ContentArea.Z() - m_TotalContentSize.Z()) );
		}

		// If the translated right-side of the content is further out than the translated right-side of our total content, then expand the "corner of maximums"

		Point3D ContentEdge( ContentArea.X() + ContentArea.Width(),
		                     ContentArea.Y() + ContentArea.Height(),
		                     ContentArea.Z() );

		Point3D TotalContentEdge( m_TotalContentSize.X() + m_TotalContentSize.Width(),
		                          m_TotalContentSize.Y() + m_TotalContentSize.Height(),
		                          m_TotalContentSize.Depth() );

		// If the content is further out, then increase the width of the total content by the difference
		if ( ContentEdge.X() > TotalContentEdge.X() )
			m_TotalContentSize.Width( m_TotalContentSize.Width() + (ContentEdge.X() - TotalContentEdge.X()) );

		if ( ContentEdge.Y() > TotalContentEdge.Y() )
			m_TotalContentSize.Height( m_TotalContentSize.Height() + (ContentEdge.Y() - TotalContentEdge.Y()) );

		if ( ContentEdge.Z() > TotalContentEdge.Z() )
			m_TotalContentSize.Depth( m_TotalContentSize.Depth() + (ContentEdge.Z() - TotalContentEdge.Z()) );
	}

	tSizeType Context::cappedAdd( tSizeType first, tSizeType second )
	{
		tSizeType Result = first + second;

		// If the added result is smaller than either original number...cap it
		if ( Result < first || Result < second )
			return -1;	// Force this signed type to be the highest possible value

		return Result;
	}

	tSizeType Context::cappedSub( tSizeType first, tSizeType second )
	{
		tSizeType Result = first - second;

		// If the subtraction result is larger than either original number...cap it
		if ( Result > first || Result > second )
			return 0;	// Force this signed type to be the lowest possible value

		return Result;
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


