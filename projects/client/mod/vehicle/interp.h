// =======================================================================//
// !
// ! Position Interpolation
// !
// =======================================================================//

void m2o_car_target_position_update(m2o_car *car) {
    if (car->interp.pos.finishTime == 0) {
        return;
    }

    // Grab the current game position
    vec3 currentPosition = car->CEntity->GetPosition();

    // Get the factor of time spent from the interpolation start to the current time.
    f64 currentTime = zpl_time_now();
    f32 alpha = zplm_unlerp(currentTime, car->interp.pos.startTime, car->interp.pos.finishTime);

    // Don't let it overcompensate the error too much
    alpha = zplm_clamp(0.0f, alpha, 1.5f);

    // Get the current error portion to compensate
    f32 currentAlpha = alpha - car->interp.pos.lastAlpha;
    car->interp.pos.lastAlpha = alpha;

    // Apply the error compensation
    vec3 compensation;
    zplm_vec3_lerp(&compensation, zplm_vec3f_zero(), car->interp.pos.error, currentAlpha);

    // If we finished compensating the error, finish it for the next pulse
    if (alpha == 1.5f) {
        car->interp.pos.finishTime = 0;
    }

    vec3 newPosition = currentPosition + compensation;

    // Check if the distance to interpolate is too far.
    // vec3 velocity = car->CCar->m_pVehicle.m_vSpeed;
    // f32 threshold = (VEHICLE_INTERPOLATION_THRESHOLD * zplm_vec3_mag(velocity)) * M2O_TICKRATE_SERVER / 0.1f;

    // // There is a reason to have this condition this way: To prevent NaNs generating new NaNs after interpolating (Comparing with NaNs always results to false).
    // if (!(zplm_vec3_mag(currentPosition - car->interp.pos.target) <= threshold)) {
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

    car->CCar->SetPos(newPosition);
}

void m2o_car_target_position_set(m2o_car *car, vec3 targetPos, f32 interpTime, bool validVelocityZ, f32 velocityZ) {
    m2o_car_target_position_update(car);

    { /* UpdateUnderFloorFix */
        bool forceLocalZ = false;
        vec3 localPos    = car->CEntity->GetPosition();

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
            car->interp.forceLocalZCounter = 0;
        } else if (car->interp.forceLocalZCounter++ > 1) {
            localPos.z = targetPos.z;
            car->CCar->SetPos(localPos);
        }
    }

    vec3 localPos = car->CEntity->GetPosition();

    car->interp.pos.start  = localPos;
    car->interp.pos.target = targetPos;
    car->interp.pos.error  = targetPos - localPos; /* Calculate the relative error */

    // Apply the error over 400ms (i.e. 1/4 per 100ms )
    car->interp.pos.error *= zplm_lerp(0.4f, 1.0f, zplm_clamp01(zplm_unlerp(interpTime, 0.1f, 0.4f)));

    // Get the interpolation interval
    car->interp.pos.startTime  = zpl_time_now();
    car->interp.pos.finishTime = car->interp.pos.startTime + interpTime;
    car->interp.pos.lastAlpha  = 0.0f;
}

// =======================================================================//
// !
// ! Rotation Interpolation
// !
// =======================================================================//

void m2o_car_target_rotation_update(m2o_car *car) {
    if (car->interp.rot.finishTime == 0) {
        return;
    }

    // Grab the current game rotation (in degrees)
    vec3 rotation = zplm_vec3_to_degrees(
        zplm_quat_to_eular(car->CEntity->GetRotation())
    );

    // Get the factor of time spent from the interpolation start to the current time.
    f64 currentTime = zpl_time_now();
    f32 alpha = zplm_unlerp(currentTime, car->interp.rot.startTime, car->interp.rot.finishTime);

    // Don't let it to overcompensate the error
    alpha = zplm_clamp(0.0f, alpha, 1.0f);

    // Get the current error portion to compensate
    f32 currentAlpha = alpha - car->interp.rot.lastAlpha;
    car->interp.rot.lastAlpha = alpha;

    vec3 compensation;
    zplm_vec3_lerp(&compensation, zplm_vec3f_zero(), car->interp.rot.error, currentAlpha);

    // If we finished compensating the error, finish it for the next pulse
    if (alpha == 1.0f) {
        car->interp.rot.finishTime = 0;
    }

    car->CCar->SetRot(zplm_quat_from_eular(
        zplm_vec3_to_radians(rotation + compensation)
    ));
}

void m2o_car_target_rotation_set(m2o_car *car, vec3 targetRot, f32 interpTime) {
    m2o_car_target_rotation_update(car);

    vec3 rotation = zplm_vec3_to_degrees(
        zplm_quat_to_eular(car->CEntity->GetRotation())
    );

    car->interp.rot.start  = rotation;
    car->interp.rot.target = targetRot;
    // Get the error
    car->interp.rot.error.x = zplm_offset_degrees(rotation.x, targetRot.x);
    car->interp.rot.error.y = zplm_offset_degrees(rotation.y, targetRot.y);
    car->interp.rot.error.z = zplm_offset_degrees(rotation.z, targetRot.z);

    // Apply the error over 250ms (i.e. 2/5 per 100ms )
    car->interp.rot.error *= zplm_lerp(0.25f, 1.0f, zplm_clamp01(zplm_unlerp(interpTime, 0.1f, 0.4f)));

    // Get the interpolation interval
    car->interp.rot.startTime  = zpl_time_now();
    car->interp.rot.finishTime = car->interp.rot.startTime + interpTime;
    car->interp.rot.lastAlpha  = 0.0f;
}

///////
#if 0
    // mod_log(
    //     "------- Vehicle interpolation ------\n"
    //     "start: %f %f %f\n"
    //     "target: %f %f %f\n"
    //     "position: %f %f %f\n"
    //     "error: %f %f %f\n"
    //     "alpha: %f\n"
    //     "interp: %s\n",
    //     car->interp.pos.start.x, car->interp.pos.start.y, car->interp.pos.start.z,
    //     car->interp.pos.target.x, car->interp.pos.target.y, car->interp.pos.target.z,
    //     newPosition.x, newPosition.y, newPosition.z,
    //     car->interp.pos.error.x, car->interp.pos.error.y, car->interp.pos.error.z,
    //     alpha,
    //     (car->interp.pos.finishTime == 0 ? "no" : "yes")
    // );
#endif
