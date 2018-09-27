// TODO: move to http://mafia2-online.com/masterlist/push
#define MASTERLIST_URLPTRN "http://139.59.142.46:8000/push?lgn=%s&key=%s"
#define MASTERLIST_PATTERN "{\"nm\":\"%s\",\"cp\":%d,\"mp\":%d,\"pp\":%d,\"sp\":\"%d\"}"
#define MASTERLIST_SECONDS 90.0

http_t* masterlist_request = NULL;

void masterlist_update(void *ptr) {
    if (masterlist_request) return;

    char url_buffer[512] = {0};
    zpl_snprintf(url_buffer, 512, MASTERLIST_URLPTRN,
        mod.settings.masterlist_login.c_str(),
        mod.settings.masterlist_secret.c_str()
    );

    char body_buffer[512] = {0};
    zpl_snprintf(body_buffer, 512, MASTERLIST_PATTERN,
        mod.settings.hostname.c_str(),
        mod.connected_amount,
        mod.settings.max_connections,
        mod.settings.password.size() != 0 ? 1 : 0,
        mod.settings.port
    );

    masterlist_request = http_post(url_buffer, body_buffer, zpl_strlen(body_buffer), NULL);
}

void masterlist_tick() {
    if (!masterlist_request) return;
    auto status = http_process(masterlist_request);

    if (status == HTTP_STATUS_FAILED) {
        // TODO: improve error message displaying
        mod_log("[error] couldn't post update to the masterlist (%d): %s", masterlist_request->status_code, masterlist_request->response_data);
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
