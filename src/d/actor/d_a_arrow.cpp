/**
 * d_a_arrow.cpp
 * Item - Arrow
 */

#include "d/actor/d_a_arrow.h"
#include "m_Do/m_Do_mtx.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_priority.h"
#include "d/d_item_data.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_player_main.h"
#include "d/actor/d_a_pz.h"
#include "d/d_jnt_hit.h"
#include "d/d_s_play.h"
#include "d/res/res_link.h"

#include "weak_data_1811.h" // IWYU pragma: keep

s16 daArrow_c::m_count;

const char daArrow_c::m_arc_name[] = "Link";

const dCcD_SrcCps daArrow_c::m_at_cps_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ AT_TYPE_NORMAL_ARROW,
        /* SrcObjAt  Atp     */ 2,
        /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsEnemy_e | cCcD_AtSPrm_VsOther_e,
        /* SrcObjTg  Type    */ 0,
        /* SrcObjTg  SPrm    */ 0,
        /* SrcObjCo  SPrm    */ 0,
        /* SrcGObjAt Se      */ dCcG_SE_ARROW,
        /* SrcGObjAt HitMark */ dCcG_AtHitMark_Nrm_e,
        /* SrcGObjAt Spl     */ 0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ 0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ 0,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGCpsS
    {
        /* Start  */ 0.0f, 0.0f, 0.0f,
        /* End    */ 0.0f, 0.0f, 0.0f,
        /* Radius */ 5.0f,
    },
};

const dCcD_SrcSph daArrow_c::m_co_sph_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ 0,
        /* SrcObjTg  SPrm    */ 0,
        /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsEnemy_e | cCcD_CoSPrm_NoCrr_e,
        /* SrcGObjAt Se      */ dCcG_SE_ARROW,
        /* SrcGObjAt HitMark */ 0,
        /* SrcGObjAt Spl     */ 0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ 0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoHitMark_e,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGSphS
    {
        /* Center */ 0.0f, 0.0f, 0.0f,
        /* Radius */ 25.0f,
    },
};

/* 800D455C-800D457C       .text createHeap_CB__FP10fopAc_ac_c */
static BOOL createHeap_CB(fopAc_ac_c* i_this) {
    return static_cast<daArrow_c*>(i_this)->_createHeap();
}

/* 800D457C-800D4648       .text _createHeap__9daArrow_cFv */
BOOL daArrow_c::_createHeap() {
    u16 modelFileIndex;
    if (mArrowType == TYPE_LIGHT) {
        modelFileIndex = LINK_BDL_ARROWGLITTER;
    } else {
        modelFileIndex = LINK_BDL_ARROW;
    }
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(m_arc_name, modelFileIndex);
    JUT_ASSERT(190, modelData != NULL);
    
    mpModel = mDoExt_J3DModel__create(modelData, 0x00080000, 0x11000022);
    if (!mpModel) {
        return FALSE;
    }
    return TRUE;
}

/* 800D4648-800D4668       .text atHit_CB__FP10fopAc_ac_cP12dCcD_GObjInfP10fopAc_ac_cP12dCcD_GObjInf */
static void atHit_CB(fopAc_ac_c* i_this, dCcD_GObjInf* thisObjInf, fopAc_ac_c* hitActor, dCcD_GObjInf* hitObjInf) {
    static_cast<daArrow_c*>(i_this)->_atHit(thisObjInf, hitActor, hitObjInf);
}

/* 800D4668-800D47C0       .text _atHit__9daArrow_cFP12dCcD_GObjInfP10fopAc_ac_cP12dCcD_GObjInf */
void daArrow_c::_atHit(dCcD_GObjInf* thisObjInf, fopAc_ac_c* hitActor, dCcD_GObjInf* hitObjInf) {
    // Keep track of which actor this arrow hit as well as the position the hit occurred at.
    if (!hitActor) {
        return;
    }
    if (!fopAc_IsActor(hitActor)) {
        return;
    }
    if (!fopAcM_GetJntHit(hitActor)) {
        return;
    }
    
    cXyz hitPos(*thisObjInf->GetAtHitPosP());
    f32 hitDist = (hitPos - current.pos).abs();
    if (hitDist < mNearestHitDist) {
        mNearestHitDist = hitDist;
        mHitActorProcID = fopAcM_GetID(hitActor);
        if (hitObjInf->ChkTgShield()) {
            clrAtHitNormal();
            setAtHitNormalActor(NULL);
        } else {
            setAtHitNormal();
            setAtHitNormalActor(hitActor);
        }
        setAtHitPosBuff(thisObjInf->GetAtHitPosP());
    }
}

/* 800D47C0-800D4814       .text checkCreater__9daArrow_cFv */
void daArrow_c::checkCreater() {
    // Check if this arrow was fired by Princess Zelda (during the Ganondorf fight).
    fopAc_ac_c* archer;
    if (fopAcM_SearchByID(fopAcM_GetLinkId(this), &archer)) {
        if (fpcM_GetName(archer) == PROC_PZ) {
            mbSetByZelda = true;
        }
    }
}

/* 800D4814-800D48E8       .text setLightEffect__9daArrow_cFv */
void daArrow_c::setLightEffect() {
    if (field_0x682 == mArrowType) {
        if (mArrowType == TYPE_NORMAL) {
            return;
        }
        if (!mbHasLightEff) {
            mLightEffPID = fopAcM_createChild(
                PROC_ARROW_LIGHTEFF, fopAcM_GetID(this),
                mArrowType, &field_0x6a8,
                current.roomNo, &shape_angle
            );
            if (mLightEffPID != fpcM_ERROR_PROCESS_ID_e) {
                mbHasLightEff = true;
            }
        }
    } else {
        fopAcM_delete(fopAcM_SearchByID(mLightEffPID));
        mbHasLightEff = false;
    }
    field_0x682 = mArrowType;
}

