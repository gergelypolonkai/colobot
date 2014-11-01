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


#include "script/cmdtoken.h"

#include "common/global.h"

#include <string.h>
#include <cstdio>


// TODO: Remove these functions


// Skips spaces.

char* SkipSpace(char *line)
{
    while ( *line == ' ' )
    {
        line ++;
    }
    return line;
}

std::string GetCmd(char* line)
{
    line = SkipSpace(line);

    int len = 0;
    for(char* x = line; *x != 0 && *x != ' ' && *x != '\t' && *x != '\n'; x++, len++);

    return std::string(line, len);
}

// Checks if a line contains a command.

bool Cmd(char *line, const char *token)
{
    char*   p;

    line = SkipSpace(line);
    
    p = strstr(line, token);
    if(p != line) return false;  // command at the beginning?
    
    unsigned int len = 0;
    for(char* x = p; *x != 0 && *x != ' ' && *x != '\t' && *x != '\n'; x++, len++);
    if(len != strlen(token)) return false; // ends with space?
    
    return true;
}

// Seeking an operator.

char* SearchOp(char *line, const char *op)
{
    char    opeq[50];
    char*   p;

    strcpy(opeq, " ");
    strcat(opeq, op);
    strcat(opeq, "=");

    p = strstr(line, opeq);
    if ( p == 0 )  // not found?
    {
        return line+strlen(line);  // point zero terminator
    }
    return p+strlen(opeq);  // after the point "="
}

// Seeks the argument.

char* SearchArg(char *line, int rank)
{
    int     i;
    char    c;

    for ( i=0 ; i<rank ; i++ )
    {
        while ( true )
        {
            c = *line++;
            if ( c == ';' )  break;
            if ( c == '=' ||
                 c ==  0  )  return line+strlen(line);
        }
    }

    line = SkipSpace(line);
    return line;
}

// Reads an integer, eventually hexadecimal.

int GetInt(char *line, int rank, int def)
{
    char*   p;
    int     n = 0;

    p = SearchArg(line, rank);
    if ( *p == 0 )  return def;

    if ( p[0] == '0' &&
         p[1] == 'x' )  // begins with "0x" (hexadecimal)?
    {
        p += 2;
        while ( true )
        {
            if ( *p >= '0' && *p <= '9' )
            {
                n *= 16;
                n += (*p++)-'0';
                continue;
            }
            if ( *p >= 'a' && *p <= 'f' )
            {
                n *= 16;
                n += (*p++)-'a'+10;
                continue;
            }
            break;
        }
    }
    else    // integer?
    {
        sscanf(p, "%d", &n);
    }
    return n;
}

// Reads a float number.

float GetFloat(char *line, int rank, float def)
{
    char*   p;
    float   n = 0.0f;

    p = SearchArg(line, rank);
    if ( *p == 0 )  return def;

    sscanf(p, "%f", &n);
    return n;
}

// Reads a string.

void GetString(char *line, int rank, char *buffer)
{
    char*   p;

    p = SearchArg(line, rank);
    *buffer = 0;
    if ( *p++ != '"' )  return;

    while ( p[0] != 0 )
    {
        if ( p[0] == '"' &&
             p[1] == '"' )
        {
            *buffer++ = *p++;
            p++;
            continue;
        }
        if ( p[0] == '"' )  break;

        *buffer++ = *p++;
    }
    *buffer = 0;
}

// Returns the type of an object.

