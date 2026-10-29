// Shared client/server policy: free locomotion; normal melee and breath costs.
// Override only stamina configuration accessors, not movement tuning or sync.
modded class CfgGameplayHandler
{
    override static float GetSprintStaminaModifierErc()
    {
        return 0.0;
    }

    override static float GetSprintStaminaModifierCro()
    {
        return 0.0;
    }

    override static float GetSprintSwimmingStaminaModifier()
    {
        return 0.0;
    }

    override static float GetSprintLadderStaminaModifier()
    {
        return 0.0;
    }

    override static float GetStaminaWeightLimitThreshold()
    {
        return 100.0;
    }

    override static float GetStaminaMax()
    {
        return 100.0;
    }

    override static float GetStaminaKgToStaminaPercentPenalty()
    {
        return 0.0;
    }

    override static float GetStaminaMinCap()
    {
        return 5.0;
    }

    override static float GetMeleeStaminaModifier()
    {
        return 1.0;
    }

    override static float GetObstacleTraversalStaminaModifier()
    {
        return 0.0;
    }

    override static float GetHoldBreathStaminaModifier()
    {
        return 1.0;
    }

}
