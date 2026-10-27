// fuck you StormZz

modded class DayZPlayerImplementAiming extends DayZPlayer
{
    protected float CalculateWeight(int stance_index, float current_stamina, float camera_sway_modifier, bool holding_breath)
    {
        if (m_PlayerDpi.GetCommand_Move() && m_PlayerDpi.GetCommand_Move().IsInRoll())//when the player is rolling, set a constant and disregard everything else
        {
        //return PlayerSwayConstants.SWAY_ROLL;
        }
        float stance_modifier;
        //float scale = SWAY_WEIGHT_SCALER;
        switch ( stance_index )
        {
            case DayZPlayerConstants.STANCEIDX_RAISEDERECT:
            stance_modifier = 0.5;
            break;
            stance_modifier = 0.85;
            break;
            case DayZPlayerConstants.STANCEIDX_RAISEDPRONE:
            stance_modifier = 0.95;
            break;
            default:
            stance_modifier = 0.95;
            break;
        }
        #ifdef DEVELOPER
    	DbgPrintAimingImplement("current_stamina: " + current_stamina);
    	DbgPrintAimingImplement("camera_sway_modifier: " + camera_sway_modifier);
    	DbgPrintAimingImplement("holding_breath: " + holding_breath);
    	#endif
    
    	return (1 - stance_modifier) * m_AimNoiseAllowed * camera_sway_modifier * SWAY_WEIGHT_SCALER;
    }
}
