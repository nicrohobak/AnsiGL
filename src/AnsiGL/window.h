#ifndef __ANSIGL_WINDOW_H__
#define __ANSIGL_WINDOW_H__

/*
 * window.h
 *
 * A surface lineage with features designed around containing information and presentation (titles, borders, scrollbars, etc) rather than being "meaningful content".
 */

#include <bitset>

#include "context.h"
#include "image.h"
#include "text.h"


namespace AnsiGL
{
	class Window : public Content
	{
	public:
		ANSIGL_POINTERS( Window )

	public:
		bool			TransparentSpaces;			// True to not render blank spaces (allows the "background" to bleed through in whitespace)

	public:
		Window():
			TransparentSpaces( TRANSPARENT_DEFAULT ),
			_Layout( std::make_shared< Context >() ),
			_Title( std::make_shared< Text >() ),
			_StatusText( std::make_shared< Text >(astring("")) ),
			_Contents( std::make_shared< Context >() ),
			_Borders( std::make_shared< Image >() ),
			_VerticalScrollbar( std::make_shared< Image >() ),
			_HorizontalScrollbar( std::make_shared< Image >() )
		{
			_Layout->AddContent( _Title, Point3D(0, 0, 2) );
			_Layout->AddContent( _VerticalScrollbar, Point3D(0, 0, 4) );
			_Layout->AddContent( _HorizontalScrollbar, Point3D(0, 0, 4) );
			_Layout->AddContent( _Borders, Point3D(0, 0, 6) );
			_Layout->AddContent( _StatusText, Point3D(0, 0, 8) );
			_Layout->AddContent( _Contents, Point3D(0, 0, 10) );

			_VerticalScrollbar->Width( 1 );
			_HorizontalScrollbar->Height( 1 );
			_VerticalScrollbar->TransparentSpaces = true;
			_HorizontalScrollbar->TransparentSpaces = true;

			_Borders->TransparentSpaces = true;
			_Title->Align( TxtAlign_Center );
			_Title->TransparentSpaces = false;
			this->Title( astring("Window Title") );
			_StatusText->TransparentSpaces = false;
		}

		Window( const astring &windowTitle, const Area2D &windowSize = Area2D(80, 24), const Point3D &viewportPos = Point3D(), bool transparentSpaces = false ):
			TransparentSpaces( transparentSpaces ),
			_Layout( std::make_shared< Context >() ),
			_Title( std::make_shared< Text >() ),
			_StatusText( std::make_shared< Text >(astring("")) ),
			_Contents( std::make_shared< Context >() ),
			_Borders( std::make_shared< Image >() ),
			_VerticalScrollbar( std::make_shared< Image >() ),
			_HorizontalScrollbar( std::make_shared< Image >() )
		{
			_Layout->AddContent( _Title, Point3D(0, 0, 2) );
			_Layout->AddContent( _VerticalScrollbar, Point3D(0, 0, 4) );
			_Layout->AddContent( _HorizontalScrollbar, Point3D(0, 0, 4) );
			_Layout->AddContent( _Borders, Point3D(0, 0, 6) );
			_Layout->AddContent( _StatusText, Point3D(0, 0, 8) );
			_Layout->AddContent( _Contents, Point3D(0, 0, 10) );

			_VerticalScrollbar->Width( 1 );
			_HorizontalScrollbar->Height( 1 );
			_VerticalScrollbar->TransparentSpaces = true;
			_HorizontalScrollbar->TransparentSpaces = true;

			this->Resize( windowSize );

			_Borders->TransparentSpaces = true;
			_Title->Align( TxtAlign_Center );
			_Title->TransparentSpaces = false;
			this->Title( windowTitle );
			_StatusText->TransparentSpaces = false;

			this->MoveViewportTo( viewportPos );
		}

		virtual ~Window()
		{
		}

		virtual void Clear();

		virtual const astring &Title() const;
		virtual void Title( const astring &title );

		virtual const astring &StatusText() const;
		virtual void StatusText( const astring &text );

		virtual const tSizeType Width() const;
		virtual void Width( tSizeType width );
		virtual const tSizeType Height() const;
		virtual void Height( tSizeType height );
		virtual void Resize( const Area2D &size );

		// These functions set the size of the window based on the desired size of content visible, rinstead of total window size
		virtual void SetContentWidth( tSizeType width );
		virtual void SetContentHeight( tSizeType height );
		virtual void SetContentSize( Area2D size );

		virtual const Point3D &CurViewportPos() const;
		virtual void MoveViewportTo( const Point3D &pos );
		virtual void MoveViewportTo( const Point2D &pos );

		virtual void ResizeViewport( const Area2D &size );

		virtual Context::Ptr Contents() const;
		virtual void AddContent( Content::Ptr content, const Point3D &pos = Point3D() );
		virtual void RemoveContent( Content::Ptr content );
		virtual void RemoveContentFrom( tPointType depth, Content::Ptr content );

		virtual void RecalculateTotalContentSize();

		virtual std::string str();
		virtual std::string Render() const;
		virtual void RenderToSurface( Surface::Ptr dest, const Point2D &pos = Point2D() ) const;

	protected:
		Surface::Ptr	_RenderBuf;
		Context::Ptr	_Layout;

		Text::Ptr		_Title;
		Text::Ptr		_StatusText;

		Context::Ptr	_Contents;

		Image::Ptr		_Borders;

		Image::Ptr		_VerticalScrollbar;
		Image::Ptr		_HorizontalScrollbar;

	protected:
		virtual void updateWindow();
		void updateWindowBorders();
		void updateScrollbars();
		void updateScrollbarSize();
		void updateScrollbarPosition();
	};
}


#endif // __ANSIGL_WINDOW_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


