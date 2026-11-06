#include "common.h"

const char *err_msg(ErrCode e)
{
    switch (e) {
    case ERR_OK: return "Thanh cong";
    case ERR_IO: return "Loi doc/ghi tep";
    case ERR_NOT_FOUND: return "Tep khong ton tai";
    case ERR_NO_READ: return "Khong co quyen doc tep";
    case ERR_IS_DIR: return "Duong dan la thu muc, khong phai tep";
    case ERR_BAD_MAGIC: return "Khong phai tep do Cipher Tool tao ra";
    case ERR_BAD_VERSION: return "Phien ban tep khong duoc ho tro";
    case ERR_BAD_CRC: return "Sai khoa hoac tep hong";
    case ERR_BAD_ARG: return "Tham so khong hop le";
    case ERR_EMPTY_KEY: return "Khoa khong duoc de trong";
    case ERR_SAME_FILE: return "Tep vao va tep ra trung nhau";
    case ERR_NOMEM: return "Khong du bo nho";
    case ERR_EOF: return "Het du lieu nhap";
    }
    return "Loi khong xac dinh";
}
