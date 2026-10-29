// Eye-zoom policy only: no camera transforms, distance, selection or animation writes.
#ifndef SERVER
modded class DayZPlayerImplement
{
    override int GetEyeZoomLevel()
    {
        int nativeZoom = super.GetEyeZoomLevel();
        if (!GetGame() || GetGame().IsDedicatedServer() || !IsAlive())
            return nativeZoom;
        if (GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE)
            return nativeZoom;

        HumanInputController controls = GetInputController();
        if (!controls || controls.WeaponADS() || m_CameraOptics || m_CameraIronsight || IsRaised())
            return nativeZoom;

        HumanMovementState state = new HumanMovementState();
        GetMovementState(state);
        if (state.m_iStanceIdx != DayZPlayerConstants.STANCEIDX_ERECT && state.m_iStanceIdx != DayZPlayerConstants.STANCEIDX_CROUCH)
            return nativeZoom;

        // Use the same hold/toggle state as native HandleView. Do not latch zoom
        // or rely on a SHALLOW value surviving a movement-camera transition.
        if (controls.IsZoomToggle())
            return ECameraZoomType.NORMAL;
        return nativeZoom;
    }
}
#endif
