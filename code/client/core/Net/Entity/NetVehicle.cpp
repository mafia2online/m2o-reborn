#include "NetVehicle.h"
#include "librg_ext.h"

namespace nmd::net
{
    void NetVehicle::UpdatePosition()
    {
        if (interp.pos.finishTime == 0)
        {
            return;
        }

        // Grab the current game position
        vec3 currentPosition; /*= car->CEntity->GetPosition();*/

        // Get the factor of time spent from the interpolation start to the current time.
        f64 currentTime = zpl_time_now();
        f32 alpha = zplm_unlerp(currentTime, interp.pos.startTime, interp.pos.finishTime);

        // Don't let it overcompensate the error too much
        alpha = zplm_clamp(0.0f, alpha, 1.5f);

        // Get the current error portion to compensate
        f32 currentAlpha = alpha - interp.pos.lastAlpha;
        interp.pos.lastAlpha = alpha;

        // Apply the error compensation
        vec3 compensation;
        zplm_vec3_lerp(&compensation, zplm_vec3f_zero(), interp.pos.error, currentAlpha);

        // If we finished compensating the error, finish it for the next pulse
        if (alpha == 1.5f) {
            interp.pos.finishTime = 0;
        }

        vec3 newPosition = currentPosition + compensation;

        // Check if the distance to interpolate is too far.
        // vec3 velocity = car->CCar->m_pVehicle.m_vSpeed;
        // f32 threshold = (VEHICLE_INTERPOLATION_THRESHOLD * zplm_vec3_mag(velocity)) * M2O_TICKRATE_SERVER / 0.1f;

        // // There is a reason to have this condition this way: To prevent NaNs generating new NaNs
        // after interpolating (Comparing with NaNs always results to false). if
        // (!(zplm_vec3_mag(currentPosition - car->interp.pos.target) <= threshold)) {
        //     // Abort all interpolation
        //     if (car->interp.rot.finishTime != 0) {
        //         car->CCar->SetRot(zplm_quat_from_eular(
        //             zplm_vec3_to_radians(car->interp.rot.target)
        //         ));
        //     }

        //     newPosition = car->interp.pos.target;
        //     car->interp.pos.finishTime = 0;
        //     car->interp.rot.finishTime = 0;
        // }

        //car->CCar->SetPos(newPosition);
    }

    void NetVehicle::UpdateRotation()
    {
        if (interp.rot.finishTime == 0) {
            return;
        }

        // Grab the current game rotation (in degrees)
        vec3 rotation; /*= zplm_vec3_to_degrees(zplm_quat_to_eular(car->CEntity->GetRotation()));*/

        // Get the factor of time spent from the interpolation start to the current time.
        f64 currentTime = zpl_time_now();
        f32 alpha = zplm_unlerp(currentTime, interp.rot.startTime, interp.rot.finishTime);

        // Don't let it to overcompensate the error
        alpha = zplm_clamp(0.0f, alpha, 1.0f);

        // Get the current error portion to compensate
        f32 currentAlpha = alpha - interp.rot.lastAlpha;
        interp.rot.lastAlpha = alpha;

        vec3 compensation;
        zplm_vec3_lerp(&compensation, zplm_vec3f_zero(), interp.rot.error, currentAlpha);

        // If we finished compensating the error, finish it for the next pulse
        if (alpha == 1.0f) {
            interp.rot.finishTime = 0;
        }

        //car->CCar->SetRot(zplm_quat_from_eular(zplm_vec3_to_radians(rotation + compensation)));
    }

    void NetVehicle::SetPosition(vec3 targetPos, f32 interpTime, bool validVelocityZ, f32 velocityZ)
    {
        UpdatePosition();

        #if 0
        { /* UpdateUnderFloorFix */
            bool forceLocalZ = false;
            vec3 localPos = car->CEntity->GetPosition();

            if (validVelocityZ) {
                // If remote z higher by too much and remote not doing any z movement, warp local z coord
                f32 deltaZ = targetPos.z - localPos.z;
                if (deltaZ > 0.4f && deltaZ < 10.0f) {
                    if (zplm_abs(velocityZ) < 0.01f) {
                        forceLocalZ = true;
                    }
                }
            }

            // Only force z coord if needed for at least two consecutive calls
            if (!forceLocalZ) {
                interp.forceLocalZCounter = 0;
            } else if (car->interp.forceLocalZCounter++ > 1) {
                localPos.z = targetPos.z;
             //   car->CCar->SetPos(localPos);
            }
        }
#endif

        vec3 localPos; /* = car->CEntity->GetPosition();*/

        interp.pos.start = localPos;
        interp.pos.target = targetPos;
        interp.pos.error = targetPos - localPos; /* Calculate the relative error */

        // Apply the error over 400ms (i.e. 1/4 per 100ms )
        interp.pos.error *= zplm_lerp(0.4f, 1.0f, zplm_clamp01(zplm_unlerp(interpTime, 0.1f, 0.4f)));

        // Get the interpolation interval
        interp.pos.startTime = zpl_time_now();
        interp.pos.finishTime = interp.pos.startTime + interpTime;
        interp.pos.lastAlpha = 0.0f;   
    }

    void NetVehicle::SetRotation(vec3 targetRot, f32 interpTime)
    {
        UpdateRotation();

        vec3 rotation; /*= zplm_vec3_to_degrees(zplm_quat_to_eular(car->CEntity->GetRotation()));*/

        interp.rot.start = rotation;
        interp.rot.target = targetRot;
        // Get the error
        interp.rot.error.x = zplm_offset_degrees(rotation.x, targetRot.x);
        interp.rot.error.y = zplm_offset_degrees(rotation.y, targetRot.y);
        interp.rot.error.z = zplm_offset_degrees(rotation.z, targetRot.z);

        // Apply the error over 250ms (i.e. 2/5 per 100ms )
        interp.pos.error *= zplm_lerp(0.25f, 1.0f, zplm_clamp01(zplm_unlerp(interpTime, 0.1f, 0.4f)));

        // Get the interpolation interval
        interp.rot.startTime = zpl_time_now();
        interp.rot.finishTime = interp.rot.startTime + interpTime;
        interp.rot.lastAlpha = 0.0f;
    }

} // namespace nmd::net
