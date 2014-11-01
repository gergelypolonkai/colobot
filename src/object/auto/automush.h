/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2014, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsiteс.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

// automush.h

#pragma once


#include "object/auto/auto.h"



enum AutoMushPhase
{
    AMP_WAIT        = 1,
    AMP_SNIF        = 2,
    AMP_ZOOM        = 3,
    AMP_FIRE        = 4,
    AMP_SMOKE       = 5,
};



class CAutoMush : public CAuto
{
public:
    CAutoMush(CObject* object);
    ~CAutoMush();

    void        DeleteObject(bool bAll=false);

    void        Init();
    bool        EventProcess(const Event &event);
    Error       GetError();

    bool        Write(char *line);
    bool        Read(char *line);

protected:
    bool        SearchTarget();

protected:
    AutoMushPhase   m_phase;
    float           m_progress;
    float           m_speed;
    float           m_lastParticle;
};

