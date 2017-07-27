void ontick(librg::events::event_t* evt)
{
    using namespace librg;

    auto event = (events::event_tick_t*) evt;

    // if (local_player.valid()) {
    //     auto gamedata   = local_player.component<gamedata_t>();
    //     auto playerdata = local_player.component<ped_t>();

    //     if (!gamedata || !playerdata) {
    //         return;
    //     }

    //     auto position   = ENTITY::GET_ENTITY_COORDS(gamedata->object, true);
    //     bool entering   = PED::IS_PED_GETTING_INTO_A_VEHICLE(gamedata->object);

    //     if (entering && !playerdata->entering_vehicle) {
    //         playerdata->entering_vehicle = true;
    //         auto vehicle_handle = PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(gamedata->object);

    //         entities->each<gamedata_t, server_owned_t, vehicle_t>(
    //             [&vehicle_handle](entity_t entity, gamedata_t data, server_owned_t owned, vehicle_t vehicle) {
    //                 if (data.object != vehicle_handle) {
    //                     return;
    //                 }

    //                 network::msg(GAME_PED_ENTER_VEHICLE, [&owned](network::bitstream_t* message) {
    //                     message->write_uint64(owned.guid);
    //                 });
    //             });
    //     }
    // }

    // if (KeyJustDown(VK_UP)) {
    //     auto position = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
    //     core::log("request veh spawn at: %f, %f, %f", position.x, position.y, position.z);
    //     network::msg(GAME_SPAWN_VEHICLE, nullptr);
    // }
}
