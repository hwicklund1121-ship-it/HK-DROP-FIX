class CfgPatches
{
	class OPM_Scripts
	{
		units[]={};
		weapons[]={};
		requiredVersion=1;
		requiredAddons[]={};
	};
};
class CfgMods
{
	class OPM_Scripts
	{
		dir="ronjam_fix";
		picture="";
		action="";
		hideName=1;
		hidePicture=1;
		name="OPM_Scripts";
		credits="Operation Monarch / Echo";
		author="Operation Monarch / Echo";
		authorID="0";
		version=1;
		extra=0;
		type="mod";
		dependencies[]=
		{
			"World"
		};
		class defs
		{
			class worldScriptModule
			{
				value="";
				files[]=
				{
					"OPM_Scripts/scripts/4_World"
				};
			};
		};
	};
};
