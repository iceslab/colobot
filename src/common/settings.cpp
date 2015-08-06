/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2015, Daniel Roux, EPSITEC SA & TerranovaTeam
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
#include "common/settings.h"

#include "app/app.h"
#include "app/input.h"

#include "common/config_file.h"

#include "graphics/engine/camera.h"
#include "graphics/engine/engine.h"

#include "object/robotmain.h"

template<> CSettings* CSingleton<CSettings>::m_instance = nullptr;

CSettings::CSettings()
{
    m_tooltips       = true;
    m_interfaceGlint = true;
    m_interfaceRain  = true;
    m_soluce4        = true;
    m_movies         = true;
    m_niceReset      = true;
    m_himselfDamage  = true;
    m_systemMouse    = false;

    m_fontSize  = 19.0f;
    m_windowPos = Math::Point(0.15f, 0.17f);
    m_windowDim = Math::Point(0.70f, 0.66f);

    m_IOPublic = false;
    m_IODim = Math::Point(320.0f/640.0f, (121.0f+18.0f*8)/480.0f);
    m_IOPos.x = (1.0f-m_IODim.x)/2.0f;  // in the middle
    m_IOPos.y = (1.0f-m_IODim.y)/2.0f;
}

void CSettings::SaveResolutionSettings(const Gfx::DeviceConfig& config)
{
    // NOTE: These settings are loaded in CApplication

    std::ostringstream ss;
    ss << config.size.x << "x" << config.size.y;
    GetConfigFile().SetStringProperty("Setup", "Resolution", ss.str());
    GetConfigFile().SetIntProperty("Setup", "Fullscreen", config.fullScreen);
    GetConfigFile().Save();
}

