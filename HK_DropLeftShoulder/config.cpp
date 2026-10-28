class CfgPatches
{
    class HK_DropLeftShoulder
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Scripts", "LUGUINSTADROPFIX"};
    };
};

class CfgMods
{
    class HK_DropLeftShoulder
    {
        dir = "HK_DropLeftShoulder";
        name = "HK Drop Left Shoulder";
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"HK_DropLeftShoulder/scripts/4_World"};
            };
        };
    };
};