ObjectType GetTypeObject(char *line, int rank, ObjectType def)
{
    char*   p;

    p = SearchArg(line, rank);
    if ( *p == 0 )  return def;

    if ( Cmd(p, "All"               ) )  return OBJECT_NULL;
    if ( Cmd(p, "Portico"           ) )  return OBJECT_PORTICO;
    if ( Cmd(p, "SpaceShip"         ) )  return OBJECT_BASE;
    if ( Cmd(p, "PracticeBot"       ) )  return OBJECT_MOBILEwt;
    if ( Cmd(p, "WingedGrabber"     ) )  return OBJECT_MOBILEfa;
    if ( Cmd(p, "TrackedGrabber"    ) )  return OBJECT_MOBILEta;
    if ( Cmd(p, "WheeledGrabber"    ) )  return OBJECT_MOBILEwa;
    if ( Cmd(p, "LeggedGrabber"     ) )  return OBJECT_MOBILEia;
    if ( Cmd(p, "WingedShooter"     ) )  return OBJECT_MOBILEfc;
    if ( Cmd(p, "TrackedShooter"    ) )  return OBJECT_MOBILEtc;
    if ( Cmd(p, "WheeledShooter"    ) )  return OBJECT_MOBILEwc;
    if ( Cmd(p, "LeggedShooter"     ) )  return OBJECT_MOBILEic;
    if ( Cmd(p, "WingedOrgaShooter" ) )  return OBJECT_MOBILEfi;
    if ( Cmd(p, "TrackedOrgaShooter") )  return OBJECT_MOBILEti;
    if ( Cmd(p, "WheeledOrgaShooter") )  return OBJECT_MOBILEwi;
    if ( Cmd(p, "LeggedOrgaShooter" ) )  return OBJECT_MOBILEii;
    if ( Cmd(p, "WingedSniffer"     ) )  return OBJECT_MOBILEfs;
    if ( Cmd(p, "TrackedSniffer"    ) )  return OBJECT_MOBILEts;
    if ( Cmd(p, "WheeledSniffer"    ) )  return OBJECT_MOBILEws;
    if ( Cmd(p, "LeggedSniffer"     ) )  return OBJECT_MOBILEis;
    if ( Cmd(p, "Thumper"           ) )  return OBJECT_MOBILErt;
    if ( Cmd(p, "PhazerShooter"     ) )  return OBJECT_MOBILErc;
    if ( Cmd(p, "Recycler"          ) )  return OBJECT_MOBILErr;
    if ( Cmd(p, "Shielder"          ) )  return OBJECT_MOBILErs;
    if ( Cmd(p, "Subber"            ) )  return OBJECT_MOBILEsa;
    if ( Cmd(p, "TargetBot"         ) )  return OBJECT_MOBILEtg;
    if ( Cmd(p, "Scribbler"         ) )  return OBJECT_MOBILEdr;
    if ( Cmd(p, "PowerSpot"         ) )  return OBJECT_MARKPOWER;
    if ( Cmd(p, "TitaniumSpot"      ) )  return OBJECT_MARKSTONE;
    if ( Cmd(p, "UraniumSpot"       ) )  return OBJECT_MARKURANIUM;
    if ( Cmd(p, "PlatinumSpot"      ) )  return OBJECT_MARKURANIUM;
    if ( Cmd(p, "KeyASpot"          ) )  return OBJECT_MARKKEYa;
    if ( Cmd(p, "KeyBSpot"          ) )  return OBJECT_MARKKEYb;
    if ( Cmd(p, "KeyCSpot"          ) )  return OBJECT_MARKKEYc;
    if ( Cmd(p, "KeyDSpot"          ) )  return OBJECT_MARKKEYd;
    if ( Cmd(p, "WayPoint"          ) )  return OBJECT_WAYPOINT;
    if ( Cmd(p, "BlueFlag"          ) )  return OBJECT_FLAGb;
    if ( Cmd(p, "RedFlag"           ) )  return OBJECT_FLAGr;
    if ( Cmd(p, "GreenFlag"         ) )  return OBJECT_FLAGg;
    if ( Cmd(p, "YellowFlag"        ) )  return OBJECT_FLAGy;
    if ( Cmd(p, "VioletFlag"        ) )  return OBJECT_FLAGv;
    if ( Cmd(p, "PowerCell"         ) )  return OBJECT_POWER;
    if ( Cmd(p, "FuelCellPlant"     ) )  return OBJECT_NUCLEAR;
    if ( Cmd(p, "FuelCell"          ) )  return OBJECT_ATOMIC;
    if ( Cmd(p, "NuclearCell"       ) )  return OBJECT_ATOMIC;
    if ( Cmd(p, "TitaniumOre"       ) )  return OBJECT_STONE;
    if ( Cmd(p, "UraniumOre"        ) )  return OBJECT_URANIUM;
    if ( Cmd(p, "PlatinumOre"       ) )  return OBJECT_URANIUM;
    if ( Cmd(p, "Titanium"          ) )  return OBJECT_METAL;
    if ( Cmd(p, "OrgaMatter"        ) )  return OBJECT_BULLET;
    if ( Cmd(p, "BlackBox"          ) )  return OBJECT_BBOX;
    if ( Cmd(p, "KeyA"              ) )  return OBJECT_KEYa;
    if ( Cmd(p, "KeyB"              ) )  return OBJECT_KEYb;
    if ( Cmd(p, "KeyC"              ) )  return OBJECT_KEYc;
    if ( Cmd(p, "KeyD"              ) )  return OBJECT_KEYd;
    if ( Cmd(p, "TNT"               ) )  return OBJECT_TNT;
    if ( Cmd(p, "Scrap1"            ) )  return OBJECT_SCRAP1;
    if ( Cmd(p, "Scrap2"            ) )  return OBJECT_SCRAP2;
    if ( Cmd(p, "Scrap3"            ) )  return OBJECT_SCRAP3;
    if ( Cmd(p, "Scrap4"            ) )  return OBJECT_SCRAP4;
    if ( Cmd(p, "Scrap5"            ) )  return OBJECT_SCRAP5;
    if ( Cmd(p, "Mine"              ) )  return OBJECT_BOMB;
    if ( Cmd(p, "Firework"          ) )  return OBJECT_WINFIRE;
    if ( Cmd(p, "Bag"               ) )  return OBJECT_BAG;
    if ( Cmd(p, "Greenery0"         ) )  return OBJECT_PLANT0;
    if ( Cmd(p, "Greenery1"         ) )  return OBJECT_PLANT1;
    if ( Cmd(p, "Greenery2"         ) )  return OBJECT_PLANT2;
    if ( Cmd(p, "Greenery3"         ) )  return OBJECT_PLANT3;
    if ( Cmd(p, "Greenery4"         ) )  return OBJECT_PLANT4;
    if ( Cmd(p, "Greenery5"         ) )  return OBJECT_PLANT5;
    if ( Cmd(p, "Greenery6"         ) )  return OBJECT_PLANT6;
    if ( Cmd(p, "Greenery7"         ) )  return OBJECT_PLANT7;
    if ( Cmd(p, "Greenery8"         ) )  return OBJECT_PLANT8;
    if ( Cmd(p, "Greenery9"         ) )  return OBJECT_PLANT9;
    if ( Cmd(p, "Greenery10"        ) )  return OBJECT_PLANT10;
    if ( Cmd(p, "Greenery11"        ) )  return OBJECT_PLANT11;
    if ( Cmd(p, "Greenery12"        ) )  return OBJECT_PLANT12;
    if ( Cmd(p, "Greenery13"        ) )  return OBJECT_PLANT13;
    if ( Cmd(p, "Greenery14"        ) )  return OBJECT_PLANT14;
    if ( Cmd(p, "Greenery15"        ) )  return OBJECT_PLANT15;
    if ( Cmd(p, "Greenery16"        ) )  return OBJECT_PLANT16;
    if ( Cmd(p, "Greenery17"        ) )  return OBJECT_PLANT17;
    if ( Cmd(p, "Greenery18"        ) )  return OBJECT_PLANT18;
    if ( Cmd(p, "Greenery19"        ) )  return OBJECT_PLANT19;
    if ( Cmd(p, "Tree0"             ) )  return OBJECT_TREE0;
    if ( Cmd(p, "Tree1"             ) )  return OBJECT_TREE1;
    if ( Cmd(p, "Tree2"             ) )  return OBJECT_TREE2;
    if ( Cmd(p, "Tree3"             ) )  return OBJECT_TREE3;
    if ( Cmd(p, "Tree4"             ) )  return OBJECT_TREE4;
    if ( Cmd(p, "Tree5"             ) )  return OBJECT_TREE5;
    if ( Cmd(p, "Mushroom1"         ) )  return OBJECT_MUSHROOM1;
    if ( Cmd(p, "Mushroom2"         ) )  return OBJECT_MUSHROOM2;
    if ( Cmd(p, "Home"              ) )  return OBJECT_HOME1;
    if ( Cmd(p, "Derrick"           ) )  return OBJECT_DERRICK;
    if ( Cmd(p, "BotFactory"        ) )  return OBJECT_FACTORY;
    if ( Cmd(p, "PowerStation"      ) )  return OBJECT_STATION;
    if ( Cmd(p, "Converter"         ) )  return OBJECT_CONVERT;
    if ( Cmd(p, "RepairCenter"      ) )  return OBJECT_REPAIR;
    if ( Cmd(p, "Destroyer"         ) )  return OBJECT_DESTROYER;
    if ( Cmd(p, "DefenseTower"      ) )  return OBJECT_TOWER;
    if ( Cmd(p, "AlienNest"         ) )  return OBJECT_NEST;
    if ( Cmd(p, "ResearchCenter"    ) )  return OBJECT_RESEARCH;
    if ( Cmd(p, "RadarStation"      ) )  return OBJECT_RADAR;
    if ( Cmd(p, "ExchangePost"      ) )  return OBJECT_INFO;
    if ( Cmd(p, "PowerPlant"        ) )  return OBJECT_ENERGY;
    if ( Cmd(p, "AutoLab"           ) )  return OBJECT_LABO;
    if ( Cmd(p, "NuclearPlant"      ) )  return OBJECT_NUCLEAR;
    if ( Cmd(p, "PowerCaptor"       ) )  return OBJECT_PARA;
    if ( Cmd(p, "Vault"             ) )  return OBJECT_SAFE;
    if ( Cmd(p, "Houston"           ) )  return OBJECT_HUSTON;
    if ( Cmd(p, "Target1"           ) )  return OBJECT_TARGET1;
    if ( Cmd(p, "Target2"           ) )  return OBJECT_TARGET2;
    if ( Cmd(p, "StartArea"         ) )  return OBJECT_START;
    if ( Cmd(p, "GoalArea"          ) )  return OBJECT_END;
    if ( Cmd(p, "AlienQueen"        ) )  return OBJECT_MOTHER;
    if ( Cmd(p, "AlienEgg"          ) )  return OBJECT_EGG;
    if ( Cmd(p, "AlienAnt"          ) )  return OBJECT_ANT;
    if ( Cmd(p, "AlienSpider"       ) )  return OBJECT_SPIDER;
    if ( Cmd(p, "AlienWasp"         ) )  return OBJECT_BEE;
    if ( Cmd(p, "AlienWorm"         ) )  return OBJECT_WORM;
    if ( Cmd(p, "WreckBotw1"        ) )  return OBJECT_RUINmobilew1;
    if ( Cmd(p, "WreckBotw2"        ) )  return OBJECT_RUINmobilew2;
    if ( Cmd(p, "WreckBott1"        ) )  return OBJECT_RUINmobilet1;
    if ( Cmd(p, "WreckBott2"        ) )  return OBJECT_RUINmobilet2;
    if ( Cmd(p, "WreckBotr1"        ) )  return OBJECT_RUINmobiler1;
    if ( Cmd(p, "WreckBotr2"        ) )  return OBJECT_RUINmobiler2;
    if ( Cmd(p, "RuinBotFactory"    ) )  return OBJECT_RUINfactory;
    if ( Cmd(p, "RuinDoor"          ) )  return OBJECT_RUINdoor;
    if ( Cmd(p, "RuinSupport"       ) )  return OBJECT_RUINsupport;
    if ( Cmd(p, "RuinRadar"         ) )  return OBJECT_RUINradar;
    if ( Cmd(p, "RuinConvert"       ) )  return OBJECT_RUINconvert;
    if ( Cmd(p, "RuinBaseCamp"      ) )  return OBJECT_RUINbase;
    if ( Cmd(p, "RuinHeadCamp"      ) )  return OBJECT_RUINhead;
    if ( Cmd(p, "Barrier0"          ) )  return OBJECT_BARRIER0;
    if ( Cmd(p, "Barrier1"          ) )  return OBJECT_BARRIER1;
    if ( Cmd(p, "Barrier2"          ) )  return OBJECT_BARRIER2;
    if ( Cmd(p, "Barrier3"          ) )  return OBJECT_BARRIER3;
    if ( Cmd(p, "Teen0"             ) )  return OBJECT_TEEN0;
    if ( Cmd(p, "Teen1"             ) )  return OBJECT_TEEN1;
    if ( Cmd(p, "Teen2"             ) )  return OBJECT_TEEN2;
    if ( Cmd(p, "Teen3"             ) )  return OBJECT_TEEN3;
    if ( Cmd(p, "Teen4"             ) )  return OBJECT_TEEN4;
    if ( Cmd(p, "Teen5"             ) )  return OBJECT_TEEN5;
    if ( Cmd(p, "Teen6"             ) )  return OBJECT_TEEN6;
    if ( Cmd(p, "Teen7"             ) )  return OBJECT_TEEN7;
    if ( Cmd(p, "Teen8"             ) )  return OBJECT_TEEN8;
    if ( Cmd(p, "Teen9"             ) )  return OBJECT_TEEN9;
    if ( Cmd(p, "Teen10"            ) )  return OBJECT_TEEN10;
    if ( Cmd(p, "Teen11"            ) )  return OBJECT_TEEN11;
    if ( Cmd(p, "Teen12"            ) )  return OBJECT_TEEN12;
    if ( Cmd(p, "Teen13"            ) )  return OBJECT_TEEN13;
    if ( Cmd(p, "Teen14"            ) )  return OBJECT_TEEN14;
    if ( Cmd(p, "Teen15"            ) )  return OBJECT_TEEN15;
    if ( Cmd(p, "Teen16"            ) )  return OBJECT_TEEN16;
    if ( Cmd(p, "Teen17"            ) )  return OBJECT_TEEN17;
    if ( Cmd(p, "Teen18"            ) )  return OBJECT_TEEN18;
    if ( Cmd(p, "Teen19"            ) )  return OBJECT_TEEN19;
    if ( Cmd(p, "Teen20"            ) )  return OBJECT_TEEN20;
    if ( Cmd(p, "Teen21"            ) )  return OBJECT_TEEN21;
    if ( Cmd(p, "Teen22"            ) )  return OBJECT_TEEN22;
    if ( Cmd(p, "Teen23"            ) )  return OBJECT_TEEN23;
    if ( Cmd(p, "Teen24"            ) )  return OBJECT_TEEN24;
    if ( Cmd(p, "Teen25"            ) )  return OBJECT_TEEN25;
    if ( Cmd(p, "Teen26"            ) )  return OBJECT_TEEN26;
    if ( Cmd(p, "Teen27"            ) )  return OBJECT_TEEN27;
    if ( Cmd(p, "Teen28"            ) )  return OBJECT_TEEN28;
    if ( Cmd(p, "Teen29"            ) )  return OBJECT_TEEN29;
    if ( Cmd(p, "Teen30"            ) )  return OBJECT_TEEN30;
    if ( Cmd(p, "Teen31"            ) )  return OBJECT_TEEN31;
    if ( Cmd(p, "Teen32"            ) )  return OBJECT_TEEN32;
    if ( Cmd(p, "Teen33"            ) )  return OBJECT_TEEN33;
    if ( Cmd(p, "Stone"             ) )  return OBJECT_TEEN34;
    if ( Cmd(p, "Teen35"            ) )  return OBJECT_TEEN35;
    if ( Cmd(p, "Teen36"            ) )  return OBJECT_TEEN36;
    if ( Cmd(p, "Teen37"            ) )  return OBJECT_TEEN37;
    if ( Cmd(p, "Teen38"            ) )  return OBJECT_TEEN38;
    if ( Cmd(p, "Teen39"            ) )  return OBJECT_TEEN39;
    if ( Cmd(p, "Teen40"            ) )  return OBJECT_TEEN40;
    if ( Cmd(p, "Teen41"            ) )  return OBJECT_TEEN41;
    if ( Cmd(p, "Teen42"            ) )  return OBJECT_TEEN42;
    if ( Cmd(p, "Teen43"            ) )  return OBJECT_TEEN43;
    if ( Cmd(p, "Teen44"            ) )  return OBJECT_TEEN44;
    if ( Cmd(p, "Quartz0"           ) )  return OBJECT_QUARTZ0;
    if ( Cmd(p, "Quartz1"           ) )  return OBJECT_QUARTZ1;
    if ( Cmd(p, "Quartz2"           ) )  return OBJECT_QUARTZ2;
    if ( Cmd(p, "Quartz3"           ) )  return OBJECT_QUARTZ3;
    if ( Cmd(p, "MegaStalk0"        ) )  return OBJECT_ROOT0;
    if ( Cmd(p, "MegaStalk1"        ) )  return OBJECT_ROOT1;
    if ( Cmd(p, "MegaStalk2"        ) )  return OBJECT_ROOT2;
    if ( Cmd(p, "MegaStalk3"        ) )  return OBJECT_ROOT3;
    if ( Cmd(p, "MegaStalk4"        ) )  return OBJECT_ROOT4;
    if ( Cmd(p, "MegaStalk5"        ) )  return OBJECT_ROOT5;
    if ( Cmd(p, "ApolloLEM"         ) )  return OBJECT_APOLLO1;
    if ( Cmd(p, "ApolloJeep"        ) )  return OBJECT_APOLLO2;
    if ( Cmd(p, "ApolloFlag"        ) )  return OBJECT_APOLLO3;
    if ( Cmd(p, "ApolloModule"      ) )  return OBJECT_APOLLO4;
    if ( Cmd(p, "ApolloAntenna"     ) )  return OBJECT_APOLLO5;
    if ( Cmd(p, "Me"                ) )  return OBJECT_HUMAN;
    if ( Cmd(p, "Tech"              ) )  return OBJECT_TECH;
    if ( Cmd(p, "MissionController" ) )  return OBJECT_CONTROLLER;

    return def;
}