void CSettings::SaveSettings()
{
    CApplication* app = CApplication::GetInstancePointer();
    CRobotMain* main = CRobotMain::GetInstancePointer();
    Gfx::CEngine* engine = Gfx::CEngine::GetInstancePointer();
    Gfx::CCamera* camera = main->GetCamera();
    CSoundInterface* sound = app->GetSound();

    GetConfigFile().SetIntProperty("Setup", "TotoMode", engine->GetTotoMode());

    GetConfigFile().SetIntProperty("Setup", "Tooltips", m_tooltips);
    GetConfigFile().SetIntProperty("Setup", "InterfaceGlint", m_interfaceGlint);
    GetConfigFile().SetIntProperty("Setup", "InterfaceRain", m_interfaceRain);
    GetConfigFile().SetIntProperty("Setup", "Soluce4", m_soluce4);
    GetConfigFile().SetIntProperty("Setup", "Movies", m_movies);
    GetConfigFile().SetIntProperty("Setup", "NiceReset", m_niceReset);
    GetConfigFile().SetIntProperty("Setup", "HimselfDamage", m_himselfDamage);
    GetConfigFile().SetIntProperty("Setup", "CameraScroll", camera->GetCameraScroll());
    GetConfigFile().SetIntProperty("Setup", "CameraInvertX", camera->GetCameraInvertX());
    GetConfigFile().SetIntProperty("Setup", "CameraInvertY", camera->GetCameraInvertY());
    GetConfigFile().SetIntProperty("Setup", "InterfaceEffect", camera->GetEffect());
    GetConfigFile().SetIntProperty("Setup", "Blood", camera->GetBlood());
    GetConfigFile().SetIntProperty("Setup", "Autosave", main->GetAutosave());
    GetConfigFile().SetIntProperty("Setup", "AutosaveInterval", main->GetAutosaveInterval());
    GetConfigFile().SetIntProperty("Setup", "AutosaveSlots", main->GetAutosaveSlots());
    GetConfigFile().SetIntProperty("Setup", "GroundShadow", engine->GetShadow());
    GetConfigFile().SetIntProperty("Setup", "GroundSpot", engine->GetGroundSpot());
    GetConfigFile().SetIntProperty("Setup", "ObjectDirty", engine->GetDirty());
    GetConfigFile().SetIntProperty("Setup", "FogMode", engine->GetFog());
    GetConfigFile().SetIntProperty("Setup", "LensMode", engine->GetLensMode());
    GetConfigFile().SetIntProperty("Setup", "SkyMode", engine->GetSkyMode());
    GetConfigFile().SetIntProperty("Setup", "PlanetMode", engine->GetPlanetMode());
    GetConfigFile().SetIntProperty("Setup", "LightMode", engine->GetLightMode());
    GetConfigFile().SetIntProperty("Setup", "UseJoystick", app->GetJoystickEnabled() ? app->GetJoystick().index : -1);
    GetConfigFile().SetFloatProperty("Setup", "ParticleDensity", engine->GetParticleDensity());
    GetConfigFile().SetFloatProperty("Setup", "ClippingDistance", engine->GetClippingDistance());
    GetConfigFile().SetFloatProperty("Setup", "GadgetQuantity", engine->GetGadgetQuantity());
    GetConfigFile().SetIntProperty("Setup", "AudioVolume", sound->GetAudioVolume());
    GetConfigFile().SetIntProperty("Setup", "MusicVolume", sound->GetMusicVolume());
    GetConfigFile().SetIntProperty("Setup", "EditIndentMode", engine->GetEditIndentMode());
    GetConfigFile().SetIntProperty("Setup", "EditIndentValue", engine->GetEditIndentValue());
    GetConfigFile().SetIntProperty("Setup", "SystemMouse", m_systemMouse);

    GetConfigFile().SetIntProperty("Setup", "MipmapLevel", engine->GetTextureMipmapLevel());
    GetConfigFile().SetIntProperty("Setup", "Anisotropy", engine->GetTextureAnisotropyLevel());
    GetConfigFile().SetFloatProperty("Setup", "ShadowColor", engine->GetShadowColor());
    GetConfigFile().SetFloatProperty("Setup", "ShadowRange", engine->GetShadowRange());
    GetConfigFile().SetIntProperty("Setup", "MSAA", engine->GetMultiSample());
    GetConfigFile().SetIntProperty("Setup", "FilterMode", engine->GetTextureFilterMode());
    GetConfigFile().SetIntProperty("Setup", "ShadowMapping", engine->GetShadowMapping());
    GetConfigFile().SetIntProperty("Setup", "ShadowMappingQuality", engine->GetShadowMappingQuality());
    GetConfigFile().SetIntProperty("Setup", "ShadowMappingResolution", engine->GetShadowMappingOffscreen() ? engine->GetShadowMappingOffscreenResolution() : 0);

    CInput::GetInstancePointer()->SaveKeyBindings();



    GetConfigFile().SetFloatProperty("Edit", "FontSize", m_fontSize);
    GetConfigFile().SetFloatProperty("Edit", "WindowPosX", m_windowPos.x);
    GetConfigFile().SetFloatProperty("Edit", "WindowPosY", m_windowPos.y);
    GetConfigFile().SetFloatProperty("Edit", "WindowDimX", m_windowDim.x);
    GetConfigFile().SetFloatProperty("Edit", "WindowDimY", m_windowDim.y);
    GetConfigFile().SetIntProperty("Edit", "IOPublic", m_IOPublic);
    GetConfigFile().SetFloatProperty("Edit", "IOPosX", m_IOPos.x);
    GetConfigFile().SetFloatProperty("Edit", "IOPosY", m_IOPos.y);
    GetConfigFile().SetFloatProperty("Edit", "IODimX", m_IODim.x);
    GetConfigFile().SetFloatProperty("Edit", "IODimY", m_IODim.y);


    GetConfigFile().Save();
}

