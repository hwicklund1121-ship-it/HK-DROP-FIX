// Client camera/UI only. Dedicated servers use their native implementations.
#ifndef SERVER
// Remove sprint's reduced eye-zoom level. Native zoom activation/release and
// FOV smoothing remain in control; no extra raw-button check is needed.
modded class DayZPlayerImplement
{
    override int GetEyeZoomLevel()
    {
        int zoom = super.GetEyeZoomLevel();
        if (!GetGame() || GetGame().IsDedicatedServer())
            return zoom;
        if (zoom != ECameraZoomType.SHALLOW || !IsAlive())
            return zoom;
        if (GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE)
            return zoom;
        HumanInputController controls = GetInputController();
        if (!controls || controls.WeaponADS() || m_CameraOptics || m_CameraIronsight)
            return zoom;
        HumanMovementState state = new HumanMovementState();
        GetMovementState(state);
        if (state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH)
            return ECameraZoomType.NORMAL;
        return zoom;
    }
}

#endif
