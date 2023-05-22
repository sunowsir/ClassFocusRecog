/*
	* File     : ocsfs_define.h
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月20日 星期六 19时31分00秒
*/

#ifndef _OCSFS_DEFINE_H
#define _OCSFS_DEFINE_H

#define OCSFS_CLIENT_TYPE_USER 0
#define OCSFS_CLIENT_TYPE_MGR  1

/* 暂时规定客户端ID长度为10 */
#define OCSFS_CLIENT_ID_LEN    10

/* 规定，协议头：client_id(64), src_client_id(64), step(int), data len(int) */
#define OCSFS_PROTO_HEAD_LEN   (OCSFS_CLIENT_ID_LEN * 2 + sizeof(int) * 2)

#define OCSFS_SERVER_CTL_PORT 7890

#endif
