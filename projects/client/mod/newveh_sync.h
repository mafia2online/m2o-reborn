// TODO: finish rewriting and attach to current veh code

#define VEHICLE_THRESHOLD_FOR_SPEED 0.6f
#define VEHICLE_INTERPOLATION_THRESHOLD 15

zplm_vec3 zplm_vec3_to_radians(zplm_vec3 value) {
    zplm_vec3 result = {0};
    result.x = zplm_to_radians(value.x);
    result.y = zplm_to_radians(value.y);
    result.z = zplm_to_radians(value.z);
    return result;
}

zplm_vec3 zplm_vec3_to_degrees(zplm_vec3 value) {
    zplm_vec3 result = {0};
    result.x = zplm_to_degrees(value.x);
    result.y = zplm_to_degrees(value.y);
    result.z = zplm_to_degrees(value.z);
    return result;
}

zplm_quat zplm_quat_from_eular(zplm_vec3 radians) {
    f32 cosZ = zplm_cos(0.5f * radians.z);
    f32 cosY = zplm_cos(0.5f * radians.y);
    f32 cosX = zplm_cos(0.5f * radians.x);

    // Calculate the sin angles
    f32 sinZ = zplm_sin(0.5f * radians.z);
    f32 sinY = zplm_sin(0.5f * radians.y);
    f32 sinX = zplm_sin(0.5f * radians.x);

    zplm_quat result = {0};

    // Compute the quaternion
    result.w = cosZ * cosY * cosX + sinZ * sinY * sinX;
    result.z = cosZ * cosY * sinX - sinZ * sinY * cosX;
    result.y = cosZ * sinY * cosX + sinZ * cosY * sinX;
    result.x = sinZ * cosY * cosX - cosZ * sinY * sinX;

    return result;
}

zplm_vec3 zplm_quat_to_eular(zplm_quat input) {
    f32 sqw = (input.w * input.w);
    f32 sqx = (input.x * input.x);
    f32 sqy = (input.y * input.y);
    f32 sqz = (input.z * input.z);

    zplm_vec3 result = {0};
    result.x = zplm_arctan2(2.f * (input.x * input.y + input.z * input.w), sqx - sqy - sqz + sqw);
    result.y = zplm_arcsin(-2.f * (input.x * input.z - input.y * input.w));
    result.z = zplm_arctan2(2.f * (input.y * input.z + input.x * input.w), -sqx - sqy + sqz + sqw);
    return result;
}

f32 zplm_offset_degrees(f32 a, f32 b) {
    f32 c = (b > a) ? b - a : 0.0f - (a - b);
    if (c > 180.0f) { c = 0.0f - (360.0f - c); }
    else if (c <= -180.0f) { c = (360.0f + c); }
    return c;
}

bool vehicle_HasTargetPosition(m2o_car *car) {
    return (car->interp.pos.finishTime != 0);
}

bool vehicle_HasTargetRotation(m2o_car *car) {
    return (car->interp.rot.finishTime != 0);
}

void vehicle_RemoveTargetPosition(m2o_car *car) {
    car->interp.pos.finishTime = 0;
}

void vehicle_RemoveTargetRotation(m2o_car *car) {
    car->interp.rot.finishTime = 0;
}


