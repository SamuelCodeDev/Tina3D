#include "Controller.h"
#include "Engine.h"
#include "Utils.h"
#include <cmath>

namespace Tina
{
    Controller::Controller() noexcept : 
        dInput{nullptr},
        joyDev{nullptr},
        joyState{},
        joyCtrl{},
        xboxState{},
        vibration{}
    {
        DirectInput8Create(
            GetModuleHandle(nullptr),
            DIRECTINPUT_VERSION,
            IID_IDirectInput8,
            reinterpret_cast<void**>(&dInput),
            nullptr
        );

        dInput->EnumDevices(
            DI8DEVCLASS_GAMECTRL,
            EnumerateControllers,
            &controllers,
            DIEDFL_ATTACHEDONLY
        );

        selected = controllers.begin();
    }

    Controller::~Controller() noexcept
    {
        if (joyDev)
        {
            joyDev->Unacquire();
            joyDev->Release();
        }

        SafeRelease(dInput);
    }

    bool Controller::Initialize() noexcept
    {
        if (!dInput)
            return false;

        if (controllers.empty())
            return false;

        if (joyDev)
        {
            joyDev->Unacquire();
            joyDev->Release();
        }    

        if (FAILED(dInput->CreateDevice(selected->guid, &joyDev, nullptr)))
            return false;

        if (FAILED(joyDev->SetCooperativeLevel(Engine::window->Id(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
            return false;

        if (FAILED(joyDev->SetDataFormat(&c_dfDIJoystick)))
            return false;

        parts.clear();
        if(FAILED(joyDev->EnumObjects(EnumControllerParts, &parts, DIDFT_ALL)))
            return false;
        
        for (const auto & p : parts)
        {
            DIPROPRANGE axisRange;
            DIPROPDWORD deadZone;
            
            if (p.type & DIDFT_AXIS)
            {
                axisRange.diph.dwSize       = sizeof(DIPROPRANGE); 
                axisRange.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
                axisRange.diph.dwHow        = DIPH_BYID; 
                axisRange.diph.dwObj        = p.type;
                axisRange.lMin              = -1000; 
                axisRange.lMax              = +1000; 
        
                if (FAILED(joyDev->SetProperty(DIPROP_RANGE, &axisRange.diph))) 
                    return false;

                deadZone.diph.dwSize       = sizeof(DIPROPDWORD);
                deadZone.diph.dwHeaderSize = sizeof(DIPROPHEADER);
                deadZone.diph.dwHow        = DIPH_BYID;
                deadZone.diph.dwObj        = p.type;
                deadZone.dwData            = 2000;  

                if (FAILED(joyDev->SetProperty(DIPROP_DEADZONE, &deadZone.diph)))
                    return false;
            }
        }

        return (FAILED(joyDev->Acquire())) ? false : true;
    }

    bool Controller::XboxInitialize(const int ctrl) const noexcept
    {
        XINPUT_CAPABILITIES caps;
        ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));
        
        XInputGetCapabilities(ctrl, XINPUT_FLAG_GAMEPAD, &caps);

        return (caps.SubType == XINPUT_DEVSUBTYPE_UNKNOWN) ? false : true;
    }

    bool Controller::UpdateState() noexcept
    {
        if (FAILED(joyDev->Poll()))
            return false;

        HRESULT result = joyDev->GetDeviceState(sizeof(DIJOYSTATE), static_cast<void*>(&joyState)); 

        if(FAILED(result))
        {
            if (result == DIERR_NOTACQUIRED || result == DIERR_INPUTLOST)
            {
                joyDev->Unacquire();
                joyDev->Acquire();
                joyDev->GetDeviceState(sizeof(DIJOYSTATE), static_cast<void*>(&joyState));
            }
            else
            {
                return false;
            }    
        }
        
        return true;
    }

    bool Controller::ButtonPress(const int button) const noexcept
    {
        if (joyCtrl[button])
        {
            if (ButtonDown(button))
            {
                joyCtrl[button] = false;
                return true;
            }
        } 
        else if (ButtonUp(button))
        {
            joyCtrl[button] = true;
        }

        return false;
    }

    long Controller::Axis(const int axis) const noexcept
    { 
        switch (axis)
        {
        case AxisX: return joyState.lX; 
        case AxisY: return joyState.lY; 
        case AxisZ: return joyState.lZ;
        case AxisRX: return joyState.lRx; 
        case AxisRY: return joyState.lRy; 
        case AxisRZ: return joyState.lRz;
        default: return 0;
        }    
    }

    bool Controller::DeviceNext() noexcept
    {
        ++selected;

        if (selected == controllers.end())
            selected = controllers.begin();        

        return (Initialize()) ? true : false;
    }

    bool Controller::DevicePrev() noexcept
    {
        if (selected == controllers.begin())
            selected = controllers.end();

        --selected;
        
        return (Initialize()) ? true : false;
    }

    BOOL CALLBACK EnumerateControllers(LPCDIDEVICEINSTANCE lpDDi, LPVOID data) noexcept
    {
        JoyInfo joy {
            .guid = lpDDi->guidInstance,
            .name = lpDDi->tszInstanceName,
        };

        ((list<JoyInfo>*) data)->push_back(joy);

        return DIENUM_CONTINUE;
    }

    BOOL CALLBACK EnumControllerParts(LPCDIDEVICEOBJECTINSTANCE lpDIdoi, LPVOID data) noexcept
    {
        JoyPart part {
            .guid = lpDIdoi->guidType,
            .type = lpDIdoi->dwType,
            .name = lpDIdoi->tszName,
        };
        
        if (lpDIdoi->guidType != GUID_Unknown)
            ((list<JoyPart>*) data)->push_back(part);

        return DIENUM_CONTINUE;
    }

    int Controller::XboxButton(const int button) const noexcept
    {
        using Buttons = xboxState.Gamepad.wButtons;
        switch (button)
        {
        case DpadUp: return (Buttons & XINPUT_GAMEPAD_DPAD_UP); break;
        case DpadDown: return (Buttons & XINPUT_GAMEPAD_DPAD_DOWN); break;
        case DpadLeft: return (Buttons & XINPUT_GAMEPAD_DPAD_LEFT); break;
        case DpadRight: return (Buttons & XINPUT_GAMEPAD_DPAD_RIGHT); break;
        case ButtonStart: return (Buttons & XINPUT_GAMEPAD_START); break;
        case ButtonBack: return (Buttons & XINPUT_GAMEPAD_BACK); break;
        case LeftThumb: return (Buttons & XINPUT_GAMEPAD_LEFT_THUMB); break;
        case RightThumb: return (Buttons & XINPUT_GAMEPAD_RIGHT_THUMB); break;
        case LeftBumper: return (Buttons & XINPUT_GAMEPAD_LEFT_SHOULDER); break;
        case RightBumper: return (Buttons & XINPUT_GAMEPAD_RIGHT_SHOULDER); break;
        case ButtonA: return (Buttons & XINPUT_GAMEPAD_A); break;
        case ButtonB: return (Buttons & XINPUT_GAMEPAD_B); break;
        case ButtonX: return (Buttons & XINPUT_GAMEPAD_X); break;
        case ButtonY: return (Buttons & XINPUT_GAMEPAD_Y); break;
        default: return false;
        }
    }

    int Controller::XboxTrigger(const int trigger) const noexcept
    {
        switch (trigger)
        {
        case LeftTrigger: return xboxState.Gamepad.bLeftTrigger; break;
        case RightTrigger: return xboxState.Gamepad.bRightTrigger; break;
        default: return 0;
        }
    }

    int Controller::XboxAnalog(const int stick) const noexcept
    {
        switch (stick)
        {
        case ThumbLX:
            if (abs(xboxState.Gamepad.sThumbLX) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
                return xboxState.Gamepad.sThumbLX;
            break;
        case ThumbLY:
            if (abs(xboxState.Gamepad.sThumbLY) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
                return xboxState.Gamepad.sThumbLY;
            break;
        case ThumbRX:
            if (abs(xboxState.Gamepad.sThumbRX) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
                return xboxState.Gamepad.sThumbRX;
            break;
        case ThumbRY:
            if (abs(xboxState.Gamepad.sThumbRY) > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
                return xboxState.Gamepad.sThumbRY;
            break;
        default:
            return 0;
        }
    }

    void Controller::XboxVibrate(const int ctrl, const int left, const int right) noexcept
    {
        vibration.wLeftMotorSpeed = left;
        vibration.wRightMotorSpeed = right;
        XInputSetState(ctrl, &vibration);
    }
}