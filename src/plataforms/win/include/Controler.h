#ifndef CONTROLER_H
#define CONTROLER_H

#define INITGUID
#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>
#include <xinput.h>
#include <list>
#include <string>
using std::list;
using std::string;

namespace Tina
{
    enum XboxCtrl
    {
        DpadUp, DpadDown, DpadLeft, DpadRight,
        ButtonStart, ButtonBack,
        LeftThumb, RightThumb,
        LeftBumper, RightBumper,
        ButtonA, ButtonB, ButtonX, ButtonY,
        LeftTrigger, RightTrigger,
        ThumbLX, ThumbLY, ThumbRX, ThumbRY
    };

    enum XboxPlayer
    {
        PLAYER1,
        PLAYER2,
        PLAYER3,
        PLAYER4
    };

    enum JoyAxis
    {
        AxisX,
        AxisY,
        AxisZ,
        AxisRX,
        AxisRY,
        AxisRZ
    };

    struct JoyInfo
    {
        GUID guid;
        string name;
    };

    struct JoyPart
    {
        GUID guid;
        DWORD type;
        string name;
    };

    class Controller final
    {
    private:
        LPDIRECTINPUT8 dInput;
        LPDIRECTINPUTDEVICE8 joyDev;
        DIJOYSTATE joyState;
        XINPUT_STATE xboxState;
        XINPUT_VIBRATION vibration;
        bool joyCtrl[32];

        list<JoyPart> parts;
        list<JoyInfo> controllers;
        list<JoyInfo>::const_iterator selected;

    public:
        Controller() noexcept;
        ~Controller() noexcept;

        bool Initialize() noexcept;
        bool XboxInitialize(const int ctrl = 0) const noexcept;

        bool UpdateState() noexcept;
        bool XboxUpdateState(const int ctrl = 0) noexcept;

        bool ButtonDown(const int button) const noexcept;
        bool ButtonUp(const int button) const noexcept;
        bool ButtonPress(int button) const noexcept;
        long Axis(const int axis) const noexcept;
        long Slider(const int slider) const noexcept;
        long Dpad(const int pov = 0) const noexcept;

        bool DeviceNext() noexcept;
        bool DevicePrev() noexcept;
        const string * DeviceName() const noexcept;
        list<JoyPart> & DeviceParts() noexcept;

        int XboxButton(const int button) const noexcept;
        int XboxTrigger(const int trigger) const noexcept;
        int XboxAnalog(const int stick) const noexcept;
        void XboxVibrate(int ctrl = 0,
            int left = 65535,
            int right = 65535) noexcept;
    };

    inline long Controller::Slider(const int slider) const noexcept
    { return joyState.rglSlider[slider]; }

    inline long Controller::Dpad(const int pov) const noexcept
    { return joyState.rgdwPOV[pov]; }

    bool Controller::XboxUpdateState(const int ctrl) noexcept
    { return (XInputGetState(ctrl, &xboxState)) ? false : true; }

    inline bool Controller::ButtonDown(const int button) const noexcept
    { return joyState.rgbButtons[button] && 0x80; }

    inline bool Controller::ButtonUp(const int button) const noexcept
    { return !(joyState.rgbButtons[button] && 0x80); }

    inline const string * Controller::DeviceName() const noexcept
    { return &(*selected).name; }

    inline list<JoyPart> & Controller::DeviceParts() noexcept
    { return parts; }

    BOOL CALLBACK EnumerateControllers(LPCDIDEVICEINSTANCE lpDDi, LPVOID data) noexcept;
    BOOL CALLBACK EnumControllerParts(LPCDIDEVICEOBJECTINSTANCE lpDIdoi, LPVOID data) noexcept;
}

#endif