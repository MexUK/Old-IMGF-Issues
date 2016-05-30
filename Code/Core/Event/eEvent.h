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
	EVENT_onCursorEnterControl,
	EVENT_onCursorExitControl,
	EVENT_onShowDropList,
	EVENT_onHideDropList
};

#endif