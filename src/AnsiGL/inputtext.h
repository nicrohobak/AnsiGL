#ifndef __ANSIGL_INPUTTEXT_H__
#define __ANSIGL_INPUTTEXT_H__

/*
 * inputtext.h
 *
 * A Text object that accepts user input.
 */


#include "content.h"
#include "context.h"
#include "text.h"


namespace AnsiGL
{
	class InputText : public Content
	{
	public:
		ANSIGL_POINTERS( InputText )

	public:
		InputText():
			_Layout( std::make_shared< Context >(Area2D(80,1)) ),	// Default width of 80
			_Input( std::make_shared< Text >(80) ),
			_Cursor( std::make_shared< Text >("^:_") ),		// Blinky cursor...
			_Enabled( true ),
			_CaptureColor( false ),
			_CaptureColorList( false ),
			_RemoveChars( 0 ),
			_ColorCodes( ActiveColorCodes )
		{
			_Layout->AddContent( _Input, Point2D(0, 0) );
			_Layout->AddContent( _Cursor, Point2D(0, 0) );
			this->Format();
		}

		InputText( tSizeType width,							// A width of 0 is unlimited
			  ENUM_TxtAlign alignment		= TxtAlign_Default,
			  ENUM_ColorDepth colorDepth	= ColorDepth_Default,
			  bool transparentSpaces		= false,
			  bool autoHeight				= true ):
			_Layout( std::make_shared< Context >(Area2D(width,1)) ),
			_Input( std::make_shared< Text >( width, alignment, colorDepth, transparentSpaces, autoHeight) ),
			_Cursor( std::make_shared< Text >("^:_") ),		// Blinky cursor...
			_Enabled( true ),
			_CaptureColor( false ),
			_CaptureColorList( false ),
			_RemoveChars( 0 ),
			_ColorCodes( ActiveColorCodes )
		{
			_Layout->AddContent( _Input, Point2D(0, 0) );
			_Layout->AddContent( _Cursor, Point2D(0, 0) );
			this->Format();
		}

		InputText( const astring &text,
			  tSizeType width				= 0,			// A width of 0 is unlimited
			  ENUM_TxtAlign alignment		= TxtAlign_Default,
			  ENUM_ColorDepth colorDepth	= ColorDepth_Default,
			  bool transparentSpaces		= false,
			  bool autoHeight				= true ):
			_Layout( std::make_shared< Context >(Area2D(width,1)) ),
			_Input( std::make_shared< Text >( width, alignment, colorDepth, transparentSpaces, autoHeight) ),
			_Cursor( std::make_shared< Text >("^:_") ),		// Blinky cursor...
			_Enabled( true ),
			_CaptureColor( false ),
			_CaptureColorList( false ),
			_RemoveChars( 0 ),
			_ColorCodes( ActiveColorCodes )
		{
			_Layout->AddContent( _Input, Point2D(0, 0) );
			_Layout->AddContent( _Cursor, Point2D(0, 0) );
			this->Format();
		}

		InputText( const ustring &text,
			  tSizeType width				= 0,			// A width of 0 is unlimited
			  ENUM_TxtAlign alignment		= TxtAlign_Default,
			  ENUM_ColorDepth colorDepth	= ColorDepth_Default,
			  bool transparentSpaces		= false,
			  bool autoHeight				= true ):
			_Layout( std::make_shared< Context >(Area2D(width,1)) ),
			_Input( std::make_shared< Text >( width, alignment, colorDepth, transparentSpaces, autoHeight) ),
			_Cursor( std::make_shared< Text >("^:_") ),		// Blinky cursor...
			_Enabled( true ),
			_CaptureColor( false ),
			_CaptureColorList( false ),
			_RemoveChars( 0 ),
			_ColorCodes( ActiveColorCodes )
		{
			_Layout->AddContent( _Input, Point2D(0, 0) );
			_Layout->AddContent( _Cursor, Point2D(0, 0) );
			this->Format();
		}

		InputText( const std::string &text,
			  tSizeType width				= 0,			// A width of 0 is unlimited
			  ENUM_TxtAlign alignment		= TxtAlign_Default,
			  ENUM_ColorDepth colorDepth	= ColorDepth_Default,
			  bool transparentSpaces		= false,
			  bool autoHeight				= true ):
			_Layout( std::make_shared< Context >(Area2D(width,1)) ),
			_Input( std::make_shared< Text >( width, alignment, colorDepth, transparentSpaces, autoHeight) ),
			_Cursor( std::make_shared< Text >("^:_") ),		// Blinky cursor...
			_Enabled( true ),
			_CaptureColor( false ),
			_CaptureColorList( false ),
			_RemoveChars( 0 ),
			_ColorCodes( ActiveColorCodes )
		{
			_Layout->AddContent( _Input, Point2D(0, 0) );
			_Layout->AddContent( _Cursor, Point2D(0, 0) );
			this->Format();
		}

		virtual ~InputText()
		{
		}

		virtual void Format();

		const astring &Value() const;
		const astring &FormattedValue() const;

		bool empty() const;

		ENUM_TxtAlign Align() const;
		void Align( ENUM_TxtAlign align );

		virtual tSizeType Length() const;					// Length of the string, rather than the width of the window

		virtual bool Enabled() const;
		virtual void Enabled( bool enabled );

		bool CapturingColor() const;						// Returns 'true' if we're currently capturing a color sequence
		void CancelColorCapture();							// Cancels the current, in-progress color sequence

		virtual void Clear();

		const tColorCode *ColorCodes() const
		{
			return _ColorCodes;
		}

		virtual void SetColorCodes( const tColorCode *colorCodes = NULL );

		virtual void InputChar( const achar &ch );			// Adds a character to the input
		virtual void InputLine( const astring &line );		// Appends a line to the input

		virtual void Width( tSizeType width );
		virtual void Height( tSizeType height );
		virtual void Resize( const Area2D &size );

		virtual std::string str();
		virtual std::string Render() const;
		virtual void RenderToSurface( Surface::Ptr dest, const Point2D &pos = Point2D() ) const;

	protected:
		Context::Ptr		_Layout;

		Text::Ptr			_Input;
		Text::Ptr			_Cursor;

		bool				_Enabled;

		bool				_CaptureColor;
		bool				_CaptureColorList;
		std::string			_ColorBuffer;
		achar				_CurColor;
		unsigned int		_RemoveChars;

		const tColorCode *	_ColorCodes;					// So we can swap out sets and still have them processed properly

	protected:
		void setCursorColor();
	};
}



#endif // __ANSIGL_INPUTTEXT_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


