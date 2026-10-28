class CfgPatches
{
    class GRZ_Movement
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Scripts"};
    };
};
class CfgMods
{
    class GRZ_Movement
    {
        dir = "GRZ_Movement";
        name = "GRZ_Movement - staged stability test";
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"GRZ_Movement/scripts/4_World"};
            };
        };
    };
};
