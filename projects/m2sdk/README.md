# m2sdk

This is a repo, containing up-to-date results of game research information provided in form of C++ header-only library.
By publishing this repo, we are hoping to find and attract more people that are interested in the Mafia 2 researching/modding/reversing,
and possibly even contributing to make this thing even better.

Prjects using this library:

* Mafia 2 Online

## Installtion

```sh
$ git clone git@github.com:mafia2online/m2sdk.git
```

```c
#include <m2sdk/m2sdk.h>
```

And add lib/m2sdk.lib to your linker.

## Usage

```c
#include <stdio.h>
#include <m2sdk/m2sdk.h>

void pluginRegisterCallback() {
    printf("game init!\n");

    M2::AttachHandler(M2_EVENT_CAR_ENTER, [](m2sdk_event *data) {
        auto player = (M2::C_Player2 *)data->arg1;
        auto car    = (M2::C_Car *)data->arg2;
        auto seat   = (int)data->arg3;

        printf("[game-event] ped entering the car on seat: %d\n", seat);
    });
}

BOOL APIENTRY DllMain(HMODULE module, DWORD  reason, LPVOID lpReserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH: {
            M2::Initialize(pluginRegisterCallback);
        }

        break;
    }
};

## Structure

Code devided into 3 different categories, by folders:

* classes - partially reversed game classes
* wrappers - lightweight wrappers around game classes to make things simplier to work w/
* utils - that is obvious

## Contributing

All contributions are highly welcomed! Feel free to suggest fixes, features, improvements.
And, of course, pull requests, we love 'em! ;)

## License

Project is licensed under Apache 2.0 license.
You can check it out [there](LICENSE).
