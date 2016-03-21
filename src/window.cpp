/*
 * window.cpp
 *
 * A surface with expanded features (titles, borders, scrollbars, etc).
 */


#include <iostream>
#include <cmath>

#include "AnsiGL/window.h"


namespace AnsiGL
{
	void Window::Clear()
	{
		_Contents->Clear();
		this->RecalculateTotalContentSize();
	}

	const astring &Window::Title() const
	{
		return _Title->Value();
	}

	void Window::Title( const astring &title )
	{
		_Title->Value( title );
		_Title->Width( title.length() );
		_Title->Offset.X( _Title->Width() / 2 );	// Offset it so the origin is effectively in the center
	}

	const astring &Window::StatusText() const
	{
		return _StatusText->Value();
	}

	void Window::StatusText( const astring &text )
	{
		_StatusText->Value( text );
		_StatusText->Width( text.length() );
	}

	const tSizeType Window::Width() const
	{
		return Content::Width();
	}

	void Window::Width( tSizeType width )
	{
		Content::Width( width );
		this->updateWindow();
	}

	const tSizeType Window::Height() const
	{
		return Content::Height();
	}

	void Window::Height( tSizeType height )
	{
		Content::Height( height );
		this->updateWindow();
	}

	void Window::Resize( const Area2D &size )
	{
		Content::Resize( size );
		this->updateWindow();
	}

	void Window::SetContentWidth( tSizeType width )
	{
		Width( width + 2 );	// Cheats for now...just adds the border size
	}

	void Window::SetContentHeight( tSizeType height )
	{
		Height( height + 2 );	// Adds the border size
	}

	void Window::SetContentSize( Area2D size )
	{
		// This also cheats by adding the borders...
		Content::Width( size.Width() + 2 );
		Content::Height( size.Height() + 2 );
		this->updateWindow();
	}

	const Point3D &Window::CurViewportPos() const
	{
		return _Contents->CurViewportPos();
	}

	void Window::MoveViewportTo( const Point3D &pos )
	{
		FixedArea3D TotalContent = _Contents->TotalContentSize();
		FixedArea3D NewViewport( _Contents->Size(), pos );

		if ( (Area2D)TotalContent < (Area2D)NewViewport )
		{
			NewViewport.Point( TotalContent.Point() );
		}
		else
		{
			if ( NewViewport.X() < TotalContent.X() )
				NewViewport.X( TotalContent.X() );
			if ( NewViewport.Y() < TotalContent.Y() )
				NewViewport.Y( TotalContent.Y() );
			if ( NewViewport.Z() < TotalContent.Z() )
				NewViewport.Z( TotalContent.Z() );

			if ( NewViewport.X() + NewViewport.Width() > TotalContent.X() + TotalContent.Width()
			        && NewViewport.X() != TotalContent.X() )
				NewViewport.X( TotalContent.X() + TotalContent.Width() - NewViewport.Width() );
			if ( NewViewport.Y() + NewViewport.Height() > TotalContent.Y() + TotalContent.Height()
			        && NewViewport.Y() != TotalContent.Y() )
				NewViewport.Y( TotalContent.Y() + TotalContent.Height() - NewViewport.Height() );
			if ( NewViewport.Z() + NewViewport.Depth() > TotalContent.Z() + TotalContent.Depth()
			        && NewViewport.Z() != TotalContent.Z() )
				NewViewport.Z( TotalContent.Z() + TotalContent.Depth() - NewViewport.Depth() );
		}

		_Contents->MoveViewportTo( NewViewport.Point() );
		updateScrollbarPosition();
	}

	void Window::MoveViewportTo( const Point2D &pos )
	{
		FixedArea3D TotalContent = _Contents->TotalContentSize();
		FixedArea3D NewViewport( _Contents->Size(), pos );

		if ( NewViewport.X() < TotalContent.X() )
			NewViewport.X( TotalContent.X() );
		if ( NewViewport.Y() < TotalContent.Y() )
			NewViewport.Y( TotalContent.Y() );

		if ( NewViewport.X() + NewViewport.Width() > TotalContent.X() + TotalContent.Width() )
			NewViewport.X( TotalContent.X() + TotalContent.Width() - NewViewport.Width() );
		if ( NewViewport.Y() + NewViewport.Height() > TotalContent.Y() + TotalContent.Height() )
			NewViewport.Y( TotalContent.Y() + TotalContent.Height() - NewViewport.Height() );

		_Contents->MoveViewportTo( NewViewport.Point() );
		updateScrollbarPosition();
	}

