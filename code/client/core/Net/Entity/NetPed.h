
#include <librg.h>
#include <m2o_types.h>
#include "m2o_config.h"

class C_Player2;
class C_Human2;
class C_Entity;
class C_Command;

namespace nmd::net
{
    class NetPed : public m2o_ped
    {
       // gfx_handle nickname_value;
       // gfx_handle healthbar_base;
       // gfx_handle healthbar_value;

        struct 
        {
            bool init;
            int counter;
            C_Command *stand;
            C_Command *movedir;

        } tasks;

        struct {
            struct {
                vec3 start;
                vec3 target;
                f32 lastAlpha;
                f64 startTime;
                f64 finishTime;
            } pos;
        } interp;

        C_Player2 *CPlayer2; /* watch out, not all peds are players */
        C_Human2 *CHuman2;
        C_Entity *CEntity;

    public:

        NetPed();
    };

} // namespace nmd::net
