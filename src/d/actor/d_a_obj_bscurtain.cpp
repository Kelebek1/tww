/**
 * d_a_obj_bscurtain.cpp
 * Object - Curtains (Beedle's Shop)
 */

#include "d/actor/d_a_obj_bscurtain.h"
#include "d/res/res_ptc.h"
#include "f_op/f_op_actor_mng.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"

#include "weak_bss_936_to_1036.h" // IWYU pragma: keep

daObj_Bscurtain_HIO_c l_HIO;

/* 000000EC-00000118       .text __ct__21daObj_Bscurtain_HIO_cFv */
daObj_Bscurtain_HIO_c::daObj_Bscurtain_HIO_c() {
    mNo = -1;
    field_0x08 = 0.0f;
    field_0x0c = 0;
}

const char daObj_Bscurtain_c::M_arcname[4] = "Ptc";

/* 00000118-000001C8       .text set_mtx__17daObj_Bscurtain_cFv */
void daObj_Bscurtain_c::set_mtx() {
    static cXyz trans = cXyz::Zero;
    mDoMtx_stack_c::transS(trans);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00000204-00000224       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daObj_Bscurtain_c*)i_this)->CreateHeap();
}

/* 00000224-00000334       .text CreateHeap__17daObj_Bscurtain_cFv */
BOOL daObj_Bscurtain_c::CreateHeap() {
    J3DModelData* modelData;
    if ((fpcM_GetParam(this) & 0xFF) != 1 || dComIfGs_isEventBit(0x1F08) != 0) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(M_arcname, PTC_INDEX_BDL_PTCO);
    } else {
        modelData = (J3DModelData*)dComIfG_getObjectRes(M_arcname, PTC_INDEX_BDL_PTCU);
    }

    JUT_ASSERT(0xa9, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x0, 0x11020203);

    return mpModel != NULL;
}

/* 00000334-00000370       .text CreateInit__17daObj_Bscurtain_cFv */
cPhs_State daObj_Bscurtain_c::CreateInit() {
    set_mtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    return cPhs_COMPLEATE_e;
}

cPhs_State daObj_Bscurtain_c::_create() {
    fopAcM_SetupActor(this, daObj_Bscurtain_c);

    cPhs_State ret = dComIfG_resLoad(&mPhs, M_arcname);

    if (ret == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x10000)) {
            ret = CreateInit();
        } else {
            ret = cPhs_ERROR_e;
        }
    }

    return ret;
}

bool daObj_Bscurtain_c::_delete() {
    dComIfG_resDelete(&mPhs, M_arcname);
    return true;
}

bool daObj_Bscurtain_c::_execute() {
    set_mtx();
    return false;
}

bool daObj_Bscurtain_c::_draw() {
    dComIfGd_setListBG();
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return true;
}

/* 00000370-00000400       .text daObj_BscurtainCreate__FPv */
static cPhs_State daObj_BscurtainCreate(void* i_this) {
    return ((daObj_Bscurtain_c*)i_this)->_create();
}

/* 00000400-00000430       .text daObj_BscurtainDelete__FPv */
static BOOL daObj_BscurtainDelete(void* i_this) {
    return ((daObj_Bscurtain_c*)i_this)->_delete();
}

/* 00000430-00000454       .text daObj_BscurtainExecute__FPv */
static BOOL daObj_BscurtainExecute(void* i_this) {
    return ((daObj_Bscurtain_c*)i_this)->_execute();
}

/* 00000454-000004F8       .text daObj_BscurtainDraw__FPv */
static BOOL daObj_BscurtainDraw(void* i_this) {
    return ((daObj_Bscurtain_c*)i_this)->_draw();
}

/* 000004F8-00000500       .text daObj_BscurtainIsDelete__FPv */
static BOOL daObj_BscurtainIsDelete(void* i_this) {
    return TRUE;
}

static actor_method_class daObj_BscurtainMethodTable = {
    (process_method_func)daObj_BscurtainCreate,
    (process_method_func)daObj_BscurtainDelete,
    (process_method_func)daObj_BscurtainExecute,
    (process_method_func)daObj_BscurtainIsDelete,
    (process_method_func)daObj_BscurtainDraw,
};

actor_process_profile_definition g_profile_Obj_Bscurtain = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0003,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Obj_Bscurtain,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObj_Bscurtain_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Obj_Bscurtain,
    /* Actor SubMtd */ &daObj_BscurtainMethodTable,
    /* Status       */ fopAcStts_NOCULLEXEC_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_4_e,
};
