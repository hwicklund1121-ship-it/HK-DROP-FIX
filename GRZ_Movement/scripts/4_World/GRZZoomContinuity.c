// Client camera/UI only. Dedicated servers use their native implementations.
#ifndef SERVER
// Carry the current eye-zoom FOV between ordinary movement cameras.
// Keep native target FOV and smoothing; do not hold camera distance or pivot.
modded class DayZPlayerCamera3rdPerson
{
    private bool GRZIsEyeZoomCamera(string camera)
    {
        return camera == "DayZPlayerCamera3rdPersonErc" || camera == "DayZPlayerCamera3rdPersonErcSpr" || camera == "DayZPlayerCamera3rdPersonErcRaised" || camera == "DayZPlayerCamera3rdPersonErcRaisedMelee" || camera == "DayZPlayerCamera3rdPersonCrouch" || camera == "DayZPlayerCamera3rdPersonCrouchRaised";
    }

    override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
    {
        if (!m_pPlayer || !m_pInput)
            return;
        super.OnActivate(pPrevCamera, pPrevCameraResult);
        if (!pPrevCamera || !pPrevCameraResult || !m_pPlayer || !m_pInput)
            return;
        if (!GRZIsEyeZoomCamera(ClassName()) || !GRZIsEyeZoomCamera(pPrevCamera.ClassName()))
            return;
        if (m_pInput.WeaponADS() || m_pPlayer.GetEyeZoomLevel() == ECameraZoomType.NONE)
            return;
        if (m_pPlayer.GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE)
            return;
        if (pPrevCameraResult.m_fFovAbsolute > 0)
        {
            m_fFovAbsolute = pPrevCameraResult.m_fFovAbsolute;
            m_fFovAbsVel[0] = 0;
        }
    }
}

#endif
