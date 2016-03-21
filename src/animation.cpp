/*
 * animation.cpp
 *
 *
 */

#include "AnsiGL/animation.h"


namespace AnsiGL
{
	Frame::Ptr Animation::CurFrame() const
	{
		if ( _CurFrame == _Frames.end() )
			return Frame::Ptr();

		return (*_CurFrame);
	}

	void Animation::AddFrame( Frame::Ptr frame )
	{
		if ( !frame )
			return;

		_Frames.push_back( frame );
		_CurFrame = _Frames.begin();
	}

	void Animation::RemoveFrame( Frame::Ptr frame )
	{
		if ( !frame )
			return;

		for ( iterator CurFrame = _Frames.begin(); CurFrame != _Frames.end(); ++CurFrame )
		{
			if ( (*CurFrame) == frame )
			{
				_Frames.erase( CurFrame );
				_CurFrame = _Frames.begin();
				return;
			}
		}
	}

	void Animation::RemoveFrame( unsigned int frameNum )
	{
		if ( frameNum >= _Frames.size() )
			return;

		unsigned int FrameCount = 0;

		for ( iterator CurFrame = _Frames.begin(); CurFrame != _Frames.end(); ++CurFrame, ++FrameCount )
		{
			if ( FrameCount == frameNum )
			{
				_Frames.erase( CurFrame );
				_CurFrame = _Frames.begin();
			}
		}
	}

	void Animation::Advance( unsigned int numFrames )
	{
		unsigned int FrameCount = 0;

		while ( FrameCount++ < numFrames )
		{
			++_CurFrame;

			// Make sure it loops
			if ( _CurFrame == _Frames.end() )
				_CurFrame = _Frames.begin();
		}
	}

	void Animation::Rewind( unsigned int numFrames )
	{
		unsigned int FrameCount = 0;

		while ( FrameCount++ < numFrames )
		{
			// Make sure it loops
			if ( _CurFrame == _Frames.begin() )
				_CurFrame = _Frames.end();

			--_CurFrame;
		}
	}

	std::string Animation::str()
	{
		if ( _CurFrame == _Frames.end() )
			return std::string("");

		return (*_CurFrame)->str();
	}

	std::string Animation::Render() const
	{
		if ( _CurFrame == _Frames.end() )
			return std::string("");

		return (*_CurFrame)->Render();
	}

	void Animation::RenderToSurface( Surface::Ptr dest, Point2D pos ) const
	{
		if ( _CurFrame == _Frames.end() )
			return;

		(*_CurFrame)->RenderToSurface( dest, pos );
	}
}


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --indent-switches --pad-oper