void CSettings::LoadSettings()
{
    CApplication* app = CApplication::GetInstancePointer();
    CRobotMain* main = CRobotMain::GetInstancePointer();
    Gfx::CEngine* engine = Gfx::CEngine::GetInstancePointer();
    Gfx::CCamera* camera = main->GetCamera();
    CSoundInterface* sound = app->GetSound();

    float       fValue;
    int         iValue;
    std::string key;

    if (GetConfigFile().GetIntProperty("Setup", "TotoMode", iValue))
        engine->SetTotoMode(iValue);


    if (GetConfigFile().GetIntProperty("Setup", "Tooltips", iValue))
        m_tooltips = iValue;

    if (GetConfigFile().GetIntProperty("Setup", "InterfaceGlint", iValue))
        m_interfaceGlint = iValue;

    if (GetConfigFile().GetIntProperty("Setup", "InterfaceRain", iValue))
        m_interfaceRain = iValue;

    if (GetConfigFile().GetIntProperty("Setup", "Soluce4", iValue))
        m_soluce4 = iValue;

    if (GetConfigFile().GetIntProperty("Setup", "Movies", iValue))
        m_movies = iValue;

    if (GetConfigFile().GetIntProperty("Setup", "NiceReset", iValue))
        m_niceReset = iValue;

    if (GetConfigFile().GetIntProperty("Setup", "HimselfDamage", iValue))
        m_himselfDamage = iValue;

    if (GetConfigFile().GetIntProperty("Setup", "CameraScroll", iValue))
        camera->SetCameraScroll(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "CameraInvertX", iValue))
        camera->SetCameraInvertX(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "CameraInvertY", iValue))
        camera->SetCameraInvertY(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "InterfaceEffect", iValue))
        camera->SetEffect(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "Blood", iValue))
        camera->SetBlood(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "Autosave", iValue))
        main->SetAutosave(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "AutosaveInterval", iValue))
        main->SetAutosaveInterval(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "AutosaveSlots", iValue))
        main->SetAutosaveSlots(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "GroundShadow", iValue))
        engine->SetShadow(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "GroundSpot", iValue))
        engine->SetGroundSpot(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "ObjectDirty", iValue))
        engine->SetDirty(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "FogMode", iValue))
    {
        engine->SetFog(iValue);
        camera->SetOverBaseColor(Gfx::Color(0.0f, 0.0f, 0.0f, 0.0f)); // TODO: color ok?
    }

    if (GetConfigFile().GetIntProperty("Setup", "LensMode", iValue))
        engine->SetLensMode(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "SkyMode", iValue))
        engine->SetSkyMode(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "PlanetMode", iValue))
        engine->SetPlanetMode(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "LightMode", iValue))
        engine->SetLightMode(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "UseJoystick", iValue))
    {
        if(iValue >= 0)
        {
            auto joysticks = app->GetJoystickList();
            for(const auto& joystick : joysticks)
            {
                if (joystick.index == iValue)
                {
                    app->ChangeJoystick(joystick);
                    app->SetJoystickEnabled(true);
                }
            }
        }
        else
        {
            app->SetJoystickEnabled(false);
        }
    }

    if (GetConfigFile().GetFloatProperty("Setup", "ParticleDensity", fValue))
        engine->SetParticleDensity(fValue);

    if (GetConfigFile().GetFloatProperty("Setup", "ClippingDistance", fValue))
        engine->SetClippingDistance(fValue);

    if (GetConfigFile().GetFloatProperty("Setup", "GadgetQuantity", fValue))
        engine->SetGadgetQuantity(fValue);

    if (GetConfigFile().GetIntProperty("Setup", "AudioVolume", iValue))
        sound->SetAudioVolume(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "MusicVolume", iValue))
        sound->SetMusicVolume(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "EditIndentMode", iValue))
        engine->SetEditIndentMode(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "EditIndentValue", iValue))
        engine->SetEditIndentValue(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "SystemMouse", iValue))
    {
        m_systemMouse = iValue;
        app->SetMouseMode(m_systemMouse ? MOUSE_SYSTEM : MOUSE_ENGINE);
    }


    if (GetConfigFile().GetIntProperty("Setup", "MipmapLevel", iValue))
        engine->SetTextureMipmapLevel(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "Anisotropy", iValue))
        engine->SetTextureAnisotropyLevel(iValue);

    if (GetConfigFile().GetFloatProperty("Setup", "ShadowColor", fValue))
        engine->SetShadowColor(fValue);

    if (GetConfigFile().GetFloatProperty("Setup", "ShadowRange", fValue))
        engine->SetShadowRange(fValue);

    if (GetConfigFile().GetIntProperty("Setup", "MSAA", iValue))
        engine->SetMultiSample(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "FilterMode", iValue))
        engine->SetTextureFilterMode(static_cast<Gfx::TexFilter>(iValue));

    if (GetConfigFile().GetIntProperty("Setup", "ShadowMapping", iValue))
        engine->SetShadowMapping(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "ShadowMappingQuality", iValue))
        engine->SetShadowMappingQuality(iValue);

    if (GetConfigFile().GetIntProperty("Setup", "ShadowMappingResolution", iValue))
    {
        if(iValue == 0)
        {
            engine->SetShadowMappingOffscreen(false);
        }
        else
        {
            engine->SetShadowMappingOffscreen(true);
            engine->SetShadowMappingOffscreenResolution(iValue);
        }
    }

    CInput::GetInstancePointer()->LoadKeyBindings();



    if (GetConfigFile().GetFloatProperty("Edit", "FontSize",    fValue)) m_fontSize    = fValue;
    if (GetConfigFile().GetFloatProperty("Edit", "WindowPosX",  fValue)) m_windowPos.x = fValue;
    if (GetConfigFile().GetFloatProperty("Edit", "WindowPosY",  fValue)) m_windowPos.y = fValue;
    if (GetConfigFile().GetFloatProperty("Edit", "WindowDimX",  fValue)) m_windowDim.x = fValue;
    if (GetConfigFile().GetFloatProperty("Edit", "WindowDimY",  fValue)) m_windowDim.y = fValue;

    if (GetConfigFile().GetIntProperty  ("Edit", "IOPublic", iValue)) m_IOPublic = iValue;
    if (GetConfigFile().GetFloatProperty("Edit", "IOPosX",   fValue)) m_IOPos.x  = fValue;
    if (GetConfigFile().GetFloatProperty("Edit", "IOPosY",   fValue)) m_IOPos.y  = fValue;
    if (GetConfigFile().GetFloatProperty("Edit", "IODimX",   fValue)) m_IODim.x  = fValue;
    if (GetConfigFile().GetFloatProperty("Edit", "IODimY",   fValue)) m_IODim.y  = fValue;
}

