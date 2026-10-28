// Preserve ordinary held eye zoom through standing/crouching transitions.
// Camera FOV values, smoothing and distances remain native.
modded class DayZPlayerImplement
{
    override void HandleView()
    {
        super.HandleView();

        if (GetGame().IsDedicatedServer() || GetGame().GetPlayer() != this)
            return;
        if (!IsAlive() || IsFighting() || GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE)
            return;

        HumanInputController controls = GetInputController();
        if (!controls || controls.WeaponADS() || m_CameraOptics || m_CameraIronsight)
            return;
        if (!controls.IsZoom() && !controls.IsZoomToggle())
            return;

        HumanMovementState zoomState = new HumanMovementState();
        GetMovementState(zoomState);
        switch (zoomState.m_iStanceIdx)
        {
            case DayZPlayerConstants.STANCEIDX_ERECT:
            case DayZPlayerConstants.STANCEIDX_RAISEDERECT:
            case DayZPlayerConstants.STANCEIDX_CROUCH:
            case DayZPlayerConstants.STANCEIDX_RAISEDCROUCH:
                break;
            default:
                return;
        }

        // Preserve the native sprint zoom policy. No saved zoom state survives
        // button release; the inherited handler decides it anew every frame.
        float requestedSpeed;
        vector requestedDirection;
        controls.GetMovement(requestedSpeed, requestedDirection);
        if (requestedSpeed >= 3)
            return;

        if (m_CameraEyeZoomLevel == ECameraZoomType.NONE)
            m_CameraEyeZoomLevel = ECameraZoomType.NORMAL;
    }
}
