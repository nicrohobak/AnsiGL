/*
 * context.cpp
 *
 *
 */


#include "AnsiGL/context.h"
#include "AnsiGL/surface.h"


namespace AnsiGL
{
	Content::Ptr ContentMarker::Target() const
	{
		return _Content;
	}

	void ContentMarker::Target( Content::Ptr content )
	{
		_Content = content;
	}


	void Context::Clear()
	{
		_Contents.clear();
	}

	const Point3D &Context::CurViewportPos() const
	{
		return _ViewportPos;
	}

	void Context::MoveViewportTo( const Point3D &pos )
	{
		_ViewportPos = pos;
	}

	void Context::MoveViewportTo( const Point2D &pos )
	{
		_ViewportPos = pos;
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
		content->_Container = ContainerPtr;
		content->_ContentMarker = NewMarker;

		// Then add it to the proper list
		_Contents[ pos.Z() ].push_back( NewMarker );

		expandTotalContentSize( NewMarker );		// Expand our content, if needed
	}

	void Context::RemoveContent( Content::Ptr content )
	{
		if ( !content )
			return;

		for ( depth_iterator CurDepth = _Contents.begin(); CurDepth != _Contents.end(); ++CurDepth )
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
		if ( !content || _Contents[depth].empty() )
			return;

		for ( content_iterator CurContent = _Contents[depth].begin(); CurContent != _Contents[depth].end(); ++CurContent )
		{
			if ( (*CurContent)->Target() == content )
			{
				_Contents[depth].erase( CurContent );
				RecalculateTotalContentSize();			// Since we're removing content, we'd better recalculate the size
				return;
			}
		}
	}

	const FixedArea3D &Context::TotalContentSize()
	{
		if ( _NeedsSizeRecalc )
		{
			depth_iterator CurDepth;
			content_iterator CurContent;

			resetTotalContentSize();

			for ( CurDepth = _Contents.begin(); CurDepth != _Contents.end(); ++CurDepth )
			{
				for ( CurContent = CurDepth->second.begin(); CurContent != CurDepth->second.end(); ++CurContent )
					expandTotalContentSize( *CurContent );
			}

			// If we ended up with nothing, then set our position to 0,0 so we look more "null" when examined
			if ( _TotalContentSize.Width() == 0 || _TotalContentSize.Height() == 0 )
			{
				_TotalContentSize.X( 0 );
				_TotalContentSize.Y( 0 );
			}

			_NeedsSizeRecalc = false;
		}

		return _TotalContentSize;
	}

	void Context::RecalculateTotalContentSize()
	{
		_NeedsSizeRecalc = true;
	}

	bool Context::Contains( Content::Ptr content ) const
	{
		if ( !content )
			return false;

		for ( const_depth_iterator CurDepth = _Contents.begin(); CurDepth != _Contents.end(); ++CurDepth )
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
		Surface::Ptr Temp = Surface::Ptr( new Surface(Area2D(Content::_Size.Width(), Content::_Size.Height())) );

		// Render to the temp surface
		RenderToSurface( Temp );

		// Then render out to a string
		return Temp->str();
	}

	std::string Context::Render() const
	{
		// Create a temporary surface for rendering assistance
		Surface::Ptr Temp = Surface::Ptr( new Surface(_Size) );

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

		// Go through each thing in our _Contents
		for ( CurDepth = _Contents.rbegin(); CurDepth != _Contents.rend(); ++CurDepth )
		{
			// If we're above our viewport depth, then we can just skip it
			if ( CurDepth->first < _ViewportPos.Z() )
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
				Point2D CurRenderPos = ActualTargetPos - (Point2D)_ViewportPos + pos;

				// And render the content to the target
				Target->RenderToSurface( dest, CurRenderPos );
			}
		}
	}

	void Context::resetTotalContentSize()
	{
		_TotalContentSize.X( 0 );
		_TotalContentSize.Y( 0 );
		_TotalContentSize.Width( 0 );
		_TotalContentSize.Height( 0 );
	}

	void Context::expandTotalContentSize( ContentMarker::Ptr content )
	{
		if ( !content || !content->Target() )
			return;

		FixedArea3D ContentArea( content->Target()->Size(), content->Target()->CurPos() );

		// Apply the content's offset so we obtain visual area
		ContentArea.Point( (Point2D)ContentArea.Point() - content->Target()->Offset );

		// If the point is less in any dimension, then we need to move the _TotalContentSize point to match since it marks the "corner of minimums", and we need to increase the area by the difference
		if ( ContentArea.X() < _TotalContentSize.X() )
		{
			_TotalContentSize.X( ContentArea.X() );
			_TotalContentSize.Width( _TotalContentSize.Width() + (ContentArea.X() - _TotalContentSize.X()) );
		}

		if ( ContentArea.Y() < _TotalContentSize.Y() )
		{
			_TotalContentSize.Y( ContentArea.Y() );
			_TotalContentSize.Height( _TotalContentSize.Height() + (ContentArea.Y() - _TotalContentSize.Y()) );
		}

		if ( ContentArea.Z() < _TotalContentSize.Z() )
		{
			_TotalContentSize.Z( ContentArea.Z() );
			_TotalContentSize.Depth( _TotalContentSize.Depth() + (ContentArea.Z() - _TotalContentSize.Z()) );
		}

		// If the translated right-side of the content is further out than the translated right-side of our total content, then expand the "corner of maximums"

		Point3D ContentEdge( ContentArea.X() + ContentArea.Width(),
		                     ContentArea.Y() + ContentArea.Height(),
		                     ContentArea.Z() );

		Point3D TotalContentEdge( _TotalContentSize.X() + _TotalContentSize.Width(),
		                          _TotalContentSize.Y() + _TotalContentSize.Height(),
		                          _TotalContentSize.Depth() );

		// If the content is further out, then increase the width of the total content by the difference
		if ( ContentEdge.X() > TotalContentEdge.X() )
			_TotalContentSize.Width( _TotalContentSize.Width() + (ContentEdge.X() - TotalContentEdge.X()) );

		if ( ContentEdge.Y() > TotalContentEdge.Y() )
			_TotalContentSize.Height( _TotalContentSize.Height() + (ContentEdge.Y() - TotalContentEdge.Y()) );

		if ( ContentEdge.Z() > TotalContentEdge.Z() )
			_TotalContentSize.Depth( _TotalContentSize.Depth() + (ContentEdge.Z() - TotalContentEdge.Z()) );
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
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