void CSettings::SetTooltips(bool tooltips)
{
    m_tooltips = tooltips;
}
bool CSettings::GetTooltips()
{
    return m_tooltips;
}

void CSettings::SetInterfaceGlint(bool interfaceGlint)
{
    m_interfaceGlint = interfaceGlint;
}
bool CSettings::GetInterfaceGlint()
{
    return m_interfaceGlint;
}

void CSettings::SetInterfaceRain(bool interfaceRain)
{
    m_interfaceRain = interfaceRain;
}
bool CSettings::GetInterfaceRain()
{
    return m_interfaceRain;
}

void CSettings::SetSoluce4(bool soluce4)
{
    m_soluce4 = soluce4;
}
bool CSettings::GetSoluce4()
{
    return m_soluce4;
}

void CSettings::SetMovies(bool movies)
{
    m_movies = movies;
}
bool CSettings::GetMovies()
{
    return m_movies;
}

void CSettings::SetNiceReset(bool niceReset)
{
    m_niceReset = niceReset;
}
bool CSettings::GetNiceReset()
{
    return m_niceReset;
}

void CSettings::SetHimselfDamage(bool himselfDamage)
{
    m_himselfDamage = himselfDamage;
}
bool CSettings::GetHimselfDamage()
{
    return m_himselfDamage;
}

void CSettings::SetSystemMouse(bool systemMouse)
{
    m_systemMouse = systemMouse;
}
bool CSettings::GetSystemMouse()
{
    return m_systemMouse;
}


void CSettings::SetFontSize(float size)
{
    m_fontSize = size;
    GetConfigFile().SetFloatProperty("Edit", "FontSize", m_fontSize);
    GetConfigFile().Save();
}

float CSettings::GetFontSize()
{
    return m_fontSize;
}

void CSettings::SetWindowPos(Math::Point pos)
{
    m_windowPos = pos;
    GetConfigFile().SetFloatProperty("Edit", "WindowPosX", m_windowPos.x);
    GetConfigFile().SetFloatProperty("Edit", "WindowPosY", m_windowPos.y);
    GetConfigFile().Save();
}

Math::Point CSettings::GetWindowPos()
{
    return m_windowPos;
}

void CSettings::SetWindowDim(Math::Point dim)
{
    m_windowDim = dim;
    GetConfigFile().SetFloatProperty("Edit", "WindowDimX", m_windowDim.x);
    GetConfigFile().SetFloatProperty("Edit", "WindowDimY", m_windowDim.y);
    GetConfigFile().Save();
}

Math::Point CSettings::GetWindowDim()
{
    return m_windowDim;
}

void CSettings::SetIOPublic(bool mode)
{
    m_IOPublic = mode;
    GetConfigFile().SetIntProperty("Edit", "IOPublic", m_IOPublic);
    GetConfigFile().Save();
}

bool CSettings::GetIOPublic()
{
    return m_IOPublic;
}

void CSettings::SetIOPos(Math::Point pos)
{
    m_IOPos = pos;
    GetConfigFile().SetFloatProperty("Edit", "IOPosX", m_IOPos.x);
    GetConfigFile().SetFloatProperty("Edit", "IOPosY", m_IOPos.y);
    GetConfigFile().Save();
}

Math::Point CSettings::GetIOPos()
{
    return m_IOPos;
}

void CSettings::SetIODim(Math::Point dim)
{
    m_IODim = dim;
    GetConfigFile().SetFloatProperty("Edit", "IODimX", m_IODim.x);
    GetConfigFile().SetFloatProperty("Edit", "IODimY", m_IODim.y);
    GetConfigFile().Save();
}

Math::Point CSettings::GetIODim()
{
    return m_IODim;
}
