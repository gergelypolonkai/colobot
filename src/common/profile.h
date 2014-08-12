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

/**
 * \file common/profile.h
 * \brief Class for loading profile (currently for loading ini config file)
 */

#pragma once

#include "common/singleton.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/replace.hpp>

#include <string>
#include <vector>

namespace fs = boost::filesystem;


/**
* \class CProfile
*
* \brief Class for loading profile (currently for loading ini config file)
*
*/
class CProfile : public CSingleton<CProfile>
{
public:
    CProfile();
    virtual ~CProfile();

    /** Set flag to force using ini file from current directory */
    void SetUseCurrentDirectory(bool useCurrentDirectory);

    /** Loads colobot.ini from current directory
     * \return return true on success
     */
    bool Init();

    /** Saves colobot.ini to current directory
     * \return return true on success
     */
    bool Save();

    /** Sets string value in section under specified key
     * \param section
     * \param key
     * \param value
     * \return return true on success
     */
    bool SetStringProperty(std::string section, std::string key, std::string value);

    /** Gets string value in section under specified key
     * \param section
     * \param key
     * \param buffer
     * \return return true on success
     */
    bool GetStringProperty(std::string section, std::string key, std::string& buffer);

    /** Sets int value in section under specified key
     * \param section
     * \param key
     * \param value
     * \return return true on success
     */
    bool SetIntProperty(std::string section, std::string key, int value);

    /** Gets int value in section under specified key
     * \param section
     * \param key
     * \param value
     * \return return true on success
     */
    bool GetIntProperty(std::string section, std::string key, int &value);

    /** Sets float value in section under specified key
     * \param section
     * \param key
     * \param value
     * \return return true on success
     */
    bool SetFloatProperty(std::string section, std::string key, float value);

    /** Gets float value in section under specified key
     * \param section
     * \param key
     * \param value
     * \return return true on success
     */
    bool GetFloatProperty(std::string section, std::string key, float &value);

    /** Gets all values in section under specified key
     * \param section
     * \param key
     * \return vector of values
     */
    std::vector< std::string > GetSection(std::string section, std::string key);

    /** Sets current user directory
     * \param dir
     */
    void SetUserDir(std::string dir);

    /** Returns path based on current user. Replaces %user% in path with current user dir or
     * uses default_dir param if no user dir is specified
     * \param dir
     * \param default_dir
     * \return path
     */
    std::string GetUserBasedPath(std::string dir, std::string defaultDir);

    /** Copy a file into the temporary folder.
     * \param filename
     * \return true on success
     */
    bool CopyFileToTemp(std::string filename);

private:
    std::string GetIniFileLocation();

private:
    boost::property_tree::ptree m_propertyTree;
    bool m_profileNeedSave;
    std::string m_userDirectory;
    bool m_useCurrentDirectory;
};

//! Global function to get profile instance
inline CProfile & GetProfile()
{
    return *CProfile::GetInstancePointer();
}

