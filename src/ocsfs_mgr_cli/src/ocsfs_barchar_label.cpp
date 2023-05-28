/*
	* File     : ocsfs_barchar_label.cpp
	* Author   : sunowsir
	* Mail     : sunowsir@163.com
	* Github   : github.com/sunowsir
	* Creation : 2023年05月28日 星期日 19时43分09秒
*/

#include "ocsfs_barchar_label.h"

OCSFS_BarChartLabel::OCSFS_BarChartLabel(const QString &client_id, QWidget *parent) : 
    QLabel(parent) {
    this->client_id = new QString(client_id);
}

bool OCSFS_BarChartLabel::get_client_id(QString &client_id) {
    client_id = *this->client_id;
    return true;
}

bool OCSFS_BarChartLabel::set_client_id(const QString &client_id) {
    *this->client_id = client_id;
    return true;
}

bool OCSFS_BarChartLabel::event(QEvent *event) {
    switch (event->type()) {
        /* 悬停 */
        case QEvent::Enter: {
            this->signal_mouse_enter(*this->client_id);
        } break;

        /* 离开 */
        case QEvent::Leave: {
            this->signal_mouse_leave(*this->client_id);
        } break;

        /* 点击 */
        case QEvent::MouseButtonPress: {
            this->signal_mouse_press(*this->client_id);
        } break;

        /* 松开 */
        case QEvent::MouseButtonRelease: {
            this->signal_mouse_release(*this->client_id);
        } break;
        default:
            break;
    }
    return QLabel::event(event);
}
