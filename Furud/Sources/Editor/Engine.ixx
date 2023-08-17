//
// Engine.ixx
//
//       Copyright (c) Furud Engine. All rights reserved.
//       @Author FongZiSing
//
// Implemention of engine.
//
module;

#include <stdint.h>
#include <windows.h>


export module Furud.Engine;

import Furud.App;



export namespace Furud
{
	class Engine final : public Furud::D3DApp
	{
	public:
		Engine();

		~Engine();

	protected:

		//--------------------------------
		//~ Begin override message handle.

		virtual bool HandleCreateEvent(uint32_t width, uint32_t height) override { return true; }
		virtual void HandleKeyDownEvent(WPARAM nKey) override {}
		virtual void HandleKeyUpEvent(WPARAM nKey) override {}
		virtual void HandleLeftMouseDownEvent(WPARAM nFlags, int32_t x, int32_t y) override {}
		virtual void HandleLeftMouseUpEvent(WPARAM nFlags, int32_t x, int32_t y) override {}
		virtual void HandleMiddleMouseDownEvent(WPARAM nFlags, int32_t x, int32_t y) override {}
		virtual void HandleMiddleMouseUpEvent(WPARAM nFlags, int32_t x, int32_t y) override {}
		virtual void HandleRightMouseDownEvent(WPARAM nFlags, int32_t x, int32_t y) override {}
		virtual void HandleRightMouseUpEvent(WPARAM nFlags, int32_t x, int32_t y) override {}
		virtual void HandleMouseMoveEvent(WPARAM nFlags, int32_t x, int32_t y) override {}
		virtual void HandleMouseWheelEvent(uint32_t nFlags, int16_t zDelta, int32_t x, int32_t y) override {}
		virtual void HandleResizeEvent(uint32_t width, uint32_t height) override {}
		virtual void HandleResizingEvent(bool bResizing, uint32_t width, uint32_t height) override {}
		virtual void HandleMaxMinEvent(bool bMaximum, bool bMinimum) override {}
		virtual void HandleDestroyEvent() override {}

		//~ End override message handle.
		//--------------------------------
	};
}
