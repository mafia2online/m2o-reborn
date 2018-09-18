#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <TlHelp32.h>

typedef HRESULT (WINAPI *input_dxi8create_cb)(HINSTANCE, DWORD, REFIID, LPVOID *, LPUNKNOWN);
typedef int (WINAPI *input_showcursor_cb)(BOOL bShow);

typedef struct {
    int id;
    int state;
} input_mousebtn_t;

enum {
    ZINPUT_MOUSE,
    ZINPUT_KEYBOARD,
    ZINPUT_JOYSTICK,
};

class CDirectInputDevice8Proxy;

struct input_state {
    IDirectInput8 *proxy;
    input_dxi8create_cb hooked_dxi8create;
    input_showcursor_cb hooked_showcursor;

    bool installed;
    bool blocked;

    CDirectInputDevice8Proxy* devices[4];

    struct {
        int x;
        int y;
        int z; /* wheel */

        struct _DIMOUSESTATE state;

        union {
            struct {
                input_mousebtn_t left;
                input_mousebtn_t right;
                input_mousebtn_t middle;
            } btn;

            input_mousebtn_t buttons[3];
        };

    } mouse;
};

static input_state _input_state;

#include "input/CDirectInputDevice8Proxy.h"
#include "input/CDirectInput8Proxy.h"

// =======================================================================//
// !
// ! Hooking
// !
// =======================================================================//

HRESULT WINAPI input_dxi8create_hook(HINSTANCE hInst, DWORD dwVersion, REFIID riidltf, LPVOID * ppvOut, LPUNKNOWN punkOuter) {
    HRESULT hr = _input_state.hooked_dxi8create(hInst, dwVersion, riidltf, ppvOut, punkOuter);

    if (SUCCEEDED(hr)) {
        IDirectInput8 *pInput = static_cast<IDirectInput8*>(*ppvOut);
        CDirectInput8Proxy *proxy = new CDirectInput8Proxy(pInput);
        proxy->AddRef();

        *ppvOut = proxy;
        _input_state.proxy = proxy;
    }

    return hr;
}

int WINAPI input_showcursor_hook(BOOL bShow) {
    if (!bShow)
        return -1;
    else
        return 1;
}

// =======================================================================//
// !
// ! Control methods
// !
// =======================================================================//

void input_init() {
    if (_input_state.installed == false) {
        _input_state.installed = true;
        _input_state.hooked_dxi8create = (input_dxi8create_cb)(Mem::Hooks::InstallDetourPatch("dinput8.dll", "DirectInput8Create", (DWORD)input_dxi8create_hook));
        _input_state.hooked_showcursor = (input_showcursor_cb)Mem::Hooks::InstallDetourPatch("user32.dll", "ShowCursor", (DWORD)input_showcursor_hook);
    }
}

void input_free() {
    if (_input_state.installed) {
        _input_state.installed = false;
        Mem::Hooks::UninstallDetourPatch(_input_state.hooked_dxi8create, (DWORD)input_dxi8create_hook);
        Mem::Hooks::UninstallDetourPatch(_input_state.hooked_showcursor, (DWORD)input_showcursor_hook);
    }
}

void input_mouse_position(int *x, int *y) {
    *x = _input_state.mouse.x;
    *y = _input_state.mouse.y;
}

int input_inject_event(void *evt) {
    auto event = (SDL_Event *)evt;

    switch (event->type) {
        case SDL_MOUSEMOTION:
            break;
    }

    return 0;
}

void input_block_set(bool value) {
    _input_state.blocked = value;

    if (value) {
        _input_state.devices[ZINPUT_MOUSE]->masterAquired = false;
        _input_state.devices[ZINPUT_MOUSE]->Unacquire();
    }
    else {
        _input_state.devices[ZINPUT_MOUSE]->masterAquired = true;
        _input_state.devices[ZINPUT_MOUSE]->Acquire();
    }

    if (value) {
        _input_state.hooked_showcursor(true);
    }
    else {
        while (_input_state.hooked_showcursor(false) >= 0) {}
    }
}

bool input_block_get() {
    return _input_state.blocked;
}

bool input_key_up(u32 key) {
    return !input_key_down(key);
}

bool input_key_down(u32 key) {
    return GetAsyncKeyState(key) & 0x1;
}
