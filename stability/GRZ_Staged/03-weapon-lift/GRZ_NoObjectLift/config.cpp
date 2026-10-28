class CfgPatches
{
    class GRZ_NoObjectLift
    {
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
        requiredAddons[] = {"DZ_Scripts", "GRZ_Movement"};
    };
};
class CfgMods
{
    class GRZ_NoObjectLift
    {
        dir = "GRZ_NoObjectLift";
        name = "GRZ_NoObjectLift - staged stability test";
        type = "mod";
        dependencies[] = {"World"};
        class defs
        {
            class worldScriptModule
            {
                value = "";
                files[] = {"GRZ_NoObjectLift/scripts/4_World"};
            };
        };
    };
};
