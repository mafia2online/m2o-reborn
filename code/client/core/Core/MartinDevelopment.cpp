
#include <Hooking.h>
#include <future>

// DO NOT TOUCH
// This is martin's dev cavehole
// I don't wanna get merge conflicts again
// be lovely and just watch, do not touch
// or I will touch you!

static nomad::base_function init([]() {

    // Fix for game shutdown function stack (esp) cleanup destroying the stack
    // causing the game to crash (we need this since we're jumping inside the game
    // startup without doing the stack preparations at the start of WinMain).
    nio::nop(0x00401D71, 10);
});