// Returns the name of an object type.

const char* GetTypeObject(ObjectType type)
{
    if ( type == OBJECT_PORTICO      )  return "Portico";
    if ( type == OBJECT_BASE         )  return "SpaceShip";
    if ( type == OBJECT_MOBILEwt     )  return "PracticeBot";
    if ( type == OBJECT_MOBILEfa     )  return "WingedGrabber";
    if ( type == OBJECT_MOBILEta     )  return "TrackedGrabber";
    if ( type == OBJECT_MOBILEwa     )  return "WheeledGrabber";
    if ( type == OBJECT_MOBILEia     )  return "LeggedGrabber";
    if ( type == OBJECT_MOBILEfc     )  return "WingedShooter";
    if ( type == OBJECT_MOBILEtc     )  return "TrackedShooter";
    if ( type == OBJECT_MOBILEwc     )  return "WheeledShooter";
    if ( type == OBJECT_MOBILEic     )  return "LeggedShooter";
    if ( type == OBJECT_MOBILEfi     )  return "WingedOrgaShooter";
    if ( type == OBJECT_MOBILEti     )  return "TrackedOrgaShooter";
    if ( type == OBJECT_MOBILEwi     )  return "WheeledOrgaShooter";
    if ( type == OBJECT_MOBILEii     )  return "LeggedOrgaShooter";
    if ( type == OBJECT_MOBILEfs     )  return "WingedSniffer";
    if ( type == OBJECT_MOBILEts     )  return "TrackedSniffer";
    if ( type == OBJECT_MOBILEws     )  return "WheeledSniffer";
    if ( type == OBJECT_MOBILEis     )  return "LeggedSniffer";
    if ( type == OBJECT_MOBILErt     )  return "Thumper";
    if ( type == OBJECT_MOBILErc     )  return "PhazerShooter";
    if ( type == OBJECT_MOBILErr     )  return "Recycler";
    if ( type == OBJECT_MOBILErs     )  return "Shielder";
    if ( type == OBJECT_MOBILEsa     )  return "Subber";
    if ( type == OBJECT_MOBILEtg     )  return "TargetBot";
    if ( type == OBJECT_MOBILEdr     )  return "Scribbler";
    if ( type == OBJECT_MARKPOWER    )  return "PowerSpot";
    if ( type == OBJECT_MARKSTONE    )  return "TitaniumSpot";
    if ( type == OBJECT_MARKURANIUM  )  return "UraniumSpot";
    if ( type == OBJECT_MARKKEYa     )  return "KeyASpot";
    if ( type == OBJECT_MARKKEYb     )  return "KeyBSpot";
    if ( type == OBJECT_MARKKEYc     )  return "KeyCSpot";
    if ( type == OBJECT_MARKKEYd     )  return "KeyDSpot";
    if ( type == OBJECT_WAYPOINT     )  return "WayPoint";
    if ( type == OBJECT_FLAGb        )  return "BlueFlag";
    if ( type == OBJECT_FLAGr        )  return "RedFlag";
    if ( type == OBJECT_FLAGg        )  return "GreenFlag";
    if ( type == OBJECT_FLAGy        )  return "YellowFlag";
    if ( type == OBJECT_FLAGv        )  return "VioletFlag";
    if ( type == OBJECT_POWER        )  return "PowerCell";
    if ( type == OBJECT_ATOMIC       )  return "NuclearCell";
    if ( type == OBJECT_STONE        )  return "TitaniumOre";
    if ( type == OBJECT_URANIUM      )  return "UraniumOre";
    if ( type == OBJECT_METAL        )  return "Titanium";
    if ( type == OBJECT_BULLET       )  return "OrgaMatter";
    if ( type == OBJECT_BBOX         )  return "BlackBox";
    if ( type == OBJECT_KEYa         )  return "KeyA";
    if ( type == OBJECT_KEYb         )  return "KeyB";
    if ( type == OBJECT_KEYc         )  return "KeyC";
    if ( type == OBJECT_KEYd         )  return "KeyD";
    if ( type == OBJECT_TNT          )  return "TNT";
    if ( type == OBJECT_SCRAP1       )  return "Scrap1";
    if ( type == OBJECT_SCRAP2       )  return "Scrap2";
    if ( type == OBJECT_SCRAP3       )  return "Scrap3";
    if ( type == OBJECT_SCRAP4       )  return "Scrap4";
    if ( type == OBJECT_SCRAP5       )  return "Scrap5";
    if ( type == OBJECT_BOMB         )  return "Mine";
    if ( type == OBJECT_WINFIRE      )  return "Firework";
    if ( type == OBJECT_BAG          )  return "Bag";
    if ( type == OBJECT_PLANT0       )  return "Greenery0";
    if ( type == OBJECT_PLANT1       )  return "Greenery1";
    if ( type == OBJECT_PLANT2       )  return "Greenery2";
    if ( type == OBJECT_PLANT3       )  return "Greenery3";
    if ( type == OBJECT_PLANT4       )  return "Greenery4";
    if ( type == OBJECT_PLANT5       )  return "Greenery5";
    if ( type == OBJECT_PLANT6       )  return "Greenery6";
    if ( type == OBJECT_PLANT7       )  return "Greenery7";
    if ( type == OBJECT_PLANT8       )  return "Greenery8";
    if ( type == OBJECT_PLANT9       )  return "Greenery9";
    if ( type == OBJECT_PLANT10      )  return "Greenery10";
    if ( type == OBJECT_PLANT11      )  return "Greenery11";
    if ( type == OBJECT_PLANT12      )  return "Greenery12";
    if ( type == OBJECT_PLANT13      )  return "Greenery13";
    if ( type == OBJECT_PLANT14      )  return "Greenery14";
    if ( type == OBJECT_PLANT15      )  return "Greenery15";
    if ( type == OBJECT_PLANT16      )  return "Greenery16";
    if ( type == OBJECT_PLANT17      )  return "Greenery17";
    if ( type == OBJECT_PLANT18      )  return "Greenery18";
    if ( type == OBJECT_PLANT19      )  return "Greenery19";
    if ( type == OBJECT_TREE0        )  return "Tree0";
    if ( type == OBJECT_TREE1        )  return "Tree1";
    if ( type == OBJECT_TREE2        )  return "Tree2";
    if ( type == OBJECT_TREE3        )  return "Tree3";
    if ( type == OBJECT_TREE4        )  return "Tree4";
    if ( type == OBJECT_TREE5        )  return "Tree5";
    if ( type == OBJECT_MUSHROOM1    )  return "Mushroom1";
    if ( type == OBJECT_MUSHROOM2    )  return "Mushroom2";
    if ( type == OBJECT_HOME1        )  return "Home";
    if ( type == OBJECT_DERRICK      )  return "Derrick";
    if ( type == OBJECT_FACTORY      )  return "BotFactory";
    if ( type == OBJECT_STATION      )  return "PowerStation";
    if ( type == OBJECT_CONVERT      )  return "Converter";
    if ( type == OBJECT_REPAIR       )  return "RepairCenter";
    if ( type == OBJECT_DESTROYER    )  return "Destroyer";
    if ( type == OBJECT_TOWER        )  return "DefenseTower";
    if ( type == OBJECT_NEST         )  return "AlienNest";
    if ( type == OBJECT_RESEARCH     )  return "ResearchCenter";
    if ( type == OBJECT_RADAR        )  return "RadarStation";
    if ( type == OBJECT_INFO         )  return "ExchangePost";
    if ( type == OBJECT_ENERGY       )  return "PowerPlant";
    if ( type == OBJECT_LABO         )  return "AutoLab";
    if ( type == OBJECT_NUCLEAR      )  return "NuclearPlant";
    if ( type == OBJECT_PARA         )  return "PowerCaptor";
    if ( type == OBJECT_SAFE         )  return "Vault";
    if ( type == OBJECT_HUSTON       )  return "Houston";
    if ( type == OBJECT_TARGET1      )  return "Target1";
    if ( type == OBJECT_TARGET2      )  return "Target2";
    if ( type == OBJECT_START        )  return "StartArea";
    if ( type == OBJECT_END          )  return "GoalArea";
    if ( type == OBJECT_MOTHER       )  return "AlienQueen";
    if ( type == OBJECT_EGG          )  return "AlienEgg";
    if ( type == OBJECT_ANT          )  return "AlienAnt";
    if ( type == OBJECT_SPIDER       )  return "AlienSpider";
    if ( type == OBJECT_BEE          )  return "AlienWasp";
    if ( type == OBJECT_WORM         )  return "AlienWorm";
    if ( type == OBJECT_RUINmobilew1 )  return "WreckBotw1";
    if ( type == OBJECT_RUINmobilew2 )  return "WreckBotw2";
    if ( type == OBJECT_RUINmobilet1 )  return "WreckBott1";
    if ( type == OBJECT_RUINmobilet2 )  return "WreckBott2";
    if ( type == OBJECT_RUINmobiler1 )  return "WreckBotr1";
    if ( type == OBJECT_RUINmobiler2 )  return "WreckBotr2";
    if ( type == OBJECT_RUINfactory  )  return "RuinBotFactory";
    if ( type == OBJECT_RUINdoor     )  return "RuinDoor";
    if ( type == OBJECT_RUINsupport  )  return "RuinSupport";
    if ( type == OBJECT_RUINradar    )  return "RuinRadar";
    if ( type == OBJECT_RUINconvert  )  return "RuinConvert";
    if ( type == OBJECT_RUINbase     )  return "RuinBaseCamp";
    if ( type == OBJECT_RUINhead     )  return "RuinHeadCamp";
    if ( type == OBJECT_BARRIER0     )  return "Barrier0";
    if ( type == OBJECT_BARRIER1     )  return "Barrier1";
    if ( type == OBJECT_BARRIER2     )  return "Barrier2";
    if ( type == OBJECT_BARRIER3     )  return "Barrier3";
    if ( type == OBJECT_TEEN0        )  return "Teen0";
    if ( type == OBJECT_TEEN1        )  return "Teen1";
    if ( type == OBJECT_TEEN2        )  return "Teen2";
    if ( type == OBJECT_TEEN3        )  return "Teen3";
    if ( type == OBJECT_TEEN4        )  return "Teen4";
    if ( type == OBJECT_TEEN5        )  return "Teen5";
    if ( type == OBJECT_TEEN6        )  return "Teen6";
    if ( type == OBJECT_TEEN7        )  return "Teen7";
    if ( type == OBJECT_TEEN8        )  return "Teen8";
    if ( type == OBJECT_TEEN9        )  return "Teen9";
    if ( type == OBJECT_TEEN10       )  return "Teen10";
    if ( type == OBJECT_TEEN11       )  return "Teen11";
    if ( type == OBJECT_TEEN12       )  return "Teen12";
    if ( type == OBJECT_TEEN13       )  return "Teen13";
    if ( type == OBJECT_TEEN14       )  return "Teen14";
    if ( type == OBJECT_TEEN15       )  return "Teen15";
    if ( type == OBJECT_TEEN16       )  return "Teen16";
    if ( type == OBJECT_TEEN17       )  return "Teen17";
    if ( type == OBJECT_TEEN18       )  return "Teen18";
    if ( type == OBJECT_TEEN19       )  return "Teen19";
    if ( type == OBJECT_TEEN20       )  return "Teen20";
    if ( type == OBJECT_TEEN21       )  return "Teen21";
    if ( type == OBJECT_TEEN22       )  return "Teen22";
    if ( type == OBJECT_TEEN23       )  return "Teen23";
    if ( type == OBJECT_TEEN24       )  return "Teen24";
    if ( type == OBJECT_TEEN25       )  return "Teen25";
    if ( type == OBJECT_TEEN26       )  return "Teen26";
    if ( type == OBJECT_TEEN27       )  return "Teen27";
    if ( type == OBJECT_TEEN28       )  return "Teen28";
    if ( type == OBJECT_TEEN29       )  return "Teen29";
    if ( type == OBJECT_TEEN30       )  return "Teen30";
    if ( type == OBJECT_TEEN31       )  return "Teen31";
    if ( type == OBJECT_TEEN32       )  return "Teen32";
    if ( type == OBJECT_TEEN33       )  return "Teen33";
    if ( type == OBJECT_TEEN34       )  return "Stone";
    if ( type == OBJECT_TEEN35       )  return "Teen35";
    if ( type == OBJECT_TEEN36       )  return "Teen36";
    if ( type == OBJECT_TEEN37       )  return "Teen37";
    if ( type == OBJECT_TEEN38       )  return "Teen38";
    if ( type == OBJECT_TEEN39       )  return "Teen39";
    if ( type == OBJECT_TEEN40       )  return "Teen40";
    if ( type == OBJECT_TEEN41       )  return "Teen41";
    if ( type == OBJECT_TEEN42       )  return "Teen42";
    if ( type == OBJECT_TEEN43       )  return "Teen43";
    if ( type == OBJECT_TEEN44       )  return "Teen44";
    if ( type == OBJECT_QUARTZ0      )  return "Quartz0";
    if ( type == OBJECT_QUARTZ1      )  return "Quartz1";
    if ( type == OBJECT_QUARTZ2      )  return "Quartz2";
    if ( type == OBJECT_QUARTZ3      )  return "Quartz3";
    if ( type == OBJECT_ROOT0        )  return "MegaStalk0";
    if ( type == OBJECT_ROOT1        )  return "MegaStalk1";
    if ( type == OBJECT_ROOT2        )  return "MegaStalk2";
    if ( type == OBJECT_ROOT3        )  return "MegaStalk3";
    if ( type == OBJECT_ROOT4        )  return "MegaStalk4";
    if ( type == OBJECT_ROOT5        )  return "MegaStalk5";
    if ( type == OBJECT_APOLLO1      )  return "ApolloLEM";
    if ( type == OBJECT_APOLLO2      )  return "ApolloJeep";
    if ( type == OBJECT_APOLLO3      )  return "ApolloFlag";
    if ( type == OBJECT_APOLLO4      )  return "ApolloModule";
    if ( type == OBJECT_APOLLO5      )  return "ApolloAntenna";
    if ( type == OBJECT_HUMAN        )  return "Me";
    if ( type == OBJECT_TECH         )  return "Tech";
    if ( type == OBJECT_CONTROLLER   )  return "MissionController";
    return "";
}

