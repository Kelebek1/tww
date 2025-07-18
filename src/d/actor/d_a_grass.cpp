//
// Generated by dtk
// Translation Unit: d_a_grass.cpp
//

#include "d/actor/d_a_grass.h"
#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_com_inf_game.h"
#include "d/d_flower.h"
#include "d/d_grass.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/d_tree.h"
#include "SSystem/SComponent/c_sxyz.h"

#include "weak_bss_936_to_1036.h" // IWYU pragma: keep

namespace daGrass_prm {
    inline s8 getItemNo(grass_class* ac) { return (fopAcM_GetParam(ac) >> 6) & 0x3F; }
    inline u8 getKind(grass_class* ac) { return (fopAcM_GetParam(ac) >> 4) & 0x03; }
    inline u8 getType(grass_class* ac) { return (fopAcM_GetParam(ac) >> 0) & 0x0F; }
};

/* 000000EC-000000F4       .text daGrass_IsDelete__FP11grass_class */
static BOOL daGrass_IsDelete(grass_class*) {
    return TRUE;
}

/* 000000F4-000000FC       .text daGrass_Delete__FP11grass_class */
static BOOL daGrass_Delete(grass_class*) {
    return TRUE;
}

/* 000000FC-000014E0       .text daGrass_Create__FP10fopAc_ac_c */
static cPhs_State daGrass_Create(fopAc_ac_c* i_ac) {
    static const csXyz l_setType0[] = {
        csXyz(0, 0, 0),
        csXyz(3, 0, -50),
        csXyz(-2, 0, 50),
        csXyz(50, 0, 27),
        csXyz(52, 0, -25),
        csXyz(-50, 0, 22),
        csXyz(-50, 0, -29),
    };
    static const csXyz l_setType1[] = {
        csXyz(-18, 0, 76),
        csXyz(-15, 0, 26),
        csXyz(133, 0, 0),
        csXyz(80, 0, 23),
        csXyz(86, 0, -83),
        csXyz(33, 0, -56),
        csXyz(83, 0, -27),
        csXyz(-120, 0, -26),
        csXyz(-18, 0, -74),
        csXyz(-20, 0, -21),
        csXyz(-73, 0, 1),
        csXyz(-67, 0, -102),
        csXyz(-21, 0, 126),
        csXyz(-120, 0, -78),
        csXyz(-70, 0, -49),
        csXyz(32, 0, 103),
        csXyz(34, 0, 51),
        csXyz(-72, 0, 98),
        csXyz(-68, 0, 47),
        csXyz(33, 0, -5),
        csXyz(135, 0, -53),
    };
    static const csXyz l_setType2[] = {
        csXyz(-75, 0, -50),
        csXyz(75, 0, -25),
        csXyz(14, 0, 106),
    };
    static const csXyz l_setType3[] = {
        csXyz(-24, 0, -28),
        csXyz(27, 0, -28),
        csXyz(-21, 0, 33),
        csXyz(-18, 0, -34),
        csXyz(44, 0, -4),
        csXyz(41, 0, 10),
        csXyz(24, 0, 39),
    };
    static const csXyz l_setType4[] = {
        csXyz(-55, 0, -22),
        csXyz(-28, 0, -50),
        csXyz(-77, 0, 11),
        csXyz(55, 0, -44),
        csXyz(83, 0, -71),
        csXyz(11, 0, -48),
        csXyz(97, 0, -34),
        csXyz(-74, 0, -57),
        csXyz(31, 0, 58),
        csXyz(59, 0, 30),
        csXyz(13, 0, 23),
        csXyz(-12, 0, 54),
        csXyz(55, 0, 97),
        csXyz(10, 0, 92),
        csXyz(33, 0, -10),
        csXyz(-99, 0, -27),
        csXyz(40, 0, -87),
    };
    static const csXyz l_setType5[] = {
        csXyz(0, 0, 3),
        csXyz(-26, 0, -29),
        csXyz(7, 0, -25),
        csXyz(31, 0, -5),
        csXyz(-7, 0, 40),
        csXyz(-35, 0, 15),
        csXyz(23, 0, 32),
    };
    static const csXyz l_setType6[] = {
        csXyz(-40, 0, 0),
        csXyz(0, 0, 0),
        csXyz(80, 0, 0),
        csXyz(-80, 0, 0),
        csXyz(40, 0, 0),
    };

    static struct OffsetData {
        u8 num;
        const csXyz * pos;
    } l_offsetData[] = {
        { 1, l_setType0 },
        { 7, l_setType0 },
        { 21, l_setType1 },
        { 3, l_setType2 },
        { 7, l_setType3 },
        { 17, l_setType4 },
        { 7, l_setType5 },
        { 5, l_setType6 },
    };

    grass_class * i_this = (grass_class*)i_ac;

    fopAcM_SetupActor(i_this, grass_class);

    u32 grp = daGrass_prm::getType(i_this);
    OffsetData * offset = &l_offsetData[grp];
    const csXyz * ofpos = offset->pos;
    u32 kind = daGrass_prm::getKind(i_this);
    cXyz * acpos = &i_this->current.pos;

    if (kind == 0) {
        // grass
        if (dComIfGp_createGrass() != NULL) {
            s8 item = daGrass_prm::getItemNo(i_this);
            if (item < 0x20 || item >= 0x40)
                item = -1;

            for (s32 i = 0; i < offset->num; ofpos++, i++) {
                cXyz pos;
                pos.x = acpos->x + ofpos->x;
                pos.y = acpos->y;
                pos.z = acpos->z + ofpos->z;
                dComIfGp_getGrass()->newData(pos, fopAcM_GetRoomNo(i_this), item);
            }
        }
    } else if (kind == 1) {
        // tree
        if (dComIfGp_createTree() != NULL) {
            f32 cosR = cM_scos(i_this->current.angle.y), sinR = cM_ssin(i_this->current.angle.y);
            cXyz pos;
            pos.y = acpos->y;

            for (s32 i = 0; i < offset->num; ofpos++, i++) {
                pos.x = acpos->x + (ofpos->x * cosR + ofpos->z * sinR);
                pos.y = acpos->y;
                pos.z = acpos->z + (ofpos->z * cosR - ofpos->x * sinR);
                dComIfGp_getTree()->newData(pos, 0, fopAcM_GetRoomNo(i_this));
            }
        }
    } else if (kind == 2 || kind == 3) {
        // white flower, pink flower
        if (dComIfGp_createFlower() != NULL) {
            s8 item = daGrass_prm::getItemNo(i_this);
            if (item < 0x20 || item >= 0x40)
                item = 0xFF;

            s8 flowerType;
            if (kind == 2)
                flowerType = 1;
            else if (kind == 3)
                flowerType = 2;

            for (s32 i = 0; i < offset->num; ofpos++, i++) {
                cXyz pos;
                pos.x = acpos->x + ofpos->x;
                pos.y = acpos->y;
                pos.z = acpos->z + ofpos->z;
                dComIfGp_getFlower()->newData(flowerType, pos, fopAcM_GetRoomNo(i_this), item);
            }
        }
    }

    return cPhs_ERROR_e;
}

static actor_method_class daGrass_METHODS = {
    (process_method_func)daGrass_Create,
    (process_method_func)daGrass_Delete,
    (process_method_func)NULL,
    (process_method_func)daGrass_IsDelete,
    (process_method_func)NULL,
};

actor_process_profile_definition g_profile_GRASS = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0007,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_GRASS,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(grass_class),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_GRASS,
    /* Actor SubMtd */ &daGrass_METHODS,
    /* Status       */ fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_0_e,
};