	void Window::ResizeViewport( const Area2D &size )
	{
		SetContentSize( size );
		//m_Layout->Resize( size );
	}

	Context::Ptr Window::Contents() const
	{
		return _Contents;
	}

	void Window::AddContent( Content::Ptr content, const Point3D &pos )
	{
		if ( !content )
			return;

		_Contents->AddContent( content, pos );
		this->updateWindow();
	}

	void Window::RemoveContent( Content::Ptr content )
	{
		_Contents->RemoveContent( content );
		this->updateWindow();
	}

	void Window::RemoveContentFrom( tPointType depth, Content::Ptr content )
	{
		_Contents->RemoveContentFrom( depth, content );
		this->updateWindow();
	}

	void Window::RecalculateTotalContentSize()
	{
		_Contents->RecalculateTotalContentSize();
		updateScrollbars();
	}

	std::string Window::str()
	{
		return _Layout->str();
	}

	std::string Window::Render() const
	{
		return _Layout->Render();
	}

	void Window::RenderToSurface( Surface::Ptr dest, const Point2D &pos ) const
	{
#if 0
		// TODO: Make this temp surface a member of Window so it isn't continually created/destroyed
		// Create a temp surface that we can size to our "actual" size
		Surface::Ptr TempSurface( new Surface );
		TempSurface->Width( Width() );
		TempSurface->Height( Height() );

		_Layout->RenderToSurface( TempSurface );
		TempSurface->RenderToSurface( dest, pos, TransparentSpaces );
#else
		// If there is no render buffer, there isn't any content to render
		if ( !_RenderBuf )
			return;

		// Clear our buffer
		_RenderBuf->Clear();

		// Render our layout to that temp surface
		_Layout->RenderToSurface( _RenderBuf );

		// Then render the temp surface to the actual destination
		_RenderBuf->RenderToSurface( dest, pos, TransparentSpaces );
#endif // 0
	}

	void Window::updateWindow()
	{
		_Layout->ResizeViewport( _Size );
		_RenderBuf = Surface::Ptr( new Surface(_Size) );

		// Move the title to the center of the window
		_Title->MoveTo( Point2D( _Size.Width() / 2, 0) );

		// Put the content just inside the borders
		_Contents->MoveTo( Point2D(1, 1) );
		_Contents->ResizeViewport( Area2D(_Size.Width() - 2, _Size.Height() - 2) );

		// Move the status text to the proper location at the bottom-left
		_StatusText->MoveTo( Point2D(2, _Size.Height() - 2) );

		updateWindowBorders();
		updateScrollbars();
	}

	void Window::updateWindowBorders()
	{
		_Borders->Resize( _Size );
		_Borders->DrawBox( _Size );

		// Draw the corners
		_Borders->DrawChar( "+", Point2D(0, 0) );
		_Borders->DrawChar( "+", Point2D(_Size.Width() - 1, 0) );
		_Borders->DrawChar( "+", Point2D(0, _Size.Height() - 1) );
		_Borders->DrawChar( "+", Point2D(_Size.Width() - 1, _Size.Height() - 1) );
	}

	void Window::updateScrollbars()
	{
		updateScrollbarSize();
		updateScrollbarPosition();
	}