/* 800D48E8-800D4994       .text setBlur__9daArrow_cFv */
void daArrow_c::setBlur() {
    JPABaseEmitter* blurEmitter = mBlurFollowCb.getEmitter();
    if (!blurEmitter) {
        return;
    }
    s32 alpha = blurEmitter->getGlobalAlpha();
    if (alpha - 50 <= 0) {
        mBlurFollowCb.remove();
    } else {
        blurEmitter->setGlobalAlpha(alpha - 50);
    }
    
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mBlurAngle);
    blurEmitter->setGlobalRTMatrix(mDoMtx_stack_c::get());
}

/* 800D4994-800D4A04       .text createBlur__9daArrow_cFv */
void daArrow_c::createBlur() {
    if (!mBlurFollowCb.getEmitter()) {
        dComIfGp_particle_setP1(dPa_name::ID_COMMON_0048, &current.pos, NULL, NULL, 0xFF, &mBlurFollowCb);
    }
}

/* 800D4A04-800D4ADC       .text setArrowShootSe__9daArrow_cFv */
void daArrow_c::setArrowShootSe() {
    static const s32 se[8] = {
        0,
        0,
        JA_SE_LK_FIT_FIRE_ARROW,
        JA_SE_LK_SHOOT_FIRE_ARROW,
        JA_SE_LK_FIT_ICE_ARROW,
        JA_SE_LK_SHOOT_ICE_ARROW,
        JA_SE_LK_FIT_LIGHT_ARROW,
        JA_SE_LK_SHOOT_LIGHT_ARROW,
    };
    
    if (mArrowType == TYPE_NORMAL) {
        return;
    }
    
    fopAcM_seStartCurrent(this, se[mArrowType*2+0], 0);
    
    fopAcM_seStartCurrent(this, se[mArrowType*2+1], 0);
}

/* 800D4ADC-800D4B70       .text setDrawShapeMaterial__9daArrow_cFv */
void daArrow_c::setDrawShapeMaterial() {
    struct ArrowAttackInfo {
        u32 mAtType;
        u8 mAtp;
        u16 mTipJointIdx;
    };
    static const ArrowAttackInfo arrow_mat[] = {
        {
            /* mAtType      */ AT_TYPE_NORMAL_ARROW,
            /* mAtp         */ 2,
            /* mTipJointIdx */ 4,
        },
        {
            /* mAtType      */ AT_TYPE_FIRE_ARROW,
            /* mAtp         */ 4,
            /* mTipJointIdx */ 2,
        },
        {
            /* mAtType      */ AT_TYPE_ICE_ARROW,
            /* mAtp         */ 4,
            /* mTipJointIdx */ 3,
        },
        {
            /* mAtType      */ AT_TYPE_LIGHT_ARROW,
            /* mAtp         */ 0xFF,
            /* mTipJointIdx */ 0,
        },
    };
    
    mAtCps.SetAtType(arrow_mat[mArrowType].mAtType);
    mAtCps.SetAtAtp(arrow_mat[mArrowType].mAtp);
    
    if (mbSetByZelda) {
        mAtCps.SetAtAtp(4);
        mAtCps.SetAtType(arrow_mat[0].mAtType);
        mAtCps.OnAtSPrmBit(cCcD_AtSPrm_GrpAll_e);
    }
    
    if (arrow_mat[mArrowType].mTipJointIdx != 0) {
        J3DModelData* modelData = mpModel->getModelData();
        J3DJoint* tipJoint = modelData->getJointNodePointer(arrow_mat[mArrowType].mTipJointIdx);
        mpTipMat = tipJoint->getMesh();
    }
}

/* 800D4B70-800D4D98       .text arrowShooting__9daArrow_cFv */
void daArrow_c::arrowShooting() {
    field_0x6a8 = current.pos;
    f32 xCos = cM_scos(current.angle.x);
    speed.x = 200.0f * cM_ssin(current.angle.y) * xCos;
    speed.y = 200.0f * cM_ssin(current.angle.x);
    speed.z = 200.0f * cM_scos(current.angle.y) * xCos;
    
    setArrowShootSe();
    
    if (mArrowType == TYPE_LIGHT && !mbSetByZelda) {
        if (strcmp(dComIfGp_getStartStageName(), "GanonK") != 0) {
            // Not in Puppet Ganon's boss room.
            mAtCps.SetAtSpl(dCcG_At_Spl_UNKB);
        }
    }
    
    mBlurAngle = shape_angle;
    
    createBlur();
    
    field_0x602 = m_count;
    m_count += 1;
    if (m_count == 5) {
        m_count = 0;
    }
    
    cXyz end = current.pos + speed*1.25f;
    mAtCps.SetStartEnd(current.pos, end);
    mAtCps.SetR(5.0f);
    mAtCps.CalcAtVec();
    
    dComIfG_Ccsp()->Set(&mAtCps);
    dComIfG_Ccsp_SetMass(&mAtCps, 1);
    
    clrAtHitNormal();
    setAtHitPosBuff(&end);
    mNearestHitDist = FLOAT_MAX;
}

/* 800D4D98-800D4DC0       .text arrowUseMp__9daArrow_cFv */
void daArrow_c::arrowUseMp() {
    static const s16 use_mp[4] = {
        -0, // Normal Arrows
        -1, // Fire Arrows
        -1, // Ice Arrows
        -2, // Light Arrows
    };
    
    dComIfGp_setItemMagicCount(use_mp[mArrowType]);
}

