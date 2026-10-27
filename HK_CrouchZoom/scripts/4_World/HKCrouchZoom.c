// Preserve a held eye zoom across crouch transitions, not weapon ADS.
modded class DayZPlayerImplement
{
    private int m_HKCrouchZoomPreviousStance = -1;
    private bool m_HKCrouchZoomLeavingCrouch;

    override void HandleView()
    {
        ECameraZoomType previousZoom = m_CameraEyeZoomLevel;
        super.HandleView();

        if (GetGame().IsDedicatedServer() || GetGame().GetPlayer() != this)
            return;

        int previousStance = m_HKCrouchZoomPreviousStance;
        m_HKCrouchZoomPreviousStance = m_MovementState.m_iStanceIdx;

        if (!IsAlive() || GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE)
        {
            m_HKCrouchZoomPreviousStance = -1;
            m_HKCrouchZoomLeavingCrouch = false;
            return;
        }

        HumanCommandMove move = GetCommand_Move();
        if (!move)
        {
            m_HKCrouchZoomLeavingCrouch = false;
            return;
        }
        int stance = m_HKCrouchZoomPreviousStance;
        bool crouched = stance == DayZPlayerConstants.STANCEIDX_CROUCH || stance == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH;
        bool wasCrouched = previousStance == DayZPlayerConstants.STANCEIDX_CROUCH || previousStance == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH;
        bool standing = stance == DayZPlayerConstants.STANCEIDX_ERECT || stance == DayZPlayerConstants.STANCEIDX_RAISEDERECT;
        if (!standing || !move.IsChangingStance())
            m_HKCrouchZoomLeavingCrouch = false;
        else if (wasCrouched)
            m_HKCrouchZoomLeavingCrouch = true;

        HumanInputController input = GetInputController();
        if (!input || !input.IsZoomToggle())
            return;

        // A deliberate raise/ADS request retains vanilla camera behavior.
        if (input.IsWeaponRaised() || input.WeaponADS() || m_CameraOptics || m_CameraIronsight)
            return;
        if (previousZoom == ECameraZoomType.NONE || m_CameraEyeZoomLevel != ECameraZoomType.NONE)
            return;

        if (crouched || m_HKCrouchZoomLeavingCrouch)
            m_CameraEyeZoomLevel = previousZoom;
    }
};

modded class DayZPlayerCamera3rdPerson
{
    override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
    {
        super.OnActivate(pPrevCamera, pPrevCameraResult);

        if (!pPrevCamera || !pPrevCameraResult || !m_pInput || !m_pPlayer)
            return;
        if (!m_pInput.IsZoomToggle() || m_pInput.IsWeaponRaised() || m_pInput.WeaponADS())
            return;
        if (m_pPlayer.GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE)
            return;

        // Exact camera names exclude sprint, jump, climb, prone and vehicles.
        // Do not carry FOV across unrelated view changes or into a scope.
        string oldCamera = pPrevCamera.ClassName();
        string newCamera = ClassName();
        bool oldCrouch = HKCrouchZoomIsCrouchCamera(oldCamera);
        bool newCrouch = HKCrouchZoomIsCrouchCamera(newCamera);
        bool oldStanding = HKCrouchZoomIsStandingCamera(oldCamera);
        bool newStanding = HKCrouchZoomIsStandingCamera(newCamera);
        if (!((oldStanding && newCrouch) || (oldCrouch && newStanding)))
            return;

        if (pPrevCameraResult.m_fFovAbsolute > 0)
        {
            m_fFovAbsolute = pPrevCameraResult.m_fFovAbsolute;
            m_fFovAbsVel[0] = 0;
        }
    }

    private bool HKCrouchZoomIsCrouchCamera(string cameraName)
    {
        return cameraName == "DayZPlayerCamera3rdPersonCrouch" || cameraName == "DayZPlayerCamera3rdPersonCrouchRaised";
    }

    private bool HKCrouchZoomIsStandingCamera(string cameraName)
    {
        return cameraName == "DayZPlayerCamera3rdPersonErc" || cameraName == "DayZPlayerCamera3rdPersonErcRaised" || cameraName == "DayZPlayerCamera3rdPersonErcRaisedMelee";
    }
};
