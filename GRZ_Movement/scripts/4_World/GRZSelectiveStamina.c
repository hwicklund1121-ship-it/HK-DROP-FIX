// Free movement with real action stamina: no continuous refill or global
// stamina disable, so jump and melee costs retain native cooldown/recovery.
modded class StaminaHandler
{
    override protected void ProcessMovementState()
    {
        super.ProcessMovementState();
        if (m_StaminaDelta < 0)
            m_StaminaDelta = 0;
    }

    override void DepleteStaminaEx(EStaminaModifiers modifier, float dT = -1, float coef = 1.0)
    {
        switch (modifier)
        {
            case EStaminaModifiers.JUMP:
            case EStaminaModifiers.MELEE_LIGHT:
            case EStaminaModifiers.MELEE_HEAVY:
                super.DepleteStaminaEx(modifier, dT, coef);
                break;
        }
    }

    override void RecalculateStaminaCap()
    {
        // Keep carrying weight from shrinking the action-stamina pool.
        m_StaminaCap = CfgGameplayHandler.GetStaminaMax();
    }

    override bool HasEnoughStaminaFor(EStaminaConsumers consumer)
    {
        if (consumer == EStaminaConsumers.JUMP || consumer == EStaminaConsumers.MELEE_HEAVY)
            return super.HasEnoughStaminaFor(consumer);
        return true;
    }

    override bool HasEnoughStaminaToStart(EStaminaConsumers consumer)
    {
        if (consumer == EStaminaConsumers.JUMP || consumer == EStaminaConsumers.MELEE_HEAVY)
            return super.HasEnoughStaminaToStart(consumer);
        return true;
    }
}