/* 800D4DC0-800D50A0       .text ShieldReflect__9daArrow_cFv */
void daArrow_c::ShieldReflect() {
    f32 vel = speed.abs();
    
    daPy_lk_c* link = daPy_getPlayerLinkActorClass();
    s16 targetAngleY = link->shape_angle.y + link->getBodyAngleY();
    s16 targetAngleX = link->getBodyAngleX();
    
    fopAc_ac_c* ganondorf;
    if (fopAcM_SearchByName(PROC_GND, &ganondorf)) {
        dAttention_c& attention = dComIfGp_getAttention();
        if (attention.LockonTruth() && attention.LockonTarget(0) == ganondorf) {
            cXyz ganondorfChestPos = ganondorf->current.pos;
#if VERSION == VERSION_DEMO
            ganondorfChestPos.y += 130.0f + REG8_F(0);
#else
            ganondorfChestPos.y = 130.0f + REG8_F(0);
#endif
            targetAngleX = -cLib_targetAngleX(&link->current.pos, &ganondorfChestPos);
#if VERSION > VERSION_DEMO
            fopAcM_SetParam(ganondorf, 0x23);
            mSparkleTimer = 15 + REG0_S(3);
            mpSparkleEmitter = dComIfGp_particle_set(dPa_name::ID_COMMON_03EE, &link->current.pos);
#endif
        }
    }
    
    Vec norm;
    norm.x = cM_ssin(targetAngleY) * cM_scos(targetAngleX);
    norm.y = -cM_ssin(targetAngleX);
    norm.z = cM_scos(targetAngleY) * cM_scos(targetAngleX);
    speed.x = norm.x * vel;
    speed.y = norm.y * vel;
    speed.z = norm.z * vel;
    
    shape_angle.x = cM_atan2s(-speed.y, -speed.absXZ());
    shape_angle.y = cM_atan2s(-speed.x, -speed.z);
    shape_angle.z = 0;
}

/* 800D50A0-800D5388       .text check_water_in__9daArrow_cFv */
bool daArrow_c::check_water_in() {
    u8 prev_field_0x699 = field_0x699;
    field_0x699 = daPy_lk_c::setItemWaterEffect(this, field_0x699, 1);
    if (prev_field_0x699 == 0 && field_0x699 == 1) {
        f32 waterY;
        fopAcM_getWaterY(&current.pos, &waterY);
        
        f32 deltaY = fabs(old.pos.y - current.pos.y);
        f32 waterDist = fabs(waterY - current.pos.y);
        cXyz waterHitPos;
        if (deltaY < 1.0f) {
            waterHitPos = current.pos;
        } else {
            f32 weight = waterDist / deltaY;
            if (weight > 1.0f) {
                weight = 1.0f;
            }
            waterHitPos = (old.pos * weight) + (current.pos * (1.0f - weight));
        }
        
        mCurrProcFunc = &daArrow_c::procWater;
        fopAcM_SetParam(this, 4);
        
        if (mArrowType == TYPE_FIRE) {
            mInWaterTimer = 1;
            dComIfGp_particle_setP1(dPa_name::ID_COMMON_035A, &waterHitPos);
            if (!field_0x6e4) {
                dKy_arrowcol_chg_on(&current.pos, 0);
            }
        } else if (mArrowType == TYPE_ICE) {
            mInWaterTimer = 10*30;
            fopAcM_createChild(
                PROC_ARROW_ICEEFF, fopAcM_GetID(this), mArrowType,
                &waterHitPos, current.roomNo, &current.angle
            );
            if (!field_0x6e4) {
                dKy_arrowcol_chg_on(&current.pos, 1);
            }
        } else if (mArrowType == TYPE_LIGHT) {
            dComIfGp_particle_setP1(dPa_name::ID_COMMON_02A1, &waterHitPos);
            fopAcM_seStartCurrent(this, JA_SE_OBJ_LIGHT_ARW_EFF, 0);
            if (!field_0x6e4) {
                dKy_arrowcol_chg_on(&current.pos, 2);
            }
            mInWaterTimer = 1;
        } else {
            mInWaterTimer = 1;
        }
        
        field_0x698 = false;
        
        return TRUE;
    } else {
        return FALSE;
    }
}

/* 800D5388-800D53AC       .text changeArrowMp__9daArrow_cFv */
BOOL daArrow_c::changeArrowMp() {
    u8 magic = dComIfGs_getMagic();
    return magic >= 1;
}

/* 800D53AC-800D553C       .text changeArrowType__9daArrow_cFv */
daArrow_c* daArrow_c::changeArrowType() {
    u8 origArrowType = mArrowType;
    mBtkFrame = 0.0f;
    
    daArrow_c* ret = this;
    
    if (mArrowType == TYPE_NORMAL) {
        if (dComIfGs_getMagic() < 1 || !(dComIfGs_getItem(dInvSlot_BOW_e) == dItem_MAGIC_ARROW_e || dComIfGs_getItem(dInvSlot_BOW_e) == dItem_LIGHT_ARROW_e)) {
            mArrowType = TYPE_NORMAL;
        } else {
            mArrowType = TYPE_FIRE;
        }
    } else if (mArrowType == TYPE_FIRE) {
        if (dComIfGs_getMagic() < 1 || !(dComIfGs_getItem(dInvSlot_BOW_e) == dItem_MAGIC_ARROW_e || dComIfGs_getItem(dInvSlot_BOW_e) == dItem_LIGHT_ARROW_e)) {
            mArrowType = TYPE_NORMAL;
        } else {
            mArrowType = TYPE_ICE;
        }
    } else if (mArrowType == TYPE_ICE) {
        if (dComIfGs_getMagic() < 2 || dComIfGs_getItem(dInvSlot_BOW_e) != dItem_LIGHT_ARROW_e) {
            mArrowType = TYPE_NORMAL;
        } else {
            mArrowType = TYPE_LIGHT;
        }
    } else if (mArrowType == TYPE_LIGHT) {
        mArrowType = TYPE_NORMAL;
    }
    
    if (mArrowType != origArrowType) {
        m_keep_type = mArrowType;
        daArrow_c* newNockedArrow = (daArrow_c*)fopAcM_fastCreate(PROC_ARROW, 0, &current.pos, current.roomNo);
        if (!newNockedArrow) {
            mArrowType = origArrowType;
            m_keep_type = origArrowType;
            setDrawShapeMaterial();
            ret = this;
        } else {
            mArrowType = origArrowType;
            fopAcM_delete(this);
            ret = newNockedArrow;
        }
    }
    
    return ret;
}

