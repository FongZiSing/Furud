//
// Engine.hpp
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @Author FongZiSing
//
// Implemention of engine.
//
#pragma once

#include "MainWindow/App.hpp"



namespace Furud
{
	class Engine final : public D2DApp
	{
	public:
		Engine() {}

		~Engine() override {}

	protected:

		//--------------------------------
		//~ Begin override message handle.

		virtual bool HandleCreateEvent(uint32 width, uint32 height) override { return true; }
		virtual void HandleKeyDownEvent(WPARAM nKey) override {}
		virtual void HandleKeyUpEvent(WPARAM nKey) override {}
		virtual void HandleLeftMouseDownEvent(WPARAM nFlags, int32 x, int32 y) override {}
		virtual void HandleLeftMouseUpEvent(WPARAM nFlags, int32 x, int32 y) override {}
		virtual void HandleMiddleMouseDownEvent(WPARAM nFlags, int32 x, int32 y) override {}
		virtual void HandleMiddleMouseUpEvent(WPARAM nFlags, int32 x, int32 y) override {}
		virtual void HandleRightMouseDownEvent(WPARAM nFlags, int32 x, int32 y) override {}
		virtual void HandleRightMouseUpEvent(WPARAM nFlags, int32 x, int32 y) override {}
		virtual void HandleMouseMoveEvent(WPARAM nFlags, int32 x, int32 y) override {}
		virtual void HandleMouseWheelEvent(uint32 nFlags, int16 zDelta, int32 x, int32 y) override {}
		virtual void HandleResizeEvent(uint32 width, uint32 height) override {}
		virtual void HandleResizingEvent(bool bResizing, uint32 width, uint32 height) override {}
		virtual void HandleMaxMinEvent(bool bMaximum, bool bMinimum) override {}
		virtual void HandleDestroyEvent() override {}

		//~ End override message handle.
		//--------------------------------
	};
}
