//
// Generated by dtk
// Translation Unit: d_a_tag_volcano.cpp
//

#include "d/actor/d_a_tag_volcano.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_priority.h"

namespace daTagvolcano {
    namespace {
        struct Attr_c {
            /* 0x00 */ u16 field_0x00;
        };

        static const Attr_c L_attr = {
            0,
        };

        inline const Attr_c & attr() { return L_attr; }
    }
}

/* 00000078-000002B4       .text _create__Q212daTagvolcano5Act_cFv */
cPhs_State daTagvolcano::Act_c::_create() {
    fopAcM_SetupActor(this, Act_c);

    field_0x298 = 0;
    field_0x299 = 0;
    mType = prm_get_type();
    field_0x2a0 = 0;

    if (mType == 0) {
        if (current.roomNo == dIsleRoom_FireMountain_e) {
            if (dComIfGs_isEventBit(0x1902)) {
                fopAcM_onSwitch(this, prm_get_swSave());
            } else if (dComIfGs_getStartPoint() == 2 && current.roomNo == dComIfGs_getRestartRoomNo()) {
                field_0x2a0 = 10;
                fopAcM_onSwitch(this, prm_get_swSave());
            } else {
                fopAcM_offSwitch(this, prm_get_swSave());
            }
        } else {
            if (dComIfGs_isEventBit(0x1901)) {
                fopAcM_onSwitch(this, prm_get_swSave());
            } else if (dComIfGs_getStartPoint() == 2 && current.roomNo == dComIfGs_getRestartRoomNo()) {
                field_0x2a0 = 10;
                fopAcM_onSwitch(this, prm_get_swSave());
            } else {
                fopAcM_offSwitch(this, prm_get_swSave());
            }
        }
    }

    field_0x2a4 = 0;
    return cPhs_COMPLEATE_e;
}

/* 000002B4-00000338       .text check_timer_clear__Q212daTagvolcano5Act_cFv */
bool daTagvolcano::Act_c::check_timer_clear() {
    if (dComIfGp_isEnableNextStage()) {
        if (strcmp(dComIfGp_getNextStageName(), "MiniKaz") == 0 || strcmp(dComIfGp_getNextStageName(), "MiniHyo") == 0)
            return false;
        else
            return true;
    } else {
        return true;
    }
}

/* 00000338-000003BC       .text _delete__Q212daTagvolcano5Act_cFv */
bool daTagvolcano::Act_c::_delete() {
    if (check_timer_clear() && dComIfG_getTimerMode() == 3)
        dComIfG_TimerDeleteRequest();
    fopAcM_offSwitch(this, prm_get_swSave());
    return true;
}