// Returns the type of water.

Gfx::WaterType GetTypeWater(char *line, int rank, Gfx::WaterType def)
{
    char*   p;

    p = SearchArg(line, rank);
    if ( *p == 0 )  return def;

    if ( Cmd(p, "NULL" ) )  return Gfx::WATER_NULL;
    if ( Cmd(p, "TT"   ) )  return Gfx::WATER_TT;
    if ( Cmd(p, "TO"   ) )  return Gfx::WATER_TO;
    if ( Cmd(p, "CT"   ) )  return Gfx::WATER_CT;
    if ( Cmd(p, "CO"   ) )  return Gfx::WATER_CO;

    return def;
}

// Returns the type of terrain.

Gfx::EngineObjectType GetTypeTerrain(char *line, int rank, Gfx::EngineObjectType def)
{
    char*   p;

    p = SearchArg(line, rank);
    if ( *p == 0 )  return def;

    if ( Cmd(p, "Terrain" ) )  return Gfx::ENG_OBJTYPE_TERRAIN;
    if ( Cmd(p, "Object"  ) )  return Gfx::ENG_OBJTYPE_FIX;
    if ( Cmd(p, "Quartz"  ) )  return Gfx::ENG_OBJTYPE_QUARTZ;
    if ( Cmd(p, "Metal"   ) )  return Gfx::ENG_OBJTYPE_METAL;

    return def;
}

