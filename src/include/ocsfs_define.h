/*
	* File     : ocsfs_define.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月20日 星期六 19时31分00秒
*/

#ifndef _OCSFS_DEFINE_H
#define _OCSFS_DEFINE_H

#include <QMap>
#include <QString>
#include <QStringList>

#define OCSFS_CLIENT_TYPE_USER 0
#define OCSFS_CLIENT_TYPE_MGR  1

/* 暂时规定客户端ID长度为10 */
#define OCSFS_CLIENT_ID_LEN    10

#define OCSFS_ZERO_ID          (QString(OCSFS_CLIENT_ID_LEN, '0')) 

#define OCSFS_SERVER_ID        (OCSFS_ZERO_ID) 

/* 规定，协议头：src_client_id(10), dst_client_id(10), step(1), data len(int) */
#define OCSFS_PROTO_HEAD_LEN   (OCSFS_CLIENT_ID_LEN * 2 + 1 + sizeof(int))

/* 服务器端口 */
#define OCSFS_SERVER_CTL_PORT  7890

/* 签到 */
#define OCSFS_CheckIn_SYN      "checkin,request"
#define OCSFS_CheckIn_ACK      "checkin,response"

/* 点名 */
#define OCSFS_RollCall_SYN     "rollcall,request"
#define OCSFS_RollCall_ACK     "rollcall,response"

/* 警告信息 */
#define OCSFS_To_User_Warning_SYN  "warning,request"
#define OCSFS_To_User_Warning_ACK  "warning,response"

/* 有学生准备好，服务器向教师端报告 */
#define OCSFS_User_Ready_SYN  "user_ready,request"

/* 侧脸编号 */
#define OCSFS_head_PROFILE     (90)

#define OCSFS_FACE_BASE        (100)
#define OCSFS_FACE_STEP        (50)
/* 平静 */
#define OCSFS_face_COMM        (OCSFS_FACE_BASE + 0 * OCSFS_FACE_STEP)
/* 高兴 */
#define OCSFS_face_HAPPY       (OCSFS_FACE_BASE + 1 * OCSFS_FACE_STEP)
/* 厌恶 */
#define OCSFS_face_HADE        (OCSFS_FACE_BASE + 2 * OCSFS_FACE_STEP)
/* 生气 */
#define OCSFS_face_ANGRY       (OCSFS_FACE_BASE + 3 * OCSFS_FACE_STEP)
/* 闭眼 */
#define OCSFS_face_CLOSEEYES   (OCSFS_FACE_BASE + 4 * OCSFS_FACE_STEP)
/* 哈欠 */
#define OCSFS_face_YAWN        (OCSFS_FACE_BASE + 5 * OCSFS_FACE_STEP)
/* 惊讶 */
#define OCSFS_face_SURPRISE    (OCSFS_FACE_BASE + 6 * OCSFS_FACE_STEP)

/* 表情编号最大值 */
#define OCSFS_FACE_MAX         (OCSFS_FACE_BASE + 6 * OCSFS_FACE_STEP)

/* 有多少中表情 */
#define OCSFS_FACE_TYPE_NUM    ((OCSFS_FACE_MAX - OCSFS_FACE_BASE) / OCSFS_FACE_STEP)

/* 积极状态 */
#define OCSFS_STATUS_TYPE_active   (10)
/* 中性状态 */
#define OCSFS_STATUS_TYPE_neutral  (11)
/* 消极状态 */
#define OCSFS_STATUS_TYPE_negative (12)

/* 持续接收到20个闭眼状态，认定为消极 */
#define OCSFS_CLOSEEYES_keep_count (20)
/* 持续接收到10个哈欠状态，认定为消极 */
#define OCSFS_YAWN_keep_count      (10)


/* 用来训练的各类表情图片各有多少，
 * 注意：要保证每种表情的图片都有如下数量
 * */
#define OCSFS_TRAIN_IMGS_NUM   (300)


#define OCSFS_MGR_STU_INFO_DIALOG_HIGH (5)

#define OCSFS_PROGRESS_Width   (300)
#define OCSFS_PROGRESS_Height  (25)

#endif
