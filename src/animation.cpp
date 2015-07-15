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
	if ( m_CurFrame == m_Frames.end() )
	    return Frame::Ptr();

	return (*m_CurFrame);
    }

    void Animation::AddFrame( Frame::Ptr frame )
    {
	if ( !frame )
	    return;

	m_Frames.push_back( frame );
	m_CurFrame = m_Frames.begin();
    }

    void Animation::RemoveFrame( Frame::Ptr frame )
    {
	if ( !frame )
	    return;

	for ( iterator CurFrame = m_Frames.begin(); CurFrame != m_Frames.end(); ++CurFrame )
	{
	    if ( (*CurFrame) == frame )
	    {
		m_Frames.erase( CurFrame );
		m_CurFrame = m_Frames.begin();
		return;
	    }
	}
    }

    void Animation::RemoveFrame( unsigned int frameNum )
    {
	if ( frameNum >= m_Frames.size() )
	    return;

	unsigned int FrameCount = 0;

	for ( iterator CurFrame = m_Frames.begin(); CurFrame != m_Frames.end(); ++CurFrame, ++FrameCount )
	{
	    if ( FrameCount == frameNum )
	    {
		m_Frames.erase( CurFrame );
		m_CurFrame = m_Frames.begin();
	    }
	}
    }

    void Animation::Advance( unsigned int numFrames )
    {
	unsigned int FrameCount = 0;

	while ( FrameCount++ < numFrames )
	{
	    ++m_CurFrame;

	    // Make sure it loops
	    if ( m_CurFrame == m_Frames.end() )
		m_CurFrame = m_Frames.begin();
	}
    }

    void Animation::Rewind( unsigned int numFrames )
    {
	unsigned int FrameCount = 0;

	while ( FrameCount++ < numFrames )
	{
	    // Make sure it loops
	    if ( m_CurFrame == m_Frames.begin() )
		m_CurFrame = m_Frames.end();

	    --m_CurFrame;
	}
    }

    std::string Animation::str()
    {
	if ( m_CurFrame == m_Frames.end() )
	    return std::string("");

	return (*m_CurFrame)->str();
    }

    std::string Animation::Render() const
    {
	if ( m_CurFrame == m_Frames.end() )
	    return std::string("");

	return (*m_CurFrame)->Render();
    }

    void Animation::RenderToSurface( Surface::Ptr dest, Point2D pos ) const
    {
	if ( m_CurFrame == m_Frames.end() )
	    return;

	(*m_CurFrame)->RenderToSurface( dest, pos );
    }
}





