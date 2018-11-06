
#include <librg.h>
#include <m2o_types.h>

class C_Car;
class C_Entity;

namespace nmd::net 
{
    class NetVehicle : public m2o_car 
    {
        // clientside
        struct { /* interpolation table */
            struct {
                vec3 start;
                vec3 target;
                vec3 error;
                f32 lastAlpha;
                f64 startTime;
                f64 finishTime;
            } pos;

            struct {
                vec3 start;
                vec3 target;
                vec3 error;
                f32 lastAlpha;
                f64 startTime;
                f64 finishTime;
            } rot;

            u32 forceLocalZCounter;

        } interp;

        C_Car *CCar;
        C_Entity *CEntity;

    public:

        void UpdatePosition();
        void UpdateRotation();

        void SetPosition(vec3 targetPos, f32 interpTime, bool validVelocityZ, f32 velocityZ);
        void SetRotation(vec3 targetRot, f32 interpTime);
    };

} // namespace nmd::net
