#include <ctime>
#include <iostream>
#include <random>

#include <AnsiGL/camera.h>
#include <AnsiGL/clone.h>
#include <AnsiGL/context.h>
#include <AnsiGL/image.h>
#include <AnsiGL/inputwindow.h>
#include <AnsiGL/surface.h>
#include <AnsiGL/text.h>
#include <AnsiGL/textwindow.h>
#include <AnsiGL/window.h>
#include <AnsiGL/ncurses/ncurses.h>


// Random Number Generator
std::mt19937 gen;


int main()
{
	//
	// The AnsiGL way to ncurses::initscr()
	// Handles ncurses initialization for us, and handles cleanup upon deconstruction
	//
	AnsiGL::ncurses::Ptr ncurses = std::make_shared< AnsiGL::ncurses >();;


	// Prepare viewport parameters for a context...
	AnsiGL::Area2D ViewportSize( ncurses->Size() );

	// ...And create a context
	AnsiGL::Context::Ptr MyContext = std::make_shared< AnsiGL::Context >( ViewportSize );


	// Create some instruction text
	const unsigned int InstructionsWidth = 40;
	AnsiGL::Text::Ptr InstructionsTitle = std::make_shared< AnsiGL::Text >( InstructionsWidth, AnsiGL::TxtAlign_Center );
	AnsiGL::Text::Ptr Instructions = std::make_shared< AnsiGL::Text >( InstructionsWidth );
	*InstructionsTitle << "Instructions:";
	*Instructions << "Move things:       w,s,a,d / arrows\n";
	*Instructions << "Add new text:      t\n";
	*Instructions << "Clear text window: c\n";
	*Instructions << "Quit:              q\n";
	MyContext->AddContent( InstructionsTitle, AnsiGL::Point3D(0, 0, 0) );
	MyContext->AddContent( Instructions, AnsiGL::Point3D(0, 1, 0) );


	// Create an image to go within that context
	AnsiGL::Image::Ptr MyImage = std::make_shared< AnsiGL::Image >( AnsiGL::Area2D(10, 10) );
	MyImage->DrawString( "^r1111111111",	AnsiGL::Point2D(0, 0) );
	MyImage->DrawString( "^g222222222",		AnsiGL::Point2D(0, 1) );
	MyImage->DrawString( "^b33333333",		AnsiGL::Point2D(0, 2) );
	MyImage->DrawString( "^R4444444",	 	AnsiGL::Point2D(0, 3) );
	MyImage->DrawString( "^G555555",	 	AnsiGL::Point2D(0, 4) );
	MyImage->DrawString( "^B66666",	 		AnsiGL::Point2D(0, 5) );
	MyImage->DrawString( "^27777",			AnsiGL::Point2D(0, 6) );
	MyImage->DrawString( "^3888",			AnsiGL::Point2D(0, 7) );
	MyImage->DrawString( "^599",		 	AnsiGL::Point2D(0, 8) );
	MyImage->DrawString( "^W0",			 	AnsiGL::Point2D(0, 9) );
	// Place this object's effective origin roughly in the center of the image
	MyImage->Offset.X( 5 );
	MyImage->Offset.Y( 5 );


	// Create another image for the context
	AnsiGL::Image::Ptr MyOtherImage = std::make_shared< AnsiGL::Image >( AnsiGL::Area2D(10, 10) );
	MyOtherImage->DrawString( "0",	 			AnsiGL::Point2D(0, 0) );
	MyOtherImage->DrawString( "^599",	 		AnsiGL::Point2D(0, 1) );
	MyOtherImage->DrawString( "^3888",			AnsiGL::Point2D(0, 2) );
	MyOtherImage->DrawString( "^27777",			AnsiGL::Point2D(0, 3) );
	MyOtherImage->DrawString( "^B66666", 		AnsiGL::Point2D(0, 4) );
	MyOtherImage->DrawString( "^G555555", 		AnsiGL::Point2D(0, 5) );
	MyOtherImage->DrawString( "^R4444444", 		AnsiGL::Point2D(0, 6) );
	MyOtherImage->DrawString( "^b33333333",		AnsiGL::Point2D(0, 7) );
	MyOtherImage->DrawString( "^g222222222",	AnsiGL::Point2D(0, 8) );
	MyOtherImage->DrawString( "^r1111111111",	AnsiGL::Point2D(0, 9) );
	MyOtherImage->TransparentSpaces = true;
	// This object's effective origin will still be the top-right


	AnsiGL::Image::Ptr MyOriginMarker = std::make_shared< AnsiGL::Image >( AnsiGL::Area2D(1, 1) );
	MyOriginMarker->DrawChar( "^{8k}O", AnsiGL::Point2D(0, 0) );


	// Create another context
	AnsiGL::Context::Ptr MySecondContext = std::make_shared< AnsiGL::Context >( ViewportSize );


	// Add both images to the second context
	MySecondContext->AddContent( MyImage, AnsiGL::Point3D(23, 12, 1) );
	MySecondContext->AddContent( MyOtherImage, AnsiGL::Point3D(2, 8, 2) );


	// Add the origin marker and the second context to the first context we created
	MySecondContext->AddContent( MyOriginMarker );
	MyContext->AddContent( MySecondContext, AnsiGL::Point3D(1, 2, 5) );


	// Create an Image for some fill operations
	AnsiGL::Image::Ptr FillTest = std::make_shared< AnsiGL::Image >(AnsiGL::Area2D(30, 10) );
	FillTest->FillGlyph( AnsiGL::Area2D(15, 10), AnsiGL::uchar("R"), AnsiGL::Point2D(0, 0) );
	FillTest->FillColor( AnsiGL::Area2D(15, 10), AnsiGL::ColorDef(AnsiGL::ANSISysColor_BoldBlack, AnsiGL::ANSISysColor_Red), AnsiGL::Point2D(0, 0) );
	FillTest->FillChar( AnsiGL::Area2D(15, 10), AnsiGL::achar("^{5W}B"), AnsiGL::Point2D(15, 0) );
	MySecondContext->AddContent( FillTest, AnsiGL::Point3D(5, 5, 3) );


	// Create an Image for box drawing operation tests
	AnsiGL::Image::Ptr BoxTest = std::make_shared< AnsiGL::Image >( AnsiGL::Area2D(32, 11) );
	BoxTest->DrawBox( BoxTest->Size(), AnsiGL::Point2D(0, 0), AnsiGL::achar("-"), AnsiGL::achar("|"), AnsiGL::achar("^{R}#") );
	BoxTest->DrawString( "^Y|---|", AnsiGL::Point2D(14, 0) );
	BoxTest->DrawString( "^Y|---|", AnsiGL::Point2D(14, 10) );
	MySecondContext->AddContent( BoxTest, AnsiGL::Point3D(45, 3, 5) );


	// Create a Clone of the BoxText Image above and place it in another location
	AnsiGL::Clone::Ptr CloneTest = std::make_shared< AnsiGL::Clone >( BoxTest );
//    MyContext->AddContent( CloneTest, AnsiGL::Point3D(10,35,5) );


	// Create a Window...just about exactly what you'd expect
	AnsiGL::Window::Ptr TestWindow = std::make_shared< AnsiGL::Window >();
	//TestWindow->TransparentSpaces = true;
	TestWindow->Resize( AnsiGL::Area2D(40, 12) );			// Sets the window size based on the total size of the window
//    TestWindow->SetContentSize( AnsiGL::Area2D(40,12) );	// Sets the window size based on the size of the content rather than the borders -- Helps with placement in quick situations
	TestWindow->Title( AnsiGL::astring("| Test Window |") );
	TestWindow->StatusText( AnsiGL::astring("^{8k}Status text is cool!") );
	MyContext->AddContent( TestWindow, AnsiGL::Point3D(40, 0, 5) );


	AnsiGL::Clone::Ptr TestWinClone1 = std::make_shared< AnsiGL::Clone >( TestWindow );
	AnsiGL::Clone::Ptr TestWinClone2 = std::make_shared< AnsiGL::Clone >( TestWindow );
	AnsiGL::Clone::Ptr TestWinClone3 = std::make_shared< AnsiGL::Clone >( TestWindow );
	MyContext->AddContent( TestWinClone1, AnsiGL::Point3D(80, 0, 5) );
	MyContext->AddContent( TestWinClone2, AnsiGL::Point3D(40, 12, 5) );
	MyContext->AddContent( TestWinClone3, AnsiGL::Point3D(80, 12, 5) );


	// Create a Text object and put it in our TestWindow so it has some content
	AnsiGL::Text::Ptr TestText = std::make_shared< AnsiGL::Text >( AnsiGL::astring("This is a test of some text in a nice little package"), 7, AnsiGL::TxtAlign_Center );
	//TestWindow->AddContent( TestText, AnsiGL::Point3D(30,10,0) );
	//TestWindow->MoveViewportTo( AnsiGL::Point3D(15,5,0) );
	AnsiGL::Clone::Ptr DebugClone1 = std::make_shared< AnsiGL::Clone >( MyImage );
	AnsiGL::Image::Ptr DebugImg1 = std::make_shared< AnsiGL::Image >( AnsiGL::Area2D(15, 7) );
//    TestWindow->AddContent( DebugImg1, AnsiGL::Point3D(0,0,10) );


	AnsiGL::Text::Ptr MoreText = std::make_shared< AnsiGL::Text >( AnsiGL::astring("This is a long string of text because I need to fill up a certain number of characters wide, and I needed to be sure that it was going to happen correctly."), 20, AnsiGL::TxtAlign_Center );
	TestWindow->AddContent( CloneTest, AnsiGL::Point3D(0, 0, 0) );
	TestWindow->AddContent( TestText, AnsiGL::Point3D(25, 15, 0) );
	TestWindow->AddContent( DebugClone1, AnsiGL::Point3D(25, 9, 1) );
	TestWindow->AddContent( MoreText, AnsiGL::Point3D(36, 4, 0) );


	AnsiGL::Camera::Ptr TestCamera = std::make_shared< AnsiGL::Camera >( TestWindow->Contents(), AnsiGL::Area2D(25, 12) );
	TestCamera->MoveCameraTo( AnsiGL::Point3D(5, 5, 0) );
	MyContext->AddContent( TestCamera, AnsiGL::Point3D(15, 28, 6) );


	//AnsiGL::TextWindow::Ptr TestTextWindow = std::make_shared< AnsiGL::TextWindow >( AnsiGL::astring("| Text |"), AnsiGL::Area2D(50, 15), 500, AnsiGL::TxtAlign_Default, true, AnsiGL::Point2D(0, 1) );
	AnsiGL::InputWindow::Ptr TestTextWindow = std::make_shared< AnsiGL::InputWindow >( AnsiGL::astring("| Input |"), AnsiGL::Area2D(50, 15), 500, AnsiGL::TxtAlign_Default, true, AnsiGL::Point2D(0, 1) );
	TestTextWindow->WordWrap( false );
	MyContext->AddContent( TestTextWindow, AnsiGL::Point3D(3, 7, 2) );


	AnsiGL::Text::Ptr LineCounter = std::make_shared< AnsiGL::Text >( AnsiGL::astring("Line Count: 0"), 20 );
	MyContext->AddContent( LineCounter, AnsiGL::Point3D(5, 23, 0) );


	int ch = 0;
	unsigned int CurLineCount = 0;

	// Perform the main ncurses loop
	do
	{
		AnsiGL::Point3D CurContextPos		= MySecondContext->CurPos();
		AnsiGL::Point3D CurViewportPos		= TestWindow->CurViewportPos();
		AnsiGL::Point3D CurCameraViewPos	= TestCamera->CurCameraPos();
		AnsiGL::Point3D CurCameraPos		= TestCamera->CurPos();
		AnsiGL::Point3D CurTextViewPos		= TestTextWindow->CurViewportPos();

		bool Exit = false;

		switch ( ch )
		{
			case 't':
			case 'T':
			{
				std::uniform_int_distribution<> NumLines(1, 5);

				switch ( NumLines(gen) )
				{
				case 1:
					TestTextWindow->AddLine( AnsiGL::astring("My ^Gg^Wo^Yd^C!^D  It's full of ^Ystars^W!") );
					TestTextWindow->Align( AnsiGL::TxtAlign_Left );
					break;

				case 2:
					TestTextWindow->AddLine( AnsiGL::astring("How'd all those ^Bf^Cis^Bh^D get up here?") );
					TestTextWindow->Align( AnsiGL::TxtAlign_Center );
					break;

				case 3:
					TestTextWindow->AddLine( AnsiGL::astring("I'm a very female ^rd^Rr^ya^Yg^Ro^rn.") );
					TestTextWindow->Align( AnsiGL::TxtAlign_Right );
					break;

				case 4:
					TestTextWindow->AddLine( AnsiGL::astring("I've got a ^Wpeaceful^D, ^ceasy^D feeling.") );
					break;

				case 5:
				default:
					TestTextWindow->AddLine( AnsiGL::astring("^KPuff's sayings may be (C) CircleMUD, but I honestly couldn't say for sure...") );
					break;
				}

				if ( ++CurLineCount > 10 )
					TestTextWindow->WordWrap( true );

				std::stringstream CounterLine;
				CounterLine << "Line Count: " << CurLineCount;
				LineCounter->Value( CounterLine.str() );
				break;
			}

			case 'c':
			case 'C':
			{
				CurLineCount = 0;
				TestTextWindow->Clear();
				TestTextWindow->WordWrap( false );
				LineCounter->Value( "Line Count: 0" );
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

			case KEY_BACKSPACE:
				TestTextWindow->InputChar( (char)127 );
				break;

			case '\n':
			{
				if ( TestTextWindow->Enabled() )
				{
					AnsiGL::astring TxtMsg( "Input) " );
					TxtMsg << TestTextWindow->CurInput().Value();
					TestTextWindow->AddLine( TxtMsg );
					TestTextWindow->ClearInput();

					std::stringstream CounterLine;
					CounterLine << "Line Count: " << ++CurLineCount;
					LineCounter->Value( CounterLine.str() );
				}
				break;
			}

			default:
				TestTextWindow->InputChar( ch );
				break;
		}

		if ( Exit )
			break;

		// Refresh our ncurses window with our rendering context
		ncurses->Refresh( MyContext );
	}
	while ( ch = ncurses->GetCh() );


	//
	// Stop ncurses by releaseing our AnsiGL::ncurses object
	//
	ncurses.reset();
	std::cout << "Exiting ncurses mode..." << std::endl;


	//
	// Render a few more tests
	//
	std::cout << MyImage->Render() << std::endl;
	std::cout << MyImage->str() << std::endl;

	std::cout << MyContext->Render() << std::endl;

	AnsiGL::astring OriginalStr( "This ^ri^gs ^ba ^Rl^Gi^Bn^De with a super-duper-rediculiously long word in it." );
	std::cout << "Original: " << std::endl << OriginalStr.Render() << std::endl;
	std::cout << "Formatted: " << OriginalStr.Format(13, AnsiGL::TxtAlign_Center).Render() << std::endl;

	std::cout << TestWindow->Render() << std::endl;

	std::cout << "Text Obj: " << std::endl << TestText->Render() << std::endl;

	return 0;
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


