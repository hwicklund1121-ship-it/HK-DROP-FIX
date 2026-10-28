modded class DayZPlayerImplement
{
    private float m_GRZProneDistance = -1;

    float GRZMovementCameraDistance(float nativeDistance, bool proneCamera)
    {
        HumanMovementState state = new HumanMovementState();
        GetMovementState(state);
        bool prone = state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE;
        bool standing = state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_ERECT || state.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT;
        if (GetCurrentCommandID() != DayZPlayerConstants.COMMANDID_MOVE)
        {
            m_GRZProneDistance = -1;
            return nativeDistance;
        }
        if (prone)
        {
            if (proneCamera)
                m_GRZProneDistance = nativeDistance;
            return nativeDistance;
        }
        if (!standing)
        {
            m_GRZProneDistance = -1;
            return nativeDistance;
        }
        // Keep the prone-distance floor throughout the standing recovery.
        // Do not retain the prone pivot, which could place the camera in terrain.
        if (!proneCamera && m_GRZProneDistance > 0)
            return Math.Max(nativeDistance, m_GRZProneDistance);
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
            player.GRZMovementCameraDistance(pOutResult.m_fDistance, true);
            return;
        }
        if (camera != "DayZPlayerCamera3rdPersonErcSpr" && camera != "DayZPlayerCamera3rdPersonErc" && camera != "DayZPlayerCamera3rdPersonErcRaised" && camera != "DayZPlayerCamera3rdPersonErcRaisedMelee" && camera != "DayZPlayerCamera3rdPersonCrouch" && camera != "DayZPlayerCamera3rdPersonCrouchRaised")
            return;
        pOutResult.m_fDistance = player.GRZMovementCameraDistance(pOutResult.m_fDistance, false);
    }
}
