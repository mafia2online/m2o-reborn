
void car_target_position_set(const CVector& vecTargetPosition, unsigned long ulDelay, bool bValidVelocityZ, float fVelocityZ) {
    car_target_position_update();

    // UpdateUnderFloorFix(vecTargetPosition, bValidVelocityZ, fVelocityZ);
    {
        CVector vecLocalPosition;
        GetPosition(vecLocalPosition);

        bool bForceLocalZ = false;
        if (bValidVelocityZ && m_eVehicleType != CLIENTVEHICLE_HELI && m_eVehicleType != CLIENTVEHICLE_PLANE)
        {
            // If remote z higher by too much and remote not doing any z movement, warp local z coord
            float fDeltaZ = vecTargetPosition.fZ - vecLocalPosition.fZ;
            if (fDeltaZ > 0.4f && fDeltaZ < 10.0f)
            {
                if (fabsf(fVelocityZ) < 0.01f)
                {
                    bForceLocalZ = true;
                }
            }
        }

        // Only force z coord if needed for at least two consecutive calls
        if (!bForceLocalZ)
            m_uiForceLocalZCounter = 0;
        else if (m_uiForceLocalZCounter++ > 1)
        {
            vecLocalPosition.fZ = vecTargetPosition.fZ;
            SetPosition(vecLocalPosition);
        }
    }

    unsigned long ulTime = CClientTime::GetTime();
    CVector       vecLocalPosition;
    GetPosition(vecLocalPosition);

#ifdef MTA_DEBUG
    m_interp.pos.vecStart = vecLocalPosition;
#endif
    m_interp.pos.vecTarget = vecTargetPosition;
    // Calculate the relative error
    m_interp.pos.vecError = vecTargetPosition - vecLocalPosition;

    // Extrapolation
    const SVehExtrapolateSettings& vehExtrapolate = g_pClientGame->GetVehExtrapolateSettings();
    if (vehExtrapolate.bEnabled)
    {
        // Base amount to account for something
        int iExtrapolateMs = vehExtrapolate.iBaseMs;

        if (CClientPlayer* pPlayerDriver = DynamicCast<CClientPlayer>((CClientEntity*)m_pDriver))
            iExtrapolateMs += pPlayerDriver->GetLatency() * vehExtrapolate.iScalePercent / 110;

        // Limit amount
        iExtrapolateMs = Clamp(0, iExtrapolateMs, vehExtrapolate.iMaxMs);

        CVector vecVelocity;
        GetMoveSpeed(vecVelocity);
        vecVelocity *= 50.f * iExtrapolateMs * (1 / 1000.f);
        m_interp.pos.vecError += vecVelocity;
    }

    // Apply the error over 400ms (i.e. 1/4 per 100ms )
    m_interp.pos.vecError *= Lerp<const float>(0.25f, UnlerpClamped(100, ulDelay, 400), 1.0f);

    // Get the interpolation interval
    m_interp.pos.ulStartTime = ulTime;
    m_interp.pos.ulFinishTime = ulTime + ulDelay;

    // Initialize the interpolation
    m_interp.pos.fLastAlpha = 0.0f;
}

bool car_target_position_exists(void) {
    return (m_interp.pos.ulFinishTime != 0);
}

void car_target_position_remove(void) {
    m_interp.pos.ulFinishTime = 0;
}

