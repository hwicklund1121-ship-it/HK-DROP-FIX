// Keep the original FOV-revert CameraHandler and all camera zoom logic intact.
// Remove only the speed-dependent height addition made by native crouch update.
modded class DayZPlayerCamera3rdPersonCrouch
{
    override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
    {
        super.OnUpdate(pDt, pOutResult);

        // Native crouch sets its model-space Y offset to 0.7 + this value.
        // Removing only the addition retains the actual crouching height,
        // mouse look, lean, shoulder offsets and camera collision handling.
        pOutResult.m_CameraTM[3][1] = pOutResult.m_CameraTM[3][1] - m_fCameraHeight;
    }
};
