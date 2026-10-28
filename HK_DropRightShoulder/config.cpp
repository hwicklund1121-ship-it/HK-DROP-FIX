class CfgPatches
{
    class HK_DropRightShoulder
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Scripts", "LUGUINSTADROPFIX"};
    };
};

class CfgMods
{
    class HK_DropRightShoulder
    {
        dir = "HK_DropRightShoulder";
        name = "HK Drop Right Shoulder";
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"HK_DropRightShoulder/scripts/4_World"};
            };
        };
    };
};