void vehicle_UpdateTargetPosition(m2o_car *car) {
    if (!vehicle_HasTargetPosition(car)) {
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
    //     if (vehicle_HasTargetRotation(car)) {
    //         car->CCar->SetRot(zplm_quat_from_eular(
    //             zplm_vec3_to_radians(car->interp.rot.target)
    //         ));
    //     }

    //     newPosition = car->interp.pos.target;
    //     car->interp.pos.finishTime = 0;
    //     car->interp.rot.finishTime = 0;
    // }

    car->CCar->SetPos(newPosition);

#ifdef M2O_DEBUG
        mod_log(
            "------- Vehicle interpolation ------\n"
            "start: %f %f %f\n"
            "target: %f %f %f\n"
            "position: %f %f %f\n"
            "error: %f %f %f\n"
            "alpha: %f\n"
            "interp: %s\n",
            car->interp.pos.start.x, car->interp.pos.start.y, car->interp.pos.start.z,
            car->interp.pos.target.x, car->interp.pos.target.y, car->interp.pos.target.z,
            newPosition.x, newPosition.y, newPosition.z,
            car->interp.pos.error.x, car->interp.pos.error.y, car->interp.pos.error.z,
            alpha,
            (car->interp.pos.finishTime == 0 ? "no" : "yes")
        );
#endif

    // TODO: add support for on-vehicle passangers
    // // Update our contact players
    // CVector vecPlayerPosition;
    // CVector vecOffset;
    // for (uint i = 0; i < m_Contacts.size(); i++)
    // {
    //     CClientPed* pModel = m_Contacts[i];
    //     pModel->GetPosition(vecPlayerPosition);
    //     vecOffset = vecPlayerPosition - vecCurrentPosition;
    //     vecPlayerPosition = newPosition + vecOffset;
    //     pModel->SetPosition(vecPlayerPosition);
    // }
}

void vehicle_UpdateTargetRotation(m2o_car *car) {
    if (!vehicle_HasTargetRotation(car)) {
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


void vehicle_SetTargetPosition(m2o_car *car, vec3 targetPos, f32 interpTime, bool validVelocityZ, f32 velocityZ) {
    vehicle_UpdateTargetPosition(car);

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

#ifdef M2O_DEBUG
    car->interp.pos.start  = localPos;
#endif
    car->interp.pos.target = targetPos;
    car->interp.pos.error  = targetPos - localPos; /* Calculate the relative error */

    // Extrapolation
    // const SVehExtrapolateSettings& vehExtrapolate = g_pClientGame->GetVehExtrapolateSettings();
    // if (true/*vehExtrapolate.bEnabled*/) {
    //     // Base amount to account for something
    //     int iExtrapolateMs = 100;//vehExtrapolate.iBaseMs;

    //     // if (CClientPlayer* pPlayerDriver = DynamicCast<CClientPlayer>((CClientEntity*)m_pDriver))
    //     //     iExtrapolateMs += pPlayerDriver->GetLatency() * vehExtrapolate.iScalePercent / 110;

    //     // // Limit amount
    //     // iExtrapolateMs = Clamp(0, iExtrapolateMs, vehExtrapolate.iMaxMs);

    //     vec3 velocity = car->CCar->m_pVehicle.m_vSpeed;
    //     velocity *= 50.f * iExtrapolateMs * (1 / 1000.f);
    //     car->interp.pos.error += velocity;
    // }

    // Apply the error over 400ms (i.e. 1/4 per 100ms )
    car->interp.pos.error *= zplm_lerp(0.4f, 1.0f, zplm_clamp01(zplm_unlerp(interpTime, 0.1f, 0.4f)));

    // Get the interpolation interval
    car->interp.pos.startTime  = zpl_time_now();
    car->interp.pos.finishTime = car->interp.pos.startTime + interpTime;

    // Initialize the interpolation
    car->interp.pos.lastAlpha = 0.0f;
}

void vehicle_SetTargetRotation(m2o_car *car, vec3 targetRot, f32 interpTime) {
    vehicle_UpdateTargetRotation(car);

    vec3 rotation = zplm_vec3_to_degrees(
        zplm_quat_to_eular(car->CEntity->GetRotation())
    );

#ifdef M2O_DEBUG
    car->interp.rot.start  = rotation;
#endif
    car->interp.rot.target = targetRot;
    // Get the error
    car->interp.rot.error.x = zplm_offset_degrees(rotation.x, targetRot.x);
    car->interp.rot.error.y = zplm_offset_degrees(rotation.y, targetRot.y);
    car->interp.rot.error.z = zplm_offset_degrees(rotation.z, targetRot.z);

    // Apply the error over 250ms (i.e. 2/5 per 100ms )
    car->interp.pos.error *= zplm_lerp(0.25f, 1.0f, zplm_clamp01(zplm_unlerp(interpTime, 0.1f, 0.4f)));

    // Get the interpolation interval
    car->interp.rot.startTime  = zpl_time_now();
    car->interp.rot.finishTime = car->interp.rot.startTime + interpTime;

    // Initialize the interpolation
    car->interp.rot.lastAlpha = 0.0f;
}

void vehicle_ResetInterpolation(m2o_car *car) {
    if (vehicle_HasTargetPosition(car)){
        car->CCar->SetPos(car->interp.pos.target);
    }
    if (vehicle_HasTargetRotation(car)) {
        car->CCar->SetRot(zplm_quat_from_eular(
            zplm_vec3_to_radians(car->interp.rot.target)
        ));
    }

    car->interp.pos.finishTime = 0;
    car->interp.rot.finishTime = 0;
}

void vehicle_Interpolate(librg_entity_t *entity, m2o_car *car) {
    auto player_ped = m2o_ped_get(mod.player);

    if (!player_ped || !player_ped->gameptr) {
        mod_log("[warning] calling interpolation w/o player ped");
        return;
    }

    // if (player_ped->vehicle != entity->id) {
        vehicle_UpdateTargetPosition(car);
        vehicle_UpdateTargetRotation(car);
    // }
    // else {
    //     // Otherwize make sure we have no interpolation stuff stored
    //     vehicle_RemoveTargetPosition(car);
    //     vehicle_RemoveTargetRotation(car);
    // }
}
