#include <SVHost.h>

int main(int argc, char *argv[]) {
    SVHost host(argc, argv);

    if (!host.Initialize()) {
        return 0;
    }

    host.Run();

    return 0;
}
