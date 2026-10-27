modded class PlayerBase extends ManBase
{
	bool IsSprinting()
	{
		return m_MovementState.m_iMovement == DayZPlayerConstants.MOVEMENT_SPRINT;
	}

	bool CanSprint()
	{
		ItemBase item = GetItemInHands();
		if ( IsRaised() || (item && item.IsHeavyBehaviour()) )
		{
			return false;
		}
		
		if (GetBrokenLegs() != eBrokenLegs.NO_BROKEN_LEGS)
		{
			return false;
		}
		
		return true;
	}
	
	bool IsInProne()
	{
		return m_MovementState.IsInProne();
	}
	
	bool IsInRasedProne()
	{
		return m_MovementState.IsInRaisedProne();
	}	
	
	bool IsLeaning()
	{
		return m_MovementState.IsLeaning();
	}	
	
	bool IsRolling()
	{
		if (GetCommand_Move() && GetCommand_Move().IsInRoll())
			return true;
		return false;
	}
	
	override bool IsRaised()
	{
		//GetMovementState(m_MovementState);
		return m_MovementState.IsRaised();
	}
override void OnStanceChange(int previousStance, int newStance)
	{
		super.OnStanceChange(previousStance, newStance);
		//Print(previousStance);
		//Print(newStance);
		
		int prone = DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDPRONE;
		int notProne = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_RAISEDERECT | DayZPlayerConstants.STANCEMASK_RAISEDCROUCH;
		
		if ( ( IsStance(previousStance, prone) && IsStance(newStance, notProne) ) || ( IsStance(previousStance, notProne) && IsStance(newStance, prone) ) )
		{
			AbortWeaponEvent();
			GetWeaponManager().RefreshAnimationState();
		}
		
		if ( GetGame().IsServer() )
		{
			//
		}
	}

	
	override void OnCommandMoveStart()
	{
		/* if ( GetGame().IsServer() )
		{
			//In case player changes stance through a different command, we refresh the anim overrides
			int prone = DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDPRONE;
			if ( !IsPlayerInStance( prone ) )
			{
				m_InjuryHandler.m_ForceInjuryAnimMask = m_InjuryHandler.m_ForceInjuryAnimMask & ~eInjuryOverrides.PRONE_ANIM_OVERRIDE;
				ForceUpdateInjuredState();
			}
		} */
	}
	override bool CanRoll()
	{
		if (!CanConsumeStamina(EStaminaConsumers.ROLL)) 
			return false;
		
		if (IsInFBEmoteState() || m_EmoteManager.m_MenuEmote)
		{
			return false;
		}
		
		return IsPlayerInStance(DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDPRONE) && GetCommand_Move();// && !GetCommand_Move().IsOnBack();
	}
	
	override void OnRollStart(bool isToTheRight)
	{
		/* DepleteStamina(EStaminaModifiers.ROLL);
		
		if (GetBrokenLegs() == eBrokenLegs.BROKEN_LEGS)
			m_ShockHandler.SetShock(PlayerConstants.BROKEN_LEGS_ROLL_SHOCK); */
	}
	
	override void OnRollFinish()
	{
	}
	
	override void CheckLiftWeapon()
	{
		// lift weapon check
		if ( GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT /*GetGame().IsClient()*/)
		{
			Weapon_Base weap;
			if ( Weapon_Base.CastTo(weap, GetItemInHands()) )
			{				
				bool limited = weap.LiftWeaponCheck(this);

				if (limited && !m_LiftWeapon_player)
					SendLiftWeaponSync(false);
				else if (!limited && m_LiftWeapon_player)
					SendLiftWeaponSync(false);
			}
			else if (m_LiftWeapon_player)
			{
				SendLiftWeaponSync(true);
			}
		}
	}
}	