/* 800D553C-800D560C       .text changeArrowTypeNotReady__9daArrow_cFv */
void daArrow_c::changeArrowTypeNotReady() {
    if (m_keep_type == TYPE_NORMAL) {
        if (dComIfGs_getMagic() < 1) {
            return;
        }
        if (dComIfGs_getItem(dInvSlot_BOW_e) == dItem_MAGIC_ARROW_e || dComIfGs_getItem(dInvSlot_BOW_e) == dItem_LIGHT_ARROW_e) {
            m_keep_type = TYPE_FIRE;
        }
    } else if (m_keep_type == TYPE_FIRE) {
        if (dComIfGs_getMagic() < 1) {
            return;
        }
        if (dComIfGs_getItem(dInvSlot_BOW_e) == dItem_MAGIC_ARROW_e || dComIfGs_getItem(dInvSlot_BOW_e) == dItem_LIGHT_ARROW_e) {
            m_keep_type = TYPE_ICE;
        }
    } else if (m_keep_type == TYPE_ICE) {
        if (dComIfGs_getMagic() >= 2 && dComIfGs_getItem(dInvSlot_BOW_e) == dItem_LIGHT_ARROW_e) {
            m_keep_type = TYPE_LIGHT;
        } else {
            m_keep_type = TYPE_NORMAL;
        }
    } else if (m_keep_type == TYPE_LIGHT) {
        m_keep_type = TYPE_NORMAL;
    }
}

/* 800D560C-800D56B0       .text setRoomInfo__9daArrow_cFv */
void daArrow_c::setRoomInfo() {
    u32 roomNo;
    
    mGndChk.SetPos(&current.pos);
    f32 groundY = dComIfG_Bgsp()->GroundCross(&mGndChk);
    if (groundY != -G_CM3D_F_INF) {
        roomNo = dComIfG_Bgsp()->GetRoomId(mGndChk);
        tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mGndChk);
    } else {
        roomNo = dComIfGp_roomControl_getStayNo();
    }
    
    tevStr.mRoomNo = roomNo;
    mStts.SetRoomId(roomNo);
    current.roomNo = roomNo;
}

/* 800D56B0-800D5854       .text setKeepMatrix__9daArrow_cFv */
void daArrow_c::setKeepMatrix() {
    // Transform the arrow onto its archer's hand.
    if (mbSetByZelda) {
        daPz_c* zelda;
        fopAcM_SearchByID(fopAcM_GetLinkId(this), (fopAc_ac_c**)&zelda);
        
        mDoMtx_stack_c::transS(0.7f, -0.07f, -0.2f);
        mDoMtx_stack_c::XYZrotM(0x238E, 0x2CDF, 0x29BE);
        mDoMtx_stack_c::revConcat(zelda->getRightHandMatrix());
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        
        MtxP mtx = mDoMtx_stack_c::get();
        current.pos.set(mtx[0][3], mtx[1][3], mtx[2][3]);
        
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &shape_angle);
        current.angle.y = shape_angle.y;
        current.angle.x = -shape_angle.x;
    } else {
        daPy_py_c* player = daPy_getPlayerActorClass();
        
        mDoMtx_stack_c::transS(7.6f, -0.8f, -0.5f);
        // This function takes three signed shorts, but one of the literals passed here is unsigned.
        // X rotation must be a float literal to force the compiler to pass an unsigned short.
        // Z rotation must be an int literal to pass a signed short as normal.
        mDoMtx_stack_c::XYZrotM((248.5f*65536)/360, 0x238E, -0x6333);
        mDoMtx_stack_c::revConcat(player->getLeftHandMatrix());
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        
        MtxP mtx = mDoMtx_stack_c::get();
        current.pos.set(mtx[0][3], mtx[1][3], mtx[2][3]);
        
        mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &shape_angle);
        current.angle.y = shape_angle.y;
        current.angle.x = -shape_angle.x;
    }
}

/* 800D5854-800D5A70       .text setStopActorMatrix__9daArrow_cFv */
void daArrow_c::setStopActorMatrix() {
    s16 xRot = 0;
    if (cLib_calcTimer(&field_0x604) != 0) {
        f32 temp = (field_0x604 / 40.0f);
        xRot = 1024.0f * temp*temp * cM_ssin(field_0x604 * 0x52FB);
    }
    fopAc_ac_c* hitActor = fopAcM_SearchByID(mHitActorProcID);
    if (!hitActor) {
        return;
    }
    JntHit_c* jntHit = fopAcM_GetJntHit(hitActor);
    if (!jntHit) {
        return;
    }
    J3DModel* hitModel = jntHit->getModel();
    
    static cXyz offset_arrow_pos(0.0f, 0.0f, -50.0f);
    
    mDoMtx_stack_c::copy(hitModel->getAnmMtx(mHitJointIndex));
    csXyz hitJointRot;
    mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &hitJointRot);
    
    mDoMtx_stack_c::transM(field_0x618);
    mDoMtx_stack_c::ZXYrotM(field_0x6e6);
    
    cXyz pos(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&pos);
    current.pos = pos;
    
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(hitJointRot);
    mDoMtx_stack_c::ZXYrotM(field_0x6e6);
    mDoMtx_stack_c::XrotM(xRot);
    mDoMtx_stack_c::transM(offset_arrow_pos);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 800D5A70-800D5B20       .text procWait__9daArrow_cFv */
BOOL daArrow_c::procWait() {
    speedF = 0.0f;
    setKeepMatrix();
    cMtx_copy(mpModel->getBaseTRMtx(), field_0x6b4);
    field_0x6e6 = shape_angle;
    
    if (fopAcM_GetParam(this) == 1) {
        if (!mbSetByZelda) {
            arrowUseMp();
            checkRestMp();
        }
        
        mCurrProcFunc = &daArrow_c::procMove;
        arrowShooting();
    }
    
    return TRUE;
}