// Returns the type of a building.

int GetBuild(char *line, int rank)
{
    char*   p;

    p = SearchArg(line, rank);
    if ( *p == 0 )  return 0;

    if ( Cmd(p, "BotFactory"    ) )  return BUILD_FACTORY;
    if ( Cmd(p, "Derrick"       ) )  return BUILD_DERRICK;
    if ( Cmd(p, "Converter"     ) )  return BUILD_CONVERT;
    if ( Cmd(p, "RadarStation"  ) )  return BUILD_RADAR;
    if ( Cmd(p, "PowerPlant"    ) )  return BUILD_ENERGY;
    if ( Cmd(p, "NuclearPlant"  ) )  return BUILD_NUCLEAR;
    if ( Cmd(p, "FuelCellPlant" ) )  return BUILD_NUCLEAR;
    if ( Cmd(p, "PowerStation"  ) )  return BUILD_STATION;
    if ( Cmd(p, "RepairCenter"  ) )  return BUILD_REPAIR;
    if ( Cmd(p, "DefenseTower"  ) )  return BUILD_TOWER;
    if ( Cmd(p, "ResearchCenter") )  return BUILD_RESEARCH;
    if ( Cmd(p, "AutoLab"       ) )  return BUILD_LABO;
    if ( Cmd(p, "PowerCaptor"   ) )  return BUILD_PARA;
    if ( Cmd(p, "ExchangePost"  ) )  return BUILD_INFO;
    if ( Cmd(p, "Destroyer"     ) )  return BUILD_DESTROYER;
    if ( Cmd(p, "FlatGround"    ) )  return BUILD_GFLAT;
    if ( Cmd(p, "Flag"          ) )  return BUILD_FLAG;

    return 0;
}

