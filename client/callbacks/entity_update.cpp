#include "../client.h"

const float DISTANCE_BEFORE_TELEPORT = 50 * 50;

/**
* Entity update in streamer
*/
void entity_update(librg::events::event_t* evt)
{
    auto event = (librg::events::event_bs_entity_t*) evt;

    auto transform  = event->entity.component<librg::transform_t>();
    auto inter      = event->entity.component<librg::interpolable_t>();
    auto remote     = event->entity.component<gamedata_t>();
    
    auto distance   = HMM_LengthSquaredVec3(HMM_SubtractVec3(inter->latest.position, transform->position));

    // if distance further, override interpolation
    if (distance > DISTANCE_BEFORE_TELEPORT) {
        inter->latest = *transform;

        ENTITY::SET_ENTITY_COORDS_NO_OFFSET(remote->object, transform->position.X, transform->position.Y, transform->position.Z, true, true, true);
        ENTITY::SET_ENTITY_QUATERNION(remote->object, transform->rotation.X, transform->rotation.Y, transform->rotation.Z, transform->rotation.W);
    }

    switch (event->type) {
        case TYPE_PLAYER: {
            auto speed = event->data->read_float();

            if (!STREAMING::HAS_ANIM_DICT_LOADED("move_m@generic"))
                STREAMING::REQUEST_ANIM_DICT("move_m@generic");

            if (speed < 2.0f && speed > 1.0f && remote->state != 1)
            {
                PED::FORCE_PED_MOTION_STATE(remote->object, GAMEPLAY::GET_HASH_KEY("motionstate_walk"), false, false, false);
                AI::TASK_PLAY_ANIM(remote->object, "move_m@generic", "walk", 8.0f, 0.0f, -1, 1, 0.0f, false, false, false);
                remote->state = 1;
            }
            else if (speed > 2.0f && speed <= 5.2f && remote->state != 2)
            {
                PED::FORCE_PED_MOTION_STATE(remote->object, GAMEPLAY::GET_HASH_KEY("motionstate_run"), false, false, false);
                AI::TASK_PLAY_ANIM(remote->object, "move_m@generic", "run", 8.0f, 0.0f, -1, 1, 0.0f, false, false, false);
                remote->state = 2;
            }
            else if (speed > 5.2f && remote->state != 3)
            {
                PED::FORCE_PED_MOTION_STATE(remote->object, -1115154469, false, false, false);
                AI::TASK_PLAY_ANIM(remote->object, "move_m@generic", "sprint", 8.0f, 0.0f, -1, 1, 0.0f, false, false, false);
                remote->state = 3;
            }
            else if (speed < 1.0f && remote->state != 0)
            {
                PED::FORCE_PED_MOTION_STATE(remote->object, GAMEPLAY::GET_HASH_KEY("motionstate_idle"), false, false, false);
                AI::TASK_PLAY_ANIM(remote->object, "move_m@generic", "idle", 8.0f, 0.0f, -1, 1, 0.0f, false, false, false);
                remote->state = 0;
            }
        }
        break;
    }
}
