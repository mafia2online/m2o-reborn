void m2o_set_last_error(unsigned int error);
unsigned int m2o_get_last_error();

unsigned int m2o_vehicle_create();
bool m2o_vehicle_destroy(unsigned int vehicleid);
bool m2o_vehicle_position_set(unsigned int vehicleid, vec3_t position);
vec3_t m2o_vehicle_position_get(unsigned int vehicleid);

unsigned int m2o_ped_create();
bool m2o_ped_destroy(unsigned int pedid);
bool m2o_ped_position_set(unsigned int pedid, vec3_t position);
vec3_t m2o_ped_position_get(unsigned int pedid);

typedef void (m2o_api_set_last_error)(unsigned int error);
typedef unsigned int (m2o_api_get_last_error)();
typedef unsigned int (m2o_api_vehicle_create)();
typedef bool (m2o_api_vehicle_destroy)(unsigned int vehicleid);
typedef bool (m2o_api_vehicle_position_set)(unsigned int vehicleid, vec3_t position);
typedef vec3_t (m2o_api_vehicle_position_get)(unsigned int vehicleid);
typedef unsigned int (m2o_api_ped_create)();
typedef bool (m2o_api_ped_destroy)(unsigned int pedid);
typedef bool (m2o_api_ped_position_set)(unsigned int pedid, vec3_t position);
typedef vec3_t (m2o_api_ped_position_get)(unsigned int pedid);

typedef struct m2o_api_vtable {
    m2o_api_set_last_error *set_last_error;
    m2o_api_get_last_error *get_last_error;
    m2o_api_vehicle_create *vehicle_create;
    m2o_api_vehicle_destroy *vehicle_destroy;
    m2o_api_vehicle_position_set *vehicle_position_set;
    m2o_api_vehicle_position_get *vehicle_position_get;
    m2o_api_ped_create *ped_create;
    m2o_api_ped_destroy *ped_destroy;
    m2o_api_ped_position_set *ped_position_set;
    m2o_api_ped_position_get *ped_position_get;
} m2o_api_vtable;

void m2o_api_init(m2o_api_vtable *api) {
    api->set_last_error = m2o_set_last_error;
    api->get_last_error = m2o_get_last_error;
    api->vehicle_create = m2o_vehicle_create;
    api->vehicle_destroy = m2o_vehicle_destroy;
    api->vehicle_position_set = m2o_vehicle_position_set;
    api->vehicle_position_get = m2o_vehicle_position_get;
    api->ped_create = m2o_ped_create;
    api->ped_destroy = m2o_ped_destroy;
    api->ped_position_set = m2o_ped_position_set;
    api->ped_position_get = m2o_ped_position_get;
}