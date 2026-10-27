modded class DayZPlayerTypeUtils
{
	override static void DayZPlayerTypeCreate(DayZPlayerType pType)
	{
		DayZPlayerTypeRegisterItems(pType);
		DayZPlayerTypeRegisterSounds(pType);
		DayZPlayerCameras.RegisterCameras(pType);
		pType.SetLookLimits(-85, 85, -160, 160);	
		pType.SetAimLimits(-85, 85, -180, 180);		
		pType.SetCameraShootParams(250, 0.01, 0.866);
		pType.RegisterHitComponentsForAI();
    }
    static void DayZPlayerTypeInitMovement(DayZPlayerType pType)
	{
	
		SHumanCommandMoveSettings hmcs = pType.CommandMoveSettingsW();

		hmcs.m_fRunSpringTimeout	            = 0.01;												
		hmcs.m_fRunSpringMaxChange	            = 200.0;
		hmcs.m_fDirFilterTimeout			    = 0.1;	
		hmcs.m_fDirFilterSprintTimeout 		    = 0.1;
		hmcs.m_fDirFilterSpeed				    = Math.DEG2RAD * 360; 
		hmcs.m_fMaxSprintAngle				    = Math.DEG2RAD * 45.01;	
		hmcs.m_fTurnAngle 			            = 17;					
		hmcs.m_fTurnEndUpdateNTime           	= 0.7;					
		hmcs.m_fTurnNextNTime		            = 0.9;					
		hmcs.m_fHeadingChangeLimiterIdle 	    = 500000; 		
		hmcs.m_fHeadingChangeLimiterWalk 	    = 2000;			
		hmcs.m_fHeadingChangeLimiterRun		    = 1500;			
		hmcs.m_iMaxSpeedNormal_WaterLevelLow	= -1;		
		hmcs.m_iMaxSpeedFast_WaterLevelLow		= -1;			
		hmcs.m_iMaxSpeedNormal_WaterLevelHigh	= -1;		
		hmcs.m_iMaxSpeedFast_WaterLevelHigh		= -1;	
        hmcs.m_fLeaningSpeed				    =  6.8;
    }
}
modded class DayzPlayerItemBehaviorCfg extends HumanItemBehaviorCfg
{
	void DayzPlayerItemBehaviorCfg()
	{
		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ALL;

		m_StanceMovements[DayZPlayerConstants.STANCEIDX_ERECT] 			= DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_CROUCH] 		= DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_PRONE] 			= DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDERECT] 	= DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDCROUCH] 	= DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDPRONE] 	= DayZPlayerConstants.MOVEMENT_IDLE;

		m_StanceRotation[DayZPlayerConstants.STANCEIDX_ERECT] 			= DayZPlayerConstants.ROTATION_ENABLE;
		m_StanceRotation[DayZPlayerConstants.STANCEIDX_CROUCH] 			= DayZPlayerConstants.ROTATION_ENABLE;
		m_StanceRotation[DayZPlayerConstants.STANCEIDX_PRONE] 			= DayZPlayerConstants.ROTATION_ENABLE;
		m_StanceRotation[DayZPlayerConstants.STANCEIDX_RAISEDERECT] 	= DayZPlayerConstants.ROTATION_ENABLE;
		m_StanceRotation[DayZPlayerConstants.STANCEIDX_RAISEDCROUCH] 	= DayZPlayerConstants.ROTATION_ENABLE;
		m_StanceRotation[DayZPlayerConstants.STANCEIDX_RAISEDPRONE] 	= DayZPlayerConstants.ROTATION_ENABLE;

		
		m_iPerItemCameraUserData 		= DayZPlayerCameras.PERITEMUD_INVALID;
		m_fMoveHeadingFilterSpan 		= CfgGameplayHandler.GetMovementRotationSpeedJog();
		m_fMoveHeadingSprintFilterSpan 	= CfgGameplayHandler.GetMovementRotationSpeedSprint();
		m_fMoveHeadingProneFilterSpan 	= 0.7;
		m_fMoveHeadingFilterSpeed		= Math.DEG2RAD * 300;
		m_fMeleeEvadeHeadingFilterSpan	= 0.07;
		m_fMeleeEvadeHeadingFilterSpeed	= Math.DEG2RAD * 1440;
		m_bAttackLean = false;
		m_bJumpAllowed = true;
	}
}

modded class DayZPlayerImplement extends DayZPlayer
{		
    void OnMovementChanged()
	{
	}
	
	void OnSprintStart()
	{
	}
	
	void OnSprintEnd()
	{
	}
}
