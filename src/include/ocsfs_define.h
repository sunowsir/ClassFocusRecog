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

/* 规定，协议头：client_id(64), src_client_id(64), step(1), data len(int) */
#define OCSFS_PROTO_HEAD_LEN   (OCSFS_CLIENT_ID_LEN * 2 + 1 + sizeof(int))

/* 服务器端口 */
#define OCSFS_SERVER_CTL_PORT 7890

#define OCSFS_FACE_BASE       (100)
#define OCSFS_FACE_STEP       (50)
/* 平静 */
#define OCSFS_face_COMM       (OCSFS_FACE_BASE + 0 * OCSFS_FACE_STEP)
/* 高兴 */
#define OCSFS_face_HAPPY      (OCSFS_FACE_BASE + 1 * OCSFS_FACE_STEP)
/* 厌恶 */
#define OCSFS_face_HADE       (OCSFS_FACE_BASE + 2 * OCSFS_FACE_STEP)
/* 生气 */
#define OCSFS_face_ANGRY      (OCSFS_FACE_BASE + 3 * OCSFS_FACE_STEP)
/* 闭眼 */
#define OCSFS_face_CLOSEEYES  (OCSFS_FACE_BASE + 4 * OCSFS_FACE_STEP)
/* 哈欠 */
#define OCSFS_face_YAWN       (OCSFS_FACE_BASE + 5 * OCSFS_FACE_STEP)

/* 表情编号最大值 */
#define OCSFS_FACE_MAX        (OCSFS_FACE_BASE + 5 * OCSFS_FACE_STEP)

/* 有多少中表情 */
#define OCSFS_FACE_TYPE_NUM   ((OCSFS_FACE_MAX - OCSFS_FACE_BASE) / OCSFS_FACE_STEP)

/* 用来训练的各类表情图片各有多少，
 * 注意：要保证每种表情的图片都有如下数量
 * */
#define OCSFS_TRAIN_IMGS_NUM  (50)


#endif
