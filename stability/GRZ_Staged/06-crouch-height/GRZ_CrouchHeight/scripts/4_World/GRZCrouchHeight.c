#ifndef SERVER
modded class DayZPlayerCamera3rdPersonCrouch
{
    override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
    {
        if (!m_pPlayer || !m_pInput || !pOutResult)
            return;
        super.OnUpdate(pDt, pOutResult);

        // The native crouch camera adds m_fCameraHeight in model-space Y.
        // Subtract only that contribution, retaining stance height and lean.
        pOutResult.m_CameraTM[3][1] = pOutResult.m_CameraTM[3][1] - m_fCameraHeight;
    }
}

#endif
