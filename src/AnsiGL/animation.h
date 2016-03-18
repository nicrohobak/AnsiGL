#ifndef __ANSIGL_ANIMATION_H__
#define __ANSIGL_ANIMATION_H__

/*
 * animation.h
 *
 *
 */

#include "image.h"


namespace AnsiGL
{
	// Just in case Frames need to be expanded into something more
	typedef Image	Frame;


	class Animation : public Content
	{
	public:
		ANSIGL_POINTERS( Animation )

		Animation()
		{
			_CurFrame = _Frames.end();
		}

		Image::Ptr CurFrame() const;

		//
		// Note: If certain frames need to be offset for the animation to look
		//	 correct...  Remember that all Content-based objects have access
		//	 to Content::Offset.  Try setting the frame's offset before
		//	 adding it to the Animation.
		//
		void AddFrame( Frame::Ptr frame );
		void RemoveFrame( Frame::Ptr frame );
		void RemoveFrame( unsigned int frameNum );
		void Clear()
		{
			_Frames.clear();
		}

		void Restart()
		{
			_CurFrame = _Frames.begin();
		}
		void Advance( unsigned int numFrames = 1 );
		void Rewind( unsigned int numFrames = 1 );

		// The render functions simply render the current frame
		virtual std::string str();
		virtual std::string Render() const;
		virtual void RenderToSurface( Surface::Ptr dest, Point2D pos = Point2D() ) const;

	protected:
		typedef std::list< Frame::Ptr >			tFrames;
		typedef tFrames::iterator				iterator;
		typedef tFrames::const_iterator			const_iterator;
		typedef tFrames::reverse_iterator		reverse_iterator;
		typedef tFrames::const_reverse_iterator	const_reverse_iterator;

		std::list< Frame::Ptr >	_Frames;
		iterator				_CurFrame;
	};
}


#endif // __ANSIGL_ANIMATION_H__


// vim: tabstop=4 shiftwidth=4
// astyle: --indent=tab=4 --style=ansi --indent-namespaces --indent-cases --pad-oper