void car_target_position_update(void) {
    if (HasTargetPosition()) {
        // Grab the current game position
        CVector vecCurrentPosition;
        GetPosition(vecCurrentPosition);

        // Get the factor of time spent from the interpolation start
        // to the current time.
        unsigned long ulCurrentTime = CClientTime::GetTime();
        float         fAlpha = SharedUtil::Unlerp(m_interp.pos.ulStartTime, ulCurrentTime, m_interp.pos.ulFinishTime);

        // Don't let it overcompensate the error too much
        fAlpha = SharedUtil::Clamp(0.0f, fAlpha, 1.5f);

        // Get the current error portion to compensate
        float fCurrentAlpha = fAlpha - m_interp.pos.fLastAlpha;
        m_interp.pos.fLastAlpha = fAlpha;

        // Apply the error compensation
        CVector vecCompensation = SharedUtil::Lerp(CVector(), fCurrentAlpha, m_interp.pos.vecError);

        // If we finished compensating the error, finish it for the next pulse
        if (fAlpha == 1.5f)
        {
            m_interp.pos.ulFinishTime = 0;
        }

        CVector vecNewPosition = vecCurrentPosition + vecCompensation;

        // Check if the distance to interpolate is too far.
        CVector vecVelocity;
        GetMoveSpeed(vecVelocity);
        float fThreshold = (VEHICLE_INTERPOLATION_WARP_THRESHOLD + VEHICLE_INTERPOLATION_WARP_THRESHOLD_FOR_SPEED * vecVelocity.Length()) *
                           g_pGame->GetGameSpeed() * TICK_RATE / 100;

        // There is a reason to have this condition this way: To prevent NaNs generating new NaNs after interpolating (Comparing with NaNs always results to
        // false).
        if (!((vecCurrentPosition - m_interp.pos.vecTarget).Length() <= fThreshold))
        {
            // Abort all interpolation
            m_interp.pos.ulFinishTime = 0;
            vecNewPosition = m_interp.pos.vecTarget;

            if (HasTargetRotation())
                SetRotationDegrees(m_interp.rot.vecTarget);
            m_interp.rot.ulFinishTime = 0;
        }

        SetPosition(vecNewPosition, false);

        if (!m_bIsCollisionEnabled)
        {
            if (m_eVehicleType != CLIENTVEHICLE_HELI && m_eVehicleType != CLIENTVEHICLE_BOAT)
            {
                // Ghostmode upwards movement compensation
                CVector MoveSpeed;
                m_pVehicle->GetMoveSpeed(&MoveSpeed);
                float SpeedXY = CVector(MoveSpeed.fX, MoveSpeed.fY, 0).Length();
                if (MoveSpeed.fZ > 0.00 && MoveSpeed.fZ < 0.02 && MoveSpeed.fZ > SpeedXY)
                    MoveSpeed.fZ = SpeedXY;
                m_pVehicle->SetMoveSpeed(&MoveSpeed);
            }
        }

#ifdef MTA_DEBUG
        if (g_pClientGame->IsShowingInterpolation() && g_pClientGame->GetLocalPlayer()->GetOccupiedVehicle() == this)
        {
            // DEBUG
            SString strBuffer(
                "-== Vehicle interpolation ==-\n"
                "vecStart: %f %f %f\n"
                "vecTarget: %f %f %f\n"
                "Position: %f %f %f\n"
                "Error: %f %f %f\n"
                "Alpha: %f\n"
                "Interpolating: %s\n",
                m_interp.pos.vecStart.fX, m_interp.pos.vecStart.fY, m_interp.pos.vecStart.fZ, m_interp.pos.vecTarget.fX, m_interp.pos.vecTarget.fY,
                m_interp.pos.vecTarget.fZ, vecNewPosition.fX, vecNewPosition.fY, vecNewPosition.fZ, m_interp.pos.vecError.fX, m_interp.pos.vecError.fY,
                m_interp.pos.vecError.fZ, fAlpha, (m_interp.pos.ulFinishTime == 0 ? "no" : "yes"));
            g_pClientGame->GetManager()->GetDisplayManager()->DrawText2D(strBuffer, CVector(0.45f, 0.05f, 0), 1.0f, 0xFFFFFFFF);
        }
#endif

        // Update our contact players
        CVector vecPlayerPosition;
        CVector vecOffset;
        for (uint i = 0; i < m_Contacts.size(); i++)
        {
            CClientPed* pModel = m_Contacts[i];
            pModel->GetPosition(vecPlayerPosition);
            vecOffset = vecPlayerPosition - vecCurrentPosition;
            vecPlayerPosition = vecNewPosition + vecOffset;
            pModel->SetPosition(vecPlayerPosition);
        }
    }
}

