/* output */
void api_printf();

/* datetime */
void api_time();
void api_date();
void api_datetime();
void api_tickcount();

/* information */
void api_getPlayerCount();
void api_getMaxPlayers();
void api_getServerName();

// /* vehicle */
// u32         api_vehicle_create();
// void        api_vehicle_destroy(u32 vehicleid);
// void        api_vehicle_position_set(u32 vehicleid, vector3 position);
// vector3     api_vehicle_position_get(u32 vehicleid);
// void        api_vehicle_rotation_set(u32 vehicleid, quaternion position);
// quaternion  api_vehicle_rotation_get(u32 vehicleid);

/* vehicle */
u32         api_ped_create();
void        api_ped_destroy(u32 pedid);
void        api_ped_position_set(u32 pedid, vector3 position);
vector3     api_ped_position_get(u32 pedid);
void        api_ped_rotation_set(u32 pedid, quaternion position);
quaternion  api_ped_rotation_get(u32 pedid);


#define M2O_API_VEHICLE_CREATE(name)  u32  name()
#define M2O_API_VEHICLE_DESTROY(name) void name(u32 vehicleid)

typedef M2O_API_VEHICLE_CREATE(api_vehicle_create);
typedef M2O_API_VEHICLE_DESTROY(api_vehicle_destroy);

#define M2O_API_NATIVE_FUNC(name) api_##name *name

typedef void (api_vehicle_create)(u32 vehicleid);

struct m2o_plugin_vtable {
    api_vehicle_create *vehicle_create;

    M2O_API_NATIVE_FUNC(vehicle_create);
    M2O_API_NATIVE_FUNC(vehicle_destroy);
}

void m2o_api_init() {
    m2o_plugin_vtable api;

    api.vehicle_create = mod_vehicle_create;
}


//////// usage part
#define m2o_export

m2o_export mod_vehicle_create() {
    // create a vehicle
    u32 vehicleid = 0;

    if (mod.plugin1.events.on_vehicle_create)
        mod.plugin1.events.on_vehicle_create(vehicleid);

    return vehicleid;
}

s
// for generator
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);
m2o_import void on_vehicle_create(u32 vehicleid);





// plugin part
void on_vehicle_create(u32 vehicleid) {
    printf("we spawned a vehicle\n");
}

void m2o_plugin_init(m2o_plugin_ctx *ctx, m2o_plugin_vtable *methods) {
    ctx->events.on_vehicle_create = on_vehicle_create;
}










#define M2O_API_EVENT_ON_PLAYER_SPAWN(name) void m2o_##name(u32 player_id)
typedef M2O_API_EVENT_ON_PLAYER_SPAWN(event_on_player_spawn_proc);








#define M2O_API_EVENT_ON_PLAYER_SPAWN(name) void m2o_##name(u32 player_id)
typedef M2O_API_EVENT_ON_PLAYER_SPAWN(event_on_player_spawn_proc);

plugin side:

M2O_API_EVENT_ON_PLAYER_SPAWN(my_on_player_spawn_func) {
  // do stuff
}


server api

typedef struct {
     m2o_event_on_player_spawn_proc *event_on_player_spawn,
// ....
} m2o_event_vtable;
plugin api

events->event_on_player_spawn = my_on_player_spawn_func;
