/*
 *  VGUI Healthbar
 *
 * https://developer.valvesoftware.com/wiki/VGUI_Health_bar
 */
#include "cbase.h" 
#include "hud.h" 
#include "hud_macros.h" 
#include "c_baseplayer.h" 
#include "hud_healthbar.h" 
#include "iclientmode.h" 
#include "vgui/ISurface.h"
#include <vgui/ILocalize.h>

using namespace vgui;

#include "tier0/memdbgon.h" 

DECLARE_HUDELEMENT (CHudHealthbar);

# define HULL_INIT 80 

//------------------------------------------------------------------------
// Purpose: Constructor
//------------------------------------------------------------------------

CHudHealthbar:: CHudHealthbar (const char * pElementName) : 
	CHudElement (pElementName), BaseClass (NULL, "HudHealthbar")
{
	vgui:: Panel * pParent = g_pClientMode-> GetViewport ();
	SetParent (pParent);

	SetHiddenBits (HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT);
}

//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------

void CHudHealthbar:: Init()
{
	Reset();
}

//------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------

void CHudHealthbar:: Reset (void)
{
	m_flHull = HULL_INIT;
	m_nHullLow = -1;
	SetBgColor (Color (0,0,0,0));
}


//------------------------------------------------------------------------
// Purpose:
//------------------------------------------------------------------------

void CHudHealthbar:: OnThink (void)
{
	float newHull = 0;
	C_BasePlayer * local = C_BasePlayer:: GetLocalPlayer ();

	if (!local)
		return;

	// Never below zero 
	newHull = max(local->GetHealth(), 0);

	// DevMsg("Sheild at is at: %f\n",newShield);
	// Only update the fade if we've changed health
	if (newHull == m_flHull)
		return;

	m_flHull = newHull;
}


//------------------------------------------------------------------------
// Purpose: draws the power bar
//------------------------------------------------------------------------

void CHudHealthbar::Paint()
{
	// draw intial background
	// we do this manually because of our extra elements
	int wide, tall;
	GetSize(wide, tall);
	surface()->DrawSetColor(0, 0, 0, 255);
	surface()->DrawFilledRect(0, 0, wide, tall);

	// draw the end piece
	surface()->DrawSetColor(0, 0, 0, 255);
	surface()->DrawFilledRect(wide, 0, 25, tall);

	// Get bar chunks

	int chunkCount = m_flBarWidth / (m_flBarChunkWidth + m_flBarChunkGap);
	int enabledChunks = (int)((float)chunkCount * (m_flHull / 100.0f) + 0.5f );

	// Draw the suit power bar
	surface()->DrawSetColor (m_HullColor);

	int xpos = m_flBarInsetX, ypos = m_flBarInsetY;

	for (int i = 0; i < enabledChunks; i++)
	{
		surface()->DrawFilledRect(xpos, ypos, xpos + m_flBarChunkWidth, ypos + m_flBarHeight);
		xpos += (m_flBarChunkWidth + m_flBarChunkGap);
	}

	// Draw the exhausted portion of the bar.
	surface()->DrawSetColor(Color(m_HullColor [0], m_HullColor [1], m_HullColor [2], m_iHullDisabledAlpha));

	for (int i = enabledChunks; i < chunkCount; i++)
	{
		surface()->DrawFilledRect(xpos, ypos, xpos + m_flBarChunkWidth, ypos + m_flBarHeight);
		xpos += (m_flBarChunkWidth + m_flBarChunkGap);
	}

	// Draw our name

	surface()->DrawSetTextFont(m_hTextFont);
	surface()->DrawSetTextColor(m_HullColor);
	surface()->DrawSetTextPos(text_xpos, text_ypos);

	surface()->DrawPrintText(L"HP", wcslen(L"HP"));
}