void car_target_position_set(const CVector& vecTargetPosition, unsigned long ulDelay, bool bValidVelocityZ, float fVelocityZ) {
    car_target_position_update();

    // UpdateUnderFloorFix(vecTargetPosition, bValidVelocityZ, fVelocityZ);
    {
        CVector vecLocalPosition;
        GetPosition(vecLocalPosition);

        bool bForceLocalZ = false;
        if (bValidVelocityZ && m_eVehicleType != CLIENTVEHICLE_HELI && m_eVehicleType != CLIENTVEHICLE_PLANE)
        {
            // If remote z higher by too much and remote not doing any z movement, warp local z coord
            float fDeltaZ = vecTargetPosition.fZ - vecLocalPosition.fZ;
            if (fDeltaZ > 0.4f && fDeltaZ < 10.0f)
            {
                if (fabsf(fVelocityZ) < 0.01f)
                {
                    bForceLocalZ = true;
                }
            }
        }

        // Only force z coord if needed for at least two consecutive calls
        if (!bForceLocalZ)
            m_uiForceLocalZCounter = 0;
        else if (m_uiForceLocalZCounter++ > 1)
        {
            vecLocalPosition.fZ = vecTargetPosition.fZ;
            SetPosition(vecLocalPosition);
        }
    }

    unsigned long ulTime = CClientTime::GetTime();
    CVector       vecLocalPosition;
    GetPosition(vecLocalPosition);

#ifdef MTA_DEBUG
    m_interp.pos.vecStart = vecLocalPosition;
#endif
    m_interp.pos.vecTarget = vecTargetPosition;
    // Calculate the relative error
    m_interp.pos.vecError = vecTargetPosition - vecLocalPosition;

    // Extrapolation
    const SVehExtrapolateSettings& vehExtrapolate = g_pClientGame->GetVehExtrapolateSettings();
    if (vehExtrapolate.bEnabled)
    {
        // Base amount to account for something
        int iExtrapolateMs = vehExtrapolate.iBaseMs;

        if (CClientPlayer* pPlayerDriver = DynamicCast<CClientPlayer>((CClientEntity*)m_pDriver))
            iExtrapolateMs += pPlayerDriver->GetLatency() * vehExtrapolate.iScalePercent / 110;

        // Limit amount
        iExtrapolateMs = Clamp(0, iExtrapolateMs, vehExtrapolate.iMaxMs);

        CVector vecVelocity;
        GetMoveSpeed(vecVelocity);
        vecVelocity *= 50.f * iExtrapolateMs * (1 / 1000.f);
        m_interp.pos.vecError += vecVelocity;
    }

    // Apply the error over 400ms (i.e. 1/4 per 100ms )
    m_interp.pos.vecError *= Lerp<const float>(0.25f, UnlerpClamped(100, ulDelay, 400), 1.0f);

    // Get the interpolation interval
    m_interp.pos.ulStartTime = ulTime;
    m_interp.pos.ulFinishTime = ulTime + ulDelay;

    // Initialize the interpolation
    m_interp.pos.fLastAlpha = 0.0f;
}

bool car_target_position_exists(void) {
    return (m_interp.pos.ulFinishTime != 0);
}

void car_target_position_remove(void) {
    m_interp.pos.ulFinishTime = 0;
}

