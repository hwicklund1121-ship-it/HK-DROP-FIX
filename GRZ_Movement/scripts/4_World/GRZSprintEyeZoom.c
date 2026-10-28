// Remove sprint's reduced eye-zoom level. Native zoom activation/release and
// FOV smoothing remain in control; no extra raw-button check is needed.
modded class DayZPlayerImplement
{
    override int GetEyeZoomLevel()
    {
        int zoom = super.GetEyeZoomLevel();
        if (zoom != ECameraZoomType.SHALLOW || !IsAlive())
            return zoom;
        if (GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE)
            return zoom;
        HumanInputController controls = GetInputController();
        if (!controls || controls.WeaponADS() || m_CameraOptics || m_CameraIronsight)
            return zoom;
        HumanMovementState state = new HumanMovementState();
        GetMovementState(state);
        if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT)
            return ECameraZoomType.NORMAL;
        return zoom;
    }
}

// Sprint must not move the camera farther away than the standing camera.
modded class DayZPlayerCamera3rdPersonErcSpr
{
    void DayZPlayerCamera3rdPersonErcSpr(DayZPlayer pPlayer, HumanInputController pInput)
    {
        m_fDistance = 1.6;
    }
}