	void Window::updateScrollbarSize()
	{
		FixedArea3D TotalContent = _Contents->TotalContentSize();

		// Enforce minimums so we don't ever divide by 0
		tSizeType TotalContentWidth = TotalContent.Width();
		tSizeType TotalContentHeight = TotalContent.Height();
		TotalContentWidth = TotalContentWidth != 0 ? TotalContentWidth : 1;
		TotalContentHeight = TotalContentHeight != 0 ? TotalContentHeight : 1;

		FixedArea3D CurViewport;
		CurViewport.Point( _Contents->CurViewportPos() );
		CurViewport.Width( _Contents->Width() );
		CurViewport.Height( _Contents->Height() );

		Area2D MaxScrollbarSize( CurViewport.Width(), CurViewport.Height() );

		_HorizontalScrollbar->Width( MaxScrollbarSize.Width() * CurViewport.Width() / TotalContentWidth );

		// Make sure we always have at least a little scrollbar showing if needed
		if ( _HorizontalScrollbar->Width() <= 0 )
			_HorizontalScrollbar->Width( 1 );

		if ( _HorizontalScrollbar->Width() >= MaxScrollbarSize.Width() )
		{
			_HorizontalScrollbar->Visible( false );
			_HorizontalScrollbar->Width( MaxScrollbarSize.Width() );
		}
		else
			_HorizontalScrollbar->Visible( true );

		for ( int i = 0; i < _HorizontalScrollbar->Width(); ++i )
			_HorizontalScrollbar->DrawChar( "#", Point2D(i, 0) );

		// Set the size of the vertical scrollbar, and draw it to be sure it exists at the size we need
		_VerticalScrollbar->Height( MaxScrollbarSize.Height() * CurViewport.Height() / TotalContentHeight );

		// Make sure we always have at least a little scrollbar showing if needed
		if ( _VerticalScrollbar->Height() <= 0 )
			_VerticalScrollbar->Height( 1 );

		if ( _VerticalScrollbar->Height() >= MaxScrollbarSize.Height() )
		{
			_VerticalScrollbar->Visible( false );
			_VerticalScrollbar->Height( MaxScrollbarSize.Height() );
		}
		else
			_VerticalScrollbar->Visible( true );

		for ( int i = 0; i < _VerticalScrollbar->Height(); ++i )
			_VerticalScrollbar->DrawChar( "#", Point2D(0, i) );
	}

	void Window::updateScrollbarPosition()
	{
		FixedArea3D TotalContent = _Contents->TotalContentSize();

		// Enforce minimums so we don't ever divide by 0
		tSizeType TotalContentWidth = TotalContent.Width();
		tSizeType TotalContentHeight = TotalContent.Height();
		TotalContentWidth = TotalContentWidth != 0 ? TotalContentWidth : 1;
		TotalContentHeight = TotalContentHeight != 0 ? TotalContentHeight : 1;

		FixedArea3D CurViewport;
		CurViewport.Point( _Contents->CurViewportPos() );
		CurViewport.Width( _Contents->Width() );
		CurViewport.Height( _Contents->Height() );

		const int WIN_BORDER_WIDTH = 1;
		const int WIN_BORDER_HEIGHT = 1;

		int WidthDifference = TotalContentWidth - CurViewport.Width();
		int HeightDifference = TotalContentHeight - CurViewport.Height();

		if ( WidthDifference == 0 )
			_HorizontalScrollbar->MoveTo( Point2D((CurViewport.Width() - _HorizontalScrollbar->Width()) * CurViewport.X() + WIN_BORDER_WIDTH, CurViewport.Height() + 1) );
		else
			_HorizontalScrollbar->MoveTo( Point2D(((CurViewport.Width() - _HorizontalScrollbar->Width()) * CurViewport.X() / WidthDifference) + WIN_BORDER_WIDTH, CurViewport.Height() + 1) );

		if ( _HorizontalScrollbar->X() > CurViewport.Width() - _HorizontalScrollbar->Width() + 1 )
			_HorizontalScrollbar->MoveTo( Point2D((tPointType)(CurViewport.Width() - _HorizontalScrollbar->Width() + 1), _HorizontalScrollbar->Y()) );

		if ( HeightDifference == 0 )
			_VerticalScrollbar->MoveTo( Point2D(_Borders->Width() - 1, ((CurViewport.Height() - _VerticalScrollbar->Height()) * CurViewport.Y()) + WIN_BORDER_HEIGHT) );
		else
			_VerticalScrollbar->MoveTo( Point2D(_Borders->Width() - 1, ((CurViewport.Height() - _VerticalScrollbar->Height()) * CurViewport.Y() / HeightDifference) + WIN_BORDER_HEIGHT) );

		if ( _VerticalScrollbar->Y() > CurViewport.Height() - _VerticalScrollbar->Height() + 1 )
			_VerticalScrollbar->MoveTo( Point2D(_VerticalScrollbar->X(), (tPointType)(CurViewport.Height() - _VerticalScrollbar->Height() + 1)) );
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


