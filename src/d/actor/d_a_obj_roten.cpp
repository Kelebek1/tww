/**
 * d_a_obj_roten.cpp
 * Object - Zunari's Shop models (wooden posts and curtains on top)
 */

#include "d/actor/d_a_obj_roten.h"
#include "d/res/res_roten.h"
#include "m_Do/m_Do_mtx.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_procname.h"
#include "d/d_priority.h"

daObj_Roten_HIO_c l_HIO;

/* 000000EC-00000118       .text __ct__17daObj_Roten_HIO_cFv */
daObj_Roten_HIO_c::daObj_Roten_HIO_c() {
    mNo = -1;
    field_0x08 = 0.0f;
    field_0x0C = 0;
}

const char daObj_Roten_c::M_arcname[] = "Roten";

/* 00000118-00000188       .text set_mtx__13daObj_Roten_cFv */
void daObj_Roten_c::set_mtx() {
    mpModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00000188-000001A8       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daObj_Roten_c*>(i_this)->CreateHeap();
}

/* 000001A8-000002E4       .text CreateHeap__13daObj_Roten_cFv */
BOOL daObj_Roten_c::CreateHeap() {
    static u32 bdl_arc_idx[] = {
        ROTEN_BDL_ROTEN02,
        ROTEN_BDL_ROTEN03,
        ROTEN_BDL_ROTEN04
    };
    static u32 dzb_arc_idx[] = {
        ROTEN_DZB_ROTEN02,
        ROTEN_DZB_ROTEN03,
        ROTEN_DZB_ROTEN04
    };

    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(M_arcname, bdl_arc_idx[mType]));
    JUT_ASSERT(0xB0, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0, 0x11020203);
    mpBgW = new dBgW();
    
    mpBgW->Set((cBgD_t*)dComIfG_getObjectRes(M_arcname, dzb_arc_idx[mType]), cBgW::MOVE_BG_e, &mpModel->getBaseTRMtx());

    return mpModel != NULL && mpBgW != NULL;
}

/* 000002E4-00000348       .text CreateInit__13daObj_Roten_cFv */
cPhs_State daObj_Roten_c::CreateInit() {
    set_mtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);
    dComIfG_Bgsp()->Regist(mpBgW, this);

    return cPhs_COMPLEATE_e;
}

/* 00000348-00000410       .text Roten_create_check__FUc */
static BOOL Roten_create_check(u8 type) {
    u8 i = 0; // fakematch?
    int temp = 0;
    for(; i <= 0xB; i++) {
        if(dComIfGs_isGetItemReserve(i)) {
            temp++;
        }
        else if(i == 0) {
            temp++;
        }
    }

    if(type == 0 && temp > 3) {
        return true;
    }

    if(type == 1 && temp > 6) {
        return true;
    }

    if(temp > 9) {
        return true;
    }

    return false;
}


cPhs_State daObj_Roten_c::_create() {
    fopAcM_SetupActor(this, daObj_Roten_c);

    mType = fopAcM_GetParam(this) >> 0x18;
    mType = cLib_minMaxLimit<u8>(mType, 0, 2);

    if(Roten_create_check(mType)) {
        field_0x2D1 = 0;
    }
    else {
        field_0x2D1 = 1;
        return cPhs_ERROR_e;
    }
    cPhs_State result = dComIfG_resLoad(&mPhs, daObj_Roten_c::M_arcname);

    u32 heap_size[] = {
        0x2BA0,
        0x2A60,
        0x2C60,
    };

    if(result == cPhs_COMPLEATE_e) {
        if(fopAcM_entrySolidHeap(this, CheckCreateHeap, heap_size[mType])) {
            result = CreateInit();
        }
        else {
            result = cPhs_ERROR_e;
        }
    }

    return result;
}

/* 00000410-00000530       .text daObj_RotenCreate__FPv */
static cPhs_State daObj_RotenCreate(void* i_actor) {
    return static_cast<daObj_Roten_c*>(i_actor)->_create();
}

bool daObj_Roten_c::_delete() {
    if(field_0x2D1 == 0) {
        dComIfG_Bgsp()->Release(mpBgW);
        dComIfG_resDelete(&mPhs, daObj_Roten_c::M_arcname);
    }

    return true;
}

/* 00000530-0000058C       .text daObj_RotenDelete__FPv */
static BOOL daObj_RotenDelete(void* i_actor) {
    return static_cast<daObj_Roten_c*>(i_actor)->_delete();
}

bool daObj_Roten_c::_execute() {
    set_mtx();
    mpBgW->Move();

    return false;
}

/* 0000058C-000005C4       .text daObj_RotenExecute__FPv */
static BOOL daObj_RotenExecute(void* i_actor) {
    return static_cast<daObj_Roten_c*>(i_actor)->_execute();
}

bool daObj_Roten_c::_draw() {
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);

    return true;
}

/* 000005C4-00000630       .text daObj_RotenDraw__FPv */
static BOOL daObj_RotenDraw(void* i_actor) {
    return static_cast<daObj_Roten_c*>(i_actor)->_draw();
}

/* 00000630-00000638       .text daObj_RotenIsDelete__FPv */
static BOOL daObj_RotenIsDelete(void*) {
    return true;
}

static actor_method_class daObj_RotenMethodTable = {
    (process_method_func)daObj_RotenCreate,
    (process_method_func)daObj_RotenDelete,
    (process_method_func)daObj_RotenExecute,
    (process_method_func)daObj_RotenIsDelete,
    (process_method_func)daObj_RotenDraw,
};

actor_process_profile_definition g_profile_Obj_Roten = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Roten,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObj_Roten_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Roten,
    /* Actor SubMtd */ &daObj_RotenMethodTable,
    /* Status       */ fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_4_e,
};
