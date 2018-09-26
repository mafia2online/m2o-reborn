#define MASTERLIST_PATTERN "{\"nm\":\"%s\",\"cp\":%d,\"mp\":%d,\"pp\":%d,\"sp\":\"%d\"}"
#define MASTERLIST_SECONDS 90.0

const char *masterlist_url = "http://localhost:8000/push";
http_t* masterlist_request = NULL;

void masterlist_update(void *ptr) {
    if (masterlist_request) return;

    char buffer[512] = {0};
    zpl_snprintf(buffer, 512, MASTERLIST_PATTERN,
        mod.settings.hostname.c_str(),
        mod.connected_amount,
        mod.settings.max_connections,
        mod.settings.password.size() != 0 ? 1 : 0,
        mod.settings.port
    );

    masterlist_request = http_post(masterlist_url, buffer, zpl_strlen(buffer), NULL);
}

void masterlist_tick() {
    if (!masterlist_request) return;

    auto status = http_process(masterlist_request);

    if (status == HTTP_STATUS_FAILED) {
        mod_log("[error] couldn't post update to the masterlist (%d): %s.\n", masterlist_request->status_code, masterlist_request->reason_phrase);
        http_release(masterlist_request);
        masterlist_request = NULL;
        return;
    }

    if (status == HTTP_STATUS_COMPLETED) {
        http_release(masterlist_request);
        masterlist_request = NULL;
        return;
    }
}