/* 800D5B20-800D6BF4       .text procMove__9daArrow_cFv */
BOOL daArrow_c::procMove() {
    speedF = 100.0f;
    current.pos += speed;
    cXyz quarterStepPos = current.pos + speed*0.25f;
    mLinChk.Set(&old.pos, &quarterStepPos, this);
    field_0x6e6 = shape_angle;
    
    cMtx_copy(mpModel->getBaseTRMtx(), field_0x6b4);
    mBlurAngle.z += 0x889;
    
    s32 hitType = 0; // No hit
    if (mAtCps.ChkAtHit()) {
        cXyz temp12;
        cXyz hitPos;
        csXyz temp11;
        if (mArrowType == TYPE_LIGHT && !mbLinkReflect && mAtCps.ChkAtShieldHit() && fpcM_GetName(mAtCps.GetAtHitAc()) == PROC_PLAYER) {
            mAtCps.GetAtHitAc();
            hitPos = *mAtCps.GetAtHitPosP();
            hitType = -1; // Reflected hit
            mbLinkReflect = true;
            ShieldReflect();
        } else {
            fopAc_ac_c* hitActor;
            BOOL hitWasBlocked;
            s32 bHitActor = chkAtHitNormal();
            if (bHitActor) {
                hitActor = getAtHitNormalActor();
                hitPos = *getAtHitPosBuffP();
                hitWasBlocked = FALSE;
            } else {
                hitActor = mAtCps.GetAtHitAc();
                hitPos = *mAtCps.GetAtHitPosP();
                hitWasBlocked = mAtCps.ChkAtShieldHit();
            }
            
            if (hitActor) {
                JntHit_c* jntHit = fopAcM_GetJntHit(hitActor);
                if (mArrowType == TYPE_LIGHT) {
                    if (fpcM_GetName(mAtCps.GetAtHitAc()) == PROC_BGN ||
                        fpcM_GetName(mAtCps.GetAtHitAc()) == PROC_BGN2 ||
                        fpcM_GetName(mAtCps.GetAtHitAc()) == PROC_BGN3) {
                        // Hit Puppet Ganon.
                        if (hitWasBlocked) {
                            field_0x6a8 = hitPos;
                            current.pos = hitPos - (speed * 0.25f);
                            
                            if (!field_0x6e4) {
                                dKy_arrowcol_chg_on(&current.pos, 2);
                            }
                            
                            mCurrProcFunc = &daArrow_c::procStop_BG;
                            fopAcM_OnStatus(this, fopAcStts_UNK4000_e);
                            fopAcM_SetParam(this, 2);
                            field_0x604 = 0x28;
                            
                            dComIfG_Bgsp()->GetTriPla(mLinChk);
                            
                            csXyz temp10;
                            temp10.x = cM_atan2s(speed.y, speed.absXZ());
                            temp10.y = cM_atan2s(speed.x, speed.z);
                            temp10.z = 0;
                            mDoMtx_stack_c::transS(current.pos);
                            mDoMtx_stack_c::ZXYrotM(temp10.x, temp10.y, 0);
                            cMtx_copy(mDoMtx_stack_c::get(), field_0x6b4);
                            
                            dComIfGp_particle_setP1(dPa_name::ID_COMMON_02A1, &field_0x6a8, &temp10);
                            fopAcM_seStartCurrent(this, JA_SE_OBJ_LIGHT_ARW_EFF, 0);
                            fopAcM_delete(this);
                        } else {
                            hitType = 0; // No hit (pass through)
                        }
                    } else {
                        hitType = 0; // No hit (pass through)
                    }
                } else if (hitWasBlocked) {
                    hitType = 1; // Blocked hit
                } else if (jntHit) {
                    mHitJointIndex = jntHit->searchJntHitPosAngleOffset(&hitPos, &shape_angle, &temp12, &temp11);
                    if (mHitJointIndex >= 0) {
                        field_0x6e6 = temp11;
                        field_0x618 = temp12;
                        hitType = 2; // Hit a joint
                    } else if (mHitJointIndex == -3) {
                        fopAcM_delete(this);
                        return TRUE;
                    }
                }
            }
        }
    }
    
    if (hitType > 0) {
        field_0x604 = 0x28;
        fopAcM_OnStatus(this, fopAcStts_UNK4000_e);
        
        if (mBlurFollowCb.getEmitter()) {
            mBlurFollowCb.remove();
        }
        
        if (hitType == 1) { // Blocked hit
            fopAcM_SetParam(this, 3);
            mCurrProcFunc = &daArrow_c::procReturn;
            speed *= -0.1f;
            speed.y += speed.absXZ();
            current.pos = old.pos;
            field_0x69c = 0x2C00;
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, 0);
            mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
            fopAcM_seStartCurrent(this, JA_SE_LK_ARROW_REBOUND, 0x20);
        } else if (hitType == 2) { // Hit a joint
            fopAcM_SetParam(this, 2);
            mCurrProcFunc = &daArrow_c::procStop_Actor;
            
            if (mArrowType == TYPE_FIRE) {
                fopAcM_seStartCurrent(this, JA_SE_OBJ_FIRE_ARW_EFF, 0);
                field_0x698 = false;
            } else if (mArrowType == TYPE_ICE) {
                fopAcM_seStartCurrent(this, JA_SE_OBJ_ICE_ARW_EFF, 0);
            } else if (mArrowType == TYPE_LIGHT) {
                fopAcM_seStartCurrent(this, JA_SE_OBJ_LIGHT_ARW_EFF, 0);
                field_0x698 = false;
            }
            
            setStopActorMatrix();
        }
    } else if (dComIfG_Bgsp()->LineCross(&mLinChk)) {
        cXyz* linEnd = mLinChk.GetLinP()->GetEndP();
        field_0x6a8 = *linEnd;
        current.pos = *linEnd - (speed * 0.25f);
        
        if (!check_water_in()) {
            s32 temp8;
            switch (mArrowType) {
            case TYPE_FIRE:
                temp8 = 0;
                break;
            case TYPE_ICE:
                temp8 = 1;
                break;
            case TYPE_LIGHT:
                temp8 = 2;
                break;
            default:
                temp8 = -1;
            }
            
            if (temp8 >= 0 && !field_0x6e4) {
                dKy_arrowcol_chg_on(&current.pos, temp8);
            }
            
            mCurrProcFunc = &daArrow_c::procStop_BG;
            fopAcM_OnStatus(this, fopAcStts_UNK4000_e);
            fopAcM_SetParam(this, 2);
            field_0x604 = 0x28;
            cM3dGPla* triPla = dComIfG_Bgsp()->GetTriPla(mLinChk);
            
            csXyz temp10;
            temp10.x = cM_atan2s(-triPla->mNormal.y, -triPla->mNormal.absXZ());
            temp10.y = cM_atan2s(-triPla->mNormal.x, -triPla->mNormal.z);
            temp10.z = 0;
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::ZXYrotM(temp10.x, temp10.y, 0);
            cMtx_copy(mDoMtx_stack_c::get(), field_0x6b4);
            
            if (mArrowType == TYPE_FIRE) {
                dComIfGp_particle_setP1(dPa_name::ID_COMMON_029A, &field_0x6a8, &temp10);
                dComIfGp_particle_setP1(dPa_name::ID_COMMON_029B, &field_0x6a8, &temp10);
                fopAcM_seStartCurrent(this, JA_SE_OBJ_FIRE_ARW_EFF, 0);
                field_0x698 = false;
            } else if (mArrowType == TYPE_ICE) {
                if (dComIfG_Bgsp()->ChkGrpInf(mLinChk, 0x200)) {
                    fopAcM_create(PROC_Obj_Magmarock, NULL, &field_0x6a8, current.roomNo);
                } else {
                    dComIfGp_particle_setP1(dPa_name::ID_COMMON_029E, &field_0x6a8, &temp10);
                    
                    fopAcM_createChild(
                        PROC_ARROW_ICEEFF, fopAcM_GetID(this),
                        mArrowType, &field_0x6a8,
                        current.roomNo, &field_0x6e6
                    );
                    
                    fopAcM_seStartCurrent(this, JA_SE_OBJ_ICE_ARW_EFF, 0);
                }
            } else if (mArrowType == TYPE_LIGHT) {
                dComIfGp_particle_setP1(dPa_name::ID_COMMON_02A1, &field_0x6a8, &temp10);
                fopAcM_seStartCurrent(this, JA_SE_OBJ_LIGHT_ARW_EFF, 0);
                field_0x698 = false;
            }
            
            s32 attribCode = dComIfG_Bgsp()->GetAttributeCode(mLinChk);
            s32 mtrlSndId = dComIfG_Bgsp()->GetMtrlSndId(mLinChk);
            
            if (mArrowType == TYPE_NORMAL && (
                attribCode == dBgS_Attr_STONE_e ||
                attribCode == dBgS_Attr_METAL_e ||
                attribCode == dBgS_Attr_ICE_e ||
                attribCode == dBgS_Attr_DAMAGE_e ||
                attribCode == dBgS_Attr_FREEZE_e
            )) {
                mCurrProcFunc = &daArrow_c::procReturn;
                fopAcM_SetParam(this, 3);
                speed *= -0.1f;
                speed.y += speed.absXZ();
                current.pos = old.pos;
                field_0x69c = 0x2C00;
                csXyz temp9;
                cM3d_CalcVecZAngle(*triPla->GetNP(), &temp9);
                
                dComIfGp_particle_setP1(dPa_name::ID_COMMON_PURPLE_HIT, &field_0x6a8, &temp9);
                fopAcM_seStartCurrent(this, JA_SE_LK_ARROW_REBOUND, mtrlSndId);
            } else {
                fopAcM_seStartCurrent(this, JA_SE_LK_ARROW_HIT, mtrlSndId);
            }
        }
    } else if (check_water_in()) {
        // Do nothing.
        // There was probably some code here that got commented out.
    }
    
    if (mCurrProcFunc == &daArrow_c::procWater) {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, 0);
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        if ((current.pos - field_0x6a8).abs() > 25000.0f) {
            speed.y -= 2.0f;
            if (speed.y < -100.0f) {
                speed.y = -100.0f;
            }
            if (field_0x6a8.y > current.pos.y) {
                fopAcM_delete(this);
                return TRUE;
            }
        } else if ((current.pos - field_0x6a8).abs() > 20000.0f) {
            field_0x6e4 = true;
        }
        
        createBlur();
        
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, 0);
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        
        cXyz end = current.pos + speed*1.25f;
        mAtCps.SetStartEnd(current.pos, end);
        mAtCps.SetR(5.0f);
        mAtCps.CalcAtVec();
        
        dComIfG_Ccsp()->Set(&mAtCps);
        dComIfG_Ccsp_SetMass(&mAtCps, 1);
    }
    
    return TRUE;
}

