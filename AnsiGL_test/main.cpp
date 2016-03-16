#include <ctime>
#include <iostream>

#include <AnsiGL/camera.h>
#include <AnsiGL/clone.h>
#include <AnsiGL/context.h>
#include <AnsiGL/image.h>
#include <AnsiGL/surface.h>
#include <AnsiGL/text.h>
#include <AnsiGL/textwindow.h>
#include <AnsiGL/window.h>
#include <AnsiGL/ncurses/ncurses.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

extern "C"
{
#include <ncurses.h>
}


// Random Number Generator
boost::random::mt19937 gen;


int main()
{
	//
	// ncurses setup
	//
	AnsiGL::StartNcurses();	// The AnsiGL way to ncurses::initscr()

	raw();
	keypad( stdscr, TRUE );
	noecho();
	curs_set( 0 );		// Make the cursor invisible

	mousemask( ALL_MOUSE_EVENTS, NULL );

	AnsiGL::Surface::Ptr AnsiGLScr( new AnsiGL::Surface(AnsiGL::Area(COLS, LINES)) );

	// If ncurses says our terminal doens't have colors, then lets not render any either
	if ( !has_colors() )
		AnsiGLScr->RenderANSI = false;
	//
	// End ncurses setup
	//

	// Prepare viewport parameters for a context...
	AnsiGL::Area2D ViewportSize( AnsiGLScr->Width(), AnsiGLScr->Height() );
	//AnsiGL::Point3D ViewportPos( -(AnsiGLScr->Width() / 2), -(AnsiGLScr->Height() / 2), 0 );
	AnsiGL::Point3D ViewportPos( 0, 0, 0 );

	// ...And create a context
	AnsiGL::Context::Ptr MyContext = AnsiGL::Context::Ptr( new AnsiGL::Context(ViewportSize, ViewportPos) );

	// Create an image to go within that context
	AnsiGL::Image::Ptr MyImage = AnsiGL::Image::Ptr( new AnsiGL::Image(AnsiGL::Area2D(10, 10)) );
	MyImage->DrawString( "^r1111111111",	AnsiGL::Point2D(0, 0) );
	MyImage->DrawString( "^g222222222",		AnsiGL::Point2D(0, 1) );
	MyImage->DrawString( "^b33333333",		AnsiGL::Point2D(0, 2) );
	MyImage->DrawString( "^R4444444",	 	AnsiGL::Point2D(0, 3) );
	MyImage->DrawString( "^G555555",	 	AnsiGL::Point2D(0, 4) );
	MyImage->DrawString( "^B66666",	 	AnsiGL::Point2D(0, 5) );
	MyImage->DrawString( "^27777",		AnsiGL::Point2D(0, 6) );
	MyImage->DrawString( "^3888",		AnsiGL::Point2D(0, 7) );
	MyImage->DrawString( "^599",	 	AnsiGL::Point2D(0, 8) );
	MyImage->DrawString( "^W0",		 	AnsiGL::Point2D(0, 9) );
	// Place this object's effective origin roughly in the center of the image
	MyImage->Offset.X( 5 );
	MyImage->Offset.Y( 5 );

	// Create another image for the context
	AnsiGL::Image::Ptr MyOtherImage = AnsiGL::Image::Ptr( new AnsiGL::Image(AnsiGL::Area2D(10, 10)) );
	MyOtherImage->DrawString( "0",	 	AnsiGL::Point2D(0, 0) );
	MyOtherImage->DrawString( "^599",	 	AnsiGL::Point2D(0, 1) );
	MyOtherImage->DrawString( "^3888",		AnsiGL::Point2D(0, 2) );
	MyOtherImage->DrawString( "^27777",		AnsiGL::Point2D(0, 3) );
	MyOtherImage->DrawString( "^B66666", 	AnsiGL::Point2D(0, 4) );
	MyOtherImage->DrawString( "^G555555", 	AnsiGL::Point2D(0, 5) );
	MyOtherImage->DrawString( "^R4444444", 	AnsiGL::Point2D(0, 6) );
	MyOtherImage->DrawString( "^b33333333",	AnsiGL::Point2D(0, 7) );
	MyOtherImage->DrawString( "^g222222222",	AnsiGL::Point2D(0, 8) );
	MyOtherImage->DrawString( "^r1111111111",	AnsiGL::Point2D(0, 9) );
	MyOtherImage->TransparentSpaces = true;
	// This object's effective origin will still be the top-right

	AnsiGL::Image::Ptr MyOriginMarker = AnsiGL::Image::Ptr( new AnsiGL::Image(AnsiGL::Area2D(1, 1)) );
	MyOriginMarker->DrawChar( "^{8k}O", AnsiGL::Point2D(0, 0) );

	// Create another context
	AnsiGL::Context::Ptr MySecondContext = AnsiGL::Context::Ptr( new AnsiGL::Context(ViewportSize, ViewportPos) );

	// Add both images to the second context
	MySecondContext->AddContent( MyImage, AnsiGL::Point3D(23, 12, 1) );
	MySecondContext->AddContent( MyOtherImage, AnsiGL::Point3D(2, 8, 2) );

	// Add the origin marker and the second context to the first context we created
	MySecondContext->AddContent( MyOriginMarker );
	MyContext->AddContent( MySecondContext, AnsiGL::Point3D(72, 21, 5) );

	// Create an Image for some fill operations
	AnsiGL::Image::Ptr FillTest = AnsiGL::Image::Ptr( new AnsiGL::Image(AnsiGL::Area2D(30, 10)) );
	FillTest->FillGlyph( AnsiGL::Area2D(15, 10), AnsiGL::uchar("R"), AnsiGL::Point2D(0, 0) );
	FillTest->FillColor( AnsiGL::Area2D(15, 10), AnsiGL::ColorDef(AnsiGL::ANSISysColor_BoldBlack, AnsiGL::ANSISysColor_Red), AnsiGL::Point2D(0, 0) );
	FillTest->FillChar( AnsiGL::Area2D(15, 10), AnsiGL::achar("^{5W}B"), AnsiGL::Point2D(15, 0) );
	MySecondContext->AddContent( FillTest, AnsiGL::Point3D(5, 5, 3) );

	// Create an Image for box drawing operation tests
	AnsiGL::Image::Ptr BoxTest = AnsiGL::Image::Ptr( new AnsiGL::Image(AnsiGL::Area2D(32, 11)) );
	BoxTest->DrawBox( BoxTest->Size(), AnsiGL::Point2D(0, 0), AnsiGL::achar("-"), AnsiGL::achar("|"), AnsiGL::achar("^{R}#") );
	BoxTest->DrawString( "^Y|---|", AnsiGL::Point2D(14, 0) );
	BoxTest->DrawString( "^Y|---|", AnsiGL::Point2D(14, 10) );
	MySecondContext->AddContent( BoxTest, AnsiGL::Point3D(45, 3, 5) );

	// Create a Clone of the BoxText Image above and place it in another location
	AnsiGL::Clone::Ptr CloneTest = AnsiGL::Clone::Ptr( new AnsiGL::Clone(BoxTest) );
//    MyContext->AddContent( CloneTest, AnsiGL::Point3D(10,35,5) );

	// Create a Window...just about exactly what you'd expect
	AnsiGL::Window::Ptr TestWindow = AnsiGL::Window::Ptr( new AnsiGL::Window() );
	//TestWindow->TransparentSpaces = true;
	TestWindow->Resize( AnsiGL::Area2D(40, 12) );			// Sets the window size based on the total size of the window
//    TestWindow->SetContentSize( AnsiGL::Area2D(40,12) );	// Sets the window size based on the size of the content rather than the borders -- Helps with placement in quick situations
	TestWindow->Title( AnsiGL::astring("Test Window") );
	TestWindow->StatusText( AnsiGL::astring("^{8k}Status text is cool!") );
	MyContext->AddContent( TestWindow, AnsiGL::Point3D(0, 0, 0) );

	AnsiGL::Clone::Ptr TestWinClone1 = AnsiGL::Clone::Ptr( new AnsiGL::Clone(TestWindow) );
	AnsiGL::Clone::Ptr TestWinClone2 = AnsiGL::Clone::Ptr( new AnsiGL::Clone(TestWindow) );
	AnsiGL::Clone::Ptr TestWinClone3 = AnsiGL::Clone::Ptr( new AnsiGL::Clone(TestWindow) );
	MyContext->AddContent( TestWinClone1, AnsiGL::Point3D(40, 0, 0) );
	MyContext->AddContent( TestWinClone2, AnsiGL::Point3D(0, 12, 0) );
	MyContext->AddContent( TestWinClone3, AnsiGL::Point3D(40, 12, 0) );

	// Create a Text object and put it in our TestWindow so it has some content
	AnsiGL::Text::Ptr TestText = AnsiGL::Text::Ptr( new AnsiGL::Text(AnsiGL::astring("This is a test of some text in a nice little package"), 7, AnsiGL::TxtAlign_Center) );
	//TestWindow->AddContent( TestText, AnsiGL::Point3D(30,10,0) );
	//TestWindow->MoveViewportTo( AnsiGL::Point3D(15,5,0) );
	AnsiGL::Clone::Ptr DebugClone1 = AnsiGL::Clone::Ptr( new AnsiGL::Clone(MyImage) );
	AnsiGL::Image::Ptr DebugImg1 = AnsiGL::Image::Ptr( new AnsiGL::Image(AnsiGL::Area2D(15, 7)) );
//    TestWindow->AddContent( DebugImg1, AnsiGL::Point3D(0,0,10) );

	AnsiGL::Text::Ptr MoreText = AnsiGL::Text::Ptr( new AnsiGL::Text(AnsiGL::astring("This is a long string of text because I need to fill up a certain number of characters wide, and I needed to be sure that it was going to happen correctly."), 20, AnsiGL::TxtAlign_Center) );
	TestWindow->AddContent( CloneTest, AnsiGL::Point3D(0, 0, 0) );
	TestWindow->AddContent( TestText, AnsiGL::Point3D(25, 15, 0) );
	TestWindow->AddContent( DebugClone1, AnsiGL::Point3D(25, 9, 1) );
	TestWindow->AddContent( MoreText, AnsiGL::Point3D(36, 4, 0) );

	AnsiGL::Camera::Ptr TestCamera = AnsiGL::Camera::Ptr( new AnsiGL::Camera(TestWindow->Contents(), AnsiGL::Area2D(25, 12)) );
	TestCamera->MoveCameraTo( AnsiGL::Point3D(5, 5, 0) );
	MyContext->AddContent( TestCamera, AnsiGL::Point3D(25, 34, 6) );

	AnsiGL::TextWindow::Ptr TestTextWindow = AnsiGL::TextWindow::Ptr( new AnsiGL::TextWindow(AnsiGL::astring("Text"), AnsiGL::Area2D(60, 25), 500, AnsiGL::TxtAlign_Default, true, AnsiGL::Point2D(0, 1)) );
	TestTextWindow->WordWrap( false );
	MyContext->AddContent( TestTextWindow, AnsiGL::Point3D(85, 25, 0) );


	AnsiGL::Text::Ptr LineCounter = AnsiGL::Text::Ptr( new AnsiGL::Text(AnsiGL::astring("Line Count: 0"), 20) );
	MyContext->AddContent( LineCounter, AnsiGL::Point3D(5, 35, 0) );


	int ch = 0;
	unsigned int CurLineCount = 0;

	// Perform the main ncurses loop
	do
	{
		AnsiGL::Point3D CurContextPos = MySecondContext->CurPos();
		AnsiGL::Point3D CurViewportPos = TestWindow->CurViewportPos();
		AnsiGL::Point3D CurCameraViewPos = TestCamera->CurCameraPos();
		AnsiGL::Point3D CurCameraPos = TestCamera->CurPos();
		AnsiGL::Point3D CurTextViewPos = TestTextWindow->CurViewportPos();

		bool Exit = false;

		switch ( ch )
		{
		case 'c':
		case 'C':
			{
				boost::random::uniform_int_distribution<> NumLines(1, 5);

				switch ( NumLines(gen) )
				{
				case 1:
					TestTextWindow->AddLine( AnsiGL::astring("My god!  It's full of stars!") );
					break;

				case 2:
					TestTextWindow->AddLine( AnsiGL::astring("How'd all those fish get up here?") );
					TestTextWindow->Align( AnsiGL::TxtAlign_Center );
					break;

				case 3:
					TestTextWindow->AddLine( AnsiGL::astring("I'm a very female dragon.") );
					break;

				case 4:
					TestTextWindow->AddLine( AnsiGL::astring("I've got a peaceful, easy feeling.") );
					TestTextWindow->WordWrap( true );
					break;

				case 5:
				default:
					TestTextWindow->AddLine( AnsiGL::astring("Puff's sayings may be (C) CircleMUD, but I honestly couldn't say for sure...") );
					break;
				}

				++CurLineCount;
				std::stringstream CounterLine;
				CounterLine << "Line Counter: " << CurLineCount;
				LineCounter->Value( CounterLine.str() );

				break;
			}

		case KEY_UP:
		case 'w':
			CurContextPos.Y( CurContextPos.Y() - 1 );
			MySecondContext->MoveTo( CurContextPos );

			CurViewportPos.Y( CurViewportPos.Y() - 1 );
			TestWindow->MoveViewportTo( CurViewportPos );

			CurCameraPos.Y( CurCameraPos.Y() + 1 );
			TestCamera->MoveTo( CurCameraPos );

			CurCameraViewPos.Y( CurCameraViewPos.Y() - 1 );
			TestCamera->MoveCameraTo( CurCameraViewPos );

			CurTextViewPos.Y( CurTextViewPos.Y() - 1 );
			TestTextWindow->MoveViewportTo( CurTextViewPos );
			break;

		case KEY_DOWN:
		case 's':
			CurContextPos.Y( CurContextPos.Y() + 1 );
			MySecondContext->MoveTo( CurContextPos );

			CurViewportPos.Y( CurViewportPos.Y() + 1 );
			TestWindow->MoveViewportTo( CurViewportPos );

			CurCameraPos.Y( CurCameraPos.Y() - 1 );
			TestCamera->MoveTo( CurCameraPos );

			CurCameraViewPos.Y( CurCameraViewPos.Y() + 1 );
			TestCamera->MoveCameraTo( CurCameraViewPos );

			CurTextViewPos.Y( CurTextViewPos.Y() + 1 );
			TestTextWindow->MoveViewportTo( CurTextViewPos );
			break;

		case KEY_LEFT:
		case 'a':
			CurContextPos.X( CurContextPos.X() - 1 );
			MySecondContext->MoveTo( CurContextPos );

			CurViewportPos.X( CurViewportPos.X() - 1 );
			TestWindow->MoveViewportTo( CurViewportPos );

			CurCameraPos.X( CurCameraPos.X() + 1 );
			TestCamera->MoveTo( CurCameraPos );

			CurCameraViewPos.X( CurCameraViewPos.X() - 1 );
			TestCamera->MoveCameraTo( CurCameraViewPos );

			CurTextViewPos.X( CurTextViewPos.X() - 1 );
			TestTextWindow->MoveViewportTo( CurTextViewPos );
			break;

		case KEY_RIGHT:
		case 'd':
			CurContextPos.X( CurContextPos.X() + 1 );
			MySecondContext->MoveTo( CurContextPos );

			CurViewportPos.X( CurViewportPos.X() + 1 );
			TestWindow->MoveViewportTo( CurViewportPos );

			CurCameraPos.X( CurCameraPos.X() - 1 );
			TestCamera->MoveTo( CurCameraPos );

			CurCameraViewPos.X( CurCameraViewPos.X() + 1 );
			TestCamera->MoveCameraTo( CurCameraViewPos );

			CurTextViewPos.X( CurTextViewPos.X() + 1 );
			TestTextWindow->MoveViewportTo( CurTextViewPos );
			break;

		case 'q':
			Exit = true;
			break;
		}

		if ( Exit )
			break;

		// Clear our screen to prep for the next frame
		AnsiGLScr->Clear();

		MyContext->RenderToSurface( AnsiGLScr );

		//RenderSurfaceToNcurses( &AnsiGLScr, stdscr );		// Render the screen
		AnsiGL::RenderSurfaceToNcurses( AnsiGLScr, stdscr );	// Render the screen
		refresh();						// Refresh stdscr to actually display on the "physical" terminal
	}
	while ( ch = getch() );

	AnsiGL::StopNcurses();	// The AnsiGL way to ncurses::endwin()
	std::cout << "Exiting ncurses mode..." << std::endl;


	// Render a few more tests
	std::cout << MyImage->Render() << std::endl;

	std::cout << MyImage->str() << std::endl;

	std::cout << MyContext->Render() << std::endl;


	AnsiGL::astring OriginalStr("This ^ri^gs ^ba ^Rl^Gi^Bn^De with a super-duper-rediculiously long word in it.");
	std::cout << "Original: " << std::endl << OriginalStr.Render() << std::endl;

	AnsiGL::astring FormattedStr( OriginalStr.Format(13, AnsiGL::TxtAlign_Center) );
	std::cout << "Formatted: " << std::endl << FormattedStr.Render() << std::endl;

	std::cout << TestWindow->Render() << std::endl;

	std::cout << "Text Obj: " << std::endl << TestText->Render() << std::endl;

	//std::cout << Background->Render() << std::endl;

#if 0
	StartNcurses();	// The AnsiGL way to ncurses::initscr()

	raw();
	keypad( stdscr, TRUE );
	noecho();
	curs_set( 0 );	// Make the cursor invisible

	mousemask( ALL_MOUSE_EVENTS, NULL );

	// Instead of using ncurses display routines, AnsiGL will be handling those instead...but we'll use ncurses to help us with the setup
	Window AnsiGLScr( Area(COLS, LINES) );

	// If ncurses says our terminal doens't have colors, then lets not render any either
	if ( !has_colors() )
		AnsiGLScr.RenderANSI = false;

	// For this, we'll be using a Window with no borders, scrollbars, margins, or title...making it a (potentially large) logical surface with a viewport that can be much smaller to represent our actual visible screen area
	AnsiGLScr.ClearAllBorders();
	AnsiGLScr.Scrollbars( false );
	AnsiGLScr.Margins( false );
	AnsiGLScr.Title( "" );


	Window Win1( Area(36, 37), Area(15, 8) );
	Point Win1Pos(7, 7);
	Win1.Title( "^{#252F#313B}¥¤ AnsiGL Test ¤¥" );
	Win1.DrawString( "^{k8}This^0 ^Ris ^Yan ^GAnsiGL ^Btest ^Mapplication^C.", Point(0, 0) );
	Win1.DrawVString( "^KThis ^ris an ^YAnsiGL ^gtest ^Bapplication^c.", Point(0, 2) );
	Win1.DrawVString( "^{k8}This^0 ^Ris an ^yAnsiGL ^Gtest ^bapplication^C.", Point(5, 2) );
	Win1.DrawVString( "^KThis ^ris an ^YAnsiGL ^gtest ^Bapplication^c.", Point(10, 2) );
	Win1.DrawVString( "^{k8}This^0 ^Ris an ^yAnsiGL ^Gtest ^bapplication^C.", Point(15, 2) );
	Win1.DrawVString( "^KThis ^ris an ^YAnsiGL ^gtest ^Bapplication^c.", Point(20, 2) );
	Win1.DrawVString( "^{8k}This^0 ^Ris an ^yAnsiGL ^Gtest ^bapplication^C.", Point(25, 2) );
	Win1.DrawVString( "^KThis ^ris an ^YAnsiGL ^gtest ^Bapplication^c.", Point(30, 2) );
	Win1.DrawVString( "^{8k}This^0 ^Ris an ^yAnsiGL ^Gtest ^bapplication^C.", Point(35, 2) );


	Window FGColorsNormal( Area(25, 9), Area(29, 13) );	// The window size is a little bigger than the area inside, so it should all show
	Point FGColorsNormalPos( 0, 1 );
	FGColorsNormal.ClearAllBorders();
	FGColorsNormal.Scrollbars( false );
	FGColorsNormal.Margins( false );
	FGColorsNormal.Title( "^WNormal Foreground Colors:" );
	FGColorsNormal.DrawString( "^{k8}k) Black" );
	FGColorsNormal.DrawString( "^rr) Red", Point(0, 1) );
	FGColorsNormal.DrawString( "^gg) Green", Point(0, 2), true ); // Draw this with a transparent space
	FGColorsNormal.DrawString( "^yy) Yellow", Point(0, 3) );
	FGColorsNormal.DrawString( "^bb) Blue", Point(0, 4) );
	FGColorsNormal.DrawString( "^mm) Magenta", Point(0, 5) );
	FGColorsNormal.DrawString( "^cc) Cyan", Point(0, 6) );
	FGColorsNormal.DrawString( "^ww) White", Point(0, 7) );
	FGColorsNormal.DrawString( "^dd) Default FG", Point(0, 8) );


	Window FGColorsBold( Area(20, 8), Area(27, 12) );
	Point FGColorsBoldPos( 32, 1 );
	FGColorsBold.ClearAllBorders();
	FGColorsBold.Scrollbars( false );
	FGColorsBold.Margins( false );
	FGColorsBold.Title( "^WBold Foreground Colors:" );
	FGColorsBold.DrawString( "^KK) Bold Black" );
	FGColorsBold.DrawString( "^RR) Bold Red", Point(0, 1) );
	FGColorsBold.DrawString( "^GG) Bold Green", Point(0, 2) );
	FGColorsBold.DrawString( "^YY) Bold Yellow", Point(0, 3) );
	FGColorsBold.DrawString( "^BB) Bold Blue", Point(0, 4) );
	FGColorsBold.DrawString( "^MM) Bold Magenta", Point(0, 5) );
	FGColorsBold.DrawString( "^CC) Bold Cyan", Point(0, 6) );
	FGColorsBold.DrawString( "^WW) Bold White", Point(0, 7) );


	int ch = 0;
	bool MoveContent = false;
	bool MoveWindow = true;
	bool ResizeMode = false;

	do
	{
		if ( ch == 'q' )
		{
			// Exit
			break;
		}

		// Clear our screen to prep for the next frame
		AnsiGLScr.Clear();

		//
		// Draw our ruler at the top
		//                     1234567890                                                                     80
		AnsiGLScr.DrawString( "|--------||--------||--------||--------||--------||--------||--------||--------|" );

		// Setup positions for our informational text
		// BUG: Fix this so it can use the AnsiGLScr sizes again!...
		//Point QuitTextPos( 0, LINES - 1 );	// Start at the bottom of the screen
		Point QuitTextPos( 0, AnsiGLScr.VisibleContentSize().Height() - 1 );	// Start at the bottom of the screen
		Point ResizeTextPos( 0, QuitTextPos.Y() - 1 );
		Point MoveTextPos( 0, ResizeTextPos.Y() - 1 );
		Point ContentTextPos( 0, MoveTextPos.Y() - 1 );
		Point ArrowTextPos( 0, ContentTextPos.Y() - 1 );

		switch ( ch )
		{
		case KEY_UP:
			if ( ResizeMode )
				Win1.WinHeight( Win1.WinHeight() - 1 );
			else if ( MoveWindow )
				Win1Pos.Y( Win1Pos.Y() - 1 );
			else
				Win1.ScrollV( -1 );
			break;

		case KEY_DOWN:
			if ( ResizeMode )
				Win1.WinHeight( Win1.WinHeight() + 1 );
			else if ( MoveWindow )
				Win1Pos.Y( Win1Pos.Y() + 1 );
			else
				Win1.ScrollV( 1 );
			break;

		case KEY_LEFT:
			if ( ResizeMode )
				Win1.WinWidth( Win1.WinWidth() - 1 );
			else if ( MoveWindow )
				Win1Pos.X( Win1Pos.X() - 1 );
			else
				Win1.ScrollH( -1 );
			break;

		case KEY_RIGHT:
			if ( ResizeMode )
				Win1.WinWidth( Win1.WinWidth() + 1 );
			else if ( MoveWindow )
				Win1Pos.X( Win1Pos.X() + 1 );
			else
				Win1.ScrollH( 1 );
			break;

		case 'c':
			MoveContent = true;
			MoveWindow = false;
			ResizeMode = false;
			break;

		case 'm':
			MoveWindow = true;
			ResizeMode = false;
			MoveContent = false;
			break;

		case 'r':
			ResizeMode = true;
			MoveWindow = false;
			MoveContent = false;
			break;

		case KEY_MOUSE:
			{
				MEVENT event;

				Point MouseTxtPos( 5, 15 );

				if ( getmouse(&event) == OK )
				{
					if ( event.bstate & BUTTON1_PRESSED )
						AnsiGLScr.DrawString( "Pressed!               ", MouseTxtPos );
					else if ( event.bstate & BUTTON1_RELEASED )
						AnsiGLScr.DrawString( "Released!               ", MouseTxtPos );
					else if ( event.bstate & REPORT_MOUSE_POSITION )
					{
						stringstream PosTxt("");
						PosTxt << "x: " << event.x << "  y: " << event.y << "  z: " << event.z;
						AnsiGLScr.DrawString( PosTxt.str(), MouseTxtPos );
					}
				}

				break;
			}

		default:
			break;
		}

		FGColorsNormal.RenderToSurface( &AnsiGLScr, FGColorsNormalPos );
		FGColorsBold.RenderToSurface( &AnsiGLScr, FGColorsBoldPos );

		// Draw our window above the others
		Win1.RenderToSurface( &AnsiGLScr, Win1Pos );

		// Draw our messages last so they'll be "on top"
		if ( ResizeMode )
			AnsiGLScr.DrawString( "Arrow keys will resize the AnsiGL Test window.", ArrowTextPos );
		else if ( MoveWindow )
			AnsiGLScr.DrawString( "Arrow keys will move the AnsiGL Test window.", ArrowTextPos );
		else
			AnsiGLScr.DrawString( "Arrow keys will move content within the AnsiGL Test window.", ArrowTextPos );
		AnsiGLScr.DrawString( "Press 'c' to move the test window content.", ContentTextPos );
		AnsiGLScr.DrawString( "Press 'm' to move the test window.", MoveTextPos );
		AnsiGLScr.DrawString( "Press 'r' to resize the test window.", ResizeTextPos );
		AnsiGLScr.DrawString( "Press 'q' to quit.", QuitTextPos );

		RenderSurfaceToNcurses( &AnsiGLScr, stdscr );		// Render the screen
		refresh();						// Refresh stdscr to actually display on the "physical" terminal
	}
	while ( ch = getch() );

	StopNcurses();	// The AnsiGL way to ncurses::endwin()
	cout << "Exiting ncurses mode..." << endl;

//////////////////////////////////////////////////////#else
	Window AnsiGLScr( Area(80, 24) );
/////////////////////////////////////////////////////#endif // 0


	cout << "Even after exiting ncurses, we can still render the screen (but alas," << endl;
	cout << "there will be no input without ncurses in this application)." << endl;
	cout << "But rather than render the entire window, just the upper-left corner" << endl;
	cout << "will be rendered instead:" << endl;

	AnsiGLScr.WinResize( Area(60, 14) );

	cout << AnsiGLScr.Render();

	cout << endl << "Now, here's a list of all of the AnsiGL supported formatting codes:" << endl;

	cout << "Note: All codes start with a '^' character.  If only one code is" << endl;
	cout << "needed, that code can follow the '^'. (Ex: \"^r\" would produce a red" << endl;
	cout << "foreground)  If multiple codes are needed, then enclose the codes" << endl;
	cout << "within curly brackets \"{ }\".  (Ex: \"^{*g5}\" would produce bold" << endl;
	cout << "green text on a blue background)" << endl << endl;

	astring PalleteStr( "^WThis is a list of available ANSI codes:\n\r" );
	PalleteStr += "Foreground Colors:\t\t\t\tBackground Colors:^D\n\r";
	PalleteStr += "^{k8}k) Black^0\t\t^KK) Bold Black^d\t\t^11) Black^0\n\r";
	PalleteStr += "^rr) Red\t\t\t^RR) Bold Red^d\t\t^22) Red^0\n\r";
	PalleteStr += "^gg) Green\t\t^GG) Bold Green^d\t\t^{k3}3) Green^0\n\r";
	PalleteStr += "^yy) Yellow\t\t^YY) Bold Yellow^d\t\t^{4k}4) Yellow^0\n\r";
	PalleteStr += "^bb) Blue\t\t\t^BB) Bold Blue^d\t\t^55) Blue^0\n\r";
	PalleteStr += "^mm) Magenta\t\t^MM) Bold Magenta^d\t\t^66) Magenta^0\n\r";
	PalleteStr += "^cc) Cyan\t\t\t^CC) Bold Cyan^d\t\t^{k7}7) Cyan^D\n\r";
	PalleteStr += "^ww) White\t\t^WW) Bold White^d\t\t^{k8}8) White^D\n\r";
	PalleteStr += "^dd) Default FG\t\t\t\t\t0) Default BG\n\r";
	PalleteStr += "\n\r";
	PalleteStr += "^WSpecial Formatting^Y*^W:\tSpecial Codes^Y*^W:\n\r";
	PalleteStr += "^**) Bold^D\t\t\tD) Reset Everything to Default\n\r";
	PalleteStr += "^*.) Bold^.Off^D\t\t^II) Invert Colors\n\r^D";	// NOTE: This closing 'default' doesn't actually close in this instance!  There is no trailing character after the code itself, so the default code here is effectively "lost in translation"
	PalleteStr += "^//) Italics^D\t\t^Ii) Un-^iInvert Colors^D\n\r";
	PalleteStr += "^/\\) Italics^\\Off^D\n\r";
	PalleteStr += "^__) Underline^D\n\r";
	PalleteStr += "^_-) Underline^-Off^D\n\r";
	PalleteStr += "^%%) Crossed-Out^D\n\r";
	PalleteStr += "^%=) Crossed-Out^=Off^D\n\r";
	PalleteStr += "^::) Blink^D\n\r";
	PalleteStr += "^:;) Blink^;Off\n\r";
	PalleteStr += "\n\r";
	PalleteStr += "^D(^Y*^d: ^cIf any codes in this column ^Idon't display correctly^i, ^*refrain from use^. on\n\r";
	PalleteStr += "this terminal type!^D)\n\r";
	cout << PalleteStr.Render() << endl;

	astring Test256("Test");

	Test256[0].Color.FG.Set( 2, 2, 1 );
	Test256[0].Color.BG.Set( 20 );
	Test256[1].Color.FG.Set( 3, 2, 1 );
	Test256[1].Color.BG.Set( 15 );
	Test256[1].Color.Inverted = true;
	Test256[2].Color.FG.Set( 4, 2, 1 );
	Test256[2].Color.BG.Set( 10 );
	Test256[3].Color.FG.Set( 5, 2, 1 );
	Test256[3].Color.BG.Set( 5 );
	Test256[3].Color.Inverted = true;

	cout << "256 color test: " << Test256.Render( true, true, ColorDepth_8Bit );

	Test256[0].Color.Inverted = true;
	Test256[1].Color.Inverted = false;
	Test256[2].Color.Inverted = true;
	Test256[3].Color.Inverted = false;

	cout << " " << Test256.Render( true, true, ColorDepth_8Bit ) << endl;

	Test256 = "This is ^&a ^{$10b#404}Test!¥£¢¡^D.";

	cout << "256 color test: " << Test256.Render( true, true, ColorDepth_8Bit ) << endl;


	Surface Palette256( Area(7, 6) );
	Palette256.DesiredDepth = ColorDepth_8Bit;
	achar Brush("‡");
	Point CurPos;
	unsigned char CurFGIndex = 0;
	unsigned char CurBGIndex = 5;

	for ( CurPos.Y(0); CurPos.Y() < 6; CurPos.Y(CurPos.Y() + 1) )
	{
		for ( CurPos.X(0); CurPos.X() < 6; CurPos.X(CurPos.X() + 1) )
		{
			Brush.Color.FG.Set( CurPos.Y(), CurFGIndex++, CurPos.X() );
			Brush.Color.BG.Set( CurPos.Y(), CurPos.X(), CurBGIndex-- );
			Palette256.DrawChar( Brush, CurPos );

			if ( CurFGIndex > 5 )
				CurFGIndex = 0;
			if ( CurBGIndex > 5 )
				CurBGIndex = 5;
		}
	}

	cout << Palette256.Render() << endl;

#endif // 0

	return 0;
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