// Returns the type of search.

int GetResearch(char *line, int rank)
{
    char*   p;

    p = SearchArg(line, rank);
    if ( *p == 0 )  return 0;

    if ( Cmd(p, "TRACKER" ) )  return RESEARCH_TANK;
    if ( Cmd(p, "WINGER"  ) )  return RESEARCH_FLY;
    if ( Cmd(p, "THUMPER" ) )  return RESEARCH_THUMP;
    if ( Cmd(p, "SHOOTER" ) )  return RESEARCH_CANON;
    if ( Cmd(p, "TOWER"   ) )  return RESEARCH_TOWER;
    if ( Cmd(p, "PHAZER"  ) )  return RESEARCH_PHAZER;
    if ( Cmd(p, "SHIELDER") )  return RESEARCH_SHIELD;
    if ( Cmd(p, "ATOMIC"  ) )  return RESEARCH_ATOMIC;
    if ( Cmd(p, "iPAW"    ) )  return RESEARCH_iPAW;
    if ( Cmd(p, "iGUN"    ) )  return RESEARCH_iGUN;
    if ( Cmd(p, "RECYCLER") )  return RESEARCH_RECYCLER;
    if ( Cmd(p, "SUBBER"  ) )  return RESEARCH_SUBM;
    if ( Cmd(p, "SNIFFER" ) )  return RESEARCH_SNIFFER;

    return 0;
}

