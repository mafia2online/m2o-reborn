void init_librg()
{
    // setup manual client mode
    librg::core_initialize(librg::mode_client_manual);

    librg::events::add(librg::events::on_log, [](librg::events::event_t* evt) {
        auto event = (librg::events::event_log_t*) evt;
        printf("%s", event->output);
        debug << event->output;
    });

    // setup callbacks
    librg::events::add(librg::events::on_tick, ontick);
    librg::events::add(librg::events::on_inter, entity_inter);
    librg::events::add(librg::events::on_create, entity_create);
    librg::events::add(librg::events::on_update, entity_update);
    librg::events::add(librg::events::on_remove, entity_remove);
    librg::events::add(librg::events::on_connect, client_connect);
    librg::events::add(librg::events::on_disconnect, client_disconnect);
    librg::events::add(librg::events::on_client_stream_entity, clientstream_update);

    auto cfg = librg::config_t{};
    cfg.ip = "inlife.no-ip.org";
    cfg.port = 27010;
    cfg.world_size = HMM_Vec3(5000.00, 5000.00, 5000.00);
    cfg.tick_delay = 64;
    cfg.max_connections = 8;
    cfg.platform_id = 1;
    cfg.proto_version = 1;
    cfg.build_version = 1;

    // start the client (network connection)
    librg::core::start(cfg);
    librg::network::start();

}
