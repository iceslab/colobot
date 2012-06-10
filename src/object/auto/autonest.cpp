// * This file is part of the COLOBOT source code
// * Copyright (C) 2001-2008, Daniel ROUX & EPSITEC SA, www.epsitec.ch
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program. If not, see  http://www.gnu.org/licenses/.

#define STRICT
#define D3D_OVERLOADS

#include <windows.h>
#include <stdio.h>
#include <d3d.h>

#include "common/struct.h"
#include "graphics/d3d/d3dengine.h"
#include "math/old/d3dmath.h"
#include "common/event.h"
#include "common/misc.h"
#include "common/iman.h"
#include "math/old/math3d.h"
#include "graphics/common/particule.h"
#include "graphics/common/terrain.h"
#include "graphics/common/camera.h"
#include "object/object.h"
#include "script/cmdtoken.h"
#include "object/auto/auto.h"
#include "object/auto/autonest.h"




// Object's constructor.

CAutoNest::CAutoNest(CInstanceManager* iMan, CObject* object)
					 : CAuto(iMan, object)
{
	Init();
}

// Object's destructor.

CAutoNest::~CAutoNest()
{
}


// Destroys the object.

void CAutoNest::DeleteObject(bool bAll)
{
	CObject*	fret;

	if ( !bAll )
	{
		fret = SearchFret();
		if ( fret != 0 )
		{
			fret->DeleteObject();
			delete fret;
		}
	}

	CAuto::DeleteObject(bAll);
}


// Initialize the object.

void CAutoNest::Init()
{
	D3DVECTOR	pos;

	m_phase    = ANP_WAIT;
	m_progress = 0.0f;
	m_speed    = 1.0f/4.0f;

	m_time     = 0.0f;
	m_lastParticule = 0.0f;

	pos = m_object->RetPosition(0);
	m_terrain->MoveOnFloor(pos);
	m_fretPos = pos;
}


// Management of an event.

bool CAutoNest::EventProcess(const Event &event)
{
	CObject*	fret;

	CAuto::EventProcess(event);

	if ( m_engine->RetPause() )  return true;
	if ( event.event != EVENT_FRAME )  return true;

	m_progress += event.rTime*m_speed;

	if ( m_phase == ANP_WAIT )
	{
		if ( m_progress >= 1.0f )
		{
			if ( !SearchFree(m_fretPos) )
			{
				m_phase    = ANP_WAIT;
				m_progress = 0.0f;
				m_speed    = 1.0f/4.0f;
			}
			else
			{
				CreateFret(m_fretPos, 0.0f, OBJECT_BULLET);
				m_phase    = ANP_BIRTH;
				m_progress = 0.0f;
				m_speed    = 1.0f/5.0f;
			}
		}
	}

	if ( m_phase == ANP_BIRTH )
	{
		fret = SearchFret();

		if ( m_progress < 1.0f )
		{
			if ( fret != 0 )
			{
				fret->SetZoom(0, m_progress);
			}
		}
		else
		{
			if ( fret != 0 )
			{
				fret->SetZoom(0, 1.0f);
				fret->SetLock(false);
			}

			m_phase    = ANP_WAIT;
			m_progress = 0.0f;
			m_speed    = 1.0f/5.0f;
		}
	}

	return true;
}


// Seeks if a site is free.

bool CAutoNest::SearchFree(D3DVECTOR pos)
{
	CObject*	pObj;
	D3DVECTOR	sPos;
	ObjectType	type;
	float		sRadius, distance;
	int			i, j;

	for ( i=0 ; i<1000000 ; i++ )
	{
		pObj = (CObject*)m_iMan->SearchInstance(CLASS_OBJECT, i);
		if ( pObj == 0 )  break;

		type = pObj->RetType();
		if ( type == OBJECT_NEST )  continue;

		j = 0;
		while ( pObj->GetCrashSphere(j++, sPos, sRadius) )
		{
			distance = Length(sPos, pos);
			distance -= sRadius;
			if ( distance < 2.0f )  return false;  // location occupied
		}
	}

	return true;  // free location
}

// Create a transportable object.

void CAutoNest::CreateFret(D3DVECTOR pos, float angle, ObjectType type)
{
	CObject*	fret;

	fret = new CObject(m_iMan);
	if ( !fret->CreateResource(pos, angle, type) )
	{
		delete fret;
		return;
	}
	fret->SetLock(true);  // not usable
	fret->SetZoom(0, 0.0f);
}

// Looking for the ball during manufacture.

CObject* CAutoNest::SearchFret()
{
	CObject*	pObj;
	D3DVECTOR	oPos;
	ObjectType	type;
	int			i;

	for ( i=0 ; i<1000000 ; i++ )
	{
		pObj = (CObject*)m_iMan->SearchInstance(CLASS_OBJECT, i);
		if ( pObj == 0 )  break;

		if ( !pObj->RetLock() )  continue;

		type = pObj->RetType();
		if ( type != OBJECT_BULLET )  continue;

		oPos = pObj->RetPosition(0);
		if ( oPos.x == m_fretPos.x &&
			 oPos.z == m_fretPos.z )
		{
			return pObj;
		}
	}

	return 0;
}


// Returns an error due the state of the automation.

Error CAutoNest::RetError()
{
	return ERR_OK;
}


// Saves all parameters of the controller.

bool CAutoNest::Write(char *line)
{
	D3DVECTOR	pos;
	char		name[100];

	if ( m_phase == ANP_WAIT )  return false;

	sprintf(name, " aExist=%d", 1);
	strcat(line, name);

	CAuto::Write(line);

	sprintf(name, " aPhase=%d", m_phase);
	strcat(line, name);

	sprintf(name, " aProgress=%.2f", m_progress);
	strcat(line, name);

	sprintf(name, " aSpeed=%.2f", m_speed);
	strcat(line, name);

	return true;
}

// Restores all parameters of the controller.

bool CAutoNest::Read(char *line)
{
	D3DVECTOR	pos;

	if ( OpInt(line, "aExist", 0) == 0 )  return false;

	CAuto::Read(line);

	m_phase = (AutoNestPhase)OpInt(line, "aPhase", ANP_WAIT);
	m_progress = OpFloat(line, "aProgress", 0.0f);
	m_speed = OpFloat(line, "aSpeed", 1.0f);

	m_lastParticule = 0.0f;

	return true;
}