// Returns the type of pyrotechnic effect.

Gfx::PyroType GetPyro(char *line, int rank)
{
    char*   p;

    p = SearchArg(line, rank);
    if ( *p == 0 )  return Gfx::PT_NULL;

    if ( Cmd(p, "FRAGt"  ) )  return Gfx::PT_FRAGT;
    if ( Cmd(p, "FRAGo"  ) )  return Gfx::PT_FRAGO;
    if ( Cmd(p, "FRAGw"  ) )  return Gfx::PT_FRAGW;
    if ( Cmd(p, "EXPLOt" ) )  return Gfx::PT_EXPLOT;
    if ( Cmd(p, "EXPLOo" ) )  return Gfx::PT_EXPLOO;
    if ( Cmd(p, "EXPLOw" ) )  return Gfx::PT_EXPLOW;
    if ( Cmd(p, "SHOTt"  ) )  return Gfx::PT_SHOTT;
    if ( Cmd(p, "SHOTh"  ) )  return Gfx::PT_SHOTH;
    if ( Cmd(p, "SHOTm"  ) )  return Gfx::PT_SHOTM;
    if ( Cmd(p, "SHOTw"  ) )  return Gfx::PT_SHOTW;
    if ( Cmd(p, "EGG"    ) )  return Gfx::PT_EGG;
    if ( Cmd(p, "BURNt"  ) )  return Gfx::PT_BURNT;
    if ( Cmd(p, "BURNo"  ) )  return Gfx::PT_BURNO;
    if ( Cmd(p, "SPIDER" ) )  return Gfx::PT_SPIDER;
    if ( Cmd(p, "FALL"   ) )  return Gfx::PT_FALL;
    if ( Cmd(p, "RESET"  ) )  return Gfx::PT_RESET;
    if ( Cmd(p, "WIN"    ) )  return Gfx::PT_WIN;
    if ( Cmd(p, "LOST"   ) )  return Gfx::PT_LOST;

    return Gfx::PT_NULL;
}

// Returns the type of camera.

Gfx::CameraType GetCamera(char *line, int rank)
{
    char*   p;

    p = SearchArg(line, rank);
    if ( *p == 0 )  return Gfx::CAM_TYPE_NULL;

    if ( Cmd(p, "BACK"    ) )  return Gfx::CAM_TYPE_BACK;
    if ( Cmd(p, "PLANE"   ) )  return Gfx::CAM_TYPE_PLANE;
    if ( Cmd(p, "ONBOARD" ) )  return Gfx::CAM_TYPE_ONBOARD;
    if ( Cmd(p, "FIX"     ) )  return Gfx::CAM_TYPE_FIX;

    return Gfx::CAM_TYPE_NULL;
}

// Returns the name of a camera.

