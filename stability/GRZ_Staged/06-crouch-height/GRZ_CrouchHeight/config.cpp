class CfgPatches
{
    class GRZ_CrouchHeight
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Scripts", "GRZ_Movement"};
    };
};
class CfgMods
{
    class GRZ_CrouchHeight
    {
        dir = "GRZ_CrouchHeight";
        name = "GRZ_CrouchHeight - staged stability test";
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"GRZ_CrouchHeight/scripts/4_World"};
            };
        };
    };
};
