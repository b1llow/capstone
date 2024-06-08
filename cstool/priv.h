#ifndef CAPSTONE_PRIV_H
#define CAPSTONE_PRIV_H

#define printf(...) stream_printf(steam, __VA_ARGS__)
#define check_type(_k) if ((op->type & (_k)) == (_k))
#define check_access(_k) if ((op->access & (_k)) == (_k))

#endif //CAPSTONE_PRIV_H
