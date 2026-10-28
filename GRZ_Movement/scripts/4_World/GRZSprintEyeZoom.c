// Retain full eye zoom during sprint when the player holds the zoom input.
// The native handler uses SHALLOW zoom for sprint; other camera behavior stays
// native, including FOV smoothing, distance, scopes and stance transitions.
modded class DayZPlayerImplement
{
    override int GetEyeZoomLevel()
    {
        int nativeZoom = super.GetEyeZoomLevel();
        if (nativeZoom != ECameraZoomType.SHALLOW)
            return nativeZoom;
        if (!IsAlive() || GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE)
            return nativeZoom;

        HumanInputController controls = GetInputController();
        if (!controls || !controls.IsZoom() || controls.WeaponADS() || m_CameraOptics || m_CameraIronsight)
            return nativeZoom;

        HumanMovementState state = new HumanMovementState();
        GetMovementState(state);
        if (state.m_iStanceIdx != DayZPlayerConstants.STANCEIDX_ERECT)
            return nativeZoom;

        float requestedSpeed;
        vector requestedDirection;
        controls.GetMovement(requestedSpeed, requestedDirection);
        if (requestedSpeed >= 3)
            return ECameraZoomType.NORMAL;
        return nativeZoom;
    }
}
