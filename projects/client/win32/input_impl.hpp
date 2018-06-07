
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <TlHelp32.h>

typedef HRESULT (WINAPI *input_dxi8create_cb)(HINSTANCE, DWORD, REFIID, LPVOID *, LPUNKNOWN);

typedef struct {
    int id;
    int state;
} input_mousebtn_t;

struct input_state {
    IDirectInput8 *proxy;
    input_dxi8create_cb method;

    bool installed;
    bool blocked;

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
    HRESULT hr = _input_state.method(hInst, dwVersion, riidltf, ppvOut, punkOuter);

    if (SUCCEEDED(hr)) {
        IDirectInput8 *pInput = static_cast<IDirectInput8*>(*ppvOut);
        CDirectInput8Proxy *proxy = new CDirectInput8Proxy(pInput);
        proxy->AddRef();

        *ppvOut = proxy;
        _input_state.proxy = proxy;
    }

    return hr;
}

// =======================================================================//
// !
// ! Control methods
// !
// =======================================================================//

void input_init() {
    if (_input_state.installed == false) {
        _input_state.installed = true;
        _input_state.method = (input_dxi8create_cb)(Mem::Hooks::InstallDetourPatch("dinput8.dll", "DirectInput8Create", (DWORD)input_dxi8create_hook));
    }
}

void input_free() {
    if (_input_state.installed) {
        _input_state.installed = false;
        Mem::Hooks::UninstallDetourPatch(_input_state.method, (DWORD)input_dxi8create_hook);
    }
}

void input_mouse_position(int *x, int *y) {
    *x = _input_state.mouse.x;
    *y = _input_state.mouse.y;
}

// void input_key_up()
