// GRZ legacy camera policy. Let DayZ resolve special cameras first, then adjust
// only ordinary third-person movement cameras. No external zoom code or assets.
modded class DayZPlayerImplement
{
    override int CameraHandler(int pCameraMode)
    {
        int selectedCamera = super.CameraHandler(pCameraMode);

        // These conditions leave vehicles, first person, ADS, unconsciousness,
        // jump and climb under the native handler's control.
        if (!m_Camera3rdPerson || !GetCommand_Move() || GetCommand_Vehicle())
            return selectedCamera;

        switch (selectedCamera)
        {
            case DayZPlayerCameras.DAYZCAMERA_3RD_ERC:
            case DayZPlayerCameras.DAYZCAMERA_3RD_CRO:
            case DayZPlayerCameras.DAYZCAMERA_3RD_PRO:
                break;
            default:
                return selectedCamera;
        }

        HumanMovementState cameraState = new HumanMovementState();
        GetMovementState(cameraState);

        // Keep the raised camera family through stance transitions instead of
        // temporarily selecting a lowered camera before selecting raised again.
        switch (cameraState.m_iStanceIdx)
        {
            case DayZPlayerConstants.STANCEIDX_RAISEDCROUCH:
                return DayZPlayerCameras.DAYZCAMERA_3RD_CRO_RAISED;
            case DayZPlayerConstants.STANCEIDX_RAISEDPRONE:
                return DayZPlayerCameras.DAYZCAMERA_3RD_PRO_RAISED;
            case DayZPlayerConstants.STANCEIDX_RAISEDERECT:
                if (m_IsWeapon)
                    return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED;
                return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED_MELEE;
        }

        // Sprint is a movement index, independent of the stance index.
        if (cameraState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT && cameraState.m_iMovement == DayZPlayerConstants.MOVEMENTIDX_SPRINT)
            return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_SPR;

        return selectedCamera;
    }
}