/* 800D6BF4-800D6E70       .text procReturn__9daArrow_cFv */
BOOL daArrow_c::procReturn() {
    speedF = 0.0f;
    speed.y -= 2.0f;
    current.pos += speed;
    shape_angle.x += field_0x69c;
    cXyz quarterStepPos = current.pos + speed*0.25f;
    field_0x699 = daPy_lk_c::setItemWaterEffect(this, field_0x699, 1);
    mLinChk.Set(&old.pos, &quarterStepPos, this);
    setBlur();
    
    if (dComIfG_Bgsp()->LineCross(&mLinChk)) {
        cM3dGPla* triPla = dComIfG_Bgsp()->GetTriPla(mLinChk);
        f32 temp2 = speed.abs();
        cXyz temp1;
        C_VECReflect(&speed, triPla->GetNP(), &temp1);
        speed.x = temp1.x*temp2*0.5f;
        speed.y = temp1.y*temp2*0.5f;
        speed.z = temp1.z*temp2*0.5f;
        
        s32 temp3 = -field_0x69c;
        field_0x69c = (temp3 / 2);
        triPla = dComIfG_Bgsp()->GetTriPla(mLinChk);
        if (cBgW_CheckBGround(triPla->GetNP()->y)) {
            field_0x69a = true;
        }
    } else if (field_0x69a && speed.y < 0.0f) {
        fopAcM_delete(this);
    }
    
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, 0);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    
    return TRUE;
}

