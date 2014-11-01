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

// taskadvance.h

#pragma once


#include "object/task/task.h"

#include "math/vector.h"



class CTaskAdvance : public CTask
{
public:
    CTaskAdvance(CObject* object);
    ~CTaskAdvance();

    bool    EventProcess(const Event &event);

    Error   Start(float length);
    Error   IsEnded();

protected:

protected:
    float       m_totalLength;
    float       m_advanceLength;
    float       m_direction;
    float       m_timeLimit;
    Math::Vector    m_startPos;
    float       m_lastDist;
    float       m_fixTime;
    bool        m_bError;
};

