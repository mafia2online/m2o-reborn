M2O_SERVER_API bool m2o_player_kick(u32 playerid) {
    M2O_FETCH_ENTITY(entity, playerid, false);

    if (!entity->client_peer || !(entity->flags & LIBRG_ENTITY_CLIENT)) {
        m2o_error_set(M2O_UNDEFINED_ENTITY);
        return false;
    }

    librg_network_kick(ctx, entity->client_peer);
    return true;
}
