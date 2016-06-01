#ifndef eEvent_H
#define eEvent_H

enum eEvent
{
	EVENT_onProgressTick,
	EVENT_onParseEntry,
	EVENT_onRebuildSerializeEntry,
	EVENT_onAddEntry,
	EVENT_onRemoveEntry,
	EVENT_onAddEntryExtension,
	EVENT_onRemoveEntryExtension,
	EVENT_onLoadRWVersion,
	EVENT_onLoadResourceType,

	EVENT_onStartMovingSeekBar,
	EVENT_onStopMovingSeekBar,
	EVENT_onMoveSeekBar,
	EVENT_onPressButton,
	EVENT_onCheckCheck,
	EVENT_onCheckRadio,
	EVENT_onShowDropList,
	EVENT_onHideDropList,

	EVENT_onGainFocus,
	EVENT_onCursorEnterControl,
	EVENT_onCursorExitControl,
	EVENT_onMouseDown,
	EVENT_onMouseUp,
	EVENT_onMouseMove,
	EVENT_onDoubleLeftDown,
	EVENT_onKeyDown,
	EVENT_onKeyUp,
	EVENT_onCharDown,
	EVENT_onRender,
	EVENT_onRenderBefore,
	EVENT_onRenderAfter
};

#endif