void car_target_position_update(void) {
    if (HasTargetPosition()) {
        // Grab the current game position
        CVector vecCurrentPosition;
        GetPosition(vecCurrentPosition);

        // Get the factor of time spent from the interpolation start
        // to the current time.
        unsigned long ulCurrentTime = CClientTime::GetTime();
        float         fAlpha = SharedUtil::Unlerp(m_interp.pos.ulStartTime, ulCurrentTime, m_interp.pos.ulFinishTime);

        // Don't let it overcompensate the error too much
        fAlpha = SharedUtil::Clamp(0.0f, fAlpha, 1.5f);

        // Get the current error portion to compensate
        float fCurrentAlpha = fAlpha - m_interp.pos.fLastAlpha;
        m_interp.pos.fLastAlpha = fAlpha;

        // Apply the error compensation
        CVector vecCompensation = SharedUtil::Lerp(CVector(), fCurrentAlpha, m_interp.pos.vecError);

        // If we finished compensating the error, finish it for the next pulse
        if (fAlpha == 1.5f)
        {
            m_interp.pos.ulFinishTime = 0;
        }

        CVector vecNewPosition = vecCurrentPosition + vecCompensation;

        // Check if the distance to interpolate is too far.
        CVector vecVelocity;
        GetMoveSpeed(vecVelocity);
        float fThreshold = (VEHICLE_INTERPOLATION_WARP_THRESHOLD + VEHICLE_INTERPOLATION_WARP_THRESHOLD_FOR_SPEED * vecVelocity.Length()) *
                           g_pGame->GetGameSpeed() * TICK_RATE / 100;

        // There is a reason to have this condition this way: To prevent NaNs generating new NaNs after interpolating (Comparing with NaNs always results to
        // false).
        if (!((vecCurrentPosition - m_interp.pos.vecTarget).Length() <= fThreshold))
        {
            // Abort all interpolation
            m_interp.pos.ulFinishTime = 0;
            vecNewPosition = m_interp.pos.vecTarget;

            if (HasTargetRotation())
                SetRotationDegrees(m_interp.rot.vecTarget);
            m_interp.rot.ulFinishTime = 0;
        }

        SetPosition(vecNewPosition, false);

        if (!m_bIsCollisionEnabled)
        {
            if (m_eVehicleType != CLIENTVEHICLE_HELI && m_eVehicleType != CLIENTVEHICLE_BOAT)
            {
                // Ghostmode upwards movement compensation
                CVector MoveSpeed;
                m_pVehicle->GetMoveSpeed(&MoveSpeed);
                float SpeedXY = CVector(MoveSpeed.fX, MoveSpeed.fY, 0).Length();
                if (MoveSpeed.fZ > 0.00 && MoveSpeed.fZ < 0.02 && MoveSpeed.fZ > SpeedXY)
                    MoveSpeed.fZ = SpeedXY;
                m_pVehicle->SetMoveSpeed(&MoveSpeed);
            }
        }

#ifdef MTA_DEBUG
        if (g_pClientGame->IsShowingInterpolation() && g_pClientGame->GetLocalPlayer()->GetOccupiedVehicle() == this)
        {
            // DEBUG
            SString strBuffer(
                "-== Vehicle interpolation ==-\n"
                "vecStart: %f %f %f\n"
                "vecTarget: %f %f %f\n"
                "Position: %f %f %f\n"
                "Error: %f %f %f\n"
                "Alpha: %f\n"
                "Interpolating: %s\n",
                m_interp.pos.vecStart.fX, m_interp.pos.vecStart.fY, m_interp.pos.vecStart.fZ, m_interp.pos.vecTarget.fX, m_interp.pos.vecTarget.fY,
                m_interp.pos.vecTarget.fZ, vecNewPosition.fX, vecNewPosition.fY, vecNewPosition.fZ, m_interp.pos.vecError.fX, m_interp.pos.vecError.fY,
                m_interp.pos.vecError.fZ, fAlpha, (m_interp.pos.ulFinishTime == 0 ? "no" : "yes"));
            g_pClientGame->GetManager()->GetDisplayManager()->DrawText2D(strBuffer, CVector(0.45f, 0.05f, 0), 1.0f, 0xFFFFFFFF);
        }
#endif

        // Update our contact players
        CVector vecPlayerPosition;
        CVector vecOffset;
        for (uint i = 0; i < m_Contacts.size(); i++)
        {
            CClientPed* pModel = m_Contacts[i];
            pModel->GetPosition(vecPlayerPosition);
            vecOffset = vecPlayerPosition - vecCurrentPosition;
            vecPlayerPosition = vecNewPosition + vecOffset;
            pModel->SetPosition(vecPlayerPosition);
        }
    }
}