const char* GetCamera(Gfx::CameraType type)
{
    if ( type == Gfx::CAM_TYPE_ONBOARD )  return "ONBOARD";
    if ( type == Gfx::CAM_TYPE_FIX     )  return "FIX";
    return "BACK";
}

// Returns the type of drive.

DriveType GetDrive(char *line, int rank)
{
    char*   p;

    p = SearchArg(line, rank);
    if ( *p == 0 )  return DRIVE_OTHER;

    if ( Cmd(p, "Wheeled" ) )  return DRIVE_WHEELED;
    if ( Cmd(p, "Tracked" ) )  return DRIVE_TRACKED;
    if ( Cmd(p, "Winged"  ) )  return DRIVE_WINGED;
    if ( Cmd(p, "Legged"  ) )  return DRIVE_LEGGED;

    return DRIVE_OTHER;
}

// Returns the name of a drive.

const char* GetDrive(DriveType type)
{
    if ( type == DRIVE_WHEELED )  return "Wheeled";
    if ( type == DRIVE_TRACKED )  return "Tracked";
    if ( type == DRIVE_WINGED  )  return "Winged";
    if ( type == DRIVE_LEGGED  )  return "Legged";
    return "Other";
}

// Returns the type of tool.

ToolType GetTool(char *line, int rank)
{
    char*   p;

    p = SearchArg(line, rank);
    if ( *p == 0 )  return TOOL_OTHER;

    if ( Cmd(p, "Grabber"     ) )  return TOOL_GRABBER;
    if ( Cmd(p, "Sniffer"     ) )  return TOOL_SNIFFER;
    if ( Cmd(p, "Shooter"     ) )  return TOOL_SHOOTER;
    if ( Cmd(p, "OrgaShooter" ) )  return TOOL_ORGASHOOTER;

    return TOOL_OTHER;
}

// Returns the name of a tool.

const char* GetTool(ToolType type)
{
    if ( type == TOOL_GRABBER     )  return "Grabber";
    if ( type == TOOL_SNIFFER     )  return "Sniffer";
    if ( type == TOOL_SHOOTER     )  return "Shooter";
    if ( type == TOOL_ORGASHOOTER )  return "OrgaShooter";
    return "Other";
}

// Returns an integer.

int OpInt(char *line, const char *op, int def)
{
    line = SearchOp(line, op);
    if ( *line == 0 )  return def;
    return GetInt(line, 0, def);
}

// Returns a float number.

float OpFloat(char *line, const char *op, float def)
{
    line = SearchOp(line, op);
    if ( *line == 0 )  return def;
    return GetFloat(line, 0, def);
}

// Returns a string.

void OpString(char *line, const char *op, char *buffer)
{
    line = SearchOp(line, op);
    if ( *line == 0 )
    {
        buffer[0] = 0;
    }
    else
    {
        GetString(line, 0, buffer);
    }
}

// Returns the type of an object.

ObjectType OpTypeObject(char *line, const char *op, ObjectType def)
{
    line = SearchOp(line, op);
    if ( *line == 0 )  return def;
    return GetTypeObject(line, 0, def);
}

// Returns the type of a water.

Gfx::WaterType OpTypeWater(char *line, const char *op, Gfx::WaterType def)
{
    line = SearchOp(line, op);
    if ( *line == 0 )  return def;
    return GetTypeWater(line, 0, def);
}

// Returns the type of a terrain.

Gfx::EngineObjectType OpTypeTerrain(char *line, const char *op, Gfx::EngineObjectType def)
{
    line = SearchOp(line, op);
    if ( *line == 0 )  return def;
    return GetTypeTerrain(line, 0, def);
}

// Returns the type of research.

int OpResearch(char *line, const char *op)
{
    line = SearchOp(line, op);
    if ( *line == 0 )  return 0;
    return GetResearch(line, 0);
}

// Returns the type of pyrotechnic effect.

Gfx::PyroType OpPyro(char *line, const char *op)
{
    line = SearchOp(line, op);
    if ( *line == 0 )  return Gfx::PT_NULL;
    return GetPyro(line, 0);
}

// Returns the type of camera.

Gfx::CameraType OpCamera(char *line, const char *op)
{
    line = SearchOp(line, op);
    if ( *line == 0 )  return Gfx::CAM_TYPE_NULL;
    return GetCamera(line, 0);
}

// Returns the type of drive.

DriveType OpDrive(char *line, const char *op)
{
    line = SearchOp(line, op);
    if ( *line == 0 )  return DRIVE_OTHER;
    return GetDrive(line, 0);
}

// Returns the type of tool.

ToolType OpTool(char *line, const char *op)
{
    line = SearchOp(line, op);
    if ( *line == 0 )  return TOOL_OTHER;
    return GetTool(line, 0);
}

// Returns the type of a building.

int OpBuild(char *line, const char *op)
{
    line = SearchOp(line, op);
    if ( *line == 0 )  return 0;
    return GetBuild(line, 0);
}

// Returns a position in the XZ plane (top view).

Math::Vector OpPos(char *line, const char *op)
{
    Math::Vector    pos;

    line = SearchOp(line, op);
    if ( *line == 0 )
    {
        pos = Math::Vector(0.0f, 0.0f, 0.0f);
        return pos;
    }
    pos.x = GetFloat(line, 0, 0.0f);
    pos.y = 0.0f;
    pos.z = GetFloat(line, 1, 0.0f);
    return pos;
}

// Returns a direction.

Math::Vector OpDir(char *line, const char *op)
{
    Math::Vector    dir;

    line = SearchOp(line, op);
    if ( *line == 0 )
    {
        dir = Math::Vector(0.0f, 0.0f, 0.0f);
        return dir;
    }
    dir.x = GetFloat(line, 0, 0.0f);
    dir.y = GetFloat(line, 1, 0.0f);
    dir.z = GetFloat(line, 2, 0.0f);
    return dir;
}

// Reads a color (-1 .. 1).
Gfx::Color  OpColor(char *line, const char *op, Gfx::Color def)
{
    Gfx::Color color;

    line = SearchOp(line, op);
    if ( *line == 0 )  return def;

    color.r = GetFloat(line, 0, 0.0f);
    color.g = GetFloat(line, 1, 0.0f);
    color.b = GetFloat(line, 2, 0.0f);
    color.a = GetFloat(line, 3, 0.0f);
    return color;
}

