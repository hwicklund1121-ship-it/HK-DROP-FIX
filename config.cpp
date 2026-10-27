class CfgPatches
{
	class LUGUINSTADROPFIX
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]={"DZ_Scripts","DZ_Characters", "DZ_Data"};
	};
};
class CfgMods
{
	class LUGUINSTADROPFIX
	{
		dir="LUGUINSTADROPFIX";
		picture="";
		action="";
		hideName=0;
		hidePicture=0;
		name="LUGUINSTADROPFIX";
		credits="LUGU";
		author="LUGU";
		authorID="0";
		version="1.0";
		extra=0;
		type="mod";
	};
};


class CfgVehicles
{
	class Man;
	class SurvivorBase: Man
	{
		class enfAnimSys
		{
			meshObject="dz\characters\bodies\player_testing.xob";
			graphName="LUGUINSTADROPFIX\IO2JF\GJDH4KD\FGHJDGH\I3FJ4\GJKL2H\GJKDF9G\JFLK21KGH\KLJSDF8F23\GHJKDSDSHIG.agr";
			defaultInstance="dz\anims\workspaces\player\player_main\player_main.asi";
			skeletonName="player_testing.xob";
			startNode="MasterControl";
		};
		class AnimSystem
		{
			meshObject="INVALID MESH OBJET NAME";
			animGraph="INVALID ANIMGRAPH NAME";
		};
	};	
};