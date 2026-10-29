// Preserve native stamina pools, recovery, synchronization and paid-action costs.
modded class StaminaHandler
{
    override void DepleteStaminaEx(EStaminaModifiers modifier, float dT = -1, float coef = 1.0)
    {
        switch (modifier)
        {
            case EStaminaModifiers.JUMP:
            case EStaminaModifiers.VAULT:
            case EStaminaModifiers.CLIMB:
            case EStaminaModifiers.ROLL:
            case EStaminaModifiers.MELEE_EVADE:
                return;
        }
        // Punches/weapon melee and holding breath keep native costs/cooldowns.
        // Drowning, pushing cars and other non-locomotion drains remain native.
        super.DepleteStaminaEx(modifier, dT, coef);
    }
}
