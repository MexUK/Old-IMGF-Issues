#ifndef eGUIStyles_H
#define eGUIStyles_H

/*
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Properties						Default Value				Potential Values								Notes
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Border
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
border-colour					RGB(0, 0, 0)
border-thickness				0
border-style					consistent					consistent, dashed, dotted
border-intersection-radius		0
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Background
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
background-colour				RGB(0xFF, 0xFF, 0xFF)
background-colour-start			RGB(0xFF, 0xFF, 0xFF)
background-colour-stop			RGB(0x80, 0x80, 0x80)
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Text
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
text-colour						RGB(0, 0, 0)
text-size						11
text-font						Verdana
text-style						default						bold, italic, underline, strikethrough			Multiple values supported.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Position/Size
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
text-align						"left top"					Sets both independently: text-align-x, text-align-y
text-align-x					"left"						left, right, center
text-align-y					"center"					left, right, center
inner-spacing					0							Sets all together: inner-spacing-left, inner-spacing-top, inner-spacing-right, inner-spacing-bottom
inner-spacing-x					0							Sets both together: inner-spacing-left, inner-spacing-right
inner-spacing-y					0							Sets both together: inner-spacing-top, inner-spacing-bottom
inner-spacing-left				0							In pixels.
inner-spacing-top				0							In pixels.
inner-spacing-right				0							In pixels.
inner-spacing-bottom			0							In pixels.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control Specific
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
caret-colour					RGB(0, 0, 0)
row-background-colour-1			RGB(255, 255, 255)															For list controls.
row-background-colour-2			RGB(200, 200, 200)															For list controls.
progress-bar-background-colour	RGB(0, 50, 150)																For progress controls.
background-colour-marked		RGB(0, 255, 0)																For radio and check controls.
background-colour-unmarked		RGB(255, 255, 255)															For radio and check controls.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Notes
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Background is drawn if at least one background property is used.
Border is drawn if at least one border property is used.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

#endif