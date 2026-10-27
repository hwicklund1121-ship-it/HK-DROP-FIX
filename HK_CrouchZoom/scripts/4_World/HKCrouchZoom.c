// Held eye zoom for standing/crouched movement, including shoulder aim.
// Weapon ironsights and scopes keep their own camera/FOV behavior.
modded class DayZPlayerImplement
{
    private bool m_HKCrouchZoomAnnounced;
    private bool m_HKCrouchZoomHolding;

    override void HandleView()
    {
        super.HandleView();

        if (GetGame().IsDedicatedServer() || GetGame().GetPlayer() != this)
            return;
        if (!m_HKCrouchZoomAnnounced)
        {
            Print("[HKCROUCHZOOM] version=2 loaded");
            m_HKCrouchZoomAnnounced = true;
        }

        if (!HKCrouchZoomCanKeepEyeZoom())
        {
            m_HKCrouchZoomHolding = false;
            return;
        }

        if (m_CameraEyeZoomLevel == ECameraZoomType.NONE)
        {
            if (!m_HKCrouchZoomHolding)
                Print("[HKCROUCHZOOM] version=2 preserving held eye zoom");
            m_CameraEyeZoomLevel = ECameraZoomType.NORMAL;
        }
        m_HKCrouchZoomHolding = true;
    }

    private bool HKCrouchZoomCanKeepEyeZoom()
    {
        if (!IsAlive() || GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE || !GetCommand_Move())
            return false;

        HumanInputController input = GetInputController();
        if (!input || !input.IsZoomToggle())
            return false;
        if (input.WeaponADS() || m_CameraOptics || m_CameraIronsight || IsFighting())
            return false;

        int stance = m_MovementState.m_iStanceIdx;
        bool crouched = stance == DayZPlayerConstants.STANCEIDX_CROUCH || stance == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH;
        bool standing = stance == DayZPlayerConstants.STANCEIDX_ERECT || stance == DayZPlayerConstants.STANCEIDX_RAISEDERECT;
        if (!crouched && !standing)
            return false;

        float speed;
        vector direction;
        input.GetMovement(speed, direction);
        // Leave standing sprint zoom restrictions to the inherited handler.
        if (standing && speed >= 3)
            return false;
        return true;
    }
};

modded class DayZPlayerCamera3rdPerson
{
    override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
    {
        super.OnActivate(pPrevCamera, pPrevCameraResult);

        if (!pPrevCamera || !pPrevCameraResult || !m_pInput || !m_pPlayer)
            return;
        if (!m_pInput.IsZoomToggle() || m_pInput.WeaponADS())
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
        if (!((oldStanding || oldCrouch) && (newStanding || newCrouch)))
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