/* 800D6E70-800D71F8       .text procStop_BG__9daArrow_cFv */
BOOL daArrow_c::procStop_BG() {
    speedF = 0.0f;
    
    if (!dComIfG_Bgsp()->ChkPolySafe(mLinChk)) {
        fopAcM_delete(this);
        return TRUE;
    }
    
    BOOL temp2 = FALSE;
    
    if (field_0x604 > 0) {
        field_0x604--;
        f32 temp4 = (field_0x604*(1/40.0f)) * 1024.0f * (field_0x604*(1/40.0f)) * cM_ssin(field_0x604*0x52FB);
        shape_angle.x = field_0x6e6.x + temp4;
        shape_angle.y = field_0x6e6.y;
        temp2 = TRUE;
    } else if (field_0x600) {
        fopAcM_delete(this);
        return TRUE;
    }
    
    setBlur();
    
    if (dComIfG_Bgsp()->ChkMoveBG(mLinChk)) {
        dComIfG_Bgsp()->MoveBgTransPos(mLinChk, true, &field_0x6a8, &current.angle, &shape_angle);
        temp2 = TRUE;
    }
    
    if (temp2) {
        f32 xCos = cM_scos(shape_angle.x);
        current.pos.x = field_0x6a8.x - cM_ssin(shape_angle.y) * 50.0f * xCos;
        current.pos.y = field_0x6a8.y + cM_ssin(shape_angle.x) * 50.0f;
        current.pos.z = field_0x6a8.z - cM_scos(shape_angle.y) * 50.0f * xCos;
        
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, 0);
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
    
    if (mArrowType == TYPE_NORMAL) {
        mCoSph.SetC(current.pos);
        dComIfG_Ccsp()->Set(&mCoSph);
        
        if (field_0x6a0 == 0) {
            field_0x600 = true;
            field_0x698 = false;
        } else {
            field_0x6a0--;
            
            if (field_0x6a0 < 60) {
                if (field_0x6a0 % 2 == 0) {
                    field_0x698 = false;
                } else {
                    field_0x698 = true;
                }
            } else {
                field_0x698 = true;
            }
        }
        
        if (mCoSph.ChkCoHit()) {
            dComIfGp_setItemArrowNumCount(1);
            fopAcM_createItemForSimpleDemo(&current.pos, dItem_ARROW_10_e, -1, NULL, NULL, 0.0f, 0.0f);
            mDoAud_seStart(JA_SE_CONSUMP_ITEM_GET);
            fopAcM_delete(this);
            return TRUE;
        }
    }
    
    if (mbSetByZelda) {
        field_0x600 = true;
    }
    
    return TRUE;
}

/* 800D71F8-800D727C       .text procStop_Actor__9daArrow_cFv */
BOOL daArrow_c::procStop_Actor() {
    speedF = 0.0f;
    if (field_0x600) {
        fopAcM_delete(this);
        return TRUE;
    }
    
    setBlur();
    
    if (fopAcM_SearchByID(mHitActorProcID)) {
        setStopActorMatrix();
    } else {
        field_0x600 = true;
    }
    
    return TRUE;
}

/* 800D727C-800D72BC       .text procWater__9daArrow_cFv */
BOOL daArrow_c::procWater() {
    if (mInWaterTimer <= 0) {
        fopAcM_delete(this);
        return TRUE;
    }
    
    mInWaterTimer--;
    return TRUE;
}

/* 800D72BC-800D72EC       .text checkRestMp__9daArrow_cFv */
void daArrow_c::checkRestMp() {
    static const s16 use_mp[4] = {
        0, // Normal Arrows
        1, // Fire Arrows
        1, // Ice Arrows
        2, // Light Arrows
    };
    
    u8 currMagic = dComIfGs_getMagic();
    if (currMagic < use_mp[m_keep_type]) {
        m_keep_type = TYPE_NORMAL;
    }
}

/* 800D72EC-800D7320       .text setTypeByPlayer__9daArrow_cFv */
void daArrow_c::setTypeByPlayer() {
    checkRestMp();
    mArrowType = m_keep_type;
}

/* 800D7320-800D74FC       .text createInit__9daArrow_cFv */
BOOL daArrow_c::createInit() {
    if (mArrowType == TYPE_LIGHT) {
        mpBtk = daPy_getPlayerLinkActorClass()->getLightArrowBtk();
    } else {
        mpBtk = daPy_getPlayerLinkActorClass()->getIceArrowBtk();
    }
    
    mCurrProcFunc = &daArrow_c::procWait;
    
    setKeepMatrix();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_SetMin(this, -6.0f, -6.0f, 0.0f);
    fopAcM_SetMax(this, 6.0f, 6.0f, 65.0f);
    
    mStts.Init(10, 0xFF, this);
    mAtCps.Set(m_at_cps_src);
    mAtCps.SetStts(&mStts);
    mAtCps.SetAtHitCallback(&atHit_CB);
    mCoSph.Set(m_co_sph_src);
    mCoSph.SetStts(&mStts);
    
    field_0x602 = -1;
    
    if (mArrowType != TYPE_LIGHT) {
        J3DModelData* modelData = mpModel->getModelData();
        modelData->getJointNodePointer(4)->getMesh()->getShape()->hide();
        modelData->getJointNodePointer(2)->getMesh()->getShape()->hide();
        modelData->getJointNodePointer(3)->getMesh()->getShape()->hide();
        modelData->getJointNodePointer(3)->getMesh()->getNext()->getShape()->hide();
    }
    setDrawShapeMaterial();
    
    field_0x698 = true;
    field_0x699 = 0;
    field_0x69a = false;
    field_0x69c = 0;
    field_0x6a0 = 300;
    mInWaterTimer = 0;
    field_0x6e4 = false;
    mbHasLightEff = false;
    mbLinkReflect = false;
    field_0x604 = 0;
    
    return TRUE;
}

