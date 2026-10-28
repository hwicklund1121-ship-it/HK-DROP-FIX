// GRZ response preset. These settings preserve the v0.15 movement behavior.
class GRZMovementPreset
{
    static const float ACCELERATION_SPAN = 0.06;
    static const float ACCELERATION_LIMIT = 100.0;
    static const float STRAFE_SPAN = 0.14;
    static const float TURN_THRESHOLD = 17.0;
    static const float LEAN_RESPONSE = 6.5;

    static void ConfigureResponse(SHumanCommandMoveSettings movement)
    {
        movement.m_fRunSpringTimeout = ACCELERATION_SPAN;
        movement.m_fRunSpringMaxChange = ACCELERATION_LIMIT;
        movement.m_fDirFilterTimeout = STRAFE_SPAN;
        movement.m_fDirFilterSprintTimeout = STRAFE_SPAN;
        movement.m_fDirFilterSpeed = Math.DEG2RAD * 360.0;
        movement.m_fMaxSprintAngle = Math.DEG2RAD * 45.01;
    }

    static void ConfigureTurning(SHumanCommandMoveSettings movement)
    {
        movement.m_fTurnAngle = TURN_THRESHOLD;
        movement.m_fTurnEndUpdateNTime = 0.7;
        movement.m_fTurnNextNTime = 0.9;
        movement.m_fHeadingChangeLimiterIdle = 500000.0;
        movement.m_fHeadingChangeLimiterWalk = 2000.0;
        movement.m_fHeadingChangeLimiterRun = 1500.0;
        movement.m_fLeaningSpeed = LEAN_RESPONSE;
    }

    static void ConfigureWaterMovement(SHumanCommandMoveSettings movement)
    {
        movement.m_iMaxSpeedNormal_WaterLevelLow = -1;
        movement.m_iMaxSpeedFast_WaterLevelLow = -1;
        movement.m_iMaxSpeedNormal_WaterLevelHigh = -1;
        movement.m_iMaxSpeedFast_WaterLevelHigh = -1;
    }

    static void ApplyTo(DayZPlayerType playerType)
    {
        SHumanCommandMoveSettings movement = playerType.CommandMoveSettingsW();
        ConfigureResponse(movement);
        ConfigureTurning(movement);
        ConfigureWaterMovement(movement);
    }
}

modded class DayZPlayerTypeUtils
{
    override static void DayZPlayerTypeInitMovement(DayZPlayerType pType)
    {
        super.DayZPlayerTypeInitMovement(pType);
        GRZMovementPreset.ApplyTo(pType);
    }
}
