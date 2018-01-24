#pragma once

namespace M2
{
    namespace Wrappers
    {
        static const float DIRECTION_MULTIPLIER = 8192.0f;

        /**
         * Returns an aproximate position of the place local player is looking at
         */
        vec3_t GetLookAt()
        {
            vec3_t lookAt;
            auto camera = M2::C_GameCamera::Get()->GetCamera(1);
            if (!camera)
                return lookAt;

            auto vCamPos = camera->m_vecCamPos;
            auto vCamUp = camera->m_vecCamUp;
            auto vDir = (vCamUp - vCamPos);

            lookAt = (vCamPos + vDir * DIRECTION_MULTIPLIER);
            return lookAt;
        }
    };
};
