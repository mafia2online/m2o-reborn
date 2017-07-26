// Copyright ReGuider Team, 2016-2017
//
#include <librg/network.h>
#include <librg/events.h>
#include <librg/components/interpolable.h>
#include <librg/components/streamable.h>

void librg::entities_interpolate(double dt)
{
    entities->each<interpolable_t, transform_t, streamable_t>([dt](entity_t entity, interpolable_t& inter, transform_t& trans, streamable_t& stream) {
        inter.time += dt;
        auto t = HMM_Clamp(0.0f, (float)(inter.time / core::config.tick_delay), 1.01f);

        // dont run additional interpolate ticks
        // to reduce performance overload
        if (t > 1.0f) {
            return;
        }

        auto lpos = inter.latest.position;
        auto tpos = inter.target.position;

        auto posx = HMM_Lerp(lpos.X, t, tpos.X);
        auto posy = HMM_Lerp(lpos.Y, t, tpos.Y);
        auto posz = HMM_Lerp(lpos.Z, t, tpos.Z);
        auto npos = hmm_vec3{ posx, posy, posz };

        auto v1   = inter.latest.rotation;
        auto v2   = inter.target.rotation;
        auto lrot = HMM_Quaternion(v1.X, v1.Y, v1.Z, v1.W);
        auto trot = HMM_Quaternion(v2.X, v2.Y, v2.Z, v2.W);
        hmm_quaternion nrot;

        if (HMM_CompareQuaternion(lrot, trot)) {
            nrot = trot;
        }
        else {
            nrot = HMM_Slerp(lrot, t, trot);
        }

        // TODO: and scale

        // ...

        auto new_transform = transform_t(npos, HMM_Vec4(nrot.X, nrot.Y, nrot.Z, nrot.W), trans.scale);
        events::trigger(events::on_inter, new events::event_entity_t(
            (void *)&new_transform, entity, entity.id().id(), stream.type
        ));
    });
}
