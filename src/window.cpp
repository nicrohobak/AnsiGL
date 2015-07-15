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
    Window::Window():
	m_Layout( new Context() ),
	m_Title( new Text() ),
	m_StatusText( new Text(astring("")) ),
	m_Contents( new Context() ),
	m_Borders( new Image() ),
	m_VerticalScrollbar( new Image() ),
	m_HorizontalScrollbar( new Image() ),
	TransparentSpaces( TRANSPARENT_DEFAULT )
    {
	m_Layout->AddContent( m_Contents, Point3D(0,0,3) );
	m_Layout->AddContent( m_StatusText, Point3D(0,0,2) );
	m_Layout->AddContent( m_Borders, Point3D(0,0,1) );
	m_Layout->AddContent( m_Title, Point3D(0,0,0) );
	m_Layout->AddContent( m_VerticalScrollbar, Point3D(0,0,0) );
	m_Layout->AddContent( m_HorizontalScrollbar, Point3D(0,0,0) );

	m_VerticalScrollbar->Width( 1 );
	m_HorizontalScrollbar->Height( 1 );
	m_VerticalScrollbar->TransparentSpaces = true;
	m_HorizontalScrollbar->TransparentSpaces = true;

	m_Borders->TransparentSpaces = true;
	m_Title->Align( TxtAlign_Center );
	m_Title->TransparentSpaces = false;
	Title( astring("Window Title") );
	m_StatusText->TransparentSpaces = false;
    }

    Window::Window( const astring &windowTitle, const Area2D &windowSize, const Point3D &viewportPos, bool transparentSpaces ):
	m_Layout( new Context() ),
	m_Title( new Text() ),
	m_StatusText( new Text(astring("")) ),
	m_Contents( new Context() ),
	m_Borders( new Image() ),
	m_VerticalScrollbar( new Image() ),
	m_HorizontalScrollbar( new Image() ),
	TransparentSpaces( transparentSpaces )
    {
	m_Layout->AddContent( m_Contents, Point3D(0,0,3) );
	m_Layout->AddContent( m_StatusText, Point3D(0,0,2) );
	m_Layout->AddContent( m_Borders, Point3D(0,0,1) );
	m_Layout->AddContent( m_Title, Point3D(0,0,0) );
	m_Layout->AddContent( m_VerticalScrollbar, Point3D(0,0,0) );
	m_Layout->AddContent( m_HorizontalScrollbar, Point3D(0,0,0) );

	m_VerticalScrollbar->Width( 1 );
	m_HorizontalScrollbar->Height( 1 );
	m_VerticalScrollbar->TransparentSpaces = true;
	m_HorizontalScrollbar->TransparentSpaces = true;

	Resize( windowSize );

	m_Borders->TransparentSpaces = true;
	m_Title->Align( TxtAlign_Center );
	m_Title->TransparentSpaces = false;
	Title( windowTitle );
	m_StatusText->TransparentSpaces = false;

	MoveViewportTo( viewportPos );
    }

    Window::~Window()
    {
    }

    const astring &Window::Title() const
    {
	return m_Title->Value();
    }

    void Window::Title( const astring &title )
    {
	m_Title->Value( title );
	m_Title->Width( title.length() + 2 );
	m_Title->Offset.X( (m_Title->Width() + 2) / 2 );	// Offset it so the origin is effectively in the center
    }

    const astring &Window::StatusText() const
    {
	return m_StatusText->Value();
    }

    void Window::StatusText( const astring &text )
    {
	m_StatusText->Value( text );
	m_StatusText->Width( text.length() );
    }

    const tSizeType &Window::Width() const
    {
	return Content::Width();
    }

    void Window::Width( tSizeType width )
    {
	Content::Width( width );
	updateWindow();
    }

    const tSizeType &Window::Height() const
    {
	return Content::Height();
    }

    void Window::Height( tSizeType height )
    {
	Content::Height( height );
	updateWindow();
    }

    void Window::Resize( const Area2D &size )
    {
	Content::Resize( size );
	updateWindow();
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
	updateWindow();
    }

    const Point3D &Window::CurViewportPos() const
    {
	return m_Contents->CurViewportPos();
    }

    void Window::MoveViewportTo( const Point3D &pos )
    {
	FixedArea3D TotalContent = m_Contents->TotalContentSize();
	FixedArea3D NewViewport( m_Contents->Size(), pos );

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

	m_Contents->MoveViewportTo( NewViewport.Point() );
	updateScrollbarPosition();
    }

    void Window::MoveViewportTo( const Point2D &pos )
    {
	FixedArea3D TotalContent = m_Contents->TotalContentSize();
	FixedArea3D NewViewport( m_Contents->Size(), pos );

	if ( NewViewport.X() < TotalContent.X() )
	    NewViewport.X( TotalContent.X() );
	if ( NewViewport.Y() < TotalContent.Y() )
	    NewViewport.Y( TotalContent.Y() );

	if ( NewViewport.X() + NewViewport.Width() > TotalContent.X() + TotalContent.Width() )
	    NewViewport.X( TotalContent.X() + TotalContent.Width() - NewViewport.Width() );
	if ( NewViewport.Y() + NewViewport.Height() > TotalContent.Y() + TotalContent.Height() )
	    NewViewport.Y( TotalContent.Y() + TotalContent.Height() - NewViewport.Height() );

	m_Contents->MoveViewportTo( NewViewport.Point() );
	updateScrollbarPosition();
    }

    void Window::ResizeViewport( const Area2D &size )
    {
	SetContentSize( size );
	//m_Layout->Resize( size );
    }

    Context::Ptr Window::Contents() const
    {
	return m_Contents;
    }

    void Window::AddContent( Content::Ptr content, const Point3D &pos )
    {
	if ( !content )
	    return;

	m_Contents->AddContent( content, pos );
	updateWindow();
    }

    void Window::RemoveContent( Content::Ptr content )
    {
	m_Contents->RemoveContent( content );
	updateWindow();
    }

    void Window::RemoveContentFrom( tPointType depth, Content::Ptr content )
    {
	m_Contents->RemoveContentFrom( depth, content );
	updateWindow();
    }

    void Window::RecalculateTotalContentSize()
    {
	m_Contents->RecalculateTotalContentSize();
	updateScrollbars();
    }

    std::string Window::str()
    {
	return m_Layout->str();
    }

    std::string Window::Render() const
    {
	return m_Layout->Render();
    }

    void Window::RenderToSurface( Surface::Ptr dest, const Point2D &pos ) const
    {
#if 0
	// TODO: Make this temp surface a member of Window so it isn't continually created/destroyed
	// Create a temp surface that we can size to our "actual" size
	Surface::Ptr TempSurface( new Surface );
	TempSurface->Width( Width() );
	TempSurface->Height( Height() );

	m_Layout->RenderToSurface( TempSurface );
	TempSurface->RenderToSurface( dest, pos, TransparentSpaces );
#else
	// If there is no render buffer, there isn't any content to render
	if ( !m_RenderBuf )
	    return;

	// Clear our buffer
	m_RenderBuf->Clear();

	// Render our layout to that temp surface
	m_Layout->RenderToSurface( m_RenderBuf );

	// Then render the temp surface to the actual destination
	m_RenderBuf->RenderToSurface( dest, pos, TransparentSpaces );
#endif // 0
    }

    void Window::updateWindow()
    {
	m_Layout->ResizeViewport( m_Size );
	m_RenderBuf = Surface::Ptr( new Surface(m_Size) );

	// Move the title to the center of the window
	m_Title->MoveTo( Point2D( m_Size.Width() / 2, 0) );

	// Put the content just inside the borders
	m_Contents->MoveTo( Point2D(1,1) );
	m_Contents->ResizeViewport( Area2D(m_Size.Width() - 2, m_Size.Height() - 2) );

	// Move the status text to the proper location at the bottom-left
	m_StatusText->MoveTo( Point2D(2, m_Size.Height() - 2) );

	updateWindowBorders();
	updateScrollbars();
    }

    void Window::updateWindowBorders()
    {
	m_Borders->Resize( m_Size );
	m_Borders->DrawBox( m_Size );

	// Draw the corners
	m_Borders->DrawChar( "+", Point2D(0,0) );
	m_Borders->DrawChar( "+", Point2D(m_Size.Width() - 1, 0) );
	m_Borders->DrawChar( "+", Point2D(0,m_Size.Height() - 1) );
	m_Borders->DrawChar( "+", Point2D(m_Size.Width() - 1,m_Size.Height() - 1) );
    }

    void Window::updateScrollbars()
    {
	updateScrollbarSize();
	updateScrollbarPosition();
    }

    void Window::updateScrollbarSize()
    {
	FixedArea3D TotalContent = m_Contents->TotalContentSize();

	// Enforce minimums so we don't ever divide by 0
	tSizeType TotalContentWidth = TotalContent.Width();
	tSizeType TotalContentHeight = TotalContent.Height();
	TotalContentWidth = TotalContentWidth != 0 ? TotalContentWidth : 1;
	TotalContentHeight = TotalContentHeight != 0 ? TotalContentHeight : 1;

	FixedArea3D CurViewport;
	CurViewport.Point( m_Contents->CurViewportPos() );
	CurViewport.Width( m_Contents->Width() );
	CurViewport.Height( m_Contents->Height() );

	Area2D MaxScrollbarSize( CurViewport.Width(), CurViewport.Height() );

	m_HorizontalScrollbar->Width( MaxScrollbarSize.Width() * CurViewport.Width() / TotalContentWidth );

	// Make sure we always have at least a little scrollbar showing if needed
	if ( m_HorizontalScrollbar->Width() <= 0 )
	    m_HorizontalScrollbar->Width( 1 );

	if ( m_HorizontalScrollbar->Width() >= MaxScrollbarSize.Width() )
	{
	    m_HorizontalScrollbar->Visible( false );
	    m_HorizontalScrollbar->Width( MaxScrollbarSize.Width() );
	}
	else
	    m_HorizontalScrollbar->Visible( true );

	for ( int i = 0; i < m_HorizontalScrollbar->Width(); ++i )
	    m_HorizontalScrollbar->DrawChar( "#", Point2D(i,0) );

	// Set the size of the vertical scrollbar, and draw it to be sure it exists at the size we need
	m_VerticalScrollbar->Height( MaxScrollbarSize.Height() * CurViewport.Height() / TotalContentHeight );

	// Make sure we always have at least a little scrollbar showing if needed
	if ( m_VerticalScrollbar->Height() <= 0 )
	    m_VerticalScrollbar->Height( 1 );

	if ( m_VerticalScrollbar->Height() >= MaxScrollbarSize.Height() )
	{
	    m_VerticalScrollbar->Visible( false );
	    m_VerticalScrollbar->Height( MaxScrollbarSize.Height() );
	}
	else
	    m_VerticalScrollbar->Visible( true );

	for ( int i = 0; i < m_VerticalScrollbar->Height(); ++i )
	    m_VerticalScrollbar->DrawChar( "#", Point2D(0,i) );
    }

    void Window::updateScrollbarPosition()
    {
	FixedArea3D TotalContent = m_Contents->TotalContentSize();

	// Enforce minimums so we don't ever divide by 0
	tSizeType TotalContentWidth = TotalContent.Width();
	tSizeType TotalContentHeight = TotalContent.Height();
	TotalContentWidth = TotalContentWidth != 0 ? TotalContentWidth : 1;
	TotalContentHeight = TotalContentHeight != 0 ? TotalContentHeight : 1;

	FixedArea3D CurViewport;
	CurViewport.Point( m_Contents->CurViewportPos() );
	CurViewport.Width( m_Contents->Width() );
	CurViewport.Height( m_Contents->Height() );

	const int WIN_BORDER_WIDTH = 1;
	const int WIN_BORDER_HEIGHT = 1;

	int WidthDifference = TotalContentWidth - CurViewport.Width();
	int HeightDifference = TotalContentHeight - CurViewport.Height();

	if ( WidthDifference == 0 )
	    m_HorizontalScrollbar->MoveTo( Point2D((CurViewport.Width() - m_HorizontalScrollbar->Width()) * CurViewport.X() + WIN_BORDER_WIDTH, CurViewport.Height() + 1) );
	else
	    m_HorizontalScrollbar->MoveTo( Point2D(((CurViewport.Width() - m_HorizontalScrollbar->Width()) * CurViewport.X() / WidthDifference) + WIN_BORDER_WIDTH, CurViewport.Height() + 1) );

	if ( m_HorizontalScrollbar->X() > CurViewport.Width() - m_HorizontalScrollbar->Width() + 1 )
	    m_HorizontalScrollbar->MoveTo( Point2D((tPointType)(CurViewport.Width() - m_HorizontalScrollbar->Width() + 1), m_HorizontalScrollbar->Y()) );

	if ( HeightDifference == 0 )
	    m_VerticalScrollbar->MoveTo( Point2D(m_Borders->Width() - 1, ((CurViewport.Height() - m_VerticalScrollbar->Height()) * CurViewport.Y()) + WIN_BORDER_HEIGHT) );
	else
	    m_VerticalScrollbar->MoveTo( Point2D(m_Borders->Width() - 1, ((CurViewport.Height() - m_VerticalScrollbar->Height()) * CurViewport.Y() / HeightDifference) + WIN_BORDER_HEIGHT) );

	if ( m_VerticalScrollbar->Y() > CurViewport.Height() - m_VerticalScrollbar->Height() + 1 )
	    m_VerticalScrollbar->MoveTo( Point2D(m_VerticalScrollbar->X(), (tPointType)(CurViewport.Height() - m_VerticalScrollbar->Height() + 1)) );
    }
}


