// Independently authored replacement for the old full CameraHandler override.
// Retains current DayZ camera selection; holds requested eye zoom and stabilizes
// the verified third-person sprint roll / crouch movement-height sources.
// Held eye zoom for standing/crouched movement, including shoulder aim.
// Weapon ironsights and scopes keep their own camera/FOV behavior.
modded class DayZPlayerImplement
{
    private bool m_HKZoomStableAnnounced;
    private bool m_HKZoomStableHolding;

    override void HandleView()
    {
        super.HandleView();

        if (GetGame().IsDedicatedServer() || GetGame().GetPlayer() != this)
            return;
        if (!m_HKZoomStableAnnounced)
        {
            Print("[HKZOOMSTABLE] version=1 loaded");
            m_HKZoomStableAnnounced = true;
        }

        if (!HKZoomStableCanKeepEyeZoom())
        {
            m_HKZoomStableHolding = false;
            return;
        }

        if (m_CameraEyeZoomLevel == ECameraZoomType.NONE)
        {
            if (!m_HKZoomStableHolding)
                Print("[HKZOOMSTABLE] version=1 preserving held eye zoom");
            m_CameraEyeZoomLevel = ECameraZoomType.NORMAL;
        }
        m_HKZoomStableHolding = true;
    }

    private bool HKZoomStableCanKeepEyeZoom()
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
    private float m_HKZoomStableLockedDistance = -1;
    private float m_HKZoomStableLogElapsed;
    private int m_HKZoomStableLogSamples;

    override void StdFovUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
    {
        super.StdFovUpdate(pDt, pOutResult);

        if (!m_pPlayer || !m_pInput)
            return;
        string cameraName = ClassName();
        bool eligibleCamera = HKZoomStableIsCrouchCamera(cameraName) || HKZoomStableIsStandingCamera(cameraName);
        PlayerBase player = PlayerBase.Cast(m_pPlayer);
        if (!eligibleCamera || !m_pInput.IsZoomToggle() || m_pInput.WeaponADS() || m_pPlayer.GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE || !player || !player.IsAlive() || player.IsFighting())
        {
            m_HKZoomStableLockedDistance = -1;
            return;
        }

        // Hold FOV at the configured eye-zoom target even if Raised briefly
        // clears the player zoom flag. Native stance-camera blending can also
        // move the boom from 0.8 m (rifle raised) to 1.4 m (lowered crouch).
        if (m_HKZoomStableLockedDistance < 0)
            m_HKZoomStableLockedDistance = pOutResult.m_fDistance;
        m_fFovAbsolute = GetDayZGame().GetFOVByZoomType(ECameraZoomType.NORMAL);
        m_fFovAbsVel[0] = 0;
        pOutResult.m_fFovAbsolute = m_fFovAbsolute;
        pOutResult.m_fFovMultiplier = 1.0;
        pOutResult.m_fDistance = m_HKZoomStableLockedDistance;
        pOutResult.m_bUpdateWhenBlendOut = true;

        // Bounded camera-level evidence if the visual pullback persists.
        m_HKZoomStableLogElapsed += pDt;
        if (m_HKZoomStableLogElapsed >= 0.25 && m_HKZoomStableLogSamples < 40)
        {
            m_HKZoomStableLogElapsed = 0;
            m_HKZoomStableLogSamples++;
            Print("[HKZOOMSTABLE] version=1 camera=" + cameraName + " eye=" + m_pPlayer.GetEyeZoomLevel().ToString() + " fov=" + pOutResult.m_fFovAbsolute.ToString() + " distance=" + pOutResult.m_fDistance.ToString() + " native_distance=" + m_fDistance.ToString());
        }
    }

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
        bool oldCrouch = HKZoomStableIsCrouchCamera(oldCamera);
        bool newCrouch = HKZoomStableIsCrouchCamera(newCamera);
        bool oldStanding = HKZoomStableIsStandingCamera(oldCamera);
        bool newStanding = HKZoomStableIsStandingCamera(newCamera);
        if (!((oldStanding || oldCrouch) && (newStanding || newCrouch)))
            return;

        if (pPrevCameraResult.m_fDistance > 0)
            m_HKZoomStableLockedDistance = pPrevCameraResult.m_fDistance;

        if (pPrevCameraResult.m_fFovAbsolute > 0)
        {
            m_fFovAbsolute = pPrevCameraResult.m_fFovAbsolute;
            m_fFovAbsVel[0] = 0;
        }
    }

    private bool HKZoomStableIsCrouchCamera(string cameraName)
    {
        return cameraName == "DayZPlayerCamera3rdPersonCrouch" || cameraName == "DayZPlayerCamera3rdPersonCrouchRaised";
    }

    private bool HKZoomStableIsStandingCamera(string cameraName)
    {
        return cameraName == "DayZPlayerCamera3rdPersonErc" || cameraName == "DayZPlayerCamera3rdPersonErcRaised" || cameraName == "DayZPlayerCamera3rdPersonErcRaisedMelee";
    }
};

// The native sprint update derives camera roll from the animated pelvis.
// Suppress the roll getter; leave its camera update and normal view controls.
modded class DayZPlayerCamera3rdPersonErcSpr
{
    override float GetCurrentRoll()
    {
        return 0.0;
    }
};

// Native crouch OnUpdate adds m_fCameraHeight based on movement speed.
// Remove only that additive Y offset after the native camera update. Stance
// height, aim pitch/yaw, lean, shoulder switching and collision remain handled
// by the normal camera. Raised crouch derives from this class as well.
modded class DayZPlayerCamera3rdPersonCrouch
{
    override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
    {
        super.OnUpdate(pDt, pOutResult);
        pOutResult.m_CameraTM[3][1] = pOutResult.m_CameraTM[3][1] - m_fCameraHeight;
    }
};
