// Smooth ordinary third-person framing, without filtering mouse rotation,
// changing player movement, or modifying the prone animation camera.
modded class DayZPlayerCamera3rdPerson
{
    private bool m_GRZFrameReady;
    private vector m_GRZFramePosition;
    private float m_GRZFrameDistance;
    private float m_GRZFrameVelocityX[1];
    private float m_GRZFrameVelocityY[1];
    private float m_GRZFrameVelocityZ[1];
    private float m_GRZFrameVelocityDistance[1];

    private bool GRZIsMovementFrame(string name)
    {
        return name == "DayZPlayerCamera3rdPersonErc" || name == "DayZPlayerCamera3rdPersonErcSpr" || name == "DayZPlayerCamera3rdPersonErcRaised" || name == "DayZPlayerCamera3rdPersonErcRaisedMelee" || name == "DayZPlayerCamera3rdPersonCrouch" || name == "DayZPlayerCamera3rdPersonCrouchRaised";
    }

    override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
    {
        super.OnActivate(pPrevCamera, pPrevCameraResult);
        m_GRZFrameReady = false;
        m_GRZFrameVelocityX[0] = 0;
        m_GRZFrameVelocityY[0] = 0;
        m_GRZFrameVelocityZ[0] = 0;
        m_GRZFrameVelocityDistance[0] = 0;
        if (pPrevCamera && pPrevCameraResult && GRZIsMovementFrame(ClassName()) && GRZIsMovementFrame(pPrevCamera.ClassName()))
        {
            m_GRZFramePosition = pPrevCameraResult.m_CameraTM[3];
            m_GRZFrameDistance = pPrevCameraResult.m_fDistance;
            m_GRZFrameReady = true;
        }
    }

    override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
    {
        super.OnUpdate(pDt, pOutResult);
        if (!GRZIsMovementFrame(ClassName()))
            return;
        // A crouch subclass removes its movement-height term after this call;
        // account for that term so it does not enter the smoothing history.
        vector targetPosition = pOutResult.m_CameraTM[3];
        float crouchHeight = 0;
        DayZPlayerCamera3rdPersonCrouch crouch = DayZPlayerCamera3rdPersonCrouch.Cast(this);
        if (crouch)
            crouchHeight = crouch.GRZGetRemovedMovementHeight();
        targetPosition[1] = targetPosition[1] - crouchHeight;
        if (!m_GRZFrameReady || pDt <= 0 || pDt > 0.25)
        {
            m_GRZFramePosition = targetPosition;
            m_GRZFrameDistance = pOutResult.m_fDistance;
            m_GRZFrameReady = true;
        }
        else
        {
            m_GRZFramePosition[0] = Math.SmoothCD(m_GRZFramePosition[0], targetPosition[0], m_GRZFrameVelocityX, 0.08, 1000, pDt);
            m_GRZFramePosition[1] = Math.SmoothCD(m_GRZFramePosition[1], targetPosition[1], m_GRZFrameVelocityY, 0.08, 1000, pDt);
            m_GRZFramePosition[2] = Math.SmoothCD(m_GRZFramePosition[2], targetPosition[2], m_GRZFrameVelocityZ, 0.08, 1000, pDt);
            m_GRZFrameDistance = Math.SmoothCD(m_GRZFrameDistance, pOutResult.m_fDistance, m_GRZFrameVelocityDistance, 0.08, 1000, pDt);
        }
        pOutResult.m_CameraTM[3] = m_GRZFramePosition;
        // The crouch subclass will subtract this once on return.
        pOutResult.m_CameraTM[3][1] = pOutResult.m_CameraTM[3][1] + crouchHeight;
        pOutResult.m_fDistance = m_GRZFrameDistance;
    }
}

modded class DayZPlayerCamera3rdPersonCrouch
{
    float GRZGetRemovedMovementHeight()
    {
        return m_fCameraHeight;
    }
}
