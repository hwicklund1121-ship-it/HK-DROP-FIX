modded class DayZPlayerImplement
{
    private bool m_GRZWasProne;
    private float m_GRZRecoveryStart = -1;

    float GRZMovementCameraDistance(float nativeDistance)
    {
        HumanMovementState state = new HumanMovementState();
        GetMovementState(state);
        bool prone = state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE;
        bool standing = state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT;
        float now = GetGame().GetTime() * 0.001;
        if (prone)
        {
            m_GRZWasProne = true;
            m_GRZRecoveryStart = -1;
            return nativeDistance;
        }
        if (m_GRZWasProne)
        {
            m_GRZWasProne = false;
            if (standing && GetCurrentCommandID() == DayZPlayerConstants.COMMANDID_MOVE)
                m_GRZRecoveryStart = now;
        }
        if (GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE)
        {
            m_GRZRecoveryStart = -1;
            return nativeDistance;
        }
        HumanInputController controls = GetInputController();
        if (controls && (controls.IsZoom() || controls.IsZoomToggle() || controls.WeaponADS()))
            return nativeDistance;
        if (standing && m_GRZRecoveryStart >= 0)
        {
            float elapsed = now - m_GRZRecoveryStart;
            // Extra room only after prone recovery: hold briefly, then fade.
            if (elapsed < 1.2)
                return nativeDistance + 0.4 * Math.Clamp((1.2 - elapsed) / 0.6, 0.0, 1.0);
            m_GRZRecoveryStart = -1;
        }
        return nativeDistance;
    }
}

modded class DayZPlayerCamera3rdPerson
{
    override void StdFovUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
    {
        super.StdFovUpdate(pDt, pOutResult);
        DayZPlayerImplement player = DayZPlayerImplement.Cast(m_pPlayer);
        if (!player)
            return;
        string camera = ClassName();
        // Observe prone for the recovery transition, without changing its result.
        if (camera == "DayZPlayerCamera3rdPersonProne" || camera == "DayZPlayerCamera3rdPersonProneRaised")
        {
            player.GRZMovementCameraDistance(pOutResult.m_fDistance);
            return;
        }
        if (camera != "DayZPlayerCamera3rdPersonErc" && camera != "DayZPlayerCamera3rdPersonErcRaised" && camera != "DayZPlayerCamera3rdPersonErcRaisedMelee" && camera != "DayZPlayerCamera3rdPersonCrouch" && camera != "DayZPlayerCamera3rdPersonCrouchRaised")
            return;
        pOutResult.m_fDistance = player.GRZMovementCameraDistance(pOutResult.m_fDistance);
    }
}
