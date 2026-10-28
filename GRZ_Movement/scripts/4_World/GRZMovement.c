// GRZ Movement: tune movement response after the engine initializes its defaults.
// Animation graphs, throwing, stance recovery and camera selection stay with
// DayZ and the separately installed LUGU addon.
class GRZMovementResponse
{
    static void Apply(DayZPlayerType playerType)
    {
        SHumanCommandMoveSettings settings = playerType.CommandMoveSettingsW();

        // Mild acceleration/strafe easing; retain the existing turning and lean.
        settings.m_fRunSpringTimeout = 0.06;
        settings.m_fRunSpringMaxChange = 100.0;
        settings.m_fDirFilterTimeout = 0.14;
        settings.m_fDirFilterSprintTimeout = 0.14;
        settings.m_fDirFilterSpeed = 360.0 * Math.DEG2RAD;
        settings.m_fMaxSprintAngle = 45.01 * Math.DEG2RAD;

        settings.m_fTurnAngle = 17.0;
        settings.m_fTurnEndUpdateNTime = 0.7;
        settings.m_fTurnNextNTime = 0.9;
        settings.m_fHeadingChangeLimiterIdle = 500000.0;
        settings.m_fHeadingChangeLimiterWalk = 2000.0;
        settings.m_fHeadingChangeLimiterRun = 1500.0;
        settings.m_fLeaningSpeed = 6.5;

        settings.m_iMaxSpeedNormal_WaterLevelLow = -1;
        settings.m_iMaxSpeedFast_WaterLevelLow = -1;
        settings.m_iMaxSpeedNormal_WaterLevelHigh = -1;
        settings.m_iMaxSpeedFast_WaterLevelHigh = -1;
    }
}

modded class DayZPlayerTypeUtils
{
    override static void DayZPlayerTypeInitMovement(DayZPlayerType pType)
    {
        super.DayZPlayerTypeInitMovement(pType);
        GRZMovementResponse.Apply(pType);
    }
}
