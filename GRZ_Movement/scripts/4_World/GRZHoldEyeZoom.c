modded class DayZPlayerImplement
{
    bool GRZHasHeldEyeZoom()
    {
        if (!IsAlive() || IsFighting() || GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE)
            return false;
        HumanInputController controls = GetInputController();
        if (!controls || controls.WeaponADS() || m_CameraOptics || m_CameraIronsight)
            return false;
        if (!controls.IsZoom() && !controls.IsZoomToggle())
            return false;
        HumanMovementState state = new HumanMovementState();
        GetMovementState(state);
        if (state.m_iStanceIdx != DayZPlayerConstants.STANCEIDX_ERECT && state.m_iStanceIdx != DayZPlayerConstants.STANCEIDX_RAISEDERECT && state.m_iStanceIdx != DayZPlayerConstants.STANCEIDX_CROUCH && state.m_iStanceIdx != DayZPlayerConstants.STANCEIDX_RAISEDCROUCH)
            return false;
        float speed;
        vector direction;
        controls.GetMovement(speed, direction);
        return speed < 3;
    }

    override int GetEyeZoomLevel()
    {
        if (GRZHasHeldEyeZoom())
            return ECameraZoomType.NORMAL;
        return super.GetEyeZoomLevel();
    }
}