/* 000003BC-00000998       .text _execute__Q212daTagvolcano5Act_cFv */
bool daTagvolcano::Act_c::_execute() {
    if (mType == 0) {
        if (fopAcM_isSwitch(this, prm_get_swSave())) {
            if (field_0x2a0 > 0) {
                field_0x2a0--;
                if (field_0x2a0 == 0)
                    fopAcM_offSwitch(this, prm_get_swSave());
            } else {
                if (field_0x298 == 1) {
                    if (dComIfG_getTimerPtr() != NULL) {
                        if (field_0x299 == 0) {
                            dComIfG_TimerStart(3, 0);
                            field_0x299 = 1;
                        } else {
                            if (dComIfGp_event_runCheck()) {
                                if (dComIfG_getTimerMode() == 3)
                                    dComIfG_TimerStop(2);
                            } else {
                                if (dComIfG_getTimerMode() == 3)
                                    dComIfG_TimerReStart(2);
                            }

                            if (dComIfG_getTimerRestTimeMs() <= 0) {
                                mDoAud_seStart(JA_SE_ISLE_TIMER_0);
                                dComIfGp_getVibration().StartShock(6, -33, cXyz(0.0f, 1.0f, 0.0f));
                                fopAcM_offSwitch(this, prm_get_swSave());
                                if (dComIfG_getTimerMode() == 3)
                                    dComIfG_TimerDeleteRequest();

                                field_0x298 = 0;
                                field_0x299 = 0;
                            }
                        }
                    }
                } else {
                    u16 timer = prm_get_timer() * 10;
                    if (current.roomNo == dIsleRoom_FireMountain_e) {
                        if (dComIfGp_evmng_endCheck("FREEZE_VOLCANO")) {
                            dTimer_createTimer(3, timer, 1, 0, 221.0f, 439.0f, 32.0f, 419.0f);
                            field_0x298 = 1;
                        }
                    } else {
                        if (dComIfGp_evmng_endCheck("MELT_ICE")) {
                            dTimer_createTimer(3, timer, 1, 0, 221.0f, 439.0f, 32.0f, 419.0f);
                            field_0x298 = 1;
                        }
                    }
                }
            }
        }
    } else {
        if (dComIfGs_isTbox(prm_get_bitTRB())) {
            if (dComIfG_getTimerPtr() != NULL && dComIfG_getTimerMode() == 3)
                dComIfG_TimerDeleteRequest();
            if (mType == 1)
                dComIfGs_onEventBit(0x1902);
            else
                dComIfGs_onEventBit(0x1901);
            field_0x2a4 = 0;
        } else if (dComIfG_getTimerPtr() != NULL) {
            if (dComIfGp_event_runCheck()) {
                if (dComIfG_getTimerMode() == 3)
                    dComIfG_TimerStop(2);
            } else {
                if (dComIfG_getTimerMode() == 3)
                    dComIfG_TimerReStart(2);
            }

            if (dComIfG_getTimerRestTimeMs() <= 0) {
                mDoAud_seStart(JA_SE_ISLE_TIMER_0);
                dComIfGp_getVibration().StartShock(6, -33, cXyz(0.0f, 1.0f, 0.0f));
                if (dComIfG_getTimerMode() == 3)
                    dComIfG_TimerDeleteRequest();

                field_0x2a4 = 1;
                fopAcM_orderOtherEvent(this, "TAG_VOLCANO");
            }
#if VERSION > VERSION_JPN
        } else {
            mDoAud_seStart(JA_SE_ISLE_TIMER_0);
            dComIfGp_getVibration().StartShock(6, -33, cXyz(0.0f, 1.0f, 0.0f));
            field_0x2a4 = 1;
            fopAcM_orderOtherEvent(this, "TAG_VOLCANO");
#endif
        }

        if (field_0x2a4 == 1) {
            if (eventInfo.checkCommandDemoAccrpt()) {
                if (mType == 1) {
                    dComIfGp_setNextStage("sea", 2, 20);
                } else {
                    dComIfGp_setNextStage("sea", 2, 40);
                }
            } else {
                fopAcM_orderOtherEvent(this, "TAG_VOLCANO");
            }
        }
    }

    return true;
}

/* 00000998-000009A0       .text _draw__Q212daTagvolcano5Act_cFv */
bool daTagvolcano::Act_c::_draw() {
    return true;
}

namespace daTagvolcano {
    namespace {
        cPhs_State Mthd_Create(void* i_this) {
            return ((Act_c*)i_this)->_create();
        }

        BOOL Mthd_Delete(void* i_this) {
            return ((Act_c*)i_this)->_delete();
        }

        BOOL Mthd_Execute(void* i_this) {
            return ((Act_c*)i_this)->_execute();
        }

        BOOL Mthd_Draw(void* i_this) {
            return ((Act_c*)i_this)->_draw();
        }

        BOOL Mthd_IsDelete(void* i_this) {
            return TRUE;
        }

        static actor_method_class Mthd_Table = {
            (process_method_func)Mthd_Create,
            (process_method_func)Mthd_Delete,
            (process_method_func)Mthd_Execute,
            (process_method_func)Mthd_IsDelete,
            (process_method_func)Mthd_Draw,
        };
    }
}

actor_process_profile_definition g_profile_Tag_Volcano = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0002,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_Tag_Volcano,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daTagvolcano::Act_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_Tag_Volcano,
    /* Actor SubMtd */ &daTagvolcano::Mthd_Table,
    /* Status       */ fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_0_e,
};