/* 800D74FC-800D7820       .text _execute__9daArrow_cFv */
BOOL daArrow_c::_execute() {
    if (mbSetByZelda) {
        if (!mbLinkReflect) {
            if (daPy_getPlayerLinkActorClass()->checkPlayerGuard()) {
                mAtCps.SetAtSpl(dCcG_At_Spl_UNK0);
                mAtCps.SetAtType(AT_TYPE_NORMAL_ARROW);
            } else {
                mAtCps.SetAtSpl(dCcG_At_Spl_UNKB);
                mAtCps.SetAtType(AT_TYPE_LIGHT_ARROW);
            }
        } else {
            mAtCps.SetAtSpl(dCcG_At_Spl_UNK0);
            mAtCps.SetAtType(AT_TYPE_NORMAL_ARROW);
        }
    }
    
    if (mSparkleTimer != 0) {
        mSparkleTimer--;
        s8 temp4 = 0;
        daPy_py_c* player = daPy_getPlayerActorClass();
        
        cXyz offset;
        offset.x = 0.0f;
        offset.y = 45.0f + REG0_F(8);
        offset.z = 30.0f + REG0_F(9);
        mDoMtx_YrotS(*calc_mtx, player->shape_angle.y);
        cXyz offsetOut;
        MtxPosition(&offset, &offsetOut);
        current.pos = player->current.pos + offsetOut;
        
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, 0);
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
        
        if (mpSparkleEmitter) {
            if (mSparkleTimer != 0) {
                f32 scaleMag = mSparkleTimer*2.0f;
                if (scaleMag > 7.0f) {
                    scaleMag = 7.0f;
                }
                mpSparkleEmitter->setGlobalTranslation(current.pos.x, current.pos.y, current.pos.z);
                JGeometry::TVec3<f32> scaleVec;
                scaleVec.x = scaleMag;
                scaleVec.y = scaleMag;
                scaleVec.z = scaleMag;
                mpSparkleEmitter->setGlobalScale(scaleVec);
            } else {
                mpSparkleEmitter->becomeInvalidEmitter();
                mpSparkleEmitter = NULL;
                temp4 = 1;
            }
        }
        if (temp4 == 0) {
            return TRUE;
        }
    }
    
    if (field_0x602 == m_count) {
        field_0x600 = true;
    }
    
    if (mBtkFrame > 0.0f) {
        mBtkFrame += 1.0f;
        if (mBtkFrame >= mpBtk->getFrameMax()) {
            mBtkFrame = 0.0f;
        }
    } else {
        if (cM_rnd() < 0.02f) {
            mBtkFrame += 1.0f;
        }
    }
    
    setLightEffect();
    
    if (mCurrProcFunc) {
        (this->*mCurrProcFunc)();
    }
    
    attention_info.position = current.pos;
    eyePos = current.pos;
    setRoomInfo();
    
    return TRUE;
}

/* 800D7820-800D7960       .text _draw__9daArrow_cFv */
BOOL daArrow_c::_draw() {
    if (!field_0x698) {
        return TRUE;
    }
    
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    
    if (mArrowType != TYPE_LIGHT) {
        mpTipMat->getShape()->show();
        J3DMaterial* nextMat = mpTipMat->getNext();
        if (nextMat) {
            nextMat->getShape()->show();
        }
    }
    
    mpBtk->setFrame(mBtkFrame);
    dComIfGd_setListP1();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    
    if (mArrowType != TYPE_LIGHT) {
        mpTipMat->getShape()->hide();
        J3DMaterial* nextMat = mpTipMat->getNext();
        if (nextMat) {
            nextMat->getShape()->hide();
        }
    }
    
    return TRUE;
}

/* 800D7960-800D7A38       .text _create__9daArrow_cFv */
cPhs_State daArrow_c::_create() {
    fopAcM_SetupActor(this, daArrow_c);
    
    checkCreater();
    
    if (mbSetByZelda) {
        mArrowType = TYPE_LIGHT;
    } else {
        setTypeByPlayer();
    }
    
    if (dComIfGp_getMiniGameType() == 8) {
        mArrowType = TYPE_NORMAL;
    }
    
    const static u32 heap_size[] = {
        0x1300,
        0x1300,
        0x1300,
        0x820,
    };
    if (!fopAcM_entrySolidHeap(this, createHeap_CB, heap_size[mArrowType])) {
        return cPhs_ERROR_e;
    }
    
    if (createInit()) {
        return cPhs_COMPLEATE_e;
    } else {
        return cPhs_ERROR_e;
    }
}

/* 800D81D0-800D8200       .text _delete__9daArrow_cFv */
BOOL daArrow_c::_delete() {
    mBlurFollowCb.remove();
    return TRUE;
}

/* 800D8200-800D8220       .text daArrowCreate__FPv */
static cPhs_State daArrowCreate(void* i_this) {
    return static_cast<daArrow_c*>(i_this)->_create();
}

/* 800D8220-800D8240       .text daArrowDelete__FPv */
static BOOL daArrowDelete(void* i_this) {
    return static_cast<daArrow_c*>(i_this)->_delete();
}

/* 800D8240-800D8260       .text daArrowExecute__FPv */
static BOOL daArrowExecute(void* i_this) {
    return static_cast<daArrow_c*>(i_this)->_execute();
}

/* 800D8260-800D8280       .text daArrowDraw__FPv */
static BOOL daArrowDraw(void* i_this) {
    return static_cast<daArrow_c*>(i_this)->_draw();
}

/* 800D8280-800D8288       .text daArrowIsDelete__FPv */
static BOOL daArrowIsDelete(void* i_this) {
    return TRUE;
}

static actor_method_class daArrowMethodTable = {
    (process_method_func)daArrowCreate,
    (process_method_func)daArrowDelete,
    (process_method_func)daArrowExecute,
    (process_method_func)daArrowIsDelete,
    (process_method_func)daArrowDraw,
};

actor_process_profile_definition g_profile_ARROW = {
    /* LayerID      */ fpcLy_CURRENT_e,
    /* ListID       */ 0x0009,
    /* ListPrio     */ fpcPi_CURRENT_e,
    /* ProcName     */ PROC_ARROW,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daArrow_c),
    /* SizeOther    */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Priority     */ PRIO_ARROW,
    /* Actor SubMtd */ &daArrowMethodTable,
    /* Status       */ fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* CullType     */ fopAc_CULLBOX_CUSTOM_e,
};
